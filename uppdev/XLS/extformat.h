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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/extformat.h,v $
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


#ifndef EXTFORMAT_H
#define EXTFORMAT_H

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/rectypes.h>
#include <XLS/record.h>
#include <XLS/colors.h>
#include <XLS/font.h>
#include <XLS/format.h>

namespace xlslib_core
{

// COMMON

  // The Cell Properties bit or-masks:
#define XF_PROP_SHIFTPOS_PARENT  (4)

#define XF_PROP_LOCKED      ((unsigned16_t)0x0001)
#define XF_PROP_HIDDEN      ((unsigned16_t)0x0002)
#define XF_PROP_STYLE       ((unsigned16_t)0x0004)
#define XF_PROP_123PREFIX   ((unsigned16_t)0x0008)
#define XF_PROP_XFPARENT    ((unsigned16_t)0xFFF0)

  // The Alignment field bit or-masks:
#define XF_ALIGN_HORIZONTAL  ((unsigned16_t)0x0007)
#define XF_ALIGN_WRAP        ((unsigned16_t)0x0008)
#define XF_ALIGN_VERTICAL    ((unsigned16_t)0x0070)
#define XF_ALIGN_JUSTLAST    ((unsigned32_t)0x0080) /* BIFF8: Used only in far-east versions of excel */
#define XF_ALIGN_ORIENTATION ((unsigned16_t)0x0300)

  // Geometric Align options
#define XF_ALIGN_SHIFTPOS_HALIGN  (0)
#define XF_ALIGN_SHIFTPOS_VALIGN  (4)

  // XF_USED_ATTRIB
#define XF_ALIGN_ATRNUM				((unsigned16_t)0x0400)
#define XF_ALIGN_ATRFONT			((unsigned16_t)0x0800)
#define XF_ALIGN_ATRALC				((unsigned16_t)0x1000)
#define XF_ALIGN_ATRBDR				((unsigned16_t)0x2000)
#define XF_ALIGN_ATRPAT				((unsigned16_t)0x4000)
#define XF_ALIGN_ATRPROT			((unsigned16_t)0x8000)

  // Style options
#define XF_FILL_NONE                ((unsigned8_t)0x00)
#define XF_FILL_SOLID               ((unsigned8_t)0x01)
#define XF_FILL_ATEN75              ((unsigned8_t)0x03)
#define XF_FILL_ATEN50              ((unsigned8_t)0x02)
#define XF_FILL_ATEN25              ((unsigned8_t)0x04)
#define XF_FILL_ATEN12              ((unsigned8_t)0x11)
#define XF_FILL_ATEN06              ((unsigned8_t)0x12)
#define XF_FILL_HORIZ_LIN           ((unsigned8_t)0x05)
#define XF_FILL_VERTICAL_LIN        ((unsigned8_t)0x06)
#define XF_FILL_DIAG                ((unsigned8_t)0x07)
#define XF_FILL_INV_DIAG            ((unsigned8_t)0x08)
#define XF_FILL_INTER_DIAG          ((unsigned8_t)0x09)
#define XF_FILL_DIAG_THICK_INTER    ((unsigned8_t)0x0a)
#define XF_FILL_HORIZ_LINES_THIN    ((unsigned8_t)0x0b)
#define XF_FILL_VERTICAL_LINES_THIN ((unsigned8_t)0x0c)
#define XF_FILL_DIAG_THIN           ((unsigned8_t)0x0d)
#define XF_FILL_INV_DIAG_THIN       ((unsigned8_t)0x0e)
#define XF_FILL_HORIZ_INT_THIN      ((unsigned8_t)0x0f)
#define XF_FILL_HORIZ_INTER_THICK   ((unsigned8_t)0x10)
  typedef enum
    {
      FILL_NONE = 0,
      FILL_SOLID,
      FILL_ATEN75,
      FILL_ATEN50,
      FILL_ATEN25,
      FILL_ATEN12,
      FILL_ATEN06,
      FILL_HORIZ_LIN,
      FILL_VERTICAL_LIN,
      FILL_DIAG,
      FILL_INV_DIAG,
      FILL_INTER_DIAG,
      FILL_DIAG_THICK_INTER,
      FILL_HORIZ_LINES_THIN,
      FILL_VERTICAL_LINES_THIN,
      FILL_DIAG_THIN,
      FILL_INV_DIAG_THIN,
      FILL_HORIZ_INT_THIN,
      FILL_HORIZ_INTER_THICK   
    } fill_option_t;

