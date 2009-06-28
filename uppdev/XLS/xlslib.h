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
 * Copyright 2008 David Hoerl
 *  
 * $Source: /cvsroot/xlslib/xlslib/src/common/xlslib.h,v $
 * $Revision: 1.1 $
 * $Author: dhoerl $
 * $Date: 2008/10/25 18:44:32 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *	all include file for users
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifdef __cplusplus

#include <XLS/workbook.h>

#define EXTERN_TYPE extern "C"

using namespace xlslib_core;

#else // "C"

#define EXTERN_TYPE extern


typedef uintptr_t			write_fptr_t;

typedef unsigned char		unsigned8_t;
typedef unsigned short int	unsigned16_t;
typedef unsigned uint32_t	unsigned32_t;

#if defined(_MSC_VER) && defined(WIN32)
typedef __int64 unsigned64_t;
#else
//typedef long long unsigned int unsigned64_t;
typedef uint64_t			unsigned64_t;
#endif
typedef char				signed8_t;
typedef short int			signed16_t;
typedef int32_t				signed32_t;

typedef enum
{
  FMT_GENERAL = 0,
  FMT_NUMBER1,
  FMT_NUMBER2,
  FMT_NUMBER3,
  FMT_NUMBER4,
  FMT_CURRENCY1,
  FMT_CURRENCY2,
  FMT_CURRENCY3,
  FMT_CURRENCY4,
  FMT_PERCENT1,
  FMT_PERCENT2,
  FMT_SCIENTIFIC1,
  FMT_FRACTION1,
  FMT_FRACTION2,
  FMT_DATE1,
  FMT_DATE2,
  FMT_DATE3,
  FMT_DATE4,
  FMT_HOUR1,
  FMT_HOUR2,
  FMT_HOUR3,
  FMT_HOUR4,
  FMT_HOURDATE,
  FMT_NUMBER5,
  FMT_NUMBER6,
  FMT_NUMBER7,
  FMT_NUMBER8,
  FMT_ACCOUNTING1,
  FMT_ACCOUNTING2,
  FMT_ACCOUNTING3,
  FMT_ACCOUNTING4,
  FMT_HOUR5,
  FMT_HOUR6,
  FMT_HOUR7,
  FMT_SCIENTIFIC2,
  FMT_TEXT          
} format_number_t;

typedef enum
{
  HALIGN_GENERAL = 0,
  HALIGN_LEFT,
  HALIGN_CENTER,
  HALIGN_RIGHT,
  HALIGN_FILL,
  HALIGN_JUSTIFY,
  HALIGN_CENTERACCROSS
} halign_option_t;

typedef enum
{
  VALIGN_TOP = 0,
  VALIGN_CENTER,
  VALIGN_BOTTOM,
  VALIGN_JUSTIFY
} valign_option_t;

typedef enum
{
  ORI_NONE = 0,      
  ORI_TOPBOTTOMTXT,
  ORI_90NOCLOCKTXT,
  ORI_90CLOCKTXT     
} txtori_option_t;

typedef enum
{
  COLOR_BLACK,
  COLOR_DARK_RED,
  COLOR_RED,
  COLOR_FUCSIA,
  COLOR_COMBINED01,
  COLOR_COMBINED02,
  COLOR_COMBINED03,
  COLOR_COMBINED04,
  COLOR_COMBINED05,
  COLOR_COMBINED06,
  COLOR_OLIVE,
  COLOR_DARK_YELLOW,
  COLOR_COMBINED07,
  COLOR_YELLOW,
  COLOR_LIGHT_YELLOW,
  COLOR_DARK_GREEN,
  COLOR_GREEN,
  COLOR_COMBINED08,
  COLOR_LIVING_GREEN,
  COLOR_LIGHT_GREEN,
  COLOR_COMBINED09,
  COLOR_BLUE_GREEN,
  COLOR_AQUAMARINA,
  COLOR_TURQOISE,
  COLOR_COMBINED10,
  COLOR_DARK_BLUE,
  COLOR_BLUE,
  COLOR_LIGHT_BLUE,
  COLOR_SKY_BLUE,
  COLOR_COMBINED11,
  COLOR_INDIGO,
  COLOR_BLUE_GRAY,
  COLOR_VIOLET,
  COLOR_PLUM,
  COLOR_LAVANDER,
  COLOR_COMBINED12,
  COLOR_GRAY50,
  COLOR_GRAY40,
  COLOR_GRAY25,
  COLOR_WHITE           
} color_name_t;

