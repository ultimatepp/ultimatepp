#include "CtrlCore.h"

#ifdef GUI_X11

#define LLOG(x)   // DLOG(x)

namespace Upp {

bool Xdnd_waiting_status;
bool Xdnd_waiting_finished;
int  Xdnd_status;
int  Xdnd_version;

static Atom XdndEnter;
static Atom XdndPosition;
static Atom XdndLeave;
static Atom XdndDrop;
static Atom XdndStatus;
static Atom XdndFinished;
static Atom XdndActionCopy;
static Atom XdndActionMove;

void InitDndAtoms()
{
	ONCELOCK {
		XdndEnter = XAtom("XdndEnter");
		XdndPosition = XAtom("XdndPosition");
		XdndLeave = XAtom("XdndLeave");
		XdndDrop = XAtom("XdndDrop");
		XdndStatus = XAtom("XdndStatus");
		XdndFinished = XAtom("XdndFinished");
		XdndActionCopy = XAtom("XdndActionCopy");
		XdndActionMove = XAtom("XdndActionMove");
	}
}

XEvent ClientMsg(Window src, Atom type, int format = 32)
{
	XEvent e;
	Zero(e);
	e.xclient.type = ClientMessage;
	e.xclient.display = Xdisplay;
	e.xclient.window = src;
	e.xclient.message_type = type;
	e.xclient.format = format;
	return e;
}

struct DnDLoop : LocalLoop {
	Image move, copy, reject;
	Vector<Atom> fmt;
	const VectorMap<String, ClipData> *data;
	Ptr<Ctrl> source;
	void SetFmts(Window w, Atom property);
	Window src, target;
	int    action;