  // Border Options
#define XF_BRDOPTION_NONE       ((unsigned8_t)0x00)
#define XF_BRDOPTION_THIN       ((unsigned8_t)0x01)
#define XF_BRDOPTION_MEDIUM     ((unsigned8_t)0x02)
#define XF_BRDOPTION_DASHED     ((unsigned8_t)0x03)
#define XF_BRDOPTION_DOTTED     ((unsigned8_t)0x04)
#define XF_BRDOPTION_THICK      ((unsigned8_t)0x05)
#define XF_BRDOPTION_DOUBLE     ((unsigned8_t)0x06)
#define XF_BRDOPTION_HAIR       ((unsigned8_t)0x07)
  typedef enum
    {
      BORDER_NONE = 0,
      BORDER_THIN,
      BORDER_MEDIUM,
      BORDER_DASHED,
      BORDER_DOTTED,
      BORDER_THICK,
      BORDER_DOUBLE,
      BORDER_HAIR    
    } border_style_t;

   // Border options
#define XF_BORDER_BOTTOM  ((unsigned8_t)0)
#define XF_BORDER_TOP     ((unsigned8_t)1)
#define XF_BORDER_LEFT    ((unsigned8_t)2)
#define XF_BORDER_RIGHT   ((unsigned8_t)3)
  typedef enum
    {
      BORDER_BOTTOM = 0,
      BORDER_TOP,
      BORDER_LEFT,
      BORDER_RIGHT,
	  DIAGONALS		// BIFF8
    } border_side_t;

  // Horizontal Align options
#define XF_HALIGN_GENERAL         ((unsigned8_t)0)
#define XF_HALIGN_LEFT            ((unsigned8_t)1)
#define XF_HALIGN_CENTER          ((unsigned8_t)2)
#define XF_HALIGN_RIGHT           ((unsigned8_t)3)
#define XF_HALIGN_FILL            ((unsigned8_t)4)
#define XF_HALIGN_JUSTIFY         ((unsigned8_t)5)
#define XF_HALIGN_CENTERACCROSS   ((unsigned8_t)6)
  typedef enum
    {
      HALIGN_GENERAL = 0,
      HALIGN_LEFT,
      HALIGN_CENTER,
      HALIGN_RIGHT,
      HALIGN_FILL,
      HALIGN_JUSTIFY,
      HALIGN_CENTERACCROSS
    } halign_option_t;

