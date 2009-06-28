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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/globalrec.h,v $
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


#ifndef GLOBALREC_H
#define GLOBALREC_H

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/biffsection.h>
#include <XLS/font.h>
#include <XLS/format.h>
#include <XLS/extformat.h>
#include <XLS/recdef.h>
#include <XLS/unit.h>

namespace xlslib_core
{

  /*
***********************************
CGlobalRecords class declaration
***********************************
*/


#define GLOBAL_NUM_DEFAULT_FONT    (5)
#define GLOBAL_NUM_DEFAULT_FORMATS (8)
#define GLOBAL_NUM_DEFAULT_XFS     (16)
#define GLOBAL_NUM_DEFAULT_STYLES  (6)



  typedef enum
    {
      GLOBAL_INIT,
      GLOBAL_BOF,
      GLOBAL_CODEPAGE,
      GLOBAL_WINDOW1,
      GLOBAL_DATEMODE,
      GLOBAL_DEFAULTFONTS,
      GLOBAL_FONTS,
      GLOBAL_FORMATS,
      GLOBAL_DEFAULTXFS,
      GLOBAL_XFS,
      GLOBAL_STYLES,
      GLOBAL_BOUNDSHEETS,
      GLOBAL_EOF,
      GLOBAL_FINISH
    } GlobalRecordDumpState_t;



  class CGlobalRecords: public CBiffSection
    {
    private:
		Font_Set_t			m_Fonts;
		Font_List_t			m_DefaultFonts;
		Format_List_t		m_Formats;
		XF_Set_t			m_XFs;
		XF_List_t			m_DefaultXFs;
		Style_List_t		m_Styles;
		Boundsheet_List_t	m_BoundSheets;

      /*
        static const font_init_t   m_Default_Fonts  [GLOBAL_NUM_DEFAULT_FONT    ];
        static const format_t m_Default_Formats[GLOBAL_NUM_DEFAULT_FORMATS ];
        static const xf_init_t     m_Default_XFs    [GLOBAL_NUM_DEFAULT_XFS     ];
        static const style_t  m_Default_Styles [GLOBAL_NUM_DEFAULT_STYLES  ];
      */

      // State Machine variables

      GlobalRecordDumpState_t m_DumpState;
      CUnit* m_pCurrentData;
   
    public:
      CGlobalRecords();
      ~CGlobalRecords();

      void AddBoundingSheet(unsigned32_t streampos,
                            unsigned16_t attributes,
                            std::string& sheetname);
#if VERSION_BIFF == VERSION_BIFF8
      void AddBoundingSheet(unsigned32_t streampos,
                            unsigned16_t attributes,
                            std::ustring& sheetname
#ifdef HAVE_ICONV							
							, std::string& iconv_code
#endif
							);
#endif
      void AddBoundingSheet(boundsheet_t* bsheetdef);


      // TODO: Check if the font already exists (using CRC or raw comparison)
      font_t* AddFont(font_t* newfont );
      format_t* AddFormat(format_t*);

      xf_t* AddXFormat(xf_t* xf);
      xf_t* ReplaceXFormat(xf_t* xf, xf_t* xfnew);
      font_t* ReplaceFont(font_t* fnt, font_t* fntnew);   

      void GetBoundingSheets(Boundsheet_List_Itor_t &bs);
   
      // A Kind of state machine that will return
      // a non-null pointer to the data unit until all data has been retrieved.
      CUnit* DumpData(bool reset = false); 

      Boundsheet_List_Itor_t GetFirstBoundSheet();
      Boundsheet_List_Itor_t GetEndBoundSheet();
      unsigned32_t GetSize(void);
      font_t* GetDefaultFont(void);
      xf_t* GetDefaultXF(void);

    private:
      // Former static variiables
      // ========================

      // From DumpData
      Font_Set_Itor_t        font;
      Font_List_Itor_t       font_dflt;
      Format_List_Itor_t     format;

      XF_Set_Itor_t          xf;
      XF_List_Itor_t         xf_dflt;
  
      unsigned16_t           next_fontindex;
      unsigned16_t           next_xfindex;
      unsigned16_t           next_formatindex;
      Style_List_Itor_t      style;
      Boundsheet_List_Itor_t bsheet;
    };

}
#endif // GLOBALREC_H


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: globalrec.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:49  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

