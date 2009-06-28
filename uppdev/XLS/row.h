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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/row.h,v $
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


#ifndef ROW_H
#define ROW_H

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/record.h>

/*
******************************
CRow class declaration
******************************
*/

namespace xlslib_core
{

#define ROW_DFLT_HEIGHT        ((unsigned16_t)0x0108)
#define ROW_DFLT_GRBIT         ((unsigned16_t)0x0080)
#define ROW_GRBIT_UNSYNC       ((unsigned16_t)0x0040)
#define ROW_DFLT_IXFE          ((unsigned16_t)0x000f)
#define ROW_OFFSET_FIRSTCOL    ((unsigned32_t)6)
#define ROW_OFFSET_LASTCOL     ((unsigned32_t)8)
#define ROW_MASK_STDHEIGHT     ((unsigned16_t)0x0108)
#define ROW_RECORD_SIZE        (20)

  class rowheight_t
    {
    public:
      rowheight_t() {};
      rowheight_t(unsigned16_t rownum, unsigned16_t rowheight) 
        : num(rownum), height(rowheight) {}
      ~rowheight_t() {};

      unsigned16_t GetRowNum() {return num;};
      void SetRowNum(unsigned16_t rownum) {num = rownum;};

      unsigned16_t GetRowHeight() {return height;};
      void SetRowHeight(unsigned16_t rowheight) {height = rowheight;};

      bool operator<(const rowheight_t& right) const{
        return (num < right.num);
      };

      bool operator>(const rowheight_t& right) const{
        return (num > right.num);
      };

      bool operator==(const rowheight_t& right) const{
        return (num == right.num);
      };
  
      bool operator!=(const rowheight_t& right) const{
        return (num != right.num);
      };


    private:
      unsigned16_t num;
      unsigned16_t height;
    };

  struct rowheightsort
  {
    public:
bool operator()(rowheight_t* const &a, rowheight_t* const &b) const
    {
      return (a->GetRowNum() < b->GetRowNum());
    };
  };

  typedef std::set<xlslib_core::rowheight_t*,rowheightsort XLSLIB_DFLT_ALLOCATOR> RowHeight_List_t;
  typedef RowHeight_List_t::iterator RowHeight_List_Itor_t;



  // NOTE: row_t has not a height field
  typedef struct
  {
    unsigned16_t rownum;
    unsigned16_t firstcol;
    unsigned16_t lastcol;
  }row_t;
  typedef std::list<xlslib_core::row_t* XLSLIB_DFLT_ALLOCATOR> Row_List_t;
  typedef Row_List_t::iterator Row_List_Itor_t;


  class CRow: public CRecord
    {
    private:

    public:
      CRow(unsigned16_t rownum,
           unsigned16_t firstcol,
           unsigned16_t lastcol,
           unsigned16_t rowheight  = ROW_DFLT_HEIGHT);
      CRow(row_t& rowdef);

      ~CRow();

      void SetFirstCol(unsigned16_t firstrow);
      void SetLastCol(unsigned16_t lastrow);
      unsigned16_t GetFirstCol(void);
      unsigned16_t GetLastCol(void);
  


    };


  /*
******************************
CDBCell class declaration
******************************
*/

#define DBC_DFLT_STARTBLOCK  ((unsigned32_t)(0x00000000))

  class CDBCell: public CRecord
    {
    private:

    public:
      CDBCell(unsigned32_t startblock = DBC_DFLT_STARTBLOCK);
      ~CDBCell();

      void AddRowOffset(unsigned16_t rowoffset );

    };


}

#endif //ROW_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: row.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

