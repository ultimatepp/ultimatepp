#include <uppCanvas/main.hpp>

App::App()
{
	CtrlLayout(*this, "Canvas Demo");

	bottom_left.Color(LtGray);
	bottom_left.AddFrame(ThinOutsetFrame());
	bottom_left.AddFrame(OutsetFrame());
	bottom_left.AddFrame(ThinOutsetFrame());

	BtnClose.SetLabel("E&xit").BottomPos(10,20).HCenterPos();
	bottom_left.Add(BtnClose);
	BtnClearSheet.SetLabel("C&learSheet").BottomPos(40,20).HCenterPos();
	bottom_left.Add(BtnClearSheet);
	BtnBGColor.SetLabel("Back&groundColor").TopPos(30,20).HCenterPos(90,0);
	bottom_left.Add(BtnBGColor);
	//SizePos means set the size from canvas equal to the parent size
	//even when the parent will be resized
	bottom_right.Add(canvas.SizePos());

	//Userevents
	// Quits the program
	BtnClose.WhenAction			=THISBACK(BtnCloseClick);
	//Clear the Sheet only
	BtnClearSheet.WhenAction=THISBACK(BtnClearSheetClick);
	//Let you choose a background color
	BtnBGColor.WhenAction		=THISBACK(BtnBGColorClick);

	//If colorPopUp is modal no callback is needed to get the color, but how?
	colorPopUp.WhenSelect		=THISBACK(OnColorSelect);
	// Now canvas can be painted without altering Canvas class
	canvas.OnCanvasPaint		=THISBACK(OnCanvasPaint);
	// The left mousebutton click is now routed to the canvas
	canvas.OnMouseLeft  		=THISBACK(OnCanvasMouseLeft);
	BackPaint(); // Avoids flickering
	this -> Sizeable().Zoomable();
}

void App::BtnCloseClick()
{
  Close();
}

void App::BtnClearSheetClick()
{
  canvas.ClearSheet();
}

void App::BtnBGColorClick()
{
  colorPopUp.PopUp(&BtnBGColor);
}

void App::OnColorSelect()
{
  canvas.BGColor(colorPopUp.Get());
}

void App::OnCanvasPaint(Draw& w)
{
//paint a grid
  Rect r=canvas.GetSize();
  const int nGrid=10;
  int hr				=(r.Width()	-r.left)%nGrid;
  int vr				=(r.Height()-r.top)%nGrid;
  int width			=(r.Width()	-r.left)-hr;
  int height		=(r.Height()-r.top)-vr;
  hr					 /=2;
  vr				   /=2;
  int gridwidth =width/nGrid;
  int gridheight=height/nGrid;

	//horizontal
	w.DrawLine(hr,vr,r.Width()-hr,vr,1,Black());
	for(int x=1; x<nGrid+2; ++x){
		w.DrawLine(r.left+hr,x*gridheight+vr,r.Width()-hr,x*gridheight+vr,1,Black());
	}
	//vertical
	w.DrawLine(hr,vr,hr,r.Height()-vr,1,Black());
	for(int y=1; y<nGrid+2; ++y){
		w.DrawLine(y*gridwidth+hr,r.top+vr,y*gridwidth+hr,r.Height()-vr,1,Black());
	}
}

void App::OnCanvasMouseLeft(Point p, dword keyflags)
{ //Paint an ugly ellipse:-)
  Rect r=canvas.GetSize();
  DrawingDraw& ds=canvas.DrawSheet();
  ds.DrawEllipse(p.x,p.y,20,25,Red(),5,Green());
  canvas.EndSheet();//draw it now
}

GUI_APP_MAIN
{
	App().Run();
}
