#include "ide.h"

struct AssistItemInfo : CppItem {
	String defined;
	String overed;
	String name;
};

void GatherVirtuals(ArrayMap<String, AssistItemInfo>& item, const String& scope,
                    Index<String>& done)
{
	if(IsNull(scope))
		return;
	if(done.Find(scope) >= 0)
		return;
	done.Add(scope);
	Vector<String> tparam;
	int q = CodeBase().Find(ParseTemplatedType(scope, tparam));
	if(q < 0)
		return;
	const Array<CppItem>& m = CodeBase()[q];
	for(int i = 0; i < m.GetCount(); i = FindNext(m, i)) {
		const CppItem& im = m[i];
		if(im.IsType()) {
			Vector<String> b = Split(im.qptype, ';');
			ResolveTParam(b, tparam);
			for(int i = 0; i < b.GetCount(); i++)
				GatherVirtuals(item, b[i], done);
		}
	}
	for(int i = 0; i < m.GetCount(); i++) {
		const CppItem& im = m[i];
		if(im.IsCode()) {
			String k = im.qitem;
			if(im.IsCode()) {
				int q = item.Find(k);
				if(q >= 0)
					item[q].overed = scope;
				else
				if(im.virt) {
					AssistItemInfo& f = item.Add(k);
					f.defined = f.overed = scope;
					f.name = im.name;
					(CppItem&)f = im;
				}
			}
		}
	}
}

struct VirtualsDlg : public WithVirtualsLayout<TopWindow> {
	ArrayMap<String, AssistItemInfo> item;

	virtual bool Key(dword key, int count)
	{
		return HotKey(key) || find.Key(key, count);
	}

	void Sync() {
		String name = ~find;
		String k = list.GetKey();
		list.Clear();
		for(int i = 0; i < item.GetCount(); i++) {
			CppItemInfo f;
			(CppItem&)f = item[i];
			f.virt = false;
			f.name = item[i].name;
			if(memcmp_i(name, f.name, name.GetCount()) == 0)
				list.Add(item.GetKey(i), f.natural, RawToValue(f),
				         item[i].defined, item[i].overed);
		}
		if(!list.FindSetCursor(k))
			list.GoBegin();
	}

	void DblClk() {
		if(list.IsCursor()) {
			list.Set(5, 1);
			Break(IDOK);
		}
	}

	void Serialize(Stream& s)
	{
		SerializePlacement(s);
		list.SerializeHeader(s);
	}

	typedef VirtualsDlg CLASSNAME;

	VirtualsDlg(const String& scope) {
		Index<String> done;
		GatherVirtuals(item, scope, done);
		CtrlLayoutOKCancel(*this, "Virtual methods");
		list.AddIndex();
		list.AddIndex();
		list.AddColumn("Virtual function").SetDisplay(Single<CppItemInfoDisplay>());
		list.AddColumn("Defined in");
		list.AddColumn("Last override");
		list.SetLineCy(Arial(11).Info().GetHeight() + 3);
		list.ColumnWidths("500 100 100");
		list.WhenLeftDouble = THISBACK(DblClk);
		list.MultiSelect();
		list.EvenRowColor();
		Sizeable().Zoomable();
		find <<= THISBACK(Sync);
		Sync();
	}
};

INITBLOCK
{
	RegisterGlobalConfig("VirtualsDlg");
}

void AssistEditor::Virtuals()
{
	Parser ctx;
	Context(ctx, GetCursor());
	if(IsNull(ctx.current_scope) || ctx.current_scope == "::" || ctx.IsInBody())
		return;
	VirtualsDlg dlg(ctx.current_scope);
	LoadFromGlobal(dlg, "VirtualsDlg");
	int c = dlg.Run();
	StoreToGlobal(dlg, "VirtualsDlg");
	if(c != IDOK)
		return;
	String cls = ctx.current_scope.Mid(ctx.current_namespacel);
	if(cls[0] == ':' && cls[1] == ':')
		cls = cls.Mid(2);
	String text;
	String ctext;
	if(!dlg.list.IsSelection() && dlg.list.IsCursor())
		dlg.list.Select(dlg.list.GetCursor());
	for(int i = 0; i < dlg.list.GetCount(); i++)
		if(dlg.list.IsSelected(i)) {
			String n = (String)dlg.list.Get(i, 1);
			text << "\tvirtual " << RemoveDefPar(n) << ";\r\n";
			ctext << MakeDefinition(cls, n) << "\n{\n}\n\n";
		}
	Paste(text.ToWString());
	WriteClipboardText(ctext);
}
