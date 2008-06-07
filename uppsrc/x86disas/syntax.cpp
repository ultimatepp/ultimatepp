/*
 *  syntax.c - Ouput/syntax
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

#include "X86Disas.h"

#define DO(x) #x,

char *mnemonics[] = {
	#include "mnemonics.i"
};

#undef DO

typedef signed char 	s8b ;
typedef unsigned char 	u8b ;
typedef signed short 	s16b;
typedef unsigned short 	u16b;
typedef signed long 	s32b;
typedef unsigned long	u32b;

struct OperandDecoderMap {
    unsigned code;	/* operand code */
    char    *scr;	/* script */
};

static unsigned char  n_scal[] = { 1, 2, 4, 8 };

/* this structure defines the decoder script for a particular
   ouput syntax.
 */

/* Decoder Script Directives (In Octal Number System)

   \1  - verbatim print delimiter
   \2  - print  8 bit sign extended offset
   \3  - print 16 bit offset
   \4  - print 32 bit offset
   \5  - print rel offset
   \6  - print seg override
   \7  - print rel cast
   \10 - print 64 bit offset
   \12 - print 8bit imm
   \13 - print 16bit imm
   \14 - print 32bit imm

   \20 - print SIB
   \21 - print SID
   \22 - print RSIB
   \23 - print RSID

   \26 - print ptr cast

   \30 - print 16 bit segment of ptr32
   \31 - print 16 bit offset of ptr32
   \32 - print 16 bit segment of ptr48
   \33 - print 32 bit offset of ptr48

   \40 - print 16b offset
   \41 - print 32b offset
   \42 - print 64b offset

   \50 - print lval

   \60 - print operand cast
   \61 - print jmp cast

*/

/* ---- INTEL Syntax ---- */

/* The Intel Syntax Operand Decoder Map
   WARNING -- Make sure the order of declaration is the same as
   that of the enumeration of operand codes in x86.h
 */
