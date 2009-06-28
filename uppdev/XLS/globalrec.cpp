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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/globalrec.cpp,v $
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


#include <XLS/globalrec.h>

using namespace std;
using namespace xlslib_core;


/*
**********************************************************************
CGlobalRecords class implementation
**********************************************************************
*/

/*

const font_init_t   CGlobalRecords::m_Default_Fonts  [GLOBAL_NUM_DEFAULT_FONT    ] =
{
{0x0000, "Arial", 0x00c8, BOLDNESS_NORMAL, UNDERLINE_NONE, SCRIPT_NONE, COLOR_BLACK, FONT_DFLT_ATTRIBUTES, FONT_DFLT_FAMILY, FONT_DFLT_CHARSET},
{0x0001, "Arial", 0x00c8, BOLDNESS_NORMAL, UNDERLINE_NONE, SCRIPT_NONE, COLOR_BLACK, FONT_DFLT_ATTRIBUTES, FONT_DFLT_FAMILY, FONT_DFLT_CHARSET},
{0x0002, "Arial", 0x00c8, BOLDNESS_NORMAL, UNDERLINE_NONE, SCRIPT_NONE, COLOR_BLACK, FONT_DFLT_ATTRIBUTES, FONT_DFLT_FAMILY, FONT_DFLT_CHARSET},
{0x0003, "Arial", 0x00c8, BOLDNESS_NORMAL, UNDERLINE_NONE, SCRIPT_NONE, COLOR_BLACK, FONT_DFLT_ATTRIBUTES, FONT_DFLT_FAMILY, FONT_DFLT_CHARSET},
{0x0005, "Arial", 0x00c8, BOLDNESS_NORMAL, UNDERLINE_NONE, SCRIPT_NONE, COLOR_BLACK, FONT_ATTR_RESERVED0,  0x02,             FONT_DFLT_CHARSET}
};

const format_t CGlobalRecords::m_Default_Formats[GLOBAL_NUM_DEFAULT_FORMATS ] =
{
   { 0x05, "\"$\"#,##0;\\-\"$\"#,##0"  },
   { 0x06, "\"$\"#,##0;[Red]\\-\"$\"#,##0"  },
   { 0x07, "\"$\"#,##0.00;\\-\"$\"#,##0.00"  },
   { 0x08, "\"$\"#,##0.00;[Red]\\-\"$\"#,##0.00"  },
   { 0x2a, "_-\"$\"* #,##0_-;\\-\"$\"* #,##0_-;_-\"$\"* \"-\"_-;_-@_-"  },
   { 0x29, "_-* #,##0_-;\\-* #,##0_-;_-* \"-\"_-;_-@_-"  },
   { 0x2c, "_-\"$\"* #,##0.00_-;\\-\"$\"* #,##0.00_-;_-\"$\"* \"-\"??_-;_-@_-"  },
   { 0x2b, "_-* #,##0.00_-;\\-* #,##0.00_-;_-* \"-\"??_-;_-@_-"  }
};

const xf_init_t CGlobalRecords::m_Default_XFs[GLOBAL_NUM_DEFAULT_XFS] =
{
   {0x0000, 0x0000, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x0001, 0x0001, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x0002, 0x0001, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x0003, 0x0002, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x0004, 0x0002, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x0005, 0x0000, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x0006, 0x0000, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x0007, 0x0000, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x0008, 0x0000, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x0009, 0x0000, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x000a, 0x0000, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x000b, 0x0000, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x000c, 0x0000, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x000d, 0x0000, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x000e, 0x0000, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK},
   {0x000f, 0x0000, 0x0000, HALIGN_GENERAL, VALIGN_BOTTOM, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_CELL , BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK}
};
const style_t  CGlobalRecords::m_Default_Styles [GLOBAL_NUM_DEFAULT_STYLES  ] =
{
   {0x0010, 0x03 },
   {0x0011, 0x06 },
   {0x0012, 0x00 },
   {0x0013, 0x07 },
   {0x0000, 0x04 },
   {0x0014, 0x05 }
};

*/

/*
****************************************
****************************************
*/

