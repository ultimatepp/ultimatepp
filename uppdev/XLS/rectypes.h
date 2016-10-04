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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/rectypes.h,v $
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

#ifndef RECTYPES_H
#define RECTYPES_H

/*
  sed -e 's/\(^\([A-Z0-9]*\)\:.*$\)/\/\* \1 \*\/\n#define RECTYPE_\2 \\/' biff_records_alpha.txt  | sed -e 's/^ \([A-F0-9]*\)\h$/                                              ((unsigned16_t)0x\1)/' >recordtypes.h
*/

#include <XLS/config.h>
#include <XLS/systype.h>

//#define MAX_RECORD_SIZE_BIFF57 (2000)
#define MAX_RECORD_SIZE_BIFF8  (8000)
#define MAX_RECORD_SIZE MAX_RECORD_SIZE_BIFF8

/* 1904: 1904 Date System */
#define RECTYPE_NULL                            \
  ((unsigned16_t)0x00)

/* 1904: 1904 Date System */
#define RECTYPE_R1904                           \
  ((unsigned16_t)0x22)
 
/* ADDIN: Workbook Is an Add-in Macro */
#define RECTYPE_ADDIN                           \
  ((unsigned16_t)0x87)
 
/* ADDMENU: Menu Addition */
#define RECTYPE_ADDMENU                         \
  ((unsigned16_t)0xC2)
 
/* ARRAY: Array-Entered Formula */
#define RECTYPE_ARRAY                           \
  ((unsigned16_t)0x221)
 
/* AUTOFILTER: AutoFilter Data */
#define RECTYPE_AUTOFILTER                      \
  ((unsigned16_t)0x9E)
 
/* AUTOFILTERINFO: Drop-Down Arrow Count */
#define RECTYPE_AUTOFILTERINFO                  \
  ((unsigned16_t)0x9D)
 
/* BACKUP: Save Backup Version of the File */
#define RECTYPE_BACKUP                          \
  ((unsigned16_t)0x40)
 
/* BLANK: Cell Value, Blank Cell */
#define RECTYPE_BLANK                           \
  ((unsigned16_t)0x201)
 
/* BOF: Beginning of File */
#define RECTYPE_BOF                             \
  ((unsigned16_t)0x809)
 
/* BOOKBOOL: Workbook Option Flag */
#define RECTYPE_BOOKBOOL                        \
  ((unsigned16_t)0xDA)
 
/* BOOLERR: Cell Value, Boolean or Error */
#define RECTYPE_BOOLERR                         \
  ((unsigned16_t)0x205)
 
/* BOTTOMMARGIN: Bottom Margin Measurement */
#define RECTYPE_BOTTOMMARGIN                    \
  ((unsigned16_t)0x29)
 
/* BOUNDSHEET: Sheet Information */
#define RECTYPE_BOUNDSHEET                      \
  ((unsigned16_t)0x85)
 
/* CALCCOUNT: Iteration Count */
#define RECTYPE_CALCCOUNT                       \
  ((unsigned16_t)0x0C)
 
/* CALCMODE: Calculation Mode */
#define RECTYPE_CALCMODE                        \
  ((unsigned16_t)0x0D)
 
/* CF
   : Conditional Formatting Conditions */
#define RECTYPE_CF                              \
  ((unsigned16_t)0x1B1)
 
/* CONDFMT: Conditional Formatting Range Information */
#define RECTYPE_CONDFMT                         \
  ((unsigned16_t)0x1B0)
 
/* CODENAME: VBE Object Name */
#define RECTYPE_CODENAME                        \
  ((unsigned16_t)0x42)
 
/* CODEPAGE: Default Code Page */
#define RECTYPE_CODEPAGE                        \
  ((unsigned16_t)0x42)
 
/* COLINFO: Column Formatting Information */
#define RECTYPE_COLINFO                         \
  ((unsigned16_t)0x7D)
 
/* CONTINUE: Continues Long Records */
#define RECTYPE_CONTINUE                        \
  ((unsigned16_t)0x3C)
 
/* COORDLIST: Polygon Object Vertex Coordinates */
#define RECTYPE_COORDLIST                       \
  ((unsigned16_t)0xA9)
 
/* COUNTRY: Default Country and WIN.INI Country */
#define RECTYPE_COUNTRY                         \
  ((unsigned16_t)0x8C)
 
