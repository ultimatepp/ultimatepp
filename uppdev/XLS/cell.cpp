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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/cell.cpp,v $
 * $Revision: 1.2 $
 * $Author: dhoerl $
 * $Date: 2008/10/25 18:39:53 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <XLS/cell.h>

using namespace std;
using namespace xlslib_core;

cell_t::cell_t()
   :m_pGlobalRecs(NULL)
{

}

cell_t::~cell_t()
{

}

unsigned16_t cell_t::GetXFIndex()
{
   unsigned16_t xfindex;
#if 0
   pxf != NULL?
      xfindex = pxf->GetIndex()
      :xfindex = 0x000f;
#endif
   if(pxf != NULL)
      xfindex = pxf->GetIndex();
   else
      xfindex = 0x000f;

   return xfindex;
}


bool cell_t::operator==(const cell_t& right) const
{
   return (row == right.row ? true : false);
}

bool cell_t::operator!=(const cell_t& right) const
{
   return (row != right.row ? true : false);
}

/*
  bool cell_t::operator<(const cell_t& right) const
  {
  return (row < right.row);
  }

  bool cell_t::operator>(const cell_t& right) const
  {
  return (row > right.row);
  }


  bool cell_t::operator%(const cell_t& right) const
  {
  return (row == right.row && col == right.col);
  }
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   Implementation of the XF record interface (xf_i pure virtual interface)
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#define SET_XF_CELL_FUNCTION(function,value)                      \
   {                                                              \
      xf_t* currentxf = pxf;                                      \
      if(pxf == NULL)                                             \
         currentxf = m_pGlobalRecs->GetDefaultXF();               \
                                                                  \
      xf_t* xfbackup = new xf_t;                                  \
      (*xfbackup) = (*currentxf);                                 \
                                                                  \
      if(currentxf->Usage() > 1)                                  \
      {                                                           \
         xf_t* newxf = new xf_t();                                \
         (*newxf) = (*currentxf);                                 \
         newxf->function(value);                                  \
         newxf = m_pGlobalRecs->AddXFormat(newxf);                \
         pxf = newxf;                                             \
         pxf->MarkUsed();                                         \
                                                                  \
      }                                                           \
      else                                                        \
      {                                                           \
         xf_t* xfnew = new xf_t;                                  \
         (*xfnew) = (*currentxf);                                 \
         xfnew->function(value);                                  \
         xfnew->MarkUsed();                                       \
         xfnew = m_pGlobalRecs->ReplaceXFormat(xfbackup, xfnew);  \
         pxf = xfnew;                                             \
         pxf->MarkUsed();                                         \
                                                                  \
      }                                                           \
      delete xfbackup;                                            \
   }

void cell_t::borderstyle(border_side_t side,
                         border_style_t style,
                         color_name_t color)
{

   xf_t* currentxf = pxf;
   if(pxf == NULL)
      currentxf = m_pGlobalRecs->GetDefaultXF();

   xf_t* xfbackup = new xf_t;
   (*xfbackup) = (*currentxf);

   if(currentxf->Usage() > 1)
   {
      xf_t* newxf = new xf_t();
      (*newxf) = (*currentxf);
      newxf->SetBorderStyle(side, style, color);
      newxf = m_pGlobalRecs->AddXFormat(newxf);
      pxf = newxf;
      pxf->MarkUsed();

   }
   else
   {
      xf_t* xfnew = new xf_t;
      (*xfnew) = (*currentxf);
      xfnew->SetBorderStyle(side, style, color);
      xfnew->MarkUsed();
      xfnew = m_pGlobalRecs->ReplaceXFormat(xfbackup, xfnew);
      pxf = xfnew;
      pxf->MarkUsed();


   }
   delete xfbackup;
}

void cell_t::font(font_t* fontidx)
{

   SET_XF_CELL_FUNCTION(SetFont, fontidx);
/*
  xf_t* currentxf = pxf;
  if(pxf == NULL)
  currentxf = m_pGlobalRecs->GetDefaultXF();

  xf_t* xfbackup = new xf_t;
  (*xfbackup) = (*currentxf);

  if(currentxf->Usage() > 1)
  {
  xf_t* newxf = new xf_t();
  (*newxf) = (*currentxf);
  newxf->SetFont(fontidx);
  newxf = m_pGlobalRecs->AddXFormat(newxf);
  pxf = newxf;
  pxf->MarkUsed();

  }
  else
  {
  xf_t* xfnew = new xf_t;
  (*xfnew) = (*currentxf);
  xfnew->SetFont(fontidx);
  xfnew->MarkUsed();
  xfnew = m_pGlobalRecs->ReplaceXFormat(xfbackup, xfnew);
  pxf = xfnew;
  }

  delete xfbackup;
*/
}

void cell_t::format(format_number_t formatidx)
{
   SET_XF_CELL_FUNCTION(SetFormat,formatidx);
}

void cell_t::halign(halign_option_t ha_option)
{

   SET_XF_CELL_FUNCTION(SetHAlign,ha_option);

}

void cell_t::valign(valign_option_t va_option)
{
   SET_XF_CELL_FUNCTION(SetVAlign,va_option);
}

