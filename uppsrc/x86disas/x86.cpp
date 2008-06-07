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

#include "x86disas.h"

/* the max nr of prefixes */
#define MAX_PREFIXES	15

/* register types */
#define T_NONE	0
#define T_GPR	1
#define T_MMX	2
#define T_CRG	3
#define T_DBG	4
#define T_SEG	5
#define T_XMM	6

/* REX, bit manip */
#define REX_W(r) ((0xF & (r))  >> 3)
#define REX_R(r) ((0x7 & (r))  >> 2)
#define REX_X(r) ((0x3 & (r))  >> 1)
#define REX_B(r) ((0x1 & (r))  >> 0)
#define REX_MASK(n) (0xF & (n >> 16))

/* SIB, bit manip */
#define SIB_S(b) ((b) >> 6)
#define SIB_I(b) (((b) >> 3) & 7)
#define SIB_B(b) ((b)  & 7)

/* MODRM, bit manip */
#define MODRM_REG(b) (((b)>>3) & 7)
#define MODRM_NNN(b) (((b)>>3) & 7)
#define MODRM_MOD(b) (((b)>>6) & 3)
#define MODRM_RM(b)   ((b) &7)

#define DEF64(n) (((n) >> 3) & 1)
#define INV64(n) (((n) >> 4) & 1)
#define SUFF(n) (((n) >> 5) & 1)
#define O32(n) (((n) >> 1) & 1)
#define A32(n) ((n) & 1)

/* literal values */
#define LVAL8(lvalp, src)  do { lvalp = src; src++;    } while(0)
#define LVAL16(lvalp, src) do { lvalp = src; src += 2; } while(0)
#define LVAL32(lvalp, src) do { lvalp = src; src += 4; } while(0)
#define LVAL64(lvalp, src) do { lvalp = src; src += 8; } while(0)

/* ResolveOpSize(..)
   resolves the size of operand depending on the current
   disassembly mode and the prefixes.
 */
unsigned char X86Disas::ResolveOpSize(unsigned char sz)
{
  if (sz == SZ_vw && G_dis_mode == 64)
	return (G_opr_prfx) ? SZ_w : SZ_q;
  else
  if (sz == SZ_v  && G_dis_mode == 64) {
	if (REX_W(G_rex))
		return SZ_q;
	else	return (G_opr_prfx) ? SZ_w : SZ_d;
  }
  else if (sz == SZ_v || sz == SZ_vw || sz == SZ_z)
	return (G_opr_mode == 16) ? SZ_w : SZ_d;
  else if (sz == SZ_p)
	return (G_opr_mode == 16) ? SZ_wp : SZ_dp;
  else return sz;
}

/* DecodeA(..)
   Decodes operands of the type seg:offset
 */
unsigned char* X86Disas::DecodeA( unsigned char sz, unsigned char *src, x86_InsnOperand *op)
{
  /* seg16:off16 */
  if (G_opr_mode == 16) {
	op->type = P_32;
	op->size = SZ_w;
	op->lvalp= src;
	src += 4;	/* 32 bits */
  }
  /* seg16:off32 */
  else {
	op->type = P_48;
	op->size = SZ_d;
	op->lvalp= src;
	src += 6;	/* 16 : 32 = 48 bits */
  }
  return src;
}