	void Request(XSelectionRequestEvent *se);
	void Sync();
	Value  GetData() const { return LocalLoop::GetData(); } // Silence CLANG warning
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
	if(target) {
		LLOG("Sending XdndLeave to " << target);
		XEvent e = ClientMsg(target, XdndLeave);
		e.xclient.data.l[0] = src;
		XSendEvent(Xdisplay, target, XFalse, 0, &e);
	}
}

void DnDLoop::Sync()
{
	GuiLock __; 
	if(Xdnd_waiting_status || Xdnd_waiting_finished)
		return;
	bool tx = Ctrl::TrapX11Errors();
	Window root;
	unsigned int d1;
	int x, y, d2;
	Window tgt = Xroot;
	for(;;) {
		if(XQueryPointer(Xdisplay, tgt, &root, &tgt, &x, &y, &d2, &d2, &d1)) {
			if(!tgt)
				break;
			Vector<int> x = GetPropertyInts(tgt, XAtom("XdndAware"));
			LLOG("XdndAware " << tgt << ": " << x.GetCount());
			if(x.GetCount()) {
				Xdnd_version = x[0];
				break;
			}
		}
		else {
			tgt = None;
			break;
		}
	}
	if(Xdnd_version < 3)
		tgt = None;
	if(tgt != target) {
		Leave();
		target = tgt;
		if(target) {
			LLOG("Sending XdndEnter to " << target << ", src = " << src);
			XEvent e = ClientMsg(target, XdndEnter);
			e.xclient.data.l[0] = src;
			e.xclient.data.l[1] = (fmt.GetCount() > 3) | (Xdnd_version << 24);
			for(int i = 0; i < min(3, fmt.GetCount()); i++)
				e.xclient.data.l[i + 2] = fmt[i];
			XSendEvent(Xdisplay, target, XFalse, 0, &e);
		}
	}

	if(target) {
		LLOG("Sending XdndPosition to " << target << " " << x << ", " << y);
		XEvent e = ClientMsg(target, XdndPosition);
		e.xclient.data.l[0] = src;
		e.xclient.data.l[1] = 0;
		e.xclient.data.l[2] = MAKELONG(y, x);
		e.xclient.data.l[3] = Xeventtime;
		int action = XdndActionCopy;
		if(source && source->GetTopCtrl()->GetWindow() == target)
			action = XdndActionMove;
		if(GetShift())
			action = XdndActionMove;
		if(GetCtrl())
			action = XdndActionCopy;
		e.xclient.data.l[4] = action;
		XSendEvent(Xdisplay, target, XFalse, 0, &e);
		XFlush(Xdisplay);
		Xdnd_waiting_status = true;
		dword timeout = msecs();
		LLOG("Waiting for XdndStatus");
		Xdnd_status = DND_NONE;
		while(Xdnd_waiting_status && msecs() - timeout < 200) {
			GuiSleep(0);
			ProcessEvents();
		}
		LLOG("Waiting status " << msecs() - timeout << "ms");
		if(Xdnd_waiting_status) {
			LLOG("XdndStatus timeout");
			Xdnd_status = DND_NONE;
			Xdnd_waiting_status = false;
		}
		else
			LLOG("XdndStatus recieved " << Xdnd_status);
	}
	Ctrl::UntrapX11Errors(tx);
}

void Ctrl::DropStatusEvent(XEvent *event)
{
	GuiLock __; 
	InitDndAtoms();
	if(event->type != ClientMessage)
		return;
	LLOG("DropStatus Client Message " << XAtomName(event->xclient.message_type));
	if(event->type == ClientMessage && dndloop && event->xclient.data.l[0] == (int)dndloop->target) {
		if(event->xclient.message_type == XdndStatus && Xdnd_waiting_status) {
			LLOG("XdndStatus, xdnd action: " << XAtomName(event->xclient.data.l[4]));
			if(Xdnd_status == DND_NONE)
				Xdnd_status = (event->xclient.data.l[1] & 1) ?
				                 event->xclient.data.l[4] == (int)XdndActionMove ? DND_MOVE : DND_COPY
				              : DND_NONE;
			Xdnd_waiting_status = false;
		}
		if(event->xclient.message_type == XdndFinished && Xdnd_waiting_finished) {
			LLOG("XdndFinished, xdnd action: " << XAtomName(event->xclient.data.l[2]));
			if(Xdnd_version == 5)
				Xdnd_status = (event->xclient.data.l[1] & 1) ?
				                 event->xclient.data.l[2] == (int)XdndActionMove ? DND_MOVE : DND_COPY
				              : DND_NONE;
			Xdnd_waiting_finished = false;
		}
	}
}

void DnDLoop::LeftUp(Point, dword)
{
	GuiLock __; 
	LLOG("DnDLoop::LeftUp");
	bool tx = TrapX11Errors();
	if(target) {
		LLOG("Sending XdndDrop to " << target);
		XEvent e = ClientMsg(target, XdndDrop);
		e.xclient.data.l[0] = src;
		e.xclient.data.l[1] = 0;
		e.xclient.data.l[2] = Xeventtime;
		Xdnd_waiting_finished = true;
		XSendEvent(Xdisplay, target, XFalse, 0, &e);
		XFlush(Xdisplay);
		int timeout = msecs();
		LLOG("Waiting for XdndFinished");
		while(Xdnd_waiting_finished && msecs() - timeout < 200) {
			GuiSleep(0);
			ProcessEvents();
		}
		LLOG("Waiting finished " << msecs() - timeout << "ms");
		if(Xdnd_waiting_status) {
			LLOG("XdndFinished timeout");
			Xdnd_status = DND_NONE;
			Xdnd_waiting_finished = false;
		}
		else
			LLOG("XdndFinished recieved");
	}
	EndLoop();
	UntrapX11Errors(tx);
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
	return Xdnd_status == DND_MOVE ? move : Xdnd_status == DND_COPY ? copy : reject;
}

void DnDLoop::SetFmts(Window w, Atom property)
{
	GuiLock __; 
	Buffer<Atom> x(fmt.GetCount());
	for(int i = 0; i < fmt.GetCount(); i++) {
		x[i] = fmt[i];
		LLOG('\t' << XAtomName(x[i]));
	}
	XChangeProperty(Xdisplay, w, property, XAtom("ATOM"),
	                32, 0, (unsigned char*)~x,
	                fmt.GetCount());
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

String DnDGetData(const String& f)
{
	GuiLock __; 
	String d;
	if(dndloop)
		d = dndloop->GetData(f);
	return d;
}

void DnDLoop::Request(XSelectionRequestEvent *se)
{
	GuiLock __; 
	LLOG("DnDRequest " << XAtomName(se->target));
	XEvent e;
	e.xselection.type      = SelectionNotify;
	e.xselection.display   = Xdisplay;
	e.xselection.requestor = se->requestor;
	e.xselection.selection = XAtom("XdndSelection");
	e.xselection.target    = se->target;
	e.xselection.time      = se->time;
	e.xselection.property  = se->property;
	if(se->target == XAtom("TARGETS")) {
		LLOG("DnDRequest targets:");
		SetFmts(se->requestor, se->property);
	}
	else {
		String d = GetData(XAtomName(se->target));
		if(d.GetCount())
			XChangeProperty(Xdisplay, se->requestor, se->property, se->target, 8, PropModeReplace,
			                d, d.GetCount());
		else
		    e.xselection.property = None;
	}
	XSendEvent(Xdisplay, se->requestor, XFalse, 0, &e);
}

void DnDRequest(XSelectionRequestEvent *se)
{
	GuiLock __; 
	if(dndloop) dndloop->Request(se);
}

void DnDClear() {}

void GuiPlatformAdjustDragImage(ImageBuffer& b);

Image MakeDragImage(const Image& arrow, Image sample);

Ptr<Ctrl> sDnDSource;

int Ctrl::DoDragAndDrop(const char *fmts, const Image& sample, dword actions,
                        const VectorMap<String, ClipData>& data)
{
	GuiLock __; 
	InitDndAtoms();
	DnDLoop d;
	Xdnd_waiting_status =  Xdnd_waiting_finished = false;
	d.reject = actions & DND_EXACTIMAGE ? CtrlCoreImg::DndNone() : MakeDragImage(CtrlCoreImg::DndNone(), sample);
	if(actions & DND_COPY)
		d.copy = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndCopy(), sample);
	if(actions & DND_MOVE)
		d.move = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndMoveX11(), sample);
	d.SetMaster(*this);
	d.data = &data;
	d.source = this;
	dndloop = &d;
	Vector<String> f = Split(fmts, ';');
	for(int i = 0; i < f.GetCount(); i++)
		d.fmt.Add(XAtom(f[i]));
	for(int i = 0; i < data.GetCount(); i++)
		d.fmt.Add(XAtom(data.GetKey(i)));
	d.SetFmts(xclipboard().win, XAtom("XdndTypeList"));
	XSetSelectionOwner(Xdisplay, XAtom("XdndSelection"), xclipboard().win, CurrentTime);
	d.src = xclipboard().win;
	d.target = None;
	sDnDSource = this;
	d.Run();
	sDnDSource = NULL;
	SyncCaret();
	LLOG("DoDragAndDrop finished");
	return Xdnd_status;
}