CGlobalRecords::CGlobalRecords()
   : m_DumpState(GLOBAL_INIT),
     m_pCurrentData(NULL)
{


   // Initialize default fonts
   font_t* newfont;
   
   newfont = new font_t(0x0000, "Arial", 0x00c8, BOLDNESS_NORMAL, UNDERLINE_NONE, SCRIPT_NONE, COLOR_BLACK, FONT_DFLT_ATTRIBUTES, FONT_DFLT_FAMILY, FONT_DFLT_CHARSET);
   newfont->MarkUsed();
   newfont->MarkUsed();
   m_DefaultFonts.push_back(newfont);

   newfont = new font_t(0x0001, "Arial", 0x00c8, BOLDNESS_NORMAL, UNDERLINE_NONE, SCRIPT_NONE, COLOR_BLACK, FONT_DFLT_ATTRIBUTES, FONT_DFLT_FAMILY, FONT_DFLT_CHARSET);
   newfont->MarkUsed();
   newfont->MarkUsed();
   m_DefaultFonts.push_back(newfont);

   newfont = new font_t(0x0002, "Arial", 0x00c8, BOLDNESS_NORMAL, UNDERLINE_NONE, SCRIPT_NONE, COLOR_BLACK, FONT_DFLT_ATTRIBUTES, FONT_DFLT_FAMILY, FONT_DFLT_CHARSET);
   newfont->MarkUsed();
   newfont->MarkUsed();
   m_DefaultFonts.push_back(newfont);

   newfont = new font_t(0x0003, "Arial", 0x00c8, BOLDNESS_NORMAL, UNDERLINE_NONE, SCRIPT_NONE, COLOR_BLACK, FONT_DFLT_ATTRIBUTES, FONT_DFLT_FAMILY, FONT_DFLT_CHARSET);
   newfont->MarkUsed();
   newfont->MarkUsed();
   m_DefaultFonts.push_back(newfont);
   
   newfont = new font_t(0x0004, "Arial", 0x00c8, BOLDNESS_NORMAL, UNDERLINE_NONE, SCRIPT_NONE, COLOR_BLACK, FONT_ATTR_RESERVED0,  0x02,             FONT_DFLT_CHARSET);
   newfont->MarkUsed();
   newfont->MarkUsed();
   m_DefaultFonts.push_back(newfont);

   // Initialize default formats
/*
  format_t* newformat;
  newformat = new format_t(0x05,"\"$\"#,##0;\\-\"$\"#,##0");
  newformat->MarkUsed();
  m_Formats.push_back(newformat);

  newformat = new format_t(0x06,"\"$\"#,##0;[Red]\\-\"$\"#,##0");
  newformat->MarkUsed();
  m_Formats.push_back(newformat);

  newformat = new format_t(0x07,"\"$\"#,##0.00;\\-\"$\"#,##0.00");
  newformat->MarkUsed();
  m_Formats.push_back(newformat);

  newformat = new format_t(0x08,"\"$\"#,##0.00;[Red]\\-\"$\"#,##0.00");
  newformat->MarkUsed();
  m_Formats.push_back(newformat);

  newformat = new format_t(0x27,"_-\"$\"* #,##0_-;\\-\"$\"* #,##0_-;_-\"$\"* \"-\"_-;_-@_-");
  newformat->MarkUsed();
  m_Formats.push_back(newformat);

  newformat = new format_t(0x29,"_-* #,##0_-;\\-* #,##0_-;_-* \"-\"_-;_-@_-");
  newformat->MarkUsed();
  m_Formats.push_back(newformat);

  newformat = new format_t(0x2c,"_-\"$\"* #,##0.00_-;\\-\"$\"* #,##0.00_-;_-\"$\"* \"-\"??_-;_-@_-");
  newformat->MarkUsed();
  m_Formats.push_back(newformat);

  newformat = new format_t(0x2b,"_-* #,##0.00_-;\\-* #,##0.00_-;_-* \"-\"??_-;_-@_-");
  newformat->MarkUsed();
  m_Formats.push_back(newformat);

*/

   // Initialize default XF records
   xf_t* newxf;
   newxf = new xf_t(0x0000, NULL, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);   

   newxf = new xf_t(0x0001, NULL/*0x0001*/, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);   

   newxf = new xf_t(0x0002, NULL/*0x0001*/, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);   

   newxf = new xf_t(0x0003, NULL/*0x0002*/, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);

   newxf = new xf_t(0x0004, NULL/*0x0002*/, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);

   newxf = new xf_t(0x0005, NULL, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);

   newxf = new xf_t(0x0006, NULL, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);

   newxf = new xf_t(0x0007, NULL, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);

   newxf = new xf_t(0x0008, NULL, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);

   newxf = new xf_t(0x0009, NULL, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);

   newxf = new xf_t(0x000a, NULL, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);

   newxf = new xf_t(0x000b, NULL, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);

   newxf = new xf_t(0x000c, NULL, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);

   newxf = new xf_t(0x000d, NULL, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);

   newxf = new xf_t(0x000e, NULL, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_STYLE, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);

   newxf = new xf_t(0x000f, NULL, FMT_GENERAL, HALIGN_GENERAL, VALIGN_BOTTOM, INDENT_NONE, ORI_NONE, COLOR_BLACK, COLOR_WHITE, FILL_NONE, XF_LOCKED, XF_NO_HIDDEN, XF_NO_WRAPPED, XF_IS_CELL , BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK, BORDER_NONE, COLOR_BLACK);
   newxf->MarkUsed();
   newxf->MarkUsed();
   m_DefaultXFs.push_back(newxf);


   style_t* newstyle;
   newstyle = new style_t;
   newstyle->xfindex = 0x0010;
   newstyle->builtintype = 0x03;
   m_Styles.push_back(newstyle);

   newstyle = new style_t;
   newstyle->xfindex = 0x0011;
   newstyle->builtintype = 0x06;
   m_Styles.push_back(newstyle);

   newstyle = new style_t;
   newstyle->xfindex = 0x0012;
   newstyle->builtintype = 0x00;
   m_Styles.push_back(newstyle);

   newstyle = new style_t;
   newstyle->xfindex = 0x0013;
   newstyle->builtintype = 0x07;
   m_Styles.push_back(newstyle);

   newstyle = new style_t;
   newstyle->xfindex = 0x0000;
   newstyle->builtintype = 0x04;
   m_Styles.push_back(newstyle);

   newstyle = new style_t;
   newstyle->xfindex = 0x0014;
   newstyle->builtintype = 0x05;
   m_Styles.push_back(newstyle);



/*
// Initialize lists with de default values:
int i;

for(i = 0; i<GLOBAL_NUM_DEFAULT_FONT; i++)
{

font_t* fontelement = new font_t(CGlobalRecords::m_Default_Fonts[i]);
m_Fonts.push_back(fontelement);
}

for(i = 0; i<GLOBAL_NUM_DEFAULT_FORMATS; i++)
{
   format_t* formatelement = new format_t;
   *formatelement = CGlobalRecords::m_Default_Formats[i];
   m_Formats.push_back(formatelement);
}

for(i = 0; i<GLOBAL_NUM_DEFAULT_XFS     ; i++)
{
   xf_t * xfelement = new xf_t(CGlobalRecords::m_Default_XFs[i]);
   m_XFs.push_back(xfelement);
}

for(i = 0; i<GLOBAL_NUM_DEFAULT_STYLES  ; i++)
{
   style_t* styleelement = new  style_t;
   *styleelement = CGlobalRecords::m_Default_Styles[i];
   m_Styles.push_back(styleelement);
}

*/
// Initialize former static variables
font = m_Fonts.begin();        
format = m_Formats.begin();    
xf = m_XFs.begin();            
style = m_Styles.begin();      
bsheet = m_BoundSheets.begin();


}
/*
****************************************
****************************************
*/