   // Vertical Align options
#define XF_VALIGN_TOP     ((unsigned8_t)0)
#define XF_VALIGN_CENTER  ((unsigned8_t)1)
#define XF_VALIGN_BOTTOM  ((unsigned8_t)2)
#define XF_VALIGN_JUSTIFY ((unsigned8_t)3)
  typedef enum
    {
      VALIGN_TOP = 0,
      VALIGN_CENTER,
      VALIGN_BOTTOM,
      VALIGN_JUSTIFY
    } valign_option_t;


#define XF_COLOR_CODE_BLACK           ((unsigned8_t)(COLOR_CODE_BLACK))
#define XF_COLOR_CODE_DARK_RED        ((unsigned8_t)(COLOR_CODE_DARK_RED))
#define XF_COLOR_CODE_RED             ((unsigned8_t)(COLOR_CODE_RED))
#define XF_COLOR_CODE_FUCSIA          ((unsigned8_t)(COLOR_CODE_FUCSIA))
#define XF_COLOR_CODE_COMBINED01      ((unsigned8_t)(COLOR_CODE_COMBINED01))
#define XF_COLOR_CODE_COMBINED02      ((unsigned8_t)(COLOR_CODE_COMBINED02))
#define XF_COLOR_CODE_COMBINED03      ((unsigned8_t)(COLOR_CODE_COMBINED03))
#define XF_COLOR_CODE_COMBINED04      ((unsigned8_t)(COLOR_CODE_COMBINED04))
#define XF_COLOR_CODE_COMBINED05      ((unsigned8_t)(COLOR_CODE_COMBINED05))
#define XF_COLOR_CODE_COMBINED06      ((unsigned8_t)(COLOR_CODE_COMBINED06))
#define XF_COLOR_CODE_OLIVE           ((unsigned8_t)(COLOR_CODE_OLIVE))
#define XF_COLOR_CODE_DARK_YELLOW     ((unsigned8_t)(COLOR_CODE_DARK_YELLOW))
#define XF_COLOR_CODE_COMBINED07      ((unsigned8_t)(COLOR_CODE_COMBINED07))
#define XF_COLOR_CODE_YELLOW          ((unsigned8_t)(COLOR_CODE_YELLOW))
#define XF_COLOR_CODE_LIGHT_YELLOW    ((unsigned8_t)(COLOR_CODE_LIGHT_YELLOW))
#define XF_COLOR_CODE_DARK_GREEN      ((unsigned8_t)(COLOR_CODE_DARK_GREEN))
#define XF_COLOR_CODE_GREEN           ((unsigned8_t)(COLOR_CODE_GREEN))
#define XF_COLOR_CODE_COMBINED08      ((unsigned8_t)(COLOR_CODE_COMBINED08))
#define XF_COLOR_CODE_LIVING_GREEN    ((unsigned8_t)(COLOR_CODE_LIVING_GREEN))
#define XF_COLOR_CODE_LIGHT_GREEN     ((unsigned8_t)(COLOR_CODE_LIGHT_GREEN))
#define XF_COLOR_CODE_COMBINED09      ((unsigned8_t)(COLOR_CODE_COMBINED09))
#define XF_COLOR_CODE_BLUE_GREEN      ((unsigned8_t)(COLOR_CODE_BLUE_GREEN))
#define XF_COLOR_CODE_AQUAMARINA      ((unsigned8_t)(COLOR_CODE_AQUAMARINA))
#define XF_COLOR_CODE_TURQOISE        ((unsigned8_t)(COLOR_CODE_TURQOISE))
#define XF_COLOR_CODE_COMBINED10      ((unsigned8_t)(COLOR_CODE_COMBINED10))
#define XF_COLOR_CODE_DARK_BLUE       ((unsigned8_t)(COLOR_CODE_DARK_BLUE))
#define XF_COLOR_CODE_BLUE            ((unsigned8_t)(COLOR_CODE_BLUE))
#define XF_COLOR_CODE_LIGHT_BLUE      ((unsigned8_t)(COLOR_CODE_LIGHT_BLUE))
#define XF_COLOR_CODE_SKY_BLUE        ((unsigned8_t)(COLOR_CODE_SKY_BLUE))
#define XF_COLOR_CODE_COMBINED11      ((unsigned8_t)(COLOR_CODE_COMBINED11))
#define XF_COLOR_CODE_INDIGO          ((unsigned8_t)(COLOR_CODE_INDIGO))
#define XF_COLOR_CODE_BLUE_GRAY       ((unsigned8_t)(COLOR_CODE_BLUE_GRAY))
#define XF_COLOR_CODE_VIOLET          ((unsigned8_t)(COLOR_CODE_VIOLET))
#define XF_COLOR_CODE_PLUM            ((unsigned8_t)(COLOR_CODE_PLUM))
#define XF_COLOR_CODE_LAVANDER        ((unsigned8_t)(COLOR_CODE_LAVANDER))
#define XF_COLOR_CODE_COMBINED12      ((unsigned8_t)(COLOR_CODE_COMBINED12))
#define XF_COLOR_CODE_GRAY50          ((unsigned8_t)(COLOR_CODE_GRAY50))
#define XF_COLOR_CODE_GRAY40          ((unsigned8_t)(COLOR_CODE_GRAY40))
#define XF_COLOR_CODE_GRAY25          ((unsigned8_t)(COLOR_CODE_GRAY25))
#define XF_COLOR_CODE_WHITE           ((unsigned8_t)(COLOR_CODE_WHITE))

#define XF_LOCKED			true
#define XF_NO_LOCKED		false

#define XF_HIDDEN			true
#define XF_NO_HIDDEN		false

#define XF_WRAPPED			true
#define XF_NO_WRAPPED		false

#define XF_IS_CELL			true
#define XF_IS_STYLE			false







