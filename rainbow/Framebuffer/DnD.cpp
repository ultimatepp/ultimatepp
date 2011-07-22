#include "Fb.h"

#ifdef GUI_FB

NAMESPACE_UPP

#define LLOG(x)  DLOG(x)

// --------------------------------------------------------------------------------------------

Ptr<Ctrl> sDnDSource;

Ctrl * Ctrl::GetDragAndDropSource()
{
	return sDnDSource;
}

struct DnDLoop : LocalLoop {
	Image move, copy, reject;
	const VectorMap<String, ClipData> *data;
	Ptr<Ctrl> source;
	int    action;

	void Sync();
	String GetData(const String& f);
	void Leave();

	virtual void  LeftUp(Point, dword);
	virtual bool  Key(dword, int);
	virtual void  MouseMove(Point p, dword);
	virtual Image CursorImage(Point, dword);
};

Ptr<DnDLoop> dndloop;

void DnDLoop::Leave()
{
	GuiLock __; 
}

void DnDLoop::Sync()
{
	GuiLock __; 
}

void DnDLoop::LeftUp(Point, dword)
{
	GuiLock __; 
	LLOG("DnDLoop::LeftUp");
/*	if(target) {
		LLOG("Sending XdndDrop to " << target);
		XEvent e = ClientMsg(target, XdndDrop);
		e.xclient.data.l[0] = src;
		e.xclient.data.l[1] = 0;
		e.xclient.data.l[2] = Xeventtime;
		Xdnd_waiting_finished = true;
		XSendEvent(Xdisplay, target, XFalse, 0, &e);
		XFlush(Xdisplay);
		int timeout = GetTickCount();
		LLOG("Waiting for XdndFinished");
		while(Xdnd_waiting_finished && GetTickCount() - timeout < 200) {
			GuiSleep(0);
			ProcessEvents();
		}
		LLOG("Waiting finished " << GetTickCount() - timeout << "ms");
		if(Xdnd_waiting_status) {
			LLOG("XdndFinished timeout");
			Xdnd_status = DND_NONE;
			Xdnd_waiting_finished = false;
		}
		else
			LLOG("XdndFinished recieved");
	}*/
	EndLoop();
}

void DnDLoop::MouseMove(Point p, dword)
{
	GuiLock __; 
	LLOG("DnDLoop::MouseMove");
	Sync();
}

bool DnDLoop::Key(dword, int)
{
	GuiLock __; 
	LLOG("DnDLoop::Key");
	Sync();
	return false;
}

Image DnDLoop::CursorImage(Point, dword)
{
	GuiLock __; 
	return action == DND_MOVE ? move : action == DND_COPY ? copy : reject;
}

String DnDLoop::GetData(const String& f)
{
	GuiLock __; 
	int i = data->Find(f);
	String d;
	if(i >= 0)
		d = (*data)[i].Render();
	else
		if(source)
			d = source->GetDropData(f);
	return d;
}

int Ctrl::DoDragAndDrop(const char *fmts, const Image& sample, dword actions,
                        const VectorMap<String, ClipData>& data)
{
	GuiLock __; 
	DnDLoop d;
	PNGEncoder().SaveFile("U://sample.png", sample); _DBG_
	d.reject = actions & DND_EXACTIMAGE ? CtrlCoreImg::DndNone() : MakeDragImage(CtrlCoreImg::DndNone(), sample);
	PNGEncoder().SaveFile("U://reject.png", d.reject); _DBG_
	if(actions & DND_COPY)
		d.copy = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndCopy(), sample);
	if(actions & DND_MOVE)
		d.move = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndMoveX11(), sample);
	d.SetMaster(*this);
	d.data = &data;
	d.source = this;
	dndloop = &d;
	sDnDSource = this;
	d.Run();
	sDnDSource = NULL;
	SyncCaret();
	LLOG("DoDragAndDrop finished");
	return d.action;
}

void Ctrl::SetSelectionSource(const char *fmts) {}

END_UPP_NAMESPACE

#endif
