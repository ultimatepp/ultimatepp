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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/index.h,v $
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


#ifndef INDEX_H
#define INDEX_H

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/record.h>

namespace xlslib_core
{

  /* 
******************************
CIndex class declaration
******************************
*/
#define INDEX_DFLT_RESERVED ((unsigned32_t)0x00000000)

#define INDEX_OFFSET_B7FIRSTROW ((unsigned32_t)8)
#define INDEX_OFFSET_B7LASTROW  ((unsigned32_t)10)
#define INDEX_OFFSET_B8FIRSTROW ((unsigned32_t)8)
#define INDEX_OFFSET_B8LASTROW  ((unsigned32_t)12)

  class CIndex: public CRecord
    {
    private:

    public:

      CIndex(unsigned32_t firstrow, 
             unsigned32_t lastrow);
      ~CIndex();
      void AddDBCellOffset(unsigned32_t dbcoffset);
      void SetRows(unsigned32_t firstrow, unsigned32_t lastrow);
      unsigned32_t GetFirstRow(void);
      unsigned32_t GetLastRow(void);

    };

}
#endif //INDEX_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: index.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