/* CRN: Nonresident Operands */
#define RECTYPE_CRN                             \
  ((unsigned16_t)0x5A)
 
/* DBCELL: Stream Offsets */
#define RECTYPE_DBCELL                          \
  ((unsigned16_t)0xD7)
 
/* DCON: Data Consolidation Information */
#define RECTYPE_DCON                            \
  ((unsigned16_t)0x50)
 
/* DCONBIN: Data Consolidation Information */
#define RECTYPE_DCONBIN                         \
  ((unsigned16_t)0x1B5)
 
/* DCONNAME: Data Consolidation Named References */
#define RECTYPE_DCONNAME                        \
  ((unsigned16_t)0x52)
 
/* DCONREF: Data Consolidation References */
#define RECTYPE_DCONREF                         \
  ((unsigned16_t)0x51)
 
/* DEFAULTROWHEIGHT: Default Row Height */
#define RECTYPE_DEFAULTROWHEIGHT                \
  ((unsigned16_t)0x225)
 
/* DEFCOLWIDTH: Default Width for Columns */
#define RECTYPE_DEFCOLWIDTH                     \
  ((unsigned16_t)0x55)
 
/* DELMENU: Menu Deletion */
#define RECTYPE_DELMENU                         \
  ((unsigned16_t)0xC3)
 
/* DELTA: Iteration Increment */
#define RECTYPE_DELTA                           \
  ((unsigned16_t)0x10)
 
/* DIMENSIONS: Cell Table Size */
#define RECTYPE_DIMENSIONS                      \
  ((unsigned16_t)0x200)
 
/* DOCROUTE: Routing Slip Information */
#define RECTYPE_DOCROUTE                        \
  ((unsigned16_t)0xB8)
 
/* DSF: Double Stream File */
#define RECTYPE_DSF                             \
  ((unsigned16_t)0x161)
 
/* DV: Data Validation Criteria */
#define RECTYPE_DV                              \
  ((unsigned16_t)0x1BE)
 
/* DVAL: Data Validation Information */
#define RECTYPE_DVAL                            \
  ((unsigned16_t)0x1B2)
 
/* EDG: Edition Globals */
#define RECTYPE_EDG                             \
  ((unsigned16_t)0x88)
 
/* EOF: End of File */
#define RECTYPE_EOF                             \
  ((unsigned16_t)0x0A)
 
/* EXTERNCOUNT: Number of External References */
#define RECTYPE_EXTERNCOUNT                     \
  ((unsigned16_t)0x16)
 
/* EXTERNNAME: Externally Referenced Name */
#define RECTYPE_EXTERNNAME                      \
  ((unsigned16_t)0x223)
 
/* EXTERNSHEET: External Reference */
#define RECTYPE_EXTERNSHEET                     \
  ((unsigned16_t)0x17)
 
/* EXTSST: Extended Shared String Table */
#define RECTYPE_EXTSST                          \
  ((unsigned16_t)0xFF)
 
/* FILEPASS: File Is Password-Protected */
#define RECTYPE_FILEPASS                        \
  ((unsigned16_t)0x2F)
 
/* FILESHARING: File-Sharing Information */
#define RECTYPE_FILESHARING                     \
  ((unsigned16_t)0x5B)
 
/* FILESHARING2: File-Sharing Information for Shared Lists */
#define RECTYPE_FILESHARING2                    \
  ((unsigned16_t)0x1A5)
 
/* FILTERMODE: Sheet Contains Filtered List */
#define RECTYPE_FILTERMODE                      \
  ((unsigned16_t)0x9B)
 
/* FNGROUPCOUNT: Built-in Function Group Count */
#define RECTYPE_FNGROUPCOUNT                    \
  ((unsigned16_t)0x9C)
 
/* FNGROUPNAME: Function Group Name */
#define RECTYPE_FNGROUPNAME                     \
  ((unsigned16_t)0x9A)
 
/* FONT: Font Description */
// NOTE: Changed temporarilly to 31h (the manual says is 231h)
#define RECTYPE_FONT                            \
  ((unsigned16_t)0x31)
 
/* FOOTER: Print Footer on Each Page */
#define RECTYPE_FOOTER                          \
  ((unsigned16_t)0x15)
 
/* FORMAT: Number Format */
#define RECTYPE_FORMAT                          \
  ((unsigned16_t)0x41E)
 
