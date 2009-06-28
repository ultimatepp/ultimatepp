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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/recdef.cpp,v $
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


#include <XLS/recdef.h>

using namespace std;
using namespace xlslib_core;

/*
******************************
CBof class implementation
******************************
*/

CBof::CBof(unsigned16_t boftype)
{
   SetRecordType(RECTYPE_BOF  );

	AddValue16(VERSION_BIFF);
	AddValue16(boftype);
	AddValue16(BOF_BUILD_DFLT);
	AddValue16(BOF_YEAR_DFLT);

#if VERSION_BIFF == VERSION_BIFF8
	AddValue32(0);				//The file hystory flags are all set to zero
	AddValue32(VERSION_BIFF);		// The lowest BIFF version
#endif

   SetRecordLength(GetDataSize()-4);
}

CBof::~CBof()
{}

/*
******************************
CEof class implementation
******************************
*/
CEof::CEof()
{
   SetRecordType(RECTYPE_EOF  );
   SetRecordLength(GetDataSize()-4);
}

CEof::~CEof()
{
}

/*
**********************************
CCodePage class implementation
**********************************
*/

CCodePage::CCodePage(unsigned16_t boftype)
{
   SetRecordType(RECTYPE_CODENAME);

   AddValue16(boftype);

   SetRecordLength(GetDataSize()-4);
}

CCodePage::~CCodePage()
{}

/*
**********************************
CWindow1 class implementation
**********************************
*/
CWindow1::CWindow1()
{
   SetRecordType(RECTYPE_WINDOW1);

   // The data of this record is hardcoded, since for file creation
   // this value takes a default (window dimensions and position).

   // NOTE: Doublecheck the previous statement!!
   unsigned8_t recwin1[] =  {0x40,0x02,0xfc,0x00,0xe0,0x37,0xe0,0x25,
                             0x38,0x00,0x00,0x00,0x00,0x00,0x01,0x00, 0x58,0x02};

   AddDataArray(recwin1, sizeof(recwin1));

   SetRecordLength(GetDataSize()-4);
}

CWindow1::~CWindow1()
{
}

/*
**********************************
CDateMode class implementation
**********************************
*/

CDateMode::CDateMode()
{
   SetRecordType(RECTYPE_R1904);

#ifdef __APPLE__
   AddValue16(1);	// 1904
#else
   AddValue16(0);	// 1900
#endif
   SetRecordLength(GetDataSize()-4);
}

CDateMode::~CDateMode()
{}

/*
**********************************
CWindow2 class implementation
**********************************
*/
CWindow2::CWindow2()
{
   SetRecordType(RECTYPE_WINDOW2);

   AddValue16(W2_DFLT_GRBIT);
   AddValue16(W2_DFLT_TOPROW);
   AddValue16(W2_DFLT_LEFTCOL);

#if VERSION_BIFF == VERSION_BIFF5
	AddValue32(W2_DFLT_COLOR);
#else
	AddValue16(0);						// 0 == BLACK
	AddValue16(0);
	AddValue16(0);						// zoom, default == 0 (W2_DFLT_ZOOMPBPREV ???)
	AddValue16(0);						// xoom, default == 0 (W2_DFLT_ZOOMNORMAL ???)
	AddValue32(W2_DFLT_RESERVED); 
#endif

   SetRecordLength(GetDataSize()-4);
}

CWindow2::~CWindow2()
{
}

/*
**********************************
CDimension class implementation
**********************************
*/
CDimension::CDimension(unsigned32_t minRow, unsigned32_t maxRow, unsigned32_t minCol, unsigned32_t maxCol)
{
   SetRecordType(RECTYPE_DIMENSIONS);

#if VERSION_BIFF == VERSION_BIFF5
	AddValue16((unsigned26_t)minRow);
	AddValue16((unsigned26_t)(maxRow+1));
#else
	AddValue32(minRow);
	AddValue32(maxRow+1);
#endif
	AddValue16(minCol);
	AddValue16(maxCol+1);						// zoom, default == 0 (W2_DFLT_ZOOMPBPREV ???)
	AddValue16(W2_DFLT_RESERVED); 

   SetRecordLength(GetDataSize()-4);
}

CDimension::~CDimension()
{
}

/*
**********************************
**********************************
*/
void CWindow2::SetSelected()
{
   unsigned16_t grbitval;
   GetValue16From((signed16_t*)&grbitval, W2_OFFSET_GRBIT);

   grbitval |= W2_GRBITMASK_SELECTED;

   SetValueAt((signed16_t)grbitval, W2_OFFSET_GRBIT);

}

/*
**********************************
**********************************
*/
void CWindow2::SetPaged()
{
   unsigned16_t grbitval;
   GetValue16From((signed16_t*)&grbitval, W2_OFFSET_GRBIT);

   grbitval |= W2_GRBITMASK_PAGED;

   SetValueAt((signed16_t)grbitval, W2_OFFSET_GRBIT);

}

