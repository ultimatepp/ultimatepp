#include "ide.h"

struct InsertImageDlg : WithInsertImageLayout<TopWindow>, Display {
	TimeCallback tm;

	VectorMap<String, VectorMap<String, Image>> imap;
	
	struct Img {
		String text;
		Image  image;
	};
	
	int tcx;
	
	bool warning = false;

	void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const override;
	void Paint(Draw& w) override { return TopWindow::Paint(w); } // silence clang warning
	void Layout() override;
	void Serialize(Stream& s) override { SerializePlacement(s); s % noupp % index; }
	
	void Load();
	
	void Sync();

	InsertImageDlg();
};

void InsertImageDlg::Sync()
{
	int sc = list.GetScroll();
	list.Clear();
	String ns = nest.GetCursor() > 0 ? nest.GetKey() : Null;
	String s = ~~search;
	for(const auto& n : ~imap)
		if(IsNull(ns) || n.key == ns)
			for(const auto& m : ~n.value) {
				if(IsNull(s) || ToUpper(m.key).Find(s) >= 0) {
					String id = n.key + "::";
					if(noupp)
						id.TrimStart("Upp::");
					if(index)
						id << "I_" << m.key;
					else
						id << m.key << "()";
					Img img;
					img.image = m.value;
					img.text = id;
					list.Add(id, RawToValue(img));
				}
			}
	list.ScrollTo(sc);
	Layout();
}

void InsertImageDlg::Layout()
{
	int w = list.GetSize().cx;
	int iw = min(Zx(300), tcx) + DPI(2);
	int cols = w / iw;
	if(cols)
		list.ItemWidth(cols ? w / cols : iw);
}

void InsertImageDlg::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
	w.DrawRect(r.right - 1, r.top, 1, r.GetHeight(), SWhiteGray);
	w.DrawRect(r.left, r.bottom - 1, r.GetWidth(), 1, SWhiteGray);
	const Img& m = q.To<Img>();
	Point p = RectC(r.left + DPI(1), r.top + DPI(1), r.GetWidth() - DPI(2), DPI(32)).CenterPos(m.image.GetSize());
	w.DrawImage(p.x, p.y, m.image);
	int tcx = GetTextSize(m.text, StdFont()).cx;
	w.DrawText(tcx > r.GetWidth() ? r.right - DPI(1) - tcx : r.left + (r.GetWidth() - tcx) / 2,
	           r.top + DPI(32), m.text, StdFont(), ink);
}

void InsertImageDlg::Load()
{
	for(const auto& f : ~CodeIndex())
		if(ToLower(GetFileExt(f.key)) == ".iml") {
			Array<ImlImage> img0;
			int format;
			LoadIml(LoadFile(f.key), img0, format);
			ArrayMap<String, ImlImage> img;
			for(const ImlImage& m : img0)
				img.GetAdd(m.name) = m;
			VectorMap<String, Image> map;
			for(const ImlImage& im : img) {
				String id = im.name;
				id.TrimEnd("__DARK");
				id.TrimEnd("__UHD");
				if(map.Find(id) < 0) {
					Image m = MakeImlImage(id, [&](int mode, const String& id) {
						return mode ? ImlImage() : img.Get(id, ImlImage());
					}, 0);
					Size isz = m.GetSize();
					if(max(isz.cx, isz.cy) > DPI(32))
						m = Rescale(m, GetFitSize(isz, DPI(32), DPI(32)));
					map.Add(id, m);
				}
			}
			for(const AnnotationItem& m : f.value.items) {
				int q = map.Find(m.name);
				if(q >= 0)
					imap.GetAdd(m.nest).GetAdd(m.name) = map[q];
			}
		}

	tcx = DPI(32);
	for(const auto& n : ~imap)
		for(const auto& m : ~n.value) {
			tcx = max(tcx, GetTextSize(n.key + "::" + m.key + "()", StdFont()).cx);
			tcx = max(tcx, GetTextSize("I_" + n.key + "::" + m.key, StdFont()).cx);
		}

	Value key = nest.GetKey();
	nest.Clear();
	nest.Add("*");
	for(const auto& n : ~imap)
		nest.Add(n.key);
	
	if(!nest.FindSetCursor(key))
		nest.GoBegin();
}

InsertImageDlg::InsertImageDlg()
{
	CtrlLayoutOKCancel(*this, "Insert .iml Image");

	nest.NoHeader().NoGrid();
	nest.AddColumn();
	nest.Add(AttrText("<all>").Italic().NormalInk(SColorHighlight()));

	search.NullText("Search");
	search.SetFilter([](int c) { return iscid(c) ? ToUpper(c) : 0; });
	
	search ^= noupp ^= index ^= nest.WhenSel = [=] { Sync(); };
	
	list.WhenLeftDouble = [=] {
		Break(IDOK);
	};
	
	list.WhenSel = [=] { ok.Enable(list.IsCursor()); };
	ok.Disable();

	list.RowMode();
	list.SetDisplay(*this);
	list.ItemHeight(DPI(32) + GetStdFontCy() + DPI(2));
	list.ItemWidth(min(Zx(300), tcx) + DPI(2));
	
	Sizeable();
	
	Load();
	
	auto SyncWarning = [=] {
		bool b = Indexer::IsRunning() && Indexer::Progress();
		if(b != warning) {
			warning = b;
			warning_lbl.Show(warning);
			if(!warning)
				Load();
		}
	};
	
	warning_lbl.Hide();
	SyncWarning();
	
	tm.Set(-250, [=] { SyncWarning(); });
}

void Ide::InsertImage()
{
	InsertImageDlg dlg;
	LoadFromGlobal(dlg, "insert-image");
	if(dlg.Run() == IDOK && dlg.list.IsCursor())
		InsertText(~~dlg.list.Get(dlg.list.GetCursor()));
	StoreToGlobal(dlg, "insert-image");
}
