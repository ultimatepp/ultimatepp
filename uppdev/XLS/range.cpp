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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/range.cpp,v $
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


#include <XLS/range.h>

using namespace std;
using namespace xlslib_core;


range::range(unsigned16_t row1,
             unsigned16_t col1,
             unsigned16_t row2,
             unsigned16_t col2,
             worksheet* pws,
             bool atomic)
   : m_Atomic(atomic), m_pWorkSheet(pws)
{
   first_row = row1;
   last_row = row2;
   first_col = col1;
   last_col = col2;
}


range::~range()
{
}

void range::cellcolor(color_name_t color)
{
   unsigned16_t r,c;
   for(r = first_row; r <= last_row; r++)
      for(c = first_col; c <= last_col; c++)
      {
         cell_t* cell = m_pWorkSheet->FindCell(r,c);
         cell->fillfgcolor(color);
         cell->fillstyle(FILL_SOLID);

      }  
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   xf_i interface implementation for range class
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
   
#define SET_RANGE_FUNCTION(function, value)              \
   {                                                     \
      uint16_t r,c;                                      \
      for(r = first_row; r <= last_row; r++)             \
         for(c = first_col; c <= last_col; c++)          \
         {                                               \
            cell_t* cell = m_pWorkSheet->FindCell(r,c);  \
            cell->function(value);                       \
         }                                               \
   }

void range::font(font_t* fontidx)
{
   SET_RANGE_FUNCTION(font, fontidx);
/*
  int r,c;
  for(r = first_row; r <= last_row; r++)
  for(c = first_col; c <= last_col; c++)
  {
  cell_t* cell = m_pWorkSheet->FindCell(r,c);
  cell->font(fontidx);
  }
*/
}

void range::borderstyle(border_side_t side,
                        border_style_t style,
                        color_name_t color)
{
   unsigned16_t r,c;
   for(r = first_row; r <= last_row; r++)
      for(c = first_col; c <= last_col; c++)
      {
         cell_t* cell = m_pWorkSheet->FindCell(r,c);
         cell->borderstyle(side, style, color);
      }

}

void range::format(format_number_t formatidx)
{
   SET_RANGE_FUNCTION(format,formatidx);
}

void range::halign(halign_option_t ha_option)
{
   SET_RANGE_FUNCTION(halign,ha_option);
}

void range::valign(valign_option_t va_option)
{
   SET_RANGE_FUNCTION(valign,va_option);
}
void range::indent(indent_option_t indent_option)
{
   SET_RANGE_FUNCTION(indent,indent_option);
}
void range::orientation(txtori_option_t ori_option)
{
   SET_RANGE_FUNCTION(orientation,ori_option);
}

void range::fillfgcolor(color_name_t color)
{
   SET_RANGE_FUNCTION(fillfgcolor,color);
}

void range::fillbgcolor(color_name_t color)
{
   SET_RANGE_FUNCTION(fillbgcolor,color);
}

void range::fillstyle(fill_option_t fill)
{
   SET_RANGE_FUNCTION(fillstyle,fill);
}

void range::locked(bool locked_opt)
{
   SET_RANGE_FUNCTION(locked,locked_opt);
}

void range::hidden(bool hidden_opt)
{
   SET_RANGE_FUNCTION(hidden,hidden_opt);
}

void range::wrap(bool wrap_opt)
{
   SET_RANGE_FUNCTION(wrap,wrap_opt);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   font_i interface implementation for range class
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void range::fontname(string fntname)
{
   SET_RANGE_FUNCTION(fontname,fntname);
}

void range::fontheight(unsigned16_t fntheight)
{
   SET_RANGE_FUNCTION(fontheight,fntheight);
}

void range::fontbold(boldness_option_t fntboldness)
{
/*
  int r,c;                                            
  for(r = first_row; r <= last_row; r++)              
  for(c = first_col; c <= last_col; c++)           
  {                                                
  cell_t* cell = m_pWorkSheet->FindCell(r,c);   
  cell->fontbold(fntboldness);                        
  }                                                
*/
   SET_RANGE_FUNCTION(fontbold,fntboldness);
}

void range::fontunderline(underline_option_t fntunderline)
{
   SET_RANGE_FUNCTION(fontunderline,fntunderline);
}

void range::fontscript(script_option_t fntscript)
{
   SET_RANGE_FUNCTION(fontscript,fntscript);
}

void range::fontcolor(color_name_t fntcolor)
{
   SET_RANGE_FUNCTION(fontcolor,fntcolor);
}

void range::fontattr(unsigned16_t attr)
{
   SET_RANGE_FUNCTION(fontattr,attr);
}

void range::fontitalic(bool italic)
{
   SET_RANGE_FUNCTION(fontitalic,italic);
}

void range::fontstrikeout(bool so)
{
   SET_RANGE_FUNCTION(fontstrikeout,so);
}

void range::fontoutline(bool ol)
{
   SET_RANGE_FUNCTION(fontoutline,ol);
}

void range::fontshadow(bool sh)
{
   SET_RANGE_FUNCTION(fontshadow,sh);
/*
	{                                                     
	int r,c;                                           
	for(r = first_row; r <= last_row; r++)             
	for(c = first_col; c <= last_col; c++)          
	{                                               
	cell_t* cell = m_pWorkSheet->FindCell(r,c);  
	cell->fontshadow(sh);                       
	}                                               
	}
*/
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: range.cpp,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:56  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

