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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/extformat.cpp,v $
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

#include <XLS/extformat.h>
#include <XLS/crc.h>

/* 
**********************************
CExtFormat class implementation
**********************************
*/

using namespace std;
using namespace xlslib_core;


CExtFormat::CExtFormat(bool is_cell)
{
   SetRecordType(RECTYPE_XF);
   InitDummy(is_cell);
   SetRecordLength(GetDataSize()-4);
}

CExtFormat::CExtFormat(xf_t* xfdef)
{
   SetRecordType(RECTYPE_XF);
   InitDummy(xfdef->IsCell());
   SetRecordLength(GetDataSize()-4);

// Here initialize the record with the values from the input structure
      
   SetFontIndex(xfdef->GetFontIndex());
   SetFormatIndex(xfdef->GetFormatIndex());
   SetHorizAlign(xfdef->GetHAlign());
   SetVertAlign(xfdef->GetVAlign());
   SetIndent(xfdef->GetIndent());
   SetTxtOrientation(xfdef->GetTxtOrientation());

   SetFGColorIndex(xfdef->GetFillFGColor());
   SetBGColorIndex(xfdef->GetFillBGColor());
   SetFillPattern(xfdef->GetFillStyle());

   if(xfdef->IsLocked()) SetLocked();
   if(xfdef->IsHidden()) SetHidden(); 
   if(xfdef->IsWrap()) SetWrap(); 

   SetBorder(BORDER_BOTTOM, xfdef->GetBorderStyle(BORDER_BOTTOM), 
             xfdef->GetBorderColor(BORDER_BOTTOM));
   SetBorder(BORDER_TOP, xfdef->GetBorderStyle(BORDER_TOP), 
             xfdef->GetBorderColor(BORDER_TOP));
   SetBorder(BORDER_LEFT, xfdef->GetBorderStyle(BORDER_LEFT), 
             xfdef->GetBorderColor(BORDER_LEFT));
   SetBorder(BORDER_RIGHT, xfdef->GetBorderStyle(BORDER_RIGHT), 
             xfdef->GetBorderColor(BORDER_RIGHT));
}

CExtFormat::~CExtFormat()
{
}


/* 
**********************************
**********************************
*/
void CExtFormat::InitDummy(bool is_cell)
{

   // An style-XF record is set by default.
   // Each field has to be modified individually before use it

   //The default style is a dummy. The flags that indicate what the style affects (byte 11)
   // are disabled (set to 1).
   unsigned8_t xfdefault[] = {

#if VERSION_BIFF == VERSION_BIFF5
/*
            Open Office offsets
      0         2         4         6         8         10        12        14         16       18        20
      0x00,0x00,0x00,0x00,0xf4,0xff,0x20,0xf0,0xc0,0x00,0x01,0x00,0x00,0x00,0x00,0x00 <- ORIGINAL
*/
      0x00,0x00,0x00,0x00,0xf4,0xff,0x20,0xf0,0xc0,0x20,0x01,0x00,0x00,0x00,0x00,0x00,
						  // STYLE_XF | INDEX=0xFFF -> Style
									// HALIGN -> General, VALIGN -> BOTTOM
										 // IGNORE TOP 4 bits (style issue)
											  // 0x40 Pattern Color, then lowest bit of 0x41 (next)
												   // 0x41 Pattern background color right shifted one bit
														// Fill Pattern -> 1 Black (well, fully colored, not a pattern)
														
#else
      0x00,0x00,0x00,0x00,0xf4,0xff,0x20,0x00,0x00,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0xc0,0x20
						  // STYLE_XF | INDEX=0xFFF -> Style
									// HALIGN -> General, VALIGN -> BOTTOM
												  // IGNORE TOP 4 bits (style issue)
																						   // Fill Pattern -> 1 Black, left shifted 2 bits
																								// 0x40 Pattern Color, then lowest bit of 0x41 (next)
																									 // 0x41 Pattern background color right shifted one bit
#endif

   };

   AddDataArray(xfdefault, sizeof(xfdefault));
    
   if(is_cell)
   {
      unsigned16_t value;

      // Get the field where options are set
      GetValue16From((signed16_t*)&value, XF_OFFSET_PROP);
      // Set the cell's style parent to Normal
      value &= (~XF_PROP_XFPARENT); 
      // Set the style = cell.
      value &= (~XF_PROP_STYLE); 

      SetValueAt((signed16_t)value, XF_OFFSET_PROP);

      // The cell doesn't heritage anything from its parent style 
      GetValue16From((signed16_t*)&value, XF_OFFSET_ALIGN);

      value &= (~(XF_ALIGN_ATRFONT|XF_ALIGN_ATRALC|XF_ALIGN_ATRBDR|XF_ALIGN_ATRPAT|XF_ALIGN_ATRPROT)); 
      SetValueAt((signed16_t)value, XF_OFFSET_ALIGN);
   }
}

/* 
**********************************
**********************************
*/
bool CExtFormat::IsCell()
{
   unsigned16_t val;
   GetValue16From((signed16_t*)&val, XF_OFFSET_PROP);

   
   return(val&XF_PROP_STYLE?true:false);

}

