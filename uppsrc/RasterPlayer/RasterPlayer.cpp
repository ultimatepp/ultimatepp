#include <CtrlLib/CtrlLib.h>
#include <RasterPlayer/RasterPlayer.h>

using namespace Upp;

Image GetRect(const Image& orig, const Rect &r) {
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
	return ib;
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
			iw.DrawImage(r, previous);
			previous = Null;
		}
		raster->SeekPage(i);
		r = raster->GetPageRect(i);
		switch (raster->GetPageDisposal(i)) {
		case 1:
		case 2:	iw.DrawRect(r, White());
				break;
		//case 2: iw.DrawRect(sz, White());		// It seems files do no comply with standard
		//		break;
		case 4:	if (i > 0) 
					previous = ::GetRect(images[i-1], r);
				iw.DrawRect(sz, White());
				break;
		}
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

void RasterPlayerThread(RasterPlayer *animatedClip) {
	TimeStop t;
	dword tFrame = 0;
	while (!animatedClip->kill) {
		INTERLOCKED_(mutex) {
			int ind = animatedClip->ind+1;
			if (ind > animatedClip->GetPageCount()-1)
				ind = 0;
			tFrame += dword(animatedClip->delays[ind]/animatedClip->speed);
		}
		while (t.Elapsed() < tFrame)
			Sleep(10);
		PostCallback(callback(animatedClip, &RasterPlayer::NextFrame));
	}
	INTERLOCKED_(mutex) {
		animatedClip->running = false;
	}
}

void RasterPlayer::Play() {
	if (images.GetCount() <= 1)
		return;
	INTERLOCKED_(mutex) {
		running = true;
		kill = false;
	}
	Thread().Run(callback1(RasterPlayerThread, this));
}

void RasterPlayer::Stop() {
	INTERLOCKED_(mutex) {
        kill = true;
    }
	while (running)
		Sleep(10);	
}

RasterPlayer::~RasterPlayer() {
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