static OperandDecoderMap odm[] = {
  { M_BX_SI,		"\60\1[\1\6\1bx+si]\1" },
  { M_BX_DI,		"\60\1[\1\6\1bx+di]\1" },
  { M_BP_SI,		"\60\1[\1\6\1bp+si]\1" },
  { M_BP_DI,		"\60\1[\1\6\1bp+di]\1" },
  { M_SI,		"\60\1[\1\6\1si]\1" },
  { M_DI,		"\60\1[\1\6\1di]\1" },
  { M_OFF16,		"\60\1[\1\6\1\1\3\1]\1" },
  { M_BX,		"\60\1[\1\6\1bx]\1" },
  { M_BX_SI_OFF8,	"\60\1[\1\6\1bx+si\1\2\1]\1" },
  { M_BX_DI_OFF8, 	"\60\1[\1\6\1bx+di\1\2\1]\1" },
  { M_BP_SI_OFF8,	"\60\1[\1\6\1bp+si\1\2\1]\1" },
  { M_BP_DI_OFF8,	"\60\1[\1\6\1bp+di\1\2\1]\1" },
  { M_SI_OFF8, 		"\60\1[\1\6\1si\1\2\1]\1" },
  { M_DI_OFF8, 		"\60\1[\1\6\1di\1\2\1]\1" },
  { M_BP_OFF8, 		"\60\1[\1\6\1bp\1\2\1]\1" },
  { M_BX_OFF8,		"\60\1[\1\6\1bx\1\2\1]\1" },
  { M_BX_SI_OFF16, 	"\60\1[\1\6\1bx+si+\1\3\1]\1" },
  { M_BX_DI_OFF16, 	"\60\1[\1\6\1bx+di+\1\3\1]\1" },
  { M_BP_SI_OFF16, 	"\60\1[\1\6\1bp+si+\1\3\1]\1" },
  { M_BP_DI_OFF16,	"\60\1[\1\6\1bp+di+\1\3\1]\1" },
  { M_SI_OFF16, 	"\60\1[\1\6\1si+\1\3\1]\1" },
  { M_DI_OFF16, 	"\60\1[\1\6\1di+\1\3\1]\1" },
  { M_BP_OFF16, 	"\60\1[\1\6\1bp+\1\3\1]\1" },
  { M_BX_OFF16,		"\60\1[\1\6\1bx+\1\3\1]\1" },
  { M_EAX,		"\60\1[\1\6\1eax]\1" },
  { M_ECX,		"\60\1[\1\6\1ecx]\1" },
  { M_EDX,		"\60\1[\1\6\1edx]\1" },
  { M_EBX,		"\60\1[\1\6\1ebx]\1" },
  { M_SIB,		"\60\1[\1\6\020\1]\1" },
  { M_OFF32,		"\60\1[\1\6\4\1]\1" },
  { M_ESI, 		"\60\1[\1\6\1esi]\1" },
  { M_EDI,		"\60\1[\1\6\1edi]\1" },
  { M_EAX_OFF8, 	"\60\1[\1\6\1eax\1\2\1]\1" },
  { M_ECX_OFF8, 	"\60\1[\1\6\1ecx\1\2\1]\1" },
  { M_EDX_OFF8, 	"\60\1[\1\6\1edx\1\2\1]\1" },
  { M_EBX_OFF8, 	"\60\1[\1\6\1ebx\1\2\1]\1" },
  { M_SIB_OFF8,		"\60\1[\1\6\020\2\1]\1" },
  { M_EBP_OFF8, 	"\60\1[\1\6\1ebp\1\2\1]\1" },
  { M_ESI_OFF8, 	"\60\1[\1\6\1esi\1\2\1]\1" },
  { M_EDI_OFF8, 	"\60\1[\1\6\1edi\1\2\1]\1" },
  { M_EAX_OFF32, 	"\60\1[\1\6\1eax+\1\4\1]\1" },
  { M_ECX_OFF32, 	"\60\1[\1\6\1ecx+\1\4\1]\1" },
  { M_EDX_OFF32, 	"\60\1[\1\6\1edx+\1\4\1]\1" },
  { M_EBX_OFF32, 	"\60\1[\1\6\1ebx+\1\4\1]\1" },
  { M_SIB_OFF32,	"\60\1[\1\6\020\1+\1\4\1]\1" },
  { M_EBP_OFF32, 	"\60\1[\1\6\1ebp+\1\4\1]\1" },
  { M_ESI_OFF32, 	"\60\1[\1\6\1esi+\1\4\1]\1" },
  { M_EDI_OFF32, 	"\60\1[\1\6\1edi+\1\4\1]\1" },
  { M_SID,		"\60\1[\1\6\021\1+\1\4\1]\1" },
  { M_RAX,		"\60\1[\1\6\1rax]\1" },
  { M_RCX,		"\60\1[\1\6\1rcx]\1" },
  { M_RDX,		"\60\1[\1\6\1rdx]\1" },
  { M_RBX,		"\60\1[\1\6\1rbx]\1" },
  { M_RSIB,		"\60\1[\1\6\022\1]\1" },
  { M_ROFF32,		"\60\1[\1\6\4\1]\1" },
  { M_RSI, 		"\60\1[\1\6\1rsi]\1" },
  { M_RDI,		"\60\1[\1\6\1rdi]\1" },
  { M_R8,		"\60\1[\1\6\1r8]\1" },
  { M_R9,		"\60\1[\1\6\1r9]\1" },
  { M_R10,		"\60\1[\1\6\1r10]\1" },
  { M_R11,		"\60\1[\1\6\1r11]\1" },
  { M_R12,		"\60\1[\1\6\1r12]\1" },
  { M_R13,		"\60\1[\1\6\1r13]\1" },
  { M_R14, 		"\60\1[\1\6\1r14]\1" },
  { M_R15,		"\60\1[\1\6\1r15]\1" },
  { M_RAX_OFF8, 	"\60\1[\1\6\1rax\1\2\1]\1" },
  { M_RCX_OFF8, 	"\60\1[\1\6\1rcx\1\2\1]\1" },
  { M_RDX_OFF8, 	"\60\1[\1\6\1rdx\1\2\1]\1" },
  { M_RBX_OFF8, 	"\60\1[\1\6\1rbx\1\2\1]\1" },
  { M_RSIB_OFF8,	"\60\1[\1\6\022\2\1]\1" },
  { M_RBP_OFF8, 	"\60\1[\1\6\1rbp\1\2\1]\1" },
  { M_RSI_OFF8, 	"\60\1[\1\6\1rsi\1\2\1]\1" },
  { M_RDI_OFF8, 	"\60\1[\1\6\1rdi\1\2\1]\1" },
  { M_R8_OFF8,		"\60\1[\1\6\1r8\1\2\1]\1" },
  { M_R9_OFF8,		"\60\1[\1\6\1r9\1\2\1]\1" },
  { M_R10_OFF8, 	"\60\1[\1\6\1r10\1\2\1]\1" },
  { M_R11_OFF8, 	"\60\1[\1\6\1r11\1\2\1]\1" },
  { M_R12_OFF8,		"\60\1[\1\6\1r12\1\2\1]\1" },
  { M_R13_OFF8, 	"\60\1[\1\6\1r13\1\2\1]\1" },
  { M_R14_OFF8, 	"\60\1[\1\6\1r14\1\2\1]\1" },
  { M_R15_OFF8, 	"\60\1[\1\6\1r15\1\2\1]\1" },
  { M_RAX_OFF32, 	"\60\1[\1\6\1rax+\1\4\1]\1" },
  { M_RCX_OFF32, 	"\60\1[\1\6\1rcx+\1\4\1]\1" },
  { M_RDX_OFF32, 	"\60\1[\1\6\1rdx+\1\4\1]\1" },
  { M_RBX_OFF32, 	"\60\1[\1\6\1rbx+\1\4\1]\1" },
  { M_SIB_OFF32,	"\60\1[\1\6\022\1+\1\4\1]\1" },
  { M_RBP_OFF32, 	"\60\1[\1\6\1rbp+\1\4\1]\1" },
  { M_RSI_OFF32, 	"\60\1[\1\6\1rsi+\1\4\1]\1" },
  { M_RDI_OFF32, 	"\60\1[\1\6\1rdi+\1\4\1]\1" },
  { M_R8_OFF32, 	"\60\1[\1\6\1r8+\1\4\1]\1" },
  { M_R9_OFF32, 	"\60\1[\1\6\1r9+\1\4\1]\1" },
  { M_R10_OFF32, 	"\60\1[\1\6\1r10+\1\4\1]\1" },
  { M_R11_OFF32, 	"\60\1[\1\6\1r11+\1\4\1]\1" },
  { M_R12_OFF32,	"\60\1[\1\6\1r12+\1\4\1]\1" },
  { M_R13_OFF32, 	"\60\1[\1\6\1r13+\1\4\1]\1" },
  { M_R14_OFF32, 	"\60\1[\1\6\1r14+\1\4\1]\1" },
  { M_R15_OFF32, 	"\60\1[\1\6\1r15+\1\4\1]\1" },
  { M_RSID,		"\60\1[\1\6\023\1+\1\4\1]\1" },
  { OFFSET16,		"\1[\1\40\1]\1" },
  { OFFSET32,		"\1[\1\41\1]\1" },
  { OFFSET64,		"\1[\1\42\1]\1" },
  { R_AL, 		"\1al\1" },
  { R_CL, 		"\1cl\1" },
  { R_DL, 		"\1dl\1" },
  { R_BL, 		"\1bl\1" },
  { R_AH, 		"\1ah\1" },
  { R_CH, 		"\1ch\1" },
  { R_DH, 		"\1dh\1" },
  { R_BH, 		"\1bh\1" },
  { R_SPL,		"\1spl\1"},
  { R_BPL,		"\1bpl\1"},
  { R_SIL,		"\1sil\1"},
  { R_DIL,		"\1dil\1"},
  { R_R8B,		"\1r8b\1"},
  { R_R9B,		"\1r9b\1"},
  { R_R10B,		"\1r10b\1"},
  { R_R11B,		"\1r11b\1"},
  { R_R12B,		"\1r12b\1"},
  { R_R13B,		"\1r13b\1"},
  { R_R14B,		"\1r14b\1"},
  { R_R15B,		"\1r15b\1"},
  { R_AX, 		"\1ax\1" },
  { R_CX, 		"\1cx\1" },
  { R_DX, 		"\1dx\1" },
  { R_BX, 		"\1bx\1" },
  { R_SP, 		"\1sp\1" },
  { R_BP, 		"\1bp\1" },
  { R_SI, 		"\1si\1" },
  { R_DI, 		"\1di\1" },
  { R_R8W,		"\1r8w\1"},
  { R_R9W,		"\1r9w\1"},
  { R_R10W,		"\1r10w\1"},
  { R_R11W,		"\1r11w\1"},
  { R_R12W,		"\1r12w\1"},
  { R_R13W,		"\1r13w\1"},
  { R_R14W,		"\1r14w\1"},
  { R_R15W,		"\1r15w\1"},
  { R_EAX, 		"\1eax\1"},
  { R_ECX, 		"\1ecx\1"},
  { R_EDX, 		"\1edx\1"},
  { R_EBX, 		"\1ebx\1"},
  { R_ESP, 		"\1esp\1"},
  { R_EBP, 		"\1ebp\1"},
  { R_ESI, 		"\1esi\1"},
  { R_EDI, 		"\1edi\1"},
  { R_R8D,		"\1r8d\1"},
  { R_R9D,		"\1r9d\1"},
  { R_R10D,		"\1r10d\1"},
  { R_R11D,		"\1r11d\1"},
  { R_R12D,		"\1r12d\1"},
  { R_R13D,		"\1r13d\1"},
  { R_R14D,		"\1r14d\1"},
  { R_R15D,		"\1r15d\1"},
  { R_RAX, 		"\1rax\1"},
  { R_RCX, 		"\1rcx\1"},
  { R_RDX, 		"\1rdx\1"},
  { R_RBX, 		"\1rbx\1"},
  { R_RSP, 		"\1rsp\1"},
  { R_RBP, 		"\1rbp\1"},
  { R_RSI, 		"\1rsi\1"},
  { R_RDI, 		"\1rdi\1"},
  { R_R8,		"\1r8\1"},
  { R_R9,		"\1r9\1"},
  { R_R10,		"\1r10\1"},
  { R_R11,		"\1r11\1"},
  { R_R12,		"\1r12\1"},
  { R_R13,		"\1r13\1"},
  { R_R14,		"\1r14\1"},
  { R_R15,		"\1r15\1"},
  { R_ES,		"\1es\1" },
  { R_CS,		"\1cs\1" },
  { R_SS,		"\1ss\1" },
  { R_DS,		"\1ds\1" },
  { R_FS,		"\1fs\1" },
  { R_GS,		"\1gs\1" },
  { R_CR0,		"\1cr0\1" },
  { R_CR1,		"\1cr1\1" },
  { R_CR2,		"\1cr2\1" },
  { R_CR3,		"\1cr3\1" },
  { R_CR4,		"\1cr4\1" },
  { R_CR5_INV,		"\0" },
  { R_CR6_INV,		"\0" },
  { R_CR7_INV,		"\0" },
  { R_DR0,		"\1dr0\1" },
  { R_DR1,		"\1dr1\1" },
  { R_DR2,		"\1dr2\1" },
  { R_DR3,		"\1dr3\1" },
  { R_DR4_INV,		"\0" },
  { R_DR5_INV,		"\0" },
  { R_DR6,		"\1dr6\1" },
  { R_DR7,		"\1dr7\1" },
  { R_MM0,		"\1mm0\1" },
  { R_MM1,		"\1mm1\1" },
  { R_MM2,		"\1mm2\1" },
  { R_MM3,		"\1mm3\1" },
  { R_MM4,		"\1mm4\1" },
  { R_MM5,		"\1mm5\1" },
  { R_MM6,		"\1mm6\1" },
  { R_MM7,		"\1mm7\1" },
  { R_ST0,		"\1st0\1" },
  { R_ST1,		"\1st1\1" },
  { R_ST2,		"\1st2\1" },
  { R_ST3,		"\1st3\1" },
  { R_ST4,		"\1st4\1" },
  { R_ST5,		"\1st5\1" },
  { R_ST6,		"\1st6\1" },
  { R_ST7,		"\1st7\1" },
  { R_XMM0,		"\1xmm0\1" },
  { R_XMM1,		"\1xmm1\1" },
  { R_XMM2,		"\1xmm2\1" },
  { R_XMM3,		"\1xmm3\1" },
  { R_XMM4,		"\1xmm4\1" },
  { R_XMM5,		"\1xmm5\1" },
  { R_XMM6,		"\1xmm6\1" },
  { R_XMM7,		"\1xmm7\1" },
  { R_XMM8,		"\1xmm8\1" },
  { R_XMM9,		"\1xmm9\1" },
  { R_XMM10,		"\1xmm10\1" },
  { R_XMM11,		"\1xmm11\1" },
  { R_XMM12,		"\1xmm12\1" },
  { R_XMM13,		"\1xmm13\1" },
  { R_XMM14,		"\1xmm14\1" },
  { R_XMM15,		"\1xmm15\1" },
  { I_BYTE, 		"\60\12"  },
  { I_WORD,		"\60\13"  },
  { I_DWRD,		"\60\14"  },
  { J_IMM, 		"\60\5"	  },
  { P_32,		"\60\30\1:\1\31" },
  { P_48,		"\60\32\1:\1\33" },
  { PRINT_I,		"\50"},
};