/* 
**********************************
**********************************
*/
void CExtFormat::SetFlag(unsigned16_t flag)
{

   unsigned16_t value;
   GetValue16From((signed16_t*)&value, XF_OFFSET_ALIGN);

   if(IsCell())
   {
      // Cells indicate that a characteristic is not equal
      //  from its parent with the flag set.
      value |= flag; 
   }
   else
   {
      // Styles indicate that a characteristic is 
      // being implemented with the flag cleared.
      value &= (~flag);
   }
   SetValueAt((signed16_t)value, XF_OFFSET_ALIGN);

}

/* 
**********************************
**********************************
*/
void CExtFormat::ClearFlag(unsigned16_t flag)
{

   unsigned16_t value;
   GetValue16From((signed16_t*)&value, XF_OFFSET_ALIGN);

   if(!IsCell())
   {
      // Cells indicate that a characteristic is not equal
      //  from its parent with the flag set.
      value |= flag; 
   }
   else
   {
      // Styles indicate that a characteristic is 
      // being implemented with the flag cleared.
      value &= (~flag);
   }
   SetValueAt((signed16_t)value, XF_OFFSET_ALIGN);

}
/* 
**********************************
**********************************
*/
int CExtFormat::SetFontIndex(unsigned16_t fontindex)
{
   // Set the index value
   int errcode = SetValueAt((signed16_t)fontindex, XF_OFFSET_FONT);
   
   // Set the related flag.
   SetFlag(XF_ALIGN_ATRFONT);
   
   return errcode;
}

/* 
**********************************
**********************************
*/
unsigned16_t CExtFormat::GetFontIndex(void)
{
   unsigned16_t fontval;
   GetValue16From((signed16_t*)&fontval, XF_OFFSET_FONT);

   return(fontval);
}

/* 
**********************************
**********************************
*/
int CExtFormat::SetFormatIndex(unsigned16_t formatindex)
{
   // Set the index value
   int errcode = SetValueAt((signed16_t)formatindex, XF_OFFSET_FORMAT);
   
   // Set the related flag.
   SetFlag(XF_ALIGN_ATRNUM);
   
   return errcode;
}

