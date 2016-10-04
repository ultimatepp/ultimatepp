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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/colinfo.h,v $
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

#ifndef COLINFO_H
#define COLINFO_H

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/record.h>
#include <XLS/extformat.h>

namespace xlslib_core
{

  typedef struct
  {
    unsigned16_t colfirst;
    unsigned16_t collast;
    unsigned16_t width;
    xf_t*      xformat;
    unsigned16_t flags;
  } colinfo_t;

  struct colinfosort
  {
    public:
bool operator()(colinfo_t* const &a, colinfo_t* const  &b) const
    {
      return (a->colfirst < b->colfirst);
    };
  };


  typedef std::set<xlslib_core::colinfo_t*, colinfosort XLSLIB_DFLT_ALLOCATOR> Colinfo_List_t;
  typedef Colinfo_List_t::iterator Colinfo_List_Itor_t;

  class CColInfo: public CRecord
    {
    public:
      CColInfo(colinfo_t* newci);
      ~CColInfo();
    };

}

#endif //COLINFO_H
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: colinfo.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:43  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

