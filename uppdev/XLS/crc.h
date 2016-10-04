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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/crc.h,v $
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


#ifndef _crc_h
#define _crc_h

#include <string>

#include <XLS/config.h>
#include <XLS/systype.h>
#include <XLS/tostr.h>

#define FALSE   0
#define TRUE    !FALSE

/*
 * Select the CRC standard from the list that follows.
 */
#define CRC32

#define CRC_NAME               "CRC-32"

#define POLYNOMIAL              0x04C11DB7
#define INITIAL_REMAINDER       0xFFFFFFFF
#define FINAL_XOR_VALUE         0xFFFFFFFF
#define REFLECT_DATA            TRUE
#define REFLECT_REMAINDER       TRUE
#define CHECK_VALUE             0xCBF43926

namespace xlslib_core
{

  typedef unsigned32_t  crc32_t;

  class crc
    {
    public:
      crc();
      ~crc();

      crc32_t   get(std::string& message);
      crc32_t   get();

      void operator<<(std::string newstr) { m_data += newstr; }
      void operator<<(unsigned32_t newdata) { m_data += str_stream(newdata); }
      void operator<<(unsigned64_t newdata) { m_data += str_stream(newdata); }
      void operator<<(unsigned char newdata) { m_data += str_stream(newdata); }
      void operator<<(unsigned short newdata) { m_data += str_stream(newdata); }
      void operator<<(bool newdata) { m_data += str_stream(newdata); }
      void operator<<(double newdata) { m_data += str_stream(newdata); }
  
      std::string& getdata();
    private:
      std::string m_data;
      static const crc32_t CRC_32_TABLE[256];
      unsigned32_t reflect(unsigned32_t data, unsigned char nBits);
    };

}

#endif /* _crc_h */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: crc.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