/* 
**********************************
**********************************
*/
unsigned16_t CExtFormat::GetFormatIndex(void)
{
   unsigned16_t formatval;
   GetValue16From((signed16_t*)&formatval, XF_OFFSET_FORMAT);

   return(formatval);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetLocked()
{
   unsigned16_t value;
   GetValue16From((signed16_t*)&value, XF_OFFSET_PROP);

   value |= XF_PROP_LOCKED;

   SetValueAt((signed16_t)value, XF_OFFSET_PROP);

   SetFlag(XF_ALIGN_ATRPROT);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetHidden()
{
   unsigned16_t value;
   GetValue16From((signed16_t*)&value, XF_OFFSET_PROP);

   value |= XF_PROP_HIDDEN;

   SetValueAt((signed16_t)value, XF_OFFSET_PROP);

   SetFlag(XF_ALIGN_ATRPROT);
}


/* 
**********************************
**********************************
*/
void CExtFormat::SetHorizAlign(unsigned8_t alignval)
{
   unsigned16_t value;
   GetValue16From((signed16_t*)&value, XF_OFFSET_ALIGN);
   value = (value&(~XF_ALIGN_HORIZONTAL))|(alignval & XF_ALIGN_HORIZONTAL);
   SetValueAt((signed16_t)value, XF_OFFSET_ALIGN);

   SetFlag(XF_ALIGN_ATRALC);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetWrap()
{
   unsigned16_t value;
   GetValue16From((signed16_t*)&value, XF_OFFSET_ALIGN);

   value |= XF_ALIGN_WRAP;

   SetValueAt((signed16_t)value, XF_OFFSET_ALIGN);

   SetFlag(XF_ALIGN_ATRALC);
}
#if VERSION_BIFF == VERSION_BIFF8
/* 
**********************************
**********************************
*/

void CExtFormat::SetIndent(unsigned8_t indentval)
{
	unsigned32_t value, mask;
   

	if(indentval & XF_INDENT_LVL) {
		mask = XF_INDENT_LVL;
	} else
	if(indentval & XF_INDENT_SHRINK2FIT) {
		mask = XF_INDENT_SHRINK2FIT;
	} else
	if(indentval & (XF_INDENT_CONTEXT|XF_INDENT_L2R|XF_INDENT_R2L)) {
		mask = XF_INDENT_DIR; 
	} else {
		mask = XF_INDENT_LVL | XF_INDENT_SHRINK2FIT | XF_INDENT_DIR;
	}
		
	mask <<= XF_INDENT_SHIFTPOS;
   
   GetValue32From((signed32_t*)&value, XF_OFFSET_ALIGN);

   unsigned32_t indentval32 = (unsigned32_t)indentval << XF_INDENT_SHIFTPOS; // Place the option at the right bit position
   value = (value&(~mask))|(indentval32 & mask);

   SetValueAt((signed32_t)value, XF_OFFSET_ALIGN);
}
#endif
/* 
**********************************
**********************************
*/
void CExtFormat::SetVertAlign(unsigned8_t alignval)
{
   unsigned16_t value;
   GetValue16From((signed16_t*)&value, XF_OFFSET_ALIGN);
   unsigned16_t alignval16 = alignval << XF_ALIGN_SHIFTPOS_VALIGN; // Place the option at the right bit position
   value = (value&(~XF_ALIGN_VERTICAL))|(alignval16 & XF_ALIGN_VERTICAL);
   SetValueAt((signed16_t)value, XF_OFFSET_ALIGN);

   SetFlag(XF_ALIGN_ATRALC);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetTxtOrientation(unsigned8_t alignval)
{
   unsigned16_t value;
   GetValue16From((signed16_t*)&value, XF_OFFSET_ALIGN);
   unsigned16_t alignval16 = alignval << XF_ORI_SHIFTPOS; // Place the option at the right bit position

   value = (value&(~XF_ALIGN_ORIENTATION))|(alignval16 & XF_ALIGN_ORIENTATION);
   SetValueAt((signed16_t)value, XF_OFFSET_ALIGN);
}


/* 
**********************************
**********************************
*/
void CExtFormat::SetFGColorIndex(unsigned8_t color)
{
   unsigned16_t value;
   GetValue16From((signed16_t*)&value, XF_OFFSET_COLOR);
   

   // value = (value&(~XF_COLOR_FOREGROUND))|(color & XF_COLOR_FOREGROUND);
   // Clear the field for Foreground color
   value &= (~XF_COLOR_FOREGROUND);
   // Set the new color
   value |= (color & XF_COLOR_FOREGROUND);

   SetValueAt((signed16_t)value, XF_OFFSET_COLOR);

   SetFlag(XF_ALIGN_ATRPAT);

}

/* 
**********************************
**********************************
*/
void CExtFormat::SetBGColorIndex(unsigned8_t color)
{
   unsigned16_t value;
   GetValue16From((signed16_t*)&value, XF_OFFSET_COLOR);
   unsigned16_t color16 = color <<  XF_COLOR_SHIFTPOS_BG;

//   value = (value&(~XF_COLOR_FOREGROUND))|(color16 & XF_COLOR_FOREGROUND);
   // Clear the field for Foreground color
   value &= (~XF_COLOR_BACKGROUND);
   // Set the new color
   value |= (color16 & XF_COLOR_BACKGROUND);


   SetValueAt((signed16_t)value, XF_OFFSET_COLOR);

   SetFlag(XF_ALIGN_ATRPAT);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetFillPattern(unsigned8_t pattern)
{
#if VERSION_BIFF == VERSION_BIFF5
   unsigned16_t value;
   GetValue16From((signed16_t*)&value, XF_OFFSET_BORDER0);

   value = (value&(~XF_BORDER_FILLPATTERN))|(pattern & XF_BORDER_FILLPATTERN);
   SetValueAt((signed16_t)value, XF_OFFSET_BORDER0);

   SetFlag(XF_ALIGN_ATRPAT);
#else
   unsigned32_t value, pattern32 = pattern;
   GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERB);

   value &= ~XF_BORDER_FILLPATTERN;
   
   pattern32 <<= XF_SHIFTPOS_FILLPATTERN;
   value |= (pattern32 & XF_BORDER_FILLPATTERN);
   SetValueAt((signed16_t)value, XF_OFFSET_BORDERB);

   SetFlag(XF_ALIGN_ATRPAT);
#endif
}


/* 
**********************************
**********************************
*/
void CExtFormat::SetBorder(border_side_t border, unsigned16_t style, unsigned16_t color)
{
   
   switch(border)
   {

      case BORDER_BOTTOM:
      {
#if VERSION_BIFF == VERSION_BIFF5
         unsigned16_t value, color16 = color;

         GetValue16From((signed16_t*)&value, XF_OFFSET_BORDER0);
         value = (value&(~XF_BORDER_BOTTOMSTYLE))|
            ((style<<XF_STYLE_SHIFTPOS_BOTTOM)
             & XF_BORDER_BOTTOMSTYLE);

         value &= (~XF_BORDER_BOTTOMCOLOR);
         color16 <<= XF_COLOR_SHIFTPOS_BOTTOM;
         value  |= (color16 & XF_BORDER_BOTTOMCOLOR);
    
         SetValueAt((signed16_t)value, XF_OFFSET_BORDER0);
#else
         signed32_t value, color32 = color, style32 = style;

         GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERA);
         value &= (~XF_BORDER_BOTTOMSTYLE);
         style32 <<= XF_STYLE_SHIFTPOS_BOTTOM;
         value |= (style32 & XF_BORDER_BOTTOMSTYLE);
         SetValueAt((signed32_t)value, XF_OFFSET_BORDERA);

         GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERB);
         value &= (~XF_BORDER_BOTTOMCOLOR);
         color32 <<= XF_COLOR_SHIFTPOS_BOTTOM;
         value |= (color32 & XF_BORDER_BOTTOMCOLOR);
         SetValueAt((signed32_t)value, XF_OFFSET_BORDERB);
#endif
         break;
      }
      case BORDER_TOP:
      {
#if VERSION_BIFF == VERSION_BIFF5
         unsigned16_t value, color16 = color;
    
         GetValue16From((signed16_t*)&value, XF_OFFSET_BORDER1);
         value = (value&(~XF_BORDER_TOPSTYLE))|
            ((style<<XF_STYLE_SHIFTPOS_TOP) & XF_BORDER_TOPSTYLE);

         value &= (~XF_BORDER_TOPCOLOR);
         color16 <<= XF_COLOR_SHIFTPOS_TOP;
         value |= (color16  & XF_BORDER_TOPCOLOR);

         SetValueAt((signed16_t)value, XF_OFFSET_BORDER1);
#else
         signed32_t value, color32 = color, style32 = style;

         GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERA);
         value &= (~XF_BORDER_TOPSTYLE);
         style32 <<= XF_STYLE_SHIFTPOS_TOP;
         value |= (style32 & XF_BORDER_TOPSTYLE);
         SetValueAt((signed32_t)value, XF_OFFSET_BORDERA);

         GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERB);
         value &= (~XF_BORDER_TOPCOLOR);
         color32 <<= XF_COLOR_SHIFTPOS_TOP;
         value |= (color32 & XF_BORDER_TOPCOLOR);
         SetValueAt((signed32_t)value, XF_OFFSET_BORDERB);

#endif
         break;
      }
      case BORDER_LEFT:   
      {
#if VERSION_BIFF == VERSION_BIFF5
		 unsigned16_t value;
         unsigned16_t color16 = color;
         unsigned16_t style16 = style;

         GetValue16From((signed16_t*)&value, XF_OFFSET_BORDER1);
         value &= (~XF_BORDER_LEFTSTYLE);
         style16 <<= XF_STYLE_SHIFTPOS_LEFT;
         value |= (style16 & XF_BORDER_LEFTSTYLE);
         SetValueAt((signed16_t)value, XF_OFFSET_BORDER1);

         GetValue16From((signed16_t*)&value, XF_OFFSET_BORDER2);
         color16 <<= XF_COLOR_SHIFTPOS_LEFT;
         value &= (~XF_BORDER_LEFTCOLOR);
         value |= (color16  & XF_BORDER_LEFTCOLOR);

         SetValueAt((signed16_t)value, XF_OFFSET_BORDER2);
#else
         signed32_t value, color32 = color, style32 = style;
    
         GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERA);
         value &= ~(XF_BORDER_LEFTSTYLE|~XF_BORDER_LEFTCOLOR);
		
         color32 <<= XF_COLOR_SHIFTPOS_LEFT;
         style32 <<= XF_STYLE_SHIFTPOS_LEFT;
         value |= (color32 & XF_BORDER_LEFTCOLOR) | (style32 & XF_BORDER_LEFTSTYLE);

         SetValueAt((signed32_t)value, XF_OFFSET_BORDERA);
#endif
         break;
      }
      case BORDER_RIGHT:  
      {
#if VERSION_BIFF == VERSION_BIFF5
		 unsigned16_t value;
         unsigned16_t color16 = color;
         unsigned16_t style16 = style;

         GetValue16From((signed16_t*)&value, XF_OFFSET_BORDER1);
         value &= (~XF_BORDER_RIGHTSTYLE);
         style16 <<= XF_STYLE_SHIFTPOS_RIGHT;
         value |= (style16 & XF_BORDER_RIGHTSTYLE);
         SetValueAt((signed16_t)value, XF_OFFSET_BORDER1);

         GetValue16From((signed16_t*)&value, XF_OFFSET_BORDER2);
         color16 <<= XF_COLOR_SHIFTPOS_RIGHT;
         value &= (~XF_BORDER_RIGHTCOLOR);
         value |= (color16 & XF_BORDER_RIGHTCOLOR);

         SetValueAt((signed16_t)value, XF_OFFSET_BORDER2);
#else
         signed32_t value, color32 = color, style32 = style;
    
         GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERA);
         value &= ~(XF_BORDER_RIGHTSTYLE|~XF_BORDER_RIGHTCOLOR);
		
         color32 <<= XF_COLOR_SHIFTPOS_RIGHT;
         style32 <<= XF_STYLE_SHIFTPOS_RIGHT;
         value |= (color32 & XF_BORDER_RIGHTCOLOR) | (style32 & XF_BORDER_RIGHTSTYLE);

         SetValueAt((signed32_t)value, XF_OFFSET_BORDERA);

#endif
         break;
      }

      default:
         break;
   }

   SetFlag(XF_ALIGN_ATRBDR);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetXFParent(unsigned16_t parent)
{
   
   if(IsCell())
   {
      unsigned8_t value;
      
      // Set the cell's style parent to Normal
      GetValue16From((signed16_t*)&value, XF_OFFSET_PROP);
      value = (value&(~XF_PROP_XFPARENT))|
         ((parent<<XF_PROP_SHIFTPOS_PARENT) & XF_ALIGN_HORIZONTAL);
      SetValueAt((signed16_t)value, XF_OFFSET_PROP);
       

   }
   else
   {
      /*Do nothing: Styles don't have parent... but still clear the flags...*/
   }


   ClearFlag(XF_ALIGN_ATRFONT|XF_ALIGN_ATRALC|XF_ALIGN_ATRBDR|
             XF_ALIGN_ATRPAT|XF_ALIGN_ATRPROT);

}


