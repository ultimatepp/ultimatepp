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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/record.h,v $
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


#ifndef RECORD_H
#define RECORD_H

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/rectypes.h>
#include <XLS/unit.h>

namespace xlslib_core
{

#define  VERSION_BIFF5				0x0500
#define  VERSION_BIFF8				0x0600
#define  VERSION_BIFF				VERSION_BIFF8

  /* 
******************************
CRecord class declaration
******************************
*/
  class CRecord: public CUnit
    {
    protected:

      unsigned16_t m_Num;

    public:

      CRecord();
      ~CRecord();
  
      void SetRecordType(unsigned16_t rtype);
      unsigned16_t GetRecordType();

      void SetRecordLength(unsigned16_t);
      unsigned16_t GetRecordLength();

      unsigned8_t* GetRecordDataBuffer();
      unsigned32_t GetRecordDataSize();



    };
}
#endif //RECORD_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: record.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:54  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