CGlobalRecords::~CGlobalRecords()
{
   // Delete dinamically created lists elements
   
   if(!m_Fonts.empty())
   {
      for(Font_Set_Itor_t fnt = m_Fonts.begin(); fnt != m_Fonts.end(); fnt++)
         delete *fnt;
      m_Fonts.clear();
   }

   if(!m_DefaultFonts.empty())
   {
      do
      {
	 delete m_DefaultFonts.front();
	 m_DefaultFonts.pop_front();
      
      }while(!m_DefaultFonts.empty());
   }

   if(!m_Formats.empty())
   {
      do
      {
         delete m_Formats.front();
         m_Formats.pop_front();
         
      }while(!m_Formats.empty());
   }



   if(!m_DefaultXFs.empty())   
   {
      do
      {
	 delete m_DefaultXFs.front();
	 m_DefaultXFs.pop_front();
      
      }while(!m_DefaultXFs.empty());
   }

   if(!m_XFs.empty())
   {

      for(XF_Set_Itor_t xf = m_XFs.begin(); xf != m_XFs.end(); xf++)
         delete *xf;
      m_XFs.clear();
   }



   if(!m_Styles.empty())
   {
      do
      {
	 delete m_Styles.front();
	 m_Styles.pop_front();
      
      }while(!m_Styles.empty());
   }

   if(!m_BoundSheets.empty())
   {
      do
      {
	 delete m_BoundSheets.front();
	 m_BoundSheets.pop_front();
      
      }while(!m_BoundSheets.empty());
   }

}



