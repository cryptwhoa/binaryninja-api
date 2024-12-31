#ifndef DECODE_PPC_H
#define DECODE_PPC_H

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

// Procedure for adding a new instruction:
// 	* Add its enum to the InstructionId in this file (decode.h)
// 	* Add logic to convert instruction bits to the new instruction in decode.c
// 	* Add its mnemonic in `GetMnemonic` in `mnemonic.c`
// 	* Add its operands in `FillOperands` in `operands.c`
//
// This only handles instruction decoding (going from bits --> a powerpc
// instruction), and not anything like lifting or the behavior of the
// instruction itself.

#define DECODE_FLAGS_PPC64      0x01
#define DECODE_FLAGS_ALTIVEC    0x02
#define DECODE_FLAGS_VSX        0x04
#define DECODE_FLAGS_QPX	0x08
#define DECODE_FLAGS_PS		0x10
#define DECODE_FLAGS_SPE	0x20

#ifdef __cplusplus
extern "C" {
#endif
	enum Register
	{
		PPC_REG_GPR0,
		PPC_REG_GPR1,
		PPC_REG_GPR2,
		PPC_REG_GPR3,
		PPC_REG_GPR4,
		PPC_REG_GPR5,
		PPC_REG_GPR6,
		PPC_REG_GPR7,
		PPC_REG_GPR8,
		PPC_REG_GPR9,
		PPC_REG_GPR10,
		PPC_REG_GPR11,
		PPC_REG_GPR12,
		PPC_REG_GPR13,
		PPC_REG_GPR14,
		PPC_REG_GPR15,
		PPC_REG_GPR16,
		PPC_REG_GPR17,
		PPC_REG_GPR18,
		PPC_REG_GPR19,
		PPC_REG_GPR20,
		PPC_REG_GPR21,
		PPC_REG_GPR22,
		PPC_REG_GPR23,
		PPC_REG_GPR24,
		PPC_REG_GPR25,
		PPC_REG_GPR26,
		PPC_REG_GPR27,
		PPC_REG_GPR28,
		PPC_REG_GPR29,
		PPC_REG_GPR30,
		PPC_REG_GPR31,

		PPC_REG_XER,
		PPC_REG_LR,
		PPC_REG_CTR,

		PPC_REG_FR0,
		PPC_REG_FR1,
		PPC_REG_FR2,
		PPC_REG_FR3,
		PPC_REG_FR4,
		PPC_REG_FR5,
		PPC_REG_FR6,
		PPC_REG_FR7,
		PPC_REG_FR8,
		PPC_REG_FR9,
		PPC_REG_FR10,
		PPC_REG_FR11,
		PPC_REG_FR12,
		PPC_REG_FR13,
		PPC_REG_FR14,
		PPC_REG_FR15,
		PPC_REG_FR16,
		PPC_REG_FR17,
		PPC_REG_FR18,
		PPC_REG_FR19,
		PPC_REG_FR20,
		PPC_REG_FR21,
		PPC_REG_FR22,
		PPC_REG_FR23,
		PPC_REG_FR24,
		PPC_REG_FR25,
		PPC_REG_FR26,
		PPC_REG_FR27,
		PPC_REG_FR28,
		PPC_REG_FR29,
		PPC_REG_FR30,
		PPC_REG_FR31,

		// each 4-bit CR register
		PPC_REG_CRF0,
		PPC_REG_CRF1,
		PPC_REG_CRF2,
		PPC_REG_CRF3,
		PPC_REG_CRF4,
		PPC_REG_CRF5,
		PPC_REG_CRF6,
		PPC_REG_CRF7,

		PPC_REG_AV_VR0,
		PPC_REG_AV_VR1,
		PPC_REG_AV_VR2,
		PPC_REG_AV_VR3,
		PPC_REG_AV_VR4,
		PPC_REG_AV_VR5,
		PPC_REG_AV_VR6,
		PPC_REG_AV_VR7,
		PPC_REG_AV_VR8,
		PPC_REG_AV_VR9,
		PPC_REG_AV_VR10,
		PPC_REG_AV_VR11,
		PPC_REG_AV_VR12,
		PPC_REG_AV_VR13,
		PPC_REG_AV_VR14,
		PPC_REG_AV_VR15,
		PPC_REG_AV_VR16,
		PPC_REG_AV_VR17,
		PPC_REG_AV_VR18,
		PPC_REG_AV_VR19,
		PPC_REG_AV_VR20,
		PPC_REG_AV_VR21,
		PPC_REG_AV_VR22,
		PPC_REG_AV_VR23,
		PPC_REG_AV_VR24,
		PPC_REG_AV_VR25,
		PPC_REG_AV_VR26,
		PPC_REG_AV_VR27,
		PPC_REG_AV_VR28,
		PPC_REG_AV_VR29,
		PPC_REG_AV_VR30,
		PPC_REG_AV_VR31,

		PPC_REG_VSX_VR0,
		PPC_REG_VSX_VR1,
		PPC_REG_VSX_VR2,
		PPC_REG_VSX_VR3,
		PPC_REG_VSX_VR4,
		PPC_REG_VSX_VR5,
		PPC_REG_VSX_VR6,
		PPC_REG_VSX_VR7,
		PPC_REG_VSX_VR8,
		PPC_REG_VSX_VR9,
		PPC_REG_VSX_VR10,
		PPC_REG_VSX_VR11,
		PPC_REG_VSX_VR12,
		PPC_REG_VSX_VR13,
		PPC_REG_VSX_VR14,
		PPC_REG_VSX_VR15,
		PPC_REG_VSX_VR16,
		PPC_REG_VSX_VR17,
		PPC_REG_VSX_VR18,
		PPC_REG_VSX_VR19,
		PPC_REG_VSX_VR20,
		PPC_REG_VSX_VR21,
		PPC_REG_VSX_VR22,
		PPC_REG_VSX_VR23,
		PPC_REG_VSX_VR24,
		PPC_REG_VSX_VR25,
		PPC_REG_VSX_VR26,
		PPC_REG_VSX_VR27,
		PPC_REG_VSX_VR28,
		PPC_REG_VSX_VR29,
		PPC_REG_VSX_VR30,
		PPC_REG_VSX_VR31,
		PPC_REG_VSX_VR32,
		PPC_REG_VSX_VR33,
		PPC_REG_VSX_VR34,
		PPC_REG_VSX_VR35,
		PPC_REG_VSX_VR36,
		PPC_REG_VSX_VR37,
		PPC_REG_VSX_VR38,
		PPC_REG_VSX_VR39,
		PPC_REG_VSX_VR40,
		PPC_REG_VSX_VR41,
		PPC_REG_VSX_VR42,
		PPC_REG_VSX_VR43,
		PPC_REG_VSX_VR44,
		PPC_REG_VSX_VR45,
		PPC_REG_VSX_VR46,
		PPC_REG_VSX_VR47,
		PPC_REG_VSX_VR48,
		PPC_REG_VSX_VR49,
		PPC_REG_VSX_VR50,
		PPC_REG_VSX_VR51,
		PPC_REG_VSX_VR52,
		PPC_REG_VSX_VR53,
		PPC_REG_VSX_VR54,
		PPC_REG_VSX_VR55,
		PPC_REG_VSX_VR56,
		PPC_REG_VSX_VR57,
		PPC_REG_VSX_VR58,
		PPC_REG_VSX_VR59,
		PPC_REG_VSX_VR60,
		PPC_REG_VSX_VR61,
		PPC_REG_VSX_VR62,
		PPC_REG_VSX_VR63,

		PPC_REG_GQR0,
		PPC_REG_GQR1,
		PPC_REG_GQR2,
		PPC_REG_GQR3,
		PPC_REG_GQR4,
		PPC_REG_GQR5,
		PPC_REG_GQR6,
		PPC_REG_GQR7,
	};

#ifndef __cplusplus
	typedef enum Register Register;
#endif

	enum OperandClass
	{
		PPC_OP_NONE = 0,
		PPC_OP_UIMM,
		PPC_OP_SIMM,
		PPC_OP_LABEL,
		PPC_OP_MEM_RA,

		PPC_OP_REG_RA,
		PPC_OP_REG_RB,
		PPC_OP_REG_RC,
		PPC_OP_REG_RD,
		PPC_OP_REG_RS,

		PPC_OP_REG_FRA,
		PPC_OP_REG_FRB,
		PPC_OP_REG_FRC,
		PPC_OP_REG_FRD,
		PPC_OP_REG_FRS,

		PPC_OP_REG_CRFD,
		PPC_OP_REG_CRFD_IMPLY0,
		PPC_OP_REG_CRFS,
		PPC_OP_REG_CRFS_IMPLY0,
		PPC_OP_CRBIT,
		PPC_OP_CRBIT_A,
		PPC_OP_CRBIT_B,
		PPC_OP_CRBIT_D,

		PPC_OP_REG_AV_VA,
		PPC_OP_REG_AV_VB,
		PPC_OP_REG_AV_VC,
		PPC_OP_REG_AV_VD,
		PPC_OP_REG_AV_VS,

		// DWORD0 means that only the 0th dword is affected, which
		// has an overlap with floating registers
		PPC_OP_REG_VSX_RA,
		PPC_OP_REG_VSX_RA_DWORD0,

		PPC_OP_REG_VSX_RB,
		PPC_OP_REG_VSX_RB_DWORD0,

		PPC_OP_REG_VSX_RC,
		PPC_OP_REG_VSX_RC_DWORD0,

		PPC_OP_REG_VSX_RD,
		PPC_OP_REG_VSX_RD_DWORD0,

		PPC_OP_REG_VSX_RS,
		PPC_OP_REG_VSX_RS_DWORD0,
	};

#ifndef __cplusplus
	typedef enum OperandClass OperandClass;
#endif

	enum VsxWidth
	{
		VSX_WIDTH_FULL,
		VSX_WIDTH_DWORD0,
	};

#ifndef __cplusplus
	typedef enum VsxWidth VsxWidth;
#endif

	struct MemoryOperand
	{
		enum Register reg;
		int32_t offset;
	};

#ifndef __cplusplus
	typedef struct MemoryOperand MemoryOperand;
#endif

	struct Operand
	{
		OperandClass cls;
		union {
			enum Register reg;
			uint64_t uimm;
			int64_t simm;
			MemoryOperand mem;
			uint64_t label;
			uint32_t crbit;
		};
	};

#ifndef __cplusplus
	typedef struct Operand Operand;
#endif

	struct OperandsList
	{
		size_t numOperands;
		Operand operands[8];
	};

#ifndef __cplusplus
	typedef struct OperandsList OperandsList;
#endif

	// trailing lowercase 'x' indicates potential other mnemonics (rc, oe, lk,
	// etc. bits)
	enum InstructionId
	{
		PPC_ID_INVALID,

		PPC_ID_ADDx,
		PPC_ID_ADDCx,
		PPC_ID_ADDEx,
		PPC_ID_ADDI,
		PPC_ID_ADDICx,
		PPC_ID_ADDIS,
		PPC_ID_ADDMEx,
		PPC_ID_ADDPCIS,
		PPC_ID_ADDZEx,
		PPC_ID_ANDx,
		PPC_ID_ANDCx,
		PPC_ID_ANDI,
		PPC_ID_ANDIS,
		PPC_ID_ATTN,
		PPC_ID_Bx,
		PPC_ID_BCx,
		PPC_ID_BCCTRx,
		PPC_ID_BCLRx,
		PPC_ID_BPERMD,
		PPC_ID_CLRBHRB,
		PPC_ID_CLRLDIx,
		PPC_ID_CLRLWIx,
		PPC_ID_CLRRWIx,
		PPC_ID_CMPB,
		PPC_ID_CMPD,
		PPC_ID_CMPDI,
		PPC_ID_CMPEQB,
		PPC_ID_CMPRB,
		PPC_ID_CMPW,
		PPC_ID_CMPWI,
		PPC_ID_CMPLD,
		PPC_ID_CMPLDI,
		PPC_ID_CMPLW,
		PPC_ID_CMPLWI,
		PPC_ID_CNTLZDx,
		PPC_ID_CNTLZWx,
		PPC_ID_CNTTZDx,
		PPC_ID_CNTTZWx,
		PPC_ID_COPY,
		PPC_ID_CP_ABORT,
		PPC_ID_CRAND,
		PPC_ID_CRANDC,
		PPC_ID_CRCLR,
		PPC_ID_CREQV,
		PPC_ID_CRMOVE,
		PPC_ID_CRNAND,
		PPC_ID_CRNOR,
		PPC_ID_CRNOT,
		PPC_ID_CROR,
		PPC_ID_CRORC,
		PPC_ID_CRSET,
		PPC_ID_CRXOR,
		PPC_ID_DARN,
		PPC_ID_DCBA,
		PPC_ID_DCBF,
		PPC_ID_DCBFEP,
		PPC_ID_DCBFL,
		PPC_ID_DCBFLP,
		PPC_ID_DCBI,
		PPC_ID_DCBST,
		PPC_ID_DCBSTEP,
		PPC_ID_DCBT,
		PPC_ID_DCBTEP,
		PPC_ID_DCBTST,
		PPC_ID_DCBTSTEP,
		PPC_ID_DCBTSTT,
		PPC_ID_DCBTT,
		PPC_ID_DCBZ,
		PPC_ID_DCBZEP,
		PPC_ID_DCBZL,
		PPC_ID_DCBZLEP,
		PPC_ID_DCCCI,
		PPC_ID_DCI,
		PPC_ID_DIVDx,
		PPC_ID_DIVDEx,
		PPC_ID_DIVDEUx,
		PPC_ID_DIVDUx,
		PPC_ID_DIVWx,
		PPC_ID_DIVWEx,
		PPC_ID_DIVWEUx,
		PPC_ID_DIVWUx,
		PPC_ID_ECIWX,
		PPC_ID_ECOWX,
		PPC_ID_EIEIO,
		PPC_ID_EQVx,
		PPC_ID_EXTSBx,
		PPC_ID_EXTSHx,
		PPC_ID_EXTSWx,
		PPC_ID_EXTSWSLIx,
		PPC_ID_FABSx,
		PPC_ID_FADDx,
		PPC_ID_FADDSx,
		PPC_ID_FCFIDx,
		PPC_ID_FCFIDSx,
		PPC_ID_FCFIDUx,
		PPC_ID_FCFIDUSx,
		PPC_ID_FCMPO,
		PPC_ID_FCMPU,
		PPC_ID_FCPSGNx,
		PPC_ID_FCTIDx,
		PPC_ID_FCTIDUx,
		PPC_ID_FCTIDUZx,
		PPC_ID_FCTIDZx,
		PPC_ID_FCTIWx,
		PPC_ID_FCTIWUx,
		PPC_ID_FCTIWUZx,
		PPC_ID_FCTIWZx,
		PPC_ID_FDIVx,
		PPC_ID_FDIVSx,
		PPC_ID_FMADDx,
		PPC_ID_FMADDSx,
		PPC_ID_FMRx,
		PPC_ID_FMSUBx,
		PPC_ID_FMSUBSx,
		PPC_ID_FMULx,
		PPC_ID_FMULSx,
		PPC_ID_FNABSx,
		PPC_ID_FNEGx,
		PPC_ID_FNMADDx,
		PPC_ID_FNMADDSx,
		PPC_ID_FNMSUBx,
		PPC_ID_FNMSUBSx,
		PPC_ID_FREx,
		PPC_ID_FRESx,
		PPC_ID_FRIMx,
		PPC_ID_FRINx,
		PPC_ID_FRIPx,
		PPC_ID_FRIZx,
		PPC_ID_FRSPx,
		PPC_ID_FRSQRTEx,
		PPC_ID_FRSQRTESx,
		PPC_ID_FSELx,
		PPC_ID_FSQRTx,
		PPC_ID_FSQRTSx,
		PPC_ID_FSUBx,
		PPC_ID_FSUBSx,
		PPC_ID_FTDIV,
		PPC_ID_FTSQRT,
		PPC_ID_HRFID,
		PPC_ID_ICBI,
		PPC_ID_ICBIEP,
		PPC_ID_ICBLC,
		PPC_ID_ICBLQ,
		PPC_ID_ICBT,
		PPC_ID_ICBTLS,
		PPC_ID_ICCCI,
		PPC_ID_ICI,
		PPC_ID_ISEL,
		PPC_ID_ISYNC,
		PPC_ID_LBARX,
		PPC_ID_LBEPX,
		PPC_ID_LBZ,
		PPC_ID_LBZCIX,
		PPC_ID_LBZU,
		PPC_ID_LBZUX,
		PPC_ID_LBZX,
		PPC_ID_LD,
		PPC_ID_LDAT,
		PPC_ID_LDARX,
		PPC_ID_LDBRX,
		PPC_ID_LDCIX,
		PPC_ID_LDU,
		PPC_ID_LDUX,
		PPC_ID_LDX,
		PPC_ID_LFD,
		PPC_ID_LFDEPX,
		PPC_ID_LFDU,
		PPC_ID_LFDUX,
		PPC_ID_LFDX,
		PPC_ID_LFIWAX,
		PPC_ID_LFIWZX,
		PPC_ID_LFS,
		PPC_ID_LFSU,
		PPC_ID_LFSUX,
		PPC_ID_LFSX,
		PPC_ID_LHA,
		PPC_ID_LHAU,
		PPC_ID_LHAUX,
		PPC_ID_LHAX,
		PPC_ID_LHARX,
		PPC_ID_LHBRX,
		PPC_ID_LHEPX,
		PPC_ID_LHZ,
		PPC_ID_LHZCIX,
		PPC_ID_LHZU,
		PPC_ID_LHZUX,
		PPC_ID_LHZX,
		PPC_ID_LI,
		PPC_ID_LIS,
		PPC_ID_LMW,
		PPC_ID_LNIA,
		PPC_ID_LSWI,
		PPC_ID_LSWX,
		PPC_ID_LWA,
		PPC_ID_LWAT,
		PPC_ID_LWAX,
		PPC_ID_LWARX,
		PPC_ID_LWAUX,
		PPC_ID_LWBRX,
		PPC_ID_LWEPX,
		PPC_ID_LWSYNC,
		PPC_ID_LWZ,
		PPC_ID_LWZCIX,
		PPC_ID_LWZU,
		PPC_ID_LWZUX,
		PPC_ID_LWZX,
		PPC_ID_MBAR,
		PPC_ID_MCRF,
		PPC_ID_MCRFS,
		PPC_ID_MCRXR,
		PPC_ID_MCRXRX,
		PPC_ID_MFBHRBE,
		PPC_ID_MFBR0,
		PPC_ID_MFBR1,
		PPC_ID_MFBR2,
		PPC_ID_MFBR3,
		PPC_ID_MFBR4,
		PPC_ID_MFBR5,
		PPC_ID_MFBR6,
		PPC_ID_MFBR7,
		PPC_ID_MFCR,
		PPC_ID_MFCTR,
		PPC_ID_MFDCR,
		PPC_ID_MFDCRUX,
		PPC_ID_MFDCRX,
		PPC_ID_MFFSx,
		PPC_ID_MFFSCDRN,
		PPC_ID_MFFSCDRNI,
		PPC_ID_MFFSCE,
		PPC_ID_MFFSCRN,
		PPC_ID_MFFSCRNI,
		PPC_ID_MFFSL,
		PPC_ID_MFLR,
		PPC_ID_MFMSR,
		PPC_ID_MFOCRF,
		PPC_ID_MFPMR,
		PPC_ID_MFSPR,
		PPC_ID_MFSR,
		PPC_ID_MFSRIN,
		PPC_ID_MFTB,
		PPC_ID_MFTBU,
		PPC_ID_MFXER,
		PPC_ID_MRx,
		PPC_ID_MSGSYNC,
		PPC_ID_MTAMR,
		PPC_ID_MTBR0,
		PPC_ID_MTBR1,
		PPC_ID_MTBR2,
		PPC_ID_MTBR3,
		PPC_ID_MTBR4,
		PPC_ID_MTBR5,
		PPC_ID_MTBR6,
		PPC_ID_MTBR7,
		PPC_ID_MTCFAR,
		PPC_ID_MTCRF,
		PPC_ID_MTCTR,
		PPC_ID_MTDAR,
		PPC_ID_MTDCCR,
		PPC_ID_MTDCR,
		PPC_ID_MTDCRUX,
		PPC_ID_MTDCRX,
		PPC_ID_MTFSB0x,
		PPC_ID_MTFSB1x,
		PPC_ID_MTFSFx,
		PPC_ID_MTFSFIx,
		PPC_ID_MODSD,
		PPC_ID_MODSW,
		PPC_ID_MODUD,
		PPC_ID_MODUW,
		PPC_ID_MTICCR,
		PPC_ID_MTLR,
		PPC_ID_MTMSR,
		PPC_ID_MTMSRD,
		PPC_ID_MTOCRF,
		PPC_ID_MTPMR,
		PPC_ID_MTSPR,
		PPC_ID_MTSR,
		PPC_ID_MTSRIN,
		PPC_ID_MTSRR2,
		PPC_ID_MTSRR3,
		PPC_ID_MTTBHI,
		PPC_ID_MTTBL,
		PPC_ID_MTTBU,
		PPC_ID_MTXER,
		PPC_ID_MULHDx,
		PPC_ID_MULHDUx,
		PPC_ID_MULHWx,
		PPC_ID_MULHWUx,
		PPC_ID_MULLDx,
		PPC_ID_MULLI,
		PPC_ID_MULLWx,
		PPC_ID_NAP,
		PPC_ID_NANDx,
		PPC_ID_NEGx,
		PPC_ID_NOP,
		PPC_ID_NORx,
		PPC_ID_ORx,
		PPC_ID_ORCx,
		PPC_ID_ORI,
		PPC_ID_ORIS,
		PPC_ID_PASTE,
		PPC_ID_POPCNTB,
		PPC_ID_POPCNTD,
		PPC_ID_POPCNTW,
		PPC_ID_PTESYNC,
		PPC_ID_RFCI,
		PPC_ID_RFDI,
		PPC_ID_RFEBB,
		PPC_ID_RFI,
		PPC_ID_RFID,
		PPC_ID_RFMCI,
		PPC_ID_RLDICLx,
		PPC_ID_RLDICRx,
		PPC_ID_RLDICx,
		PPC_ID_RLDIMIx,
		PPC_ID_RLDCLx,
		PPC_ID_RLDCRx,
		PPC_ID_RLWIMIx,
		PPC_ID_RLWINMx,
		PPC_ID_RLWNMx,
		PPC_ID_ROTLDx,
		PPC_ID_ROTLDIx,
		PPC_ID_ROTLWx,
		PPC_ID_ROTLWIx,
		PPC_ID_SC,
		PPC_ID_SETB,
		PPC_ID_SLBIA,
		PPC_ID_SLBIE,
		PPC_ID_SLBIEG,
		PPC_ID_SLBMFEE,
		PPC_ID_SLBMFEV,
		PPC_ID_SLBMTE,
		PPC_ID_SLBSYNC,
		PPC_ID_SLDx,
		PPC_ID_SLDIx,
		PPC_ID_SLWx,
		PPC_ID_SLWIx,
		PPC_ID_SRADx,
		PPC_ID_SRADIx,
		PPC_ID_SRAWx,
		PPC_ID_SRAWIx,
		PPC_ID_SRDx,
		PPC_ID_SRDIx,
		PPC_ID_SRWx,
		PPC_ID_SRWIx,
		PPC_ID_STB,
		PPC_ID_STBCX,
		PPC_ID_STBU,
		PPC_ID_STBUX,
		PPC_ID_STBX,
		PPC_ID_STBCIX,
		PPC_ID_STBEPX,
		PPC_ID_STD,
		PPC_ID_STDAT,
		PPC_ID_STDBRX,
		PPC_ID_STDCIX,
		PPC_ID_STDCX,
		PPC_ID_STDEPX,
		PPC_ID_STDU,
		PPC_ID_STDUX,
		PPC_ID_STDX,
		PPC_ID_STFD,
		PPC_ID_STFDEPX,
		PPC_ID_STFDU,
		PPC_ID_STFDUX,
		PPC_ID_STFDX,
		PPC_ID_STFIWX,
		PPC_ID_STFS,
		PPC_ID_STFSU,
		PPC_ID_STFSUX,
		PPC_ID_STFSX,
		PPC_ID_STH,
		PPC_ID_STHBRX,
		PPC_ID_STHCX,
		PPC_ID_STHCIX,
		PPC_ID_STHEPX,
		PPC_ID_STHU,
		PPC_ID_STHUX,
		PPC_ID_STHX,
		PPC_ID_STMW,
		PPC_ID_STOP,
		PPC_ID_STSWI,
		PPC_ID_STSWX,
		PPC_ID_STW,
		PPC_ID_STWAT,
		PPC_ID_STWBRX,
		PPC_ID_STWCIX,
		PPC_ID_STWCX,
		PPC_ID_STWEPX,
		PPC_ID_STWU,
		PPC_ID_STWUX,
		PPC_ID_STWX,
		PPC_ID_SUBFx,
		PPC_ID_SUBFCx,
		PPC_ID_SUBFEx,
		PPC_ID_SUBFIC,
		PPC_ID_SUBFMEx,
		PPC_ID_SUBFZEx,
		PPC_ID_SYNC,
		PPC_ID_TABORT,
		PPC_ID_TABORTDC,
		PPC_ID_TABORTDCI,
		PPC_ID_TABORTWC,
		PPC_ID_TABORTWCI,
		PPC_ID_TBEGIN,
		PPC_ID_TCHECK,
		PPC_ID_TD,
		PPC_ID_TDEQ,
		PPC_ID_TDEQI,
		PPC_ID_TDGT,
		PPC_ID_TDGTI,
		PPC_ID_TDI,
		PPC_ID_TDLGT,
		PPC_ID_TDLGTI,
		PPC_ID_TDLLT,
		PPC_ID_TDLLTI,
		PPC_ID_TDLT,
		PPC_ID_TDLTI,
		PPC_ID_TDNE,
		PPC_ID_TDNEI,
		PPC_ID_TDU,
		PPC_ID_TDUI,
		PPC_ID_TEND,
		PPC_ID_TLBIA,
		PPC_ID_TLBIE,
		PPC_ID_TLBIEL,
		PPC_ID_TLBIVAX,
		PPC_ID_TLBLI,
		PPC_ID_TLBSX,
		PPC_ID_TLBSYNC,
		PPC_ID_TLBRE,
		PPC_ID_TLBRELO,
		PPC_ID_TLBREHI,
		PPC_ID_TLBWE,
		PPC_ID_TLBWEHI,
		PPC_ID_TLBWELO,
		PPC_ID_TRAP,
		PPC_ID_TRECHKPT,
		PPC_ID_TRECLAIM,
		PPC_ID_TSR,
		PPC_ID_TW,
		PPC_ID_TWEQ,
		PPC_ID_TWEQI,
		PPC_ID_TWGT,
		PPC_ID_TWGTI,
		PPC_ID_TWGEI,
		PPC_ID_TWI,
		PPC_ID_TWLEI,
		PPC_ID_TWLLEI,
		PPC_ID_TWLGT,
		PPC_ID_TWLGTI,
		PPC_ID_TWLLT,
		PPC_ID_TWLLTI,
		PPC_ID_TWLT,
		PPC_ID_TWLTI,
		PPC_ID_TWNE,
		PPC_ID_TWNEI,
		PPC_ID_TWU,
		PPC_ID_TWUI,
		PPC_ID_WAIT,
		PPC_ID_WAITIMPL,
		PPC_ID_WAITRSV,
		PPC_ID_WRTEE,
		PPC_ID_WRTEEI,
		PPC_ID_XNOP,
		PPC_ID_XORx,
		PPC_ID_XORI,
		PPC_ID_XORIS,

		// AltiVec instructions
		PPC_ID_AV_VABSDUB,
		PPC_ID_AV_VABSDUH,
		PPC_ID_AV_VABSDUW,
		PPC_ID_AV_VADDUQM,
		PPC_ID_AV_VADDCUQ,
		PPC_ID_AV_BCDADD,
		PPC_ID_AV_BCDCFN,
		PPC_ID_AV_BCDCFSQ,
		PPC_ID_AV_BCDCFZ,
		PPC_ID_AV_BCDCPSGN,
		PPC_ID_AV_BCDCTN,
		PPC_ID_AV_BCDCTSQ,
		PPC_ID_AV_BCDCTZ,
		PPC_ID_AV_BCDS,
		PPC_ID_AV_BCDSETSGN,
		PPC_ID_AV_BCDSR,
		PPC_ID_AV_BCDSUB,
		PPC_ID_AV_BCDTRUNC,
		PPC_ID_AV_BCDUS,
		PPC_ID_AV_BCDUTRUNC,
		PPC_ID_AV_DSS,
		PPC_ID_AV_DSSALL,
		PPC_ID_AV_DST,
		PPC_ID_AV_DSTST,
		PPC_ID_AV_DSTSTT,
		PPC_ID_AV_DSTT,
		PPC_ID_AV_LVEBX,
		PPC_ID_AV_LVEHX,
		PPC_ID_AV_LVEWX,
		PPC_ID_AV_LVSL,
		PPC_ID_AV_LVSR,
		PPC_ID_AV_LVX,
		PPC_ID_AV_LVXL,
		PPC_ID_AV_MADDHD,
		PPC_ID_AV_MADDHDU,
		PPC_ID_AV_MADDLD,
		PPC_ID_AV_MFVSCR,
		PPC_ID_AV_MTVSCR,
		PPC_ID_AV_STVEBX,
		PPC_ID_AV_STVEHX,
		PPC_ID_AV_STVEWX,
		PPC_ID_AV_STVX,
		PPC_ID_AV_STVXL,
		PPC_ID_AV_VADDCUW,
		PPC_ID_AV_VADDECUQ,
		PPC_ID_AV_VADDEUQM,
		PPC_ID_AV_VADDFP,
		PPC_ID_AV_VADDSBS,
		PPC_ID_AV_VADDSHS,
		PPC_ID_AV_VADDSWS,
		PPC_ID_AV_VADDUBM,
		PPC_ID_AV_VADDUBS,
		PPC_ID_AV_VADDUDM,
		PPC_ID_AV_VADDUHM,
		PPC_ID_AV_VADDUHS,
		PPC_ID_AV_VADDUWM,
		PPC_ID_AV_VADDUWS,
		PPC_ID_AV_VAND,
		PPC_ID_AV_VANDC,
		PPC_ID_AV_VAVGSB,
		PPC_ID_AV_VAVGSH,
		PPC_ID_AV_VAVGSW,
		PPC_ID_AV_VAVGUB,
		PPC_ID_AV_VAVGUH,
		PPC_ID_AV_VAVGUW,
		PPC_ID_AV_VBPERMD,
		PPC_ID_AV_VBPERMQ,
		PPC_ID_AV_VCFSX,
		PPC_ID_AV_VCFUX,
		PPC_ID_AV_VCIPHER,
		PPC_ID_AV_VCIPHERLAST,
		PPC_ID_AV_VCLZB,
		PPC_ID_AV_VCLZD,
		PPC_ID_AV_VCLZH,
		PPC_ID_AV_VCLZLSBB,
		PPC_ID_AV_VCLZW,
		PPC_ID_AV_VCMPBFPx,
		PPC_ID_AV_VCMPEQFPx,
		PPC_ID_AV_VCMPEQUBx,
		PPC_ID_AV_VCMPEQUDx,
		PPC_ID_AV_VCMPEQUHx,
		PPC_ID_AV_VCMPEQUWx,
		PPC_ID_AV_VCMPGEFPx,
		PPC_ID_AV_VCMPGTFPx,
		PPC_ID_AV_VCMPGTSBx,
		PPC_ID_AV_VCMPGTSDx,
		PPC_ID_AV_VCMPGTSHx,
		PPC_ID_AV_VCMPGTSWx,
		PPC_ID_AV_VCMPGTUBx,
		PPC_ID_AV_VCMPGTUDx,
		PPC_ID_AV_VCMPGTUHx,
		PPC_ID_AV_VCMPGTUWx,
		PPC_ID_AV_VCMPNEBx,
		PPC_ID_AV_VCMPNEHx,
		PPC_ID_AV_VCMPNEWx,
		PPC_ID_AV_VCMPNEZBx,
		PPC_ID_AV_VCMPNEZHx,
		PPC_ID_AV_VCMPNEZWx,
		PPC_ID_AV_VCTSXS,
		PPC_ID_AV_VCTUXS,
		PPC_ID_AV_VCTZB,
		PPC_ID_AV_VCTZD,
		PPC_ID_AV_VCTZH,
		PPC_ID_AV_VCTZLSBB,
		PPC_ID_AV_VCTZW,
		PPC_ID_AV_VEQV,
		PPC_ID_AV_VEXPTEFP,
		PPC_ID_AV_VEXTRACTD,
		PPC_ID_AV_VEXTRACTUB,
		PPC_ID_AV_VEXTRACTUH,
		PPC_ID_AV_VEXTRACTUW,
		PPC_ID_AV_VEXTSB2D,
		PPC_ID_AV_VEXTSB2W,
		PPC_ID_AV_VEXTSH2D,
		PPC_ID_AV_VEXTSH2W,
		PPC_ID_AV_VEXTSW2D,
		PPC_ID_AV_VEXTUBLX,
		PPC_ID_AV_VEXTUHLX,
		PPC_ID_AV_VEXTUWLX,
		PPC_ID_AV_VEXTUBRX,
		PPC_ID_AV_VEXTUHRX,
		PPC_ID_AV_VEXTUWRX,
		PPC_ID_AV_VGBBD,
		PPC_ID_AV_VINSERTB,
		PPC_ID_AV_VINSERTD,
		PPC_ID_AV_VINSERTH,
		PPC_ID_AV_VINSERTW,
		PPC_ID_AV_VLOGEFP,
		PPC_ID_AV_VMADDFP,
		PPC_ID_AV_VMAXFP,
		PPC_ID_AV_VMAXSB,
		PPC_ID_AV_VMAXSD,
		PPC_ID_AV_VMAXSH,
		PPC_ID_AV_VMAXSW,
		PPC_ID_AV_VMAXUB,
		PPC_ID_AV_VMAXUD,
		PPC_ID_AV_VMAXUH,
		PPC_ID_AV_VMAXUW,
		PPC_ID_AV_VMHADDSHS,
		PPC_ID_AV_VMHRADDSHS,
		PPC_ID_AV_VMINFP,
		PPC_ID_AV_VMINSB,
		PPC_ID_AV_VMINSD,
		PPC_ID_AV_VMINSH,
		PPC_ID_AV_VMINSW,
		PPC_ID_AV_VMINUB,
		PPC_ID_AV_VMINUD,
		PPC_ID_AV_VMINUH,
		PPC_ID_AV_VMINUW,
		PPC_ID_AV_VMLADDUHM,
		PPC_ID_AV_VMR,
		PPC_ID_AV_VMRGEW,
		PPC_ID_AV_VMRGHB,
		PPC_ID_AV_VMRGHH,
		PPC_ID_AV_VMRGHW,
		PPC_ID_AV_VMRGLB,
		PPC_ID_AV_VMRGLH,
		PPC_ID_AV_VMRGLW,
		PPC_ID_AV_VMRGOW,
		PPC_ID_AV_VMSUMMBM,
		PPC_ID_AV_VMSUMSHM,
		PPC_ID_AV_VMSUMSHS,
		PPC_ID_AV_VMSUMUBM,
		PPC_ID_AV_VMSUMUHM,
		PPC_ID_AV_VMSUMUHS,
		PPC_ID_AV_VMUL10CUQ,
		PPC_ID_AV_VMUL10EUQ,
		PPC_ID_AV_VMUL10ECUQ,
		PPC_ID_AV_VMUL10UQ,
		PPC_ID_AV_VMULESB,
		PPC_ID_AV_VMULESH,
		PPC_ID_AV_VMULESW,
		PPC_ID_AV_VMULEUB,
		PPC_ID_AV_VMULEUH,
		PPC_ID_AV_VMULEUW,
		PPC_ID_AV_VMULOSB,
		PPC_ID_AV_VMULOSH,
		PPC_ID_AV_VMULOSW,
		PPC_ID_AV_VMULOUB,
		PPC_ID_AV_VMULOUH,
		PPC_ID_AV_VMULOUW,
		PPC_ID_AV_VMULUWM,
		PPC_ID_AV_VNAND,
		PPC_ID_AV_VNCIPHER,
		PPC_ID_AV_VNCIPHERLAST,
		PPC_ID_AV_VNMSUBFP,
		PPC_ID_AV_VNEGD,
		PPC_ID_AV_VNEGW,
		PPC_ID_AV_VNOR,
		PPC_ID_AV_VNOT,
		PPC_ID_AV_VOR,
		PPC_ID_AV_VORC,
		PPC_ID_AV_VPERM,
		PPC_ID_AV_VPERMR,
		PPC_ID_AV_VPERMXOR,
		PPC_ID_AV_VPKPX,
		PPC_ID_AV_VPKSDSS,
		PPC_ID_AV_VPKSDUS,
		PPC_ID_AV_VPKSHSS,
		PPC_ID_AV_VPKSHUS,
		PPC_ID_AV_VPKSWSS,
		PPC_ID_AV_VPKSWUS,
		PPC_ID_AV_VPKUDUM,
		PPC_ID_AV_VPKUDUS,
		PPC_ID_AV_VPKUHUM,
		PPC_ID_AV_VPKUHUS,
		PPC_ID_AV_VPKUWUM,
		PPC_ID_AV_VPKUWUS,
		PPC_ID_AV_VPMSUMB,
		PPC_ID_AV_VPMSUMD,
		PPC_ID_AV_VPMSUMH,
		PPC_ID_AV_VPMSUMW,
		PPC_ID_AV_VPOPCNTB,
		PPC_ID_AV_VPOPCNTD,
		PPC_ID_AV_VPOPCNTH,
		PPC_ID_AV_VPOPCNTW,
		PPC_ID_AV_VPRTYBD,
		PPC_ID_AV_VPRTYBQ,
		PPC_ID_AV_VPRTYBW,
		PPC_ID_AV_VREFP,
		PPC_ID_AV_VRFIM,
		PPC_ID_AV_VRFIN,
		PPC_ID_AV_VRFIP,
		PPC_ID_AV_VRFIZ,
		PPC_ID_AV_VRLB,
		PPC_ID_AV_VRLD,
		PPC_ID_AV_VRLDNM,
		PPC_ID_AV_VRLDMI,
		PPC_ID_AV_VRLH,
		PPC_ID_AV_VRLW,
		PPC_ID_AV_VRLWMI,
		PPC_ID_AV_VRLWNM,
		PPC_ID_AV_VRSQRTEFP,
		PPC_ID_AV_VSBOX,
		PPC_ID_AV_VSEL,
		PPC_ID_AV_VSHASIGMAD,
		PPC_ID_AV_VSHASIGMAW,
		PPC_ID_AV_VSL,
		PPC_ID_AV_VSLB,
		PPC_ID_AV_VSLD,
		PPC_ID_AV_VSLDOI,
		PPC_ID_AV_VSLH,
		PPC_ID_AV_VSLO,
		PPC_ID_AV_VSLV,
		PPC_ID_AV_VSLW,
		PPC_ID_AV_VSPLTB,
		PPC_ID_AV_VSPLTH,
		PPC_ID_AV_VSPLTISB,
		PPC_ID_AV_VSPLTISH,
		PPC_ID_AV_VSPLTISW,
		PPC_ID_AV_VSPLTW,
		PPC_ID_AV_VSR,
		PPC_ID_AV_VSRAB,
		PPC_ID_AV_VSRAD,
		PPC_ID_AV_VSRAH,
		PPC_ID_AV_VSRAW,
		PPC_ID_AV_VSRB,
		PPC_ID_AV_VSRD,
		PPC_ID_AV_VSRH,
		PPC_ID_AV_VSRO,
		PPC_ID_AV_VSRV,
		PPC_ID_AV_VSRW,
		PPC_ID_AV_VSUBCUQ,
		PPC_ID_AV_VSUBCUW,
		PPC_ID_AV_VSUBECUQ,
		PPC_ID_AV_VSUBEUQM,
		PPC_ID_AV_VSUBFP,
		PPC_ID_AV_VSUBSBS,
		PPC_ID_AV_VSUBSHS,
		PPC_ID_AV_VSUBSWS,
		PPC_ID_AV_VSUBUBM,
		PPC_ID_AV_VSUBUBS,
		PPC_ID_AV_VSUBUDM,
		PPC_ID_AV_VSUBUHM,
		PPC_ID_AV_VSUBUHS,
		PPC_ID_AV_VSUBUQM,
		PPC_ID_AV_VSUBUWM,
		PPC_ID_AV_VSUBUWS,
		PPC_ID_AV_VSUMSWS,
		PPC_ID_AV_VSUM2SWS,
		PPC_ID_AV_VSUM4SBS,
		PPC_ID_AV_VSUM4SHS,
		PPC_ID_AV_VSUM4UBS,
		PPC_ID_AV_VUPKHPX,
		PPC_ID_AV_VUPKHSB,
		PPC_ID_AV_VUPKHSH,
		PPC_ID_AV_VUPKHSW,
		PPC_ID_AV_VUPKLPX,
		PPC_ID_AV_VUPKLSB,
		PPC_ID_AV_VUPKLSH,
		PPC_ID_AV_VUPKLSW,
		PPC_ID_AV_VXOR,

		// VSX instructions
		PPC_ID_VSX_LXSD,
		PPC_ID_VSX_LXSDX,
		PPC_ID_VSX_LXSIBZX,
		PPC_ID_VSX_LXSIHZX,
		PPC_ID_VSX_LXSIWAX,
		PPC_ID_VSX_LXSIWZX,
		PPC_ID_VSX_LXSSP,
		PPC_ID_VSX_LXSSPX,
		PPC_ID_VSX_LXV,
		PPC_ID_VSX_LXVB16X,
		PPC_ID_VSX_LXVD2X,
		PPC_ID_VSX_LXVDSX,
		PPC_ID_VSX_LXVH8X,
		PPC_ID_VSX_LXVL,
		PPC_ID_VSX_LXVLL,
		PPC_ID_VSX_LXVW4X,
		PPC_ID_VSX_LXVWSX,
		PPC_ID_VSX_LXVX,
		PPC_ID_VSX_MFFPRD,
		PPC_ID_VSX_MFVSRD,
		PPC_ID_VSX_MFVSRLD,
		PPC_ID_VSX_MFVSRWZ,
		PPC_ID_VSX_MTVSRD,
		PPC_ID_VSX_MTVSRDD,
		PPC_ID_VSX_MTVSRWA,
		PPC_ID_VSX_MTVSRWS,
		PPC_ID_VSX_MTVSRWZ,
		PPC_ID_VSX_STXSD,
		PPC_ID_VSX_STXSDX,
		PPC_ID_VSX_STXSIBX,
		PPC_ID_VSX_STXSIHX,
		PPC_ID_VSX_STXSIWX,
		PPC_ID_VSX_STXSSP,
		PPC_ID_VSX_STXSSPX,
		PPC_ID_VSX_STXV,
		PPC_ID_VSX_STXVB16X,
		PPC_ID_VSX_STXVD2X,
		PPC_ID_VSX_STXVH8X,
		PPC_ID_VSX_STXVL,
		PPC_ID_VSX_STXVLL,
		PPC_ID_VSX_STXVW4X,
		PPC_ID_VSX_STXVX,
		PPC_ID_VSX_XSABSDP,
		PPC_ID_VSX_XSABSQP,
		PPC_ID_VSX_XSADDDP,
		PPC_ID_VSX_XSADDSP,
		PPC_ID_VSX_XSADDQPx,
		PPC_ID_VSX_XSCMPEQDP,
		PPC_ID_VSX_XSCMPEXPDP,
		PPC_ID_VSX_XSCMPEXPQP,
		PPC_ID_VSX_XSCMPGEDP,
		PPC_ID_VSX_XSCMPGTDP,
		PPC_ID_VSX_XSCMPODP,
		PPC_ID_VSX_XSCMPOQP,
		PPC_ID_VSX_XSCMPUDP,
		PPC_ID_VSX_XSCMPUQP,
		PPC_ID_VSX_XSCPSGNDP,
		PPC_ID_VSX_XSCPSGNQP,
		PPC_ID_VSX_XSCVDPHP,
		PPC_ID_VSX_XSCVDPQP,
		PPC_ID_VSX_XSCVDPSP,
		PPC_ID_VSX_XSCVDPSPN,
		PPC_ID_VSX_XSCVDPSXDS,
		PPC_ID_VSX_XSCVDPSXWS,
		PPC_ID_VSX_XSCVDPUXDS,
		PPC_ID_VSX_XSCVDPUXWS,
		PPC_ID_VSX_XSCVHPDP,
		PPC_ID_VSX_XSCVQPDPx,
		PPC_ID_VSX_XSCVQPSDZ,
		PPC_ID_VSX_XSCVQPSWZ,
		PPC_ID_VSX_XSCVQPUDZ,
		PPC_ID_VSX_XSCVQPUWZ,
		PPC_ID_VSX_XSCVSDQP,
		PPC_ID_VSX_XSCVSPDP,
		PPC_ID_VSX_XSCVSPDPN,
		PPC_ID_VSX_XSCVSXDDP,
		PPC_ID_VSX_XSCVSXDSP,
		PPC_ID_VSX_XSCVUDQP,
		PPC_ID_VSX_XSCVUXDDP,
		PPC_ID_VSX_XSCVUXDSP,
		PPC_ID_VSX_XSDIVDP,
		PPC_ID_VSX_XSDIVSP,
		PPC_ID_VSX_XSDIVQPx,
		PPC_ID_VSX_XSIEXPDP,
		PPC_ID_VSX_XSIEXPQP,
		PPC_ID_VSX_XSMADDADP,
		PPC_ID_VSX_XSMADDASP,
		PPC_ID_VSX_XSMADDMDP,
		PPC_ID_VSX_XSMADDMSP,
		PPC_ID_VSX_XSMADDQPx,
		PPC_ID_VSX_XSMAXCDP,
		PPC_ID_VSX_XSMAXDP,
		PPC_ID_VSX_XSMAXJDP,
		PPC_ID_VSX_XSMINCDP,
		PPC_ID_VSX_XSMINDP,
		PPC_ID_VSX_XSMINJDP,
		PPC_ID_VSX_XSMSUBADP,
		PPC_ID_VSX_XSMSUBASP,
		PPC_ID_VSX_XSMSUBMDP,
		PPC_ID_VSX_XSMSUBMSP,
		PPC_ID_VSX_XSMSUBQPx,
		PPC_ID_VSX_XSMULDP,
		PPC_ID_VSX_XSMULSP,
		PPC_ID_VSX_XSMULQPx,
		PPC_ID_VSX_XSNABSDP,
		PPC_ID_VSX_XSNABSQP,
		PPC_ID_VSX_XSNEGDP,
		PPC_ID_VSX_XSNEGQP,
		PPC_ID_VSX_XSNMADDADP,
		PPC_ID_VSX_XSNMADDASP,
		PPC_ID_VSX_XSNMADDMDP,
		PPC_ID_VSX_XSNMADDMSP,
		PPC_ID_VSX_XSNMADDQPx,
		PPC_ID_VSX_XSNMSUBADP,
		PPC_ID_VSX_XSNMSUBASP,
		PPC_ID_VSX_XSNMSUBMDP,
		PPC_ID_VSX_XSNMSUBMSP,
		PPC_ID_VSX_XSNMSUBQPx,
		PPC_ID_VSX_XSRDPI,
		PPC_ID_VSX_XSRDPIC,
		PPC_ID_VSX_XSRDPIM,
		PPC_ID_VSX_XSRDPIP,
		PPC_ID_VSX_XSRDPIZ,
		PPC_ID_VSX_XSREDP,
		PPC_ID_VSX_XSRESP,
		PPC_ID_VSX_XSRQPIx,
		PPC_ID_VSX_XSRQPXP,
		PPC_ID_VSX_XSRSP,
		PPC_ID_VSX_XSRSQRTEDP,
		PPC_ID_VSX_XSRSQRTESP,
		PPC_ID_VSX_XSSQRTDP,
		PPC_ID_VSX_XSSQRTQPx,
		PPC_ID_VSX_XSSQRTSP,
		PPC_ID_VSX_XSSUBDP,
		PPC_ID_VSX_XSSUBSP,
		PPC_ID_VSX_XSSUBQPx,
		PPC_ID_VSX_XSTDIVDP,
		PPC_ID_VSX_XSTDIVSP,
		PPC_ID_VSX_XSTSTDCQP,
		PPC_ID_VSX_XSTSTDCDP,
		PPC_ID_VSX_XSTSTDCSP,
		PPC_ID_VSX_XSTSQRTDP,
		PPC_ID_VSX_XSXEXPDP,
		PPC_ID_VSX_XSXEXPQP,
		PPC_ID_VSX_XSXSIGDP,
		PPC_ID_VSX_XSXSIGQP,
		PPC_ID_VSX_XVABSDP,
		PPC_ID_VSX_XVABSSP,
		PPC_ID_VSX_XVADDDP,
		PPC_ID_VSX_XVADDSP,
		PPC_ID_VSX_XVCMPEQDPx,
		PPC_ID_VSX_XVCMPEQSPx,
		PPC_ID_VSX_XVCMPGEDPx,
		PPC_ID_VSX_XVCMPGESPx,
		PPC_ID_VSX_XVCMPGTDPx,
		PPC_ID_VSX_XVCMPGTSPx,
		PPC_ID_VSX_XVCPSGNDP,
		PPC_ID_VSX_XVCPSGNSP,
		PPC_ID_VSX_XVCVDPSP,
		PPC_ID_VSX_XVCVDPSXDS,
		PPC_ID_VSX_XVCVDPSXWS,
		PPC_ID_VSX_XVCVDPUXDS,
		PPC_ID_VSX_XVCVDPUXWS,
		PPC_ID_VSX_XVCVHPSP,
		PPC_ID_VSX_XVCVSPDP,
		PPC_ID_VSX_XVCVSPHP,
		PPC_ID_VSX_XVCVSPSXDS,
		PPC_ID_VSX_XVCVSPSXWS,
		PPC_ID_VSX_XVCVSPUXDS,
		PPC_ID_VSX_XVCVSPUXWS,
		PPC_ID_VSX_XVCVSXDDP,
		PPC_ID_VSX_XVCVSXDSP,
		PPC_ID_VSX_XVCVSXWDP,
		PPC_ID_VSX_XVCVSXWSP,
		PPC_ID_VSX_XVCVUXDDP,
		PPC_ID_VSX_XVCVUXDSP,
		PPC_ID_VSX_XVCVUXWDP,
		PPC_ID_VSX_XVCVUXWSP,
		PPC_ID_VSX_XVDIVDP,
		PPC_ID_VSX_XVDIVSP,
		PPC_ID_VSX_XVIEXPDP,
		PPC_ID_VSX_XVIEXPSP,
		PPC_ID_VSX_XVMADDADP,
		PPC_ID_VSX_XVMADDASP,
		PPC_ID_VSX_XVMADDMDP,
		PPC_ID_VSX_XVMADDMSP,
		PPC_ID_VSX_XVMAXDP,
		PPC_ID_VSX_XVMAXSP,
		PPC_ID_VSX_XVMINDP,
		PPC_ID_VSX_XVMINSP,
		PPC_ID_VSX_XVMOVDP,
		PPC_ID_VSX_XVMOVSP,
		PPC_ID_VSX_XVMSUBADP,
		PPC_ID_VSX_XVMSUBASP,
		PPC_ID_VSX_XVMSUBMDP,
		PPC_ID_VSX_XVMSUBMSP,
		PPC_ID_VSX_XVMULDP,
		PPC_ID_VSX_XVMULSP,
		PPC_ID_VSX_XVNABSDP,
		PPC_ID_VSX_XVNABSSP,
		PPC_ID_VSX_XVNMADDADP,
		PPC_ID_VSX_XVNMADDASP,
		PPC_ID_VSX_XVNMADDMDP,
		PPC_ID_VSX_XVNMADDMSP,
		PPC_ID_VSX_XVNEGDP,
		PPC_ID_VSX_XVNEGSP,
		PPC_ID_VSX_XVNMSUBADP,
		PPC_ID_VSX_XVNMSUBASP,
		PPC_ID_VSX_XVNMSUBMDP,
		PPC_ID_VSX_XVNMSUBMSP,
		PPC_ID_VSX_XVRDPI,
		PPC_ID_VSX_XVRDPIC,
		PPC_ID_VSX_XVRDPIM,
		PPC_ID_VSX_XVRDPIP,
		PPC_ID_VSX_XVRDPIZ,
		PPC_ID_VSX_XVREDP,
		PPC_ID_VSX_XVRESP,
		PPC_ID_VSX_XVRSPI,
		PPC_ID_VSX_XVRSPIC,
		PPC_ID_VSX_XVRSPIM,
		PPC_ID_VSX_XVRSPIP,
		PPC_ID_VSX_XVRSPIZ,
		PPC_ID_VSX_XVRSQRTEDP,
		PPC_ID_VSX_XVRSQRTESP,
		PPC_ID_VSX_XVSQRTDP,
		PPC_ID_VSX_XVSQRTSP,
		PPC_ID_VSX_XVSUBDP,
		PPC_ID_VSX_XVSUBSP,
		PPC_ID_VSX_XVTDIVDP,
		PPC_ID_VSX_XVTDIVSP,
		PPC_ID_VSX_XVTSQRTDP,
		PPC_ID_VSX_XVTSQRTSP,
		PPC_ID_VSX_XVTSTDCDP,
		PPC_ID_VSX_XVTSTDCSP,
		PPC_ID_VSX_XVXEXPDP,
		PPC_ID_VSX_XVXEXPSP,
		PPC_ID_VSX_XVXSIGDP,
		PPC_ID_VSX_XVXSIGSP,
		PPC_ID_VSX_XXBRD,
		PPC_ID_VSX_XXBRH,
		PPC_ID_VSX_XXBRQ,
		PPC_ID_VSX_XXBRW,
		PPC_ID_VSX_XXEXTRACTUW,
		PPC_ID_VSX_XXINSERTW,
		PPC_ID_VSX_XXLAND,
		PPC_ID_VSX_XXLANDC,
		PPC_ID_VSX_XXLEQV,
		PPC_ID_VSX_XXLNAND,
		PPC_ID_VSX_XXLNOR,
		PPC_ID_VSX_XXLOR,
		PPC_ID_VSX_XXLORC,
		PPC_ID_VSX_XXLXOR,
		PPC_ID_VSX_XXMRGHD,
		PPC_ID_VSX_XXMRGHW,
		PPC_ID_VSX_XXMRGLD,
		PPC_ID_VSX_XXMRGLW,
		PPC_ID_VSX_XXPERM,
		PPC_ID_VSX_XXPERMDI,
		PPC_ID_VSX_XXPERMR,
		PPC_ID_VSX_XXSEL,
		PPC_ID_VSX_XXSLDWI,
		PPC_ID_VSX_XXSPLTD,
		PPC_ID_VSX_XXSPLTIB,
		PPC_ID_VSX_XXSPLTW,
		PPC_ID_VSX_XXSWAPD,

		// Pair-stored
		PPC_ID_PSQ_L,
		PPC_ID_PSQ_LU,
		PPC_ID_PSQ_LUX,
		PPC_ID_PSQ_LX,
		PPC_ID_PSQ_ST,
		PPC_ID_PSQ_STU,
		PPC_ID_PSQ_STUX,
		PPC_ID_PSQ_STX,

		// SPE (Signal Processing Engine) instructions
		PPC_ID_SPE_BRINC,
		PPC_ID_SPE_EFDABS,
		PPC_ID_SPE_EFDADD,
		PPC_ID_SPE_EFDCFS,
		PPC_ID_SPE_EFDCFSF,
		PPC_ID_SPE_EFDCFSI,
		PPC_ID_SPE_EFDCFSID,
		PPC_ID_SPE_EFDCFUF,
		PPC_ID_SPE_EFDCFUI,
		PPC_ID_SPE_EFDCFUID,
		PPC_ID_SPE_EFDCMPEQ,
		PPC_ID_SPE_EFDCMPGT,
		PPC_ID_SPE_EFDCMPLT,
		PPC_ID_SPE_EFDCTSF,
		PPC_ID_SPE_EFDCTSI,
		PPC_ID_SPE_EFDCTSIDZ,
		PPC_ID_SPE_EFDCTSIZ,
		PPC_ID_SPE_EFDCTUF,
		PPC_ID_SPE_EFDCTUI,
		PPC_ID_SPE_EFDCTUIDZ,
		PPC_ID_SPE_EFDCTUIZ,
		PPC_ID_SPE_EFDDIV,
		PPC_ID_SPE_EFDMUL,
		PPC_ID_SPE_EFDNABS,
		PPC_ID_SPE_EFDNEG,
		PPC_ID_SPE_EFDSUB,
		PPC_ID_SPE_EFDTSTEQ,
		PPC_ID_SPE_EFDTSTGT,
		PPC_ID_SPE_EFDTSTLT,
		PPC_ID_SPE_EFSABS,
		PPC_ID_SPE_EFSADD,
		PPC_ID_SPE_EFSCFD,
		PPC_ID_SPE_EFSCFSF,
		PPC_ID_SPE_EFSCFSI,
		PPC_ID_SPE_EFSCFUF,
		PPC_ID_SPE_EFSCFUI,
		PPC_ID_SPE_EFSCMPEQ,
		PPC_ID_SPE_EFSCMPGT,
		PPC_ID_SPE_EFSCMPLT,
		PPC_ID_SPE_EFSCTSF,
		PPC_ID_SPE_EFSCTSI,
		PPC_ID_SPE_EFSCTSIZ,
		PPC_ID_SPE_EFSCTUF,
		PPC_ID_SPE_EFSCTUI,
		PPC_ID_SPE_EFSCTUIZ,
		PPC_ID_SPE_EFSDIV,
		PPC_ID_SPE_EFSMUL,
		PPC_ID_SPE_EFSNABS,
		PPC_ID_SPE_EFSNEG,
		PPC_ID_SPE_EFSSUB,
		PPC_ID_SPE_EFSTSTEQ,
		PPC_ID_SPE_EFSTSTGT,
		PPC_ID_SPE_EFSTSTLT,
		PPC_ID_SPE_EVABS,
		PPC_ID_SPE_EVADDIW,
		PPC_ID_SPE_EVADDSMIAAW,
		PPC_ID_SPE_EVADDSSIAAW,
		PPC_ID_SPE_EVADDUMIAAW,
		PPC_ID_SPE_EVADDUSIAAW,
		PPC_ID_SPE_EVADDW,
		PPC_ID_SPE_EVAND,
		PPC_ID_SPE_EVANDC,
		PPC_ID_SPE_EVCMPEQ,
		PPC_ID_SPE_EVCMPGTS,
		PPC_ID_SPE_EVCMPGTU,
		PPC_ID_SPE_EVCMPLTS,
		PPC_ID_SPE_EVCMPLTU,
		PPC_ID_SPE_EVCNTLSW,
		PPC_ID_SPE_EVCNTLZW,
		PPC_ID_SPE_EVDIVWS,
		PPC_ID_SPE_EVDIVWU,
		PPC_ID_SPE_EVEQV,
		PPC_ID_SPE_EVEXTSB,
		PPC_ID_SPE_EVEXTSH,
		PPC_ID_SPE_EVFSABS,
		PPC_ID_SPE_EVFSADD,
		PPC_ID_SPE_EVFSCFSF,
		PPC_ID_SPE_EVFSCFSI,
		PPC_ID_SPE_EVFSCFUF,
		PPC_ID_SPE_EVFSCFUI,
		PPC_ID_SPE_EVSCFUI,
		PPC_ID_SPE_EVFSCMPEQ,
		PPC_ID_SPE_EVFSCMPGT,
		PPC_ID_SPE_EVFSCMPLT,
		PPC_ID_SPE_EVFSCTSF,
		PPC_ID_SPE_EVFSCTSI,
		PPC_ID_SPE_EVFSCTSIZ,
		PPC_ID_SPE_EVFSCTUF,
		PPC_ID_SPE_EVFSCTUI,
		PPC_ID_SPE_EVFSCTUIZ,
		PPC_ID_SPE_EVFSDIV,
		PPC_ID_SPE_EVFSMUL,
		PPC_ID_SPE_EVFSNABS,
		PPC_ID_SPE_EVFSNEG,
		PPC_ID_SPE_EVFSSUB,
		PPC_ID_SPE_EVFSTSTEQ,
		PPC_ID_SPE_EVFSTSTGT,
		PPC_ID_SPE_EVFSTSTLT,
		PPC_ID_SPE_EVLDD,
		PPC_ID_SPE_EVLDDX,
		PPC_ID_SPE_EVLDH,
		PPC_ID_SPE_EVLDHX,
		PPC_ID_SPE_EVLDW,
		PPC_ID_SPE_EVLDWX,
		PPC_ID_SPE_EVLHHESPLAT,
		PPC_ID_SPE_EVLHHESPLATX,
		PPC_ID_SPE_EVLHHOSSPLAT,
		PPC_ID_SPE_EVLHHOSSPLATX,
		PPC_ID_SPE_EVLHHOUSPLAT,
		PPC_ID_SPE_EVLHHOUSPLATX,
		PPC_ID_SPE_EVLWHE,
		PPC_ID_SPE_EVLWHEX,
		PPC_ID_SPE_EVLWHOS,
		PPC_ID_SPE_EVLWHOSX,
		PPC_ID_SPE_EVLWHOU,
		PPC_ID_SPE_EVLWHOUX,
		PPC_ID_SPE_EVLWHSPLAT,
		PPC_ID_SPE_EVLWHSPLATX,
		PPC_ID_SPE_EVLWWSPLAT,
		PPC_ID_SPE_EVLWWSPLATX,
		PPC_ID_SPE_EVMERGEHI,
		PPC_ID_SPE_EVMERGEHILO,
		PPC_ID_SPE_EVMERGELO,
		PPC_ID_SPE_EVMERGELOHI,
		PPC_ID_SPE_EVMHEGSMFAA,
		PPC_ID_SPE_EVMHEGSMFAN,
		PPC_ID_SPE_EVMHEGSMIAA,
		PPC_ID_SPE_EVMHEGSMIAN,
		PPC_ID_SPE_EVMHEGUMIAA,
		PPC_ID_SPE_EVMHEGUMIAN,
		PPC_ID_SPE_EVMHESMF,
		PPC_ID_SPE_EVMHESMFA,
		PPC_ID_SPE_EVMHESMFAAW,
		PPC_ID_SPE_EVMHESMFANW,
		PPC_ID_SPE_EVMHESMI,
		PPC_ID_SPE_EVMHESMIA,
		PPC_ID_SPE_EVMHESMIAAW,
		PPC_ID_SPE_EVMHESMIANW,
		PPC_ID_SPE_EVMHESSF,
		PPC_ID_SPE_EVMHESSFA,
		PPC_ID_SPE_EVMHESSFAAW,
		PPC_ID_SPE_EVMHESSFANW,
		PPC_ID_SPE_EVMHESSIAAW,
		PPC_ID_SPE_EVMHESSIANW,
		PPC_ID_SPE_EVMHEUMI,
		PPC_ID_SPE_EVMHEUMIA,
		PPC_ID_SPE_EVMHEUMIAAW,
		PPC_ID_SPE_EVMHEUMIANW,
		PPC_ID_SPE_EVMHEUSIAAW,
		PPC_ID_SPE_EVMHEUSIANW,
		PPC_ID_SPE_EVMHOGSMFAA,
		PPC_ID_SPE_EVMHOGSMFAN,
		PPC_ID_SPE_EVMHOGSMIAA,
		PPC_ID_SPE_EVMHOGSMIAN,
		PPC_ID_SPE_EVMHOGUMIAA,
		PPC_ID_SPE_EVMHOGUMIAN,
		PPC_ID_SPE_EVMHOSMF,
		PPC_ID_SPE_EVMHOSMFA,
		PPC_ID_SPE_EVMHOSMFAAW,
		PPC_ID_SPE_EVMHOSMFANW,
		PPC_ID_SPE_EVMHOSMI,
		PPC_ID_SPE_EVMHOSMIA,
		PPC_ID_SPE_EVMHOSMIAAW,
		PPC_ID_SPE_EVMHOSMIANW,
		PPC_ID_SPE_EVMHOSSF,
		PPC_ID_SPE_EVMHOSSFA,
		PPC_ID_SPE_EVMHOSSFAAW,
		PPC_ID_SPE_EVMHOSSFANW,
		PPC_ID_SPE_EVMHOSSIAAW,
		PPC_ID_SPE_EVMHOSSIANW,
		PPC_ID_SPE_EVMHOUMI,
		PPC_ID_SPE_EVMHOUMIA,
		PPC_ID_SPE_EVMHOUMIAAW,
		PPC_ID_SPE_EVMHOUMIANW,
		PPC_ID_SPE_EVMHOUSIAAW,
		PPC_ID_SPE_EVMHOUSIANW,
		PPC_ID_SPE_EVMR,
		PPC_ID_SPE_EVMRA,
		PPC_ID_SPE_EVMWHSMF,
		PPC_ID_SPE_EVMWHSMFA,
		PPC_ID_SPE_EVMWHSMI,
		PPC_ID_SPE_EVMWHSMIA,
		PPC_ID_SPE_EVMWHSSF,
		PPC_ID_SPE_EVMWHSSFA,
		PPC_ID_SPE_EVMWHUMI,
		PPC_ID_SPE_EVMWHUMIA,
		PPC_ID_SPE_EVMWHUSIAAW,
		PPC_ID_SPE_EVMWHUSIANW,
		PPC_ID_SPE_EVMWLSMIAAW,
		PPC_ID_SPE_EVMWLSMIANW,
		PPC_ID_SPE_EVMWLSSIANW,
		PPC_ID_SPE_EVMWLSSIAAW,
		PPC_ID_SPE_EVMWLUMI,
		PPC_ID_SPE_EVMWLUMIA,
		PPC_ID_SPE_EVMWLUMIAAW,
		PPC_ID_SPE_EVMWLUMIANW,
		PPC_ID_SPE_EVMWLUSIAAW,
		PPC_ID_SPE_EVMWLUSIANW,
		PPC_ID_SPE_EVMWSMF,
		PPC_ID_SPE_EVMWSMFA,
		PPC_ID_SPE_EVMWSMFAA,
		PPC_ID_SPE_EVMWSMFAN,
		PPC_ID_SPE_EVMWSMI,
		PPC_ID_SPE_EVMWSMIA,
		PPC_ID_SPE_EVMWSMIAA,
		PPC_ID_SPE_EVMWSMIAN,
		PPC_ID_SPE_EVMWSSF,
		PPC_ID_SPE_EVMWSSFA,
		PPC_ID_SPE_EVMWSSFAA,
		PPC_ID_SPE_EVMWSSFAN,
		PPC_ID_SPE_EVMWUMI,
		PPC_ID_SPE_EVMWUMIA,
		PPC_ID_SPE_EVMWUMIAA,
		PPC_ID_SPE_EVMWUMIAN,
		PPC_ID_SPE_EVNAND,
		PPC_ID_SPE_EVNEG,
		PPC_ID_SPE_EVNOR,
		PPC_ID_SPE_EVNOT,
		PPC_ID_SPE_EVOR,
		PPC_ID_SPE_EVORC,
		PPC_ID_SPE_EVRLW,
		PPC_ID_SPE_EVRLWI,
		PPC_ID_SPE_EVRNDW,
		PPC_ID_SPE_EVSEL,
		PPC_ID_SPE_EVSLW,
		PPC_ID_SPE_EVSLWI,
		PPC_ID_SPE_EVSPLATFI,
		PPC_ID_SPE_EVSPLATI,
		PPC_ID_SPE_EVSRWIS,
		PPC_ID_SPE_EVSRWIU,
		PPC_ID_SPE_EVSRWS,
		PPC_ID_SPE_EVSRWU,
		PPC_ID_SPE_EVSTDD,
		PPC_ID_SPE_EVSTDDX,
		PPC_ID_SPE_EVSTDH,
		PPC_ID_SPE_EVSTDHX,
		PPC_ID_SPE_EVSTDW,
		PPC_ID_SPE_EVSTDWX,
		PPC_ID_SPE_EVSTWHE,
		PPC_ID_SPE_EVSTWHEX,
		PPC_ID_SPE_EVSTWHO,
		PPC_ID_SPE_EVSTWHOX,
		PPC_ID_SPE_EVSTWWE,
		PPC_ID_SPE_EVSTWWEX,
		PPC_ID_SPE_EVSTWWO,
		PPC_ID_SPE_EVSTWWOX,
		PPC_ID_SPE_EVSUBFSMIAAW,
		PPC_ID_SPE_EVSUBFSSIAAW,
		PPC_ID_SPE_EVSUBFUMIAAW,
		PPC_ID_SPE_EVSUBFUSIAAW,
		PPC_ID_SPE_EVSUBFW,
		PPC_ID_SPE_EVSUBIFW,
		PPC_ID_SPE_EVXOR,
	};

#ifndef __cplusplus
	typedef enum InstructionId InstructionId;
#endif

	struct Instruction
	{
		InstructionId id;

		// these acronyms are from the documentation; sometimes they
		// refer to the same bit (like RC and LK)
		struct {
			uint32_t rc: 1;
			uint32_t oe: 1;
			uint32_t lk: 1;
			uint32_t aa: 1;
			uint32_t branchLikelyHint: 2;
			uint32_t round2odd: 1;
			uint32_t inexact: 1;
		} flags;

		size_t numOperands;
		Operand operands[8];
	};

#ifndef __cplusplus
	typedef struct Instruction Instruction;
#endif

	// returns 0 if data_length is too small to tell
	size_t GetInstructionLength(const uint8_t* data, size_t data_length, uint32_t flags);

	bool Decompose32(Instruction* instruction, uint32_t word32, uint64_t address, uint32_t flags);
	void FillBcxOperands(OperandsList* bcx, const Instruction* instruction);
	void FillBcctrxOperands(OperandsList* bcctrx, const Instruction* instruction);
	void FillBclrxOperands(OperandsList* bclrx, const Instruction* instruction);
	const char* GetMnemonic(const Instruction* instruction);
	const char* PowerPCRegisterName(uint32_t regId);
	const char* OperandClassName(uint32_t cls);
	const char* GetCRBitName(uint32_t crbit);

#ifdef __cplusplus
}
#endif


#endif // DECODE_PPC_H