/* FORMULA: Cell Formula */
#define RECTYPE_FORMULA                         \
  ((unsigned16_t)0x406)
 
/* GCW: Global Column-Width Flags */
#define RECTYPE_GCW                             \
  ((unsigned16_t)0xAB)
 
/* GRIDSET: State Change of Gridlines Option */
#define RECTYPE_GRIDSET                         \
  ((unsigned16_t)0x82)
 
/* GUTS: Size of Row and Column Gutters */
#define RECTYPE_GUTS                            \
  ((unsigned16_t)0x80)
 
/* HCENTER: Center Between Horizontal Margins */
#define RECTYPE_HCENTER                         \
  ((unsigned16_t)0x83)
 
/* HEADER: Print Header on Each Page */
#define RECTYPE_HEADER                          \
  ((unsigned16_t)0x14)
 
/* HIDEOBJ: Object Display Options */
#define RECTYPE_HIDEOBJ                         \
  ((unsigned16_t)0x8D)
 
/* HLINK: Hyperlink */
#define RECTYPE_HLINK                           \
  ((unsigned16_t)0x1B8)
 
/* HORIZONTALPAGEBREAKS: Explicit Row Page Breaks */
#define RECTYPE_HORIZONTALPAGEBREAKS            \
  ((unsigned16_t)0x1B)
 
/* IMDATA: Image Data */
#define RECTYPE_IMDATA                          \
  ((unsigned16_t)0x7F)
 
/* INDEX: Index Record */
#define RECTYPE_INDEX                           \
  ((unsigned16_t)0x20B)
 
/* INTERFACEEND: End of User Interface Records */
#define RECTYPE_INTERFACEEND                    \
  ((unsigned16_t)0xE2)
 
/* INTERFACEHDR: Beginning of User Interface Records */
#define RECTYPE_INTERFACEHDR                    \
  ((unsigned16_t)0xE1)
 
/* ITERATION: Iteration Mode */
#define RECTYPE_ITERATION                       \
  ((unsigned16_t)0x11)
 
/* LABEL: Cell Value, String Constant */
#define RECTYPE_LABEL                           \
  ((unsigned16_t)0x204)
 
/* LABELSST: Cell Value, String Constant/SST */
#define RECTYPE_LABELSST                        \
  ((unsigned16_t)0xFD)
 
/* LEFTMARGIN: Left Margin Measurement */
#define RECTYPE_LEFTMARGIN                      \
  ((unsigned16_t)0x26)
 
/* LHNGRAPH: Named Graph Information */
#define RECTYPE_LHNGRAPH                        \
  ((unsigned16_t)0x95)
 
/* LHRECORD: .WK? File Conversion Information */
#define RECTYPE_LHRECORD                        \
  ((unsigned16_t)0x94)
 
/* LPR: Sheet Was Printed Using LINE.PRINT( */
#define RECTYPE_LPR                             \
  ((unsigned16_t)0x98)
 
/* MMS: ADDMENU/DELMENU Record Group Count */
#define RECTYPE_MMS                             \
  ((unsigned16_t)0xC1)
 
/* MSODRAWING: Microsoft Office Drawing */
#define RECTYPE_MSODRAWING                      \
  ((unsigned16_t)0xEC)
 
/* MSODRAWINGGROUP: Microsoft Office Drawing Group */
#define RECTYPE_MSODRAWINGGROUP                 \
  ((unsigned16_t)0xEB)
 
/* MSODRAWINGSELECTION: Microsoft Office Drawing Selection */
#define RECTYPE_MSODRAWINGSELECTION             \
  ((unsigned16_t)0xED)
 
/* MULBLANK: Multiple Blank Cells */
#define RECTYPE_MULBLANK                        \
  ((unsigned16_t)0xBE)
 
/* MULRK: Multiple RK Cells */
#define RECTYPE_MULRK                           \
  ((unsigned16_t)0xBD)
 
/* NAME: Defined Name */
#define RECTYPE_NAME                            \
  ((unsigned16_t)0x218)
 
/* NOTE: Comment Associated with a Cell */
#define RECTYPE_NOTE                            \
  ((unsigned16_t)0x1C)
 
/* NUMBER: Cell Value, Floating-Point Number */
#define RECTYPE_NUMBER                          \
  ((unsigned16_t)0x203)
 
