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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/merged.cpp,v $
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


#include <XLS/merged.h>

using namespace std;
using namespace xlslib_core;

/*
******************************
CMergedCells class implementation
****************************** 
*/
CMergedCells::CMergedCells()
{
   SetRecordType(RECTYPE_MERGEDCELLS);

   // By default the record is empty
   AddValue16(0x00);

   SetRecordLength(GetDataSize()-4);
}

CMergedCells::~CMergedCells()
{
}

/*
******************************
****************************** 
*/

void CMergedCells::AddRange(range_t* rng)
{
   AddValue16(rng->first_row);
   AddValue16(rng->last_row);
   AddValue16(rng->first_col);
   AddValue16(rng->last_col);

   SetRecordLength(GetDataSize()-4);
}

/*
******************************
****************************** 
*/
void CMergedCells::SetNumRanges(unsigned16_t numranges)
{
   
   SetValueAt((signed16_t)numranges, 0x04);

}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: merged.cpp,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:51  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


