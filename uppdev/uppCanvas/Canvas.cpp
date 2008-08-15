/*
	This widget is copyrighted © 2006 by Thomas Michels
	License: Free
	Add this copyright to your About widget,please :-)
	Mail: thommichels@web.de
*/


#include "Canvas.hpp"

Canvas::Canvas():
	m_bckcolor(199,199,205),OnCanvasPaint(0),
	isUserPainting(false),isCleared(true)
{
  // For the sake of better outfit
	AddFrame(InsetFrame());
	AddFrame(InsetFrame());
	AddFrame(BlackFrame());

}

void Canvas::Paint(Draw& w)
{
  // Get inner rectangle of the frames
	Size sz = GetSize();
	// Paint the background first
	w.DrawRect(sz,m_bckcolor);
	// Execute callback only if it was correctly assigned!
	if(OnCanvasPaint){
	  // Important flag to avoid recursiv callback calls
	  isUserPainting=true;
	// Now painting what ever the user of Canvas want
	  OnCanvasPaint(w);
	  // Danger is over
	  isUserPainting=false;
	};
	  // Clears the sheet
	if(isCleared){
		// Set the size of the sheet
  	m_drawsheet.Create(sz);
  	m_drawing.Clear();//sheet
		w.DrawDrawing(sz,m_drawing);//make clearing visible
	}
	// At least paint the sheet, if no sheetclearing wanted
	if(!isCleared){
	  // Make sure no "stretching" effects occures while resizing
	  //comment it out and see what happen:-)
	  m_drawing.SetSize(m_drawing.RatioSize(sz));//For this i searched 5 hours!
		//Paint the sheet and make it visible
		w.DrawDrawing(sz, m_drawing);
	}
	isCleared=false;
}

void Canvas::LeftDown(Point p, dword keyflags)
{
	if (OnMouseLeft)//assigned
	  // Execute
	  OnMouseLeft(p,keyflags);
}

Color const& Canvas::BGColor(Color const& cl)
{  // Setter and Getter method in one function
   // If not Defaultvalue then usage:
   // BGColor (color);
	if(!cl.IsNullInstance()){
	  m_bckcolor=cl;
	  // Invoke Canvas::Paint
	  Refresh();
	};
	// If defaultvalue then usage:
	// color=BGColor();
	return m_bckcolor;
}

DrawingDraw& Canvas::DrawSheet()
{	// Reference to the sheet to paint on it
  return m_drawsheet;
}

void Canvas::EndSheet()
{ // This method must be called immediately after
  // the paintings on the sheet, otherwise the sheet
  // stays invisible:-)
  // DrawDrawing needs that
  m_drawing=m_drawsheet;
  // Don't Refresh while Painting to avoid unwanted recursiv calls!-)
  if(!isUserPainting)
  	Refresh();
}

void Canvas::ClearSheet()
{	// Sometimes it is usefull to start a new paint
  isCleared=true;
  // Follow my own recommendation
  EndSheet();
}
