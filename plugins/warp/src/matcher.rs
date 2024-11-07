use binaryninja::architecture::Architecture as BNArchitecture;
use binaryninja::binaryview::{BinaryView, BinaryViewExt};
use binaryninja::function::Function as BNFunction;
use binaryninja::platform::Platform;
use binaryninja::rc::Guard;
use binaryninja::rc::Ref as BNRef;
use dashmap::DashMap;
use std::cmp::Ordering;
use std::collections::{HashMap, HashSet};
use std::hash::{DefaultHasher, Hasher};
use std::path::PathBuf;
use std::sync::OnceLock;
use walkdir::{DirEntry, WalkDir};
use warp::r#type::class::TypeClass;
use warp::r#type::guid::TypeGUID;
use warp::r#type::Type;
use warp::signature::function::{Function, FunctionGUID};
use warp::signature::Data;

use crate::cache::{cached_call_site_constraints, cached_function_match, try_cached_function_guid};
use crate::convert::to_bn_type;
use crate::plugin::on_matched_function;

pub static PLAT_MATCHER_CACHE: OnceLock<DashMap<PlatformID, Matcher>> = OnceLock::new();

pub fn cached_function_matcher(function: &BNFunction) {
    let platform = function.platform();
    let platform_id = PlatformID::from(platform.as_ref());
    let matcher_cache = PLAT_MATCHER_CACHE.get_or_init(Default::default);
    match matcher_cache.get(&platform_id) {
        Some(matcher) => matcher.match_function(function),
        None => {
            let matcher = Matcher::from_platform(platform);
            matcher.match_function(function);
            matcher_cache.insert(platform_id, matcher);
        }
    }
}

// TODO: Maybe just clear individual platforms? This works well enough either way.
pub fn invalidate_function_matcher_cache() {
    let matcher_cache = PLAT_MATCHER_CACHE.get_or_init(Default::default);
    matcher_cache.clear();
}

#[derive(Debug, Default, Clone)]
pub struct Matcher {
    // TODO: Storing the settings here means that they are effectively global.
    // TODO: If we want scoped or view settings they must be moved out.
    pub settings: MatcherSettings,
    pub functions: DashMap<FunctionGUID, Vec<Function>>,
    pub types: DashMap<TypeGUID, Type>,
    pub named_types: DashMap<String, Type>,
}

impl Matcher {
    /// Create a matcher from the platforms signature subdirectory.
    pub fn from_platform(platform: BNRef<Platform>) -> Self {
        let platform_name = platform.name().to_string();
        // Get core signatures for the given platform
        let install_dir = binaryninja::install_directory().unwrap();
        let core_dir = install_dir.parent().unwrap();
        #[cfg(target_os = "macos")]
        let root_core_sig_dir = core_dir.join("Resources").join("signatures");
        #[cfg(not(target_os = "macos"))]
        let root_core_sig_dir = core_dir.join("signatures");
        let plat_core_sig_dir = root_core_sig_dir.join(&platform_name);
        let mut data = get_data_from_dir(&plat_core_sig_dir);

        // Get user signatures for the given platform
        let user_dir = binaryninja::user_directory().unwrap();
        let root_user_sig_dir = user_dir.join("signatures");
        let plat_user_sig_dir = root_user_sig_dir.join(&platform_name);
        // If the dir has not been created, create it.
        let user_data = get_data_from_dir(&plat_user_sig_dir);

        data.extend(user_data);
        let merged_data = Data::merge(&data.values().cloned().collect::<Vec<_>>());
        log::debug!("Loaded signatures: {:?}", data.keys());
        Matcher::from_data(merged_data)
    }

    pub fn from_data(data: Data) -> Self {
        let functions = data.functions.into_iter().fold(
            DashMap::new(),
            |map: DashMap<FunctionGUID, Vec<_>>, func| {
                map.entry(func.guid).or_default().push(func);
                map
            },
        );
        let types = data
            .types
            .iter()
            .map(|ty| (ty.guid, ty.ty.clone()))
            .collect();
        let named_types = data
            .types
            .into_iter()
            .filter_map(|ty| ty.ty.name.to_owned().map(|name| (name, ty.ty)))
            .collect();

        Self {
            settings: Default::default(),
            functions,
            types,
            named_types,
        }
    }

    pub fn extend_with_matcher(&mut self, matcher: Matcher) {
        self.functions.extend(matcher.functions);
        self.types.extend(matcher.types);
        self.named_types.extend(matcher.named_types);
    }