/* 64bit regs */
static char *q_regs[] = {
  "rax",  "rcx",  "rdx",  "rbx",  "rsp",  "rbp",  "rsi",  "rdi",
  "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
};

/* 32bit regs */
static char *d_regs[] = {
  "eax",  "ecx",  "edx",  "ebx",  "esp",  "ebp",  "esi",  "edi",
  "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d"
};

/* segment registers */
static char *s_regs[] = {
  "es", "cs",  "ss",  "ds",  "fs",  "gs"
};

/* Operand Cast */
void X86Disas::print_opr_cast(unsigned char sz)
{
  if(cast) {
	switch(sz) {
		case SZ_sb:
		case SZ_b : operands << Sprintf("byte " ); break;
		case SZ_w : operands << Sprintf("word " ); break;
		case SZ_d : operands << Sprintf("dword "); break;
		case SZ_wp: operands << Sprintf("word far "); break;
		case SZ_dp: operands << Sprintf("dword far "); break;
		case SZ_q : operands << Sprintf("qword "); break;
	}
	cast--;
  }
}

/* Relative offset cast */
void X86Disas::print_rel_cast(unsigned char sz)
{
  switch(sz) {
	case SZ_b: operands << Sprintf( "short "); break;
	case SZ_w: operands << Sprintf( "near " ); break;
	case SZ_d: operands << Sprintf( "dword "); break;
    }
}

