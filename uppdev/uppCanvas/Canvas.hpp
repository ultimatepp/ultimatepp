/*
	This widget is copyrighted © 2006 by Thomas Michels
	License: Free
	Add this copyright to your About widget,please :-)
	Mail: thommichels@web.de
	Requirements: Ultimate++  http://upp.sourceforge.net/index.html
*/
#ifndef _Canvas_hpp_
#define _Canvas_hpp_

#include <CtrlLib/CtrlLib.h>
/*
	Canvas provides an easy way to draw on rectangular
	area embedded in a parentcontrol of your choice e.g.
	a StaticRect(so far I tested). But how is it possible to
	receive mouseclicks or to paint on Canvas without altering
	or override the virtual methods in a child class of Canvas?
	The solution are Callbacks. Consider a callback as a pointer
	to a method of Canvas called from another place (the parent
	window). In order to draw on the Canvas the parent window must
	provide and assign methods with matching signaturs to the Call-
	backs of Canvas. e.g.:

//Declaration
struct App : public TopWindow{
	typedef App CLASSNAME; //Needed by THISBACK macro
	Canvas canvas;				 //Prepair your brushes

	App();
	//Same signatur as canvas.paint(Draw& w)
	void OnCanvasPaint(Draw& w); //Want to paint on canvas
	//Paint on Click see below
	void OnCanvasMouseLeft(Point p, dword keyflags);
};

//Definition
App::App(){
	.
	.
	// The magic assignment:-)
	canvas.OnCanvasPaint		=THISBACK(OnCanvasPaint);
		// The left mousebutton click is now routed to the canvas
	canvas.OnMouseLeft  		=THISBACK(OnCanvasMouseLeft);
}

void App::OnCanvasPaint(Draw& w)
{	// Your paintings on Canvas goes here:
  Rect r=canvas.GetSize();
  //Draw a Line
  w.DrawLine(0,0,r.Width(),r.height(),1,Black());
}

And not still enough, Canvas offers a way to paint not only
in a Paint Callback. For this purpose you can use the DrawSheet()
function. To make it short here is an Example:

void App::OnCanvasMouseLeft(Point p, dword keyflags)
{ //Paint an ugly ellipse:-)
  Rect r=canvas.GetSize();
  DrawingDraw& ds=canvas.DrawSheet();
  ds.DrawEllipse(p.x,p.y,20,20,Red(),5,Green());
  canvas.EndSheet();//draw it now
}

Consider the Drawsheet as an additional paintlayer you can paint at.
Please compile this package to see work it all together:-)
Happy coding...

*/
class Canvas:public Ctrl{
public:
	Canvas();
	// Events fired by system
	// Paint draw the canvas
  virtual void Paint(Draw& w);
	// If left mousebutton pressed
	virtual void LeftDown(Point p, dword keyflags);
	// Callback router, one for Paint
	Callback1<Draw&> OnCanvasPaint;
	// and one for left mousebutton
	Callback2<Point,dword> OnMouseLeft;
	// Set or Get Backgroundcolor for canvas
	Color const& BGColor(Color const& cl=Color());
	// Reference to paint on the drawsheet
	DrawingDraw& DrawSheet();
	// Paint the sheet
	void EndSheet();
	// Clear the sheet
	void ClearSheet();
private:
	Color m_bckcolor;
	Drawing m_drawing;
	DrawingDraw m_drawsheet;
	bool isUserPainting;
	bool isCleared;
};

#endif