/* 
**********************************
xf_t class implementation
**********************************
*/

/* 
**********************************
**********************************
*/

const unsigned8_t xf_t::HALIGN_OPTIONS_TABLE[] = 
{
   XF_HALIGN_GENERAL      ,
   XF_HALIGN_LEFT         ,
   XF_HALIGN_CENTER       ,
   XF_HALIGN_RIGHT        ,
   XF_HALIGN_FILL         ,
   XF_HALIGN_JUSTIFY      ,
   XF_HALIGN_CENTERACCROSS
};


/* 
**********************************
**********************************
*/
const unsigned8_t xf_t::VALIGN_OPTIONS_TABLE[] = 
{
   XF_VALIGN_TOP    ,
   XF_VALIGN_CENTER ,
   XF_VALIGN_BOTTOM ,
   XF_VALIGN_JUSTIFY
};

/* 
**********************************
**********************************
*/

const unsigned8_t xf_t::INDENT_OPTIONS_TABLE[] = 
{
	XF_INDENT_NONE,
	XF_INDENT_1,
	XF_INDENT_2,
	XF_INDENT_3,
	XF_INDENT_4,
	XF_INDENT_SHRINK2FIT,
	XF_INDENT_L2R,
	XF_INDENT_R2L
};


/* 
**********************************
**********************************
*/
const unsigned8_t xf_t::TXTORI_OPTIONS_TABLE[] = 
{
   XF_ORI_NONE        ,
   XF_ORI_TOPBOTTOMTXT,
   XF_ORI_90NOCLOCKTXT,
   XF_ORI_90CLOCKTXT  
};