/* OBJ: Describes a Graphic Object */
#define RECTYPE_OBJ                             \
  ((unsigned16_t)0x5D)
 
/* OBJPROTECT: Objects Are Protected */
#define RECTYPE_OBJPROTECT                      \
  ((unsigned16_t)0x63)
 
/* OBPROJ: Visual Basic Project */
#define RECTYPE_OBPROJ                          \
  ((unsigned16_t)0xD3)
 
/* OLESIZE: Size of OLE Object */
#define RECTYPE_OLESIZE                         \
  ((unsigned16_t)0xDE)
 
/* PALETTE: Color Palette Definition */
#define RECTYPE_PALETTE                         \
  ((unsigned16_t)0x92)
 
/* PANE: Number of Panes and Their Position */
#define RECTYPE_PANE                            \
  ((unsigned16_t)0x41)
 
/* PARAMQRY: Query Parameters */
#define RECTYPE_PARAMQRY                        \
  ((unsigned16_t)0xDC)
 
/* PASSWORD: Protection Password */
#define RECTYPE_PASSWORD                        \
  ((unsigned16_t)0x13)
 
/* PLS: Environment-Specific Print Record */
#define RECTYPE_PLS                             \
  ((unsigned16_t)0x4D)
 
/* PRECISION: Precision */
#define RECTYPE_PRECISION                       \
  ((unsigned16_t)0x0E)
 
/* PRINTGRIDLINES: Print Gridlines Flag */
#define RECTYPE_PRINTGRIDLINES                  \
  ((unsigned16_t)0x2B)
 
/* PRINTHEADERS: Print Row/Column Labels */
#define RECTYPE_PRINTHEADERS                    \
  ((unsigned16_t)0x2A)
 
/* PROTECT: Protection Flag */
#define RECTYPE_PROTECT                         \
  ((unsigned16_t)0x12)
 
/* PROT4REV: Shared Workbook Protection Flag */
#define RECTYPE_PROT4REV                        \
  ((unsigned16_t)0x1AF)
 
/* QSI: External Data Range */
#define RECTYPE_QSI                             \
  ((unsigned16_t)0x1AD)
 
/* RECIPNAME: Recipient Name */
#define RECTYPE_RECIPNAME                       \
  ((unsigned16_t)0xB9)
 
/* REFMODE: Reference Mode */
#define RECTYPE_REFMODE                         \
  ((unsigned16_t)0x0F)
 
/* REFRESHALL: Refresh Flag */
#define RECTYPE_REFRESHALL                      \
  ((unsigned16_t)0x1B7)
 
/* RIGHTMARGIN: Right Margin Measurement */
#define RECTYPE_RIGHTMARGIN                     \
  ((unsigned16_t)0x27)
 
/* RK: Cell Value, RK Number */
#define RECTYPE_RK                              \
  ((unsigned16_t)0x7E)
 
/* ROW: Describes a Row */
#define RECTYPE_ROW                             \
  ((unsigned16_t)0x208)
 
/* RSTRING: Cell with Character Formatting */
#define RECTYPE_RSTRING                         \
  ((unsigned16_t)0xD6)
 
/* SAVERECALC: Recalculate Before Save */
#define RECTYPE_SAVERECALC                      \
  ((unsigned16_t)0x5F)
 
/* SCENARIO: Scenario Data */
#define RECTYPE_SCENARIO                        \
  ((unsigned16_t)0xAF)
 
/* SCENMAN: Scenario Output Data */
#define RECTYPE_SCENMAN                         \
  ((unsigned16_t)0xAE)
 
/* SCENPROTECT: Scenario Protection */
#define RECTYPE_SCENPROTECT                     \
  ((unsigned16_t)0xDD)
 
/* SCL: Window Zoom Magnification */
#define RECTYPE_SCL                             \
  ((unsigned16_t)0xA0)
 
/* SELECTION: Current Selection */
#define RECTYPE_SELECTION                       \
  ((unsigned16_t)0x1D)
 
/* SETUP: Page Setup */
#define RECTYPE_SETUP                           \
  ((unsigned16_t)0xA1)
 
/* SHRFMLA: Shared Formula */
#define RECTYPE_SHRFMLA                         \
  ((unsigned16_t)0xBC)
 
/* SORT: Sorting Options */
#define RECTYPE_SORT                            \
  ((unsigned16_t)0x90)
 
