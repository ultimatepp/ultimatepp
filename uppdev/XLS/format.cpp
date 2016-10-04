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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/format.cpp,v $
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


#include <XLS/format.h>

using namespace std;
using namespace xlslib_core;

/* 
**********************************
CFormat class implementation
**********************************
*/
CFormat::CFormat(string&  formatstr,      
                 unsigned16_t index)
{
   SetRecordType(RECTYPE_FORMAT);

   AddValue16(index);

#if VERSION_BIFF == VERSION_BIFF5
   AddValue8((unsigned8_t)formatstr.size());
   AddDataArray((unsigned8_t*)formatstr.c_str(), formatstr.size());
#else
   AddUnicodeStrlen((unsigned16_t)formatstr.size());
   AddUnicodeString((unsigned8_t*)formatstr.c_str(), formatstr.size());
#endif
    
   SetRecordLength(GetDataSize()-4);
}

CFormat::CFormat(format_t* formatdef)
{
   SetRecordType(RECTYPE_FORMAT);

   AddValue16(formatdef->GetIndex());
#if VERSION_BIFF == VERSION_BIFF5
   AddValue8((unsigned8_t)((formatdef->GetFormatStr()).size()));
   AddDataArray((unsigned8_t*)((formatdef->GetFormatStr()).c_str()), 
                (formatdef->GetFormatStr()).size());
#else
   AddUnicodeStrlen((unsigned16_t)((formatdef->GetFormatStr()).size()));
   AddUnicodeString((unsigned8_t*)((formatdef->GetFormatStr()).c_str()), 
                (formatdef->GetFormatStr()).size());
#endif
    
   SetRecordLength(GetDataSize()-4);
}


CFormat::~CFormat()
{
}

/* 
**********************************
**********************************
*/

/* 
**********************************
**********************************
*/
int CFormat::GetFormatStr(string& formatstr)
{
   int errcode = NO_ERRORS;

   signed16_t formatsize;
   GetValue16From(&formatsize, FORMAT_OFFSET_NAMELENGTH);

   formatstr =  "";
   for(int i=0; i<formatsize; i++)
      formatstr += operator[](FORMAT_OFFSET_NAME+i);

   return errcode;
}

/* 
**********************************
**********************************
*/
int CFormat::SetIndex(unsigned16_t index)
{
   return (SetValueAt((signed16_t)index, FORMAT_OFFSET_INDEX));
}

/* 
**********************************
**********************************
*/
unsigned16_t CFormat::GetIndex(void)
{
   unsigned16_t indexval;
   GetValue16From((signed16_t*)&indexval, FORMAT_OFFSET_INDEX);

   return(indexval);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: format.cpp,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:48  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