/* 
**********************************
**********************************
*/
const unsigned8_t xf_t::COLOR_OPTIONS_TABLE[] =
{
   XF_COLOR_CODE_BLACK,
   XF_COLOR_CODE_DARK_RED,
   XF_COLOR_CODE_RED,
   XF_COLOR_CODE_FUCSIA,
   XF_COLOR_CODE_COMBINED01,
   XF_COLOR_CODE_COMBINED02,
   XF_COLOR_CODE_COMBINED03,
   XF_COLOR_CODE_COMBINED04,
   XF_COLOR_CODE_COMBINED05,
   XF_COLOR_CODE_COMBINED06,
   XF_COLOR_CODE_OLIVE,
   XF_COLOR_CODE_DARK_YELLOW,
   XF_COLOR_CODE_COMBINED07,
   XF_COLOR_CODE_YELLOW,
   XF_COLOR_CODE_LIGHT_YELLOW,
   XF_COLOR_CODE_DARK_GREEN,
   XF_COLOR_CODE_GREEN,
   XF_COLOR_CODE_COMBINED08,
   XF_COLOR_CODE_LIVING_GREEN,
   XF_COLOR_CODE_LIGHT_GREEN,
   XF_COLOR_CODE_COMBINED09,
   XF_COLOR_CODE_BLUE_GREEN,
   XF_COLOR_CODE_AQUAMARINA,
   XF_COLOR_CODE_TURQOISE,
   XF_COLOR_CODE_COMBINED10,
   XF_COLOR_CODE_DARK_BLUE,
   XF_COLOR_CODE_BLUE,
   XF_COLOR_CODE_LIGHT_BLUE,
   XF_COLOR_CODE_SKY_BLUE,
   XF_COLOR_CODE_COMBINED11,
   XF_COLOR_CODE_INDIGO,
   XF_COLOR_CODE_BLUE_GRAY,
   XF_COLOR_CODE_VIOLET,
   XF_COLOR_CODE_PLUM,
   XF_COLOR_CODE_LAVANDER,
   XF_COLOR_CODE_COMBINED12,
   XF_COLOR_CODE_GRAY50,
   XF_COLOR_CODE_GRAY40,
   XF_COLOR_CODE_GRAY25,
   XF_COLOR_CODE_WHITE        
};

/* 
**********************************
**********************************
*/
const unsigned8_t xf_t::FILL_OPTIONS_TABLE[] =
{
   XF_FILL_NONE,
   XF_FILL_SOLID,
   XF_FILL_ATEN75,
   XF_FILL_ATEN50,
   XF_FILL_ATEN25,
   XF_FILL_ATEN12,
   XF_FILL_ATEN06,
   XF_FILL_HORIZ_LIN,
   XF_FILL_VERTICAL_LIN,
   XF_FILL_DIAG,
   XF_FILL_INV_DIAG,
   XF_FILL_INTER_DIAG,
   XF_FILL_DIAG_THICK_INTER,
   XF_FILL_HORIZ_LINES_THIN,
   XF_FILL_VERTICAL_LINES_THIN,
   XF_FILL_DIAG_THIN,
   XF_FILL_INV_DIAG_THIN,
   XF_FILL_HORIZ_INT_THIN,
   XF_FILL_HORIZ_INTER_THICK   
};