void cell_t::indent(indent_option_t indent_option)
{
   SET_XF_CELL_FUNCTION(SetIndent,indent_option);
}

void cell_t::orientation(txtori_option_t ori_option)
{
   SET_XF_CELL_FUNCTION(SetTxtOrientation,ori_option);
}

void cell_t::fillfgcolor(color_name_t color)
{
   SET_XF_CELL_FUNCTION(SetFillFGColor,color);
/*
  xf_t* currentxf = pxf;
  if(pxf == NULL)
  currentxf = m_pGlobalRecs->GetDefaultXF();

  xf_t* xfbackup = new xf_t;
  (*xfbackup) = (*currentxf);

  if(currentxf->Usage() > 1)
  {
  xf_t* newxf = new xf_t();
  (*newxf) = (*currentxf);
  newxf->SetFillFGColor(color);
  newxf = m_pGlobalRecs->AddXFormat(newxf);
  pxf = newxf;
  pxf->MarkUsed();

  }
  else
  {
  xf_t* xfnew = new xf_t;
  (*xfnew) = (*currentxf);
  xfnew->SetFillFGColor(color);
  xfnew->MarkUsed();
  xfnew = m_pGlobalRecs->ReplaceXFormat(xfbackup, xfnew);
  pxf = xfnew;
  pxf->MarkUsed();
  }
  delete xfbackup;
*/
}

void cell_t::fillbgcolor(color_name_t color)
{
   SET_XF_CELL_FUNCTION(SetFillBGColor,color);
}

void cell_t::fillstyle(fill_option_t fill)
{
   SET_XF_CELL_FUNCTION(SetFillStyle,fill);
/*
  xf_t* currentxf = pxf;
  if(pxf == NULL)
  currentxf = m_pGlobalRecs->GetDefaultXF();

  xf_t* xfbackup = new xf_t;
  (*xfbackup) = (*currentxf);

  if(currentxf->Usage() > 1)
  {
  xf_t* newxf = new xf_t();
  (*newxf) = (*currentxf);
  newxf->SetFillStyle(fill);
  newxf = m_pGlobalRecs->AddXFormat(newxf);
  pxf = newxf;
  pxf->MarkUsed();

  }
  else
  {
  xf_t* xfnew = new xf_t;
  (*xfnew) = (*currentxf);
  xfnew->SetFillStyle(fill);
  xfnew = m_pGlobalRecs->ReplaceXFormat(xfbackup, xfnew);
  pxf = xfnew;
  pxf->MarkUsed();
  
  }
  delete xfbackup;
*/
}

void cell_t::locked(bool locked_opt)
{
   SET_XF_CELL_FUNCTION(SetLocked,locked_opt);
}

void cell_t::hidden(bool hidden_opt)
{
   SET_XF_CELL_FUNCTION(SetHidden,hidden_opt);
}

