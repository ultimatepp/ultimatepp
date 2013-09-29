#include "Local.h"

#ifdef GUI_SDL20GL

NAMESPACE_UPP

#define LLOG(x)  // DLOG(x)

// --------------------------------------------------------------------------------------------

Ptr<Ctrl> sDnDSource;

Ctrl * Ctrl::GetDragAndDropSource()
{
	return sDnDSource;
}

struct DnDLoop : LocalLoop {
	const VectorMap<String, ClipData> *data;
	Vector<String> fmts;

	Image move, copy, reject;
	Ptr<Ctrl> target;
	int    action;
	byte   actions;

	void   Sync();
	String GetData(const String& f);
	void   DnD(bool paste);

	virtual void  LeftUp(Point, dword);
	virtual bool  Key(dword, int);
	virtual void  MouseMove(Point p, dword);
	virtual Image CursorImage(Point, dword);
};

Ptr<DnDLoop> dndloop;

bool PasteClip::IsAvailable(const char *fmt) const
{
	GuiLock __;
	return dnd ? dndloop && FindIndex(dndloop->fmts, fmt) >= 0
	           : IsClipboardAvailable(fmt);
}

String DnDLoop::GetData(const String& f)
{
	GuiLock __;
	int i = data->Find(f);
	String d;
	if(i >= 0)
		d = (*data)[i].Render();
	else
		if(sDnDSource)
			d = sDnDSource->GetDropData(f);
	return d;
}

String PasteClip::Get(const char *fmt) const
{
	return dnd ? dndloop ? dndloop->GetData(fmt) : String() : ReadClipboard(fmt);
}

void PasteClip::GuiPlatformConstruct()
{
	dnd = false;
}

void DnDLoop::DnD(bool paste)
{
	PasteClip d;
	d.paste = paste;
	d.accepted = false;
	d.allowed = (byte)actions;
	d.action = GetCtrl() ? DND_COPY : DND_MOVE;
	d.dnd = true;
	if(target)
		target->DnD(GetMousePos(), d);
	action = d.IsAccepted() ? d.GetAction() : DND_NONE;
}

void DnDLoop::Sync()
{
	GuiLock __;
	Ptr<Ctrl> t = FindMouseTopCtrl();
	if(t != target)
		if(target)
			target->DnDLeave();
	target = t;
	DnD(false);
}

void DnDLoop::LeftUp(Point, dword)
{
	GuiLock __; 
	LLOG("DnDLoop::LeftUp");
	DnD(true);
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

int Ctrl::DoDragAndDrop(const char *fmts, const Image& sample, dword actions,
                        const VectorMap<String, ClipData>& data)
{
	GuiLock __; 
	DnDLoop d;
	d.actions = (byte)actions;
	d.reject = actions & DND_EXACTIMAGE ? CtrlCoreImg::DndNone() : MakeDragImage(CtrlCoreImg::DndNone(), sample);
	if(actions & DND_COPY)
		d.copy = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndCopy(), sample);
	if(actions & DND_MOVE)
		d.move = actions & DND_EXACTIMAGE ? sample : MakeDragImage(CtrlCoreImg::DndMoveX11(), sample);
	d.SetMaster(*this);
	d.data = &data;
	d.action = DND_NONE;
	d.fmts = Split(fmts, ';');
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