/* returns decoded GPR */
unsigned int X86Disas::DecodeGPR(unsigned char sz, unsigned char rm)
{
  /* Mode dependent, operand prefix independent, size
     specifier: SZ_m. In 64 bits mode, the GPR is 64bits
     in all other cases its 32 bits.
   */
  if (sz == SZ_m)
	return (G_dis_mode == 64) ? R_RAX + rm : R_EAX + rm;

  /* byte sized operand, in 64 bit mode. In this mode, 16 different
     8 bit registers can be chosen.
   */
  if (sz == SZ_b && G_dis_mode == 64) {
	/* is it > bl ? if so, then the next register will
	   be sil instead of ah (in 64 bits)
	*/
	if (rm >= 4)
		return R_SPL + (rm-4);
	else return R_AL + rm;
  }
  else if (sz == SZ_b)
	return R_AL + rm;

  /* word/dword/qword sized operands, in 64 bit mode. In this mode,
     16 different 16 bit registers can be chosen.
  */
  else if ((sz == SZ_x || sz == SZ_v) && G_dis_mode == 64) {
	/* if REX.W is specified then select a quad register */
	if (REX_W(G_rex))
		return R_RAX + rm;
	/* in 64 bit mode, 0x66 means a 16 bit register */
	else if (G_opr_prfx)
		return R_AX  + rm;
	/* default, 32 bit register */
	else	return R_EAX + rm;
  }
  else if ((sz == SZ_d || sz == SZ_x) || (sz == SZ_v && G_opr_mode == 32))
	/* if mode is 32its then select 32bit registers */
	return R_EAX + rm;
  else if ((sz == SZ_w) || (sz == SZ_v && G_opr_mode == 16))
	/* if mode is 16bits then select 16bit registers */
	return R_AX + rm;
  else	return 0;
}

/* ResolveGPR64 (...)
   Resolves any ambiguity in register(including 64bit) selection.
 */
unsigned int X86Disas::ResolveGPR64 (unsigned int gpr_op)
{
  /* in 64, the defualt register is 32bits, but if a REX
     prefix is specified, then it its a 64 bits register
   */
  if (G_dis_mode == 64) {

	/* extensible 64 bit gprs */
	if (gpr_op >= OP_rAXr8 && gpr_op <= OP_rDIr15)
		gpr_op = gpr_op | (REX_B(G_rex) << 3);

	/* is it a default 64 bit instruction ? */
	if (G_def_64)
		return (gpr_op-OP_rAX) + R_RAX;
	else
	/* is W bit of REX = 1 ? */
	if (REX_W(G_rex))
		/* its a 64 bits reg */
		return (gpr_op-OP_rAX) + R_RAX;
	else
	if (G_opr_prfx)
		/* if prefixed by 0x66 */
		return (gpr_op-OP_rAX) + R_AX;
		/* else, its a 32 bits reg */
	else	return (gpr_op-OP_rAX) + R_EAX;
  }
  else 	{
  	/* is operand mode 16 ? */
	if (G_opr_mode == 16) {
		/* extensible 64 bit gprs */
		if (gpr_op >= OP_rAXr8 && gpr_op <= OP_rDIr15)
			return (gpr_op-OP_rAXr8) + R_AX;
		/* if so, return a 16 bit reg */
		return (gpr_op-OP_rAX) + R_AX;
	}
		/* else, return a 32 bit reg */
	else {
		/* extensible 64 bit gprs */
		if (gpr_op >= OP_rAXr8 && gpr_op <= OP_rDIr15)
			return (gpr_op-OP_rAXr8) + R_EAX;
		return (gpr_op-OP_rAX) + R_EAX;
	}
 }
}

/* ResolveGPR32 (...)
   Resolves any ambiguity in register(excluding 64bit) selection.
 */
unsigned int X86Disas::ResolveGPR32 (unsigned int gpr_op)
{
  /* in 64, the defualt register is 32bits, but if a REX
     prefix is specified, then it its a 64 bits register
   */
  if (G_dis_mode == 64) {
	if (G_opr_prfx)
		/* if prefixed by 0x66 */
		return (gpr_op-OP_eAX) + R_AX;
		/* else, its a 32 bits reg */
	else	return (gpr_op-OP_eAX) + R_EAX;
  }
  else 	{
  	/* is operand mode 16 ? */
	if (G_opr_mode == 16)
		/* if so, return a 16 bit reg */
		return (gpr_op-OP_eAX) + R_AX;
		/* else, return a 32 bit reg */
	else	return (gpr_op-OP_eAX) + R_EAX;
 }
}

/* ResolveReg(...)
   Auxiliary function for Resolving the register type
   and returning the corresponding reg vaue.
 */
unsigned int X86Disas::ResolveReg(unsigned type, unsigned rm) {
  switch (type) {
	case T_MMX :	/* mmx register */
		return R_MM0 + rm;
	case T_XMM :	/*xmm register */
		return R_XMM0 + rm;
	case T_CRG :	/* control register */
		return R_CR0 + rm;
	case T_DBG :	/* debug register */
		return R_DR0 + rm;
	case T_NONE:
	default:
		return 0;
  }
}