typedef enum
{
  FILL_NONE = 0,
  FILL_SOLID,
  FILL_ATEN75,
  FILL_ATEN50,
  FILL_ATEN25,
  FILL_ATEN12,
  FILL_ATEN06,
  FILL_HORIZ_LIN,
  FILL_VERTICAL_LIN,
  FILL_DIAG,
  FILL_INV_DIAG,
  FILL_INTER_DIAG,
  FILL_DIAG_THICK_INTER,
  FILL_HORIZ_LINES_THIN,
  FILL_VERTICAL_LINES_THIN,
  FILL_DIAG_THIN,
  FILL_INV_DIAG_THIN,
  FILL_HORIZ_INT_THIN,
  FILL_HORIZ_INTER_THICK   
} fill_option_t;

typedef enum
{
  BORDER_NONE = 0,
  BORDER_THIN,
  BORDER_MEDIUM,
  BORDER_DASHED,
  BORDER_DOTTED,
  BORDER_THICK,
  BORDER_DOUBLE,
  BORDER_HAIR    
} border_style_t;

typedef enum
{
  BORDER_BOTTOM = 0,
  BORDER_TOP,
  BORDER_LEFT,
  BORDER_RIGHT 
} border_side_t;

typedef enum
{
  BOLDNESS_BOLD = 0,
  BOLDNESS_HALF,
  BOLDNESS_NORMAL,
  BOLDNESS_DOUBLE
} boldness_option_t;

typedef enum
{
  SCRIPT_NONE = 0,
  SCRIPT_SUPER,
  SCRIPT_SUB

} script_option_t;

typedef enum
{

  UNDERLINE_NONE = 0,
  UNDERLINE_SINGLE,
  UNDERLINE_DOUBLE,
  UNDERLINE_SINGLEACC,
  UNDERLINE_DOUBLEACC

} underline_option_t;

typedef enum
{
	BOLDNESS_BOLD = 0,
	BOLDNESS_HALF,
	BOLDNESS_NORMAL,
	BOLDNESS_DOUBLE
} boldness_option_t;
typedef enum
{
	SCRIPT_NONE = 0,
	SCRIPT_SUPER,
	SCRIPT_SUB
} script_option_t;
typedef enum
{
	UNDERLINE_NONE = 0,
	UNDERLINE_SINGLE,
	UNDERLINE_DOUBLE,
	UNDERLINE_SINGLEACC,
	UNDERLINE_DOUBLEACC
} underline_option_t;

typedef struct _workbook workbook;
typedef struct _worksheet worksheet;
typedef struct _font_t font_t;
typedef struct _format_t format_t;
typedef struct _cell_t cell_t;
typedef struct _xf_t xf_t;
typedef struct _range range;

#ifndef bool
typedef int		bool;
#endif

#endif

