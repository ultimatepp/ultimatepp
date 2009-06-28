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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/recdef.h,v $
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


#ifndef RECDEF_H
#define RECDEF_H

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/rectypes.h>
#include <XLS/record.h>

namespace xlslib_core
{

/*
******************************
CBof class declaration
******************************
*/
#define  BOF_TYPE_WBGLOBALS           ((unsigned16_t)0x0005)
#define  BOF_TYPE_VBMODULE            ((unsigned16_t)0x0006)
#define  BOF_TYPE_WORKSHEET           ((unsigned16_t)0x0010)
#define  BOF_TYPE_CHART               ((unsigned16_t)0x0020)
#define  BOF_TYPE_EXCEL4_MACROSH      ((unsigned16_t)0x0040)
#define  BOF_TYPE_WSFILE              ((unsigned16_t)0x0100)

#define  BOF_BUILD_DFLT               ((unsigned16_t)0x1d5f)
#define  BOF_YEAR_DFLT                ((unsigned16_t)0x07cd)

#define CODEPAGE_IBMPC               ((unsigned16_t)0x01b5)
#define CODEPAGE_APPLE               ((unsigned16_t)0x8000)
#define CODEPAGE_ANSI                ((unsigned16_t)0x04e4)

#define BOF_RECORD_SIZE  (12)

  class CBof: public CRecord
    {
    protected:

    public:

      CBof(unsigned16_t boftype);
      ~CBof();
    };

/*
******************************
CEof class declaration
******************************
*/
  class CEof: public CRecord
    {
    protected:

    public:

      CEof();
      ~CEof();
    };

/*
******************************
CCodePage class declaration
******************************
*/
  class CCodePage: public CRecord
    {
    protected:

    public:

      CCodePage(unsigned16_t boftype);
      ~CCodePage();
    };

  /*
******************************
CWindow1 class declaration
******************************
*/

  class CWindow1: public CRecord
    {
    protected:

    public:

      CWindow1();
      ~CWindow1();
    };

/*
******************************
CDateMode class declaration
******************************
*/
  class CDateMode: public CRecord
    {
    protected:

    public:

      CDateMode();
      ~CDateMode();
    };



/*
******************************
CWindow2 class declaration
******************************
*/

#define W2_OFFSET_GRBIT          ((unsigned32_t)4)
#define W2_OFFSET_TOPROW         ((unsigned32_t)6)
#define W2_OFFSET_LEFTCOL        ((unsigned32_t)8)
#define W2_OFFSET_COLOR          ((unsigned32_t)10)
#define W2_OFFSET_ZOOMPREVIEW    ((unsigned32_t)14)
#define W2_OFFSET_ZOOMNORMAL     ((unsigned32_t)16)
#define W2_OFFSET_RESERVED       ((unsigned32_t)18)


#define W2_DFLT_TOPROW     ((unsigned16_t)0x0000)
#define W2_DFLT_LEFTCOL    ((unsigned16_t)0x0000)
#define W2_DFLT_COLOR      ((unsigned32_t)0x00000000)
  // NOTE: Check a BIFF8 example to verify the units of the two following values
#define W2_DFLT_ZOOMPBPREV ((unsigned16_t)0x0100)
#define W2_DFLT_ZOOMNORMAL ((unsigned16_t)0x0100)
#define W2_DFLT_RESERVED   ((unsigned32_t)0x00000000)

  // GRBIT mask-flags:
#define W2_GRBITMASK_FMLA          ((unsigned16_t)0x0001)
#define W2_GRBITMASK_GRIDS         ((unsigned16_t)0x0002)
#define W2_GRBITMASK_HROWCOL       ((unsigned16_t)0x0004)
#define W2_GRBITMASK_FROZEN        ((unsigned16_t)0x0008)
#define W2_GRBITMASK_ZEROS         ((unsigned16_t)0x0010)
#define W2_GRBITMASK_DFLTHDRCOLOR  ((unsigned16_t)0x0020)
#define W2_GRBITMASK_ARABIC        ((unsigned16_t)0x0040)
#define W2_GRBITMASK_GUTS          ((unsigned16_t)0x0080)
#define W2_GRBITMASK_FRZNOSPLIT    ((unsigned16_t)0x0100)
#define W2_GRBITMASK_SELECTED      ((unsigned16_t)0x0200)
#define W2_GRBITMASK_PAGED         ((unsigned16_t)0x0400)
#define W2_GRBITMASK_SLV           ((unsigned16_t)0x0800)
#define W2_GRBITMASK_RESERVED      ((unsigned16_t)0xf000)

