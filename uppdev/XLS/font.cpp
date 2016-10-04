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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/font.cpp,v $
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



#include <XLS/font.h>
#include <XLS/crc.h>

using namespace std;
using namespace xlslib_core;


/* 
**********************************
CFont class implementation
**********************************
*/
CFont::CFont(string	name,			
	     unsigned16_t height, 
	     unsigned16_t boldstyle,
	     unsigned8_t  underline,
	     unsigned16_t script, 
	     unsigned16_t color,	 
	     unsigned16_t attributes,
	     unsigned8_t  family, 
	     unsigned8_t  charset)
{
   SetRecordType(RECTYPE_FONT);

   AddValue16(height);
   AddValue16(attributes);
   AddValue16(color);
   AddValue16(boldstyle);
   AddValue16(script);
   AddValue8(underline);
   AddValue8(family);
   AddValue8(charset);
   AddValue8(FONT_RESERVED);

#if VERSION_BIFF == VERSION_BIFF5
   AddValue8((unsigned8_t)name.size());
   AddDataArray((const unsigned8_t*)name.c_str(), name.size());
#else
   AddUnicodeStrlen((unsigned8_t)name.size());
   AddUnicodeString((const unsigned8_t*)name.c_str(), name.size());
#endif

   SetRecordLength(GetDataSize()-4);
}

CFont::CFont(font_t* fontdef)
{
   SetRecordType(RECTYPE_FONT);

   AddValue16(fontdef->GetHeight());
   AddValue16(fontdef->GetAttributes());
   AddValue16(fontdef->GetColor());
   AddValue16(fontdef->GetBoldStyle());
   AddValue16(fontdef->GetScriptStyle());
   AddValue8(fontdef->GetUnderlineStyle());
   AddValue8(fontdef->GetFamily());
   AddValue8(fontdef->GetCharset());
   AddValue8(FONT_RESERVED);

#if VERSION_BIFF == VERSION_BIFF5
   AddValue8((unsigned8_t)((fontdef->GetName()).size()));
   AddDataArray((const unsigned8_t*)((fontdef->GetName()).c_str()), 
		(fontdef->GetName()).size());
#else
   AddUnicodeStrlen((unsigned8_t)((fontdef->GetName()).size()));
   AddUnicodeString((const unsigned8_t*)((fontdef->GetName()).c_str()), 
		(fontdef->GetName()).size());
#endif
	 
   SetRecordLength(GetDataSize()-4);
}

CFont::~CFont()
{
}

/* 
**********************************
**********************************
*/


int CFont::SetName(string& name)
{
   int errcode = NO_ERRORS;

   name = name;
   /*
     NOTE: Unimplemented. 
     Temporarilly this function won't set the name
   */
  
   return errcode;
}

/* 
**********************************
**********************************
*/
int CFont::GetName(string& name)
{
   int errcode = NO_ERRORS;

   signed16_t namesize;
   GetValue16From(&namesize, FONT_OFFSET_NAMELENGTH);

   name =  "";
   for(int i=0; i<namesize; i++)
      name += operator[](FONT_OFFSET_NAME+i);

   return errcode;
}

/* 
**********************************
**********************************
*/
int CFont::SetBoldValue(unsigned16_t boldstyle)
{
   return (SetValueAt((signed16_t)boldstyle, FONT_OFFSET_BOLDSTYLE));
}

/* 
**********************************
**********************************
*/
unsigned16_t CFont::GetBoldValue(void)
{
   unsigned16_t boldstyleval;
   GetValue16From((signed16_t*)&boldstyleval, FONT_OFFSET_BOLDSTYLE);

   return(boldstyleval);
}

/* 
**********************************
**********************************
*/
int CFont::SetUnderlineStyle(unsigned8_t ul)
{
   return (SetAt(ul, FONT_OFFSET_UNDERLINE));
}

/* 
**********************************
**********************************
*/
unsigned8_t CFont::GetUnderlineStyle(void)
{
   unsigned8_t ulval;
   GetAt((signed8_t*)&ulval, FONT_OFFSET_UNDERLINE);
	
   return(ulval);


}

