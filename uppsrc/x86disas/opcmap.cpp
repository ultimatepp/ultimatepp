/*
 *  opcmap.c - Opcode Maps for x86,x87,sse,mmx,3dnow
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

/* operand types */
#define Ap	{	OP_A,	SZ_p	}
#define M	{	OP_M,	0	}
#define Md	{	OP_M,	SZ_d	}
#define Mb	{	OP_M,	SZ_b	}
#define Mw	{	OP_M,	SZ_w	}
#define Ms	{	OP_M,	SZ_w	}
#define Mq	{	OP_M,	SZ_q	}
#define E	{	OP_E,	0	}
#define Eb	{	OP_E,	SZ_b	}
#define Ew	{	OP_E,	SZ_w	}
#define Ev	{	OP_E,	SZ_v	}
#define Evw	{	OP_E,	SZ_vw	}
#define Ed	{	OP_E,	SZ_d	}
#define Ex	{	OP_E,	SZ_x	}
#define Ep	{	OP_E,	SZ_p	}
#define Ez	{	OP_E,	SZ_z	}
#define G	{	OP_G,	0	}
#define Gx	{	OP_G,	SZ_x	}
#define Gb	{	OP_G,	SZ_b	}
#define Gw	{	OP_I,	SZ_w	}
#define Gv	{	OP_G,	SZ_v	}
#define Gd	{	OP_G,	SZ_d	}
#define Gvw	{	OP_G,	SZ_vw	}
#define	I1	{	OP_I1,	0	}
#define	I3	{	OP_I3,	0	}
#define Ib	{	OP_I,	SZ_b	}
#define Isb	{	OP_I,	SZ_sb	}
#define Iw	{	OP_I,	SZ_w	}
#define Iv	{	OP_I,	SZ_v	}
#define Iz	{	OP_I,	SZ_z	}
#define Jv	{	OP_J,	SZ_v	}
#define Jz	{	OP_J,	SZ_z	}
#define Jb	{	OP_J,	SZ_b	}
#define Jsb	{	OP_J,	SZ_sb	}
#define Jw	{	OP_J,	SZ_w	}
#define R	{	OP_R,	SZ_m	}
#define C	{	OP_C,	0	}
#define D	{	OP_D,	0	}
#define S	{	OP_S,	0	}
#define Ob	{	OP_O,	SZ_b	}
#define Ow	{	OP_O,	SZ_w	}
#define Ov	{	OP_O,	SZ_v	}
#define V	{	OP_V,	0	}
#define W	{	OP_W,	0	}
#define P	{	OP_P,	0	}
#define Q	{	OP_Q,	0	}
#define VR	{	OP_VR,	0	}
#define PR	{	OP_PR,	0	}
#define AL	{	OP_AL,	0	}
#define CL	{	OP_CL,	0	}
#define DL	{	OP_DL,	0	}
#define BL	{	OP_BL,	0	}
#define AH	{	OP_AH,	0	}
#define CH	{	OP_CH,	0	}
#define DH	{	OP_DH,	0	}
#define BH	{	OP_BH,	0	}
#define AX	{	OP_AX,	0	}
#define CX	{	OP_CX,	0	}
#define DX	{	OP_DX,	0	}
#define BX	{	OP_BX,	0	}
#define SI	{	OP_SI,	0	}
#define DI	{	OP_DI,	0	}
#define SP	{	OP_SP,	0	}
#define BP	{	OP_BP,	0	}
#define eAX	{	OP_eAX,	0	}
#define eCX	{	OP_eCX,	0	}
#define eDX	{	OP_eDX,	0	}
#define eBX	{	OP_eBX,	0	}
#define eSI	{	OP_eSI,	0	}
#define eDI	{	OP_eDI,	0	}
#define eSP	{	OP_eSP,	0	}
#define eBP	{	OP_eBP,	0	}
#define rAX	{	OP_rAX,	0	}
#define rCX	{	OP_rCX,	0	}
#define rBX	{	OP_rDX,	0	}
#define rDX	{	OP_rDX,	0	}
#define rSI	{	OP_rSI,	0	}
#define rDI	{	OP_rDI,	0	}
#define rSP	{	OP_rSP,	0	}
#define rBP	{	OP_rBP,	0	}
#define ES	{	OP_ES,	0	}
#define CS	{	OP_CS,	0	}
#define DS	{	OP_DS,	0	}
#define SS	{	OP_SS,	0	}
#define GS	{	OP_GS,	0	}
#define FS	{	OP_FS,	0	}
#define ST0	{	OP_ST0,	0	}
#define ST1	{	OP_ST1,	0	}
#define ST2	{	OP_ST2,	0	}
#define ST3	{	OP_ST3,	0	}
#define ST4	{	OP_ST4,	0	}
#define ST5	{	OP_ST5,	0	}
#define ST6	{	OP_ST6,	0	}
#define ST7	{	OP_ST7,	0	}
#define NOARG	{	0,	0	}
#define ALr8b	{	OP_ALr8b,0	}
#define CLr9b	{	OP_CLr9b,0	}
#define DLr10b	{	OP_DLr10b,0	}
#define BLr11b	{	OP_BLr11b,0	}
#define AHr12b	{	OP_AHr12b,0	}
#define CHr13b	{	OP_CHr13b,0	}
#define DHr14b	{	OP_DHr14b,0	}
#define BHr15b	{	OP_BHr15b,0	}
#define rAXr8	{	OP_rAXr8,0	}
#define rCXr9	{	OP_rCXr9,0	}
#define rDXr10	{	OP_rDXr10,0	}
#define rBXr11	{	OP_rBXr11,0	}
#define rSPr12	{	OP_rSPr12,0	}
#define rBPr13	{	OP_rBPr13,0	}
#define rSIr14	{	OP_rSIr14,0	}
#define rDIr15	{	OP_rDIr15,0	}


/* Instruction Prefixes */
#define Pnone	0x00
#define Pa32	0x01
#define Po32	0x02
#define Prex	0x04
#define Pdef64	0x08
#define Pinv64	0x10
#define Psuff	0x20

/* REX (AMD64) */
#define _W	8
#define _R	4
#define _X	2
#define _B	1
#define REX(c)	((40 | c) << 16)