// Workbook
EXTERN_TYPE workbook *newWorkbook(void);
#ifdef XLS_PHP
EXTERN_TYPE workbook *newWorkbook_w_fptr(write_fptr_t w_fptr);
#endif
EXTERN_TYPE void deleteWorkbook(workbook *w);
EXTERN_TYPE worksheet *callWorkbookSheet(workbook *w, char *sheetname);
#if VERSION_BIFF == VERSION_BIFF8
EXTERN_TYPE worksheet *callWorkbookSheetW(workbook *w, uchar_t *sheetname);
#endif
EXTERN_TYPE worksheet *callWorkbookGetSheet(workbook *w, unsigned16_t sheetnum);
EXTERN_TYPE font_t *callWorkbookFont(workbook *w, char *name);
EXTERN_TYPE xf_t *callWorkbookxFormat(workbook *w);
EXTERN_TYPE xf_t *callWorkbookxFormatFont(workbook *w, font_t *font);
EXTERN_TYPE int callWorkbookDump(workbook *w, char *filename);
#ifdef XLS_PHP
EXTERN_TYPE void callWorkbookDisposition(workbook *w, Stroke_Disposition_t disp);
EXTERN_TYPE void callWorkbookStroke(workbook *w, char *filename);
#endif
// Worksheet
EXTERN_TYPE cell_t *callWorksheetFindCell(worksheet *w, unsigned16_t row, unsigned16_t col);
EXTERN_TYPE void callWorksheetMerge(worksheet *w, unsigned16_t first_row, unsigned16_t first_col, unsigned16_t last_row, unsigned16_t last_col);
EXTERN_TYPE void callWorksheetColwidth(worksheet *w, unsigned16_t col, unsigned16_t width);
EXTERN_TYPE void callWorksheetRowheight(worksheet *w, unsigned16_t row, unsigned16_t height);
EXTERN_TYPE range *callWorksheetRangegroup(worksheet *w, unsigned16_t row1, unsigned16_t col1, unsigned16_t row2, unsigned16_t col2);
EXTERN_TYPE cell_t *callWorksheetLabel(worksheet *w, unsigned16_t row, unsigned16_t col, char *strlabel, xf_t *pxformat);
#if VERSION_BIFF == VERSION_BIFF8
EXTERN_TYPE cell_t *callWorksheetLabelW(worksheet *w, unsigned16_t row, unsigned16_t col, uchar_t *strlabel, xf_t *pxformat);
#endif
EXTERN_TYPE cell_t *callWorksheetBlank(worksheet *w, unsigned16_t row, unsigned16_t col, xf_t *pxformat);
EXTERN_TYPE cell_t *callWorksheetNumber(worksheet *w, unsigned16_t row, unsigned16_t col, double numval, format_number_t fmtval, xf_t *pxformat);
// cell: xfi
EXTERN_TYPE void callCellFont(cell_t *c, font_t *fontidx);
EXTERN_TYPE void callCellFormat(cell_t *c, format_number_t formatidx);
EXTERN_TYPE void callCellHalign(cell_t *c, halign_option_t ha_option);
EXTERN_TYPE void callCellValign(cell_t *c, valign_option_t va_option);
EXTERN_TYPE void callCellOrientation(cell_t *c, txtori_option_t ori_option);
EXTERN_TYPE void callCellFillfgcolor(cell_t *c, color_name_t color);
EXTERN_TYPE void callCellFillbgcolor(cell_t *c, color_name_t color);
EXTERN_TYPE void callCellFillstyle(cell_t *c, fill_option_t fill);
EXTERN_TYPE void callCellLocked(cell_t *c, bool locked_opt);
EXTERN_TYPE void callCellHidden(cell_t *c, bool hidden_opt);
EXTERN_TYPE void callCellWrap(cell_t *c, bool wrap_opt);
EXTERN_TYPE void callCellBorderstyle(cell_t *c, border_side_t side, border_style_t style, color_name_t color);
EXTERN_TYPE void callCellFontname(cell_t *c, char *fntname);
// cell: font
EXTERN_TYPE void callCellFontheight(cell_t *c, unsigned16_t fntheight);
EXTERN_TYPE void callCellFontbold(cell_t *c, boldness_option_t fntboldness);
EXTERN_TYPE void callCellFontunderline(cell_t *c, underline_option_t fntunderline);
EXTERN_TYPE void callCellFontscript(cell_t *c, script_option_t fntscript);
EXTERN_TYPE void callCellFontcolor(cell_t *c, color_name_t fntcolor);
EXTERN_TYPE void callCellFontattr(cell_t *c, unsigned16_t attr);
EXTERN_TYPE void callCellFontitalic(cell_t *c, bool italic);
EXTERN_TYPE void callCellFontstrikeout(cell_t *c, bool so);
EXTERN_TYPE void callCellFontoutline(cell_t *c, bool ol);
EXTERN_TYPE void callCellFontshadow(cell_t *c, bool sh);
EXTERN_TYPE unsigned16_t callCellGetRow(cell_t *c);
EXTERN_TYPE unsigned16_t callCellGetCol(cell_t *c);
EXTERN_TYPE unsigned16_t callCellGetXFIndex(cell_t *c);
EXTERN_TYPE void callCellSetXF(cell_t *c, xf_t *pxfval);
// range
EXTERN_TYPE void callRangeCellcolor(range *r, color_name_t color);
// xformat
EXTERN_TYPE void callXformatSetFont(xf_t *x, font_t* fontidx);
EXTERN_TYPE unsigned16_t callXformatGetFontIndex(xf_t *x);
EXTERN_TYPE font_t* callXformatGetFont(xf_t *x);
EXTERN_TYPE void callXformatSetFormat(xf_t *x, format_number_t formatidx);
EXTERN_TYPE unsigned16_t callXformatGetFormatIndex(xf_t *x);
EXTERN_TYPE format_number_t callXformatGetFormat(xf_t *x);
EXTERN_TYPE void callXformatSetHAlign(xf_t *x, halign_option_t ha_option);
EXTERN_TYPE unsigned8_t callXformatGetHAlign(xf_t *x);
EXTERN_TYPE void callXformatSetVAlign(xf_t *x, valign_option_t va_option);
EXTERN_TYPE unsigned8_t callXformatGetVAlign(xf_t *x);
EXTERN_TYPE void callXformatSetTxtOrientation(xf_t *x, txtori_option_t ori_option);
EXTERN_TYPE unsigned8_t callXformatGetTxtOrientation(xf_t *x);
EXTERN_TYPE void callXformatSetFillFGColor(xf_t *x, color_name_t color);
EXTERN_TYPE unsigned8_t callXformatGetFillFGColor(xf_t *x);
EXTERN_TYPE void callXformatSetFillBGColor(xf_t *x, color_name_t color);
EXTERN_TYPE unsigned8_t callXformatGetFillBGColor(xf_t *x);
EXTERN_TYPE void callXformatSetFillStyle(xf_t *x, fill_option_t fill);
EXTERN_TYPE unsigned8_t callXformatGetFillStyle(xf_t *x);
EXTERN_TYPE void callXformatSetLocked(xf_t *x, bool locked_opt);
EXTERN_TYPE bool callXformatIsLocked(xf_t *x);
EXTERN_TYPE void callXformatSetHidden(xf_t *x, bool hidden_opt);
EXTERN_TYPE bool callXformatIsHidden(xf_t *x);
EXTERN_TYPE void callXformatSetWrap(xf_t *x, bool wrap_opt);
EXTERN_TYPE bool callXformatIsWrap(xf_t *x);
EXTERN_TYPE void callXformatSetCellMode(xf_t *x, bool cellmode);
EXTERN_TYPE bool callXformatIsCell(xf_t *x);
EXTERN_TYPE void callXformatSetBorderStyle(xf_t *x, border_side_t side, border_style_t style, color_name_t color);
EXTERN_TYPE unsigned8_t callXformatGetBorderStyle(xf_t *x, border_side_t side);
EXTERN_TYPE unsigned8_t callXformatGetBorderColor(xf_t *x, border_side_t side);
EXTERN_TYPE unsigned32_t callXformatGetSignature(xf_t *x);
// Font
EXTERN_TYPE void callFontSetName(font_t *f, char *name);
EXTERN_TYPE char *callFontGetName(font_t *f, char *name);
EXTERN_TYPE void callFontSetHeight(font_t *f, unsigned16_t fntheight);
EXTERN_TYPE unsigned16_t callFontGetHeight(font_t *f);
EXTERN_TYPE void callFontSetBoldStyle(font_t *f, boldness_option_t fntboldness);
EXTERN_TYPE unsigned16_t callFontGetBoldStyle(font_t *f);
EXTERN_TYPE void callFontSetUnderlineStyle(font_t *f, underline_option_t fntunderline);
EXTERN_TYPE unsigned8_t callFontGetUnderlineStyle(font_t *f);
EXTERN_TYPE void callFontSetScriptStyle(font_t *f, script_option_t fntscript);
EXTERN_TYPE unsigned16_t callFontGetScriptStyle(font_t *f);
EXTERN_TYPE void callFontSetColor(font_t *f, color_name_t fntcolor);
EXTERN_TYPE unsigned16_t callFontGetColor(font_t *f);
EXTERN_TYPE void callFontSetItalic(font_t *f, bool italic);
EXTERN_TYPE void callFontSetStrikeout(font_t *f, bool so);
EXTERN_TYPE void callFontSetAttributes(font_t *f, unsigned16_t attr);
EXTERN_TYPE unsigned16_t callFontGetAttributes(font_t *f);
EXTERN_TYPE void callFontSetOutline(font_t *f, bool ol);
EXTERN_TYPE void callFontSetShadow(font_t *f, bool sh);
