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
 * $Source: /cvsroot/xlslib/xlslib/src/common/config.h,v $
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


#ifndef CONFIG_H
#define CONFIG_H

#ifdef HAVE_CONFIG_H
#include <XLS/xlconfig.h>

#define CFG_TRUE          1
#define CFG_FALSE         0

#define ALLOCATOR_STL     0
#define ALLOCATOR_BTI     1


#if defined(USE_BTI_ALLOC)
#   define ALLOCATOR_DFLT ALLOCATOR_BTI
#elif defined(ALLOC_BTI)
#   define ALLOCATOR_DFLT ALLOCATOR_STL
#else
#   define ALLOCATOR_DFLT ALLOCATOR_STL
#endif


#else // HAVE_CONFIG_H

#if defined(_MSC_VER) && defined(WIN32)
// Fakes the inclusion of crtdbg.h to avoid duplicated symbols
// of (inlined) new() and delete() operators
#define _INC_CRTDBG
#else
#endif

#define CFG_TRUE          1
#define CFG_FALSE         0

#define ALLOCATOR_STL     0
#define ALLOCATOR_BTI     1


#if defined(ALLOC_STL)
#   define ALLOCATOR_DFLT ALLOCATOR_STL
#elif defined(ALLOC_BTI)
#   define ALLOCATOR_DFLT ALLOCATOR_BTI
#else
#   define ALLOCATOR_DFLT ALLOCATOR_STL
#endif


#endif // HAVE_CONFIG_H

#endif //CONFIG_H
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: config.h,v $
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

