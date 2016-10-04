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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/cell.h,v $
 * $Revision: 1.2 $
 * $Author: dhoerl $
 * $Date: 2008/10/25 18:39:53 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef CELL_H
#define CELL_H

#include <XLS/config.h>
#include <XLS/common.h>

#include <XLS/unit.h>
#include <XLS/extformat.h>
#include <XLS/globalrec.h>

namespace xlslib_core
{

  class cell_t
    : public xf_i , public font_i
    {
    public:
      cell_t();
      virtual ~cell_t();
      unsigned16_t GetXFIndex(void);
      unsigned16_t GetRow(void){return row;};
      unsigned16_t GetCol(void){return col;};

      void SetXF(xf_t* pxfval){pxf = pxfval;};
      xf_t* GetXF(void){return pxf;};

      virtual unsigned16_t GetSize() = 0;
      virtual CUnit* GetData() = 0;
      /*
        bool operator<(const cell_t& right) const;
        bool operator>(const cell_t& right) const;
        bool operator%(const cell_t& right) const;
      */
      bool operator==(const cell_t& right) const;
      bool operator!=(const cell_t& right) const;

      void SetGlobalRecs(CGlobalRecords* pglobalr) {m_pGlobalRecs = pglobalr;};
    private:
      CGlobalRecords *m_pGlobalRecs;

    protected:
      unsigned16_t row;
      unsigned16_t col;

      xf_t* pxf;
    public: // xf_i interface
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
    public: //font_i interface
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

  // #ifdef WIN32
  // A bug in the compiler doesn't allow to use the functor direcly
  // with the list::sort() function
  // template<>
  // struct std::greater<cell_t*>  : public binary_function<cell_t* ,cell_t*, bool>
  // {
  //   public:
  //   bool operator()(cell_t* &a, cell_t* &b)
  //   {
  //     return (a->GetRow() < b->GetRow());
  //   };
  // };
  //
  //
  //
  // struct rowsort: public greater<cell_t*>
  // {
  //   public:
  //   bool operator()(cell_t* &a, cell_t* &b) const
  //   {
  //     return (a->GetRow() < b->GetRow());
  //   };
  // };
  // #endif

  struct rowsort
  {
    public:
bool operator()(cell_t* &a, cell_t* &b) const
    {
      return (a->GetRow() < b->GetRow() ? true : false);
    };
  };

  class insertsort
    {
    public:
      bool operator()(cell_t* a,  cell_t* b) const
        {
          unsigned32_t aval = 100000*(a->GetRow()) + a->GetCol();
          unsigned32_t bval = 100000*(b->GetRow()) + b->GetCol();

          return (aval < bval ? true : false);
        };
    };


  typedef std::set<xlslib_core::cell_t*,insertsort XLSLIB_DFLT_ALLOCATOR> Cell_List_t;
  typedef Cell_List_t::iterator Cell_List_Itor_t;


}

#endif // CELL_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: cell.h,v $
 * Revision 1.2  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:47  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