/* 1 byte opcode */
x86_TableEntry insntbl_1byte[0x100] = {

	/* Instruction, op1, op2, op3, Valid Prefixes */

/* 00 */ { Iadd,	Eb,	Gb,	NOARG,	Pa32 | REX(_X|_B) },
/* 01 */ { Iadd,	Ev,	Gv,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 02 */ { Iadd,	Gb,	Eb,	NOARG,	Pa32 | REX(_X|_B) },
/* 03 */ { Iadd,	Gv,	Ev,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 04 */ { Iadd,	AL,	Ib,	NOARG,	Pnone },
/* 05 */ { Iadd,	rAX,	Iv,	NOARG,	Prex | Po32 | REX(_W) },
/* 06 */ { Ipush,	ES,	NOARG,	NOARG,	Pnone },
/* 07 */ { Ipop,	ES,	NOARG,	NOARG,	Pnone },
/* 08 */ { Ior,		Eb,	Gb,	NOARG,	Prex | Pa32 | REX(_X|_B) },
/* 09 */ { Ior,		Ev,	Gv,	NOARG,	Prex | Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 0A */ { Ior,		Gb,	Eb,	NOARG,	Prex | Pa32 | REX(_X|_B)},
/* 0B */ { Ior,		Gv,	Ev,	NOARG,	Prex | Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 0C */ { Ior,		AL,	Ib,	NOARG,	Pnone },
/* 0D */ { Ior,		rAX,	Iv,	NOARG,	Prex | Po32 | REX(_W) },
/* 0E */ { Ipush,	CS,	NOARG,	NOARG,	Pnone },
/* 0F */ { Iesc,	NOARG,	NOARG,	NOARG,	Pnone },
/* 10 */ { Iadc,	Eb,	Gb,	NOARG,	Pa32 | REX(_X|_B)  },
/* 11 */ { Iadc,	Ev,	Gv,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 12 */ { Iadc,	Gb,	Eb,	NOARG,	Pa32 | REX(_X|_B)  },
/* 13 */ { Iadc,	Gv,	Ev,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 14 */ { Iadc,	AL,	Ib,	NOARG,	Pnone },
/* 15 */ { Iadc,	rAX,	Iv,	NOARG,	Prex | Po32 | REX(_W) },
/* 16 */ { Ipush,	SS,	NOARG,	NOARG,	Pnone },
/* 17 */ { Ipop,	SS,	NOARG,	NOARG,	Pnone },
/* 18 */ { Isbb,	Eb,	Gb,	NOARG,	Pa32 | REX(_X|_B)  },
/* 19 */ { Isbb,	Ev,	Gv,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 1A */ { Isbb,	Gb,	Eb,	NOARG,	Pa32 | REX(_X|_B)  },
/* 1B */ { Isbb,	Gv,	Ev,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 1C */ { Isbb,	AL,	Ib,	NOARG,	Pnone },
/* 1D */ { Isbb,	rAX,	Iv,	NOARG,	Prex | Po32 | REX(_W) },
/* 1E */ { Ipush,	DS,	NOARG,	NOARG,	Pnone },
/* 1F */ { Ipop,	DS,	NOARG,	NOARG,	Pnone },
/* 20 */ { Iand,	Eb,	Gb,	NOARG,	Pa32 | REX(_X|_B)  },
/* 21 */ { Iand,	Ev,	Gv,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 22 */ { Iand,	Gb,	Eb,	NOARG,	Pa32 | REX(_X|_B)  },
/* 23 */ { Iand,	Gv,	Ev,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 24 */ { Iand,	AL,	Ib,	NOARG,	Pnone },
/* 25 */ { Iand,	rAX,	Iv,	NOARG,	Prex | Po32 | REX(_W) },
/* 26 */ { Ies,		ES,	NOARG,	NOARG,	Pnone },
/* 27 */ { Idaa,	NOARG,	NOARG,	NOARG,	Pnone },
/* 28 */ { Isub,	Eb,	Gb,	NOARG,	Pa32 | REX(_X|_B)  },
/* 29 */ { Isub,	Ev,	Gv,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 2A */ { Isub,	Gb,	Eb,	NOARG,	Pa32 | REX(_X|_B)  },
/* 2B */ { Isub,	Gv,	Ev,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 2C */ { Isub,	AL,	Ib,	NOARG,	Pnone },
/* 2D */ { Isub,	rAX,	Iv,	NOARG,	Prex | Po32 | REX(_W) },
/* 2E */ { Ics,		CS,	NOARG,	NOARG,	Pnone },
/* 2F */ { Idas,	NOARG,	NOARG,	NOARG,	Prex | Po32 | REX(_W) },
/* 30 */ { Ixor,	Eb,	Gb,	NOARG,	Pa32 | REX(_X|_B)  },
/* 31 */ { Ixor,	Ev,	Gv,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 32 */ { Ixor,	Gb,	Eb,	NOARG,	Pa32 | REX(_X|_B)  },
/* 33 */ { Ixor,	Gv,	Ev,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 34 */ { Ixor,	AL,	Ib,	NOARG,	Pnone },
/* 35 */ { Ixor,	rAX,	Iv,	NOARG,	Prex | Po32 | REX(_W) },
/* 36 */ { Iss,		ES,	NOARG,	NOARG,	Pnone },
/* 37 */ { Iaaa,	NOARG,	NOARG,	NOARG,	Pnone },
/* 38 */ { Icmp,	Eb,	Gb,	NOARG,	Pa32 | REX(_X|_B)  },
/* 39 */ { Icmp,	Ev,	Gv,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 3A */ { Icmp,	Gb,	Eb,	NOARG,	Pa32 | REX(_X|_B)  },
/* 3B */ { Icmp,	Gv,	Ev,	NOARG,	Pa32 | Po32 | REX(_W|_R|_X|_B) },
/* 3C */ { Icmp,	AL,	Ib,	NOARG,	Pnone },
/* 3D */ { Icmp,	rAX,	Iv,	NOARG,	Prex | Po32 | REX(_W) },
/* 3E */ { Ids,		ES,	NOARG,	NOARG,	Pnone },
/* 3F */ { Iaas,	NOARG,	NOARG,	NOARG,	Pnone },
/* 40 */ { Iinc,	eAX,	NOARG,	NOARG,	Po32 },
/* 41 */ { Iinc,	eCX,	NOARG,	NOARG,	Po32 },
/* 42 */ { Iinc,	eDX,	NOARG,	NOARG,	Po32 },
/* 43 */ { Iinc,	eBX,	NOARG,	NOARG,	Po32 },
/* 44 */ { Iinc,	eSP,	NOARG,	NOARG,	Po32 },
/* 45 */ { Iinc,	eBP,	NOARG,	NOARG,	Po32 },
/* 46 */ { Iinc,	eSI,	NOARG,	NOARG,	Po32 },
/* 47 */ { Iinc,	eDI,	NOARG,	NOARG,	Po32 },
/* 48 */ { Idec,	eAX,	NOARG,	NOARG,	Po32 },
/* 49 */ { Idec,	eCX,	NOARG,	NOARG,	Po32 },
/* 4A */ { Idec,	eDX,	NOARG,	NOARG,	Po32 },
/* 4B */ { Idec,	eBX,	NOARG,	NOARG,	Po32 },
/* 4C */ { Idec,	eSP,	NOARG,	NOARG,	Po32 },
/* 4D */ { Idec,	eBP,	NOARG,	NOARG,	Po32 },
/* 4E */ { Idec,	eSI,	NOARG,	NOARG,	Po32 },
/* 4F */ { Idec,	eDI,	NOARG,	NOARG,	Po32 },
/* 50 */ { Ipush,	rAXr8,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 51 */ { Ipush,	rCXr9,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 52 */ { Ipush,	rDXr10,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 53 */ { Ipush,	rBXr11,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 54 */ { Ipush,	rSPr12,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 55 */ { Ipush,	rBPr13,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 56 */ { Ipush,	rSIr14,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 57 */ { Ipush,	rDIr15,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 58 */ { Ipop,	rAXr8,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 59 */ { Ipop,	rCXr9,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 5A */ { Ipop,	rDXr10,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 5B */ { Ipop,	rBXr11,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 5C */ { Ipop,	rSPr12,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 5D */ { Ipop,	rBPr13,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 5E */ { Ipop,	rSIr14,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 5F */ { Ipop,	rDIr15,	NOARG,	NOARG,	Po32 | Pdef64 | REX(_B) },
/* 60 */ { Ipusha,	NOARG,	NOARG,	NOARG,	Po32 | Pinv64 | Psuff },
/* 61 */ { Ipopa,	NOARG,	NOARG,	NOARG,	Po32 | Pinv64 | Psuff },
/* 62 */ { Ibound,	Gv,	E,	NOARG,	Po32 | Pa32 | Pinv64 },
/* 63 */ { Iarpl,	Ew,	Gw,	NOARG,	Pa32 | Pinv64 },
/* 64 */ { Ifs,		ES,	NOARG,	NOARG,	Pnone },
/* 65 */ { Igs,		GS,	NOARG,	NOARG,	Pnone },
/* 66 */ { Ia32,	NOARG,	NOARG,	NOARG,	Pnone },
/* 67 */ { Io32,	NOARG,	NOARG,	NOARG,	Pnone },
/* 68 */ { Ipush,	Iv,	NOARG,	NOARG,	Po32 },
/* 69 */ { Iimul,	Gv,	Ev,	Iv,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 6A */ { Ipush,	Ib,	NOARG,	NOARG,	Pnone },
/* 6B */ { Iimul,	Gv,	Ev,	Isb,	Po32 | Po32 | REX(_W|_R|_X|_B) },
/* 6C */ { Iinsb,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6D */ { Iins,	NOARG,	NOARG,	NOARG,	Po32 | Psuff },
/* 6E */ { Ioutsb,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6F */ { Iouts,	NOARG,	NOARG,	NOARG,	Po32 | Psuff },
/* 70 */ { Ijo,		Jb,	NOARG,	NOARG,	Pnone },
/* 71 */ { Ijno,	Jb,	NOARG,	NOARG,	Pnone },
/* 72 */ { Ijb,		Jb,	NOARG,	NOARG,	Pnone },
/* 73 */ { Ijnb,	Jb,	NOARG,	NOARG,	Pnone },
/* 74 */ { Ijz,		Jb,	NOARG,	NOARG,	Pnone },
/* 75 */ { Ijnz,	Jb,	NOARG,	NOARG,	Pnone },
/* 76 */ { Ijbe,	Jb,	NOARG,	NOARG,	Pnone },
/* 77 */ { Ijnbe,	Jb,	NOARG,	NOARG,	Pnone },
/* 78 */ { Ijs,		Jb,	NOARG,	NOARG,	Pnone },
/* 79 */ { Ijns,	Jb,	NOARG,	NOARG,	Pnone },
/* 7A */ { Ijp,		Jb,	NOARG,	NOARG,	Pnone },
/* 7B */ { Ijnp,	Jb,	NOARG,	NOARG,	Pnone },
/* 7C */ { Ijl,		Jb,	NOARG,	NOARG,	Pnone },
/* 7D */ { Ijnl,	Jb,	NOARG,	NOARG,	Pnone },
/* 7E */ { Ijle,	Jb,	NOARG,	NOARG,	Pnone },
/* 7F */ { Ijnle,	Jb,	NOARG,	NOARG,	Pnone },
/* 80 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* 81 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* 82 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pinv64 },
/* 83 */ { Igrp,	Ev,	Isb,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 84 */ { Itest,	Eb,	Gb,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 85 */ { Itest,	Ev,	Gv,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
/* 86 */ { Ixchg,	Eb,	Gb,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 87 */ { Ixchg,	Ev,	Gv,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
/* 88 */ { Imov,	Eb,	Gb,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 89 */ { Imov,	Ev,	Gv,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
/* 8A */ { Imov,	Gb,	Eb,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 8B */ { Imov,	Gv,	Ev,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
/* 8C */ { Imov,	Ev,	S,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 8D */ { Ilea,	Gv,	E,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 8E */ { Imov,	S,	Ev,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 8F */ { Ipop,	Ev,	NOARG,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
/* 90 */ { Ixchg,	rAXr8,	rAX,	NOARG,	Po32 | REX(_W|_B) },
/* 91 */ { Ixchg,	rCXr9,	rAX,	NOARG,	Po32 | REX(_W|_B) },
/* 92 */ { Ixchg,	rDXr10,	rAX,	NOARG,	Po32 | REX(_W|_B) },
/* 93 */ { Ixchg,	rBXr11,	rAX,	NOARG,	Po32 | REX(_W|_B) },
/* 94 */ { Ixchg,	rSPr12,	rAX,	NOARG,	Po32 | REX(_W|_B) },
/* 95 */ { Ixchg,	rBPr13,	rAX,	NOARG,	Po32 | REX(_W|_B) },
/* 96 */ { Ixchg,	rSIr14,	rAX,	NOARG,	Po32 | REX(_W|_B) },
/* 97 */ { Ixchg,	rDIr15,	rAX,	NOARG,	Po32 | REX(_W|_B) },
/* 98 */ { Icbw,	NOARG,	NOARG,	NOARG,	Po32 },
/* 99 */ { Icwd,	NOARG,	NOARG,	NOARG,	Po32 },
/* 9A */ { Icall,	Ap,	NOARG,	NOARG,	Pa32 | Pinv64 },
/* 9B */ { Iwait,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9C */ { Ipushf,	NOARG,	NOARG,	NOARG,	Po32 | Psuff },
/* 9D */ { Ipopf,	NOARG,	NOARG,	NOARG,	Po32 | Psuff },
/* 9E */ { Isahf,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9F */ { Ilahf,	NOARG,	NOARG,	NOARG,	Pnone },
/* A0 */ { Imov,	AL,	Ob,	NOARG,	Pnone },
/* A1 */ { Imov,	rAX,	Ov,	NOARG,	Po32 },
/* A2 */ { Imov,	Ob,	AL,	NOARG,	Pnone },
/* A3 */ { Imov,	Ov,	rAX, 	NOARG,	Po32 },
/* A4 */ { Imovsb,	NOARG,	NOARG,	NOARG,	Pnone },
/* A5 */ { Imovs,	NOARG,	NOARG,	NOARG,	Psuff | REX(_W) },
/* A6 */ { Icmpsb,	NOARG,	NOARG,	NOARG,	Pnone },
/* A7 */ { Icmps,	NOARG,	NOARG,	NOARG,	Psuff | REX(_W) },
/* A8 */ { Itest,	AL,	Ib,	NOARG,	Pnone },
/* A9 */ { Itest,	rAX,	Iv,	NOARG,	Po32 | REX(_W) },
/* AA */ { Istosb,	NOARG,	NOARG,	NOARG,	Pnone },
/* AB */ { Istos,	NOARG,	NOARG,	NOARG,	Psuff | REX(_W) },
/* AC */ { Ilodsb,	NOARG,	NOARG,	NOARG,	Pnone },
/* AD */ { Ilods,	NOARG,	NOARG,	NOARG,	Psuff | REX(_W) },
/* AE */ { Iscasb,	NOARG,	NOARG,	NOARG,	Pnone },
/* AF */ { Iscas,	NOARG,	NOARG,	NOARG,	Psuff | REX(_W) },
/* B0 */ { Imov,	ALr8b,	Ib,	NOARG,	REX(_B) },
/* B1 */ { Imov,	CLr9b,	Ib,	NOARG,	REX(_B) },
/* B2 */ { Imov,	DLr10b,	Ib,	NOARG,	REX(_B) },
/* B3 */ { Imov,	BLr11b,	Ib,	NOARG,	REX(_B) },
/* B4 */ { Imov,	AHr12b,	Ib,	NOARG,	REX(_B) },
/* B5 */ { Imov,	CHr13b,	Ib,	NOARG,	REX(_B) },
/* B6 */ { Imov,	DHr14b,	Ib,	NOARG,	REX(_B) },
/* B7 */ { Imov,	BHr15b,	Ib,	NOARG,	REX(_B) },
/* B8 */ { Imov,	rAXr8,	Iv,	NOARG,	Po32 | REX(_W|_B) },
/* B9 */ { Imov,	rCXr9,	Iv,	NOARG,	Po32 | REX(_W|_B) },
/* BA */ { Imov,	rDXr10,	Iv,	NOARG,	Po32 | REX(_W|_B) },
/* BB */ { Imov,	rBXr11,	Iv,	NOARG,	Po32 | REX(_W|_B) },
/* BC */ { Imov,	rSPr12,	Iv,	NOARG,	Po32 | REX(_W|_B) },
/* BD */ { Imov,	rBPr13,	Iv,	NOARG,	Po32 | REX(_W|_B) },
/* BE */ { Imov,	rSIr14,	Iv,	NOARG,	Po32 | REX(_W|_B) },
/* BF */ { Imov,	rDIr15,	Iv,	NOARG,	Po32 | REX(_W|_B) },
/* C0 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* C1 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* C2 */ { Iret,	Iw,	NOARG,	NOARG,	Pnone },
/* C3 */ { Iret,	NOARG,	NOARG,	NOARG,	Pnone },
/* C4 */ { Iles,	Gv,	E,	NOARG,	Po32 | Pa32 | Pinv64 },
/* C5 */ { Ilds,	Gv,	E,	NOARG,	Po32 | Pa32 | Pinv64 },
/* C6 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* C7 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* C8 */ { Ienter,	Iw,	Ib,	NOARG,	Pnone },
/* C9 */ { Ileave,	NOARG,	NOARG,	NOARG,	Pnone },
/* CA */ { Iretf,	Iw,	NOARG,	NOARG,	Pnone },
/* CB */ { Iretf,	NOARG,	NOARG,	NOARG,	Pnone },
/* CC */ { Iint,	I3,	NOARG,	NOARG,	Pnone },
/* CD */ { Iint,	Ib,	NOARG,	NOARG,	Pnone },
/* CE */ { Iinto,	NOARG,	NOARG,	NOARG,	Pinv64 },
/* CF */ { Iiret,	NOARG,	NOARG,	NOARG,	Psuff | REX(_W) },
/* D0 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* D1 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* D2 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* D3 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* D4 */ { Iaam,	Ib,	NOARG,	NOARG,	Pinv64 },
/* D5 */ { Iaad,	Ib,	NOARG,	NOARG,	Pinv64 },
/* D6 */ { Isalc,	NOARG,	NOARG,	NOARG,	Pinv64 },
/* D7 */ { Ixlat,	NOARG,	NOARG,	NOARG,	Pnone },
/* D8 */ { Ix87,	NOARG,	NOARG,	NOARG,	Pnone },
/* D9 */ { Ix87,	NOARG,	NOARG,	NOARG,	Pnone },
/* DA */ { Ix87,	NOARG,	NOARG,	NOARG,	Pnone },
/* DB */ { Ix87,	NOARG,	NOARG,	NOARG,	Pnone },
/* DC */ { Ix87,	NOARG,	NOARG,	NOARG,	Pnone },
/* DD */ { Ix87,	NOARG,	NOARG,	NOARG,	Pnone },
/* DE */ { Ix87,	NOARG,	NOARG,	NOARG,	Pnone },
/* DF */ { Ix87,	NOARG,	NOARG,	NOARG,	Pnone },
/* E0 */ { Iloopn,	Jb,	NOARG,	NOARG,	Pnone },
/* E1 */ { Iloope,	Jb,	NOARG,	NOARG,	Pnone },
/* E2 */ { Iloop,	Jb,	NOARG,	NOARG,	Pnone },
/* E3 */ { Ijcxz,	Jb,	NOARG,	NOARG,	Pnone },
/* E4 */ { Iin,		AL,	Ib,	NOARG,	Pnone },
/* E5 */ { Iin,		eAX,	Ib,	NOARG,	Pnone },
/* E6 */ { Iout,	Ib,	AL,	NOARG,	Pnone },
/* E7 */ { Iout,	Ib,	eAX,	NOARG,	Pnone },
/* E8 */ { Icall,	Jv,	NOARG,	NOARG,	Po32  },
/* E9 */ { Ijmp,	Jv,	NOARG,	NOARG,	Po32  },
/* EA */ { Ijmp,	Ap,	NOARG,	NOARG,	Pinv64 },
/* EB */ { Ijmp,	Jb,	NOARG,	NOARG,	Pnone },
/* EC */ { Iin,		AL,	DX,	NOARG,	Pnone },
/* ED */ { Iin,		eAX,	DX,	NOARG,	Pnone },
/* EE */ { Iout,	DX,	AL,	NOARG,	Pnone },
/* ED */ { Iout,	DX,	eAX,	NOARG,	Pnone },
/* F0 */ { Ilock,	NOARG,	NOARG,	NOARG,	Pnone },
/* F1 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F2 */ { Irepne,	NOARG,	NOARG,	NOARG,	Pnone },
/* F3 */ { Irep,	NOARG,	NOARG,	NOARG,	Pnone },
/* F4 */ { Ihlt,	NOARG,	NOARG,	NOARG,	Pnone },
/* F5 */ { Icmc,	NOARG,	NOARG,	NOARG,	Pnone },
/* F6 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* F7 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* F8 */ { Iclc,	NOARG,	NOARG,	NOARG,	Pnone },
/* F9 */ { Istc,	NOARG,	NOARG,	NOARG,	Pnone },
/* FA */ { Icli,	NOARG,	NOARG,	NOARG,	Pnone },
/* FB */ { Isti,	NOARG,	NOARG,	NOARG,	Pnone },
/* FC */ { Icld,	NOARG,	NOARG,	NOARG,	Pnone },
/* FD */ { Istd,	NOARG,	NOARG,	NOARG,	Pnone },
/* FE */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* FF */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone }

};


/* prefix n/a
   This is the table for 2byte no-prefix opcodes
 */
x86_TableEntry insntbl_2byte[0x100] = {

	/* Instruction, op1, op2, op3, Valid Prefixes */

/* 00 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* 01 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* 02 */ { Ilar,	Gv,	Ew,	NOARG,	Po32  },
/* 03 */ { Ilsl,	Gv,	Ew,	NOARG,	Po32  },
/* 04 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 05 */ { Isyscall,	NOARG,	NOARG,	NOARG,	Pnone },
/* 06 */ { Iclts,	NOARG,	NOARG,	NOARG,	Pnone },
/* 07 */ { Isysret,	NOARG,	NOARG,	NOARG,	Pnone },
/* 08 */ { Iinvd,	NOARG,	NOARG,	NOARG,	Pnone },
/* 09 */ { Iwbinvd,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0B */ { Iud2,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0D */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0E */ { Ifemms,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0F */ { I3dnow,	NOARG,	NOARG,	NOARG,	Pnone },
/* 10 */ { Imovups,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 11 */ { Imovups,	W,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 12 */ { Imovlps,	V,	M,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 13 */ { Imovlps,	M,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 14 */ { Iunpcklps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 15 */ { Iunpckhps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 16 */ { Imovhps,	V,	M,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 17 */ { Imovhps,	M,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 18 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone  },
/* 19 */ { Inop,	NOARG,	NOARG,	NOARG,	Pinv64 },
/* 1A */ { Inop,	NOARG,	NOARG,	NOARG,	Pinv64 },
/* 1B */ { Inop,	NOARG,	NOARG,	NOARG,	Pinv64 },
/* 1C */ { Inop,	NOARG,	NOARG,	NOARG,	Pinv64 },
/* 1D */ { Inop,	NOARG,	NOARG,	NOARG,	Pinv64 },
/* 1E */ { Inop,	NOARG,	NOARG,	NOARG,	Pinv64 },
/* 1F */ { Inop,	NOARG,	NOARG,	NOARG,	Pinv64 },
/* 20 */ { Imov,	R,	C,	NOARG,	Pnone },
/* 21 */ { Imov,	R,	D,	NOARG,	Pnone },
/* 22 */ { Imov,	C,	R,	NOARG,	Pnone },
/* 23 */ { Imov,	D,	R,	NOARG,	Pnone },
/* 24 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 25 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 26 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 27 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 28 */ { Imovaps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 29 */ { Imovaps,	W,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2A */ { Icvtpi2ps,	V,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2B */ { Imovntps,	M,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2C */ { Icvttps2pi,	P,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2D */ { Icvtps2pi,	P,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2E */ { Iucomiss,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2F */ { Icomiss,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 30 */ { Iwrmsr,	NOARG,	NOARG,	NOARG,	},
/* 31 */ { Irdtsc,	NOARG,	NOARG,	NOARG,	},
/* 32 */ { Irdmsr,	NOARG,	NOARG,	NOARG,	},
/* 33 */ { Irdpmc,	NOARG,	NOARG,	NOARG,	},
/* 34 */ { Isysenter,	NOARG,	NOARG,	NOARG,	Pinv64 },
/* 35 */ { Isysexit,	NOARG,	NOARG,	NOARG,	Pinv64 },
/* 36 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 37 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 38 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 39 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 40 */ { Icmovo,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 41 */ { Icmovno,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 42 */ { Icmovb,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 43 */ { Icmovnb,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 44 */ { Icmovz,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 45 */ { Icmovnz,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 46 */ { Icmovbe,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 47 */ { Icmovnbe,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 48 */ { Icmovs,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 49 */ { Icmovns,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 4A */ { Icmovp,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 4B */ { Icmovnp,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 4C */ { Icmovl,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 4D */ { Icmovnl,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 4E */ { Icmovle,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 4F */ { Icmovnle,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* 50 */ { Imovmskps,	Gv,	VR,	NOARG,	Po32 | REX(_W|_R|_B) },
/* 51 */ { Isqrtps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 52 */ { Irsqrtps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 53 */ { Ircpps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 54 */ { Iandps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 55 */ { Iandnps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 56 */ { Iorps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 57 */ { Ixorps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 58 */ { Iaddps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 59 */ { Imulps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5A */ { Icvtps2pd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5B */ { Icvtdq2ps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5C */ { Isubps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5D */ { Iminps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5E */ { Idivps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5F */ { Imaxps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 60 */ { Ipunpcklbw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 61 */ { Ipunpcklwd,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 62 */ { Ipunpckldq,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 63 */ { Ipackusdw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 64 */ { Ipcmpgtb,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 65 */ { Ipcmpgtw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 66 */ { Ipcmpgtd,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 67 */ { Ipacksswb,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 68 */ { Ipunpckhbw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 69 */ { Ipunpckhwd,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 6A */ { Ipunpckhdq,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 6B */ { Ipackssdw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 6C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6E */ { Imovd,	P,	Ex,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 6F */ { Imovq,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 70 */ { Ipshufw,	P,	Q,	Ib,	Pa32 | REX(_R|_X|_B) },
/* 71 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* 72 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* 73 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* 74 */ { Ipcmpeqb,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 75 */ { Ipcmpeqw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 76 */ { Ipcmpeqd,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 77 */ { Iemms,	NOARG,	NOARG,	NOARG,	Pnone },
/* 78 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 79 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7E */ { Imovd,	Ex,	P,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 7F */ { Imovq,	Q,	P,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 80 */ { Ijo,		Jz,	NOARG,	NOARG,	Po32 },
/* 81 */ { Ijno,	Jz,	NOARG,	NOARG,	Po32 },
/* 82 */ { Ijb,		Jz,	NOARG,	NOARG,	Po32 },
/* 83 */ { Ijnb,	Jz,	NOARG,	NOARG,	Po32 },
/* 84 */ { Ijz,		Jz,	NOARG,	NOARG,	Po32 },
/* 85 */ { Ijnz,	Jz,	NOARG,	NOARG,	Po32 },
/* 86 */ { Ijbe,	Jz,	NOARG,	NOARG,	Po32 },
/* 87 */ { Ijnbe,	Jz,	NOARG,	NOARG,	Po32 },
/* 88 */ { Ijs,		Jz,	NOARG,	NOARG,	Po32 },
/* 89 */ { Ijns,	Jz,	NOARG,	NOARG,	Po32 },
/* 8A */ { Ijp,		Jz,	NOARG,	NOARG,	Po32 },
/* 8B */ { Ijnp,	Jz,	NOARG,	NOARG,	Po32 },
/* 8C */ { Ijl,		Jz,	NOARG,	NOARG,	Po32 },
/* 8D */ { Ijnl,	Jz,	NOARG,	NOARG,	Po32 },
/* 8E */ { Ijle,	Jz,	NOARG,	NOARG,	Po32 },
/* 8F */ { Ijnle,	Jz,	NOARG,	NOARG,	Po32 },
/* 90 */ { Iseto,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 91 */ { Isetno,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 92 */ { Isetb,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 93 */ { Isetnb,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 94 */ { Isetz,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 95 */ { Isetnz,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 96 */ { Isetbe,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 97 */ { Isetnbe,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 98 */ { Isets,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 99 */ { Isetns,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 9A */ { Isetp,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 9B */ { Isetnp,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 9C */ { Isetl,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 9D */ { Isetnl,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 9E */ { Isetle,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 9F */ { Isetnle,	Eb,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* A0 */ { Ipush,	FS,	NOARG,	NOARG,	Pnone },
/* A1 */ { Ipop,	FS,	NOARG,	NOARG,	Pnone },
/* A2 */ { Icpuid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A3 */ { Ibt,		Ev,	Gv,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* A4 */ { Ishld,	Ev,	Gv,	Ib,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* A5 */ { Ishld,	Ev,	Gv,	CL,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* A6 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A7 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A8 */ { Ipush,	GS,	NOARG,	NOARG,	Pnone },
/* A9 */ { Ipop,	GS,	NOARG,	NOARG,	Pnone },
/* AA */ { Irsm,	NOARG,	NOARG,	NOARG,	Pnone },
/* AB */ { Ibts,	Ev,	Gv,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* AC */ { Ishrd,	Ev,	Gv,	Ib,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* AD */ { Ishrd,	Ev,	Gv,	CL,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* AE */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* AF */ { Iimul,	Gv,	Ev,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* B0 */ { Icmpxchg,	Eb,	Gb,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
/* B1 */ { Icmpxchg,	Ev,	Gv,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* B2 */ { Ilss,	Gv,	E,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* B3 */ { Ibtr,	Ev,	Gv,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* B4 */ { Ilfs,	Gv,	E,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* B5 */ { Ilgs,	Gv,	E,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* B6 */ { Imovzx,	Gv,	Eb,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* B7 */ { Imovzx,	Gv,	Ew,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* B8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B9 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* BA */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* BB */ { Ibtc,	Ev,	Gv,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* BC */ { Ibsf,	Ev,	Gv,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* BD */ { Ibsr,	Ev,	Gv,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* BE */ { Imovsx,	Gv,	Eb,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* BF */ { Imovsx,	Gv,	Ew,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* C0 */ { Ixadd,	Eb,	Gb,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* C1 */ { Ixadd,	Ev,	Gv,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* C2 */ { Icmpps,	V,	W,	Ib,	Pa32 | REX(_R|_X|_B) },
/* C3 */ { Imovnti,	M,	Gx,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
/* C4 */ { Ipinsrw,	P,	Ew,	Ib,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* C5 */ { Ipextrw,	Gd,	PR,	Ib,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* C6 */ { Ishufps,	V,	W,	Ib,	Pa32 | REX(_R|_X|_B) },
/* C7 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* C8 */ { Ibswap,	rAXr8,	NOARG,	NOARG,	Po32 | REX(_W|_B) },
/* C9 */ { Ibswap,	rCXr9,	NOARG,	NOARG,	Po32 | REX(_W|_B) },
/* CA */ { Ibswap,	rDXr10,	NOARG,	NOARG,	Po32 | REX(_W|_B) },
/* CB */ { Ibswap,	rBXr11,	NOARG,	NOARG,	Po32 | REX(_W|_B) },
/* CC */ { Ibswap,	rSPr12,	NOARG,	NOARG,	Po32 | REX(_W|_B) },
/* CD */ { Ibswap,	rBPr13,	NOARG,	NOARG,	Po32 | REX(_W|_B) },
/* CE */ { Ibswap,	rSIr14,	NOARG,	NOARG,	Po32 | REX(_W|_B) },
/* CF */ { Ibswap,	rDIr15,	NOARG,	NOARG,	Po32 | REX(_W|_B) },
/* D0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D1 */ { Ipsrlw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* D2 */ { Ipsrld,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* D3 */ { Ipsrlq,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* D4 */ { Ipaddq,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* D5 */ { Ipmullw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* D6 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D7 */ { Ipmovmskb,	Gd,	PR,	NOARG,	Pnone },
/* D8 */ { Ipsubusb,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* D9 */ { Ipsubusw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* DA */ { Ipminub,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* DB */ { Ipand,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* DC */ { Ipaddusb,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* DD */ { Ipaddusw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* DE */ { Ipmaxub,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* DF */ { Ipandn,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E0 */ { Ipavgb,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E1 */ { Ipsraw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E2 */ { Ipsrad,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E3 */ { Ipavgw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E4 */ { Ipmulhuw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E5 */ { Ipmulhw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E6 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E7 */ { Imovntq,	M,	P,	NOARG,	Pnone },
/* E8 */ { Ipsubsb,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E9 */ { Ipsubsw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* EA */ { Ipminsw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* EB */ { Ipor,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* EC */ { Ipaddsb,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* ED */ { Ipaddsw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* EE */ { Ipmaxsw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* EF */ { Ipxor,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F1 */ { Ipsllw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F2 */ { Ipslld,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F3 */ { Ipsllq,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F4 */ { Ipmuludq,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F5 */ { Ipmaddwd,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F6 */ { Ipsadbw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F7 */ { Imaskmovq,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F8 */ { Ipsubb,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F9 */ { Ipsubw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* FA */ { Ipsubd,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* FB */ { Ipsubq,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* FC */ { Ipaddb,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* FD */ { Ipaddw,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* FE */ { Ipaddd,	P,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* FF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone }
};

x86_TableEntry insntbl_2byte_prefixF3[0x100] = {
/* 00 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 01 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 02 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 03 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 04 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 05 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 06 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 07 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 08 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 09 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 10 */ { Imovss,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 11 */ { Imovss,	W,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 12 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 13 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 14 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 15 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 16 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 17 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 18 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 19 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 20 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 21 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 22 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 23 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 24 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 25 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 26 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 27 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 28 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 29 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 2A */ { Icvtsi2ss,	V,	Evw,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 2C */ { Icvttsi2ss,	Gvw,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2D */ { Icvtss2si,	Gvw,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 2F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 30 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 31 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 32 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 33 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 34 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 35 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 36 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 37 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 38 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 39 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 40 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 41 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 42 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 43 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 44 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 45 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 46 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 47 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 48 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 49 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 50 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 51 */ { Isqrtss,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 52 */ { Irsqrtss,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 53 */ { Ircpss,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 54 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 55 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 56 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 57 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 58 */ { Iaddss,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 59 */ { Imulss,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5A */ { Icvtss2sd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5B */ { Icvttps2dq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5C */ { Isubss,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5D */ { Iminss,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5E */ { Idivss,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5F */ { Imaxss,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 60 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 61 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 62 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 63 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 64 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 65 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 66 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 67 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 68 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 69 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6F */ { Imovdqu,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 70 */ { Ipshufhw,	V,	W,	Ib,	Pa32 | REX(_R|_X|_B) },
/* 71 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 72 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 73 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 74 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 75 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 76 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 77 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 78 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 79 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7E */ { Imovq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 7F */ { Imovdqu,	W,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 80 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 81 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 82 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 83 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 84 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 85 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 86 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 87 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 88 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 89 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 90 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 91 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 92 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 93 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 94 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 95 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 96 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 97 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 98 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 99 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A1 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A2 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A6 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A7 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AD */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B1 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B2 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B6 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B7 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BD */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C0 */ { Ixadd,	Eb,	Gb,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
/* C1 */ { Ixadd,	Ev,	Gv,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
/* C2 */ { Icmpss,	V,	W,	Ib,	Pa32 | REX(_R|_X|_B) },
/* C3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C6 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C7 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* C8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CD */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D1 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D2 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D6 */ { Imovq2dq,	V,	PR,	NOARG,	Pa32  },
/* D7 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* DA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* DB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* DC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* DD */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* DE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E1 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E2 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E6 */ { Icvtdq2pd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E7 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* EA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* EB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* EC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* ED */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* EE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* EF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F1 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F2 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F6 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F7 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* FA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* FB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* FC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* FD */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* FE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* FF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone }
};

x86_TableEntry insntbl_2byte_prefix66[0x100] = {
/* 00 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 01 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 02 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 03 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 04 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 05 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 06 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 07 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 08 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 09 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 10 */ { Imovupd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 11 */ { Imovupd,	W,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 12 */ { Imovlpd,	V,	M,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 13 */ { Imovlpd,	M,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 14 */ { Iunpcklps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 15 */ { Iunpckhps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 16 */ { Imovhps,	V,	M,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 17 */ { Imovhps,	M,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 18 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 19 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 20 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 21 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 22 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 23 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 24 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 25 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 26 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 27 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 28 */ { Imovapd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 29 */ { Imovapd,	W,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2A */ { Icvtpi2pd,	V,	Q,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2B */ { Imovntpd,	M,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2C */ { Icvttpd2pi,	P,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2D */ { Icvtpd2pi,	P,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2E */ { Iucomisd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2F */ { Icomisd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 30 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 31 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 32 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 33 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 34 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 35 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 36 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 37 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 38 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 39 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 40 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 41 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 42 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 43 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 44 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 45 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 46 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 47 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 48 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 49 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 50 */ { Imovmskpd,	Gv,	VR,	NOARG,	Po32 | REX(_W|_R|_B) },
/* 51 */ { Isqrtpd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 52 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 53 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 54 */ { Iandpd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 55 */ { Iandnpd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 56 */ { Iorpd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 57 */ { Ixorpd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 58 */ { Iaddpd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 59 */ { Imulpd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5A */ { Icvtpd2ps,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5B */ { Icvtps2dq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5C */ { Isubpd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5D */ { Iminpd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5E */ { Idivpd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5F */ { Imaxpd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 60 */ { Ipunpcklbw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 61 */ { Ipunpcklwd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 62 */ { Ipunpckldq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 63 */ { Ipackusdw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 64 */ { Ipcmpgtb,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 65 */ { Ipcmpgtw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 66 */ { Ipcmpgtd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 67 */ { Ipacksswb,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 68 */ { Ipunpckhbw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 69 */ { Ipunpckhwd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 6A */ { Ipunpckhdq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 6B */ { Ipackssdw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 6C */ { Ipunpcklqdq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 6D */ { Ipunpckhqdq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 6E */ { Imovd,	V,	Ex,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 6F */ { Imovqa,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 70 */ { Ipshufd,	V,	W,	Ib,	Pa32 | REX(_R|_X|_B) },
/* 71 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* 72 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* 73 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* 74 */ { Ipcmpeqb,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 75 */ { Ipcmpeqw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 76 */ { Ipcmpeqd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 77 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 78 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 79 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7E */ { Imovd,	Evw,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 7F */ { Imovdqa,	W,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 80 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 81 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 82 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 83 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 84 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 85 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 86 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 87 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 88 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 89 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 90 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 91 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 92 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 93 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 94 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 95 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 96 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 97 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 98 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 99 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A1 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A2 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A6 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A7 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AD */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B1 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B2 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B6 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B7 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BD */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C0 */ { Ixadd,	Eb,	Gb,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
/* C1 */ { Ixadd,	Ev,	Gv,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
/* C2 */ { Icmppd,	V,	W,	Ib,	Pa32 | REX(_R|_X|_B) },
/* C3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C4 */ { Ipinsrw,	V,	Ew,	Ib,	Pa32 | REX(_W|_R|_X|_B) },
/* C5 */ { Ipextrw,	Gd,	VR,	Ib,	Pa32  },
/* C6 */ { Ishufpd,	V,	W,	Ib,	Pa32 | REX(_R|_X|_B) },
/* C7 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* C8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CD */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D1 */ { Ipsrlw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* D2 */ { Ipsrld,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* D3 */ { Ipsrlq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* D4 */ { Ipaddq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* D5 */ { Ipmullw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* D6 */ { Iinvalid,	W,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* D7 */ { Ipmovmskb,	Gd,	VR,	NOARG,	Pnone },
/* D8 */ { Ipsubusb,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* D9 */ { Ipsubusw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* DA */ { Ipminub,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* DB */ { Ipand,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* DC */ { Ipaddusb,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* DD */ { Ipaddusw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* DE */ { Ipmaxub,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* DF */ { Ipandn,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E0 */ { Ipavgb,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E1 */ { Ipsraw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E2 */ { Ipsrad,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E3 */ { Ipavgw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E4 */ { Ipmulhuw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E5 */ { Ipmulhw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E6 */ { Icvttpd2dq,	V,	W,	NOARG,	Pnone },
/* E7 */ { Imovntdq,	M,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E8 */ { Ipsubsb,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E9 */ { Ipsubsw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* EA */ { Ipminsw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* EB */ { Ipor,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* EC */ { Ipaddsb,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* ED */ { Ipaddsw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* EE */ { Ipmaxsw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* EF */ { Ipxor,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F1 */ { Ipsllw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F2 */ { Ipslld,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F3 */ { Ipsllq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F4 */ { Ipmuludq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F5 */ { Ipmaddwd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F6 */ { Ipsadbw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F7 */ { Imaskmovq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F8 */ { Ipsubb,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* F9 */ { Ipsubw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* FA */ { Ipsubd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* FB */ { Ipsubq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* FC */ { Ipaddb,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* FD */ { Ipaddw,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* FE */ { Ipaddd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* FF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone }
};

x86_TableEntry insntbl_2byte_prefixF2[0x100] = {
/* 00 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 01 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 02 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 03 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 04 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 05 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 06 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 07 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 08 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 09 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 0F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 10 */ { Imovsd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 11 */ { Imovsd,	W,	V,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 12 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 13 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 14 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 15 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 16 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 17 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 18 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 19 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 1F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 20 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 21 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 22 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 23 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 24 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 25 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 26 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 27 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 28 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 29 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 2A */ { Icvtsi2sd,	V,	Evw,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
/* 2B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 2C */ { Icvttsi2sd,	Gvw,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2D */ { Icvtsd2si,	Gvw,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 2E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 2F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 30 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 31 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 32 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 33 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 34 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 35 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 36 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 37 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 38 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 39 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 3F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 40 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 41 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 42 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 43 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 44 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 45 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 46 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 47 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 48 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 49 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 4F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 50 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 51 */ { Isqrtsd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 52 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 53 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 54 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 55 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 56 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 57 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 58 */ { Iaddsd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 59 */ { Imulsd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5A */ { Icvtsd2ss,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 5C */ { Isubsd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5D */ { Iminsd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5E */ { Idivsd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 5F */ { Imaxsd,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* 60 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 61 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 62 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 63 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 64 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 65 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 66 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 67 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 68 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 69 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 6F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 70 */ { Ipshuflw,	V,	W,	Ib,	Pa32 | REX(_R|_X|_B) },
/* 71 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 72 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 73 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 74 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 75 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 76 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 77 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 78 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 79 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 7F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 80 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 81 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 82 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 83 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 84 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 85 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 86 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 87 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 88 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 89 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 8F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 90 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 91 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 92 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 93 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 94 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 95 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 96 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 97 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 98 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 99 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9A */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9B */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9C */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9D */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9E */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* 9F */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A1 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A2 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A6 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A7 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* A9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AD */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* AF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B1 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B2 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B6 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B7 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* B9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BD */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* BF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C0 */ { Ixadd,	Eb,	Gb,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
/* C1 */ { Ixadd,	Ev,	Gv,	NOARG,	Po32 | Pa32 | REX(_R|_X|_B) },
/* C2 */ { Icmpsd,	V,	W,	Ib,	Pa32 | REX(_R|_X|_B) },
/* C3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C6 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C7 */ { Igrp,	NOARG,	NOARG,	NOARG,	Pnone },
/* C8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* C9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CD */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* CF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D1 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D2 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D6 */ { Imovdq2q,	P,	VR,	NOARG,	Pa32  },
/* D7 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* D9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* DA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* DB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* DC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* DD */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* DE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* DF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E1 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E2 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E6 */ { Icvtpd2dq,	V,	W,	NOARG,	Pa32 | REX(_R|_X|_B) },
/* E7 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* E9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* EA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* EB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* EC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* ED */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* EE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* EF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F0 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F1 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F2 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F3 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F4 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F5 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F6 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F7 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F8 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* F9 */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* FA */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* FB */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* FC */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* FD */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* FE */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
/* FF */ { Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone }
};

x86_TableEntry insntbl_group1_op80[0x8] = {
	{ Iadd,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ior,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Iadc,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Isbb,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Iand,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Isub,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ixor,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Icmp,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) }
};

x86_TableEntry insntbl_group1_op81[0x8] = {
	{ Iadd,		Ez,	Iz,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ior,		Ez,	Iz,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Iadc,		Ez,	Iz,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Isbb,		Ez,	Iz,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Iand,		Ez,	Iz,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Isub,		Ez,	Iz,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ixor,		Ez,	Iz,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Icmp,		Ez,	Iz,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) }
};

x86_TableEntry insntbl_group1_op82[0x8] = {
	{ Iadd,		Eb,	Ib,	NOARG,	Pinv64 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ior,		Eb,	Ib,	NOARG,	Pinv64 | Pa32 | REX(_W|_R|_X|_B) },
	{ Iadc,		Eb,	Ib,	NOARG,	Pinv64 | Pa32 | REX(_W|_R|_X|_B) },
	{ Isbb,		Eb,	Ib,	NOARG,	Pinv64 | Pa32 | REX(_W|_R|_X|_B) },
	{ Iand,		Eb,	Ib,	NOARG,	Pinv64 | Pa32 | REX(_W|_R|_X|_B) },
	{ Isub,		Eb,	Ib,	NOARG,	Pinv64 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ixor,		Eb,	Ib,	NOARG,	Pinv64 | Pa32 | REX(_W|_R|_X|_B) },
	{ Icmp,		Eb,	Ib,	NOARG,	Pinv64 | Pa32 | REX(_W|_R|_X|_B) }
};

x86_TableEntry insntbl_group1A_op8F[0x8] = {
	{ Ipop,		Ev,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone }
};


x86_TableEntry insntbl_group1_op83[0x8] = {
	{ Iadd,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_R|_X|_B) },
	{ Ior,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_R|_X|_B) },
	{ Iadc,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_R|_X|_B) },
	{ Isbb,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_R|_X|_B) },
	{ Iand,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_R|_X|_B) },
	{ Isub,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_R|_X|_B) },
	{ Ixor,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_R|_X|_B) },
	{ Icmp,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_R|_X|_B) }
};

x86_TableEntry insntbl_group2_opC0[0x8] = {
	{ Iror,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Irol,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ircl,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ircr,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ishl,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ishr,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Isal,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Isar,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) }
};

x86_TableEntry insntbl_group2_opC1[0x8] = {
	{ Iror,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Irol,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ircl,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ircr,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ishl,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ishr,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Isal,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Isar,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) }
};

x86_TableEntry insntbl_group2_opD0[0x8] = {
	{ Iror,		Eb,	I1,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Irol,		Eb,	I1,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ircl,		Eb,	I1,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ircr,		Eb,	I1,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ishl,		Eb,	I1,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ishr,		Eb,	I1,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Isal,		Eb,	I1,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Isar,		Eb,	I1,	NOARG,	Pa32 | REX(_W|_R|_X|_B) }
};

x86_TableEntry insntbl_group2_opD1[0x8] = {
	{ Iror,		Ev,	I1,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Irol,		Ev,	I1,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ircl,		Ev,	I1,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ircr,		Ev,	I1,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ishl,		Ev,	I1,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ishr,		Ev,	I1,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Isal,		Ev,	I1,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Isar,		Ev,	I1,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) }
};

x86_TableEntry insntbl_group2_opD2[0x8] = {
	{ Iror,		Eb,	CL,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Irol,		Eb,	CL,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ircl,		Eb,	CL,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ircr,		Eb,	CL,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ishl,		Eb,	CL,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ishr,		Eb,	CL,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Isal,		Eb,	CL,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Isar,		Eb,	CL,	NOARG,	Pa32 | REX(_W|_R|_X|_B) }
};

x86_TableEntry insntbl_group2_opD3[0x8] = {
	{ Iror,		Ev,	CL,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Irol,		Ev,	CL,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ircl,		Ev,	CL,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ircr,		Ev,	CL,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ishl,		Ev,	CL,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ishr,		Ev,	CL,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Isal,		Ev,	CL,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Isar,		Ev,	CL,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) }
};

x86_TableEntry insntbl_group3_opF6[0x8] = {
	{ Itest,	Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Itest,	Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Inot,		Eb,	NOARG,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ineg,		Eb,	NOARG,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Imul,		Eb,	NOARG,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Iimul,	Eb,	NOARG,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Idiv,		Eb,	NOARG,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Iidiv,	Eb,	NOARG,	NOARG,	Pa32 | REX(_W|_R|_X|_B) }
};

x86_TableEntry insntbl_group3_opF7[0x8] = {
	{ Itest,	Ev,	Iz,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Itest,	Ev,	Iz,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Inot,		Ev,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ineg,		Ev,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Imul,		Ev,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Iimul,	Ev,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Idiv,		Ev,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Iidiv,	Ev,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) }
};

x86_TableEntry insntbl_group4_opFE[0x8] = {
	{ Iinc,		Eb,	NOARG,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Idec,		Eb,	NOARG,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone }
};

x86_TableEntry insntbl_group5_opFF[0x8] = {
	{ Iinc,		Evw,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Idec,		Evw,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Icall,	Evw,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Icall,	Ep,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ijmp,		Evw,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ijmp,		Ep,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ipush,	Evw,	NOARG,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Iinvalid,	NOARG, 	NOARG,	NOARG,	Pnone }
};

/* group 6 */
x86_TableEntry insntbl_group6_op0F00[0x8] = {
	{ Isldt,	E,	NOARG,	NOARG,	Po32 | Pa32 | REX(_R|_X|_B) },
	{ Istr,		E,	NOARG,	NOARG,	Po32 | Pa32 | REX(_R|_X|_B) },
	{ Illdt,	E,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iltr,		E,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iverr,	E,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iverw,	E,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone }
};


/* group 7  */
x86_TableEntry insntbl_group7_op0F01[0x8] = {
	{ Isgdt,	M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Isidt,	M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ilgdt,	M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ilidt,	M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ismsw,	E,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ilmsw,	E,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iinvlpg,	M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) }
};

/* group 8  */
x86_TableEntry insntbl_group8_op0FBA[0x8] = {
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ibt,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ibts,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ibtr,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Ibtc,		Ev,	Ib,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
};

/* group 9  */
x86_TableEntry insntbl_group9_op0FC7[0x8] = {
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Icmpxchg8b,	Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
};

/* group A  */
x86_TableEntry insntbl_groupA_op0FB9[0x8] = {
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
};

/* group B  */
x86_TableEntry insntbl_groupB_opC6[0x8] = {
	{ Imov,		Eb,	Ib,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
};

/* group B  */
x86_TableEntry insntbl_groupB_opC7[0x8] = {
	{ Imov,		Ev,	Iz,	NOARG,	Po32 | Pa32 | REX(_W|_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
};

/* group C  */
x86_TableEntry insntbl_groupC_op0F71[0x8] = {
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsrlw,	PR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsraw,	PR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsllw,	PR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
};

/* group C  */
x86_TableEntry insntbl_groupC_op0F71_prefix66[0x8] = {
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsrlw,	VR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsraw,	VR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsllw,	VR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
};

/* group D  */
x86_TableEntry insntbl_groupD_op0F72[0x8] = {
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsrld,	PR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsrad,	PR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipslld,	PR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
};

/* group D, prefixed by 0x66  */
x86_TableEntry insntbl_groupD_op0F72_prefix66[0x8] = {
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsrld,	VR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsrad,	VR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipslld,	VR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
};

/* group E  */
x86_TableEntry insntbl_groupE_op0F73[0x8] = {
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsrlq,	PR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsraq,	PR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsllq,	PR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
};

/* group F  */
x86_TableEntry insntbl_groupF_op0FAE[0x8] = {
	{ Ifxsave,	M,	NOARG,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ifxrstor,	M,	NOARG,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Ildmxcsr,	Md,	NOARG,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Istmxcsr,	Md,	NOARG,	NOARG,	Pa32 | REX(_W|_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
};

/* group E, prefixed by 0x66  */
x86_TableEntry insntbl_groupE_op0F73_prefix66[0x8] = {
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsrlq,	VR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsraq,	VR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ipsllq,	VR,	Ib,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
};

/* D8 Opcode Map */
x86_TableEntry insntbl_x87_opD8reg[0x8] = {
	{ Ifadd,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifmul,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifcom,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifcomp,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifsub,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifsubr,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifdiv,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifdivr,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) }
};

/* D9 Opcode Map */
x86_TableEntry insntbl_x87_opD9reg[0x8] = {
	{ Ifld,		Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifst,		Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifstp,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifldenv,	M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifldcw,	Mw,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifstenv,	M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifstcw,	Mw,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) }
};

/* DA Opcode Map */
x86_TableEntry insntbl_x87_opDAreg[0x8] = {
	{ Ifiadd,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifimul,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ificom,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ificomp,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifisub,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifisubr,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifidiv,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifidivr,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) }
};

/* DB Opcode Map */
x86_TableEntry insntbl_x87_opDBreg[0x8] = {
	{ Ifild,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifist,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifistp,	Md,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifld,		M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iinvalid,  	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifstp,	M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) }
};

/* DC Opcode Map */
x86_TableEntry insntbl_x87_opDCreg[0x8] = {
	{ Ifadd,	Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifmul,	Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifcom,	Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifcomp,	Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifsub,	Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifsubr,	Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifdiv,	Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifdivr,	Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) }
};

/* DD Opcode Map */
x86_TableEntry insntbl_x87_opDDreg[0x8] = {
	{ Ifld,		Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifst,		Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifstp,	Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifrstor,	M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifsave,	M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifstsw,	M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) }
};

/* DE Opcode Map */
x86_TableEntry insntbl_x87_opDEreg[0x8] = {
	{ Ifiadd,	Mw,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifimul,	Mw,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ificom,	Mw,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ificomp,	Mw,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifisub,	Mw,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifisubr,	Mw,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifidiv,	Mw,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifidivr,	Mw,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) }
};

/* DF Opcode Map */
x86_TableEntry insntbl_x87_opDFreg[0x8] = {
	{ Ifild,	Mw,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifist,	Mw,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifistp,	Mw,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifbld,	M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifild,	Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifbstp,	M,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) },
	{ Ifistp,	Mq,	NOARG,	NOARG,	Pa32 | REX(_R|_X|_B) }
};

/* D8 Opcode Map */
x86_TableEntry insntbl_x87_opD8[0x8*0x8] = {
	{ Ifadd,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifadd,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifadd,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifadd,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifadd,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifadd,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifadd,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifadd,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifmul,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifmul,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifmul,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifmul,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifmul,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifmul,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifmul,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifmul,	ST0,	ST7,	NOARG,	Pnone },
  	{ Ifcom,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifcom,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifcom,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifcom,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifcom,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifcom,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifcom,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifcom,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifcomp,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifcomp,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifcomp,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifcomp,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifcomp,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifcomp,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifcomp,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifcomp,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifsub,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifsub,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifsub,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifsub,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifsub,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifsub,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifsub,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifsub,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifsubr,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifsubr,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifsubr,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifsubr,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifsubr,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifsubr,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifsubr,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifsubr,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifdiv,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifdiv,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifdiv,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifdiv,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifdiv,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifdiv,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifdiv,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifdiv,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifdivr,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifdivr,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifdivr,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifdivr,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifdivr,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifdivr,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifdivr,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifdivr,	ST0,	ST7,	NOARG,	Pnone }
};

/* D9 Opcode Map */
x86_TableEntry insntbl_x87_opD9[0x8*0x8] = {
	{ Ifld,		ST0,	ST0,	NOARG,	Pnone },
	{ Ifld,		ST0,	ST1,	NOARG,	Pnone },
	{ Ifld,		ST0,	ST2,	NOARG,	Pnone },
	{ Ifld,		ST0,	ST3,	NOARG,	Pnone },
	{ Ifld,		ST0,	ST4,	NOARG,	Pnone },
	{ Ifld,		ST0,	ST5,	NOARG,	Pnone },
	{ Ifld,		ST0,	ST6,	NOARG,	Pnone },
	{ Ifld,		ST0,	ST7,	NOARG,	Pnone },
	{ Ifxch,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifxch,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifxch,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifxch,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifxch,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifxch,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifxch,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifxch,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifnop,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Inone,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Inone,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Inone,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Inone,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Inone,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Inone,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Inone,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Inone,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifchs,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifabs,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iftst,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifxam,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifld1,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifldl2t,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifldl2e,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifldlpi,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifldlg2,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifldln2,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifldz,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ If2xm1,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifyl2x,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifptan,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifpatan,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifpxtract,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifprem1,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifdecstp,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifncstp,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifprem,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifyl2xp1,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifsqrt,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifsincos,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifrndint,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifscale,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifsin,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifcos,	NOARG,	NOARG,	NOARG,	Pnone }
};

/* DA Opcode Map */
x86_TableEntry insntbl_x87_opDA[0x8*0x8] = {
	{ Ifcmovb,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifcmovb,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifcmovb,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifcmovb,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifcmovb,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifcmovb,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifcmovb,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifcmovb,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifcmove,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifcmove,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifcmove,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifcmove,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifcmove,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifcmove,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifcmove,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifcmove,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifcmovbe,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifcmovbe,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifcmovbe,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifcmovbe,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifcmovbe,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifcmovbe,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifcmovbe,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifcmovbe,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifcmovu,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifcmovu,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifcmovu,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifcmovu,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifcmovu,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifcmovu,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifcmovu,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifcmovu,	ST0,	ST7,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifucompp,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone }
};

/* DB Opcode Map */
x86_TableEntry insntbl_x87_opDB[0x8*0x8] = {
	{ Ifcmovnb,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifcmovnb,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifcmovnb,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifcmovnb,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifcmovnb,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifcmovnb,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifcmovnb,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifcmovnb,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifcmovne,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifcmovne,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifcmovne,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifcmovne,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifcmovne,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifcmovne,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifcmovne,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifcmovne,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifcmovnbe,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifcmovnbe,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifcmovnbe,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifcmovnbe,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifcmovnbe,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifcmovnbe,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifcmovnbe,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifcmovnbe,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifcmovnu,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifcmovnu,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifcmovnu,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifcmovnu,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifcmovnu,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifcmovnu,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifcmovnu,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifcmovnu,	ST0,	ST7,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifclex,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifinit,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifucomi,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifucomi,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifucomi,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifucomi,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifucomi,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifucomi,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifucomi,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifucomi,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifcomi,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifcomi,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifcomi,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifcomi,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifcomi,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifcomi,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifcomi,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifcomi,	ST0,	ST7,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone }
};

/* DC Opcode Map */
x86_TableEntry insntbl_x87_opDC[0x8*0x8] = {
	{ Ifadd,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifadd,	ST1,	ST0,	NOARG,	Pnone },
	{ Ifadd,	ST2,	ST0,	NOARG,	Pnone },
	{ Ifadd,	ST3,	ST0,	NOARG,	Pnone },
	{ Ifadd,	ST4,	ST0,	NOARG,	Pnone },
	{ Ifadd,	ST5,	ST0,	NOARG,	Pnone },
	{ Ifadd,	ST6,	ST0,	NOARG,	Pnone },
	{ Ifadd,	ST7,	ST0,	NOARG,	Pnone },
	{ Ifmul,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifmul,	ST1,	ST0,	NOARG,	Pnone },
	{ Ifmul,	ST2,	ST0,	NOARG,	Pnone },
	{ Ifmul,	ST3,	ST0,	NOARG,	Pnone },
	{ Ifmul,	ST4,	ST0,	NOARG,	Pnone },
	{ Ifmul,	ST5,	ST0,	NOARG,	Pnone },
	{ Ifmul,	ST6,	ST0,	NOARG,	Pnone },
	{ Ifmul,	ST7,	ST0,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifsubr,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifsubr,	ST1,	ST0,	NOARG,	Pnone },
	{ Ifsubr,	ST2,	ST0,	NOARG,	Pnone },
	{ Ifsubr,	ST3,	ST0,	NOARG,	Pnone },
	{ Ifsubr,	ST4,	ST0,	NOARG,	Pnone },
	{ Ifsubr,	ST5,	ST0,	NOARG,	Pnone },
	{ Ifsubr,	ST6,	ST0,	NOARG,	Pnone },
	{ Ifsubr,	ST7,	ST0,	NOARG,	Pnone },
	{ Ifsub,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifsub,	ST1,	ST0,	NOARG,	Pnone },
	{ Ifsub,	ST2,	ST0,	NOARG,	Pnone },
	{ Ifsub,	ST3,	ST0,	NOARG,	Pnone },
	{ Ifsub,	ST4,	ST0,	NOARG,	Pnone },
	{ Ifsub,	ST5,	ST0,	NOARG,	Pnone },
	{ Ifsub,	ST6,	ST0,	NOARG,	Pnone },
	{ Ifsub,	ST7,	ST0,	NOARG,	Pnone },
	{ Ifdivr,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifdivr,	ST1,	ST0,	NOARG,	Pnone },
	{ Ifdivr,	ST2,	ST0,	NOARG,	Pnone },
	{ Ifdivr,	ST3,	ST0,	NOARG,	Pnone },
	{ Ifdivr,	ST4,	ST0,	NOARG,	Pnone },
	{ Ifdivr,	ST5,	ST0,	NOARG,	Pnone },
	{ Ifdivr,	ST6,	ST0,	NOARG,	Pnone },
	{ Ifdivr,	ST7,	ST0,	NOARG,	Pnone },
	{ Ifdiv,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifdiv,	ST1,	ST0,	NOARG,	Pnone },
	{ Ifdiv,	ST2,	ST0,	NOARG,	Pnone },
	{ Ifdiv,	ST3,	ST0,	NOARG,	Pnone },
	{ Ifdiv,	ST4,	ST0,	NOARG,	Pnone },
	{ Ifdiv,	ST5,	ST0,	NOARG,	Pnone },
	{ Ifdiv,	ST6,	ST0,	NOARG,	Pnone },
	{ Ifdiv,	ST7,	ST0,	NOARG,	Pnone }
};

/* DD Opcode Map */
x86_TableEntry insntbl_x87_opDD[0x8*0x8] = {
	{ Iffree,	ST0,	NOARG,	NOARG,	Pnone },
	{ Iffree,	ST1,	NOARG,	NOARG,	Pnone },
	{ Iffree,	ST2,	NOARG,	NOARG,	Pnone },
	{ Iffree,	ST3,	NOARG,	NOARG,	Pnone },
	{ Iffree,	ST4,	NOARG,	NOARG,	Pnone },
	{ Iffree,	ST5,	NOARG,	NOARG,	Pnone },
	{ Iffree,	ST6,	NOARG,	NOARG,	Pnone },
	{ Iffree,	ST7,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifst,		ST0,	NOARG,	NOARG,	Pnone },
	{ Ifst,		ST1,	NOARG,	NOARG,	Pnone },
	{ Ifst,		ST2,	NOARG,	NOARG,	Pnone },
	{ Ifst,		ST3,	NOARG,	NOARG,	Pnone },
	{ Ifst,		ST4,	NOARG,	NOARG,	Pnone },
	{ Ifst,		ST5,	NOARG,	NOARG,	Pnone },
	{ Ifst,		ST6,	NOARG,	NOARG,	Pnone },
	{ Ifst,		ST7,	NOARG,	NOARG,	Pnone },
	{ Ifstp,	ST0,	NOARG,	NOARG,	Pnone },
	{ Ifstp,	ST1,	NOARG,	NOARG,	Pnone },
	{ Ifstp,	ST2,	NOARG,	NOARG,	Pnone },
	{ Ifstp,	ST3,	NOARG,	NOARG,	Pnone },
	{ Ifstp,	ST4,	NOARG,	NOARG,	Pnone },
	{ Ifstp,	ST5,	NOARG,	NOARG,	Pnone },
	{ Ifstp,	ST6,	NOARG,	NOARG,	Pnone },
	{ Ifstp,	ST7,	NOARG,	NOARG,	Pnone },
	{ Ifucom,	ST0,	NOARG,	NOARG,	Pnone },
	{ Ifucom,	ST1,	NOARG,	NOARG,	Pnone },
	{ Ifucom,	ST2,	NOARG,	NOARG,	Pnone },
	{ Ifucom,	ST3,	NOARG,	NOARG,	Pnone },
	{ Ifucom,	ST4,	NOARG,	NOARG,	Pnone },
	{ Ifucom,	ST5,	NOARG,	NOARG,	Pnone },
	{ Ifucom,	ST6,	NOARG,	NOARG,	Pnone },
	{ Ifucom,	ST7,	NOARG,	NOARG,	Pnone },
	{ Ifucomp,	ST0,	NOARG,	NOARG,	Pnone },
	{ Ifucomp,	ST1,	NOARG,	NOARG,	Pnone },
	{ Ifucomp,	ST2,	NOARG,	NOARG,	Pnone },
	{ Ifucomp,	ST3,	NOARG,	NOARG,	Pnone },
	{ Ifucomp,	ST4,	NOARG,	NOARG,	Pnone },
	{ Ifucomp,	ST5,	NOARG,	NOARG,	Pnone },
	{ Ifucomp,	ST6,	NOARG,	NOARG,	Pnone },
	{ Ifucomp,	ST7,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone }
};

/* DE Opcode Map */
x86_TableEntry insntbl_x87_opDE[0x8*0x8] = {
	{ Ifaddp,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifaddp,	ST1,	ST0,	NOARG,	Pnone },
	{ Ifaddp,	ST2,	ST0,	NOARG,	Pnone },
	{ Ifaddp,	ST3,	ST0,	NOARG,	Pnone },
	{ Ifaddp,	ST4,	ST0,	NOARG,	Pnone },
	{ Ifaddp,	ST5,	ST0,	NOARG,	Pnone },
	{ Ifaddp,	ST6,	ST0,	NOARG,	Pnone },
	{ Ifaddp,	ST7,	ST0,	NOARG,	Pnone },
	{ Ifmulp,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifmulp,	ST1,	ST0,	NOARG,	Pnone },
	{ Ifmulp,	ST2,	ST0,	NOARG,	Pnone },
	{ Ifmulp,	ST3,	ST0,	NOARG,	Pnone },
	{ Ifmulp,	ST4,	ST0,	NOARG,	Pnone },
	{ Ifmulp,	ST5,	ST0,	NOARG,	Pnone },
	{ Ifmulp,	ST6,	ST0,	NOARG,	Pnone },
	{ Ifmulp,	ST7,	ST0,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifcompp,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifsubrp,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifsubrp,	ST1,	ST0,	NOARG,	Pnone },
	{ Ifsubrp,	ST2,	ST0,	NOARG,	Pnone },
	{ Ifsubrp,	ST3,	ST0,	NOARG,	Pnone },
	{ Ifsubrp,	ST4,	ST0,	NOARG,	Pnone },
	{ Ifsubrp,	ST5,	ST0,	NOARG,	Pnone },
	{ Ifsubrp,	ST6,	ST0,	NOARG,	Pnone },
	{ Ifsubrp,	ST7,	ST0,	NOARG,	Pnone },
	{ Ifsubp,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifsubp,	ST1,	ST0,	NOARG,	Pnone },
	{ Ifsubp,	ST2,	ST0,	NOARG,	Pnone },
	{ Ifsubp,	ST3,	ST0,	NOARG,	Pnone },
	{ Ifsubp,	ST4,	ST0,	NOARG,	Pnone },
	{ Ifsubp,	ST5,	ST0,	NOARG,	Pnone },
	{ Ifsubp,	ST6,	ST0,	NOARG,	Pnone },
	{ Ifsubp,	ST7,	ST0,	NOARG,	Pnone },
	{ Ifdivrp,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifdivrp,	ST1,	ST0,	NOARG,	Pnone },
	{ Ifdivrp,	ST2,	ST0,	NOARG,	Pnone },
	{ Ifdivrp,	ST3,	ST0,	NOARG,	Pnone },
	{ Ifdivrp,	ST4,	ST0,	NOARG,	Pnone },
	{ Ifdivrp,	ST5,	ST0,	NOARG,	Pnone },
	{ Ifdivrp,	ST6,	ST0,	NOARG,	Pnone },
	{ Ifdivrp,	ST7,	ST0,	NOARG,	Pnone },
	{ Ifdivp,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifdivp,	ST1,	ST0,	NOARG,	Pnone },
	{ Ifdivp,	ST2,	ST0,	NOARG,	Pnone },
	{ Ifdivp,	ST3,	ST0,	NOARG,	Pnone },
	{ Ifdivp,	ST4,	ST0,	NOARG,	Pnone },
	{ Ifdivp,	ST5,	ST0,	NOARG,	Pnone },
	{ Ifdivp,	ST6,	ST0,	NOARG,	Pnone },
	{ Ifdivp,	ST7,	ST0,	NOARG,	Pnone }
};

/* DF Opcode Map */
x86_TableEntry insntbl_x87_opDF[0x8*0x8] = {
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifstsw,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Ifucomip,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifucomip,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifucomip,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifucomip,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifucomip,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifucomip,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifucomip,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifucomip,	ST0,	ST7,	NOARG,	Pnone },
	{ Ifcomip,	ST0,	ST0,	NOARG,	Pnone },
	{ Ifcomip,	ST0,	ST1,	NOARG,	Pnone },
	{ Ifcomip,	ST0,	ST2,	NOARG,	Pnone },
	{ Ifcomip,	ST0,	ST3,	NOARG,	Pnone },
	{ Ifcomip,	ST0,	ST4,	NOARG,	Pnone },
	{ Ifcomip,	ST0,	ST5,	NOARG,	Pnone },
	{ Ifcomip,	ST0,	ST6,	NOARG,	Pnone },
	{ Ifcomip,	ST0,	ST7,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone },
	{ Iinvalid,	NOARG,	NOARG,	NOARG,	Pnone }
};

/* AMD 3DNow! Instructions */
x86_TableEntry insntbl_3DNow =
	{ I3dnow,	P,	Q,	NOARG  };

/* 3D Now instructions with suffix */
unsigned X86Disas::Get3DNowInsnMnemonic(unsigned char suffix)
{
  switch(suffix) {
	case 0x0C: return Ipi2fw;
	case 0x0D: return Ipi2fd;
	case 0x1C: return Ipf2iw;
	case 0x1D: return Ipf2id;
	case 0x8A: return Ipfnacc;
	case 0x90: return Ipfcmpge;
	case 0x94: return Ipfmin;
	case 0x96: return Ipfrcp;
	case 0x97: return Ipfrsqrt;
	case 0x9A: return Ipfsub;
	case 0x9E: return Ipfadd;
	case 0xA0: return Ipfcmpgt;
	case 0xA4: return Ipfmax;
	case 0xA6: return Ipfrcpit1;
	case 0xA7: return Ipfrsqit1;
	case 0xAA: return Ipfsubr;
	case 0xAE: return Ipfacc;
	case 0xB0: return Ipfcmpeq;
	case 0xB4: return Ipfmul;
	case 0xB6: return Ipfrcpit2;
	case 0xB7: return Ipmulhrw;
	case 0xBB: return Ipswapd;
	case 0xBF: return Ipavgusb;
  }
  return NULL;
}

x86_TableEntry *insntbl_x87_reg[] = {
	insntbl_x87_opD8reg,
	insntbl_x87_opD9reg,
	insntbl_x87_opDAreg,
	insntbl_x87_opDBreg,
	insntbl_x87_opDCreg,
	insntbl_x87_opDDreg,
	insntbl_x87_opDEreg,
	insntbl_x87_opDFreg
};

x86_TableEntry *insntbl_x87[] = {
	insntbl_x87_opD8,
	insntbl_x87_opD9,
	insntbl_x87_opDA,
	insntbl_x87_opDB,
	insntbl_x87_opDC,
	insntbl_x87_opDD,
	insntbl_x87_opDE,
	insntbl_x87_opDF
};

#define MODRM_REG(b) (((b)>>3) & 7)
#define MODRM_NNN(b) (((b)>>3) & 7)
#define MODRM_MOD(b) (((b)>>6) & 3)
#define MODRM_RM(b)   ((b) &7)

/* udis86_GetInsnEntry(...)
   Searches the x86 opcode table for the instruction corresponding
   to the opcode given by `*src'. The `*te' is pointed to the table
   entry and the src pointer is returned.
 */
unsigned char *X86Disas::GetInsnEntry(unsigned char *src, x86_TableEntry **te) {
  *te = NULL;
  /* check for two byte opcodes (0x0F) */
  if (0x0F == *src) {
	/* move to next byte */
	src++;
	/* check instruct for prefix 0x66 */
	if (G_opr_prfx) {
		/* get table entry */
		*te = &insntbl_2byte_prefix66[*src];
		if ((*te)->mnm != Iinvalid)
			G_opr_prfx = 0;
		else	*te = NULL;
	}
	/* check for prefixe 0xF3 */
	if ((*te) == NULL && G_rep_prfx) {
		/* get table entry */
		*te = &insntbl_2byte_prefixF3[*src];
		if ((*te)->mnm != Iinvalid)
			G_rep_prfx = 0;
		else	*te = NULL;
	}
	/* check for prefixe 0xF2 */
	if ((*te) == NULL && G_repne_prfx) {
		/* get table entry */
		*te = &insntbl_2byte_prefixF2[*src];
		if ((*te)->mnm != Iinvalid)
			G_repne_prfx = 0;
		else	*te = NULL;
	}
	/* no prefix */
	if ((*te) == NULL)	/* get table entry */
		*te = &insntbl_2byte[*src];

	/* check if the opcode points to a group */
	if ((*te)->mnm == Igrp) {
		switch (*src) {
			/* group 6 */
			case 0x00:
				*te = &insntbl_group6_op0F00[MODRM_REG(*(src+1))];
				break;
			/* group 7 */
			case 0x01:
				*te = &insntbl_group7_op0F01[MODRM_REG(*(src+1))];
				break;
			/* group 8 */
			case 0xBA:
				*te = &insntbl_group8_op0FBA[MODRM_REG(*(src+1))];
				break;
			/* group 9 */
			case 0xC7:
				*te = &insntbl_group9_op0FC7[MODRM_REG(*(src+1))];
				break;
			/* group A */
			case 0xB9:
				*te = &insntbl_groupA_op0FB9[MODRM_REG(*(src+1))];
				break;
			/* group C */
			case 0x71:
				if (G_opr_prfx) {
					*te = &insntbl_groupC_op0F71_prefix66[MODRM_REG(*(src+1))];
					G_opr_prfx = 0;
				}
				else	*te = &insntbl_groupC_op0F71[MODRM_REG(*(src+1))];
				break;
			/* group D */
			case 0x72:
				if (G_opr_prfx) {
					*te = &insntbl_groupD_op0F72_prefix66[MODRM_REG(*(src+1))];
					G_opr_prfx = 0;
				}
				else	*te = &insntbl_groupD_op0F72[MODRM_REG(*(src+1))];
				break;
			/* group E */
			case 0x73:
				if (G_opr_prfx) {
					*te = &insntbl_groupE_op0F73_prefix66[MODRM_REG(*(src+1))];
					G_opr_prfx = 0;
				}
				else	*te = &insntbl_groupE_op0F73[MODRM_REG(*(src+1))];
				break;
			/* group F */
			case 0xAE:
				*te = &insntbl_groupF_op0FAE[MODRM_REG(*(src+1))];
				break;
		}
	}
	else
	/* check if the opcode points to a group */
	if ((*te)->mnm == I3dnow)
		*te = &insntbl_3DNow;
  }
  /* else its a 1byte opcode */
  else {
	*te = &insntbl_1byte[*src];
	/* check if the opcode points to a group */
	if ((*te)->mnm == Igrp) {
		switch(*src) {
			/* group 1 */
			case 0x80:
				*te = &insntbl_group1_op80[MODRM_REG(*(src+1))];
				break;
			case 0x81:
				*te = &insntbl_group1_op81[MODRM_REG(*(src+1))];
				break;
			case 0x82:
				*te = &insntbl_group1_op82[MODRM_REG(*(src+1))];
				break;
			case 0x83:
				*te = &insntbl_group1_op83[MODRM_REG(*(src+1))];
				break;
			case 0x8F:
				*te = &insntbl_group1A_op8F[MODRM_REG(*(src+1))];
				break;
			/* group 2 */
			case 0xC0:
				*te = &insntbl_group2_opC0[MODRM_REG(*(src+1))];
				break;
			case 0xC1:
				*te = &insntbl_group2_opC1[MODRM_REG(*(src+1))];
				break;
			/* group 11 */
			case 0xC6:
				*te = &insntbl_groupB_opC6[MODRM_REG(*(src+1))];
				break;
			case 0xC7:
				*te = &insntbl_groupB_opC7[MODRM_REG(*(src+1))];
				break;
			case 0xD0:
				*te = &insntbl_group2_opD0[MODRM_REG(*(src+1))];
				break;
			case 0xD1:
				*te = &insntbl_group2_opD1[MODRM_REG(*(src+1))];
				break;
			case 0xD2:
				*te = &insntbl_group2_opD2[MODRM_REG(*(src+1))];
				break;
			case 0xD3:
				*te = &insntbl_group2_opD3[MODRM_REG(*(src+1))];
				break;
			/* group 3 */
			case 0xF6:
				*te = &insntbl_group3_opF6[MODRM_REG(*(src+1))];
				break;
			case 0xF7:
				*te = &insntbl_group3_opF7[MODRM_REG(*(src+1))];
				break;
			/* group 4 */
			case 0xFE:
				*te = &insntbl_group4_opFE[MODRM_REG(*(src+1))];
				break;
			/* group 5 */
			case 0xFF:
				*te = &insntbl_group5_opFF[MODRM_REG(*(src+1))];
				break;
		}
	}
	else
	/* check if the opcode points to an x87 instruction */
	if ((*te)->mnm == Ix87) {
		/* When the ModRM byte value falls within the range of
		   0x00 - 0xBF, then the reg field selects the inst.
		 */
		if (*(src+1) <= 0xBF)
			*te = &insntbl_x87_reg[(*src)-0xD8][MODRM_REG(*(src+1))];
		else {
			*te = &insntbl_x87[(*src)-0xD8][(*(src+1)) - 0xC0];
			++src;
		}
	}
  }

  return src;
}
