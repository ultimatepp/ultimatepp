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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/blank.cpp,v $
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
#include <XLS/blank.h>

using namespace std;
using namespace xlslib_core;

CBlank::CBlank(unsigned16_t row,
               unsigned16_t col,
               xf_t* pxfval)
{

   SetRecordType(RECTYPE_BLANK);
   AddValue16(row);
   AddValue16(col);

   unsigned16_t xfindex;
   pxfval != NULL? 
      xfindex = pxfval->GetIndex()
      :xfindex = 0x000f;
   AddValue16(xfindex);   
   SetRecordLength(GetDataSize()-4);

}

CBlank::CBlank(blank_t& blankdef)
{
   SetRecordType(RECTYPE_BLANK);	// DFH - was RECTYPE_LABEL, typo ??? (just like the number class)
   AddValue16(blankdef.GetRow());
   AddValue16(blankdef.GetCol());
   AddValue16(blankdef.GetXFIndex());

   SetRecordLength(GetDataSize()-4);
}

CBlank::~CBlank()
{
}

/*
*********************************
blank_t class implementation
*********************************
*/
blank_t::blank_t(unsigned16_t rowval, 
                 unsigned16_t colval, 
                 xf_t* pxfval)
{
   row = rowval;
   col = colval;

   if(pxfval != NULL)
      pxfval->MarkUsed();
   pxf = pxfval;
}

CUnit* blank_t::GetData() {
   CUnit* datablank = 
      (CUnit*)( new CBlank(row,col,pxf));
   return datablank; // NOTE: this pointer HAS to be deleted elsewhere.
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: blank.cpp,v $
 * Revision 1.2  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:54  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