/* SIB */
void X86Disas::print_sib(x86_Insn *ip, unsigned n)
{
  operands << Sprintf("%s+%s", d_regs[(ip->op[n].sib.base)], d_regs[(ip->op[n].sib.indx)]);
  if (n_scal[ip->op[n].sib.scal] > 1)
	operands << Sprintf( "*%d", n_scal[ip->op[n].sib.scal]);
}

/* SIB(64) */
void X86Disas::print_rsib(x86_Insn *ip, unsigned n)
{
  operands << Sprintf("%s+%s", q_regs[(ip->op[n].sib.base)], q_regs[(ip->op[n].sib.indx)]);
  if (n_scal[ip->op[n].sib.scal] > 1)
	operands << Sprintf( "*%d", n_scal[ip->op[n].sib.scal]);
}

/* SI */
void X86Disas::print_si(x86_Insn *ip, unsigned n)
{
  operands << Sprintf("%s*%d", d_regs[(ip->op[n].sib.indx)], n_scal[ip->op[n].sib.scal]);
}

/* SIB(64) */
void X86Disas::print_rsi(x86_Insn *ip, unsigned n)
{
  operands << Sprintf("%s*%d", q_regs[(ip->op[n].sib.indx)], n_scal[ip->op[n].sib.scal]);
}

