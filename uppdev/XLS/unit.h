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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/unit.h,v $
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

#ifndef UNIT_H
#define UNIT_H

#include <XLS/config.h>
#include <XLS/common.h>

namespace xlslib_core
{


#define UNIT_MAX_SIZE (0xFFFF)

  // Error codes
#define ERR_DATASTORAGE_EMPTY (-2)
#define ERR_INVALID_INDEX     (-3)
#define ERR_UNNABLE_TOALLOCATE_MEMORY     (-3)

  //Block definitions
#define BIG_BLOCK_SIZE     (0x200)
#define SMALL_BLOCK_SIZE   (0x040)
#define PROP_BLOCK_SIZE    (0x080)

  class CUnit {
    // Attributes
  protected:
	size_t			m_nSize;		// Size of data stored
	unsigned8_t*	m_pData;		// Data storage
	unsigned32_t	m_nDataSize;	// Next empty space
	//  bool m_ShadowUnit;

	// Static attributes
	static const unsigned8_t DefaultInflateSize;

    // Operations
  public:
    CUnit();
    virtual ~CUnit();

    virtual unsigned8_t& operator[](const unsigned32_t index);
    virtual CUnit&   operator=(CUnit& newvalue);
    virtual CUnit&   operator+=(CUnit& from);
    virtual CUnit&   operator+= ( unsigned8_t from );

    virtual size_t GetSize (void);
    virtual unsigned32_t GetDataSize (void);
    unsigned8_t* GetBuffer (void);
    signed8_t Init (unsigned8_t* data, 
                    const size_t size, 
                    const unsigned32_t datasz);

    signed8_t AddDataArray (const unsigned8_t* newdata, size_t size);
    signed8_t AddFixedDataArray (const unsigned8_t value, size_t size);

#if VERSION_BIFF == VERSION_BIFF8
#ifdef HAVE_ICONV
	void Conv32to16(std::string& iconv_code, std::ustring& str1, u16string& str2);
#endif
	// DFH - to convert ascii string to unicode strings for BIFF8
	signed8_t AddUnicodeStrlen(unsigned8_t newdata);
	signed8_t AddUnicodeStrlen(unsigned16_t newdata);
    signed8_t AddUnicodeString (const unsigned8_t* newdata, unsigned16_t size);
    signed8_t AddUnicodeString (const unsigned16_t* newdata, unsigned16_t size);
#endif
    /*
      void SetShadow(bool shadowval);
      void CopyShadowUnit(unsigned8_t* data, unsigned32_t size);
    */
  protected:

    signed8_t Append (CUnit& newunit);
    signed8_t SetAt(unsigned8_t newval, unsigned32_t index);// Modify specific position

    signed8_t AddValue16(unsigned16_t newval);
    signed8_t AddValue32(unsigned32_t newval);
    signed8_t AddValue64(unsigned64_t newval);

    signed8_t SetValueAt(signed16_t newval, unsigned32_t index);
    signed8_t SetValueAt(signed32_t newval, unsigned32_t index);

    signed8_t GetValue16From(signed16_t* val, unsigned32_t index);
    signed8_t GetValue32From(signed32_t* val, unsigned32_t index);

    signed8_t SetArrayAt (const unsigned8_t* newdata, size_t size, unsigned32_t index);
    signed8_t GetAt(signed8_t* data, unsigned32_t  index);// Get specific data from storage
    //  signed8_t GetData(unsigned8_t** data, unsigned32_t from, unsigned32_t to );

    signed8_t RemoveTrailData (unsigned32_t remove_size);
    signed8_t AddValue8(unsigned8_t newdata);
    signed8_t InitFill (unsigned8_t data, unsigned32_t size);
    signed8_t Inflate(size_t increase = 0);

  };

}
#endif // UNIT_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: unit.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