  // The XF-record field offsets:
#if VERSION_BIFF == VERSION_BIFF5

#define XF_OFFSET_FONT         ((unsigned32_t) 4)
#define XF_OFFSET_FORMAT       ((unsigned32_t) 6)
#define XF_OFFSET_PROP         ((unsigned32_t) 8)
#define XF_OFFSET_ALIGN        ((unsigned32_t)10)	// 2 bytes
#define XF_OFFSET_COLOR        ((unsigned32_t)12)
#define XF_OFFSET_BORDER0      ((unsigned32_t)14)
#define XF_OFFSET_BORDER1      ((unsigned32_t)16)
#define XF_OFFSET_BORDER2      ((unsigned32_t)18)

  // Text Orientation Options
#define XF_ORI_SHIFTPOS	(8)
#define XF_ORI_NONE             ((unsigned8_t)0)
#define XF_ORI_TOPBOTTOMTXT     ((unsigned8_t)1)
#define XF_ORI_90NOCLOCKTXT     ((unsigned8_t)2)
#define XF_ORI_90CLOCKTXT       ((unsigned8_t)3)
  typedef enum
    {
      ORI_NONE = 0,      
      ORI_TOPBOTTOMTXT,
      ORI_90NOCLOCKTXT,
      ORI_90CLOCKTXT     
    } txtori_option_t;

  // XF_USED_ATTRIB
#define XF_ATTRIB_SHIFTPOS		(0)

  // The Color field bit or-masks:
#define XF_COLOR_FOREGROUND    ((unsigned16_t)0x007f)
#define XF_COLOR_BACKGROUND    ((unsigned16_t)0x1f80)

#define XF_COLOR_SHIFTPOS_FG		(0)
#define XF_COLOR_SHIFTPOS_BG		(7)

  // The Border0 field bit or-masks:

#define XF_STYLE_SHIFTPOS_BOTTOM	(6)
#define XF_COLOR_SHIFTPOS_BOTTOM	(9)

#define XF_BORDER_FILLPATTERN   ((unsigned16_t)0x003f)
#define XF_BORDER_BOTTOMSTYLE   ((unsigned16_t)0x01c0)
#define XF_BORDER_BOTTOMCOLOR   ((unsigned16_t)0xfe00)

  // The Border1 field bit or-masks:
#define XF_STYLE_SHIFTPOS_TOP   (0)
#define XF_STYLE_SHIFTPOS_LEFT  (3)
#define XF_STYLE_SHIFTPOS_RIGHT (6)
#define XF_COLOR_SHIFTPOS_TOP   (9)

#define XF_BORDER_TOPSTYLE     ((unsigned16_t)0x0007)
#define XF_BORDER_LEFTSTYLE    ((unsigned16_t)0x0038)
#define XF_BORDER_RIGHTSTYLE   ((unsigned16_t)0x01c0)
#define XF_BORDER_TOPCOLOR     ((unsigned16_t)0xfe00)

  // The Border2 field bit or-masks:
#define XF_COLOR_SHIFTPOS_LEFT  (0)
#define XF_COLOR_SHIFTPOS_RIGHT (7)

#define XF_BORDER_LEFTCOLOR    ((unsigned16_t)0x007f)
#define XF_BORDER_RIGHTCOLOR   ((unsigned16_t)0x3f80)
//#define XF_BORDER2_RESERVED     ((unsigned16_t)0xc000)




#else	// VERSION_BIFF == VERSION_BIFF8




#define XF_OFFSET_FONT         ((unsigned32_t) 4)
#define XF_OFFSET_FORMAT       ((unsigned32_t) 6)
#define XF_OFFSET_PROP         ((unsigned32_t) 8)
#define XF_OFFSET_ALIGN        ((unsigned32_t)10)	// 4 bytes
#define XF_OFFSET_BORDERA      ((unsigned32_t)14)	// 4 bytes
#define XF_OFFSET_BORDERB      ((unsigned32_t)18)	// 4 bytes
#define XF_OFFSET_COLOR        ((unsigned32_t)22)

  // Text Orientation Options
#define XF_ORI_SHIFTPOS			(8)
#define XF_ORI_NONE             ((unsigned8_t)0)
#define XF_ORI_90NOCLOCKTXT     ((unsigned8_t)90)
#define XF_ORI_90CLOCKTXT       ((unsigned8_t)180)
#define XF_ORI_TOPBOTTOMTXT     ((unsigned8_t)255)
  typedef enum
    {
      ORI_NONE = 0,      
      ORI_TOPBOTTOMTXT,
      ORI_90NOCLOCKTXT,
      ORI_90CLOCKTXT     
    } txtori_option_t;