void cell_t::wrap(bool wrap_opt)
{
   SET_XF_CELL_FUNCTION(SetWrap,wrap_opt);
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   Implementation of the FONT record interface (font_i pure virtual interface)
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define SET_CELL_FONT_FUNCTION(function,value)                       \
   {                                                                 \
      bool samexf = true;                                            \
                                                                     \
      xf_t* currentxf = pxf;                                         \
      if(pxf == NULL)                                                \
         currentxf = m_pGlobalRecs->GetDefaultXF();                  \
                                                                     \
      xf_t* xfbackup = new xf_t;                                     \
      (*xfbackup) = (*currentxf);                                    \
                                                                     \
      xf_t* newxf = currentxf;                                       \
      if(currentxf->Usage() > 1)                                     \
         samexf = false;                                             \
                                                                     \
      newxf = new xf_t();                                            \
      (*newxf) = (*currentxf);                                       \
                                                                     \
      font_t* currentfont = newxf->GetFont();                        \
      if(currentfont == NULL)                                        \
         currentfont = m_pGlobalRecs->GetDefaultFont();              \
      if(currentfont->Usage() > 1)                                   \
      {                                                              \
         font_t* newfont = new font_t();                             \
         (*newfont) = (*currentfont);                                \
         newfont->function(value);                                   \
         newfont = m_pGlobalRecs->AddFont(newfont);                  \
         newxf->SetFont(newfont);                                    \
      }                                                              \
      else                                                           \
      {                                                              \
         font_t* fntnew = new font_t;                                \
         (*fntnew) = (*currentfont);                                 \
         fntnew->function(value);                                    \
         fntnew = m_pGlobalRecs->ReplaceFont(currentfont, fntnew);   \
         newxf->SetFont(fntnew) ;                                    \
      }                                                              \
      if(samexf)                                                     \
      {                                                              \
         newxf = m_pGlobalRecs->ReplaceXFormat(xfbackup, newxf);     \
         pxf = newxf;                                                \
         pxf->MarkUsed();                                            \
      }                                                              \
      else                                                           \
      {                                                              \
         newxf = m_pGlobalRecs->AddXFormat(newxf);                   \
         pxf = newxf;                                                \
         pxf->MarkUsed();                                            \
      }                                                              \
      delete xfbackup;                                               \
   }

void cell_t::fontname(string fntname)
{


   SET_CELL_FONT_FUNCTION(SetName, fntname);
/*
  bool samexf = true;                                          
                                                                  
  xf_t* currentxf = pxf;                                       
  if(pxf == NULL)                                           
  currentxf = m_pGlobalRecs->GetDefaultXF();             
                                                                  
  xf_t* xfbackup = new xf_t;                                   
  (*xfbackup) = (*currentxf);                               
                                                                  
  xf_t* newxf = currentxf;                                  
  if(currentxf->Usage() > 1)                                   
  samexf = false;                                        
                                                                  
  newxf = new xf_t();                                          
  (*newxf) = (*currentxf);                                  
                                                                  
  font_t* currentfont = newxf->GetFont();                   
  if(currentfont == NULL)                                      
  currentfont = m_pGlobalRecs->GetDefaultFont();            
  if(currentfont->Usage() > 1)                                 
  {                                                            
  font_t* newfont = new font_t();                           
  (*newfont) = (*currentfont);                              
  newfont->SetName(fntname);                                
  newfont = m_pGlobalRecs->AddFont(newfont);             
  newxf->SetFont(newfont);                               
  }                                                            
  else                                                         
  {                                                            
  font_t* fntnew = new font_t;                              
  (*fntnew) = (*currentfont);                            
  fntnew->SetName(fntname);                                 
  fntnew = m_pGlobalRecs->ReplaceFont(currentfont, fntnew);
  newxf->SetFont(fntnew) ;                               
  }                                                            
  if(samexf)                                                   
  {                                                            
  newxf = m_pGlobalRecs->ReplaceXFormat(xfbackup, newxf);            
  pxf = newxf;
  pxf->MarkUsed();      
  }                                                            
  else                                                         
  {                                                            
  newxf = m_pGlobalRecs->AddXFormat(newxf);                 
  pxf = newxf;                                           
  pxf->MarkUsed();                                          
  }                                                            
  delete xfbackup;                                             
*/
}




void cell_t::fontheight(unsigned16_t fntheight)
{
   SET_CELL_FONT_FUNCTION(SetHeight,fntheight);
}

void cell_t::fontbold(boldness_option_t fntboldness)
{
/*
  bool samexf = true;

  xf_t* currentxf = pxf;
  if(pxf == NULL)
  currentxf = m_pGlobalRecs->GetDefaultXF();

  xf_t* xfbackup = new xf_t;
  (*xfbackup) = (*currentxf);


  xf_t* newxf = currentxf;
  if(currentxf->Usage() > 1)
  samexf = false;

  newxf = new xf_t();
  (*newxf) = (*currentxf);

  font_t* currentfont = newxf->GetFont();
  if(currentfont == NULL)
  currentfont = m_pGlobalRecs->GetDefaultFont();
  if(currentfont->Usage() > 1)
  {
  font_t* newfont = new font_t();
  (*newfont) = (*currentfont);
  newfont->SetBoldStyle(fntboldness);
  newfont = m_pGlobalRecs->AddFont(newfont);
  newxf->SetFont(newfont);

  }
  else
  {
  font_t* fntnew = new font_t;
  (*fntnew) = (*currentfont);
  fntnew->SetBoldStyle(fntboldness);
  fntnew = m_pGlobalRecs->ReplaceFont(currentfont, fntnew);
  newxf->SetFont(fntnew) ;
  }

  if(samexf)
  {
  newxf = m_pGlobalRecs->ReplaceXFormat(xfbackup, newxf);
  pxf = newxf;
  }
  else
  {
  newxf = m_pGlobalRecs->AddXFormat(newxf);
  pxf = newxf;
  pxf->MarkUsed();
  }


  delete xfbackup;
*/
   SET_CELL_FONT_FUNCTION(SetBoldStyle,fntboldness);
}

void cell_t::fontunderline(underline_option_t fntunderline)
{
   SET_CELL_FONT_FUNCTION(SetUnderlineStyle,fntunderline);
}

void cell_t::fontscript(script_option_t fntscript)
{
   SET_CELL_FONT_FUNCTION(SetScriptStyle,fntscript);
}

void cell_t::fontcolor(color_name_t fntcolor)
{
   SET_CELL_FONT_FUNCTION(SetColor,fntcolor);
}

void cell_t::fontattr(unsigned16_t attr)
{
   SET_CELL_FONT_FUNCTION(SetAttributes,attr);
}

void cell_t::fontitalic(bool italic)
{
   SET_CELL_FONT_FUNCTION(SetItalic,italic);
}

void cell_t::fontstrikeout(bool so)
{
   SET_CELL_FONT_FUNCTION(SetStrikeout,so);
}

void cell_t::fontoutline(bool ol)
{
   SET_CELL_FONT_FUNCTION(SetOutline,ol);
}

void cell_t::fontshadow(bool sh)
{
   SET_CELL_FONT_FUNCTION(SetShadow,sh);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: cell.cpp,v $
 * Revision 1.2  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:54  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

