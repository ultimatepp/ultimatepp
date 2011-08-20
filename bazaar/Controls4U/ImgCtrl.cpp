#include <CtrlLib/CtrlLib.h>

using namespace Upp;

//#include "ImageProcessing.h"
#include "ImgCtrl.h"
#include "SysInfo/SysInfo.h"
#include "Candidates/CandidatesGui.h"

ImgCtrl::ImgCtrl()
{ 
	BackPaint();
	
	x0 = y0 = xDelta = yDelta = 0.;
	
	moving = false;
	lastPoint = Point(0,0);
	
	factor = 0.0;
	width = height = 0;
	
	showZoom = true;
	thumb = true;
}

bool ImgCtrl::IsEmpty()
{
	return width == 0;
}

void ImgCtrl::Paint(Draw& w)
{
	WString strTemp;
	Size sz = GetSize();

	w.DrawRect(GetSize(), White);
	if(imageRep) {
		w.DrawImage((int)x0, (int)y0, imageRep);  
		if(showZoom) {
			strTemp = Format(t_("Zoom: %2.2f"), factor).ToWString();
			w.DrawText(sz.cx-120, sz.cy-13, strTemp, Arial(10));
		}
	}
	if (thumb && width > 0) {
		double rate = 100./width;
		int twidth = 100;
		int theight = int(rate*height); 
		int tx = sz.cx-twidth-20;
		int ty = sz.cy-theight-20;

		Color black = Black();
		DrawRectLine(w, tx-1, ty-1, twidth+1, theight+1, 1, black);	// Borde de la imagen principal
		w.DrawImage(tx, ty, twidth, theight, image);
		double rateRep = 100./imageRep.GetWidth();
		DrawRectLine(w, int(tx-x0*rateRep-1), int(ty-y0*rateRep-1), 
						1+int(twidth*sz.cx/imageRep.GetWidth()), 
						1+int(theight*sz.cy/imageRep.GetHeight()), 1, black);	// Rectangulo con la parte visible de la imagen
	}
}

Size ZoomProportional(Size frame, Size space, double &x0, double &y0, double scale, double step) {
	if (scale == 0) 
		return Null;
	
	int frameWidth = frame.cx;
	int frameHeight  = frame.cy;
	
	Size ret((int)(scale*space.cx), (int)(scale*space.cy));
	
	scale = step;
	x0 = frameWidth*(1-scale)/2  + x0*scale;
	y0 = frameHeight*(1-scale)/2 + y0*scale;

	return ret;
}

void ImgCtrl::Zoom(double m_fStep)
{
	if(factor*m_fStep > MAX_ZOOM_FACTOR || factor*m_fStep < MIN_ZOOM_FACTOR) 
		return;

	factor = factor*m_fStep;
	Size s = ZoomProportional(GetSize(), image.GetSize(), x0, y0, factor, m_fStep);
	
	xDelta = x0;
	yDelta = y0;
	imageRep = Rescale(image, s, Rect(0, 0, width, height));          
	
	Refresh();
}