/* 
**********************************
**********************************
*/
const unsigned8_t xf_t::BORDERSTYLE_OPTIONS_TABLE[] =
{
   XF_BRDOPTION_NONE,
   XF_BRDOPTION_THIN,
   XF_BRDOPTION_MEDIUM,
   XF_BRDOPTION_DASHED,
   XF_BRDOPTION_DOTTED,
   XF_BRDOPTION_THICK,
   XF_BRDOPTION_DOUBLE,
   XF_BRDOPTION_HAIR
};


const unsigned16_t xf_t::FORMAT_NUM_OPTIONS_TABLE[] =
{
   FMTCODE_GENERAL,
   FMTCODE_NUMBER1,
   FMTCODE_NUMBER2,
   FMTCODE_NUMBER3,
   FMTCODE_NUMBER4,
   FMTCODE_CURRENCY1,
   FMTCODE_CURRENCY2,
   FMTCODE_CURRENCY3,
   FMTCODE_CURRENCY4,
   FMTCODE_PERCENT1,
   FMTCODE_PERCENT2,
   FMTCODE_SCIENTIFIC1,
   FMTCODE_FRACTION1,
   FMTCODE_FRACTION2,
   FMTCODE_DATE1,
   FMTCODE_DATE2,
   FMTCODE_DATE3,
   FMTCODE_DATE4,
   FMTCODE_HOUR1,
   FMTCODE_HOUR2,
   FMTCODE_HOUR3,
   FMTCODE_HOUR4,
   FMTCODE_HOURDATE,
   FMTCODE_NUMBER5,
   FMTCODE_NUMBER6,
   FMTCODE_NUMBER7,
   FMTCODE_NUMBER8,
   FMTCODE_ACCOUNTING1,
   FMTCODE_ACCOUNTING2,
   FMTCODE_ACCOUNTING3,
   FMTCODE_ACCOUNTING4,
   FMTCODE_HOUR5,
   FMTCODE_HOUR6,
   FMTCODE_HOUR7,
   FMTCODE_SCIENTIFIC2,
   FMTCODE_TEXT
};
/* 
**********************************
**********************************
*/
xf_t::xf_t() 
   : m_usage_counter(0) 
{
   // Set a default value  
   SetIndex(0x0000);
   SetFont(NULL);
   SetFormat(FMT_GENERAL);
   SetHAlign(HALIGN_GENERAL);
   SetVAlign(VALIGN_BOTTOM);
   SetIndent(INDENT_NONE);
   SetTxtOrientation(ORI_NONE);
   SetFillFGColor(COLOR_BLACK);
   SetFillBGColor(COLOR_WHITE);
   SetFillStyle(FILL_NONE);
   SetLocked(XF_LOCKED);
   SetHidden(XF_NO_HIDDEN);
   SetWrap(XF_NO_WRAPPED);
   SetCellMode(XF_IS_CELL);
   SetBorderStyle(BORDER_BOTTOM ,BORDER_NONE, COLOR_BLACK);
   SetBorderStyle(BORDER_TOP    ,BORDER_NONE, COLOR_BLACK);
   SetBorderStyle(BORDER_LEFT   ,BORDER_NONE, COLOR_BLACK);
   SetBorderStyle(BORDER_RIGHT  ,BORDER_NONE, COLOR_BLACK);

}
/* 
**********************************
**********************************
*/
xf_t::xf_t(const xf_init_t& xfinit)  
   : m_usage_counter(0)
{
    
   SetIndex(xfinit.index);
   SetFont(xfinit.font);
   SetFormat(xfinit.format);
   SetHAlign(xfinit.halign);
   SetVAlign(xfinit.valign);
   SetIndent(xfinit.indent);
   SetTxtOrientation(xfinit.txtorientation);
   SetFillFGColor(xfinit.fill_fgcolor);
   SetFillBGColor(xfinit.fill_bgcolor);
   SetFillStyle(xfinit.fillstyle);
   SetLocked(xfinit.locked);
   SetHidden(xfinit.hidden);
   SetWrap(xfinit.wrap);
   SetCellMode(xfinit.is_cell);
   SetBorderStyle(BORDER_BOTTOM,xfinit.bottom_border_style, xfinit.bottom_border_color);
   SetBorderStyle(BORDER_TOP,xfinit.top_border_style, xfinit.top_border_color);
   SetBorderStyle(BORDER_LEFT,xfinit.left_border_style, xfinit.left_border_color);
   SetBorderStyle(BORDER_RIGHT,xfinit.right_border_style, xfinit.right_border_color);

}

