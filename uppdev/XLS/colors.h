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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/colors.h,v $
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


#include <XLS/config.h>

#ifndef COLORS_H
#define COLORS_H

namespace xlslib_core
{


#define COLOR_CODE_BLACK              (0x08)
#define COLOR_CODE_DARK_RED           (0x10)
#define COLOR_CODE_RED                (0x0a)
#define COLOR_CODE_FUCSIA             (0x0e)
#define COLOR_CODE_COMBINED01         (0x2d)
#define COLOR_CODE_COMBINED02         (0x3c)
#define COLOR_CODE_COMBINED03         (0x35)
#define COLOR_CODE_COMBINED04         (0x34)
#define COLOR_CODE_COMBINED05         (0x33)
#define COLOR_CODE_COMBINED06         (0x2f)
#define COLOR_CODE_OLIVE              (0x3b)
#define COLOR_CODE_DARK_YELLOW        (0x13)
#define COLOR_CODE_COMBINED07         (0x32)
#define COLOR_CODE_YELLOW             (0x0d)
#define COLOR_CODE_LIGHT_YELLOW       (0x2b)
#define COLOR_CODE_DARK_GREEN         (0x3a)
#define COLOR_CODE_GREEN              (0x11)
#define COLOR_CODE_COMBINED08         (0x39)
#define COLOR_CODE_LIVING_GREEN       (0x0b)
#define COLOR_CODE_LIGHT_GREEN        (0x2a)
#define COLOR_CODE_COMBINED09         (0x38)
#define COLOR_CODE_BLUE_GREEN         (0x15)
#define COLOR_CODE_AQUAMARINA         (0x31)
#define COLOR_CODE_TURQOISE           (0x0f)
#define COLOR_CODE_COMBINED10         (0x29)
#define COLOR_CODE_DARK_BLUE          (0x12)
#define COLOR_CODE_BLUE               (0x0c)
#define COLOR_CODE_LIGHT_BLUE         (0x30)
#define COLOR_CODE_SKY_BLUE           (0x28)
#define COLOR_CODE_COMBINED11         (0x2c)
#define COLOR_CODE_INDIGO             (0x3e)
#define COLOR_CODE_BLUE_GRAY          (0x36)
#define COLOR_CODE_VIOLET             (0x14)
#define COLOR_CODE_PLUM               (0x3d)
#define COLOR_CODE_LAVANDER           (0x2e)
#define COLOR_CODE_COMBINED12         (0x3f)
#define COLOR_CODE_GRAY50             (0x17)
#define COLOR_CODE_GRAY40             (0x37)
#define COLOR_CODE_GRAY25             (0x16)
#define COLOR_CODE_WHITE              (0x09)

  typedef enum
    {
      COLOR_BLACK,
      COLOR_DARK_RED,
      COLOR_RED,
      COLOR_FUCSIA,
      COLOR_COMBINED01,
      COLOR_COMBINED02,
      COLOR_COMBINED03,
      COLOR_COMBINED04,
      COLOR_COMBINED05,
      COLOR_COMBINED06,
      COLOR_OLIVE,
      COLOR_DARK_YELLOW,
      COLOR_COMBINED07,
      COLOR_YELLOW,
      COLOR_LIGHT_YELLOW,
      COLOR_DARK_GREEN,
      COLOR_GREEN,
      COLOR_COMBINED08,
      COLOR_LIVING_GREEN,
      COLOR_LIGHT_GREEN,
      COLOR_COMBINED09,
      COLOR_BLUE_GREEN,
      COLOR_AQUAMARINA,
      COLOR_TURQOISE,
      COLOR_COMBINED10,
      COLOR_DARK_BLUE,
      COLOR_BLUE,
      COLOR_LIGHT_BLUE,
      COLOR_SKY_BLUE,
      COLOR_COMBINED11,
      COLOR_INDIGO,
      COLOR_BLUE_GRAY,
      COLOR_VIOLET,
      COLOR_PLUM,
      COLOR_LAVANDER,
      COLOR_COMBINED12,
      COLOR_GRAY50,
      COLOR_GRAY40,
      COLOR_GRAY25,
      COLOR_WHITE           
    }color_name_t;

}

#endif //COLORS_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: colors.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:44  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