  // NOTE: Hardcoded from an excel example
#define W2_DFLT_GRBIT ((unsigned16_t)0x06b6)
  /*
    #define W2_DFLT_GRBIT ((unsigned16_t) \
    (W2_GRBITMASK_GRIDS|W2_GRBITMASK_HROWCOL|W2_GRBITMASK_DFLTHDRCOLOR)) 
  */
  class CWindow2: public CRecord
    {
    private:

    public:

      // TODO: Create a constructor that gets user-defined arguments that specify the appearence
      // The following constructor stablishes default values.
      CWindow2();
      ~CWindow2();

      void SetSelected();
      void SetPaged();
      void ClearSelected();
      void ClearPaged();
    };

/*
******************************
CDimension class declaration
******************************
*/
  class CDimension: public CRecord
    {
    protected:

    public:

      CDimension(unsigned32_t minRow, unsigned32_t maxRow, unsigned32_t minCol, unsigned32_t maxCol);
      ~CDimension();
    };

  /*
******************************
CStyle class declaration
******************************
*/

  typedef struct
  {
    unsigned16_t xfindex;
    unsigned8_t builtintype;
    
  } style_t;
  typedef std::list<xlslib_core::style_t* XLSLIB_DFLT_ALLOCATOR> Style_List_t;
  typedef Style_List_t::iterator Style_List_Itor_t;

  class CStyle: public CRecord
    {
    protected:

    public:

      CStyle(unsigned16_t xfindex, unsigned8_t builtintype);
      CStyle(style_t* styledef);
      ~CStyle();
    };


  /*
******************************
CBSheet class declaration
******************************
*/

#define BSHEET_OFFSET_POSITION    ((unsigned32_t)4)
#define BSHEET_OFFSET_FLAGS       ((unsigned32_t)8)
#define BSHEET_OFFSET_NAMELENGHT  ((unsigned32_t)10)
#define BSHEET_OFFSET_B7NAME      ((unsigned32_t)11)
#define BSHEET_OFFSET_B8NAME      ((unsigned32_t)12)

#define BSHEET_ATTR_WORKSHEET  ((unsigned16_t)0x0000)
#define BSHEET_ATTR_EX4MACRO   ((unsigned16_t)0x0001)
#define BSHEET_ATTR_CHART      ((unsigned16_t)0x0002)
#define BSHEET_ATTR_VBMODULE   ((unsigned16_t)0x0006)

#define BSHEET_ATTR_VISIBLE     ((unsigned16_t)0x0000)
#define BSHEET_ATTR_HIDDEN      ((unsigned16_t)0x0100)
#define BSHEET_ATTR_VERYHIDDEN  ((unsigned16_t)0x0200)

// TODO - DFH - are strings leaked????
  typedef struct
  {
    unsigned32_t streampos;
    std::string asheetname;
#if VERSION_BIFF == VERSION_BIFF8
    std::ustring usheetname;
#ifdef HAVE_ICONV
	std::string  iconv_code;
#endif
#endif
    bool _free :1;
    bool worksheet:1;      
    bool ex4macro:1;
    bool chart:1;
    bool vbmodule:1;
    bool visible:1;      
    bool hidden:1;
    bool veryhidden:1;
  } boundsheet_t;
  typedef std::list<xlslib_core::boundsheet_t* XLSLIB_DFLT_ALLOCATOR> Boundsheet_List_t;
  typedef Boundsheet_List_t::iterator Boundsheet_List_Itor_t;

  class CBSheet: public CRecord
    {
    private:

    public:

      CBSheet(unsigned32_t streampos,
              unsigned16_t attributes,
              std::string& sheetname);
#if VERSION_BIFF == VERSION_BIFF8
      CBSheet(unsigned32_t streampos,
              unsigned16_t attributes,
              std::ustring& sheetname
#ifdef HAVE_ICONV
			  , std::string& iconv_code
#endif
		);
#endif
      CBSheet(boundsheet_t* bsheetdef);

      ~CBSheet();

      void SetStreamPosition(unsigned32_t pos);
    };
}
#endif //RECDEF_H


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: recdef.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:57  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

