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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/record.cpp,v $
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


#include <XLS/record.h>

using namespace std;
using namespace xlslib_core;

/* 
******************************
CRecord class implementation
******************************
*/


CRecord::CRecord()
   :m_Num(RECTYPE_NULL)

{

   // Initialize (and create) the space for record type
   // and record length
   const unsigned8_t array[] = {0,0,0,0} ; 
   AddDataArray(array, 4);
  


}

CRecord::~CRecord()
{
}

/* 
******************************
******************************
*/
void CRecord::SetRecordType(unsigned16_t rtype)
{
   SetValueAt((signed16_t)rtype, 0);

}


/* 
******************************
******************************
*/

unsigned16_t CRecord::GetRecordType()
{
   unsigned16_t value;
   GetValue16From((signed16_t*)&value, 0);

   return value;
}

/* 
******************************
******************************
*/
void CRecord::SetRecordLength(unsigned16_t rlength)
{
   SetValueAt((signed16_t)rlength, 2);

}


/* 
******************************
******************************
*/

unsigned16_t CRecord::GetRecordLength()
{
   unsigned16_t value;
   GetValue16From((signed16_t*)&value, 2);

   return value;
}


/* 
******************************
******************************
*/

unsigned8_t* CRecord::GetRecordDataBuffer()
{
   return GetBuffer() + 4;

}


/* 
******************************
******************************
*/
unsigned32_t CRecord::GetRecordDataSize()
{
   return GetDataSize() - 4;
}

/* 
******************************
******************************
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: record.cpp,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

