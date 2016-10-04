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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/font.h,v $
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

#ifndef FONT_H
#define FONT_H

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/rectypes.h>
#include <XLS/record.h>
#include <XLS/colors.h>

namespace xlslib_core
{
  // Bold style most used values 
  // (it can be a number of some range. See Documentation for details):
#define FONT_BOLDNESS_BOLD			((unsigned16_t)0x02bc)
#define FONT_BOLDNESS_HALF			((unsigned16_t)0x0226)
#define FONT_BOLDNESS_NORMAL		((unsigned16_t)0x0190)
#define FONT_BOLDNESS_DOUBLE		((unsigned16_t)0x0320)

	typedef enum
	{
		BOLDNESS_BOLD = 0,
		BOLDNESS_HALF,
		BOLDNESS_NORMAL,
		BOLDNESS_DOUBLE
	} boldness_option_t;


  // Super/subscript field option values
#define FONT_SCRIPT_NONE			((unsigned16_t)0x0000)
#define FONT_SCRIPT_SUPER			((unsigned16_t)0x0001)
#define FONT_SCRIPT_SUB				((unsigned16_t)0x0002)

	typedef enum
	{
		SCRIPT_NONE = 0,
		SCRIPT_SUPER,
		SCRIPT_SUB
	} script_option_t;

  // Underline field option values:
#define FONT_UNDERLINE_NONE			((unsigned8_t)0x00)
#define FONT_UNDERLINE_SINGLE		((unsigned8_t)0x01)
#define FONT_UNDERLINE_DOUBLE		((unsigned8_t)0x02)
#define FONT_UNDERLINE_SINGLEACC	((unsigned8_t)0x21)
#define FONT_UNDERLINE_DOUBLEACC	((unsigned8_t)0x22)

	typedef enum
	{
		UNDERLINE_NONE = 0,
		UNDERLINE_SINGLE,
		UNDERLINE_DOUBLE,
		UNDERLINE_SINGLEACC,
		UNDERLINE_DOUBLEACC
	} underline_option_t;



  // The following are default values used when the font's
  // constructor is called without args:
#define FONT_DFLT_FAMILY			((unsigned8_t)0x00)
#define FONT_DFLT_CHARSET			((unsigned8_t)0x00)
#define FONT_DFLT_HEIGHT			((unsigned16_t)0x00c8)
#define FONT_DFLT_ATTRIBUTES		((unsigned16_t)0x0000)
#define FONT_DFLT_PALETTE			((unsigned16_t)0x7fff)
#define FONT_DFLT_FONTNAME			((std::string)"Arial")

#define FONT_RESERVED				((unsigned8_t)0x00)

  // The font-record field offsets:
#define FONT_OFFSET_HEIGHT			((unsigned32_t) 4)
#define FONT_OFFSET_ATTRIBUTES		((unsigned32_t) 6)
#define FONT_OFFSET_PALETTE			((unsigned32_t) 8)
#define FONT_OFFSET_BOLDSTYLE		((unsigned32_t)10)
#define FONT_OFFSET_SCRIPT			((unsigned32_t)12)
#define FONT_OFFSET_UNDERLINE		((unsigned32_t)14)
#define FONT_OFFSET_FAMILY			((unsigned32_t)15)
#define FONT_OFFSET_CHARSET			((unsigned32_t)16)
#define FONT_OFFSET_NAMELENGTH		((unsigned32_t)18)
#define FONT_OFFSET_NAME			((unsigned32_t)19)


  // The attribute bit or-masks:
#define FONT_ATTR_RESERVED0			((unsigned16_t)0x0001)
#define FONT_ATTR_ITALIC			((unsigned16_t)0x0002)
#define FONT_ATTR_RESERVED1			((unsigned16_t)0x0004)
#define FONT_ATTR_STRIKEOUT			((unsigned16_t)0x0008)
#define FONT_ATTR_OUTLINEMACH		((unsigned16_t)0x0010)
#define FONT_ATTR_SHADOWMACH		((unsigned16_t)0x0020)
#define FONT_ATTR_RESERVED2			((unsigned16_t)0x00c0)
#define FONT_ATTR_UNUSED			((unsigned16_t)0xff00)