/*
****************************************
****************************************
*/

CUnit* CGlobalRecords::DumpData(bool reset)
{


   if(reset)
   {
      m_DumpState = GLOBAL_INIT;
      //Delete_Pointer(m_pCurrentData);

   }

   XTRACE("CGlobalRecords::DumpData");

   bool repeat = false;

   do
   {
   
      switch(m_DumpState)
      {

         case GLOBAL_INIT:
            font = m_Fonts.begin();        
            font_dflt = m_DefaultFonts.begin();        
            format = m_Formats.begin();    
            xf = m_XFs.begin();            
            xf_dflt = m_DefaultXFs.begin();            
            next_xfindex = 0;
            next_formatindex = 0;
            next_fontindex = 0;
            style = m_Styles.begin();      
            bsheet = m_BoundSheets.begin();

            m_DumpState = GLOBAL_BOF;

            repeat = true;

            break;


         case GLOBAL_BOF:			// ********** STATE 1A *************
            XTRACE("\tBOF");

            repeat = false;

            //MSVC
            // Delete_Pointer(m_pCurrentData);
            m_pCurrentData = (CUnit*)(new CBof(BOF_TYPE_WBGLOBALS));
            m_DumpState = GLOBAL_CODEPAGE; // DFH GLOBAL_WINDOW1;
            break;

         case GLOBAL_CODEPAGE:		// ********** STATE 1B *************
           XTRACE("\tCODEPAGE");
 
			repeat = false;

            //Delete_Pointer(m_pCurrentData);
#if VERSION_BIFF == VERSION_BIFF8
            m_pCurrentData = (CUnit*)(new CCodePage(1200));	// UTF-16
#else
            m_pCurrentData = (CUnit*)(new CCodePage(367));	// ASCII
#endif
            m_DumpState = GLOBAL_WINDOW1;
            break;

         case GLOBAL_WINDOW1:		// ********** STATE 2A *************
            XTRACE("\tWINDOW1");
 
			repeat = false;

            //Delete_Pointer(m_pCurrentData);
            m_pCurrentData = (CUnit*)(new CWindow1());
            m_DumpState = GLOBAL_DATEMODE; // GLOBAL_DEFAULTFONTS;
            break;

         case GLOBAL_DATEMODE:		// ********** STATE 2B *************
            XTRACE("\tDATEMODE");
 
			repeat = false;

            //Delete_Pointer(m_pCurrentData);
            m_pCurrentData = (CUnit*)(new CDateMode());
            m_DumpState = GLOBAL_DEFAULTFONTS;
            break;

         case GLOBAL_DEFAULTFONTS:	// ********** STATE 3A *************
            XTRACE("\tDEFAULTFONTS");
			 {

				repeat = false;
				
				// According documentation, for backwards
				// compatibility, there is no index 4.
				if(next_fontindex==4) next_fontindex++;
				(*font_dflt)->SetIndex(next_fontindex);
				next_fontindex++;
				m_pCurrentData = (CUnit*)(new CFont(*font_dflt));

				if(font_dflt != (--m_DefaultFonts.end()))
				{// if it was'nt the last font from the list, increment to get the next one
				   font_dflt++;
				}
				else
				{// if it was the last from the list, change the DumpState
				   m_DumpState = GLOBAL_FONTS;
				   font_dflt = m_DefaultFonts.begin();
				}

				break;
			 }
         case GLOBAL_FONTS:// ********** STATE 3B *************
			XTRACE("\tFONTS");
			 {

				repeat = false;

				if(!m_Fonts.empty())
				{// First check if the list of fonts is not empty...

				
				   if((*font)->IsUsed())
				   {
					  // According documentation, for backwards
					  // compatibility, there is no index 4.
					  if(next_fontindex==4) next_fontindex++;
					  (*font)->SetIndex(next_fontindex);
					  next_fontindex++;
					  m_pCurrentData = (CUnit*)(new CFont(*font));
				   }
				   else
				   {
					  repeat = true;
				   }


				   if(font != (--m_Fonts.end()))
				   {// if it was'nt the last font from the list, increment to get the next one
					  font++;
				   }
				   else
				   {// if it was the last from the list, change the DumpState
					  m_DumpState = GLOBAL_FORMATS;
					  font = m_Fonts.begin();
				   }
				}
				else
				{// if the list is empty, change the dump state.
				   m_DumpState = GLOBAL_FORMATS;
				   //font = m_Fonts.begin();
				   repeat = true;
				}

				break;
			 }

         case GLOBAL_FORMATS: // ********** STATE 4 *************
         {

            repeat = false;

            XTRACE("\tFORMATS");
            if(!m_Formats.empty())
            {// First check if the list of fonts is not empty...

            
               if(/*(*format)->IsUsed()*/true)
               {
                  if((*format)->GetIndex()!=0)
                     next_formatindex = (*format)->GetIndex();

                  (*format)->SetIndex(next_formatindex);
                  next_formatindex++;
                  m_pCurrentData = (CUnit*)(new CFormat(*format));
               }
               else
               {
                  repeat = true;
               }
                     
               if(format != (--m_Formats.end()))
               {// if it was'nt the last font from the list, increment to get the next one
                  format++;
               }
               else
               {// if it was the last from the list, change the DumpState
                  m_DumpState = GLOBAL_DEFAULTXFS;
                  format = m_Formats.begin();
               }
            }
            else
            {// if the list is empty, change the dump state.
               m_DumpState = GLOBAL_DEFAULTXFS;
               // format = m_Formats.begin();
               repeat = true;
            }

            break;
         }

         case GLOBAL_DEFAULTXFS: // ********** STATE 5a *************
         {

            repeat = false;

            XTRACE("\tXDEFAULTFS");

            (*xf_dflt)->SetIndex(next_xfindex);
            next_xfindex++;

            m_pCurrentData = (CUnit*)(new CExtFormat(*xf_dflt));


            if(xf_dflt != (--m_DefaultXFs.end()))
            {// if it was'nt the last font from the list, increment to get the next one
               xf_dflt++;
            }
            else
            {// if it was the last from the list, change the DumpState
               m_DumpState = GLOBAL_XFS;
               xf_dflt = m_DefaultXFs.begin();
            }

            break;
         }

         case GLOBAL_XFS: // ********** STATE 5b *************
         {

            repeat = false;

            XTRACE("\tXFS");
            if(!m_XFs.empty())
            {// First check if the list of fonts is not empty...

               if((*xf)->IsUsed())
               {
                  (*xf)->SetIndex(next_xfindex);
                  next_xfindex++;

                  m_pCurrentData = (CUnit*)(new CExtFormat(*xf));
               }
               else
               {
                  repeat = true;
               }

               if(xf != (--m_XFs.end()))
               {// if it was'nt the last font from the list, increment to get the next one
                  xf++;
               }
               else
               {// if it was the last from the list, change the DumpState
                  m_DumpState = GLOBAL_STYLES;
                  xf = m_XFs.begin();
               }
            }
            else
            {// if the list is empty, change the dump state.
               m_DumpState = GLOBAL_STYLES;
               //xf = m_XFs.begin();
               repeat = true;
            }

            break;
         }

         case GLOBAL_STYLES: // ********** STATE 6 *************
         {
            XTRACE("\tSTYLES");
            repeat = false;

            if(!m_Styles.empty())
            {// First check if the list of fonts is not empty...

               //Delete_Pointer(m_pCurrentData);
               m_pCurrentData = (CUnit*)(new CStyle(*style));

               if(style != (--m_Styles.end()))
               {// if it was'nt the last font from the list, increment to get the next one
                  style++;
               }
               else
               {// if it was the last from the list, change the DumpState
                  m_DumpState = GLOBAL_BOUNDSHEETS;
                  style = m_Styles.begin();
               }
            }
            else
            {// if the list is empty, change the dump state.
               m_DumpState = GLOBAL_BOUNDSHEETS;
               //style = m_Styles.begin();
               repeat = true;
            }

            break;
         }


         case GLOBAL_BOUNDSHEETS: // ********** STATE 7 *************
         {
            repeat = false;

            XTRACE("\tBOUNDSHEETS");
            if(!m_BoundSheets.empty())
            {// First check if the list of fonts is not empty...

               //Delete_Pointer(m_pCurrentData);
               m_pCurrentData = (CUnit*)(new CBSheet(*bsheet));

               if(bsheet != (--m_BoundSheets.end()))
               {// if it was'nt the last font from the list, increment to get the next one
                  bsheet++;
               }
               else
               {// if it was the last from the list, change the DumpState
                  m_DumpState = GLOBAL_EOF;
                  bsheet = m_BoundSheets.begin();
               }
            }
            else
            {// if the list is empty, change the dump state.
          
               m_DumpState = GLOBAL_EOF;
               bsheet = m_BoundSheets.begin();
               repeat = true;
            }

            break;
         }

         case GLOBAL_EOF:// ********** STATE 8 *************
            XTRACE("\tEOF");

            repeat = false;
            //Delete_Pointer(m_pCurrentData);
            m_pCurrentData = (CUnit*)(new CEof());
            m_DumpState = GLOBAL_FINISH;

            break;

         case GLOBAL_FINISH: // ********** STATE 9 *************

            XTRACE("\tFINISH");
            //Delete_Pointer(m_pCurrentData);
            m_pCurrentData = NULL;
            m_DumpState = GLOBAL_INIT;

            break;

         default:
            /* It shouldn't get here */
            XTRACE("\tDEFAULT");
            break;


      }
   } while(repeat);

   return m_pCurrentData;

}