/* 
**********************************
**********************************
*/
int CFont::SetScriptStyle(unsigned16_t script)
{
   return (SetValueAt((signed16_t)script, FONT_OFFSET_SCRIPT));
}

/* 
**********************************
**********************************
*/
unsigned16_t CFont::GetScriptStyle(void)
{
   unsigned16_t scriptval;
   GetValue16From((signed16_t*)&scriptval, FONT_OFFSET_SCRIPT);

   return(scriptval);

}

/* 
**********************************
**********************************
*/
int CFont::SetColorIndex(unsigned16_t color)
{
   return (SetValueAt((signed16_t)color, FONT_OFFSET_PALETTE));	
}

/* 
**********************************
**********************************
*/
unsigned16_t CFont::GetColorIndex(void)
{
   unsigned16_t colorval;
   GetValue16From((signed16_t*)&colorval, FONT_OFFSET_PALETTE);

   return(colorval);

}

/* 
**********************************
**********************************
*/
int CFont::SetAttributes(unsigned16_t attr)
{
   return (SetValueAt((signed16_t)attr, FONT_OFFSET_ATTRIBUTES));	  
}

/* 
**********************************
**********************************
*/
unsigned16_t CFont::GetAttributes(void)
{
   unsigned16_t attrval;
   GetValue16From((signed16_t*)&attrval, FONT_OFFSET_ATTRIBUTES);

   return(attrval);

}

/* 
**********************************
**********************************
*/
int CFont::SetFamily(unsigned8_t family)
{
   return (SetAt(family, FONT_OFFSET_FAMILY));
}

/* 
**********************************
**********************************
*/
unsigned8_t CFont::GetFamily(void)
{
   unsigned8_t familyval;
   GetAt((signed8_t*)&
	 familyval, FONT_OFFSET_FAMILY);
	
   return(familyval);

}

/* 
**********************************
**********************************
*/
int CFont::SetCharSet(unsigned8_t charset)
{
   return (SetAt(charset, FONT_OFFSET_CHARSET));
}

/* 
**********************************
**********************************
*/
unsigned8_t CFont::GetCharSet(void)
{
   unsigned8_t charsetval;
   GetAt((signed8_t*)&charsetval, FONT_OFFSET_CHARSET);
	
   return(charsetval);
}

/* 
**********************************
font_t class implementation
**********************************
*/

const unsigned16_t font_t::BOLD_OPTION_TABLE[] = {
   FONT_BOLDNESS_BOLD,
   FONT_BOLDNESS_HALF,
   FONT_BOLDNESS_NORMAL,
   FONT_BOLDNESS_DOUBLE 
};


const unsigned16_t font_t::SCRIPT_OPTION_TABLE[] = {
   FONT_SCRIPT_NONE,
   FONT_SCRIPT_SUPER,
   FONT_SCRIPT_SUB
};


const unsigned8_t font_t::UNDERLINE_OPTION_TABLE[] = {
   FONT_UNDERLINE_NONE,
   FONT_UNDERLINE_SINGLE,
   FONT_UNDERLINE_DOUBLE,
   FONT_UNDERLINE_SINGLEACC,
   FONT_UNDERLINE_DOUBLEACC
};


