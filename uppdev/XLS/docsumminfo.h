/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) Yeico S. A. de C. V.
 *  
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/docsumminfo.h,v $
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


#ifndef DOCSUMMARYINFO_H
#define DOCSUMMARYINFO_H 

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/datast.h>


namespace xlslib_core
{

  /*
********************************
CDocSummaryInfo class declaration
********************************
*/


  class CDocSummaryInfo: public CDataStorage
    {
    private:
      static const  unsigned8_t doc_summ_info_data[];

    protected:

    public:
      CDocSummaryInfo();
      ~CDocSummaryInfo();

      CUnit* DumpData();

    };

}
#endif //DOCSUMMARYINFO_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: docsumminfo.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:54  darioglz
 * Initial Import.
 *
 * Revision 1.6  2004/04/06 22:35:43  dgonzalez
 * + Added namespace xlslib_core to isolate the core xlslib library names.
 *
 * Revision 1.5  2004/01/29 03:18:55  dgonzalez
 * + Using the config.h file
 *
 * Revision 1.4  2003/12/05 00:30:06  dgonzalez
 * + Common include files were placed in common.h (and this file was
 *     included instead.
 *
 * Revision 1.3  2003/11/26 16:49:26  dgonzalez
 * + Indented and untabified
 *
 * Revision 1.2  2003/10/24 23:24:32  dgonzalez
 * + Added CVS-Keyword substitution.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
