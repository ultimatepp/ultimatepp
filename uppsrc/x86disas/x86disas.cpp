#include "x86disas.h"

X86Disas::X86Disas(dword ip, byte *data)
{
	G_dis_mode = 32;	/* disassembly mode, (16/32/64) */
	G_opr_mode = 32;	/* operand mode, (16/32) */
	G_adr_mode = 32;	/* address mode, (16/32) */
	G_opr_prfx =  0;	/* 0x66 prefix flag */
	G_adr_prfx =  0;	/* 0x67 prefix flag */
	G_lock_prfx=  0;	/* 0xF0 (lock) prefix flag */
	G_rep_prfx =  0;	/* 0xF3 (rep) prefix flag */
	G_repe_prfx=  0;	/* 0xF3 (repe) prefix flag */
	G_repne_prfx= 0;	/* 0xF2 (repne) prefix flag */
	G_prg_ctr  =  0;	/* program counter */
	G_err      =  0;	/* disassembly abort flag */
	G_rex      =  0;	/* REX prefix for 64bit mode */
	G_suff	   =  0;  /* instruction suffix flag */
	G_def_64   =  0;	/* default mode 64 flag */
	G_temp	   =  0;  /* a temp global var */

	G_prg_ctr = ip;

	x86_Insn ins;
	DisasmInsn(data, &ins);
	size = G_prg_ctr - ip;
	Disasm(&ins);
	iscall = ins.mnm == Icall;
}
