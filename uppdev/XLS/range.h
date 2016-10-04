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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/range.h,v $
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

#ifndef RANGE_H
#define RANGE_H

#include <XLS/config.h>
#include <XLS/common.h>

#include <XLS/sheetrec.h>
#include <XLS/font.h>
#include <XLS/cell.h>
#include <XLS/extformat.h>

namespace xlslib_core
{
  class worksheet;
  class range
    :  public xf_i,
    public font_i,
    private range_t 

    {
    public:
      range(unsigned16_t row1, unsigned16_t col1,
            unsigned16_t row2, unsigned16_t col2,
            worksheet* pws,
            bool atomic = false);
      virtual ~range();
  
      void cellcolor(color_name_t color);  

    private:
      bool m_Atomic;
      worksheet* m_pWorkSheet;

    public: // xf_i interface declaration
      void font(font_t* fontidx);
      void format(format_number_t formatidx);
      void halign(halign_option_t ha_option);
      void valign(valign_option_t va_option);
      void indent(indent_option_t indent_option);
      void orientation(txtori_option_t ori_option);
      void fillfgcolor(color_name_t color);
      void fillbgcolor(color_name_t color);
      void fillstyle(fill_option_t fill);
      void locked(bool locked_opt);
      void hidden(bool hidden_opt);
      void wrap(bool wrap_opt);
      void borderstyle(border_side_t side, 
                       border_style_t style, 
                       color_name_t color);

    public: // font_i interface declaration
      void fontname(std::string fntname);
      void fontheight(unsigned16_t fntheight);
      void fontbold(boldness_option_t fntboldness);
      void fontunderline(underline_option_t fntunderline);
      void fontscript(script_option_t fntscript);
      void fontcolor(color_name_t fntcolor);
      void fontattr(unsigned16_t attr);
      void fontitalic(bool italic);
      void fontstrikeout(bool so);
      void fontoutline(bool ol);
      void fontshadow(bool sh);
    };
}

#endif //RANGE_H 

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: range.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:49  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
 