/* DecodeImm(...)
   Decodes Immediate values
 */
unsigned char* X86Disas::DecodeImm(unsigned char sz, unsigned char *src, x86_InsnOperand *op)
{
  /* byte sized immediate */
  if (sz == SZ_b || sz == SZ_sb) {
	op->type = I_BYTE;
	LVAL8(op->lvalp, src);
  }

  /* if disassembler mode is 64 bits, then a 0x66 prefix means
     a 16 bit immediate value for SZ_v */
  if(sz == SZ_z && G_dis_mode == 64) {
	/* if prefixed by 0x66, then a 16bit immediate */
	if (G_opr_prfx) {
		op->type = I_WORD;
		LVAL16(op->lvalp, src);
	}
	/* else its dword */
	else {
		op->type = I_DWRD;
		LVAL32(op->lvalp, src);
	}
  }
  else /* if SZ_w or in 16bits operand mode */
  if (sz == SZ_w || (sz == SZ_z && G_opr_mode == 16)) {
	op->type = I_WORD;
	LVAL16(op->lvalp, src);
  }
  else /* if SZ_d or in 32bits operand mode */
  /*if (sz == SZ_d || (sz == SZ_z && G_opr_mode == 32))*/ {
	op->type = I_DWRD;
	LVAL32(op->lvalp, src);
  }

  /* get size of operand */
  op->size = ResolveOpSize(sz);
  return src;
}

/* DecodeModRM(...)
   Decodes ModRM
 */
unsigned char* X86Disas::DecodeModRM(
	unsigned char *src,	/* source */
	x86_InsnOperand *op,	/* ptr to instruction operand struct */
	unsigned char sz,	/* operand sz */
	unsigned char rm_type,	/* the type of operand specified by rm, if mod = 11b */
	x86_InsnOperand *opreg,	/* ptr to instruction operand struct for reg operand */
	unsigned char reg_sz,	/* the size of operand specified by reg */
	unsigned char reg_type	/* the type of operand specified by reg */
	)
{
  /* get mod, r/m and reg fields */
  unsigned char mod = MODRM_MOD(*src);
  unsigned char rm  = (REX_B(G_rex) << 3) | MODRM_RM(*src);
  unsigned char reg = (REX_R(G_rex) << 3) | MODRM_REG(*src);

  /* move to next byte in source */
  ++src;

  /* if mod is 11b, then the r_m field specifies a general
     purpose register or mmx/sse/control/debug register
   */
  if (mod == 3) {
	if (rm_type == 	T_GPR)	/* general purpose register */
		op->type = DecodeGPR(sz, rm);
	else	op->type = ResolveReg(rm_type, rm);
  }
  else {
	/* if in 64 bit mode, only 32/64 bit reg addressing is allowed. */
	if (G_dis_mode == 64)	{
		/* if the lower 3 bits of rm = 4 then the next byte is an
		   SIB byte */
		if ((rm & 7) == 4) {
			op->type = (mod * 8) + M_RAX + rm;
			op->sib.scal = SIB_S(*src);
			op->sib.indx = SIB_I(*src) + REX_X(G_rex);
			op->sib.base = SIB_B(*src) + REX_B(G_rex);
			/* if lower 3 bits of base = 5, then its scale * index */
			if ((op->sib.base & 7) == 5 && op->type == M_RSIB)
				op->type = M_RSID;
			++src;
		}
		else
		/* An address size prefix denotes the use of 32 bit registers */
		if (G_adr_prfx)
			op->type = (mod * 8) + M_EAX + rm;
		/* By default its 64 bit registers */
		else	op->type = (mod * 16) + M_RAX + rm;
	} else {
		/* 16 bit addressing */
		if (G_adr_mode == 16) {
			op->type = (mod * 8) + M_BX_SI + rm;
		}
		/* 32 bit addressing */
		else {
			op->type = (mod * 8) + M_EAX + rm;
			/* if the lower 3 bits of rm = 4 then the next byte is an
			   SIB byte */
			if (rm == 4) {
				op->sib.scal = SIB_S(*src);
				op->sib.indx = SIB_I(*src);
				op->sib.base = SIB_B(*src);
				/* if lower 3 bits of base = 5, then its scale * index */
				if (op->sib.base == 5 && op->type == M_SIB)
					op->type = M_SID;
				++src;
			}
		}
	}
  }

  /* 8 bit displacements */
  if ((M_BX_SI_OFF8 <= op->type && op->type <= M_BX_OFF8) ||
      (M_EAX_OFF8 <= op->type && op->type <= M_EDI_OFF8)  ||
      (M_RAX_OFF8 <= op->type && op->type <= M_R15_OFF8) )
	LVAL8(op->lvalp, src);
  else
  /* 16 bit displacements */
  if ((M_BX_SI_OFF16 <= op->type && op->type <= M_BX_OFF16) ||
      (M_OFF16 == op->type) )
	LVAL16(op->lvalp, src);
  else
  /* 32 bit displacements */
  if ((M_EAX_OFF32 <= op->type && op->type <= M_EDI_OFF32) ||
      (M_RAX_OFF32 <= op->type && op->type <= M_R15_OFF32) ||
      (M_OFF32 == op->type) ||
      (M_SID   == op->type) ||
      (M_RSID  == op->type) )
	LVAL32(op->lvalp, src);

  if (opreg) {
	/* resolve register encoded in reg field */
	if (reg_type == T_GPR) /* general purpose register */
		opreg->type = DecodeGPR(reg_sz, reg);
	else	opreg->type = ResolveReg(reg_type, reg);
  }

  /* determine the operand sizes */
  op->size = ResolveOpSize(sz);
  if (opreg) opreg->size = ResolveOpSize(reg_sz);

  return src;
}

