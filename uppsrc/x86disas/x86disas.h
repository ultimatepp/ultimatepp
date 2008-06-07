#ifndef _x86disas_x86disas_h
#define _x86disas_x86disas_h

#include <Core/Core.h>

/*
 *  x86.c - x86 Decoder Functions
 *
 *  Copyright (c) 2002,2003 Vivek Mohan <vivek@sig9.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

struct x86_TableEntry {
	unsigned int  mnm;	/* instruction mnemonic code */
	unsigned char op1[2];	/* operand 1, code/sz */
	unsigned char op2[2];	/* operand 2, code/sz */
	unsigned char op3[2];	/* operand 3, code/sz */
	unsigned int  prfx;	/* allowed prefixes   */
};

enum {
	OP_NONE,
	OP_A,
	OP_E,
	OP_M,
	OP_G,
	OP_I,
	OP_AL, OP_CL, OP_DL, OP_BL, OP_AH, OP_CH, OP_DH, OP_BH,
	OP_ALr8b, OP_CLr9b, OP_DLr10b, OP_BLr11b, OP_AHr12b, OP_CHr13b, OP_DHr14b, OP_BHr15b,
	OP_AX, OP_CX, OP_DX, OP_BX, OP_SI, OP_DI, OP_SP, OP_BP,
	OP_rAX, OP_rCX, OP_rDX, OP_rBX, OP_rSP, OP_rBP, OP_rSI, OP_rDI,
	OP_rAXr8, OP_rCXr9, OP_rDXr10, OP_rBXr11, OP_rSPr12, OP_rBPr13, OP_rSIr14, OP_rDIr15,
	OP_eAX, OP_eCX, OP_eDX, OP_eBX, OP_eSI, OP_eDI, OP_eSP, OP_eBP,
	OP_ES, OP_CS, OP_DS, OP_SS, OP_FS, OP_GS,
	OP_ST0, OP_ST1, OP_ST2, OP_ST3, OP_ST4, OP_ST5, OP_ST6, OP_ST7,
	OP_J,
	OP_S,
	OP_O,
	OP_I1, OP_I3,
	OP_V,
	OP_W,
	OP_Q,
	OP_P,
	OP_R,
	OP_C,
	OP_D,
	OP_VR,
	OP_PR
};

/* operand sizes enumerated */

enum {
	SZ_b = 1, SZ_w, SZ_z, SZ_v, SZ_vw, SZ_d, SZ_q, SZ_p, SZ_wp, SZ_dp, SZ_sb,
	SZ_jb, SZ_jw, SZ_jd, SZ_x, SZ_m
};

/* WARNING - Make sure the order of enumeration is same as order
   of operand script declarations in syntax.c
 */

enum {

 /* 16 bit addressing modes */
 M_BX_SI = 0, M_BX_DI, M_BP_SI, M_BP_DI, M_SI, M_DI, M_OFF16, M_BX,
 M_BX_SI_OFF8, M_BX_DI_OFF8, M_BP_SI_OFF8, M_BP_DI_OFF8,
 M_SI_OFF8, M_DI_OFF8, M_BP_OFF8, M_BX_OFF8,
 M_BX_SI_OFF16, M_BX_DI_OFF16, M_BP_SI_OFF16, M_BP_DI_OFF16,
 M_SI_OFF16, M_DI_OFF16, M_BP_OFF16, M_BX_OFF16,

 /* 32 bit addressing modes */
 M_EAX, M_ECX, M_EDX, M_EBX, M_SIB, M_OFF32, M_ESI, M_EDI,
 M_EAX_OFF8, M_ECX_OFF8, M_EDX_OFF8, M_EBX_OFF8, M_SIB_OFF8,
 M_EBP_OFF8, M_ESI_OFF8, M_EDI_OFF8,
 M_EAX_OFF32, M_ECX_OFF32, M_EDX_OFF32, M_EBX_OFF32, M_SIB_OFF32,
 M_EBP_OFF32, M_ESI_OFF32, M_EDI_OFF32,
 M_SID,

