#include "PlotCtrl.h"

#define IMAGECLASS PlotCursors
#define IMAGEFILE <PlotCtrl/cursors.iml>
#include <Draw/iml.h>

PlotCtrl::PlotCtrl():tr(*this),mpos(Null),cursor(PC_ARROW){
	tr.sync=THISBACK(TrackerSync);
	BackPaint();
}
PlotCtrl& PlotCtrl::Zoom(double rx,double ry,Pointf C){
	//OverrideCursor(Image::Wait());
	cursor=PC_WAIT;
	Rectf g=GetLimits();
	if(C.IsNullInstance()){C=g.CenterPoint();}
	SetLimits(RectfC(C.x-rx*(C.x-g.left),C.y-ry*(C.y-g.top),rx*g.Width(),ry*g.Height()));
	SetModify();
	Refresh();
	Sync();
	MouseMove(GetMousePos(),0);
	return *this;
}
PlotCtrl& PlotCtrl::ZoomIn(double ratio,const Pointf& C){
	return Zoom(1/ratio,1/ratio,C);
}
PlotCtrl& PlotCtrl::ZoomOut(double ratio,const Pointf& C){
	return Zoom(ratio,ratio,C);
}
PlotCtrl& PlotCtrl::ZoomAll(bool visibleonly){
	//OverrideCursor(Image::Wait());
	cursor=PC_WAIT;
	Rectf bb=BoundingBox(visibleonly);DUMP(bb);
	if(bb.Width()==0||bb.Height()==0){
		SetLimits(-1,1,-1,1);LOG("empty");
	}else{
		SetLimits(bb);LOG("not empty");
	}
	SetModify();
	Refresh();
	Sync();
	MouseMove(GetMousePos(),0);
	return *this;
}
void PlotCtrl::Layout(){
	SetPlotSize(GetSize());
	SetModify();
}
void PlotCtrl::Paint(Draw& w){
	if(IsModified()){
		WhenSync();
		img=GetImage();
		if(img.IsEmpty()){
			SetLimits(-1,1,-1,1);
			PromptOK("You've reached precision limits.&Zoom setting was reset.");
			Refresh();
			Sync();
			return;
		}else{
			ClearModify();
		}
	}
	w.DrawImage(0,0,img);
}
void PlotCtrl::LeftDown(Point pt,dword keyflags){
	if(keyflags&K_SHIFT){
		MiddleDown(pt,keyflags);
	}else if(keyflags&K_CTRL){
		ZoomAll();
	}else{
		if(tr.InLoop()){tr.Close();tr.OverrideCursor(Null);} //sometimes RectTracker "forgets" to leave the loop...
		Rectf r=tr.Track(RectfC(pt.x,pt.y,0,0),ALIGN_NULL,ALIGN_NULL);
		tr.OverrideCursor(Null);
		if(r.IsEmpty()){
			ZoomIn(2,ImgToPlot(pt));
		}else{
			SetLimits(ImgToPlot(r));
		}
		//OverrideCursor(PlotCursors::cross);
		if(CheckRange()){
			SetModify();
			Refresh();
			Sync();
		}
	}
}
void PlotCtrl::MouseMove(Point pt, dword keyflags){
	if(!mpos.IsNullInstance()){
		SetLimits(lim); //this is to make ImgToPlot conversion work correctly
		SetLimits(lim.Offseted(mpos-ImgToPlot(pt)));
		SetModify();
		//OverrideCursor(PlotCursors::pan);
		cursor=PC_PAN;
		Refresh();Sync();
		return;
	}
	String pos=ImgToPlotFormatted(pt);
	ImageDraw w(15+StrWidth(pos),15+GetFont().GetHeight());
	w.DrawRect(0,0,15+StrWidth(pos),15+GetFont().GetHeight(),Black());
	w.Alpha().DrawRect(0,0,15+StrWidth(pos),15+GetFont().GetHeight(),GrayColor(0));
	w.Alpha().DrawImage(0,0,PlotCursors::cross,GrayColor(255));
	w.Alpha().DrawText(11,11,pos,GetFont(),GrayColor(255));
	ImageBuffer b(w);
	b.SetHotSpot(Point(8,8));
	cimg=b;
	cursor=PC_LOCAL;
}
void PlotCtrl::RightDown(Point pt,dword keyflags){
	Pointf pf(1.0*pt.x,1.0*pt.y);
	ZoomOut(2,ImgToPlot(pt));
	if(!CheckRange()){
		SetModify();
		PromptOK("Oups, you'have reached double precision limits...&Plot changed its zoom settings to avoid freezing application.");
	}
}
void PlotCtrl::MouseWheel(Point pt, int zdelta, dword keyflags){
	if(IsModified()) return; // only accept event if ready
	if(zdelta<0){ZoomOut(-zdelta/90.0,ImgToPlot(pt));}
	else{ZoomIn(zdelta/90.0,ImgToPlot(pt));}
}
void PlotCtrl::MiddleDown(Point pt,dword keyflags){
	mpos=ImgToPlot(pt);
	lim=GetLimits();
	cursor=PC_PAN;
}
void PlotCtrl::MiddleUp(Point pt,dword keyflags){
	mpos=Null;
	MouseMove(pt,keyflags);
}
void PlotCtrl::LeftUp(Point pt,dword keyflags){
	mpos=Null;
	MouseMove(pt,keyflags);
}
void PlotCtrl::MouseLeave(){
	mpos=Null;
}

void PlotCtrl::TrackerSync(Rect r){
	String pos=ImgToPlotFormatted(r);
	Rect t(0,0,15+StrWidth(pos),15+2*GetFont().GetHeight());
	ImageDraw w(t.Width(),t.Height());
	w.DrawRect(t,Black());
	w.Alpha().DrawRect(t,GrayColor(0));
	w.Alpha().DrawImage(0,0,PlotCursors::cross,GrayColor(255));
	w.Alpha().DrawText(11,11,pos,GetFont(),GrayColor(255));
	ImageBuffer b(w);
	b.SetHotSpot(Point(8,8));
	tr.OverrideCursor(b);
}

Image PlotCtrl::CursorImage(Point p, dword keyflags) {
	switch (cursor) {
		case PC_LOCAL: if(GetShift()) return PlotCursors::pan(); else return cimg;
		case PC_PAN:   return PlotCursors::pan();
		case PC_CROSS: return PlotCursors::cross();
		case PC_ARROW: return Image::Arrow();
		case PC_WAIT:  return Image::Wait();
	}
	return Image();
}