/* DecodeO(..)
   Decodes offset
 */
unsigned char* X86Disas::DecodeO( unsigned char sz, unsigned char *src, x86_InsnOperand *op)
{
  /* In 64bits mode */
  if (G_dis_mode == 64) {
	/* if 0x67, the offest is 32bits long */
	if (G_adr_prfx) {
		LVAL32(op->lvalp, src);
		op->type = OFFSET32;

	}
	/* else its 64bits long */
	else {
		LVAL64(op->lvalp, src);
		op->type = OFFSET64;
	}
  }
  else
  /* if the addressing mode is 16bits, so is offset */
  if (G_adr_mode == 16) {
	LVAL16(op->lvalp, src);
	op->type = OFFSET16;
  }
  /* if the addressing mode is 32bits, so is offset */
  else {
	LVAL32(op->lvalp, src);
	op->type = OFFSET32;
  }

  op->size = ResolveOpSize(sz);
  return src;
}


/*
 DisasmOperands(...)
  Arguments
  	src  -> source buffer ptr
  	ip   -> instruction pointer
  	insn -> instruction table entry pointer

*/
unsigned char* X86Disas::DisasmOperands( unsigned char *src, x86_Insn *ip, x86_TableEntry *insn)
{

/* Some macros for clarity of code */
/* TE_OP - Table Entry, Operand Types*/
#define TE_OP1T insn->op1[0]
#define TE_OP2T insn->op2[0]
#define TE_OP3T insn->op3[0]
#define TE_OP1S insn->op1[1]
#define TE_OP2S insn->op2[1]
#define TE_OP3S insn->op3[1]
/* DI_OP - Disassembled Instruction Pointer, Operand */
#define DI_OP1  ip->op[0]
#define DI_OP2  ip->op[1]
#define DI_OP3  ip->op[2]
#define DI_OP1T ip->op[0].type
#define DI_OP2T ip->op[1].type
#define DI_OP3T ip->op[2].type
#define DI_OP1S ip->op[0].size
#define DI_OP2S ip->op[1].size
#define DI_OP3S ip->op[2].size

  switch(TE_OP1T) {
	case OP_A :
		src = DecodeA(TE_OP1S, src, &(DI_OP1));
		break;
	/* M[b] */
	case OP_M :
		if (MODRM_RM(*src) == 3)
			G_err = 1;
	/* E, G/P/V/I/CL/1 */
	case OP_E :
		if (TE_OP2T == OP_G)	/* E, G */
			src = DecodeModRM(src, &(DI_OP1), TE_OP1S, T_GPR, &(DI_OP2), TE_OP2S, T_GPR);
		else
		if (TE_OP2T == OP_P)	/* E, P */
			src = DecodeModRM(src, &(DI_OP1), TE_OP1S, T_GPR, &(DI_OP2), TE_OP2S, T_MMX);
		else
		if (TE_OP2T == OP_V)	/* E, V */
			src = DecodeModRM(src, &(DI_OP1), TE_OP1S, T_GPR, &(DI_OP2), TE_OP2S, T_XMM);
		else {	/* E */
			src = DecodeModRM(src, &(DI_OP1), TE_OP1S, T_GPR, NULL, 0, T_NONE);
			if (TE_OP2T == OP_CL) {	/* E, CL */
				DI_OP2T = R_CL;
				DI_OP2S = SZ_b;
			}
			else
			if (TE_OP2T == OP_I1) {	/* E, 1 */
				DI_OP2T = PRINT_I;
				G_temp = 1;
				DI_OP2.lvalp = (unsigned char*) (&G_temp);
			}
			else
			if (TE_OP2T == OP_I)	/* E, I */
				src = DecodeImm(TE_OP2S, src, &(DI_OP2));
		}
		break;

	/* G, E/PR[,I]/VR */
	case OP_G :
		if (TE_OP2T == OP_E)	/* G, E */
			src = DecodeModRM(src, &(DI_OP2), TE_OP2S, T_GPR, &(DI_OP1), TE_OP1S, T_GPR);
		else
		if (TE_OP2T == OP_PR) {	/* G, PR */
			/* the mod field ought to be zero */
			if (MODRM_RM(*src) != 3)
				G_err = 1;
			src = DecodeModRM(src, &(DI_OP2), TE_OP2S, T_MMX, &(DI_OP1), TE_OP1S, T_GPR);
			if (TE_OP3T == OP_I) /* G, PR, I */
				src = DecodeImm(TE_OP3S, src, &(DI_OP3));
		}
		else
		if (TE_OP2T == OP_VR) {	/* G, VR */
			/* the mod field ought to be zero */
			if (MODRM_RM(*src) != 3)
				G_err = 1;
			src = DecodeModRM(src, &(DI_OP2), TE_OP2S, T_XMM, &(DI_OP1), TE_OP1S, T_GPR);
		}
		break;

	/* AL..BH, I/O/DX] */
	case OP_AL : case OP_CL : case OP_DL : case OP_BL :
	case OP_AH : case OP_CH : case OP_DH : case OP_BH :
		DI_OP1T = R_AL + (TE_OP1T - OP_AL);
		DI_OP1S = SZ_b;
		if (TE_OP2T == OP_I)	/* AL..BH, Ib */
			src = DecodeImm(TE_OP2S, src, &(DI_OP2));
		else
		if (TE_OP2T == OP_DX)	/* AL..BH, DX */
			DI_OP2T = R_DX;
		else
		if (TE_OP2T == OP_O)	/* AL..BH, O */
			src = DecodeO(TE_OP2S, src, &(DI_OP2));
		break;

	/* rAX[r8]..rDI[r15], I/rAX..rDI/O */
	case OP_rAXr8 : case OP_rCXr9 : case OP_rDXr10 : case OP_rBXr11 :
	case OP_rSPr12: case OP_rBPr13: case OP_rSIr14 : case OP_rDIr15 :
	case OP_rAX : case OP_rCX : case OP_rDX : case OP_rBX :
	case OP_rSP : case OP_rBP : case OP_rSI : case OP_rDI :
		DI_OP1T = ResolveGPR64(TE_OP1T);
		if (TE_OP2T == OP_I)	/* rAX[r8]..rDI[r15], I */
			src = DecodeImm(TE_OP2S, src, &(DI_OP2));
		else	/* rAX[r8]..rDI[r15], rAX..rDI */
		if (TE_OP2T >= OP_rAX && TE_OP2T <= OP_rDI)
			DI_OP2T = ResolveGPR64(TE_OP2T);
		else
		if (TE_OP2T == OP_O)	/* rAX[r8]..rDI[r15], O */
			src = DecodeO( insn->op2[1], src, &(DI_OP2));
		break;

	/* AL[r8b]..BH[r15b], I */
	case OP_ALr8b : case OP_CLr9b : case OP_DLr10b : case OP_BLr11b :
	case OP_AHr12b: case OP_CHr13b: case OP_DHr14b : case OP_BHr15b :
	{
		unsigned long gpr = TE_OP1T;
		/* extensible 8 bit gprs */
		if (TE_OP1T >= OP_ALr8b && TE_OP1T <= OP_BHr15b)
			gpr = gpr | (REX_B(G_rex) << 3);
		DI_OP1T = (gpr - OP_AL) + R_AL;
		if (TE_OP2T == OP_I)	/* AL[r8b]..BH[r15b], I */
			src = DecodeImm(TE_OP2S, src, &(DI_OP2));
		break;
	}

	/* eAX..eDX, DX/I */
	case OP_eAX : case OP_eCX : case OP_eDX : case OP_eBX :
	case OP_eSP : case OP_eBP : case OP_eSI : case OP_eDI :
		DI_OP1T = ResolveGPR32(TE_OP1T);
		if (TE_OP2T == OP_DX)	/* eAX..eDX, DX */
			DI_OP2T = R_DX;
		else
		if (TE_OP2T == OP_I)	/* eAX..eDX, I */
			src = DecodeImm(TE_OP2S, src, &(DI_OP2));
		break;

	/* ES..GS */
	case OP_ES : case OP_CS : case OP_DS :
	case OP_SS : case OP_FS : case OP_GS :
		/* in 64bits mode, only fs and gs are allowed */
		if (G_dis_mode == 64)
			if (TE_OP1T != OP_FS && TE_OP1T != OP_GS)
				G_err = 1;
		DI_OP1T = (TE_OP1T - OP_ES) + R_ES;
		break;

	/* J */
	case OP_J :
		src =  DecodeImm( TE_OP1S, src, &(DI_OP1));
		DI_OP1T = J_IMM;
		switch(DI_OP1S) {
			case SZ_b : DI_OP1S = SZ_jb; break;
			case SZ_w : DI_OP1S = SZ_jw; break;
			case SZ_d : DI_OP1S = SZ_jd; break;
		}
		break ;

	/* P, Q/W/E[,I] */
	case OP_P :
		if (TE_OP2T == OP_Q)	/* P, Q */
			src = DecodeModRM (src, &(DI_OP2), TE_OP2S, T_MMX, &(DI_OP1), TE_OP1S, T_MMX);
		else
		if (TE_OP2T == OP_W)	/* P, W */
			src = DecodeModRM(src, &(DI_OP2), TE_OP2S, T_XMM, &(DI_OP1), TE_OP1S, T_MMX);
		else
		if (TE_OP2T == OP_E) {	/* P, E */
			src = DecodeModRM(src, &(DI_OP2), TE_OP2S, T_GPR, &(DI_OP1), TE_OP1S, T_MMX);
			if (TE_OP3T == OP_I)	/* P, E, I */
				src = DecodeImm(TE_OP3S, src, &(DI_OP3));
		}
		break;

	/* R, C/D */
	case OP_R :
		if (TE_OP2T == OP_C)	/* R, C */
			src = DecodeModRM(src, &(DI_OP1), TE_OP1S, T_GPR, &(DI_OP2), TE_OP2S, T_CRG);
		else
		if (TE_OP2T == OP_D)	/* R, D */
			src = DecodeModRM(src, &(DI_OP1), TE_OP1S, T_GPR, &(DI_OP2), TE_OP2S, T_DBG);
		break;

	/* C, R */
	case OP_C :
		src = DecodeModRM (src, &(DI_OP2), TE_OP2S, T_GPR, &(DI_OP1), TE_OP1S, T_CRG);
		break;

	/* D, R */
	case OP_D :
		src = DecodeModRM (src, &(DI_OP2), TE_OP2S, T_GPR, &(DI_OP1), TE_OP1S, T_DBG);
		break;

	/* Q, P */
	case OP_Q :
		src = DecodeModRM (src, &(DI_OP1), TE_OP1S, T_MMX, &(DI_OP2), TE_OP2S, T_MMX);
		break;

	/* W, V */
	case OP_W :
		src = DecodeModRM (src, &(DI_OP1), TE_OP1S, T_XMM, &(DI_OP2), TE_OP2S, T_XMM);
		break;

	/* V, W[,I]/Q */
	case OP_V :
		if (TE_OP2T == OP_W) {	/* V, W */
			src = DecodeModRM (src, &(DI_OP2), TE_OP2S, T_XMM, &(DI_OP1), TE_OP1S, T_XMM);
			if (TE_OP3T == OP_I)	/* V, W, I */
				src = DecodeImm(TE_OP3S, src, &(DI_OP3));
		}
		else
		if (TE_OP2T == OP_Q)	/* V, Q */
			src = DecodeModRM (src, &(DI_OP2), TE_OP2S, T_MMX, &(DI_OP1), TE_OP1S, T_XMM);
		break;

	/* DX, eAX/AL */
	case OP_DX :
		DI_OP1T = R_DX;
		if (TE_OP2T == OP_eAX)	/* DX, eAX */
			DI_OP2T = ResolveGPR32(TE_OP2T);
		else
		if (TE_OP2T == OP_AL)	/* DX, AL */
			DI_OP2T = R_AL;
		break;

	/* I, I/AL/eAX */
	case OP_I :
		src = DecodeImm( TE_OP1S, src, &(DI_OP1));
		if (TE_OP2T == OP_I)	/* I, I */
			src = DecodeImm( TE_OP2S, src, &(DI_OP2));
		else
		if (TE_OP2T == OP_AL)	/* I, AL */
			DI_OP2T = R_AL;
		else
		if (TE_OP2T == OP_eAX)	/* I, eAX */
			DI_OP2T = ResolveGPR32(TE_OP2T);
		break;

	/* O, AL/eAX */
	case OP_O :
		src = DecodeO(TE_OP1S, src, &(DI_OP1));
		if (TE_OP2T == OP_AL)	/* O, AL */
			DI_OP2T = R_AL;
		else
		if (TE_OP2T == OP_eAX)	/* O, eAX */
			DI_OP2T = ResolveGPR32(TE_OP2T);
		break;

	/* 3 */
	case OP_I3 :
		DI_OP2T = PRINT_I;
		G_temp = 3;
		DI_OP2.lvalp = (unsigned char*)(&G_temp);
		break;
	case OP_ST0 : case OP_ST1 : case OP_ST2 : case OP_ST3 :
	case OP_ST4 : case OP_ST5 : case OP_ST6 : case OP_ST7 :
		DI_OP1T = (TE_OP1T-OP_ST0)  + R_ST0;
		if (TE_OP2T >= OP_ST0 && TE_OP2T <= OP_ST7)
			DI_OP2T = (TE_OP2T-OP_ST0) + R_ST0;
		break;

	/* none */
	default :
		DI_OP1T = DI_OP2T =
		DI_OP3T = -1;
  }
  return src;

/* undefine macros, their services are no
   longer required
 */
#undef TE_OP1T
#undef TE_OP2T
#undef TE_OP3T
#undef TE_OP1S
#undef TE_OP2S
#undef TE_OP3S
#undef DI_OP1
#undef DI_OP2
#undef DI_OP3
#undef DI_OP1T
#undef DI_OP2T
#undef DI_OP3T
#undef DI_OP1S
#undef DI_OP2S
#undef DI_OP3S

}

