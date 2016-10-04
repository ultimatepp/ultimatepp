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
 * $Source: /cvsroot/xlslib/xlslib/src/common/systype.h,v $
 * $Revision: 1.3 $
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
 
#ifndef SYSTYPE_H
#define SYSTYPE_H

#if defined(_MSC_VER) && defined(WIN32)
#pragma warning(disable: 4786) // Long debug information related to STL
#pragma warning(disable: 4290) // C++ Exception Specification ignored
#endif

//#   define PHPEXT_TSRMLS_C  tsrm_ls
//#   define PHPEXT_TSRMLS_D  void ***tsrm_ls

// Global includes
#include <stdio.h>
#include <stdint.h>
#include <cstring>
#include <string>
#include <string.h>	// DFH 10-2-08
#include <iostream>
#include <list>
#include <set>
#include <algorithm>

#define INLINE inline

//#define NULL ((void*)(0))

#define NO_ERRORS        0
#define FILE_ERROR      -1
#define GENERAL_ERROR     ((signed8_t)-100)

#define PTYPE_DIRECTORY   ((unsigned8_t)(0x01))
#define PTYPE_FILE        ((unsigned8_t)(0x02))
#define PTYPE_ROOT        ((unsigned8_t)(0x05))

#define PCOLOR_BLACK      ((unsigned8_t)(0x01))
#define PCOLOR_RED		  ((unsigned8_t)(0x00))

#define PLINK_EMPTY       ((signed32_t)(-1))
#define PBLOCK_END        ((signed32_t)(-2))


#if USE_ALLOCATOR==ALLOCATOR_STL
#define XLSLIB_DFLT_ALLOCATOR

#elif USE_ALLOCATOR==ALLOCATOR_BTI
#include "bti_thread_alloc.h"
#define XLSLIB_DFLT_ALLOCATOR ,bti::thr_alloc

#else
#   error ERROR: Invalid allocator option
#endif

#ifdef uint32_t	// if we have this one we will have all the others too

typedef unsigned char      unsigned8_t;
typedef unsigned short int unsigned16_t;
typedef unsigned int       unsigned32_t;

typedef char               signed8_t;
typedef short int          signed16_t;
typedef int                signed32_t;

#else

typedef uint8_t				unsigned8_t;
typedef uint16_t			unsigned16_t;
typedef uint32_t			unsigned32_t;

typedef int8_t				signed8_t;
typedef int16_t				signed16_t;
typedef int32_t				signed32_t;

#endif

#if defined(_MSC_VER) && defined(WIN32)

typedef wchar_t uchar_t;
#define ustring wstring
//typedef wstring ustring;

#elif defined(__FRAMEWORK__)	// For Mac-OSX

#undef HAVE_ICONV

#if ! defined(unichar)
typedef uint16_t unichar;
#endif

typedef unichar uchar_t;
//typedef std::basic_string<unsigned16_t> ustring;
#define ustring basic_string<uint16_t>


#else

typedef wchar_t uchar_t;
#define ustring wstring
typedef std::basic_string<unsigned16_t> u16string;

#endif


#if defined(_MSC_VER) && defined(WIN32)

typedef __int64 unsigned64_t;

#else // not windows

#ifdef _UINT64_T

typedef uint64_t unsigned64_t;

#else

typedef unsigned long long unsigned64_t;

#endif

#endif

typedef std::list<std::string* XLSLIB_DFLT_ALLOCATOR> StringList_t;
typedef StringList_t::const_iterator StringListItor_t;

// Macros
# define MASK_BYTE0 0x00000000000000ff
# define MASK_BYTE1 0x000000000000ff00
# define MASK_BYTE2 0x0000000000ff0000
# define MASK_BYTE3 0x00000000ff000000

#if defined(_MSC_VER) && defined(WIN32)

# define MASK_BYTE4 0x000000ff00000000
# define MASK_BYTE5 0x0000ff0000000000
# define MASK_BYTE6 0x00ff000000000000
# define MASK_BYTE7 0xff00000000000000

#else