 /* (amd)64 bit addressing modes */
 M_RAX, M_RCX, M_RDX, M_RBX, M_RSIB, M_ROFF32, M_RSI, M_RDI,
 M_R8, M_R9, M_R10, M_R11, M_R12, M_R13, M_R14, M_R15,
 M_RAX_OFF8, M_RCX_OFF8, M_RDX_OFF8, M_RBX_OFF8, M_RSIB_OFF8,
 M_RBP_OFF8, M_RSI_OFF8, M_RDI_OFF8,
 M_R8_OFF8, M_R9_OFF8, M_R10_OFF8, M_R11_OFF8, M_R12_OFF8,
 M_R13_OFF8, M_R14_OFF8, M_R15_OFF8,
 M_RAX_OFF32, M_RCX_OFF32, M_RDX_OFF32, M_RBX_OFF32, M_RSIB_OFF32,
 M_RBP_OFF32, M_RSI_OFF32, M_RDI_OFF32,
 M_R8_OFF32, M_R9_OFF32, M_R10_OFF32, M_R11_OFF32, M_R12_OFF32,
 M_R13_OFF32, M_R14_OFF32, M_R15_OFF32,
 M_RSID,

 /* mem offset */
 OFFSET16, OFFSET32, OFFSET64,

 /* 8 bit GPRs */
 R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH, R_SPL, R_BPL, R_SIL, R_DIL,
 R_R8B, R_R9B, R_R10B, R_R11B, R_R12B, R_R13B, R_R14B, R_R15B,
 /* 16 bit GPRs */
 R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI,
 R_R8W, R_R9W, R_R10W, R_R11W, R_R12W, R_R13W, R_R14W, R_R15W,
 /* 32 bit GPRs */
 R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI,
 R_R8D, R_R9D, R_R10D, R_R11D, R_R12D, R_R13D, R_R14D, R_R15D,
 /* 64 bit GPRs */
 R_RAX, R_RCX, R_RDX, R_RBX, R_RSP, R_RBP, R_RSI, R_RDI,
 R_R8, R_R9, R_R10, R_R11, R_R12, R_R13, R_R14, R_R15,

 /* segment registers */
 R_ES, R_CS, R_SS, R_DS, R_FS, R_GS,
 /* control registers*/
 R_CR0, R_CR1, R_CR2, R_CR3, R_CR4, R_CR5_INV, R_CR6_INV, R_CR7_INV,
 /* debug registers */
 R_DR0, R_DR1, R_DR2, R_DR3, R_DR4_INV, R_DR5_INV, R_DR6, R_DR7,
 /* mmx registers */
 R_MM0, R_MM1, R_MM2, R_MM3, R_MM4, R_MM5, R_MM6, R_MM7,
 /* x87 registers */
 R_ST0, R_ST1, R_ST2, R_ST3, R_ST4, R_ST5, R_ST6, R_ST7,
 /* extended multimedia registers */
 R_XMM0, R_XMM1, R_XMM2, R_XMM3, R_XMM4, R_XMM5, R_XMM6, R_XMM7,
 R_XMM8, R_XMM9, R_XMM10, R_XMM11, R_XMM12, R_XMM13, R_XMM14, R_XMM15,

 I_BYTE, I_WORD, I_DWRD,
 J_IMM,
 P_32, P_48,
 PRINT_I,
 STRING
};

extern char *mnemonics[];

#define DO(x)      I##x,

enum {
#include "mnemonics.i"
};

#undef DO

class X86Disas {
	struct x86_InsnOperand {
		unsigned char size;		/* operand size */
		signed type;			/* operand type */
		unsigned char *lvalp;
		unsigned long lval[2];		/* literal values */
		struct	{			/* SIB decoded */
			unsigned char base;	/* - Base  */
			unsigned char indx;	/* - Index */
			unsigned char scal;	/* - Scale */
		} sib;
	};

