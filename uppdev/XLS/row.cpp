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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/row.cpp,v $
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


#include <XLS/row.h>

using namespace std;
using namespace xlslib_core;

/*
******************************
CRow class implementation
******************************
*/



CRow::CRow(unsigned16_t rownum,  
           unsigned16_t firstcol,
           unsigned16_t lastcol, 
           unsigned16_t rowheight)
{
   SetRecordType(RECTYPE_ROW);
   AddValue16(rownum);
   AddValue16(firstcol);
   AddValue16((lastcol+1));
   AddValue16(rowheight);

   // A field used by MS for "optimizing" (?) the loading of a file.
   // Doc says it shall be set to 0 if I'm creating a BIFF...
   AddValue16(0);
   // A reserved value:
   AddValue16(0);

   // TODO: The following flag-word can be used for outline cells. Later.
   // As a defaule the GhostDirty flag is is set, so the row has a default
   // format (set by the index of byte 18).
   if(rowheight == ROW_DFLT_HEIGHT)
   {
      AddValue16((unsigned16_t)(ROW_DFLT_GRBIT));
   }
   else
   {
      AddValue16((unsigned16_t)(ROW_DFLT_GRBIT|ROW_GRBIT_UNSYNC));   
   }
   AddValue16((unsigned16_t)ROW_DFLT_IXFE);


   SetRecordLength(GetDataSize()-4);
}

// NOTE: row_t has not a height field
CRow::CRow(row_t& rowdef)
{
   SetRecordType(RECTYPE_ROW);
   AddValue16(rowdef.rownum);
   AddValue16(rowdef.firstcol);
   AddValue16((rowdef.lastcol+1));
   AddValue16(ROW_DFLT_HEIGHT);

   // A field used by MS for "optimizing" (?) the loading of a file.
   // Doc says it shall be set to 0 if I'm creating a BIFF...
   AddValue16((unsigned16_t)0);
   // A reserved value:
   AddValue16((unsigned16_t)0);

   // TODO: The following flag-word can be used for outline cells. Later.
   // As a defaule the GhostDirty flag is is set, so the row has a default
   // format (set by the index of byte 18).
   AddValue16((unsigned16_t)(ROW_DFLT_GRBIT|ROW_GRBIT_UNSYNC));
   AddValue16((unsigned16_t)ROW_DFLT_IXFE);


   SetRecordLength(GetDataSize()-4);
}


CRow::~CRow()
{
}


/*
******************************
******************************
*/
void CRow::SetFirstCol(unsigned16_t firstcol)
{
   SetValueAt((signed16_t)firstcol, ROW_OFFSET_FIRSTCOL);
}

/*
******************************
******************************
*/
void CRow::SetLastCol(unsigned16_t lastcol)
{
   SetValueAt((signed16_t)lastcol, ROW_OFFSET_LASTCOL);
}
  
/*
******************************
******************************
*/
unsigned16_t CRow::GetFirstCol(void)
{

   signed16_t firstcol;
   GetValue16From(&firstcol,ROW_OFFSET_FIRSTCOL);
   return (unsigned16_t)firstcol;

}

/*
******************************
******************************
*/
unsigned16_t CRow::GetLastCol(void)
{
   signed16_t lastcol;
   GetValue16From(&lastcol,ROW_OFFSET_LASTCOL);
   return (unsigned16_t)lastcol;
}





/*
******************************
CDBCell class implementation
******************************
*/

CDBCell::CDBCell(unsigned32_t startblock)
{

   // The new initializated DBCell record points to nowhere and has no 
   // extra rows (the array of stream offsets is empty);
   SetRecordType(RECTYPE_DBCELL);
   AddValue32((unsigned32_t) startblock);
    
    

   SetRecordLength(GetDataSize()-4);
}

CDBCell::~CDBCell()
{
}


void CDBCell::AddRowOffset(unsigned16_t rowoffset)
{    

   AddValue16((unsigned16_t) rowoffset);
   SetRecordLength(GetDataSize()-4);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: row.cpp,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:53  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

