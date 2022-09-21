#include "CtrlCore.h"

#define LLOG(x)

namespace Upp {

Ctrl::Frame Ctrl::AllocFrames(int alloc)
{
	Frame m;
	size_t sz0 = alloc * sizeof(Frame);
	size_t sz = sz0;
	m.frames = (Frame *)MemoryAllocSz(sz);
//	memset(m.frames, 0, sz); _DBG_
	m.multi.alloc = alloc + (int)((sz - sz0) / sizeof(Frame));
	return m;
}

void Ctrl::InsertFrame(int i, CtrlFrame& fr)
{
	GuiLock __;
	int fc = GetFrameCount();
	ASSERT(i <= fc);
	if(i == 0 && fc == 0) {
		frame.frame = &fr;
		multi_frame = false;
	}
	else {
		if(!multi_frame) {
			Frame h = AllocFrames(2);
			h.frames[0].frame = frame.frame;
			h.multi.count = 1;
			frame = h;
			multi_frame = true;
		}
		if(frame.multi.count + 1 > frame.multi.alloc) {
			Frame h = AllocFrames(3 * frame.multi.count / 2 + 1);
			memcpy(h.frames, frame.frames, i * sizeof(Frame));
			memcpy(h.frames + i + 1, frame.frames + i, (frame.multi.count - i) * sizeof(Frame));
			h.multi.count = frame.multi.count;
			FreeFrames();
			frame = h;
		}
		else
			memmove(frame.frames + i + 1, frame.frames + i, (frame.multi.count - i) * sizeof(Frame));
		frame.frames[i].frame = &fr;
		frame.frames[i].SetView(Null);
		frame.multi.count++;
	}
	fr.FrameAdd(*this);
	SyncLayout();
	RefreshFrame();
}

Ctrl& Ctrl::AddFrame(CtrlFrame& fr) {
	InsertFrame(GetFrameCount(), fr);
	return *this;
}

void Ctrl::RemoveFrame(int i) {
	ASSERT(i < GetFrameCount());
	GetFrame(i).FrameRemove();
	if(multi_frame) {
		ASSERT(frame.multi.count > 1);
		memmove(frame.frames + i, frame.frames + i + 1, (frame.multi.count - i - 1) * sizeof(Frame));
		frame.multi.count--;
		if(frame.multi.count == 1) {
			CtrlFrame *h = frame.frames[0].frame;
			FreeFrames();
			multi_frame = false;
			frame.frame = h;
		}
		else
		if(3 * frame.multi.count < frame.multi.alloc) {
			Frame h = AllocFrames(3 * frame.multi.count / 2 + 1);
			memcpy(h.frames, frame.frames, frame.multi.count * sizeof(Frame));
			h.multi.count = frame.multi.count;
			FreeFrames();
			frame = h;
		}
	}
	else
		frame.frame = nullptr;
	if(GetFrameCount() == 0)
		SetFrame(NullFrame());
	SyncLayout();
	RefreshFrame();
}

Ctrl& Ctrl::SetFrame(int i, CtrlFrame& fr) {
	GuiLock __;
	LLOG("SetFrame " << typeid(fr).name());
	while(GetFrameCount() <= i)
		AddFrame(NullFrame());
	Frame& f = GetFrame0(i);
	f.frame->FrameRemove();
	f.frame = &fr;
	fr.FrameAdd(*this);
	SyncLayout();
	RefreshFrame();
	return *this;
}

void Ctrl::ClearFrames() {
	GuiLock __;
	int n = GetFrameCount();
	for(int i = 0; i < n; i++)
		GetFrame(i).FrameRemove();
	FreeFrames();
	multi_frame = false;
	frame.frame = &NullFrame();
	SyncLayout();
	RefreshFrame();
}

int  Ctrl::FindFrame(CtrlFrame& frm) const
{
	GuiLock __;
	int n = GetFrameCount();
	for(int i = 0; i < n; i++)
		if(&GetFrame(i) == &frm)
			return i;
	return -1;
}

void Ctrl::RemoveFrame(CtrlFrame& frm)
{
	GuiLock __;
	int i = FindFrame(frm);
	if(i >= 0)
		RemoveFrame(i);
}

};