/*
**********************************
**********************************
*/
void CWindow2::ClearSelected()
{
   unsigned16_t grbitval;
   GetValue16From((signed16_t*)&grbitval, W2_OFFSET_GRBIT);

   grbitval &= (~W2_GRBITMASK_SELECTED);

   SetValueAt((signed16_t)grbitval, W2_OFFSET_GRBIT);

}

/*
**********************************
**********************************
*/
void CWindow2::ClearPaged()
{
   unsigned16_t grbitval;
   GetValue16From((signed16_t*)&grbitval, W2_OFFSET_GRBIT);

   grbitval &= (~W2_GRBITMASK_PAGED);

   SetValueAt((signed16_t)grbitval, W2_OFFSET_GRBIT);

}


/*
**********************************
CPalette class implementation
**********************************
*/
/*
  CPalette::CPalette()
  {
  SetRecordType(RECTYPE_PALETTE);

  // The data of this record is hardcoded, since for file creation
  // this value takes a default (Documentation says that if the BIFF
  // file doesn't have a PALETTE record, it uses a default palette, but
  // somehow if this record is removed Excel displays an error).
  unsigned8_t palette[] =
  {
  0x38,0x00,0x00,0x00, 0x00,0x00,0xff,0xff,0xff,0x00,0xff,0x00,0x00,0x00,
  0x00,0xff,0x00,0x00,0x00,0x00,0xff,0x00, 0xff,0xff,0x00,0x00,0xff,0x00,0xff,0x00,
  0x00,0xff,0xff,0x00,0x80,0x00,0x00,0x00, 0x00,0x80,0x00,0x00,0x00,0x00,0x80,0x00,
  0x80,0x80,0x00,0x00,0x80,0x00,0x80,0x00, 0x00,0x80,0x80,0x00,0xc0,0xc0,0xc0,0x00,
  0x80,0x80,0x80,0x00,0x99,0x99,0xff,0x00, 0x99,0x33,0x66,0x00,0xff,0xff,0xcc,0x00,
  0xcc,0xff,0xff,0x00,0x66,0x00,0x66,0x00, 0xff,0x80,0x80,0x00,0x00,0x66,0xcc,0x00,
  0xcc,0xcc,0xff,0x00,0x00,0x00,0x80,0x00, 0xff,0x00,0xff,0x00,0xff,0xff,0x00,0x00,
  0x00,0xff,0xff,0x00,0x80,0x00,0x80,0x00, 0x80,0x00,0x00,0x00,0x00,0x80,0x80,0x00,
  0x00,0x00,0xff,0x00,0x00,0xcc,0xff,0x00, 0xcc,0xff,0xff,0x00,0xcc,0xff,0xcc,0x00,
  0xff,0xff,0x99,0x00,0x99,0xcc,0xff,0x00, 0xff,0x99,0xcc,0x00,0xcc,0x99,0xff,0x00,
  0xe3,0xe3,0xe3,0x00,0x33,0x66,0xff,0x00, 0x33,0xcc,0xcc,0x00,0x99,0xcc,0x00,0x00,
  0xff,0xcc,0x00,0x00,0xff,0x99,0x00,0x00, 0xff,0x66,0x00,0x00,0x66,0x66,0x99,0x00,
  0x96,0x96,0x96,0x00,0x00,0x33,0x66,0x00, 0x33,0x99,0x66,0x00,0x00,0x33,0x00,0x00,
  0x33,0x33,0x00,0x00,0x99,0x33,0x00,0x00, 0x99,0x33,0x66,0x00,0x33,0x33,0x99,0x00,
  0x33,0x33,0x33,0x00
  };


  AddDataArray(palette, sizeof(palette));

  SetRecordLength(GetDataSize()-4);
  }

  CPalette::~CPalette()
  {
  }

*/
/*
******************************
CStyle class implementation
******************************
*/
#define STYLE_BUILTIN_NORMAL      ((unsigned8_t)0x00)
#define STYLE_BUILTIN_ROWLEVELN   ((unsigned8_t)0x01)
#define STYLE_BUILTIN_COLLEVELN   ((unsigned8_t)0x02)
#define STYLE_BUILTIN_COMMA       ((unsigned8_t)0x03)
#define STYLE_BUILTIN_CURRENCY    ((unsigned8_t)0x04)
#define STYLE_BUILTIN_PERCENT     ((unsigned8_t)0x05)
#define STYLE_BUILTIN_COMMAT      ((unsigned8_t)0x06)
#define STYLE_BUILTIN_CURRENCYT   ((unsigned8_t)0x07)

#define STYLE_BUILTIN_BIT   ((unsigned16_t)0x8000)



#define STYLE_LEVEL_DUMMY         ((unsigned8_t)0x00)

CStyle::CStyle(style_t* styledef)
{

   // TODO: Implement user-defined styles. So far only built-in are used.
   SetRecordType(RECTYPE_STYLE);

   AddValue16(styledef->xfindex|STYLE_BUILTIN_BIT);
   AddValue8(styledef->builtintype);
   AddValue8(STYLE_LEVEL_DUMMY);


   SetRecordLength(GetDataSize()-4);
}

