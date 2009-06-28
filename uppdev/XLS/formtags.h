/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * xlslib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * xlslib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with xlslib.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Copyright 2004 Yeico S. A. de C. V.
 * Copyright 2008 David Hoerl
 *  
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/formtags.h,v $
 * $Revision: 1.2 $
 * $Author: dhoerl $
 * $Date: 2008/10/25 18:39:54 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <XLS/config.h>
/*
  sed -e 's/^\([A-Za-z0-9]*\)$/#define FT_\1 \\/' biff_formula_tags_norm.txt  | sed -e 's/^ \([A-F0-9]*\)\h$/                                              ((unsigned char)0x\1)/' | sed  -e "s/^ \([oc].*\)//"  > formtags.h
*/
 
#define FT_ptgExp                               \
  ((unsigned char)0x01)
#define FT_ptgTbl                               \
  ((unsigned char)0x02)
#define FT_ptgAdd                               \
  ((unsigned char)0x03)
#define FT_ptgSub                               \
  ((unsigned char)0x04)
#define FT_ptgMul                               \
  ((unsigned char)0x05)
#define FT_ptgDiv                               \
  ((unsigned char)0x06)
#define FT_ptgPower                             \
  ((unsigned char)0x07)
#define FT_ptgConcat                            \
  ((unsigned char)0x08)
#define FT_ptgLT                                \
  ((unsigned char)0x09)
#define FT_ptgLE                                \
  ((unsigned char)0x0A)
#define FT_ptgEQ                                \
  ((unsigned char)0x0B)
#define FT_ptgGE                                \
  ((unsigned char)0x0C)
#define FT_ptgGT                                \
  ((unsigned char)0x0D)
#define FT_ptgNE                                \
  ((unsigned char)0x0E)
#define FT_ptgIsect                             \
  ((unsigned char)0x0F)
#define FT_ptgUnion                             \
  ((unsigned char)0x10)
#define FT_ptgRange                             \
  ((unsigned char)0x11)
#define FT_ptgUplus                             \
  ((unsigned char)0x12)
#define FT_ptgUminus                            \
  ((unsigned char)0x13)
#define FT_ptgPercent                           \
  ((unsigned char)0x14)
#define FT_ptgParen                             \
  ((unsigned char)0x15)
#define FT_ptgMissArg                           \
  ((unsigned char)0x16)
#define FT_ptgStr                               \
  ((unsigned char)0x17)
#define FT_ptgAttr                              \
  ((unsigned char)0x19)
#define FT_ptgSheet                             \
  ((unsigned char)0x1A)
#define FT_ptgEndSheet                          \
  ((unsigned char)0x1B)
#define FT_ptgErr                               \
  ((unsigned char)0x1C)
#define FT_ptgBool                              \
  ((unsigned char)0x1D)
#define FT_ptgInt                               \
  ((unsigned char)0x1E)
#define FT_ptgNum                               \
  ((unsigned char)0x1F)
#define FT_ptgArray                             \
  ((unsigned char)0x20)
#define FT_ptgFunc                              \
  ((unsigned char)0x21)
#define FT_ptgFuncVar                           \
  ((unsigned char)0x22)
#define FT_ptgName                              \
  ((unsigned char)0x23)
#define FT_ptgRef                               \
  ((unsigned char)0x24)
#define FT_ptgArea                              \
  ((unsigned char)0x25)
#define FT_ptgMemArea                           \
  ((unsigned char)0x26)
#define FT_ptgMemErr                            \
  ((unsigned char)0x27)
#define FT_ptgMemNoMem                          \
  ((unsigned char)0x28)
#define FT_ptgMemFunc                           \
  ((unsigned char)0x29)
#define FT_ptgRefErr                            \
  ((unsigned char)0x2A)
#define FT_ptgAreaErr                           \
  ((unsigned char)0x2B)
#define FT_ptgRefN                              \
  ((unsigned char)0x2C)
#define FT_ptgAreaN                             \
  ((unsigned char)0x2D)
#define FT_ptgMemAreaN                          \
  ((unsigned char)0x2E)
