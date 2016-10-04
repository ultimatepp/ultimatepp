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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/workbook.cpp,v $
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

#include <XLS/workbook.h>

#ifdef HAVE_ICONV
#include <errno.h>
#endif

using namespace std;
using namespace xlslib_core;

#define CHANGE_DUMPSTATE(state) {               \
   m_PreviousDumpState = m_DumpState;           \
   m_DumpState = state;                         \
}

/*
**********************************************************************
workbook class implementation
**********************************************************************
*/

workbook::workbook()
   : m_DumpState(WB_INIT),
     m_PreviousDumpState(WB_FINISH),
     m_pCurrentData(NULL),
     m_pContinueRecord(NULL),
     dump_not_started(true),
     globalrec_size(0),
     current_sheet(0),
     offset(0)
{
#ifdef XLS_PHP
   write_fptr = NULL;
   m_Disposition = STK_ATTACHMENT;
#endif
#ifdef HAVE_ICONV
	iconv_code = "wchar_t";
#endif	
}

#ifdef XLS_PHP
workbook::workbook(write_fptr_t w_fptr)
   : m_DumpState(WB_INIT),
     m_PreviousDumpState(WB_FINISH),
     m_pCurrentData(NULL),
     m_pContinueRecord(NULL),
     dump_not_started(true),
     globalrec_size(0),
     current_sheet(0),
     offset(0)
{
	write_fptr = w_fptr;
	m_Disposition = STK_ATTACHMENT;
}
#endif

workbook::~workbook()
{
   if(!m_Sheets.empty())
   {
      for(unsigned32_t i = 0; i<m_Sheets.size(); i++)
      {
		delete m_Sheets[i];
      }
   }
}

#ifdef HAVE_ICONV
int workbook::iconvInType(const char *inType)
{
	int			ret;	
	iconv_t		cd;
	
	cd = iconv_open("UCS-2-INTERNAL", inType);
	if(cd != (iconv_t)(-1)) {
		iconv_close(cd);
		iconv_code = inType;
	
		ret = 0;
	} else {
		ret = errno;
	}
	return ret;
}
#endif

/*
***********************************
***********************************
*/

worksheet* workbook::sheet(string sheetname)
{

   worksheet* pnewsheet = new worksheet(&m_GlobalRecords);

   m_Sheets.push_back(pnewsheet);

   // NOTE: Streampos defaults to 0
   // It has to be set somewhere else
   m_GlobalRecords.AddBoundingSheet(0, BSHEET_ATTR_WORKSHEET, sheetname);

   // Return a pointer to the just added sheet
   return (m_Sheets.back());
}
#if VERSION_BIFF == VERSION_BIFF8
worksheet* workbook::sheet(ustring sheetname)
{
#ifdef HAVE_ICONV
   worksheet* pnewsheet = new worksheet(&m_GlobalRecords, iconv_code);
   m_Sheets.push_back(pnewsheet);

   // NOTE: Streampos defaults to 0
   // It has to be set somewhere else
   m_GlobalRecords.AddBoundingSheet(0, BSHEET_ATTR_WORKSHEET, sheetname, iconv_code);
#else
   worksheet* pnewsheet = new worksheet(&m_GlobalRecords);
   
   m_Sheets.push_back(pnewsheet);

   // NOTE: Streampos defaults to 0
   // It has to be set somewhere else
   m_GlobalRecords.AddBoundingSheet(0, BSHEET_ATTR_WORKSHEET, sheetname);
#endif   

   // Return a pointer to the just added sheet
   return (m_Sheets.back());
}
#endif

/*
***********************************
***********************************
*/

worksheet* workbook::GetSheet(unsigned16_t sheetnum)
{
   if(sheetnum < m_Sheets.size())
   {
      return m_Sheets[sheetnum];
   } else {
      return NULL;
   }
}

/*
***********************************
***********************************
*/
font_t* workbook::font(string  name)
{
   font_t* newfont = new font_t;
   newfont->SetName(name);

   return(m_GlobalRecords.AddFont(newfont));
}

/*
***********************************
***********************************
*/
format_t* workbook::format(string  formatstr)
{
   format_t* newformat = new  format_t(0x0000, formatstr);

   return(m_GlobalRecords.AddFormat(newformat));
}

xf_t* workbook::xformat(void)
{
   xf_t* newxf = new xf_t;
   newxf->MarkUsed();

   return(m_GlobalRecords.AddXFormat(newxf));
}

xf_t* workbook::xformat(font_t* font)
{
   xf_t* newxf = new xf_t;
   newxf->MarkUsed();
   newxf->SetFont(font);

   return(m_GlobalRecords.AddXFormat(newxf));
}