  // Font colors
#define FONT_COLOR_CODE_BLACK					((unsigned16_t)(COLOR_CODE_BLACK))
#define FONT_COLOR_CODE_DARK_RED				((unsigned16_t)(COLOR_CODE_DARK_RED))
#define FONT_COLOR_CODE_RED						((unsigned16_t)(COLOR_CODE_RED))
#define FONT_COLOR_CODE_FUCSIA					((unsigned16_t)(COLOR_CODE_FUCSIA))
#define FONT_COLOR_CODE_COMBINED01				((unsigned16_t)(COLOR_CODE_COMBINED01))
#define FONT_COLOR_CODE_COMBINED02				((unsigned16_t)(COLOR_CODE_COMBINED02))
#define FONT_COLOR_CODE_COMBINED03				((unsigned16_t)(COLOR_CODE_COMBINED03))
#define FONT_COLOR_CODE_COMBINED04				((unsigned16_t)(COLOR_CODE_COMBINED04))
#define FONT_COLOR_CODE_COMBINED05				((unsigned16_t)(COLOR_CODE_COMBINED05))
#define FONT_COLOR_CODE_COMBINED06				((unsigned16_t)(COLOR_CODE_COMBINED06))
#define FONT_COLOR_CODE_OLIVE					((unsigned16_t)(COLOR_CODE_OLIVE))
#define FONT_COLOR_CODE_DARK_YELLOW				((unsigned16_t)(COLOR_CODE_DARK_YELLOW))
#define FONT_COLOR_CODE_COMBINED07				((unsigned16_t)(COLOR_CODE_COMBINED07))
#define FONT_COLOR_CODE_YELLOW					((unsigned16_t)(COLOR_CODE_YELLOW))
#define FONT_COLOR_CODE_LIGHT_YELLOW			((unsigned16_t)(COLOR_CODE_LIGHT_YELLOW))
#define FONT_COLOR_CODE_DARK_GREEN				((unsigned16_t)(COLOR_CODE_DARK_GREEN))
#define FONT_COLOR_CODE_GREEN					((unsigned16_t)(COLOR_CODE_GREEN))
#define FONT_COLOR_CODE_COMBINED08				((unsigned16_t)(COLOR_CODE_COMBINED08))
#define FONT_COLOR_CODE_LIVING_GREEN			((unsigned16_t)(COLOR_CODE_LIVING_GREEN))
#define FONT_COLOR_CODE_LIGHT_GREEN				((unsigned16_t)(COLOR_CODE_LIGHT_GREEN))
#define FONT_COLOR_CODE_COMBINED09				((unsigned16_t)(COLOR_CODE_COMBINED09))
#define FONT_COLOR_CODE_BLUE_GREEN				((unsigned16_t)(COLOR_CODE_BLUE_GREEN))
#define FONT_COLOR_CODE_AQUAMARINA				((unsigned16_t)(COLOR_CODE_AQUAMARINA))
#define FONT_COLOR_CODE_TURQOISE				((unsigned16_t)(COLOR_CODE_TURQOISE))
#define FONT_COLOR_CODE_COMBINED10				((unsigned16_t)(COLOR_CODE_COMBINED10))
#define FONT_COLOR_CODE_DARK_BLUE				((unsigned16_t)(COLOR_CODE_DARK_BLUE))
#define FONT_COLOR_CODE_BLUE					((unsigned16_t)(COLOR_CODE_BLUE))
#define FONT_COLOR_CODE_LIGHT_BLUE				((unsigned16_t)(COLOR_CODE_LIGHT_BLUE))
#define FONT_COLOR_CODE_SKY_BLUE				((unsigned16_t)(COLOR_CODE_SKY_BLUE))
#define FONT_COLOR_CODE_COMBINED11				((unsigned16_t)(COLOR_CODE_COMBINED11))
#define FONT_COLOR_CODE_INDIGO					((unsigned16_t)(COLOR_CODE_INDIGO))
#define FONT_COLOR_CODE_BLUE_GRAY				((unsigned16_t)(COLOR_CODE_BLUE_GRAY))
#define FONT_COLOR_CODE_VIOLET					((unsigned16_t)(COLOR_CODE_VIOLET))
#define FONT_COLOR_CODE_PLUM					((unsigned16_t)(COLOR_CODE_PLUM))
#define FONT_COLOR_CODE_LAVANDER				((unsigned16_t)(COLOR_CODE_LAVANDER))
#define FONT_COLOR_CODE_COMBINED12				((unsigned16_t)(COLOR_CODE_COMBINED12))
#define FONT_COLOR_CODE_GRAY50					((unsigned16_t)(COLOR_CODE_GRAY50))
#define FONT_COLOR_CODE_GRAY40					((unsigned16_t)(COLOR_CODE_GRAY40))
#define FONT_COLOR_CODE_GRAY25					((unsigned16_t)(COLOR_CODE_GRAY25))
#define FONT_COLOR_CODE_WHITE					((unsigned16_t)(COLOR_CODE_WHITE))