CStyle::CStyle(unsigned16_t xfindex, unsigned8_t builtintype)
{
   // TODO: Implement user-defined styles. So far only built-in are used.
   SetRecordType(RECTYPE_STYLE);

   AddValue16(xfindex|STYLE_BUILTIN_BIT);
   AddValue8(builtintype);
   AddValue8(STYLE_LEVEL_DUMMY);


   SetRecordLength(GetDataSize()-4);


}


CStyle::~CStyle()
{
}



/*
******************************
CBSheet class implementation
******************************
*/
CBSheet::CBSheet(unsigned32_t streampos, 
                 unsigned16_t attributes, 
                 string& sheetname)
{
	SetRecordType(RECTYPE_BOUNDSHEET);
	AddValue32(streampos);
	AddValue16(attributes);

#if VERSION_BIFF == VERSION_BIFF5
	AddValue8((unsigned8_t)sheetname.size());
	AddDataArray((unsigned8_t*)sheetname.c_str(), sheetname.size());
#endif
#if VERSION_BIFF == VERSION_BIFF8
	AddUnicodeStrlen((unsigned8_t)sheetname.size());
	AddUnicodeString((unsigned8_t* )sheetname.c_str(), sheetname.size());
#endif

	SetRecordLength(GetDataSize()-4);
}
#if VERSION_BIFF == VERSION_BIFF8
CBSheet::CBSheet(unsigned32_t streampos, 
                 unsigned16_t attributes, 
                 ustring& sheetname
#ifdef HAVE_ICONV
				 , string& iconv_code
#endif
) {

	SetRecordType(RECTYPE_BOUNDSHEET);
	AddValue32(streampos);
	AddValue16(attributes);

#ifdef HAVE_ICONV
	u16string u16sheetname;
	
	Conv32to16(iconv_code, sheetname, u16sheetname);
//	size_t	outSize, resultSize;
// size_t iconv (iconv_t cd, char **restrict inbuf, size_t *restrict inbytesleft, char **restrict outbuf, size_t *restrict outbytesleft);
	AddUnicodeStrlen((unsigned8_t)u16sheetname.size());
	AddUnicodeString((unsigned16_t* )u16sheetname.c_str(), u16sheetname.size());
#else
	AddUnicodeStrlen((unsigned8_t)sheetname.size());
	AddUnicodeString((unsigned16_t* )sheetname.c_str(), sheetname.size());
#endif

	SetRecordLength(GetDataSize()-4);
}
#endif

CBSheet::CBSheet(boundsheet_t* bsheetdef)
{
   SetRecordType(RECTYPE_BOUNDSHEET);
   AddValue32(bsheetdef->streampos);
    
   // Set the flags in the attribute variables
   unsigned16_t attrflags = 0;
   attrflags |=  bsheetdef->worksheet? BSHEET_ATTR_WORKSHEET:0;
   attrflags |=  bsheetdef->ex4macro?  BSHEET_ATTR_EX4MACRO:0;
   attrflags |=  bsheetdef->chart? BSHEET_ATTR_CHART:0;
   attrflags |=  bsheetdef->vbmodule? BSHEET_ATTR_VBMODULE:0;
   attrflags |=  bsheetdef->visible? BSHEET_ATTR_VISIBLE:0;
   attrflags |=  bsheetdef->hidden? BSHEET_ATTR_HIDDEN:0;
   attrflags |=  bsheetdef->veryhidden? BSHEET_ATTR_VERYHIDDEN:0;

   AddValue16(attrflags);

#if VERSION_BIFF == VERSION_BIFF5
	AddValue8((unsigned8_t)bsheetdef->sheetname.size());
	AddDataArray((unsigned8_t*)((bsheetdef->sheetname).c_str()), (bsheetdef->sheetname).size());
#endif
#if VERSION_BIFF == VERSION_BIFF8
	if(bsheetdef->asheetname.size()) {
		AddUnicodeStrlen((unsigned8_t)bsheetdef->asheetname.size());
		AddUnicodeString((unsigned8_t*)((bsheetdef->asheetname).c_str()), (bsheetdef->asheetname).size());
	} else {
#ifdef HAVE_ICONV
		u16string u16sheetname;
		
		Conv32to16(bsheetdef->iconv_code, bsheetdef->usheetname, u16sheetname);

		AddUnicodeStrlen((unsigned8_t)u16sheetname.size());
		AddUnicodeString((unsigned16_t* )u16sheetname.c_str(), u16sheetname.size());
#else
		AddUnicodeStrlen((unsigned8_t)bsheetdef->usheetname.size());
		AddUnicodeString((unsigned16_t*)(bsheetdef->usheetname.c_str()), (bsheetdef->usheetname).size());
#endif
	}
#endif

   SetRecordLength(GetDataSize()-4);   
}

CBSheet::~CBSheet()
{
}

/* 
**********************************
**********************************
*/

void CBSheet::SetStreamPosition(unsigned32_t pos)
{
   SetValueAt((signed32_t)pos, BSHEET_OFFSET_POSITION);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: recdef.cpp,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:48  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