/*
****************************************
****************************************
*/
void CGlobalRecords::AddBoundingSheet(unsigned32_t streampos,
                                      unsigned16_t attributes,
                                      string& sheetname)
{

   boundsheet_t* bsheetdef = new boundsheet_t;

   bsheetdef->worksheet  = (bool)((attributes & BSHEET_ATTR_WORKSHEET  ) == BSHEET_ATTR_WORKSHEET );
   bsheetdef->ex4macro   = (bool)((attributes & BSHEET_ATTR_EX4MACRO   ) == BSHEET_ATTR_EX4MACRO  );
   bsheetdef->chart      = (bool)((attributes & BSHEET_ATTR_CHART      ) == BSHEET_ATTR_CHART     );
   bsheetdef->vbmodule   = (bool)((attributes & BSHEET_ATTR_VBMODULE   ) == BSHEET_ATTR_VBMODULE  );
   bsheetdef->visible    = (bool)((attributes & BSHEET_ATTR_VISIBLE    ) == BSHEET_ATTR_VISIBLE   );
   bsheetdef->hidden     = (bool)((attributes & BSHEET_ATTR_HIDDEN     ) == BSHEET_ATTR_HIDDEN    );
   bsheetdef->veryhidden = (bool)((attributes & BSHEET_ATTR_VERYHIDDEN ) == BSHEET_ATTR_VERYHIDDEN);

   bsheetdef->asheetname = sheetname;
   bsheetdef->streampos  = streampos;

   m_BoundSheets.push_back(bsheetdef);

}
#if VERSION_BIFF == VERSION_BIFF8
void CGlobalRecords::AddBoundingSheet(unsigned32_t streampos,
                                      unsigned16_t attributes,
                                      ustring& sheetname
#ifdef HAVE_ICONV
									  , string& iconv_code
#endif
									  )
{
   boundsheet_t* bsheetdef = new boundsheet_t;

   bsheetdef->worksheet  = (bool)((attributes & BSHEET_ATTR_WORKSHEET  ) == BSHEET_ATTR_WORKSHEET );
   bsheetdef->ex4macro   = (bool)((attributes & BSHEET_ATTR_EX4MACRO   ) == BSHEET_ATTR_EX4MACRO  );
   bsheetdef->chart      = (bool)((attributes & BSHEET_ATTR_CHART      ) == BSHEET_ATTR_CHART     );
   bsheetdef->vbmodule   = (bool)((attributes & BSHEET_ATTR_VBMODULE   ) == BSHEET_ATTR_VBMODULE  );
   bsheetdef->visible    = (bool)((attributes & BSHEET_ATTR_VISIBLE    ) == BSHEET_ATTR_VISIBLE   );
   bsheetdef->hidden     = (bool)((attributes & BSHEET_ATTR_HIDDEN     ) == BSHEET_ATTR_HIDDEN    );
   bsheetdef->veryhidden = (bool)((attributes & BSHEET_ATTR_VERYHIDDEN ) == BSHEET_ATTR_VERYHIDDEN);

   bsheetdef->usheetname = sheetname;
#ifdef HAVE_ICONV
   bsheetdef->iconv_code = iconv_code;
#endif
   bsheetdef->streampos  = streampos;

   m_BoundSheets.push_back(bsheetdef);

}
#endif