  /* 
******************************
CFont class declaration
******************************
*/

  typedef struct
  {
    unsigned16_t index;
    std::string name;
    unsigned16_t height;
    boldness_option_t boldstyle;
    underline_option_t underline;
    script_option_t script;
    color_name_t color;
    unsigned16_t attributes;
    unsigned8_t family;
    unsigned8_t charset;

  } font_init_t;

  struct font_i
  {
    virtual void fontname(std::string fntname) = 0;
    virtual void fontheight(unsigned16_t fntheight) = 0;
    virtual void fontbold(boldness_option_t fntboldness) = 0;
    virtual void fontunderline(underline_option_t fntunderline) = 0;
    virtual void fontscript(script_option_t fntscript) = 0;
    virtual void fontcolor(color_name_t fntcolor) = 0;
    virtual void fontattr(unsigned16_t attr) = 0;
    virtual void fontitalic(bool italic) = 0;
    virtual void fontstrikeout(bool so) = 0;
    virtual void fontoutline(bool ol) = 0;
    virtual void fontshadow(bool sh) = 0;                              
  };


  class font_t
    {
    public:
      font_t();
      font_t(unsigned16_t index, std::string name,
             unsigned16_t height, boldness_option_t boldstyle,
             underline_option_t underline, script_option_t script,
             color_name_t color, unsigned16_t attributes,
             unsigned8_t family, unsigned8_t charset);
      font_t(const font_init_t& fontinit);

      virtual ~font_t() {};


      void MarkUsed();
      bool IsUsed();
      unsigned16_t Usage(){return m_usage_counter;};


      /* FONT Index wrappers*/
      void         SetIndex(unsigned16_t fntidx);
      unsigned16_t GetIndex(void);


      /* FONT Index wrappers*/
      void   SetName(std::string fntname);
      std::string GetName(void);

      /* FONT height wrappers*/
      void         SetHeight(unsigned16_t fntheight);
      unsigned16_t GetHeight(void);

      /* FONT boldstyle wrappers*/
      void SetBoldStyle(boldness_option_t fntboldness);
      unsigned16_t GetBoldStyle(void);

      /* FONT underline wrappers*/
      void        SetUnderlineStyle(underline_option_t fntunderline);
      unsigned8_t GetUnderlineStyle(void);

      /* FONT script wrappers*/
      void         SetScriptStyle(script_option_t fntscript);
      unsigned16_t GetScriptStyle(void);

      /* FONT script wrappers*/
      void         SetColor(color_name_t fntcolor);
      unsigned16_t GetColor(void);


      void SetItalic(bool italic);
      void SetStrikeout(bool so);


      /* FONT  attributes wrappers */
      void SetAttributes(unsigned16_t attr);
      unsigned16_t GetAttributes(void);


      // OSX only
      void SetOutline(bool ol);
      void SetShadow(bool sh);


      // Miscellaneous;
      void        SetFamily(unsigned8_t fam);
      unsigned8_t GetFamily(void);

      void         SetCharset(unsigned8_t chrset);
      unsigned8_t  GetCharset(void);
      unsigned32_t GetSignature();

      void operator=(font_t& right);

    private:
      unsigned16_t		index;
      std::string		name;
      unsigned16_t		height;
      unsigned16_t		boldstyle;
      unsigned8_t		underline;
      unsigned16_t		script;
      unsigned16_t		color;
      unsigned16_t		attributes;
      unsigned8_t       family;
      unsigned8_t       charset;

      unsigned16_t m_usage_counter;
      void UpdateSignature();
      unsigned32_t m_signature;
      bool m_sigchanged;

      static const unsigned16_t		BOLD_OPTION_TABLE[];
      static const unsigned16_t		SCRIPT_OPTION_TABLE[];
      static const unsigned8_t		UNDERLINE_OPTION_TABLE[];
      static const unsigned16_t		COLOR_OPTION_TABLE[];

    };

  class fontbysig
    {
    public:
      bool operator()(font_t* a, font_t* b) const
        {
          return (a->GetSignature() < b->GetSignature() ? true : false );
        };
    };

  typedef std::set<xlslib_core::font_t*,fontbysig XLSLIB_DFLT_ALLOCATOR> Font_Set_t;
  typedef Font_Set_t::iterator Font_Set_Itor_t;

  typedef std::list<xlslib_core::font_t* XLSLIB_DFLT_ALLOCATOR> Font_List_t;
  typedef Font_List_t::iterator Font_List_Itor_t;