const unsigned16_t font_t::COLOR_OPTION_TABLE[] = {

   FONT_COLOR_CODE_BLACK,
   FONT_COLOR_CODE_DARK_RED,
   FONT_COLOR_CODE_RED,
   FONT_COLOR_CODE_FUCSIA,
   FONT_COLOR_CODE_COMBINED01,
   FONT_COLOR_CODE_COMBINED02,
   FONT_COLOR_CODE_COMBINED03,
   FONT_COLOR_CODE_COMBINED04,
   FONT_COLOR_CODE_COMBINED05,
   FONT_COLOR_CODE_COMBINED06,
   FONT_COLOR_CODE_OLIVE,
   FONT_COLOR_CODE_DARK_YELLOW,
   FONT_COLOR_CODE_COMBINED07,
   FONT_COLOR_CODE_YELLOW,
   FONT_COLOR_CODE_LIGHT_YELLOW,
   FONT_COLOR_CODE_DARK_GREEN,
   FONT_COLOR_CODE_GREEN,
   FONT_COLOR_CODE_COMBINED08,
   FONT_COLOR_CODE_LIVING_GREEN,
   FONT_COLOR_CODE_LIGHT_GREEN,
   FONT_COLOR_CODE_COMBINED09,
   FONT_COLOR_CODE_BLUE_GREEN,
   FONT_COLOR_CODE_AQUAMARINA,
   FONT_COLOR_CODE_TURQOISE,
   FONT_COLOR_CODE_COMBINED10,
   FONT_COLOR_CODE_DARK_BLUE,
   FONT_COLOR_CODE_BLUE,
   FONT_COLOR_CODE_LIGHT_BLUE,
   FONT_COLOR_CODE_SKY_BLUE,
   FONT_COLOR_CODE_COMBINED11,
   FONT_COLOR_CODE_INDIGO,
   FONT_COLOR_CODE_BLUE_GRAY,
   FONT_COLOR_CODE_VIOLET,
   FONT_COLOR_CODE_PLUM,
   FONT_COLOR_CODE_LAVANDER,
   FONT_COLOR_CODE_COMBINED12,
   FONT_COLOR_CODE_GRAY50,
   FONT_COLOR_CODE_GRAY40,
   FONT_COLOR_CODE_GRAY25,
   FONT_COLOR_CODE_WHITE
};


font_t::font_t() :
   index(0x0000),
   name(FONT_DFLT_FONTNAME),
   height(FONT_DFLT_HEIGHT),
   boldstyle(FONT_BOLDNESS_NORMAL),
   underline(FONT_UNDERLINE_NONE),
   script(FONT_SCRIPT_NONE),
   color(FONT_DFLT_PALETTE),
   attributes(FONT_DFLT_ATTRIBUTES),
   family(FONT_DFLT_FAMILY),
   charset(FONT_DFLT_CHARSET),
   m_usage_counter(0)
{ 
   UpdateSignature();

}


font_t::font_t(unsigned16_t index,
	       string name,
	       unsigned16_t height,
	       boldness_option_t boldstyle,
	       underline_option_t underline,
	       script_option_t script,
	       color_name_t color,
	       unsigned16_t attributes,
	       unsigned8_t family,
	       unsigned8_t charset)
   : m_usage_counter(0)
{

   SetIndex(index);
   SetName(name);
   SetHeight(height);
   SetBoldStyle(boldstyle);
   SetUnderlineStyle(underline);
   SetScriptStyle(script);
   SetColor(color);
   SetAttributes(attributes);
   SetFamily(family);
   SetCharset(charset);

   UpdateSignature();

}


font_t::font_t(const font_init_t& fontinit)
   :		m_usage_counter(0)
{
   SetIndex(fontinit.index);
   SetName(fontinit.name);
   SetHeight(fontinit.height);
   SetBoldStyle(fontinit.boldstyle);
   SetUnderlineStyle(fontinit.underline);
   SetScriptStyle(fontinit.script);
   SetColor(fontinit.color);
   SetAttributes(fontinit.attributes);
   SetFamily(fontinit.family);
   SetCharset(fontinit.charset);

   UpdateSignature();

}

void font_t::operator=(font_t& right)
{
   index = 0x0000;
  
   name       = right.name      ;
   height     = right.height    ;
   boldstyle  = right.boldstyle ;
   underline  = right.underline ;
   script     = right.script    ;
   color      = right.color     ;
   attributes = right.attributes;
   family     = right.family    ;
   charset    = right.charset   ;
   
   m_sigchanged = true;

}

void font_t::UpdateSignature()
{
   crc xfcrc;

   xfcrc<<index;
   xfcrc<<name;
   xfcrc<<height;
   xfcrc<<boldstyle;
   xfcrc<<underline;
   xfcrc<<script;
   xfcrc<<color;
   xfcrc<<attributes;
   xfcrc<<family;
   xfcrc<<charset;

   m_signature = xfcrc.get();
   m_sigchanged = false;
}

