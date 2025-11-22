#include "CtrlLib.h"

namespace Upp {

static bool s_beginner_info_enabled = true;

void EnableBeginnerInfo(bool b)
{
	s_beginner_info_enabled = b;
}

bool IsBeginnerInfoEnabled()
{
	return s_beginner_info_enabled;
}

void PaintBeginnerInfo(Draw& w, const Rect& cr, const char *qtf)
{
	if(!s_beginner_info_enabled)
		return;
	RichText text = ParseQTF(qtf);
	text.ApplyZoom(GetRichTextStdScreenZoom());
	
	int u = 2 * GetStdFontCy() + DPI(4);

	Size sz = cr.GetSize();

	int cx = min(text.GetWidth(), sz.cx - 2 * u);
	int cy = text.GetHeight(cx);
	
	Rect r = RectC(cr.left + sz.cx - cx - u, cr.top + sz.cy - cy - u, cx + DPI(8), cy + DPI(8));
	DrawRoundRect(w, r, DPI(4), Blend(SYellow(), SWhite(), 225), 1, Gray());
	text.Paint(w, r.left + DPI(4), r.top + DPI(4), cx);
}

void PaintBeginnerInfoTopic(Draw& w, Size sz, const char *topic)
{
	PaintBeginnerInfo(w, sz, GetTopic(topic));
}

void PaintBeginnerInfo(Ctrl *ctrl, Draw& w, const Rect& cr, const char *qtf, const char *key)
{
	static Index<String> done_keys;
	
	if(!s_beginner_info_enabled || done_keys.Find(key) >= 0)
		return;

	struct Record : Moveable<Record> {
		Ptr<Ctrl> ctrl;
		String    key;
		Point     mousepos;
		int       tm;
	};
	
	static Vector<Record> records;

	static auto Sweep = [] {
		for(const Record& r : records)
			if(!r.ctrl)
				done_keys.FindAdd(r.key);
		records.RemoveIf([&](int i) { return !records[i].ctrl; });
	};
	
	int i = FindMatch(records, [&](const Record& r) { return r.ctrl == ctrl; });
	if(i < 0) {
		Record& r = records.Add();
		r.ctrl = ctrl;
		r.key = key;
		r.mousepos = GetMousePos();
		r.tm = msecs();
	}

	typedef bool (*MouseHook)(Ctrl *ctrl, bool inframe, int event, Point p,
	                          int zdelta, dword keyflags);
	typedef bool (*KeyHook)(Ctrl *ctrl, dword key, int count);
	typedef bool (*StateHook)(Ctrl *ctrl, int reason);
	typedef void (*PaintHook)(Ctrl *ctrl, Draw& draw, const Rect& clip);
	
	static auto Stop = [](bool testmousepos = false) {
		int tm = msecs();
		Point mousepos;
		if(testmousepos)
			mousepos = GetMousePos();
		for(Record& r : records) {
			if(testmousepos && Distance(r.mousepos, mousepos) < DPI(50))
				continue;
			if(tm - r.tm > 250) {
				if(r.ctrl) {
					r.ctrl->Refresh();
					r.ctrl = nullptr;
				}
			}
		}
		Sweep();
	};

	ONCELOCK {
		Ctrl::InstallKeyHook([](Ctrl *, dword, int) -> bool {
			Stop();
			return false;
		});
		Ctrl::InstallMouseHook([](Ctrl *ctrl, bool inframe, int event, Point p,
	                          int zdelta, dword keyflags) -> bool {
			if(event == Ctrl::MOUSEMOVE)
				Stop(true);
			else
			if(event != Ctrl::CURSORIMAGE)
				Stop();
			return false;
		});
	}
	
	PaintBeginnerInfo(w, cr, qtf);
}

void PaintBeginnerInfo(Ctrl *ctrl, Draw& w, const char *qtf, const char *key)
{
	PaintBeginnerInfo(ctrl, w, ctrl->GetSize(), qtf, key);
}

void PaintBeginnerInfoTopic(Ctrl *ctrl, Draw& w, const Rect& cr, const char *topic)
{
	PaintBeginnerInfo(ctrl, w, cr, GetTopic(topic), topic);
}

void PaintBeginnerInfoTopic(Ctrl *ctrl, Draw& w, const char *topic)
{
	PaintBeginnerInfo(ctrl, w, ctrl->GetSize(), GetTopic(topic), topic);
}

}