/*
****************************************
****************************************
*/
void CGlobalRecords::AddBoundingSheet(boundsheet_t* bsheetdef)
{
   m_BoundSheets.push_back(bsheetdef);
}


/*
****************************************
It returns pointers to BoundingSheets one by one until
all are spanned, in which case the returned pointer is NULL
****************************************
*/
void CGlobalRecords::GetBoundingSheets(Boundsheet_List_Itor_t& bs)
{
   if(bs != m_BoundSheets.end())
      bs++;
   else
      bs = m_BoundSheets.begin();
}


Boundsheet_List_Itor_t CGlobalRecords::GetFirstBoundSheet()
{
   return m_BoundSheets.begin();
}

Boundsheet_List_Itor_t CGlobalRecords::GetEndBoundSheet()
{
   return m_BoundSheets.end();
}

/*
****************************************
****************************************
*/

font_t* CGlobalRecords::AddFont(font_t* newfont )
{

   newfont->SetIndex(0x0000);
   pair<Font_Set_Itor_t, bool> itorpair = m_Fonts.insert(newfont);

   if(newfont != *itorpair.first)
      delete newfont;

   return(*itorpair.first);

}

/*
****************************************
****************************************
*/

format_t* CGlobalRecords::AddFormat(format_t* newformat )
{

   newformat->SetIndex(0x0000);
   m_Formats.push_back(newformat);
   return(m_Formats.back());

}