  // Indent field
#define XF_INDENT_SHIFTPOS		(16)
#define XF_INDENT_LVL			((unsigned8_t)0x0F)
#define XF_INDENT_NONE			((unsigned8_t)0x00)
#define XF_INDENT_1				((unsigned8_t)0x01)
#define XF_INDENT_2				((unsigned8_t)0x02)
#define XF_INDENT_3				((unsigned8_t)0x03)
#define XF_INDENT_4				((unsigned8_t)0x04)
#define XF_INDENT_SHRINK2FIT    ((unsigned8_t)0x10)
#define XF_INDENT_DIR			((unsigned8_t)0xC0)
#define XF_INDENT_CONTEXT		((unsigned8_t)0x00)
#define XF_INDENT_L2R			((unsigned8_t)0x40)
#define XF_INDENT_R2L			((unsigned8_t)0x80)
  typedef enum
    {
      INDENT_NONE = 0,
      INDENT_1,
      INDENT_2,
      INDENT_3,
      INDENT_4,
      INDENT_SHRINK2FIT,
      INDENT_L2R,
	  INDENT_R2L
    } indent_option_t;

  // XF_USED_ATTRIB
#define XF_ATTRIB_SHIFTPOS		(16)	// bits shifted by 8 already

  // The Border A field bit or-masks:

#define XF_STYLE_SHIFTPOS_LEFT		(0)
#define XF_STYLE_SHIFTPOS_RIGHT		(4)
#define XF_STYLE_SHIFTPOS_TOP		(8)
#define XF_STYLE_SHIFTPOS_BOTTOM	(12)

  // The BorderA field bit or-masks:
#define XF_COLOR_SHIFTPOS_LEFT  (16)
#define XF_COLOR_SHIFTPOS_RIGHT (23)

#define XF_BORDER_LEFTSTYLE		((unsigned32_t)0x0000000F)
#define XF_BORDER_RIGHTSTYLE	((unsigned32_t)0x000000F0)
#define XF_BORDER_TOPSTYLE		((unsigned32_t)0x00000F00)
#define XF_BORDER_BOTTOMSTYLE   ((unsigned32_t)0x0000F000)

#define XF_BORDER_LEFTCOLOR		((unsigned32_t)0x007f0000)
#define XF_BORDER_RIGHTCOLOR	((unsigned32_t)0x3f800000)
#define XF_DIAG_TL2BR			((unsigned32_t)0x40000000)
#define XF_DIAG_BL2TR			((unsigned32_t)0x80000000)

  // BORDER B 

#define XF_COLOR_SHIFTPOS_TOP		(0)
#define XF_COLOR_SHIFTPOS_BOTTOM	(7)
#define XF_COLOR_SHIFTPOS_DIAG		(14)
#define XF_STYLE_SHIFTPOS_DIAG		(21)
#define XF_SHIFTPOS_FILLPATTERN		(26)

#define XF_BORDER_TOPCOLOR     ((unsigned16_t)0x0000007f)
#define XF_BORDER_BOTTOMCOLOR  ((unsigned16_t)0x00003f80)
#define XF_BORDER_DIAGCOLOR    ((unsigned16_t)0x001fc000)
#define XF_BORDER_DIAGSTYLE    ((unsigned16_t)0x01e00000)
#define XF_BORDER_FILLPATTERN  ((unsigned32_t)0xFC000000)

//#define XF_STYLE_SHIFTPOS_LEFT  (3)
//#define XF_STYLE_SHIFTPOS_RIGHT (6)
//#define XF_COLOR_SHIFTPOS_TOP   (9)
//#define XF_BORDER1_TOPSTYLE     ((unsigned16_t)0x0007)
//#define XF_BORDER1_LEFTSTYLE    ((unsigned16_t)0x0038)
//#define XF_BORDER1_RIGHTSTYLE   ((unsigned16_t)0x01c0)

