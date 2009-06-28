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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/number.cpp,v $
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

#include <XLS/number.h>

using namespace std;
using namespace xlslib_core;

CNumber::CNumber(unsigned16_t row,
                 unsigned16_t col,
                 double num,
                 xf_t* pxfval)
{

   SetRecordType(RECTYPE_NUMBER);
   AddValue16(row);
   AddValue16(col);

   unsigned16_t xfindex;
   pxfval != NULL? 
      xfindex = pxfval->GetIndex()
      :xfindex = 0x000f;
   AddValue16(xfindex);   
   
   void* pnumieee = (void*) &num;
   unsigned64_t numieee = *((unsigned64_t*)(pnumieee));
   AddValue64(numieee);

   SetRecordLength(GetDataSize()-4);

}

CNumber::CNumber(number_t& numdef)
{
   SetRecordType(RECTYPE_NUMBER);	// DFH was RECTYPE_LABEL, must have been a type ????
   AddValue16(numdef.GetRow());
   AddValue16(numdef.GetCol());
   AddValue16(numdef.GetXFIndex());

   double num = numdef.GetNumber();
   void* pnumieee = (void*) &num;
   unsigned64_t numieee = *((unsigned64_t*)(pnumieee));

   AddValue64(numieee);

   SetRecordLength(GetDataSize()-4);
}

CNumber::~CNumber()
{
}

/*
*********************************
number_t class implementation
*********************************
*/
number_t::number_t(unsigned16_t rowval, 
                   unsigned16_t colval, 
                   double numval, 
                   xf_t* pxfval)
{
   row = rowval;
   col = colval;
   number = numval;

   if(pxfval != NULL)
      pxfval->MarkUsed();
   pxf = pxfval;

}

CUnit* number_t::GetData() {
   CUnit* datanum = 
      (CUnit*)( new CNumber(row,col,number, pxf));
   return datanum; // NOTE: this pointer HAS to be deleted elsewhere.
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: number.cpp,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:55  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