/* clear instruction pointer */
void X86Disas::Clear(x86_Insn *ip)
{
  ip->mnm         = 0;
  ip->seg         = 0;
  ip->a32         = 0;
  ip->o32         = 0;
  ip->op[0].size  = 0;
  ip->op[1].size  = 0;
  ip->op[2].size  = 0;
  ip->op[0].type  = (-1);
  ip->op[1].type  = (-1);
  ip->op[2].type  = (-1);
  G_rex           = 0;
  G_rep_prfx	  = 0;
  G_repne_prfx	  = 0;
  G_lock_prfx	  = 0;
  G_opr_prfx	  = 0;
  G_adr_prfx	  = 0;
  G_err           = 0;
  G_suff          = 0;
}

/* x86_DisasmInsn(...)  */
unsigned char* X86Disas::DisasmInsn (unsigned char *src, x86_Insn *ip)
{
  /* save the value of the ptr.. so tat we can calculate the number of
     bytes diassembled in the end.
   */
  unsigned char *srcref = src;
  unsigned char  prfxflag = 0;
  unsigned i;
  /* ptr to instruction in table */
  x86_TableEntry *insn = NULL;
  /* initialize instruction pointer (ip) */
  Clear(ip);
  /* search for prefixes */
  for (i =0; prfxflag == 0; ++i) {
	if (0x40 <= *src && *src <= 0x4F) {
		if (G_dis_mode == 64) {
			G_rex = *src;
			++src;
		}
		else prfxflag = 1;
	}
	else switch(*src) {
		case 0x2E : ip->seg = R_CS; src++; break;
		case 0x36 : ip->seg = R_SS; src++; break;
		case 0x3E : ip->seg = R_DS; src++; break;
		case 0x26 : ip->seg = R_ES; src++; break;
		case 0x64 : ip->seg = R_FS; src++; break;
		case 0x65 : ip->seg = R_GS; src++; break;
		case 0x66 : G_opr_prfx  = 0x66; src++ ; break ;
		case 0x67 : G_adr_prfx  = 0x67; src++ ; break ;
		case 0xF0 : G_lock_prfx = 0xF0; src++ ; break;
		case 0xF2 : G_repne_prfx= 0xF2; src++ ; break;
		case 0xF3 : G_rep_prfx  = 0xF3; src++ ; break;
		default :  prfxflag = 1;
	}
	/* if > MAX_PREFIXES, disintegrate */
	if (i >= MAX_PREFIXES) {
		G_err = 1;
		break;	/* break from loop */
	}
  }
  /* search map for instruction */
  src = GetInsnEntry(src, &insn);
  /* assign instruction mnemonic code */
  ip->mnm = insn->mnm;
  /* if invalid set error */
  if (ip->mnm == Iinvalid) G_err = 1;

  /* PREFIXES */
  G_rex = G_rex & REX_MASK(insn->prfx);		/* rex */
  G_def_64 = DEF64(insn->prfx);			/* set the default 64 flag */
  G_suff = SUFF(insn->prfx);			/* set the suffix flag */
  /* check if the instruction is invalid in 64 bits mode */
  if (G_dis_mode == 64 && INV64(insn->prfx))
	G_err = 1;
  /* set operand mode */
  if (G_opr_prfx) {
	if (G_dis_mode == 64)
		G_opr_mode = 16;
	else	G_opr_mode = (G_opr_mode == 32)	? 16 : 32 ;
  }
  /* set address mode */
  if (G_adr_prfx) {
	if (G_dis_mode == 64)
		G_adr_mode = 16;
	else	G_adr_mode = (G_adr_mode == 32)	? 16 : 32 ;
  }
  /* increment source value */
  ++src;
  /* disassemble operands */
  src = DisasmOperands(src, ip, insn);

  /* get suffix size */
  if (G_suff)
	ip->op[0].size = ResolveOpSize(SZ_v);
  /* check if o32 prefix was used */
  if (O32(insn->prfx) && G_opr_prfx)
  	G_opr_prfx = 0;
  /* check if a32 prefix was used */
  if (A32(insn->prfx) && G_adr_prfx)
  	G_adr_prfx = 0;
  /* if its a 3dnow! instruction, get mnemonic code based
     on the suffix */
  if (ip->mnm == I3dnow) {
	ip->mnm = Get3DNowInsnMnemonic(*src);
	++src;
  }
  /* check for G_err */
  if (G_err) {
	Clear(ip);
	/* init as define byte */
	ip->mnm = Idb;
	ip->op[0].type = I_BYTE;
	ip->op[0].size = 0;
	/* the first byte as operand */
	ip->op[0].lvalp = srcref;
	src = srcref + 1;
  }
  /* increase program counter */
  G_prg_ctr += (unsigned long)((src)-(srcref));
  return src;
}