  // The Color field bit or-masks:
#define XF_COLOR_SHIFTPOS_FG  (0)
#define XF_COLOR_SHIFTPOS_BG  (7)
#define XF_COLOR_FOREGROUND    ((unsigned16_t)0x007f)
#define XF_COLOR_BACKGROUND    ((unsigned16_t)0x3f80)
#define XF_COLOR_DIAG          ((unsigned16_t)0xc000)


#endif
  /*
******************************
CExtFormat class declaration
******************************
*/

  typedef struct
  {
    unsigned16_t index;

    font_t* font;
    format_number_t format;

    halign_option_t halign;
    valign_option_t valign;
	indent_option_t indent;
    txtori_option_t txtorientation;

    color_name_t fill_fgcolor;
    color_name_t fill_bgcolor;
    fill_option_t fillstyle;

    bool locked;
    bool hidden;
    bool wrap;
    bool is_cell;

    border_style_t top_border_style;
    color_name_t top_border_color;

    border_style_t bottom_border_style;
    color_name_t bottom_border_color;

    border_style_t left_border_style;
    color_name_t  left_border_color;

    border_style_t right_border_style;
    color_name_t right_border_color;

  } xf_init_t;

  struct xf_i
  {
    virtual void font(font_t* fontidx) = 0;
    virtual void format(format_number_t format) = 0;
    virtual void halign(halign_option_t ha_option) = 0;
    virtual void valign(valign_option_t va_option) = 0;
    virtual void indent(indent_option_t indent_opt) = 0;
    virtual void orientation(txtori_option_t ori_option) = 0;
    virtual void fillfgcolor(color_name_t color) = 0;
    virtual void fillbgcolor(color_name_t color) = 0;
    virtual void fillstyle(fill_option_t fill) = 0;
    virtual void locked(bool locked_opt) = 0;
    virtual void hidden(bool hidden_opt) = 0;
    virtual void wrap(bool wrap_opt) = 0;
    virtual void borderstyle(border_side_t side,
                             border_style_t style,
                             color_name_t color) = 0;
  };


  class xf_t
    {

    public:
      xf_t();
      xf_t(const xf_init_t& xfinit);
      xf_t(unsigned16_t index, font_t* font, format_number_t format, halign_option_t halign,
           valign_option_t valign, indent_option_t indent, txtori_option_t txtorientation, color_name_t fill_fgcolor,
           color_name_t fill_bgcolor, fill_option_t fillstyle, bool locked, bool hidden,
           bool wrap, bool is_cell, border_style_t top_border_style, color_name_t top_border_color,
           border_style_t bottom_border_style, color_name_t bottom_border_color, border_style_t left_border_style,
           color_name_t  left_border_color, border_style_t right_border_style, color_name_t right_border_color);
      virtual ~xf_t();

      void         MarkUsed(void);
      bool         IsUsed(void);
      unsigned16_t Usage(){return m_usage_counter;};


      /* XF Index wrappers*/
      void         SetIndex(unsigned16_t xfidx){index = xfidx;};
      unsigned16_t GetIndex(void){return index;};

      /* Font Index wrappers*/
      void         SetFont(font_t* fontidx);
      unsigned16_t GetFontIndex(void);
      font_t*      GetFont(void);

      /* Format Index wrappers*/
      void            SetFormat(format_number_t formatidx);
      unsigned16_t    GetFormatIndex(void);
      format_number_t GetFormat(void);


      /* Horizontal Align option wrappers*/
      void        SetHAlign(halign_option_t ha_option);
      unsigned8_t GetHAlign(void);

      /* Vertical Align option wrappers*/
      void        SetVAlign(valign_option_t va_option);
      unsigned8_t GetVAlign(void);

      /* Vertical Align option wrappers*/
      void        SetIndent(indent_option_t indent_option);
      unsigned8_t GetIndent(void);

      /* Text orientation option wrappers*/
      void		  SetTxtOrientation(txtori_option_t ori_option);
      unsigned8_t GetTxtOrientation(void);


      /* Fill Foreground color option wrappers*/
      void        SetFillFGColor(color_name_t color);
      unsigned8_t GetFillFGColor(void);


      /* Fill Background color option wrappers*/
      void        SetFillBGColor(color_name_t color);
      unsigned8_t GetFillBGColor(void);


      /* Fill Style option wrappers*/
      void        SetFillStyle(fill_option_t fill);
      unsigned8_t GetFillStyle(void);