/*
***********************************
***********************************
*/
#define SETWORKBOOK5()                                              \
   CDataStorage biffdata;					   \
   CUnit* precorddata;						   \
   bool keep = true;						   \
   do { 							   \
      precorddata = DumpData();					   \
								   \
      if(precorddata != NULL)					   \
         biffdata += precorddata;				   \
      else							   \
         keep = false;						   \
   }while(keep);						   \
                                                                   \
   AddFile("/Book",&biffdata);					   \
   string name;							   \
   name = (char)0x05;						   \
   name += "SummaryInformation";				   \
   AddFile(name, &m_SummaryInfo);				   \
   name = (char)0x05;						   \
   name += "DocumentSummaryInformation";			   \
   AddFile(name, &m_DocSummaryInfo);
   
#define SETWORKBOOK8()						\
   CDataStorage biffdata;					\
   CUnit* precorddata;						\
   bool keep = true;						\
   do {										\
      precorddata = DumpData();				\
											\
      if(precorddata != NULL)				\
         biffdata += precorddata;			\
      else									\
         keep = false;						\
   } while(keep);							\
											\
   AddFile("/Workbook",&biffdata);			\
   string name;								\
   name = (char)0x05;						\
   name += "SummaryInformation";			\
   AddFile(name, &m_SummaryInfo);			\
   name = (char)0x05;						\
   name += "DocumentSummaryInformation";	\
   AddFile(name, &m_DocSummaryInfo);

#ifdef XLS_PHP
/******************************
 * This function has to be called before any output to the php-string
 * (this includes any non-php-script text in the source file or 
 * zend_printf call) because it makes use of headers() php-function,
 * which has to be called before any of those kind of output.
 * If this restraint isn't followed, the html-header won't be modified
 * and the binary data will be displayed in the browser (in the best of
 * cases).
 * This can also be avoided using buffering in the PHP environment
 * See chapter LXXVIII. "Output Control Functions" of the PHP manual.
 ******************************/
void workbook::Stroke(string filename)
{
      SetStroke();
	  
#if VERSION_BIFF == VERSION_BIFF5
      SETWORKBOOK5();
#else				// VERSION_BIFF8
      SETWORKBOOK8();
#endif
      DumpOleFile();
}

void workbook::Disposition(Stroke_Disposition_t disp)
{
   m_Disposition = disp;
}
#endif


int workbook::Dump(string filename)
{
   int errors;
   
   if(m_Sheets.empty()) {
	return GENERAL_ERROR;
   }
   
#ifdef XLS_PHP
   UnSetStroke();
#endif

   errors = Open(filename);
   if(errors == NO_ERRORS) {
#if VERSION_BIFF == VERSION_BIFF5
      SETWORKBOOK5();
#else				// VERSION_BIFF8
      SETWORKBOOK8();
#endif
	   errors = DumpOleFile();
	   Close();
   }
   return errors;
}

/*
***********************************
***********************************
*/