/* 
**********************************
**********************************
*/
xf_t::xf_t(unsigned16_t index, font_t* font, format_number_t format, halign_option_t halign,
           valign_option_t valign, indent_option_t indent, txtori_option_t txtorientation, color_name_t fill_fgcolor,
           color_name_t fill_bgcolor, fill_option_t fillstyle, bool locked, bool hidden,
           bool wrap, bool is_cell, border_style_t top_border_style, color_name_t top_border_color,
           border_style_t bottom_border_style, color_name_t bottom_border_color, border_style_t left_border_style,
           color_name_t  left_border_color, border_style_t right_border_style, color_name_t right_border_color)
   :m_usage_counter(0)
{
   SetIndex(index);

   SetFont(font);
   SetFormat(format);
   SetHAlign(halign);
   SetVAlign(valign);
   SetIndent(indent);
   SetTxtOrientation(txtorientation);
   SetFillFGColor(fill_fgcolor);
   SetFillBGColor(fill_bgcolor);
   SetFillStyle(fillstyle);
   SetLocked(locked);
   SetHidden(hidden);
   SetWrap(wrap);
   SetCellMode(is_cell);
   SetBorderStyle(BORDER_BOTTOM,bottom_border_style, bottom_border_color);
   SetBorderStyle(BORDER_TOP,top_border_style, top_border_color);
   SetBorderStyle(BORDER_LEFT,left_border_style, left_border_color);
   SetBorderStyle(BORDER_RIGHT,right_border_style, right_border_color);

   UpdateSignature();
}

void xf_t::UpdateSignature()
{
   crc xfcrc;
   
   xfcrc<<index;
//   if(sizeof(font) >= sizeof(unsigned64_t)) {
//      xfcrc<<(unsigned64_t)font;
//   } else {
      xfcrc<<(unsigned64_t)font;
//   }
   xfcrc<<(unsigned32_t)format;
   xfcrc<<halign;
   xfcrc<<valign;
   xfcrc<<indent;
   xfcrc<<txtorientation;
   xfcrc<<fill_fgcolor;
   xfcrc<<fill_bgcolor;
   xfcrc<<fillstyle;
   xfcrc<<locked;
   xfcrc<<hidden;
   xfcrc<<wrap;
   xfcrc<<is_cell;
   xfcrc<<top_border_style;
   xfcrc<<top_border_color;
   xfcrc<<bottom_border_style;
   xfcrc<<bottom_border_color;
   xfcrc<<left_border_style;
   xfcrc<<left_border_color;
   xfcrc<<right_border_style;
   xfcrc<<right_border_color;

   m_signature = xfcrc.get();
   m_sigchanged = false;

}
/* 
**********************************
**********************************
*/
xf_t::~xf_t()
{
}

unsigned32_t xf_t::GetSignature()
{
   if(m_sigchanged)
   {
      UpdateSignature();      
      m_sigchanged = false;
   }
   
   return m_signature;
}

/* 
**********************************
**********************************
*/
void xf_t::MarkUsed(void) 
{
   m_usage_counter++;
   
   if(font != NULL)
      font->MarkUsed();

}

/* 
**********************************
**********************************
*/
bool xf_t::IsUsed(void) 
{
   return(m_usage_counter != 0);
}

/* 
**********************************
**********************************
*/
void xf_t::SetFont(font_t* fontidx)
{
   if(fontidx != NULL)
      fontidx->MarkUsed();
   font = fontidx;

   m_sigchanged = true;

}

font_t* xf_t::GetFont(void)
{
   return font;
}

unsigned16_t xf_t::GetFontIndex(void)
{
   if(font != NULL)
      return font->GetIndex();
   else
      return 0x0000;
}

/* 
**********************************
**********************************
*/

void xf_t::SetFormat(format_number_t formatidx){

   format = formatidx;
   m_sigchanged = true;
};

unsigned16_t xf_t::GetFormatIndex(void)
{
   return xf_t::FORMAT_NUM_OPTIONS_TABLE[format];
};

format_number_t xf_t::GetFormat(void)
{
   return format;
}
/* 
**********************************
**********************************
*/

/* Cell option wrappers*/
void xf_t::SetBorderStyle(border_side_t side, 
                          border_style_t style, 
                          color_name_t color) 
{
   switch (side)
   {
      case BORDER_BOTTOM:
         bottom_border_style = BORDERSTYLE_OPTIONS_TABLE[style];
         bottom_border_color = COLOR_OPTIONS_TABLE[color];

         break;

      case BORDER_TOP:
         top_border_style = BORDERSTYLE_OPTIONS_TABLE[style];
         top_border_color = COLOR_OPTIONS_TABLE[color];

         break;

      case BORDER_LEFT:
         left_border_style = BORDERSTYLE_OPTIONS_TABLE[style];
         left_border_color = COLOR_OPTIONS_TABLE[color];

         break;

      case BORDER_RIGHT:
         right_border_style = BORDERSTYLE_OPTIONS_TABLE[style];
         right_border_color = COLOR_OPTIONS_TABLE[color];

         break;

      default:
         // It cannot get here
         break;

   }

   m_sigchanged = true;

}