/* SOUND: Sound Note */
#define RECTYPE_SOUND                           \
  ((unsigned16_t)0x96)
 
/* SST: Shared String Table */
#define RECTYPE_SST                             \
  ((unsigned16_t)0xFC)
 
/* STANDARDWIDTH: Standard Column Width */
#define RECTYPE_STANDARDWIDTH                   \
  ((unsigned16_t)0x99)
 
/* STRING: String Value of a Formula */
#define RECTYPE_STRING                          \
  ((unsigned16_t)0x207)
 
/* STYLE: Style Information */
#define RECTYPE_STYLE                           \
  ((unsigned16_t)0x293)
 
/* SUB: Subscriber */
#define RECTYPE_SUB                             \
  ((unsigned16_t)0x91)
 
/* SUPBOOK: Supporting Workbook */
#define RECTYPE_SUPBOOK                         \
  ((unsigned16_t)0x1AE)
 
/* SXDB: PivotTable Cache Data */
#define RECTYPE_SXDB                            \
  ((unsigned16_t)0xC6)
 
/* SXDBEX: PivotTable Cache Data */
#define RECTYPE_SXDBEX                          \
  ((unsigned16_t)0x122)
 
/* SXDI: Data Item */
#define RECTYPE_SXDI                            \
  ((unsigned16_t)0xC5)
 
/* SXEX: PivotTable View Extended Information */
#define RECTYPE_SXEX                            \
  ((unsigned16_t)0xF1)
 
/* SXEXT: External Source Information */
#define RECTYPE_SXEXT                           \
  ((unsigned16_t)0xDC)
 
/* SXFDBTYPE: SQL Datatype Identifier */
#define RECTYPE_SXFDBTYPE                       \
  ((unsigned16_t)0x1BB)
 
/* SXFILT: PivotTable Rule Filter */
#define RECTYPE_SXFILT                          \
  ((unsigned16_t)0xF2)
 
/* SXFORMAT: PivotTable Format Record */
#define RECTYPE_SXFORMAT                        \
  ((unsigned16_t)0xFB)
 
/* SXFORMULA: PivotTable Formula Record */
#define RECTYPE_SXFORMULA                       \
  ((unsigned16_t)0x103)
 
/* SXFMLA: PivotTable Parsed Expression */
#define RECTYPE_SXFMLA                          \
  ((unsigned16_t)0xF9)
 
/* SXIDSTM: Stream ID */
#define RECTYPE_SXIDSTM                         \
  ((unsigned16_t)0xD5)
 
/* SXIVD: Row/Column Field IDs */
#define RECTYPE_SXIVD                           \
  ((unsigned16_t)0xB4)
 
/* SXLI: Line Item Array */
#define RECTYPE_SXLI                            \
  ((unsigned16_t)0xB5)
 
/* SXNAME: PivotTable Name  */
#define RECTYPE_SXNAME                          \
  ((unsigned16_t)0xF6)
 
/* SXPAIR: PivotTable Name Pair */
#define RECTYPE_SXPAIR                          \
  ((unsigned16_t)0xF8)
 
/* SXPI: Page Item */
#define RECTYPE_SXPI                            \
  ((unsigned16_t)0xB6)
 
/* SXRULE: PivotTable Rule Data */
#define RECTYPE_SXRULE                          \
  ((unsigned16_t)0xF0)
 
/* SXSTRING: String */
#define RECTYPE_SXSTRING                        \
  ((unsigned16_t)0xCD)
 
/* SXSELECT: PivotTable Selection Information */
#define RECTYPE_SXSELECT                        \
  ((unsigned16_t)0xF7)
 
/* SXTBL: Multiple Consolidation Source Info */
#define RECTYPE_SXTBL                           \
  ((unsigned16_t)0xD0)
 
/* SXTBPG: Page Item Indexes */
#define RECTYPE_SXTBPG                          \
  ((unsigned16_t)0xD2)
 
/* SXTBRGIITM: Page Item Name Count */
#define RECTYPE_SXTBRGIITM                      \
  ((unsigned16_t)0xD1)
 
/* SXVD: View Fields */
#define RECTYPE_SXVD                            \
  ((unsigned16_t)0xB1)
 
