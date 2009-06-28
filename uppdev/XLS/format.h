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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/format.h,v $
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


#ifndef FORMAT_H
#define FORMAT_H

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/rectypes.h>
#include <XLS/record.h>

namespace xlslib_core
{

  // The font-record field offsets:
#define FORMAT_OFFSET_INDEX        ((unsigned32_t) 4)
#define FORMAT_OFFSET_NAMELENGTH   ((unsigned32_t) 6)
#define FORMAT_OFFSET_NAME         ((unsigned32_t) 7)

#define FMTCODE_GENERAL            ((unsigned16_t)0x0000) 
#define FMTCODE_NUMBER1            ((unsigned16_t)0x0001) 
#define FMTCODE_NUMBER2            ((unsigned16_t)0x0002)
#define FMTCODE_NUMBER3            ((unsigned16_t)0x0003)
#define FMTCODE_NUMBER4            ((unsigned16_t)0x0004)
#define FMTCODE_CURRENCY1          ((unsigned16_t)0x0005)
#define FMTCODE_CURRENCY2          ((unsigned16_t)0x0006)
#define FMTCODE_CURRENCY3          ((unsigned16_t)0x0007)
#define FMTCODE_CURRENCY4          ((unsigned16_t)0x0008)
#define FMTCODE_PERCENT1           ((unsigned16_t)0x0009)
#define FMTCODE_PERCENT2           ((unsigned16_t)0x000a)
#define FMTCODE_SCIENTIFIC1        ((unsigned16_t)0x000b)
#define FMTCODE_FRACTION1          ((unsigned16_t)0x000c)
#define FMTCODE_FRACTION2          ((unsigned16_t)0x000d)
#define FMTCODE_DATE1              ((unsigned16_t)0x000e)
#define FMTCODE_DATE2              ((unsigned16_t)0x000f)
#define FMTCODE_DATE3              ((unsigned16_t)0x0010)
#define FMTCODE_DATE4              ((unsigned16_t)0x0011)
#define FMTCODE_HOUR1              ((unsigned16_t)0x0012)
#define FMTCODE_HOUR2              ((unsigned16_t)0x0013)
#define FMTCODE_HOUR3              ((unsigned16_t)0x0014)
#define FMTCODE_HOUR4              ((unsigned16_t)0x0015)
#define FMTCODE_HOURDATE           ((unsigned16_t)0x0016)
#define FMTCODE_NUMBER5            ((unsigned16_t)0x0025)
#define FMTCODE_NUMBER6            ((unsigned16_t)0x0026)
#define FMTCODE_NUMBER7            ((unsigned16_t)0x0027)
#define FMTCODE_NUMBER8            ((unsigned16_t)0x0028)
#define FMTCODE_ACCOUNTING1        ((unsigned16_t)0x0029)
#define FMTCODE_ACCOUNTING2        ((unsigned16_t)0x002a)
#define FMTCODE_ACCOUNTING3        ((unsigned16_t)0x002b)
#define FMTCODE_ACCOUNTING4        ((unsigned16_t)0x002c)
#define FMTCODE_HOUR5              ((unsigned16_t)0x002d)
#define FMTCODE_HOUR6              ((unsigned16_t)0x002e)
#define FMTCODE_HOUR7              ((unsigned16_t)0x002f)
#define FMTCODE_SCIENTIFIC2        ((unsigned16_t)0x0030)
#define FMTCODE_TEXT               ((unsigned16_t)0x0031)

  typedef enum
    {
      FMT_GENERAL = 0,
      FMT_NUMBER1,
      FMT_NUMBER2,
      FMT_NUMBER3,
      FMT_NUMBER4,
      FMT_CURRENCY1,
      FMT_CURRENCY2,
      FMT_CURRENCY3,
      FMT_CURRENCY4,
      FMT_PERCENT1,
      FMT_PERCENT2,
      FMT_SCIENTIFIC1,
      FMT_FRACTION1,
      FMT_FRACTION2,
      FMT_DATE1,
      FMT_DATE2,
      FMT_DATE3,
      FMT_DATE4,
      FMT_HOUR1,
      FMT_HOUR2,
      FMT_HOUR3,
      FMT_HOUR4,
      FMT_HOURDATE,
      FMT_NUMBER5,
      FMT_NUMBER6,
      FMT_NUMBER7,
      FMT_NUMBER8,
      FMT_ACCOUNTING1,
      FMT_ACCOUNTING2,
      FMT_ACCOUNTING3,
      FMT_ACCOUNTING4,
      FMT_HOUR5,
      FMT_HOUR6,
      FMT_HOUR7,
      FMT_SCIENTIFIC2,
      FMT_TEXT          
    } format_number_t;

  /* 
******************************
CFormat class declaration
******************************
*/

  class format_t
    {
    public:
      format_t(unsigned16_t idx, std::string fmtstr)
        : index(idx), formatstr(fmtstr), m_usage_counter(0)
        {};
      ~format_t(){};
   
      unsigned16_t GetIndex(){return index;};
      void SetIndex(unsigned16_t idx){index = idx;};
      
      std::string& GetFormatStr(void) {return formatstr;};
      void SetFormatStr(std::string& fmtstr) {formatstr = fmtstr;};

      void MarkUsed() {m_usage_counter++;};
      bool IsUsed() {return(m_usage_counter != 0);};

    private:
  

      unsigned16_t index;
      std::string formatstr;

      unsigned16_t m_usage_counter;    
    };
  typedef std::list<xlslib_core::format_t* XLSLIB_DFLT_ALLOCATOR> Format_List_t;
  typedef Format_List_t::iterator Format_List_Itor_t;




  class CFormat: public CRecord
    {
    protected:

    public:

      CFormat(std::string&  formatstr, unsigned16_t index);
      CFormat(format_t* formatdef);
      ~CFormat();


      int SetFormatStr(std::string& formatstr);
      int GetFormatStr(std::string& formatstr);

      int SetIndex(unsigned16_t index);
      unsigned16_t GetIndex(void);

      //  int SetName(std::string& formatstr);

    };

}

#endif //FORMAT_H


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: format.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:57  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


