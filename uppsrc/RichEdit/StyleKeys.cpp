#include "RichEdit.h"

namespace Upp {

class StyleKeysDlg : public WithStyleKeysLayout<TopWindow> {
	typedef StyleKeysDlg CLASSNAME;
	
	Array<DropList>    style;
	Array<DropList>    face;
	Array<DropList>    height;
	Array<ColorPusher> ink;
	Array<ColorPusher> paper;

public:
	void Init(const RichEdit& edit, RichEdit::StyleKey *key);
	void Retrieve(RichEdit::StyleKey *k);

	StyleKeysDlg();
};

StyleKeysDlg::StyleKeysDlg()
{
	CtrlLayoutOKCancel(*this, "Styling keys");
	
	list.AddColumn("Key");
	list.AddColumn("Paragraph style");
	list.AddColumn("Font");
	list.AddColumn("Height");
	list.AddColumn("Ink");
	list.AddColumn("Paper");
	list.SetLineCy(EditField::GetStdHeight() + 4);
	list.NoHorzGrid().EvenRowColor().NoCursor();
	list.ColumnWidths("117 160 160 75 90 90");
}
	
void StyleKeysDlg::Init(const RichEdit& edit, RichEdit::StyleKey *key)
{
	const RichText& text = edit.text;
	for(int i = 0; i < 20; i++) {
		RichEdit::StyleKey& k = key[i];
		list.Add((i >= 10 ? "Shift+Alt+" : "Alt+") + AsString(i % 10));

		DropList& st = style.At(i);
		st.Add(Null);
		const RichStyles& ts = text.GetStyles();
		for(int j = 0; j < ts.GetCount(); j++) {
			st.Add(ts.GetKey(j), ts[j].name);
		}
		if(st.FindKey(k.styleid) < 0)
			st.Add(k.styleid, k.stylename);
		st <<= k.styleid;
		list.SetCtrl(i, 1, st, false);
		
		DropList& fc = face.At(i);
		fc.Add(Null);
		for(int j = 0; j < edit.face.GetCount(); j++)
			fc.Add(Font::GetFaceName(edit.face.GetKey(j)));
		if(fc.Find(k.face) < 0)
			fc.Add(k.face);
		fc <<= k.face;
		list.SetCtrl(i, 2, fc, false);

		DropList& hg = height.At(i);
		hg.Add(Null);
		for(int j = 0; j < edit.height.GetCount(); j++)
			hg.Add(edit.height.Get(j));
		if(hg.Find(k.face) < 0)
			hg.Add(k.face);
		list.SetCtrl(i, 3, hg, false);
		
		ColorPusher& n = ink.At(i);
		n.NullText("");
		n <<= k.ink;
		list.SetCtrl(i, 4, n, false);
		
		ColorPusher& p = paper.At(i);
		p.NullText("");
		p <<= k.paper;
		list.SetCtrl(i, 5, p, false);
	}
}

void StyleKeysDlg::Retrieve(RichEdit::StyleKey *key)
{
	for(int i = 0; i < 20; i++) {
		RichEdit::StyleKey& k = key[i];
		k.styleid = ~style[i];
		k.stylename = style[i].GetValue();
		k.face = ~face[i];
		k.height = ~height[i];
		k.ink = ~ink[i];
		k.paper = ~paper[i];
	}
}

void RichEdit::StyleKeys()
{
	StyleKeysDlg dlg;
	dlg.Init(*this, stylekey);
	if(dlg.Run() == IDOK)
		dlg.Retrieve(stylekey);
}

void RichEdit::ApplyStyleKey(int i)
{
	if(i < 0 || i >= 20)
		return;
	const StyleKey& k = stylekey[i];
	if(!IsNull(k.styleid)) {
		int q = style.FindKey(k.styleid);
		if(q >= 0) {
			style.SetIndex(q);
			Style();
		}
	}
	if(!IsNull(k.face)) {
		int q = face.FindValue(k.face);
		if(q >= 0) {
			face.SetIndex(q);
			SetFace();
		}
	}
	if(!IsNull(k.height)) {
		height <<= k.height;
		SetHeight();
	}
	if(!IsNull(k.ink)) {
		ink <<= k.ink;
		SetInk();
	}
	if(!IsNull(k.paper)) {
		paper <<= k.paper;
		SetPaper();
	}
}

RichEdit::StyleKey::StyleKey()
{
	styleid = Null;
	height = Null;
	ink = Null;
	paper = Null;
}

}