      /* Locked option wrappers*/
      void SetLocked(bool locked_opt);
      bool IsLocked(void);


      /* Hidden option wrappers*/
      void SetHidden(bool hidden_opt);
      bool IsHidden(void);

      /* Wrap option wrappers*/
      void SetWrap(bool wrap_opt);
      bool IsWrap(void);


      /* Cell option wrappers*/
      void SetCellMode(bool cellmode);
      bool IsCell(void);

      /* Cell option wrappers*/
      void         SetBorderStyle(border_side_t side,
                                  border_style_t style,
                                  color_name_t color);
      unsigned8_t  GetBorderStyle(border_side_t side);
      unsigned8_t  GetBorderColor(border_side_t side);
      unsigned32_t GetSignature();

      void operator=(xf_t& right) ;

    private:
      // Lookup tables for options
      static const unsigned8_t HALIGN_OPTIONS_TABLE[];
      static const unsigned8_t VALIGN_OPTIONS_TABLE[];
      static const unsigned8_t INDENT_OPTIONS_TABLE[];
      static const unsigned8_t TXTORI_OPTIONS_TABLE[];
      static const unsigned8_t COLOR_OPTIONS_TABLE[];
      static const unsigned8_t FILL_OPTIONS_TABLE[];
      static const unsigned8_t BORDERSTYLE_OPTIONS_TABLE[];
      static const unsigned16_t FORMAT_NUM_OPTIONS_TABLE[];
      void UpdateSignature();
    private:
      unsigned16_t m_usage_counter;

      unsigned16_t index;

      font_t* font;
      format_number_t format;

      unsigned8_t halign;
      unsigned8_t valign;
      unsigned8_t indent;
      unsigned8_t txtorientation;

      unsigned8_t fill_fgcolor;
      unsigned8_t fill_bgcolor;
      unsigned8_t fillstyle;

      bool locked;
      bool hidden;
      bool wrap;
      bool is_cell;

      unsigned8_t top_border_style;
      unsigned8_t top_border_color;

      unsigned8_t bottom_border_style;
      unsigned8_t bottom_border_color;

      unsigned8_t left_border_style;
      unsigned8_t left_border_color;

      unsigned8_t right_border_style;
      unsigned8_t right_border_color;

      unsigned32_t m_signature;
      bool m_sigchanged;

    };

  class xfbysig
    {
    public:
      bool operator()(xf_t* a, xf_t* b) const
        {
          return (a->GetSignature() < b->GetSignature() ? true : false );	// DFH
        };
    };

  typedef std::set<xlslib_core::xf_t*, xfbysig XLSLIB_DFLT_ALLOCATOR> XF_Set_t;
  typedef XF_Set_t::iterator XF_Set_Itor_t;

  typedef std::list<xlslib_core::xf_t* XLSLIB_DFLT_ALLOCATOR> XF_List_t;
  typedef XF_List_t::iterator XF_List_Itor_t;



  class CExtFormat: public CRecord
    {
    private:

      void SetFlag(unsigned16_t flag);
      void ClearFlag(unsigned16_t flag);

      void InitDummy(bool is_cell);

    public:

      CExtFormat(bool is_cell = false);
      CExtFormat(xf_t* xfdef);
      ~CExtFormat();

      bool IsCell();

      int SetFontIndex(unsigned16_t fontindex);
      unsigned16_t GetFontIndex(void);

      int SetFormatIndex(unsigned16_t formatindex);
      unsigned16_t GetFormatIndex(void);

      void SetLocked();
      void SetHidden();
      void SetHorizAlign(unsigned8_t alignval);
      void SetWrap();
#if VERSION_BIFF == VERSION_BIFF8
	  void SetIndent(unsigned8_t indentval);
#endif
      void SetVertAlign(unsigned8_t alignval);
      void SetTxtOrientation(unsigned8_t alignval);
      void SetFGColorIndex(unsigned8_t color);
      void SetBGColorIndex(unsigned8_t color);
      void SetFillPattern(unsigned8_t color);
      void SetBorder(border_side_t border, unsigned16_t style, unsigned16_t color);
      void SetXFParent(unsigned16_t parent);
    };

}
#endif //EXTFORMAT_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: extformat.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