# define MASK_BYTE4 (unsigned64_t)0x000000ff00000000LL
# define MASK_BYTE5 (unsigned64_t)0x0000ff0000000000LL
# define MASK_BYTE6 (unsigned64_t)0x00ff000000000000LL
# define MASK_BYTE7 (unsigned64_t)0xff00000000000000LL

#endif

#define BYTE_0(num) (unsigned8_t) (num & MASK_BYTE0)
#define BYTE_1(num) (unsigned8_t)((num & MASK_BYTE1)>>0x08)
#define BYTE_2(num) (unsigned8_t)((num & MASK_BYTE2)>>0x10)
#define BYTE_3(num) (unsigned8_t)((num & MASK_BYTE3)>>0x18)

#define BYTE_4(num) (unsigned8_t)((num & MASK_BYTE4)>>0x20)
#define BYTE_5(num) (unsigned8_t)((num & MASK_BYTE5)>>0x28)
#define BYTE_6(num) (unsigned8_t)((num & MASK_BYTE6)>>0x30)
#define BYTE_7(num) (unsigned8_t)((num & MASK_BYTE7)>>0x38)



#define PROPERTY_DFLT_NOTUSED		((unsigned8_t)(0x00))
#define PROPERTY_MAX_NAME_LENGTH	((unsigned8_t)(32))

#define PROPERTY_TYPE_DIR			((unsigned8_t)(0x01))
#define PROPERTY_TYPE_FILE			((unsigned8_t)(0x02))
#define PROPERTY_TYPE_ROOT			((unsigned8_t)(0x05))

#define PROPERTY_COLOR_NODE_RED		((unsigned8_t)(0x00))
#define PROPERTY_COLOR_NODE_BLACK	((unsigned8_t)(0x01))
#define PROPERTY_LINK_EMPTY			((signed32_t)(-1))

#define PPTPOS_NAME					((unsigned8_t)(0x00))
#define PPTPOS_NAMELENGTH			((unsigned8_t)(0x40))
#define PPTPOS_TYPE					((unsigned8_t)(0x42))
#define PPTPOS_NODECOLOR			((unsigned8_t)(0x43))
#define PPTPOS_PREVIOUS				((unsigned8_t)(0x44))
#define PPTPOS_NEXT					((unsigned8_t)(0x48))
#define PPTPOS_CHILD				((unsigned8_t)(0x4C))
#define PPTPOS_UNUSED_EMPTY0		((unsigned8_t)(0x50))


#define PPTPOS_SECS1				((unsigned32_t)(0x64))
#define PPTPOS_SECS2				((unsigned32_t)(0x68))
#define PPTPOS_DAYS1				((unsigned32_t)(0x6c))
#define PPTPOS_DAYS2				((unsigned32_t)(0x70))

#define PPTPOS_STARTBLOCK			((unsigned8_t)(0x74))
#define PPTPOS_SIZE					((unsigned8_t)(0x78))


#define PPT_DFLT_SECS1				((signed32_t)(0))
#define PPT_DFLT_SECS2				((signed32_t)(0))
#define PPT_DFLT_DAYS1				((signed32_t)(0))
#define PPT_DFLT_DAYS2				((signed32_t)(0))


// More macros

/*  
****************************************
****************************************
*/
#define Delete_Pointer(pt) {                    \
  if(pt != NULL)                                \
    {                                           \
      delete pt;                                \
      pt = NULL;                                \
    }                                           \
}


/*  
****************************************
****************************************
*/
#ifdef __DEBUG__
#define XTRACE(str) {                           \
  cout<<"DEBUG: "<<str<<endl;                   \
}

#else
#define XTRACE(str)
#endif


#ifdef __PHP_DEBUG__
#define PHPTRACE(str) {                         \
  cout<<"PHPDEBUG: "<<str<<endl;                \
}

#else
#define PHPTRACE(str)
#endif


#endif //SYSTYPE_H


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: systype.h,v $
 * Revision 1.3  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.2  2004/09/01 00:47:04  darioglz
 * + Modified to gain independence of target
 *
 * Revision 1.1.1.1  2004/08/27 16:32:05  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