  class CFont: public CRecord
    {
    protected:

    public:

      CFont(std::string name		= FONT_DFLT_FONTNAME,
            unsigned16_t height		= FONT_DFLT_HEIGHT,
            unsigned16_t boldstyle  = FONT_BOLDNESS_NORMAL,
            unsigned8_t underline	= FONT_UNDERLINE_NONE,
            unsigned16_t script		= FONT_SCRIPT_NONE,
            unsigned16_t color		= FONT_DFLT_PALETTE,
            unsigned16_t attributes	= FONT_DFLT_ATTRIBUTES,
            unsigned8_t family		= FONT_DFLT_FAMILY,
            unsigned8_t charset		= FONT_DFLT_CHARSET   );

      CFont(font_t* fontdef);
      ~CFont();


      int SetName(std::string& name);
      int GetName(std::string& name);

      int SetBoldValue(unsigned16_t boldstyle);
      unsigned16_t GetBoldValue(void);

      int SetUnderlineStyle(unsigned8_t ul);
      unsigned8_t GetUnderlineStyle(void);

      int SetScriptStyle(unsigned16_t script);
      unsigned16_t GetScriptStyle(void);

      int SetColorIndex(unsigned16_t color);
      unsigned16_t GetColorIndex(void);

      int SetAttributes(unsigned16_t attr);
      unsigned16_t GetAttributes(void);

      int SetFamily(unsigned8_t family);
      unsigned8_t GetFamily(void);

      int SetCharSet(unsigned8_t charset);
      unsigned8_t GetCharSet(void);

    };

}

#endif //FONT_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: font.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * Revision 1.17  2004/04/06 22:35:43  dgonzalez
 * + Added namespace xlslib_core to isolate the core xlslib library names.
 *
 * Revision 1.16  2004/04/06 15:10:24  dgonzalez
 * + Added keys to generate distributable headers
 *
 * Revision 1.15  2004/01/29 03:18:55  dgonzalez
 * + Using the config.h file
 *
 * Revision 1.14  2004/01/15 17:17:31  dgonzalez
 * + The memory allocator used by STL-containers is defined #conditionally.
 *
 * Revision 1.13  2003/12/09 19:04:21  dgonzalez
 * + Not a real difference
 *
 * Revision 1.12  2003/12/05 01:18:27  dgonzalez
 * + Common include files were placed in common.h (and this file was
 *     included instead.
 * + The font_interface was redefined to something lighter. Due name
 *     clashing with xf_i when these interfaces were defined as parent of
 *     cell_t.
 * + Signature was implemented to font_t. Any interface function that modi
 *    fies any data member of the class set a flag for later signature
 *    update. The signature is CRC based and is used to uniquify font_t
 *    instances inside an stl-set-container.
 * + The fontbysig functor was added for being used in the set container
 *     as a sorting criteria. It is based on the signature.
 * + The stl-set-container of pointers to font_t* typedef  was defined here.
 *
 * Revision 1.11  2003/11/29 17:53:49  dgonzalez
 * + The larger functions were uninlined.
 * + The virtual pure interface font_i was defined
 *
 * Revision 1.10        2003/11/28 01:31:30     dgonzalez
 * + The MarkUsed functionality was added (similar to xf_t)
 *
 * Revision 1.9  2003/11/26 16:49:26  dgonzalez
 * + Indented and untabified
 *
 * Revision 1.8  2003/11/04 17:07:58  dgonzalez
 * + Changed the initialization of CGlobalRecord's defaults for compatibility with
 *              MSVC++. The original code was left commented out for future use.
 *
 * Revision 1.7  2003/11/04 01:04:42  dgonzalez
 * + All the list containers changed to manage pointers.
 * + Valgrind utility reports 960 posible memory leaks. TODO: Verify it
 *
 * Revision 1.6  2003/11/03 16:58:56  dgonzalez
 * + The following typedefed enums were added to improve typechecking in the font
 *        definition:
 *                - boldness_option_t
 *                - script_option_t
 *                - underline_option_t
 * + Font colors were defined based in the color codes defined in the module colors.h.
 * + font_t is now implemented as a class. A full set of wrapper functions were implemented
 *        inline for Get/Set font properties.
 * + font_init_t struct was added for storing initialization values of default font of a
 *        document.
 * + The following lookup tables were added to retrieve the field's options used for setting
 *        the font's values.
 *
 * Revision 1.5  2003/10/24 23:24:32  dgonzalez
 * + Added CVS-Keyword substitution.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