CUnit* workbook::DumpData(void)
{

   bool repeat = false;
   XTRACE("\nworkbook::DumpData");

   do
   {
      switch(m_DumpState)
      {
         case WB_INIT:

            dump_not_started = true;
            globalrec_size = 0;
            current_sheet = 0;
            offset = 0;

            CHANGE_DUMPSTATE(WB_GLOBALRECORDS);

            repeat = true;
            break;

         case WB_GLOBALRECORDS:

            XTRACE("\tGLOBALRECORDS");

            repeat = false;

            if(dump_not_started)
            {	// the following code is executed only if this is the first pas to the first dump-state
				// Basically, streampositions of BoundingSheet records are initialized.
               dump_not_started = false;

               globalrec_size = m_GlobalRecords.GetSize();
               offset =  globalrec_size;

               unsigned32_t offset_from_globalrecs = 0;

               if(!m_Sheets.empty())
               {

                  Boundsheet_List_Itor_t bs = m_GlobalRecords.GetFirstBoundSheet();

                  for(unsigned16_t i = 0; i < m_Sheets.size(); i++)
                  {

                     (*bs) -> streampos = globalrec_size + offset_from_globalrecs;
                     m_GlobalRecords.GetBoundingSheets(bs);
                     offset_from_globalrecs += m_Sheets[i]->GetSize();
                  }
               }
               else
               {
                  //Nothing else to do.
               }


               //TODO: Add other EOF/BOF blocks here
            }

            m_pCurrentData = m_GlobalRecords.DumpData();
            if(m_pCurrentData == NULL)
            {
               if(!m_Sheets.empty())
               {
                  // get in advance the first record of the first sheet
                  //m_pCurrentData = m_Sheets[0].DumpData(globalrec_size);
                  repeat = true;
                  CHANGE_DUMPSTATE(WB_SHEETS);
               }
               else
               {
                  // Nothing else to do. Branch to the FINISH state
                  m_pCurrentData = NULL;

                  CHANGE_DUMPSTATE(WB_FINISH);
               }
            }
            else
            {
               // Do nothing. Continue in this state.
            }

            break;

         case WB_SHEETS:
         {

            XTRACE("\tSHEETS");

            repeat = false;
            // If I got here, there's at least one sheet which has already
            // sent the first record block of data.
            //m_GlobalRecords.GetSize();

            m_pCurrentData = m_Sheets[current_sheet]->DumpData(offset);

            if(m_pCurrentData == NULL)
            {
               if((unsigned16_t)(current_sheet+1) < m_Sheets.size())
               {
                  // Update the offset for the next sheet
                  offset += m_Sheets[current_sheet]->GetSize();
                  current_sheet++;

                  // ... and get the first record of the next sheet.
                  // m_pCurrentData = m_Sheets[current_sheet].DumpData(offset);
                  repeat = true;
               }
               else
               {
                  // I'm done with all the sheets
                  // Nothing else to do. Branch to the FINISH state
                  CHANGE_DUMPSTATE(WB_FINISH);

                  repeat = true;
               }
            }


            break;
         }
         case WB_FINISH:
            XTRACE("\tFINISH");

            repeat = false;
            m_pCurrentData  = NULL;

            CHANGE_DUMPSTATE(WB_INIT);

            break;

         case WB_CONTINUE_REC:
            XTRACE("\tCONTINUE-REC");

            repeat = false;

            if(m_ContinueIndex == 0)
            {
               //Create a new data unit containing the max data size
               m_pContinueRecord = (CUnit*)(new CRecord());

               // The real size of the record is the size of the buffer minus the
               // size of the header record

               ((CUnit*)(m_pContinueRecord))->AddDataArray(((CRecord*)m_pCurrentData)->GetRecordDataBuffer(),MAX_RECORD_SIZE);
               ((CRecord*)(m_pContinueRecord))->SetRecordType(((CRecord*)m_pCurrentData)->GetRecordType());
               ((CRecord*)(m_pContinueRecord))->SetRecordLength(MAX_RECORD_SIZE);

//        m_pContinueRecord->SetValueAt(MAX_RECORD_SIZE-4,2);
               m_ContinueIndex++;

               return m_pContinueRecord;
            }
            else
            {
               //Delete_Pointer(m_pContinueRecord);

               // Get a pointer to the next chunk of data
               unsigned8_t* pdata = (((CRecord*)m_pCurrentData)->GetRecordDataBuffer()) + m_ContinueIndex*MAX_RECORD_SIZE;

               // Get the size of the chunk of data (that is the MAX_REC_SIZE except by the last one)
               unsigned32_t csize = 0;
               if(( ((CRecord*)m_pCurrentData)->GetRecordDataSize()/MAX_RECORD_SIZE) > (m_ContinueIndex))
               {
                  csize = MAX_RECORD_SIZE;
                  m_ContinueIndex++;

                  m_pContinueRecord =(CUnit*) ( new CContinue(pdata, csize));

                  return m_pContinueRecord;
               }
               else
               {
                  unsigned32_t data_size = ((CRecord*)m_pCurrentData)->GetRecordDataSize();

                  csize = (data_size - (m_ContinueIndex) * MAX_RECORD_SIZE);

                  // Restore the previous state (*Don't use the macro*)
                  m_DumpState = m_PreviousDumpState;
                  m_PreviousDumpState = WB_CONTINUE_REC;

                  m_ContinueIndex = 0;

                  if(csize)
                  {
                     m_pContinueRecord = (CUnit*) new CContinue(pdata, csize);
                     Delete_Pointer(m_pCurrentData);
                     return m_pContinueRecord;
                  }
                  else
                  {
                     Delete_Pointer(m_pCurrentData);
                     repeat = true;
                  }
               }
            }

            break;

         default:
            XTRACE("\tDEFAULT");
            break;
      }

      if(m_pCurrentData != NULL)
         if(((CRecord*)m_pCurrentData)->GetRecordDataSize() >= MAX_RECORD_SIZE && m_DumpState != WB_CONTINUE_REC)
         {

            // Save the current dump satate and change to the CONTINUE Record state
            CHANGE_DUMPSTATE(WB_CONTINUE_REC);

            m_ContinueIndex = 0;

            repeat = true;
         }
   } while(repeat);

   return m_pCurrentData;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: workbook.cpp,v $
 * Revision 1.3  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.2  2004/09/01 00:47:21  darioglz
 * + Modified to gain independence of target
 *
 * Revision 1.1.1.1  2004/08/27 16:31:49  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
