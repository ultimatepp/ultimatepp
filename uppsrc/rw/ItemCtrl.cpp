#include "Designer.h"
//#include <TCtrlLib/TCtrlLib.h>

typedef VectorMap<String, CtrlCreator> CtrlCreatorMap;
GLOBAL_VAR(CtrlCreatorMap, LayoutDesigner::ClassMap);

void LayoutDesigner::Register(const String& classname, CtrlCreator creator) {
	ClassMap().GetAdd(classname) = creator;
}

String    LayoutDesigner::Lng(const Vector<String>& x) const {
	return langi < x.GetCount() ? x[langi] : String();
}

String&   LayoutDesigner::ItemLabel(int i) {
	return Lng(Item(i).label);
}

String    LayoutDesigner::ItemLabel(int i) const {
	return Lng(Item(i).label);
}

String&   LayoutDesigner::Help(int i) {
	return Lng(Item(i).help);
}

String    LayoutDesigner::Help(int i) const {
	return Lng(Item(i).help);
}

void LayoutDesigner::SynchronizeCtrl(int im) {
	const ItemInfo& ii = Item(im);
	SynchronizeCtrl(ctrl[im], ii);
	itab.Set(im, 0, ii.classname);
	itab.Set(im, 1, ii.variable);
	itab.Set(im, 2, (langi < ii.label.GetCount() ? ii.label[langi] : String()));
}

void LayoutDesigner::SynchronizeCtrl(Ctrl& q, const ItemInfo& ii)
{
	q.SetLabel(Lng(ii.label));
//	q.Help(Help(im));
	q.Tip(Lng(ii.help));
	q.DesignMode();
	q.SetPos(ii.pos);
}

void LayoutDesigner::SynchronizeCtrl() {
	SynchronizeCtrl(itemi.Top());
}

void LayoutDesigner::SynchronizeCtrls() {
	mnem_ctrl.Clear();
	ref_ctrl.Clear();
	ctrl.Clear();
	for(int i = 0; i < ItemCount(); i++) {
		ItemInfo& ii = Item(i);
		Ctrl& q = ctrl.Add(CreateCtrl(ii.classname));
		form.Add(q);
		SynchronizeCtrl(i);
	}
	ShowHideLayouts();
}

int LayoutDesigner::ParseLayoutRef(String cls, String& base) const
{
	const char *p = cls;
	if(p[0] != 'W' || p[1] != 'i' || p[2] != 't' || p[3] != 'h')
		return -1;
	const char *b = (p += 4);
	while(IsAlNum(*p) || *p == '_')
		p++;
	if(p <= b)
		return -1;
	String layoutid(b, p);
	int fi = FindFieldIndex(layout, &Form::name, layoutid);
	if(fi < 0)
		return -1;
	while(*p && (byte)*p <= ' ')
		p++;
	if(*p++ != '<')
		return -1;
	while(*p && (byte)*p <= ' ')
		p++;
	const char *e = cls.End();
	while(e > p && (byte)e[-1] <= ' ')
		e--;
	if(e > p && e[-1] == '>')
		e--;
	while(e > p && (byte)e[-1] <= ' ')
		e--;
	base = String(p, e);
	return fi;
}

Ctrl *LayoutDesigner::CreateLayoutCtrl(String classname, Index<String>& recursed)
{
	String base;
	int fi = ParseLayoutRef(classname, base);
	if(fi < 0 || recursed.Find(layout[fi].name) >= 0 || recursed.Find(base) >= 0)
		return NULL;
	recursed.Add(layout[fi].name);
	recursed.Add(base);
	Ctrl *ctrl = CreateCtrl(base, recursed);
	if(!ctrl)
	{
		ctrl = new Ctrl();
		ctrl->Transparent();
	}
	const Form& form = layout[fi];
	for(int i = 0; i < form.item.GetCount(); i++) {
		const ItemInfo& ii = form.item[i];
		Ctrl *q = CreateCtrl(ii.classname, recursed);
		if(!q)
		{
			q = new UrgCtrl(ii.classname);
			q->DesignMode();
		}
		ref_ctrl.Add(q);
		ctrl -> Add(*q);
		SynchronizeCtrl(*q, ii);
	}
	recursed.Drop();
	recursed.Drop();
	return ctrl;
}

Ctrl *LayoutDesigner::CreateCtrl(const String& classname, Index<String>& recursed)
{
	Ctrl *q = CreateLayoutCtrl(classname, recursed);
	if(q)
		return q;
	int ci = ClassMap().Find(alias_map.Convert(classname));
	if(ci >= 0)
	{
		q = ClassMap()[ci]();
		q->DesignMode();
	}
	return q;
}

Ctrl *LayoutDesigner::CreateCtrl(const String& classname) {
	Index<String> temp;
	Ctrl *q = CreateCtrl(classname, temp);
	if(!q)
	{
		q = new UrgCtrl(~classname);
		q->DesignMode();
	}
	return q;
}

