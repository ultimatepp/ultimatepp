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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/cbridge.cpp,v $
 * $Revision: 1.1 $
 * $Author: dhoerl $
 * $Date: 2008/10/25 18:44:32 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <sys/types.h>
#include <string>

#include <XLS/xlslib.h>

using namespace std;
//using namespace xlslib_core;

#if 0
EXTERN_TYPE worksheet *newWorksheet(CGlobalRecords *pglobalrec);
EXTERN_TYPE void deleteWorksheet(worksheet *w);
	worksheet *newWorksheet(CGlobalRecords *pglobalrec)			{ return new worksheet(pglobalrec); }
	void deleteWorksheet(worksheet *w)							{ delete w; }
#endif

extern "C" {
	// Workbook
	workbook *newWorkbook(void)									{ return new workbook; }
	//workbook *newWorkbook_w_fptr(write_fptr_t w_fptr)			{ return new workbook(w_fptr); }
	void deleteWorkbook(workbook *w) { delete w; }
	worksheet *callWorkbookSheet(workbook *w, char *sheetname)	{
																	std::string str = sheetname;
																	return w->sheet(str);
																}
#if VERSION_BIFF == VERSION_BIFF8
	worksheet *callWorkbookSheetW(workbook *w, uchar_t *sheetname)	{
																	std::ustring str = sheetname;
																	return w->sheet(str);
																}
#endif
	worksheet *callWorkbookGetSheet(workbook *w, unsigned16_t sheetnum)	{ return w->GetSheet(sheetnum); }

	font_t *callWorkbookFont(workbook *w, char *name)			{
																	std::string str = name;
																	return w->font(str);
																}
	xf_t *callWorkbookxFormat(workbook *w)						{ return w->xformat(); }
	xf_t *callWorkbookxFormatFont(workbook *w, font_t *font)	{ return w->xformat(font); }
	int callWorkbookDump(workbook *w, char *filename)			{
																	std::string str = filename;
																	return w->Dump(str);
																}
#ifdef XLS_PHP
	void callWorkbookDisposition(workbook *w, Stroke_Disposition_t disp) { return w->Disposition(disp); }
	void callWorkbookStroke(workbook *w, char *filename)		{
																	std::string str = filename;
																	return w->Stroke(str);
																}
#endif
	// Worksheet
	cell_t *callWorksheetFindCell(worksheet *w, unsigned16_t row, unsigned16_t col) { return w->FindCell(row, col); }
	// Cell operations
	void callWorksheetMerge(worksheet *w, unsigned16_t first_row, unsigned16_t first_col, unsigned16_t last_row, unsigned16_t last_col)		
																{ return w->merge(first_row, first_col, last_row, last_col); }
	void callWorksheetColwidth(worksheet *w, unsigned16_t col, unsigned16_t width)
																{ return w->colwidth(col, width); }
	void callWorksheetRowheight(worksheet *w, unsigned16_t row, unsigned16_t height)
																{ return w->rowheight(row, height); } 
																	// Ranges
	range *callWorksheetRangegroup(worksheet *w, unsigned16_t row1, unsigned16_t col1, unsigned16_t row2, unsigned16_t col2)
																{ return w->rangegroup(row1, col1, row2, col2); }
	// Cells
	cell_t *callWorksheetLabel(worksheet *w, unsigned16_t row, unsigned16_t col, char *strlabel, xf_t *pxformat)
																{ 
																	std:string str = strlabel;
																	return w->label(row, col, strlabel, pxformat);
																}
#if VERSION_BIFF == VERSION_BIFF8
	cell_t *callWorksheetLabelW(worksheet *w, unsigned16_t row, unsigned16_t col, uchar_t *strlabel, xf_t *pxformat)
																{ 
																	std:ustring str = strlabel;
																	return w->label(row, col, strlabel, pxformat);
																}
#endif
	cell_t *callWorksheetBlank(worksheet *w, unsigned16_t row, unsigned16_t col, xf_t *pxformat)
																{ return w->blank(row, col, pxformat); }

	cell_t *callWorksheetNumber(worksheet *w, unsigned16_t row, unsigned16_t col, double numval, format_number_t fmtval, xf_t *pxformat)
																{ return w->number(row, col, numval, fmtval, pxformat); }
																
	// Cells
	// xf_i interface
	void callCellFont(cell_t *c, font_t *fontidx)				{ return c->font(fontidx); }
	void callCellFormat(cell_t *c, format_number_t formatidx)	{ return c->format(formatidx); }
	void callCellHalign(cell_t *c, halign_option_t ha_option)	{ return c->halign(ha_option); }
	void callCellValign(cell_t *c, valign_option_t va_option)	{ return c->valign(va_option); }	
	void callCellOrientation(cell_t *c, txtori_option_t ori_option) { return c->orientation(ori_option); }
	void callCellFillfgcolor(cell_t *c, color_name_t color)		{ return c->fillfgcolor(color); }
	void callCellFillbgcolor(cell_t *c, color_name_t color)		{ return c->fillbgcolor(color); }
	void callCellFillstyle(cell_t *c, fill_option_t fill)		{ return c->fillstyle(fill); }
	void callCellLocked(cell_t *c, bool locked_opt)				{ return c->locked(locked_opt); }
	void callCellHidden(cell_t *c, bool hidden_opt)				{ return c->hidden(hidden_opt); }
	void callCellWrap(cell_t *c, bool wrap_opt)					{ return c->wrap(wrap_opt); }
	void callCellBorderstyle(cell_t *c, border_side_t side, border_style_t style, color_name_t color)
																{ return c->borderstyle(side, style, color); }
	//font_i interface
	void callCellFontname(cell_t *c, char *fntname)				{
																	std::string str = fntname;
																	return c->fontname(str);
																}
	void callCellFontheight(cell_t *c, unsigned16_t fntheight)	{ return c->fontheight(fntheight); }
	void callCellFontbold(cell_t *c, boldness_option_t fntboldness) { return c->fontbold(fntboldness); }
	void callCellFontunderline(cell_t *c, underline_option_t fntunderline) { return c->fontunderline(fntunderline); }
	void callCellFontscript(cell_t *c, script_option_t fntscript) { return c->fontscript(fntscript); }
	void callCellFontcolor(cell_t *c, color_name_t fntcolor)	{ return c->fontcolor(fntcolor); }
	void callCellFontattr(cell_t *c, unsigned16_t attr)			{ return c->fontattr(attr); }
	void callCellFontitalic(cell_t *c, bool italic)				{ return c->fontitalic(italic); }
	void callCellFontstrikeout(cell_t *c, bool so)				{ return c->fontstrikeout(so); }
	void callCellFontoutline(cell_t *c, bool ol)				{ return c->fontoutline(ol); }
	void callCellFontshadow(cell_t *c, bool sh)					{ return c->fontshadow(sh); }

	unsigned16_t callCellGetRow(cell_t *c)						{ return c->GetRow(); }
	unsigned16_t callCellGetCol(cell_t *c)						{ return c->GetCol(); }
	unsigned16_t callCellGetXFIndex(cell_t *c)					{ return c->GetXFIndex(); }
	void callCellSetXF(cell_t *c, xf_t *pxfval)					{ return c->SetXF(pxfval); }
	
	// range
	void callRangeCellcolor(range *r, color_name_t color)		{ return r->cellcolor(color); }
	
	// xformat
	void callXformatSetFont(xf_t *x, font_t* fontidx)			{ return x->SetFont(fontidx); }
	unsigned16_t callXformatGetFontIndex(xf_t *x)				{ return x->GetFontIndex(); }
	font_t* callXformatGetFont(xf_t *x)							{ return x->GetFont(); }
	/* Format Index wrappers*/
	void callXformatSetFormat(xf_t *x, format_number_t formatidx) { return x->SetFormat(formatidx); }
	unsigned16_t callXformatGetFormatIndex(xf_t *x)				{ return x->GetFormatIndex(); }
	format_number_t callXformatGetFormat(xf_t *x)				{ return x->GetFormat(); }
	/* Horizontal Align option wrappers*/
	void callXformatSetHAlign(xf_t *x, halign_option_t ha_option)	{ return x->SetHAlign(ha_option); }
	unsigned8_t callXformatGetHAlign(xf_t *x)					{ return x->GetHAlign(); }
	/* Vertical Align option wrappers*/
	void callXformatSetVAlign(xf_t *x, valign_option_t va_option) { return x->SetVAlign(va_option); }
	unsigned8_t callXformatGetVAlign(xf_t *x)					{ return x->GetVAlign(); }
	/* Text orientation option wrappers*/
	void callXformatSetTxtOrientation(xf_t *x, txtori_option_t ori_option) { return x->SetTxtOrientation(ori_option); }
	unsigned8_t callXformatGetTxtOrientation(xf_t *x)			{ return x->GetTxtOrientation(); }
	/* Fill Foreground color option wrappers*/
	void callXformatSetFillFGColor(xf_t *x, color_name_t color)	{ return x->SetFillFGColor(color); }
	unsigned8_t callXformatGetFillFGColor(xf_t *x)				{ return x->GetFillFGColor(); }
	/* Fill Background color option wrappers*/
	void callXformatSetFillBGColor(xf_t *x, color_name_t color) { return x->SetFillBGColor(color); }
	unsigned8_t callXformatGetFillBGColor(xf_t *x)				{ return x->GetFillBGColor(); }
	/* Fill Style option wrappers*/
	void callXformatSetFillStyle(xf_t *x, fill_option_t fill)	{ return x->SetFillStyle(fill); }
	unsigned8_t callXformatGetFillStyle(xf_t *x)				{ return x->GetFillStyle(); }
	/* Locked option wrappers*/
	void callXformatSetLocked(xf_t *x, bool locked_opt)			{ return x->SetLocked(locked_opt); }
	bool callXformatIsLocked(xf_t *x)							{ return x->IsLocked(); }
	/* Hidden option wrappers*/
	void callXformatSetHidden(xf_t *x, bool hidden_opt)			{ return x->SetHidden(hidden_opt); }
	bool callXformatIsHidden(xf_t *x)							{ return x->IsHidden(); }
	/* Wrap option wrappers*/
	void callXformatSetWrap(xf_t *x, bool wrap_opt)				{ return x->SetWrap(wrap_opt); }
	bool callXformatIsWrap(xf_t *x)								{ return x->IsWrap(); }
	/* Cell option wrappers*/
	void callXformatSetCellMode(xf_t *x, bool cellmode)			{ return x->SetCellMode(cellmode); }
	bool callXformatIsCell(xf_t *x)								{ return x->IsCell(); }
	/* Cell option wrappers*/
	void callXformatSetBorderStyle(xf_t *x, border_side_t side, border_style_t style, color_name_t color)
																{ return x->SetBorderStyle(side, style, color); }
	unsigned8_t callXformatGetBorderStyle(xf_t *x, border_side_t side) { return x->GetBorderStyle(side); }
	unsigned8_t callXformatGetBorderColor(xf_t *x, border_side_t side) { return x->GetBorderColor(side); }
	unsigned32_t callXformatGetSignature(xf_t *x)				{ return x->GetSignature(); }

	// Font
	void callFontSetName(font_t *f, char *fntname)				{
																	std::string str = fntname;
																	f->SetName(str);
																	return;
																}
	char *callFontGetName(font_t *f, char *fntname)				{ return strcpy(fntname, (f->GetName()).c_str() ); }
	/* FONT height wrappers*/
	void callFontSetHeight(font_t *f, unsigned16_t fntheight)	{ return f->SetHeight(fntheight); }
	unsigned16_t callFontGetHeight(font_t *f)					{ return f->GetHeight(); }
	/* FONT boldstyle wrappers*/
	void callFontSetBoldStyle(font_t *f, boldness_option_t fntboldness) { return f->SetBoldStyle(fntboldness); }
	unsigned16_t callFontGetBoldStyle(font_t *f)				{ return f->GetBoldStyle(); }
	/* FONT underline wrappers*/
	void callFontSetUnderlineStyle(font_t *f, underline_option_t fntunderline) { return f->SetUnderlineStyle(fntunderline); }
	unsigned8_t callFontGetUnderlineStyle(font_t *f)			{ return f->GetUnderlineStyle(); }
	/* FONT script wrappers*/
	void callFontSetScriptStyle(font_t *f, script_option_t fntscript) { return f->SetScriptStyle(fntscript); }
	unsigned16_t callFontGetScriptStyle(font_t *f)				{ return f->GetScriptStyle(); }
	/* FONT script wrappers*/
	void callFontSetColor(font_t *f, color_name_t fntcolor)		{ return f->SetColor(fntcolor); }
	unsigned16_t callFontGetColor(font_t *f)					{ return f->GetColor(); }
	void callFontSetItalic(font_t *f, bool italic)				{ return f->SetItalic(italic); }
	void callFontSetStrikeout(font_t *f, bool so)				{ return f->SetStrikeout(so); }
	/* FONT  attributes wrappers */
	void callFontSetAttributes(font_t *f, unsigned16_t attr)	{ return f->SetAttributes(attr); }
	unsigned16_t callFontGetAttributes(font_t *f)				{ return f->GetAttributes(); }
	// Macintosh only
	void callFontSetOutline(font_t *f, bool ol)					{ return f->SetOutline(ol); }
	void callFontSetShadow(font_t *f, bool sh)					{ return f->SetShadow(sh); }
}
