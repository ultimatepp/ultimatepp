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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/workbook.h,v $
 * $Revision: 1.3 $
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

#ifndef WORKBOOK_H
#define WORKBOOK_H 

#include <sstream>

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/ole/oledoc.h>
#include <XLS/datast.h>
#include <XLS/globalrec.h>
#include <XLS/sheetrec.h>
#include <XLS/recdef.h>
#include <XLS/continue.h>
#include <XLS/summinfo.h>
#include <XLS/docsumminfo.h>

namespace xlslib_core
{

  typedef enum
    {
      WB_INIT,
      WB_GLOBALRECORDS,
      WB_SHEETS,
      WB_CONTINUE_REC,
      WB_FINISH
    } WorkbookDumpState_t;


#ifdef XLS_PHP
  typedef enum
    {
      STK_INLINE=0,
      STK_ATTACHMENT
    } Stroke_Disposition_t;
#endif
  class workbook
    :public COleDoc
    {

    private:
		CSummaryInfo m_SummaryInfo;
		CDocSummaryInfo m_DocSummaryInfo;

		CGlobalRecords m_GlobalRecords;
		Sheets_Vector_t m_Sheets;
		WorkbookDumpState_t m_DumpState;
		WorkbookDumpState_t m_PreviousDumpState;

		CUnit* m_pCurrentData;

		// Continue record variables:
		CUnit* m_pContinueRecord;
		unsigned16_t m_ContinueIndex;

#ifdef HAVE_ICONV
		std::string		iconv_code;
#endif
    protected:

    public:
      workbook();
#ifdef XLS_PHP
      workbook(write_fptr_t w_fptr);
#endif
      ~workbook();

      worksheet* sheet(std::string sheetname);
#if VERSION_BIFF == VERSION_BIFF8
      worksheet* sheet(std::ustring sheetname);
#endif
      worksheet* GetSheet(unsigned16_t sheetnum);
   
      font_t* font(std::string name);
      format_t* format(std::string formatstr);

      xf_t* xformat(void);
      xf_t* xformat(font_t* font);

#ifdef HAVE_ICONV
      int iconvInType(const char *inType);
#endif

      CUnit* DumpData(void);	// oledoc use

      int Dump(std::string filename);

#ifdef XLS_PHP
      void Disposition(Stroke_Disposition_t disp);
      void Stroke(std::string filename);
      Stroke_Disposition_t m_Disposition;
#endif

    private:
      bool         dump_not_started;
      unsigned32_t globalrec_size;
      unsigned16_t current_sheet;
      unsigned32_t offset;               
    };

}
#endif //WORKBOOK_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: workbook.h,v $
 * Revision 1.3  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.2  2004/09/01 00:47:21  darioglz
 * + Modified to gain independence of target
 *
 * Revision 1.1.1.1  2004/08/27 16:31:44  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