    pub fn add_type_to_view<A: BNArchitecture>(&self, view: &BinaryView, arch: &A, ty: &Type) {
        fn inner_add_type_to_view<A: BNArchitecture>(
            matcher: &Matcher,
            view: &BinaryView,
            arch: &A,
            visited_refs: &mut HashSet<String>,
            ty: &Type,
        ) {
            let ty_id_str = TypeGUID::from(ty).to_string();
            if view.get_type_by_id(&ty_id_str).is_some() {
                // Type already added.
                return;
            }
            // Type not already added to the view.
            // Verify all nested types are added before adding type.
            match ty.class.as_ref() {
                TypeClass::Pointer(c) => {
                    inner_add_type_to_view(matcher, view, arch, visited_refs, &c.child_type)
                }
                TypeClass::Array(c) => {
                    inner_add_type_to_view(matcher, view, arch, visited_refs, &c.member_type)
                }
                TypeClass::Structure(c) => {
                    for member in &c.members {
                        inner_add_type_to_view(matcher, view, arch, visited_refs, &member.ty)
                    }
                }
                TypeClass::Enumeration(c) => {
                    inner_add_type_to_view(matcher, view, arch, visited_refs, &c.member_type)
                }
                TypeClass::Union(c) => {
                    for member in &c.members {
                        inner_add_type_to_view(matcher, view, arch, visited_refs, &member.ty)
                    }
                }
                TypeClass::Function(c) => {
                    for out_member in &c.out_members {
                        inner_add_type_to_view(matcher, view, arch, visited_refs, &out_member.ty)
                    }
                    for in_member in &c.in_members {
                        inner_add_type_to_view(matcher, view, arch, visited_refs, &in_member.ty)
                    }
                }
                TypeClass::Referrer(c) => {
                    // Check to see if the referrer has been added to the view.
                    let mut resolved = false;
                    if let Some(ref_guid) = c.guid {
                        // NOTE: We do not need to check for cyclic reference here because
                        // NOTE: GUID references are unable to be referenced by themselves.
                        if view.get_type_by_id(ref_guid.to_string()).is_none() {
                            // Add the referrer to the view if it is in the Matcher types
                            if let Some(ref_ty) = matcher.types.get(&ref_guid) {
                                inner_add_type_to_view(matcher, view, arch, visited_refs, &ref_ty);
                                resolved = true;
                            }
                        }
                    }

                    if let Some(ref_name) = &c.name {
                        // Only try and resolve by name if not already visiting.
                        if !resolved
                            && visited_refs.insert(ref_name.to_string())
                            && view.get_type_by_name(ref_name).is_none()
                        {
                            // Add the ref to the view if it is in the Matcher types
                            if let Some(ref_ty) = matcher.named_types.get(ref_name) {
                                inner_add_type_to_view(matcher, view, arch, visited_refs, &ref_ty);
                            }
                            // No longer visiting type.
                            visited_refs.remove(ref_name);
                        }
                    }

                    // All nested types _should_ be added now, we can add this type.
                    let ty_name = ty.name.to_owned().unwrap_or_else(|| ty_id_str.clone());
                    view.define_auto_type_with_id(ty_name, ty_id_str, &to_bn_type(arch, ty));
                }
                _ => {}
            }
        }
        inner_add_type_to_view(self, view, arch, &mut HashSet::new(), ty)
    }

    pub fn match_function(&self, function: &BNFunction) {
        // Call this the first time you matched on the function.
        let on_new_match = |matched: &Function| {
            // We also want to resolve the types here.
            if let TypeClass::Function(c) = matched.ty.class.as_ref() {
                // Recursively go through the function type and resolve referrers
                let view = function.view();
                let arch = function.arch();
                for out_member in &c.out_members {
                    self.add_type_to_view(&view, &arch, &out_member.ty);
                }
                for in_member in &c.in_members {
                    self.add_type_to_view(&view, &arch, &in_member.ty);
                }
            }
        };

        if let Some(matched_function) = cached_function_match(function, || {
            // We have yet to match on this function.
            // TODO: Expand this check to be less broad.
            let function_delta = function.highest_address() - function.lowest_address();
            let is_function_trivial = { function_delta < self.settings.trivial_function_len };
            let warp_func_guid = try_cached_function_guid(function)?;
            match self.functions.get(&warp_func_guid) {
                Some(matched) if matched.len() == 1 && !is_function_trivial => {
                    on_new_match(&matched[0]);
                    Some(matched[0].to_owned())
                }
                Some(matched) => {
                    let matched_on = self.match_function_from_constraints(function, &matched)?;
                    on_new_match(matched_on);
                    Some(matched_on.to_owned())
                }
                None => None,
            }
        }) {
            on_matched_function(function, &matched_function);
        }
    }