	typedef struct x86_Insn {
		unsigned mnm;		/* the mnemonic code */
		unsigned char seg;	/* segment override */
		unsigned char a32;	/* address 32 prefix */
		unsigned char o32;	/* operand 32 prefix */
		x86_InsnOperand op[3];	/* 3 operands for instruction */
	};

	/* operand types enumerated */

	unsigned char G_dis_mode;	/* disassembly mode, (16/32/64) */
	unsigned char G_opr_mode;	/* operand mode, (16/32) */
	unsigned char G_adr_mode;	/* address mode, (16/32) */
	unsigned char G_opr_prfx;	/* 0x66 prefix flag */
	unsigned char G_adr_prfx;	/* 0x67 prefix flag */
	unsigned char G_lock_prfx;	/* 0xF0 (lock) prefix flag */
	unsigned char G_rep_prfx;	/* 0xF3 (rep) prefix flag */
	unsigned char G_repe_prfx;	/* 0xF3 (repe) prefix flag */
	unsigned char G_repne_prfx;	/* 0xF2 (repne) prefix flag */
	unsigned long G_prg_ctr;	/* program counter */
	unsigned char G_err;	/* disassembly abort flag */
	unsigned char G_rex;	/* REX prefix for 64bit mode */
	unsigned char G_suff;  /* instruction suffix flag */
	unsigned char G_def_64;	/* default mode 64 flag */
	unsigned long G_temp;  /* a temp global var */

	// from opcmap.cpp
	unsigned       Get3DNowInsnMnemonic(unsigned char suffix);
	unsigned char *GetInsnEntry(unsigned char *src, x86_TableEntry **te);

	// from x86.cpp
	unsigned char  ResolveOpSize(unsigned char sz);
	unsigned char* DecodeA( unsigned char sz, unsigned char *src, x86_InsnOperand *op);
	unsigned int   DecodeGPR(unsigned char sz, unsigned char rm);
	unsigned int   ResolveGPR64 (unsigned int gpr_op);
	unsigned int   ResolveGPR32 (unsigned int gpr_op);
	unsigned int   ResolveReg(unsigned type, unsigned rm);
	unsigned char* DecodeO( unsigned char sz, unsigned char *src, x86_InsnOperand *op);
	unsigned char* DecodeImm(unsigned char sz, unsigned char *src, x86_InsnOperand *op);
	unsigned char* DecodeModRM(
		unsigned char *src,	/* source */
		x86_InsnOperand *op,	/* ptr to instruction operand struct */
		unsigned char sz,	/* operand sz */
		unsigned char rm_type,	/* the type of operand specified by rm, if mod = 11b */
		x86_InsnOperand *opreg,	/* ptr to instruction operand struct for reg operand */
		unsigned char reg_sz,	/* the size of operand specified by reg */
		unsigned char reg_type	/* the type of operand specified by reg */
	);
	unsigned char* DisasmOperands( unsigned char *src, x86_Insn *ip, x86_TableEntry *insn);
	void           Clear(x86_Insn *ip);
	unsigned char* DisasmInsn (unsigned char *src, x86_Insn *ip);

	// from syntax.cpp
	unsigned long cast;

	void print_opr_cast(unsigned char sz);
	void print_rel_cast(unsigned char sz);
	void print_si(x86_Insn *ip, unsigned n);
	void print_sib(x86_Insn *ip, unsigned n);
	void print_rsib(x86_Insn *ip, unsigned n);
	void print_rsi(x86_Insn *ip, unsigned n);
	void opscript(x86_Insn *ip, unsigned n, char *script);
	void Disasm(x86_Insn *ip);

public:
	String mnemonic;
	String operands;
	bool   iscall;
	int    size;

	X86Disas(dword ip, byte *data);
};

#endif
