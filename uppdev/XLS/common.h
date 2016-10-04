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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/common.h,v $
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

#ifndef COMMON_H
#define COMMON_H

#include <list>
#include <vector>
#include <algorithm>
#include <string>

#include <XLS/systype.h>

#ifdef HAVE_ICONV
#include <iconv.h>
#endif

namespace xlslib_core
{

  // Some typedefs used only by xlslib core
  typedef struct
  {
    unsigned16_t first_row;
    unsigned16_t last_row;

    unsigned16_t first_col;
    unsigned16_t last_col;
                
  } range_t;


#if defined(_MSC_VER) && defined(WIN32)
  typedef std::list<range_t* XLSLIB_DFLT_ALLOCATOR> Range_List_t;
  typedef Range_List_t::iterator Range_List_Itor_t;

  class range;
  typedef std::list<range* XLSLIB_DFLT_ALLOCATOR> RangeObj_List_t;
  typedef RangeObj_List_t::iterator RangeObj_List_Itor_t;

#else  
  typedef std::list<xlslib_core::range_t* XLSLIB_DFLT_ALLOCATOR> Range_List_t;
  typedef Range_List_t::iterator Range_List_Itor_t;

  class range;
  typedef std::list<xlslib_core::range* XLSLIB_DFLT_ALLOCATOR> RangeObj_List_t;
  typedef RangeObj_List_t::iterator RangeObj_List_Itor_t;
#endif
}
#endif //COMMON_H
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: common.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:57  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