    pub fn match_function_from_constraints<'a>(
        &self,
        function: &BNFunction,
        matched_functions: &'a [Function],
    ) -> Option<&'a Function> {
        // TODO: To prevent invoking adjacent constraint function analysis, we must call call_site constraints specifically.
        let call_sites = cached_call_site_constraints(function);

        // NOTE: We are only matching with call_sites for now, as adjacency requires we run after all analysis has completed.
        if call_sites.is_empty() {
            return None;
        }

        // Check call site guids
        let mut highest_guid_count = 0;
        let mut matched_guid_func = None;
        let call_site_guids = call_sites
            .iter()
            .filter_map(|c| c.guid)
            .collect::<HashSet<_>>();
        for matched in matched_functions {
            let matched_call_site_guids = matched
                .constraints
                .call_sites
                .iter()
                .filter_map(|c| c.guid)
                .collect::<HashSet<_>>();
            let common_guid_count = call_site_guids
                .intersection(&matched_call_site_guids)
                .count();
            match common_guid_count.cmp(&highest_guid_count) {
                Ordering::Equal => {
                    // Multiple matches with same count, don't match on ONE of them.
                    matched_guid_func = None;
                }
                Ordering::Greater => {
                    highest_guid_count = common_guid_count;
                    matched_guid_func = Some(matched);
                }
                Ordering::Less => {}
            }
        }

        // Check call site symbol names
        let mut highest_symbol_count = 0;
        let mut matched_symbol_func = None;
        let call_site_symbol_names = call_sites
            .into_iter()
            .filter_map(|c| Some(c.symbol?.name))
            .collect::<HashSet<_>>();
        for matched in matched_functions {
            let matched_call_site_symbol_names = matched
                .constraints
                .call_sites
                .iter()
                .filter_map(|c| Some(c.symbol.to_owned()?.name))
                .collect::<HashSet<_>>();
            let common_symbol_count = call_site_symbol_names
                .intersection(&matched_call_site_symbol_names)
                .count();
            match common_symbol_count.cmp(&highest_symbol_count) {
                Ordering::Equal => {
                    // Multiple matches with same count, don't match on ONE of them.
                    matched_symbol_func = None;
                }
                Ordering::Greater => {
                    highest_symbol_count = common_symbol_count;
                    matched_symbol_func = Some(matched);
                }
                Ordering::Less => {}
            }
        }

        match highest_guid_count.cmp(&highest_symbol_count) {
            Ordering::Less => matched_symbol_func,
            Ordering::Greater => matched_guid_func,
            Ordering::Equal => {
                // If the two highest our the same we can use it.
                let ty_is_same = matched_guid_func?.ty == matched_symbol_func?.ty;
                let sym_is_same = matched_guid_func?.symbol == matched_symbol_func?.symbol;
                if ty_is_same && sym_is_same {
                    matched_guid_func
                } else {
                    // We matched equally on two different functions
                    None
                }
            }
        }
    }
}

fn get_data_from_dir(dir: &PathBuf) -> HashMap<PathBuf, Data> {
    let data_from_entry = |entry: DirEntry| {
        let path = entry.path();
        let contents = std::fs::read(path).ok()?;
        Data::from_bytes(&contents)
    };

    WalkDir::new(dir)
        .into_iter()
        .filter_map(|e| e.ok())
        .filter(|e| e.file_type().is_file())
        .filter_map(|e| Some((e.clone().into_path(), data_from_entry(e)?)))
        .collect()
}

#[derive(Debug, Clone)]
pub struct MatcherSettings {
    /// Any function under this length will be required to constrain.
    ///
    /// This is set to [MatcherSettings::DEFAULT_TRIVIAL_FUNCTION_LEN] by default.
    pub trivial_function_len: u64,
}

impl MatcherSettings {
    pub const TRIVIAL_FUNCTION_LEN_DEFAULT: u64 = 20;
    pub const TRIVIAL_FUNCTION_LEN_SETTING: &'static str = "analysis.warp.trivial_function_len";

    /// Populates the [MatcherSettings] to the current Binary Ninja settings instance.
    ///
    /// Call this once when you initialize so that the settings exist.
    pub fn write_settings(&self) {
        let bn_settings = binaryninja::settings::Settings::new("");
        bn_settings.set_integer(
            Self::TRIVIAL_FUNCTION_LEN_SETTING,
            self.trivial_function_len,
            None,
            None,
        );
    }

    pub fn from_view(_view: &BinaryView) -> Self {
        let mut settings = MatcherSettings::default();
        let bn_settings = binaryninja::settings::Settings::new("");
        if bn_settings.contains(Self::TRIVIAL_FUNCTION_LEN_SETTING) {
            settings.trivial_function_len =
                bn_settings.get_integer(Self::TRIVIAL_FUNCTION_LEN_SETTING, None, None);
        }
        settings
    }
}

impl Default for MatcherSettings {
    fn default() -> Self {
        Self {
            trivial_function_len: MatcherSettings::TRIVIAL_FUNCTION_LEN_DEFAULT,
        }
    }
}

/// A unique platform ID, used for caching.
#[derive(Copy, Clone, Debug, Hash, PartialEq, Eq, PartialOrd, Ord)]
pub struct PlatformID(u64);

impl From<&Platform> for PlatformID {
    fn from(value: &Platform) -> Self {
        let mut hasher = DefaultHasher::new();
        hasher.write(value.name().to_bytes());
        Self(hasher.finish())
    }
}

impl From<BNRef<Platform>> for PlatformID {
    fn from(value: BNRef<Platform>) -> Self {
        Self::from(value.as_ref())
    }
}

impl From<Guard<'_, Platform>> for PlatformID {
    fn from(value: Guard<'_, Platform>) -> Self {
        Self::from(value.as_ref())
    }
}
