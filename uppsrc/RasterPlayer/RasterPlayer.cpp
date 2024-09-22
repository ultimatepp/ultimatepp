#include <CtrlLib/CtrlLib.h>
#include <RasterPlayer/RasterPlayer.h>

namespace Upp {

Image GetRect_(const Image& orig, const Rect &r) {
	if(r.IsEmpty())
		return Image();
	ImageBuffer ib(r.GetSize());
	for(int y = r.top; y < r.bottom; y++) {
		const RGBA *s = orig[y] + r.left;
		const RGBA *e = orig[y] + r.right;
		RGBA *t = ib[y - r.top];
		while(s < e) {
			*t = *s;
			t++;
			s++;
		}
	}
	return Image(ib);
}

Rect Fit(const Size &frame, const Size &object) {
	double objectAspect = object.cx/(double)object.cy;		
	if (frame.cx/(double)frame.cy > objectAspect) {
		double x = (frame.cx - objectAspect*frame.cy)/2.;
		return Rect(int(x), 0, int(x + objectAspect*frame.cy), frame.cy);
	} else {
		double y = (frame.cy - frame.cx/objectAspect)/2.;
		return Rect(0, int(y), frame.cx, int(y + frame.cx/objectAspect));
	}
}

RasterPlayer::RasterPlayer() {
	Transparent();
	NoWantFocus();
	background = Null;
	running = 0;
	kill = 1;
	speed = 1;
	mt = false;
}

void RasterPlayer::Paint(Draw& w) {
	Size sz = GetSize();
	if (!IsNull(background))
		w.DrawRect(sz, background);
	if (images.IsEmpty())
		return;
	if (!images[ind]) 
		return;
	w.DrawImage(Fit(sz, images[ind].GetSize()), images[ind]);
}

static StaticCriticalSection mutex;

bool RasterPlayer::LoadBuffer(const String &buffer) {
	if (buffer.IsEmpty())
		return false;
	StringStream str(buffer);
	One<StreamRaster> raster = StreamRaster::OpenAny(str);
	if(!raster) 
		return false;
	
	Stop();	
	images.Clear();
	delays.Clear();	
	
	Size sz = raster->GetSize();
	ImageDraw iw(sz); 
	Image previous;
	Rect r;
	for (int i = 0; i < raster->GetPageCount(); ++i) {
		if (previous) {
			iw.Alpha().DrawImage(r, previous, GrayColor(255));
			iw.DrawImage(r, previous);
			previous = Null;
		}
		raster->SeekPage(i);
		r = raster->GetPageRect(i);
		switch (raster->GetPageDisposal(i)) {
		case 0:	
		case 1:	break;
		case 2: iw.DrawRect(sz, SColorFace());	  
				break;
		case 3:	if (i > 0) 
					previous = GetRect_(images[i-1], r);
				iw.DrawRect(sz, SColorFace()); 
				break;
		}
		iw.Alpha().DrawImage(r.left, r.top, raster->GetImage(0, 0, r.right-r.left, r.bottom-r.top), GrayColor(255));
		iw.DrawImage(r.left, r.top, raster->GetImage(0, 0, r.right-r.left, r.bottom-r.top));
		images.Add(iw);
		delays.Add(max(50, 10*raster->GetPageDelay(i)));
	}
	ind = 0;
	return true;	
}

bool RasterPlayer::Load(const String &fileName) {
	FileIn in(fileName);
	return LoadBuffer(LoadStream(in));
}

bool RasterPlayer::IsKilled() {
	bool ret;
	INTERLOCKED_(mutex) {ret = kill;}
	return ret;	
}

#ifdef _MULTITHREADED
void RasterPlayerThread(RasterPlayer *animatedClip) {
	TimeStop t;
	double tFrame_ms = 0;
	while (!animatedClip->IsKilled()) {
		INTERLOCKED_(mutex) {
			int ind = animatedClip->ind + 1;
			if (ind > animatedClip->GetPageCount() - 1)
				ind = 0;
			tFrame_ms = animatedClip->delays[ind]/animatedClip->speed;
		}
		while (t.Elapsed()/1000. < tFrame_ms && !animatedClip->IsKilled())
			Sleep(10);
		t.Reset();
		PostCallback(callback(animatedClip, &RasterPlayer::NextFrame));
	}
INTERLOCKED_(mutex) {
	animatedClip->running = false;
}
}
#endif

void RasterPlayer::TimerFun() {
INTERLOCKED_(mutex) {
	if (kill || !running)
		return;
}
	if (tTime.Elapsed()/1000. < tFrame_ms)
		return;	 
	tTime.Reset();
	int iFrame = ind + 1;
	if (iFrame > GetPageCount() - 1)
		iFrame = 0;
	tFrame_ms = delays[iFrame]/speed;
	NextFrame();
}

void RasterPlayer::Play() {
	if (images.GetCount() <= 1)
		return;
INTERLOCKED_(mutex) {
	running  = true;
	kill = false;
}
#ifdef _MULTITHREADED
	if (mt)
		Thread().Run(callback1(RasterPlayerThread, this));
	else
#endif
	{
		tFrame_ms = 0;
		tTime.Reset();
		SetTimeCallback(-50, callback(this, &RasterPlayer::TimerFun), 1);
	}
}

void RasterPlayer::Stop() {
INTERLOCKED_(mutex) {kill = true;}
#ifdef _MULTITHREADED
	if (mt) {
		while (running)
			Sleep(10);	
	} else 
#endif
	{
		KillTimeCallback(1);
		INTERLOCKED_(mutex) {running = false;}
	}
}

RasterPlayer& RasterPlayer::SetMT(bool _mt)	{
#ifndef _MULTITHREADED
	return *this;
#endif
	bool wasrunning;
	INTERLOCKED_(mutex) {
		wasrunning = running;
	}
	Stop(); 
	mt = _mt; 
	if (wasrunning)
		Play();
	return *this;
}

RasterPlayer::~RasterPlayer() noexcept {
	Stop();	
}

void RasterPlayer::NextFrame() {
	if (images.GetCount() <= 1)
		return;
	ind++;
	if (ind >= images.GetCount())
		ind = 0;
	if (WhenShown)
		WhenShown();
	Refresh();
}

}