unsigned32_t font_t::GetSignature()
{
   if(m_sigchanged)
   {
      UpdateSignature();      
      m_sigchanged = false;
   }
   
   return m_signature;
}

void font_t::MarkUsed() 
{
   m_usage_counter++;
}

bool font_t::IsUsed() 
{
   return(m_usage_counter != 0);
}

void font_t::SetItalic(bool italic) {
   if(italic)
      attributes |= FONT_ATTR_ITALIC;
   else
      attributes &= (~FONT_ATTR_ITALIC);

   m_sigchanged = true;
}

void font_t::SetStrikeout(bool so) {
   if(so)
      attributes |= FONT_ATTR_STRIKEOUT;
   else
      attributes &= (~FONT_ATTR_STRIKEOUT);

   m_sigchanged = true;
}

// Machintoch only
void font_t::SetOutline(bool ol) {
   if(ol)
      attributes |= FONT_ATTR_OUTLINEMACH;
   else
      attributes &= (~FONT_ATTR_OUTLINEMACH);

   m_sigchanged = true;
}

// OSX (Mac) only
void font_t::SetShadow(bool sh) {
   if(sh)
      attributes |= FONT_ATTR_SHADOWMACH;
   else
      attributes &= (~FONT_ATTR_SHADOWMACH);

   m_sigchanged = true;
}


/* FONT Index wrappers*/
void font_t::SetIndex(unsigned16_t fntidx) 
{
   index = fntidx;
   m_sigchanged = true;
}

unsigned16_t font_t::GetIndex(void) 
{
   return index;
}

/* FONT Index wrappers*/
void font_t::SetName(string fntname) 
{
   name = fntname;
   m_sigchanged = true;
}

string font_t::GetName(void) 
{
   return name;
}

/* FONT height wrappers*/
void font_t::SetHeight(unsigned16_t fntheight) 
{
   height = fntheight;
   m_sigchanged = true;
}
unsigned16_t font_t::GetHeight(void) 
{
   return height;
}

/* FONT boldstyle wrappers*/
void font_t::SetBoldStyle(boldness_option_t fntboldness) 
{
   boldstyle = font_t::BOLD_OPTION_TABLE[fntboldness];
   m_sigchanged = true;
}
unsigned16_t font_t::GetBoldStyle(void) 
{
   return boldstyle;
}

/* FONT underline wrappers*/
void font_t::SetUnderlineStyle(underline_option_t fntunderline) 
{
   underline = font_t::UNDERLINE_OPTION_TABLE[fntunderline];
   m_sigchanged = true;
}
unsigned8_t font_t::GetUnderlineStyle(void)
{
   return underline;
}

/* FONT script wrappers*/
void font_t::SetScriptStyle(script_option_t fntscript) 
{
   script = font_t::SCRIPT_OPTION_TABLE[fntscript];
   m_sigchanged = true;
}
unsigned16_t font_t::GetScriptStyle(void)
{
   return script;
}

/* FONT script wrappers*/
void font_t::SetColor(color_name_t fntcolor) 
{
   color = font_t::COLOR_OPTION_TABLE[fntcolor];
   m_sigchanged = true;
}
unsigned16_t font_t::GetColor(void) 
{
   return color;
}

/* FONT  attributes wrappers */
void font_t::SetAttributes(unsigned16_t attr) 
{
   attributes = attr;
   m_sigchanged = true;
}
	
unsigned16_t font_t::GetAttributes(void) 
{
   return attributes;
}


// Miscellaneous;
void font_t::SetFamily(unsigned8_t fam) 
{
   family = fam;
   m_sigchanged = true;
}
unsigned8_t font_t::GetFamily(void) 
{
   return family;
}

void font_t::SetCharset(unsigned8_t chrset) 
{
   charset = chrset;
   m_sigchanged = true;
}

unsigned8_t font_t::GetCharset(void) 
{
   return charset;
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: font.cpp,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:52  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