#define LVAL(c, lp)	( *( (c*)(lp) ) )
#define PBW(sz, lp)			\
  do {					\
    unsigned _sz = sz;			\
    for(; _sz >= 0; _sz -= 8);		\
      operands << Sprintf("%x", *(u8b*)(lp+sz));	\
  } while(0)

/* operand script interpreter */
void X86Disas::opscript(x86_Insn *ip, unsigned n, char *script)
{
  for (;*(script); ++script) {
	switch (*script) {
		case 01: /* verbatim print delimiter */
			for (++script;*(script) != 01; ++script)
				operands << Sprintf("%c", *script);
			break;
		case 02: /* print 8 bit offet (sign extended) */
			if ( LVAL(s8b,ip->op[n].lvalp) < 0 )
				operands << Sprintf("-0x%x", -1 * LVAL(s8b,ip->op[n].lvalp) );
			else
				operands << Sprintf("+0x%x",  LVAL(s8b,ip->op[n].lvalp) );
			break;
		case 03: /* print 16 bit offset */
			operands << Sprintf("0x%x", LVAL(u16b,ip->op[n].lvalp) );
			break;
		case 04: /* print 32 bit offset */
			operands << Sprintf("0x%lx", LVAL(u32b,ip->op[n].lvalp) );
			break;
		case 010:/* print 64 bit offset */
			PBW(64, ip->op[n].lvalp);
			break;
		case 05:/* relative offset */
			switch(ip->op[n].size) {
				case SZ_jb:
					operands << Sprintf("0x%lx",G_prg_ctr+LVAL(s8b,ip->op[n].lvalp));
					break;
				case SZ_jw:
					operands << Sprintf("0x%lx",G_prg_ctr+LVAL(s16b,ip->op[n].lvalp));
					break;
				case SZ_jd:
					operands << Sprintf("0x%lx",G_prg_ctr+LVAL(s32b,ip->op[n].lvalp));
					break;
				default :
					operands << Sprintf(":%d", ip->op[n].size);
			}
			break;
		case 06: /* print segment override */
			if (ip->seg)
				operands << Sprintf( "%s:", s_regs[(ip->seg)-R_ES]);
			break;
		case 07: /* relative cast */
			print_rel_cast(ip->op[n].size);
			break;
		case 012: /* print 8bit imm */
			if (ip->op[n].size == SZ_sb) {
				if ( LVAL(s8b,ip->op[n].lvalp) < 0 )
					operands << Sprintf("-0x%x", -1 * LVAL(s8b,ip->op[n].lvalp) );
				else
					operands << Sprintf("+0x%x",  LVAL(s8b,ip->op[n].lvalp) );
			}
			else	operands << Sprintf("0x%x", LVAL(u8b,ip->op[n].lvalp));
			break;
		case 013: /* print 16bit imm */
			operands << Sprintf("0x%x",LVAL(u16b,ip->op[n].lvalp));
			break;
		case 014: /* print 32bit imm */
			operands << Sprintf("0x%lx",LVAL(u32b,ip->op[n].lvalp));
			break;
		case 020: /* print sib */
			print_sib(ip, n);
			break;
		case 021: /* print si */
			print_si(ip, n);
			break;
		case 022: /* print rsib */
			print_rsib(ip, n);
			break;
		case 023: /* print rsi */
			print_rsi(ip, n);
			break;
		case 030: /* print 16 bit segment of ptr32 */
			operands << Sprintf("0x%x", (u16b)(LVAL(u32b,ip->op[n].lvalp)>>16));
			break;
		case 031: /* print 16 bit offset of ptr32 */
			operands << Sprintf("0x%x", (u16b)(LVAL(u32b,ip->op[n].lvalp)&0xFFFF));
			break;
		case 032: /* print 16 bit segment of ptr48 */
			operands << Sprintf("0x%x", (u16b)(LVAL(u32b,ip->op[n].lvalp+4)));
			break;
		case 033: /* print 32 bit offset of ptr48 */
			operands << Sprintf("0x%lx", LVAL(u32b,ip->op[n].lvalp));
			break;
		case 040: /* print offset 16b */
			operands << Sprintf("0x%x",LVAL(u16b,ip->op[n].lvalp));
			break;
		case 041: /* print offset 32b */
			operands << Sprintf("0x%lx",LVAL(u32b,ip->op[n].lvalp));
			break;
		case 042: /* print offset 64b */
			PBW(64, ip->op[n].lvalp);
			break;
		case 050: /* print lval */
			operands << Sprintf("0x%lx",ip->op[n].lval[0]);
			break;
		case 060: /* print operand cast */
			print_opr_cast(ip->op[n].size);
			break;
	}
  }
}