/*
****************************************
****************************************
*/

xf_t* CGlobalRecords::AddXFormat(xf_t* xf)
{
   xf->SetIndex(0x0000);
   pair<XF_Set_Itor_t, bool> itorpair = m_XFs.insert(xf);
   
   if(xf != *itorpair.first)
      delete xf;

   return(*itorpair.first);
}

xf_t* CGlobalRecords::ReplaceXFormat(xf_t* xf, xf_t* xfnew)
{
   XF_Set_Itor_t existing_xf;
   existing_xf = m_XFs.find(xf);

   xf_t* inserted = NULL;
   if(existing_xf != m_XFs.end())
   {
      xf_t*  todiexf = *existing_xf;
      m_XFs.erase(existing_xf);
      delete todiexf;
      pair<XF_Set_Itor_t, bool> itorpair = m_XFs.insert(xfnew);

      inserted = *itorpair.first;
      if(inserted != xfnew)
         delete xfnew;
   }
   else
   {
      // This should never happen
      // since the previous pointer is taken
      // from the container itself... but anyway
      pair<XF_Set_Itor_t, bool> itorpair = m_XFs.insert(xfnew);
      
      inserted = *itorpair.first;
      if(inserted != xfnew)
         delete xfnew;
   }
   return inserted;
}

font_t* CGlobalRecords::ReplaceFont(font_t* fnt, font_t* fntnew)
{
   Font_Set_Itor_t existing_font;
   existing_font = m_Fonts.find(fnt);
      
   font_t *inserted = NULL;
   if(existing_font != m_Fonts.end())
   {
      font_t* todiefont = *existing_font;
      m_Fonts.erase(existing_font);
      delete todiefont;

      pair<Font_Set_Itor_t, bool> itorpair = m_Fonts.insert(fntnew);

      inserted = *itorpair.first;
      if(inserted != fntnew)
         delete fntnew;

   }
   else
   {
      // This should never happen
      // since the previous pointer is taken
      // from the container itself... but anyway
      pair<Font_Set_Itor_t, bool> itorpair = m_Fonts.insert(fntnew);
   
      inserted = *itorpair.first;
      if(inserted != fntnew)
         delete fntnew;

   }
   return inserted;

}


font_t* CGlobalRecords::GetDefaultFont(void)
{
   return *m_DefaultFonts.begin();

}

xf_t* CGlobalRecords::GetDefaultXF(void)
{
   return m_DefaultXFs.back();

}

/*
****************************************
Maybe the implementation of this procedure
can be optimized.
****************************************
*/

unsigned32_t CGlobalRecords::GetSize(void)
{

   CUnit* pdata = NULL;
   unsigned32_t total_datasize = 0;
   bool keep = true;
   do
   {

      pdata = DumpData();
      if(pdata != NULL)
      {
         total_datasize += pdata->GetDataSize();
         delete pdata;
      }
      else
      {
         keep = false;
      }
      
   }while(keep);

   return total_datasize;


} 

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: globalrec.cpp,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:56  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