Ctrl *Ctrl::GetDragAndDropSource()
{
	GuiLock __; 
	return sDnDSource;
}

Index<String>    Ctrl::drop_formats;

int   XdndAction;
Point XdndPos;

PasteClip sMakeDropClip(bool paste)
{
	PasteClip d;
	d.type = 1;
	d.paste = paste;
	d.accepted = false;
	d.allowed = DND_MOVE|DND_COPY;
	d.action = XdndAction;
	return d;
}

void Ctrl::DnD(Window src, bool paste)
{
	GuiLock __; 
	PasteClip d = sMakeDropClip(paste);
	LLOG("Source action " << XdndAction);
	DnD(XdndPos, d);
	XdndAction = d.GetAction();
	LLOG("Target action " << XdndAction);
	XEvent e = ClientMsg(src, paste ? XdndFinished : XdndStatus);
	e.xclient.data.l[0] = GetWindow();
	e.xclient.data.l[paste ? 2 : 4] = XdndAction == DND_MOVE ? XdndActionMove : XdndActionCopy;
	if(d.IsAccepted())
		e.xclient.data.l[1] = 1;
	LLOG("Sending status/finished to " << src << " accepted: " << d.IsAccepted());
	XSendEvent(Xdisplay, src, XFalse, 0, &e);
}

void Ctrl::DropEvent(XWindow& w, XEvent *event)
{
	GuiLock __; 
	InitDndAtoms();
	if(event->type != ClientMessage)
		return;
	Window src = event->xclient.data.l[0];
	LLOG("Client Message " << GetWindow() << " " << XAtomName(event->xclient.message_type)
	      << ", src: " << src);
	if(event->xclient.message_type == XdndEnter) {
		LLOG("DnDEnter");
		drop_formats.Clear();
		if(event->xclient.data.l[1] & 1) {
			Vector<int> v = GetPropertyInts(src, XAtom("XdndTypeList"), XA_ATOM);
			for(int i = 0; i < v.GetCount(); i++)
				drop_formats.Add(XAtomName(v[i]));
		}
		else
			for(int i = 2; i <= 4; i++)
				drop_formats.Add(XAtomName(event->xclient.data.l[i]));
	}
	static Point xdndpos;
	if(event->xclient.message_type == XdndPosition) {
		dword x = event->xclient.data.l[2];
		XdndPos = Point(HIWORD(x), LOWORD(x));
		LLOG("XdndPosition " << XdndPos << ", action " << XAtomName(event->xclient.data.l[4]));
		XdndAction = event->xclient.data.l[4] == (int)XdndActionMove ? DND_MOVE : DND_COPY;
		DnD(src, false);
	}
	if(event->xclient.message_type == XdndLeave)
		DnDLeave();
	if(event->xclient.message_type == XdndDrop && dndctrl) {
		LLOG("XdndDrop to " << UPP::Name(dndctrl));
		DnD(src, true);
		DnDLeave();
	}
}

}

#endif