void LayoutDesigner::AddCtrl(const String& cls, const String& var, const String& lbl,
							 const String& help) {
	int imi = ctrl.GetCount();
	ItemInfo& ii = ActualForm().item.Add();
	ii.classname = cls;
	ii.variable = var;
	Lng(ii.label) = lbl;
	Lng(ii.help) = help;
	Ctrl& q = ctrl.Add(CreateCtrl(cls));
	q.TopPos(0, max(10, q.GetStdSize().cy)).LeftPos(0, 50);
	ii.pos = q.GetPos();
	q.SetLabel(lbl);
//	q.Help(help);
	q.Tip(help);
	q.DesignMode();
	form.Add(q);
	Refresh();
	itab.Add(cls, var, lbl);
	SelectOneItem(imi);
}

class MnemCtrl : public Ctrl
{
public:
	MnemCtrl(char letter, Font font) : letter(letter), font(font) {}

public:
	virtual void Paint(Draw& draw)
	{
		draw.DrawRect(draw.GetClip(), LtRed);
		draw.DrawText(1, 0, &letter, font, SBlack, 1);
	}

private:
	char letter;
	Font font;
};

static void AddMnemonic(VectorMap< int, Vector<Ctrl *> >& out, Array<Ctrl>& in)
{
	for(int i = in.GetCount(); --i >= 0;)
	{
		Ctrl& c = in[i];
		int m;
		if(c.GetParent() && c.GetParent() -> IsShown() && (m = c.GetMnemonic()) != 0)
			out.GetAdd(ToUpper(m)).Add(&c);
	}
}

void LayoutDesigner::SynchronizeMnemonic()
{
	mnem_ctrl.Clear();

	VectorMap< int, Vector<Ctrl *> > mnemonic;
	AddMnemonic(mnemonic, ctrl);
	AddMnemonic(mnemonic, ref_ctrl);
	Point p = sb;
	Size shift = Point(10, 10) - p;
	Font mfont = StdFont().Bold();
	FontInfo minfo = ScreenInfo().GetFontInfo(mfont);
	int mht = minfo.GetHeight();
	for(int m = 0; m < mnemonic.GetCount(); m++)
		if(mnemonic[m].GetCount() >= 2)
		{
			const Vector<Ctrl *>& mdup = mnemonic[m];
			for(int c = 0; c < mdup.GetCount(); c++)
			{
				char mnem = mnemonic.GetKey(m);
				int mwd = minfo[(byte)mnem];
				Ctrl& mctrl = mnem_ctrl.Add(new MnemCtrl(mnem, mfont));
				mdup[c] -> Add(mctrl.RightPos(0, mwd + 2).BottomPos(0, mht));
			}
		}
}

void LayoutDesigner::CleanRefCtrl()
{
	for(int i = ref_ctrl.GetCount(); --i >= 0;)
		if(!ref_ctrl[i].GetParent())
			ref_ctrl.Remove(i);
}

void LayoutDesigner::SetClass(int im) {
	Ctrl& q = ctrl.Set(im, CreateCtrl(Item(im).classname));
	CleanRefCtrl();
	form.Add(q);
	SynchronizeCtrl(im);
}

void LayoutDesigner::Swap(int i1, int i2) {
	ActualForm().item.Swap(i1, i2);
	SetClass(i1);
	SetClass(i2);
}

REGCTRLCLASS(Button)
REGCTRLCLASS(EditField)
REGCTRLCLASS(EditInt)
REGCTRLCLASS(EditDouble)
REGCTRLCLASS(EditDate)
//REGCTRLCLASS(EditTime)
REGCTRLCLASS(EditIntNotNull)
REGCTRLCLASS(EditDoubleNotNull)
REGCTRLCLASS(EditDateNotNull)
REGCTRLCLASS(EditIntSpin)
REGCTRLCLASS(EditString)
REGCTRLCLASS(EditStringNotNull)
//REGCTRLCLASS(TableArray)
REGCTRLCLASS(ArrayCtrl)
REGCTRLCLASS(Option)
REGCTRLCLASS(Switch)
REGCTRLCLASS(Label)
REGCTRLCLASS(LabelBox)
REGCTRLCLASS(WithDropChoice<EditString>)
REGCTRLCLASS(DropList)
REGCTRLCLASS(ScrollBar)
REGCTRLCLASS(DocEdit)
REGCTRLCLASS(QtfCtrl)
REGCTRLCLASS(StaticRect)
REGCTRLCLASS(Icon)
REGCTRLCLASS(ProgressIndicator)
/*
REGCTRLCLASS(Picture)
REGCTRLCLASS(SqlTable)
REGCTRLCLASS(SqlArray)
REGCTRLCLASS(SqlOption)
*/
REGCTRLCLASS(ToolBar)
REGCTRLCLASS(MenuBar)
//REGCTRLCLASS(InfoBar)
/*
REGCTRLCLASS(TableCheck)
REGCTRLCLASS(ValueOption)
REGCTRLCLASS(FileCtrl)
REGCTRLCLASS(MenuButton)
REGCTRLCLASS(DragDropCtrl)
REGCTRLCLASS(ScrollTable)
REGCTRLCLASS(StaticCtrl)
*/
REGCTRLCLASS(DataPusher)
//REGCTRLCLASS(ColorCtrl)
REGCTRLCLASS(TabCtrl)
//REGCTRLCLASS(OldTreeCtrl)
