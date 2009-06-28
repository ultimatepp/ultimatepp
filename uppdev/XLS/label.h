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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/label.h,v $
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


#ifndef LABEL_H
#define LABEL_H

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/cell.h>
#include <XLS/record.h>
#include <XLS/unit.h>
#include <XLS/extformat.h>

namespace xlslib_core
{

#define LABEL_DFLT_XFINDEX              ((unsigned16_t)0x000f)
#define LABEL_OFFSET_FIRSTCOL           ((unsigned8_t)6)
#define LABEL_OFFSET_LASTCOL            ((unsigned8_t)8)

  class label_t;
  class CLabel: public CRecord
    {
    private:

    public:
      CLabel(unsigned16_t row,
             unsigned16_t col,
             std::string& strlabel,
             xf_t* pxfval = NULL);
#if VERSION_BIFF == VERSION_BIFF8
      CLabel(unsigned16_t row,
             unsigned16_t col,
             std::ustring& strlabel
#ifdef HAVE_ICONV			 
			 , std::string& iconv_code
#endif
			 , xf_t* pxfval = NULL);
#endif
      CLabel(label_t& labeldef);
      ~CLabel();
    };

  class label_t: public cell_t
    {

    private:
      std::string	aStrlabel;
#if VERSION_BIFF == VERSION_BIFF8
      std::ustring	uStrlabel;
#ifdef HAVE_ICONV
	  std::string	iconv_code;
#endif
#endif
    public:
      label_t(unsigned16_t rowval, 
              unsigned16_t colval, 
              std::string& labelstrval,
              xf_t* pxfval = NULL);

#if VERSION_BIFF == VERSION_BIFF8
      label_t(unsigned16_t rowval, 
              unsigned16_t colval, 
              std::ustring& labelstrval
#ifdef HAVE_ICONV
			  , std::string& code
#endif
			  , xf_t* pxfval = NULL);
#endif
      ~label_t(){};

      unsigned16_t			GetStrLabelSize() {return aStrlabel.size();};
      const unsigned8_t*	GetStrLabelBuffer(){return (unsigned8_t*)aStrlabel.c_str();};
#if VERSION_BIFF == VERSION_BIFF8
      unsigned16_t			GetWStrLabelSize() {return uStrlabel.size();};
      const uchar_t			*GetWStrLabelBuffer(){return (uchar_t*)uStrlabel.c_str();};
	  std::ustring			*GetUstrLabel() { return &uStrlabel; };
#ifdef HAVE_ICONV
	  std::string			*GetIconvCode() { return &iconv_code; };
#endif
#endif
      unsigned16_t			GetSize();
      CUnit*				GetData();

    };

}
#endif //LABEL_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: label.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