/* disasms the passed instruction pointer to intel syntax */
void X86Disas::Disasm(x86_Insn *ip)
{
  cast = 1;
  if (ip->op[0].size != ip->op[1].size && ip->op[2].type == -1)
	cast = 2;
  else
  if ( ip->op[2].type != -1 && ((ip->op[0].size != ip->op[1].size) ||
      (ip->op[1].size != ip->op[2].size) || (ip->op[0].size != ip->op[2].size)) )
	cast = 2;

  /* address mode override */
  if (G_opr_prfx)
	mnemonic << Sprintf( "o32 ");
  /* operand mode override */
  if (G_adr_prfx)
	mnemonic << Sprintf( "a32 ");
  /* lock prefix */
  if (G_lock_prfx)
	mnemonic << Sprintf( "lock ");
  /* rep prefix */
  if (G_rep_prfx)
	mnemonic << Sprintf( "rep ");
  /* repne prefix */
  if (G_repne_prfx)
	mnemonic << Sprintf( "repne ");

  /* print the instruction mnemonic */
  mnemonic << Sprintf("%s",mnemonics[ip->mnm]);

  /* string operation suffix */
  if (G_suff) {
	switch(ip->op[0].size) {
		case SZ_w : mnemonic << Sprintf("w"); break;
		case SZ_d : mnemonic << Sprintf("d"); break;
		case SZ_q : mnemonic << Sprintf("q"); break;
	}
  }

  /* space between insn menmonic and operands */
  mnemonic << Sprintf(" ");

  /* operand 1 */
  if (ip->op[0].type != -1) {
	opscript(ip,0, odm[ip->op[0].type].scr);
	/* operand 2 */
	if (ip->op[1].type != -1) {
		operands << Sprintf(", ");
		opscript(ip, 1, odm[ip->op[1].type].scr);
		/* operand 3 */
		if (ip->op[2].type != -1) {
			operands << Sprintf(", ");
			opscript(ip, 2, odm[ip->op[2].type].scr);
		}
	}
  }
}