/* SXVDEX: Extended PivotTable View Fields */
#define RECTYPE_SXVDEX                          \
  ((unsigned16_t)0x100)
 
/* SXVI: View Item */
#define RECTYPE_SXVI                            \
  ((unsigned16_t)0xB2)
 
/* SXVIEW: View Definition */
#define RECTYPE_SXVIEW                          \
  ((unsigned16_t)0xB0)
 
/* SXVS: View Source */
#define RECTYPE_SXVS                            \
  ((unsigned16_t)0xE3)
 
/* TABID: Sheet Tab Index Array */
#define RECTYPE_TABID                           \
  ((unsigned16_t)0x13D)
 
/* TABIDCONF: Sheet Tab ID of Conflict History */
#define RECTYPE_TABIDCONF                       \
  ((unsigned16_t)0xEA)
 
/* TABLE: Data Table */
#define RECTYPE_TABLE                           \
  ((unsigned16_t)0x236)
 
/* TEMPLATE: Workbook Is a Template */
#define RECTYPE_TEMPLATE                        \
  ((unsigned16_t)0x60)
 
/* TOPMARGIN: Top Margin Measurement */
#define RECTYPE_TOPMARGIN                       \
  ((unsigned16_t)0x28)
 
/* TXO: Text Object */
#define RECTYPE_TXO                             \
  ((unsigned16_t)0x1B6)
 
/* UDDESC: Description String for Chart Autoformat */
#define RECTYPE_UDDESC                          \
  ((unsigned16_t)0xDF)
 
/* UNCALCED: Recalculation Status */
#define RECTYPE_UNCALCED                        \
  ((unsigned16_t)0x5E)
 
/* USERBVIEW: Workbook Custom View Settings */
#define RECTYPE_USERBVIEW                       \
  ((unsigned16_t)0x1A9)
 
/* USERSVIEWBEGIN: Custom View Settings */
#define RECTYPE_USERSVIEWBEGIN                  \
  ((unsigned16_t)0x1AA)
 
/* USERSVIEWEND: End of Custom View Records */
#define RECTYPE_USERSVIEWEND                    \
  ((unsigned16_t)0x1AB)
 
/* USESELFS: Natural Language Formulas Flag */
#define RECTYPE_USESELFS                        \
  ((unsigned16_t)0x160)
 
/* VCENTER: Center Between Vertical Margins */
#define RECTYPE_VCENTER                         \
  ((unsigned16_t)0x84)
 
/* VERTICALPAGEBREAKS: Explicit Column Page Breaks */
#define RECTYPE_VERTICALPAGEBREAKS              \
  ((unsigned16_t)0x1A)
 
/* WINDOW1: Window Information */
#define RECTYPE_WINDOW1                         \
  ((unsigned16_t)0x3D)
 
/* WINDOW2: Sheet Window Information */
#define RECTYPE_WINDOW2                         \
  ((unsigned16_t)0x23E)
 
/* WINDOWPROTECT: Windows Are Protected */
#define RECTYPE_WINDOWPROTECT                   \
  ((unsigned16_t)0x19)
 
/* WRITEACCESS: Write Access User Name */
#define RECTYPE_WRITEACCESS                     \
  ((unsigned16_t)0x5C)
 
/* WRITEPROT: Workbook Is Write-Protected */
#define RECTYPE_WRITEPROT                       \
  ((unsigned16_t)0x86)
 
/* WSBOOL: Additional Workspace Information */
#define RECTYPE_WSBOOL                          \
  ((unsigned16_t)0x81)
 
/* XCT: CRN Record Count */
#define RECTYPE_XCT                             \
  ((unsigned16_t)0x59)
 
/* XF: Extended Format */
#define RECTYPE_XF                              \
  ((unsigned16_t)0xE0)
 
/* XL5MODIFY: Flag for DSF */
#define RECTYPE_XL5MODIFY                       \
  ((unsigned16_t)0x162)

/* TOOLBARHDR: Not documented */
#define RECTYPE_TOOLBARHDR                      \
  ((unsigned16_t)0xbf)

/* TOOLBAREND: Not documented */
#define RECTYPE_TOOLBAREND                      \
  ((unsigned16_t)0xc0)

/* TOOLBAREND: Not documented */
#define RECTYPE_MERGEDCELLS                     \
  ((unsigned16_t)0xe5)

#endif //RECTYPES_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: rectypes.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:51  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

