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
 * $Source: /cvsroot/xlslib/xlslib/src/oledoc/binfile.h,v $
 * $Revision: 1.2 $
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


#ifndef BINFILE_H
#define BINFILE_H

#include <fstream>
#include <string>

#include <XLS/config.h>
#include <XLS/systype.h>
#include <XLS/unit.h>

namespace xlslib_core
{

  /* 
******************************
CBinFile class declaration 
******************************
*/

#ifdef XLS_PHP
  typedef int (*write_fptr_t)(const char *str, unsigned int str_length);
#endif

  class CBinFile
    {
    private:
      int write_service(const char *buffer, size_t size);
      static const int STROKE_CHUNK_SIZE;
    public:
      CBinFile();
      CBinFile(const std::string& name);
      ~CBinFile();


      int Open(const std::string& name);
      int Close(void);   
      int Write ( unsigned8_t * data, unsigned32_t size );
      int Write (CUnit& data_unit );
    
      int WriteByte(unsigned8_t byte);
      int WriteUnsigned16(unsigned16_t data);
      int WriteUnsigned32(unsigned32_t data);
      int WriteSigned16(signed16_t data);
      int WriteSigned32(signed32_t data);
      int WriteByteArray(const unsigned8_t *data, size_t size);
      int SerializeFixedArray(const unsigned8_t data, unsigned32_t size);
	  
	  unsigned32_t Position();	// debugging, position of stream
    
#ifdef XLS_PHP
      void SetStroke(){  
        is_stroke= true;
      }
      void UnSetStroke(){  
        is_stroke= false;
      }
#endif

    protected:
      std::ofstream m_File;
#ifdef XLS_PHP
      write_fptr_t write_fptr;
      bool is_stroke;
#endif

    };

}

#endif //BINFILE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: binfile.h,v $
 * Revision 1.2  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:43  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

