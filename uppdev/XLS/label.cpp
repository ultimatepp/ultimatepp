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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/label.cpp,v $
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


#include <XLS/label.h>

using namespace std;
using namespace xlslib_core;

/*
******************************
CLabel class implementation
******************************
*/
CLabel::CLabel(unsigned16_t row,	  
	       unsigned16_t col,	  
	       string& strlabel,	  
	       xf_t* pxfval)
{
   SetRecordType(RECTYPE_LABEL);
   AddValue16(row);
   AddValue16(col);

   unsigned16_t xfindex;
   if(pxfval != NULL)
      xfindex = pxfval->GetIndex();
   else
      xfindex = 0x000f;


   AddValue16(xfindex);	  

   signed16_t labelsize = strlabel.size();
#if VERSION_BIFF == VERSION_BIFF5
   AddValue16((unsigned16_t)labelsize);
   AddDataArray((unsigned8_t*)strlabel.c_str(), strlabel.size());
#else
   AddUnicodeStrlen((unsigned16_t)labelsize);
   AddUnicodeString((unsigned8_t*)strlabel.c_str(), strlabel.size());
#endif
	
   SetRecordLength(GetDataSize()-4);
}

#if VERSION_BIFF == VERSION_BIFF8

CLabel::CLabel(unsigned16_t row,	  
	       unsigned16_t col,	  
	       ustring& ustrlabel
#ifdef HAVE_ICONV		   
		   , string& iconv_code
#endif
		   , xf_t* pxfval)
{
   unsigned16_t xfindex;

   SetRecordType(RECTYPE_LABEL);
   AddValue16(row);
   AddValue16(col);

   if(pxfval != NULL)
      xfindex = pxfval->GetIndex();
   else
      xfindex = 0x000f;

   AddValue16(xfindex);	  

#ifdef HAVE_ICONV
	u16string u16sheetname;
	
	Conv32to16(iconv_code, ustrlabel, u16sheetname);
//	size_t	outSize, resultSize;
// size_t iconv (iconv_t cd, char **restrict inbuf, size_t *restrict inbytesleft, char **restrict outbuf, size_t *restrict outbytesleft);
	AddUnicodeStrlen((unsigned16_t)u16sheetname.size());
	AddUnicodeString((unsigned16_t* )u16sheetname.c_str(), u16sheetname.size());
#else
	AddUnicodeStrlen((unsigned16_t)ustrlabel.size());
	AddUnicodeString((unsigned16_t*)ustrlabel.c_str(), ustrlabel.size());
#endif

   SetRecordLength(GetDataSize()-4);
}
#endif

CLabel::CLabel(label_t& labeldef)

{
	SetRecordType(RECTYPE_LABEL);
	AddValue16(labeldef.GetRow());
	AddValue16(labeldef.GetCol());
	AddValue16(labeldef.GetXFIndex());

	signed16_t labelsize = labeldef.GetStrLabelSize() ;

#if VERSION_BIFF == VERSION_BIFF5
	AddValue16((unsigned16_t)labelsize);
	AddDataArray(labeldef.GetStrLabelBuffer(), labelsize);
#else
	if(labeldef.GetStrLabelSize()) {
		AddUnicodeStrlen((unsigned16_t)labelsize);
		AddUnicodeString(labeldef.GetStrLabelBuffer(), labelsize);
	} else {
#ifdef HAVE_ICONV
		u16string u16sheetname;
		string		iconvStr;
		ustring		label;
		
		iconvStr	= *labeldef.GetIconvCode();
		label		= *labeldef.GetUstrLabel();
		
		Conv32to16(iconvStr, label, u16sheetname);
	//	size_t	outSize, resultSize;
	// size_t iconv (iconv_t cd, char **restrict inbuf, size_t *restrict inbytesleft, char **restrict outbuf, size_t *restrict outbytesleft);
		AddUnicodeStrlen((unsigned16_t)u16sheetname.size());
		AddUnicodeString((unsigned16_t* )u16sheetname.c_str(), u16sheetname.size());
#else
		labelsize = labeldef.GetWStrLabelSize() ;
		AddUnicodeStrlen((unsigned16_t)labelsize);
		AddUnicodeString((unsigned16_t*)labeldef.GetWStrLabelBuffer(), labelsize);
#endif
	}
#endif

	SetRecordLength(GetDataSize()-4);
}

CLabel::~CLabel()
{
}

/*
******************************
label_t class implementation
******************************
*/
xlslib_core::label_t::label_t(unsigned16_t rowval, unsigned16_t colval, 
 		 string& labelstrval, xf_t* pxfval)
{
   row = rowval;
   col = colval;
   if(pxfval != NULL)
      pxfval->MarkUsed();
   pxf = pxfval;

#if VERSION_BIFF == VERSION_BIFF5
   if(labelstrval.size() > 255)
   {

      labelstrval.resize(255);
      labelstrval[252] = '.'; 
      labelstrval[253] = '.'; 
      labelstrval[254] = '.'; 

   }
#endif

   aStrlabel = labelstrval;
}

#if VERSION_BIFF == VERSION_BIFF8
xlslib_core::label_t::label_t(unsigned16_t rowval, unsigned16_t colval, 
 		 ustring& labelstrval
#ifdef HAVE_ICONV		 
		 , string& code
#endif		 
		 , xf_t* pxfval)
{
   row = rowval;
   col = colval;
   if(pxfval != NULL)
      pxfval->MarkUsed();
   pxf = pxfval;

   uStrlabel	= labelstrval;
#ifdef HAVE_ICONV
   iconv_code	= code;
#endif
}
#endif

/*
******************************
******************************
*/

unsigned16_t xlslib_core::label_t::GetSize() 
{
   unsigned16_t size = 0;

   size += 12; // Minimal label's size
   size += aStrlabel.size();

	  
   return size;
}
/*
******************************
******************************
*/

CUnit* xlslib_core::label_t::GetData()
{
   CUnit* datalabel;
   
#if VERSION_BIFF == VERSION_BIFF5
	datalabel = (CUnit*)( new CLabel(row,col,aStrlabel,pxf));
#else
	if(aStrlabel.length() > 0) {
		datalabel = (CUnit*)( new CLabel(row,col,aStrlabel,pxf));
	} else {
#ifdef HAVE_ICONV
		datalabel = (CUnit*)( new CLabel(row,col,uStrlabel,iconv_code, pxf));
#else
		datalabel = (CUnit*)( new CLabel(row,col,uStrlabel, pxf));
#endif
	}
#endif
	
   return datalabel; // NOTE: this pointer HAS to be deleted elsewhere.
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: label.cpp,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:53  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