/* 
**********************************
**********************************
*/
unsigned8_t xf_t::GetBorderStyle(border_side_t side) 
{
   unsigned8_t ret_style = XF_BRDOPTION_NONE;

   switch (side)
   {
   
      case BORDER_BOTTOM:
         ret_style = bottom_border_style;
         break;

      case BORDER_TOP:
         ret_style = top_border_style;
         break;

      case BORDER_LEFT:
         ret_style = left_border_style;
         break;

      case BORDER_RIGHT:
         ret_style = right_border_style;
         break;

      default:
         // It cannot get here
         break;

   }

   return ret_style;
}

/* 
**********************************
**********************************
*/
unsigned8_t xf_t::GetBorderColor(border_side_t side) 
{
   unsigned8_t ret_color = XF_COLOR_CODE_BLACK;

   switch (side)
   {
   
      case BORDER_BOTTOM:
         ret_color = bottom_border_color;
         break;

      case BORDER_TOP:
         ret_color = top_border_color;
         break;

      case BORDER_LEFT:
         ret_color = left_border_color;
         break;

      case BORDER_RIGHT:
         ret_color = right_border_color;
         break;

      default:
         // It cannot get here
         break;

   }
   return ret_color;

}

void xf_t::operator=(xf_t& right)
{
   index = 0x0000;
   font = right.font;
   format = right.format;
   halign = right.halign;
   valign = right.valign;
   indent = right.indent;
   
   txtorientation = right.txtorientation;  
   fill_fgcolor = right.fill_fgcolor;
   fill_bgcolor = right.fill_bgcolor;
   fillstyle = right.fillstyle;       

   locked = right.locked; 
   hidden = right.hidden; 
   wrap = right.wrap;   
   is_cell  = right.is_cell;   

   top_border_style = right.top_border_style;    
   top_border_color = right.top_border_color;    
                     
   bottom_border_style = right.bottom_border_style; 
   bottom_border_color = right.bottom_border_color; 
                     
   left_border_style = right.left_border_style;   
   left_border_color = right.left_border_color;   
                     
   right_border_style = right.right_border_style;  
   right_border_color = right.right_border_color;  
 
   m_sigchanged = true;

}

/* Horizontal Align option wrappers*/
void xf_t::SetHAlign(halign_option_t ha_option)
{
   halign = xf_t::HALIGN_OPTIONS_TABLE[ha_option];
   m_sigchanged = true;
}
 
unsigned8_t xf_t::GetHAlign(void)
{
   return halign;
}

/* Vertical Align option wrappers*/
void xf_t::SetVAlign(valign_option_t va_option)
{
   valign = xf_t::VALIGN_OPTIONS_TABLE[va_option];
   m_sigchanged = true;
}

unsigned8_t xf_t::GetVAlign(void)
{
   return valign;
}

void xf_t::SetIndent(indent_option_t indent_option)
{

   indent = xf_t::INDENT_OPTIONS_TABLE[indent_option];

   m_sigchanged = true;
}
unsigned8_t xf_t::GetIndent(void)
{
   return indent;
}

/* Text orientation option wrappers*/
void xf_t::SetTxtOrientation(txtori_option_t ori_option)  
{
   txtorientation = xf_t::TXTORI_OPTIONS_TABLE[ori_option];
   m_sigchanged = true;
}

unsigned8_t xf_t::GetTxtOrientation(void)
{
   return txtorientation;
}

/* Fill Foreground color option wrappers*/
void        xf_t::SetFillFGColor(color_name_t color)
{
   fill_fgcolor = xf_t::COLOR_OPTIONS_TABLE[color];
   m_sigchanged = true;
}

unsigned8_t xf_t::GetFillFGColor(void)
{
   return fill_fgcolor;
}


/* Fill Background color option wrappers*/
void xf_t::SetFillBGColor(color_name_t color)
{
   fill_bgcolor = xf_t::COLOR_OPTIONS_TABLE[color];
   m_sigchanged = true;
}

unsigned8_t xf_t::GetFillBGColor(void)
{
   return fill_bgcolor;
}

/* Fill Style option wrappers*/
void xf_t::SetFillStyle(fill_option_t fill)
{
   fillstyle = xf_t::FILL_OPTIONS_TABLE[fill];
   m_sigchanged = true;
}

unsigned8_t xf_t::GetFillStyle(void)
{
   return fillstyle;
}

/* Locked option wrappers*/
void xf_t::SetLocked(bool locked_opt)
{
   locked = locked_opt;
   m_sigchanged = true;
}

bool xf_t::IsLocked(void)
{
   return locked;
}

/* Hidden option wrappers*/
void xf_t::SetHidden(bool hidden_opt)
{
   hidden = hidden_opt;
   m_sigchanged = true;
}
bool xf_t::IsHidden(void)
{
   return hidden;
}

/* Wrap option wrappers*/
void xf_t::SetWrap(bool wrap_opt)
{
   wrap = wrap_opt;
   m_sigchanged = true;
};

bool xf_t::IsWrap(void)
{
   return wrap;
};

/* Cell option wrappers*/
void xf_t::SetCellMode(bool cellmode)
{
   is_cell = cellmode;
   m_sigchanged = true;
}

bool xf_t::IsCell(void)
{
   return is_cell;
}





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: extformat.cpp,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:48  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