/*
Rectf ZoomProportional(Size frame, Rectf &image, double step) {
//	if (scale == 0) 
//		return Null;
	
	int frameWidth = frame.cx;
	int frameHeight  = frame.cy;
	
	Rectf ret;
	ret.left = frameWidth*(1-step)/2  + image.left*step;
	ret.top = frameHeight*(1-step)/2 + image.top*step;
	ret.right = ret.left + step*image.GetWidth();
	ret.bottom = ret.top + step*image.GetHeight();

	return ret;
}

void ImgCtrl::Zoom(double m_fStep)
{
	if(m_fFactor*m_fStep > MAX_ZOOM_FACTOR || m_fFactor*m_fStep < MIN_ZOOM_FACTOR) 
		return;

	Rectf r = Rectf(x0, m_p0y, m_fFactor*m_Img.GetWidth(), m_fFactor*m_Img.GetHeight());
	Rectf rr = ZoomProportional(GetSize(), r, m_fStep);
	m_fFactor = m_fFactor*m_fStep;	
	
	m_pUx = x0 = rr.left;
	m_pUy = m_p0y = rr.top;
	m_ImgRep = Rescale(m_Img, Size(rr.GetWidth(), rr.GetHeight()), Rect(0, 0, Width, Height));          
	
	Refresh();
}
*/
// Resize and center to fit in the control
void ImgCtrl::Fit()
{
	if (image) {
		Size szImage = image.GetSize(); 
	
		Rect r = FitInFrame(GetSize(), szImage);
	
		factor = r.GetWidth()/(double)szImage.cx;
		
		xDelta = x0 = r.left;
		yDelta = y0 = r.top;
		imageRep = Rescale(image, Size(r.Width(), r.Height()), Rect(0, 0, szImage.cx, szImage.cy));          
	}
	Refresh();
}
// Resize to real img size
void ImgCtrl::RealSize()
{
	if(image) {
		xDelta = x0 = 0.;
		yDelta = y0 = 0.;
		factor = 1.0;
		imageRep = image;

		Refresh();
	}
}
void ImgCtrl::GetRectRep(double &left, double &top, double &_width, double &_height)
{
	left = x0;
	top = y0;
	_width = imageRep.GetWidth();
	_height = imageRep.GetHeight();
}
void ImgCtrl::GetRepXY(double x, double y, int &repx, int &repy)
{
	repx = int(x*imageRep.GetWidth()/GetWidth() + x0);	
	repy = int(y*imageRep.GetHeight()/GetHeight() + y0);	
}
void ImgCtrl::GetXYRep(int repx, int repy, double &x, double &y)
{
	x = double((repx - xDelta)*GetWidth() - x0)/imageRep.GetWidth();	
	y = double((repy - yDelta)*GetHeight() - y0)/imageRep.GetHeight();	
}
void ImgCtrl::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(zdelta > 0)
		Zoom(1.2);
	else
		Zoom(1/1.2);
}
void ImgCtrl::LocalMenu(Bar& bar)
{
	if(imageRep) {
		bar.Add("Window size", THISBACK(Fit)); 
		bar.Add("Real size", THISBACK(RealSize));	
		bar.Add("B&N", THISBACK(BW));	
	}
}
void ImgCtrl::RightDown(Point p, dword keyflags)
{
	MenuBar::Execute(THISBACK(LocalMenu));
}
void ImgCtrl::LeftDown(Point p, dword keyflags) 
{
	lastPoint = p;
    moving = true;
}
void ImgCtrl::LeftUp(Point p, dword keyflags) 
{
	xDelta = x0;	//Largou-se o rapo, guardar deslocamento onde parou
	yDelta = y0;
    moving = false;
}
void ImgCtrl::MouseMove(Point p, dword keyflags) 
{
	Point pDif;
	
    if(moving) {
         pDif = lastPoint - p;
         
         //Exclamation(Format("Last(%d,%d) Actual(%d,%d), Dif(%d,%d)",m_pLastPoint.x,m_pLastPoint.y,p.x,p.y,pDif.x,pDif.y));
         int deslocX = -pDif.x;
         int deslocY = -pDif.y;
         
         x0 = xDelta + deslocX;
         y0 = yDelta + deslocY;
         
         Refresh();
    }
}
void ImgCtrl::Load(const WString fileName) 
{
    image.Clear();
	
	image = StreamRaster::LoadFileAny(fileName.ToString());
	if(image)
		SetImage(image);
}

bool ImgCtrl::Save(String fileName, String ext, int qualityBpp) 
{
	return SaveImage(image, qualityBpp, fileName, ext);
};

void ImgCtrl::Print(int n_draw_origin_x, int n_draw_origin_y, int n_image_cx, int n_image_cy)
{
	PrintImage(image, n_draw_origin_x, n_draw_origin_y, n_image_cx, n_image_cy);
}

void ImgCtrl::SetImage(ImageBuffer img) {
	image = img; 
	width = image.GetWidth();
	height = image.GetHeight();
	Fit();
};

void ImgCtrl::SetImage(Image img) {
	image = img; 
	width = image.GetWidth();
	height = image.GetHeight();
	Fit();
};

void ImgCtrl::SetImage(const char *fileName) {
	SetImage(StreamRaster::LoadFileAny(fileName));
}

void ImgCtrl::SetZoomVisible(const bool b_show)
{
	showZoom = b_show;
}
void ImgCtrl::SetThumbVisible(bool _thumb)
{
	thumb = _thumb;;
}

void ImgCtrl::BW()
{
	image = Grayscale(image);
	Zoom(1);
}