#define FT_ptgMemNoMemN                         \
  ((unsigned char)0x2F)
#define FT_ptgNameX                             \
  ((unsigned char)0x39)
#define FT_ptgRef3d                             \
  ((unsigned char)0x3A)
#define FT_ptgArea3d                            \
  ((unsigned char)0x3B)
#define FT_ptgRefErr3d                          \
  ((unsigned char)0x3C)
#define FT_ptgAreaErr3d                         \
  ((unsigned char)0x3D)
#define FT_ptgArrayV                            \
  ((unsigned char)0x40)
#define FT_ptgFuncV                             \
  ((unsigned char)0x41)
#define FT_ptgFuncVarV                          \
  ((unsigned char)0x42)
#define FT_ptgNameV                             \
  ((unsigned char)0x43)
#define FT_ptgRefV                              \
  ((unsigned char)0x44)
#define FT_ptgAreaV                             \
  ((unsigned char)0x45)
#define FT_ptgMemAreaV                          \
  ((unsigned char)0x46)
#define FT_ptgMemErrV                           \
  ((unsigned char)0x47)
#define FT_ptgMemNoMemV                         \
  ((unsigned char)0x48)
#define FT_ptgMemFuncV                          \
  ((unsigned char)0x49)
#define FT_ptgRefErrV                           \
  ((unsigned char)0x4A)
#define FT_ptgAreaErrV                          \
  ((unsigned char)0x4B)
#define FT_ptgRefNV                             \
  ((unsigned char)0x4C)
#define FT_ptgAreaNV                            \
  ((unsigned char)0x4D)
#define FT_ptgMemAreaNV                         \
  ((unsigned char)0x4E)
#define FT_ptgMemNoMemNV                        \
  ((unsigned char)0x4F)
#define FT_ptgFuncCEV                           \
  ((unsigned char)0x58)
#define FT_ptgNameXV                            \
  ((unsigned char)0x59)
#define FT_ptgRef3dV                            \
  ((unsigned char)0x5A)
#define FT_ptgArea3dV                           \
  ((unsigned char)0x5B)
#define FT_ptgRefErr3dV                         \
  ((unsigned char)0x5C)
#define FT_ptgAreaErr3dV                        \
  ((unsigned char)0x5D)
#define FT_ptgArrayA                            \
  ((unsigned char)0x60)
#define FT_ptgFuncA                             \
  ((unsigned char)0x61)
#define FT_ptgFuncVarA                          \
  ((unsigned char)0x62)
#define FT_ptgNameA                             \
  ((unsigned char)0x63)
#define FT_ptgRefA                              \
  ((unsigned char)0x64)
#define FT_ptgAreaA                             \
  ((unsigned char)0x65)
#define FT_ptgMemAreaA                          \
  ((unsigned char)0x66)
#define FT_ptgMemErrA                           \
  ((unsigned char)0x67)
#define FT_ptgMemNoMemA                         \
  ((unsigned char)0x68)
#define FT_ptgMemFuncA                          \
  ((unsigned char)0x69)
#define FT_ptgRefErrA                           \
  ((unsigned char)0x6A)
#define FT_ptgAreaErrA                          \
  ((unsigned char)0x6B)
#define FT_ptgRefNA                             \
  ((unsigned char)0x6C)
#define FT_ptgAreaNA                            \
  ((unsigned char)0x6D)
#define FT_ptgMemAreaNA                         \
  ((unsigned char)0x6E)
#define FT_ptgMemNoMemNA                        \
  ((unsigned char)0x6F)
#define FT_ptgFuncCEA                           \
  ((unsigned char)0x78)
#define FT_ptgNameXA                            \
  ((unsigned char)0x79)
#define FT_ptgRef3dA                            \
  ((unsigned char)0x7A)
#define FT_ptgArea3dA                           \
  ((unsigned char)0x7B)
#define FT_ptgRefErr3dA                         \
  ((unsigned char)0x7C)
#define FT_ptgAreaErr3dA                        \
  ((unsigned char)0x7D)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: formtags.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

