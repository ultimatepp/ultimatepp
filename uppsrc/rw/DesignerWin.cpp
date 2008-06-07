#include "Designer.h"
#pragma hdrstop
#include "iderw.h"

void LayoutDesigner::SetupGrid() {
	CtrlLayoutOKCancel(grid, "Grid settings");
	grid.x.MinMax(1, 32);
	grid.y.MinMax(1, 32);
	grid.Backup();
	grid.Execute();
}

void LayoutDesigner::FileBar(Bar& bar) {
	bar.Add("New", THISBACK(NewFile))
		.Key(K_CTRL_N);
	bar.Add("Open..", THISBACK(OpenFile))
		.Key(K_CTRL_O);
	bar.Add("Save", THISBACK(SaveFile))
		.Key(K_CTRL_S);
	bar.Add("Save as..", THISBACK(SaveFileAs));
	LruList(bar);
	bar.MenuSeparator();
	bar.Add(
#ifdef flagIDERW
	"Close"
#else
	"Exit"
#endif
	, THISBACK(CloseFile));
}

void LayoutDesigner::EditBar(Bar& bar) {
	bar.Add("Cut", CtrlImg::cut(), THISBACK(Cut))
		.Key(K_CTRL_X);
	bar.Add("Copy", CtrlImg::copy(), THISBACK(Copy))
		.Key(K_CTRL_C);
	bar.Add("Paste", CtrlImg::paste(), THISBACK(Paste))
		.Key(K_CTRL_V);
	bar.Add("Duplicate", THISBACK(Duplicate))
		.Key(K_CTRL_D);
	bar.Add("Delete", CtrlImg::remove(), THISBACK(Delete))
		.Key(K_DELETE);
	bar.MenuSeparator();
	bar.Add("Select All", THISBACK(SelectAll))
		.Key(K_CTRL_A);
}

void LayoutDesigner::LayoutBar(Bar& bar) {
	bar.Add("New layout..", THISBACK(NewLayout));
	bar.Add(layoutlist.GetCount(), "Remove layout", THISBACK(RemoveLayout));
	bar.Add(layoutlist.GetCount(), "Rename layout..", THISBACK(RenameLayout));
	if(layoutlist.GetCount() > 1) {
		bar.Separator();
		bar.Add("Previous layout", THISBACK(PrevLayout))
			.Key(K_ALT_UP);
		bar.Add("Next layout", THISBACK(NextLayout))
			.Key(K_ALT_DOWN);
	}
}

void LayoutDesigner::LangBar(Bar& bar) {
	bar.Add("New language..", THISBACK(NewLanguage));
	bar.Add(langlist.GetCount(), "Remove language", THISBACK(RemoveLanguage));
	bar.Add(langlist.GetCount(), "Rename language..", THISBACK(RenameLanguage));
	if(langlist.GetCount() > 1) {
		bar.Separator();
		bar.Add("Previous language", THISBACK(PrevLang))
			.Key(K_ALT_LEFT);
		bar.Add("Next language", THISBACK(NextLang))
			.Key(K_ALT_RIGHT);
	}
}

void LayoutDesigner::OptionsBar(Bar& bar) {
	bar.Add("Alias map", THISBACK(SetupAlias));
	bar.Add("Use grid", TB_GRID(), THISBACK(ToggleGrid))
		.Check(usegrid);
	bar.Add("Grid settings..", THISBACK(SetupGrid));
	bar.Add("Ignore min size", THISBACK(ToggleIgnoreMin))
		.Check(ignoremin);
	bar.Add("QLIB2 output", THISBACK(ToggleQLIB2Output))
		.Check(qlib2_output);
}

void LayoutDesigner::AlignBar(Bar& bar) {
	int m = itemi.GetCount();
	bar.Add(m, "Vertical center in layout", TB_VERTCENTER(), THISBACK(VertCenter))
		.Key(K_CTRL_F9);
	bar.Add(m, "Align label", TB_ALIGNLBL(), THISBACK(AlignLbl))
		.Key(K_ALT_F9);
	bar.Add(m, "Horizontal center in layout", TB_HORZCENTER(), THISBACK(HorzCenter))
		.Key(K_CTRL|K_SHIFT_F9);
	bar.Separator();
	bar.Add(m > 1, "Left", TB_ALIGNLEFT(), THISBACK(AlignLeft))
		.Key(K_ALT_LEFT);
	bar.Add(m > 1, "Horizontal center", TB_ALIGNHCENTER(), THISBACK(AlignHCenter))
		.Key(K_SHIFT_F9);
	bar.Add(m > 1, "Right", TB_ALIGNRIGHT(), THISBACK(AlignRight))
		.Key(K_ALT_RIGHT);
	bar.Separator();
	bar.Add(m > 1, "Top", TB_ALIGNTOP(), THISBACK(AlignTop))
		.Key(K_ALT_UP);
	bar.Add(m > 1, "Vertical center", TB_ALIGNVCENTER(), THISBACK(AlignVCenter))
		.Key(K_F9);
	bar.Add(m > 1, "Bottom", TB_ALIGNBOTTOM(), THISBACK(AlignBottom))
		.Key(K_ALT_DOWN);
}

void LayoutDesigner::SizeBar(Bar& bar) {
	int m = itemi.GetCount();
	bar.Add(m > 1, "Same width", TB_SAMEWIDTH(), THISBACK(SameWidth))
		.Key(K_SHIFT_F8);
	bar.Add(m > 1, "Same height", TB_SAMEHEIGHT(), THISBACK(SameHeight))
		.Key(K_CTRL_F8);
	bar.Add(m > 1, "Same size", TB_SAMESIZE(), THISBACK(SameSize))
		.Key(K_F8);
	bar.Add(m, "Minimal width", TB_MINWIDTH(), THISBACK(MinWidth))
		.Key(K_SHIFT_F7);
	bar.Add(m, "Minimal height", TB_MINHEIGHT(), THISBACK(MinHeight))
		.Key(K_CTRL_F7);
}

void LayoutDesigner::SpringsBar(Bar& bar) {
	bool m = !itemi.IsEmpty();
	int im = GetItemPosMask();
	bar.Add(m, "Left", TB_SPRINGLEFT(), THISBACK1(ItemPos, MAKELONG(POS_LEFT, POS_NONE)))
		.Check(im & MAKELONG(1 << POS_LEFT, 0));
	bar.Add(m, "Right", TB_SPRINGRIGHT(), THISBACK1(ItemPos, MAKELONG(POS_RIGHT, POS_NONE)))
		.Check(im & MAKELONG(1 << POS_RIGHT, 0));
	bar.Add(m, "Horz size", TB_SPRINGHSIZE(), THISBACK1(ItemPos, MAKELONG(POS_SIZE, POS_NONE)))
		.Check(im & MAKELONG(1 << POS_SIZE, 0));
	bar.Add(m, "Horz center", TB_SPRINGHCENTER(), THISBACK1(ItemPos, MAKELONG(POS_CENTER, POS_NONE)))
		.Check(im & MAKELONG(1 << POS_CENTER, 0));
	bar.Separator();
	bar.Add(m, "Top", TB_SPRINGTOP(), THISBACK1(ItemPos, MAKELONG(POS_NONE, POS_TOP)))
		.Check(im & MAKELONG(0, 1 << POS_TOP));
	bar.Add(m, "Bottom", TB_SPRINGBOTTOM(), THISBACK1(ItemPos, MAKELONG(POS_NONE, POS_BOTTOM)))
		.Check(im & MAKELONG(0, 1 << POS_BOTTOM));
	bar.Add(m, "Vert size", TB_SPRINGVSIZE(), THISBACK1(ItemPos, MAKELONG(POS_NONE, POS_SIZE)))
		.Check(im & MAKELONG(0, 1 << POS_SIZE));
	bar.Add(m, "Vert center", TB_SPRINGVCENTER(), THISBACK1(ItemPos, MAKELONG(POS_NONE, POS_CENTER)))
		.Check(im & MAKELONG(0, 1 << POS_CENTER));
	bar.Separator();
	bar.Add(m, "Auto springs", TB_SPRINGAUTO(), THISBACK1(ItemPos, MAKELONG(POS_AUTO, POS_AUTO)))
		.Key(K_CTRL_R);
	bar.Add(m, "Topleft springs", TB_SPRINGTOPLEFT(), THISBACK1(ItemPos, MAKELONG(POS_LEFT, POS_TOP)))
		.Key(im == MAKELONG(1 << POS_LEFT, 1 << POS_TOP));
}

void LayoutDesigner::ItemBar(Bar& bar) {
	int m = itemi.GetCount();
	bar.Add(layouti >= 0, "New item", THISBACK(AddItem))
		.Key(K_ENTER);
	bar.Add(m, "Remove item", THISBACK(Delete))
		.Key(K_DELETE);
	bar.Add(m, "Move item up", THISBACK(MoveUp))
		.Key(K_CTRL_UP);
	bar.Add(m, "Move item down", THISBACK(MoveDown))
		.Key(K_CTRL_DOWN);
	bar.MenuSeparator();
	bar.Add("Alignment", THISBACK(AlignBar));
	bar.Add("Size", THISBACK(SizeBar));
	bar.Add("Springs", THISBACK(SpringsBar));
	bar.MenuSeparator();
	bar.Add(m, "Matrix..", THISBACK(Matrix))
		.Key(K_CTRL_M);
}

void LayoutDesigner::MainBar(Bar& bar) {
	bar.Add("File", THISBACK(FileBar));
	bar.Add("Edit", THISBACK(EditBar));
	bar.Add("Layout", THISBACK(LayoutBar));
	bar.Add("Language", THISBACK(LangBar));
	bar.Add("Item", THISBACK(ItemBar));
	bar.Add("Options", THISBACK(OptionsBar));
}

void LayoutDesigner::Tool(Bar& bar) {
	EditBar(bar);
//	bar.Separator();
	ItemBar(bar);
	bar.Separator();
	OptionsBar(bar);
	bar.Break();
	bar.Add(layoutlist, 200);
	bar.Gap();
	bar.Add(langlist, 100);
}

void LayoutDesigner::OpenWindow() {
	if(!frame.IsOpen()) {
		alias_map_file = AliasMap::StdFileName();
		alias_map.Load(alias_map_file);
		if(alias_map.IsEmpty())
			alias_map.SetDefault();
		UpdateClassList();
		frame.OpenMain();
		MakeCaption();
	}
}

void LayoutDesigner::Execute() {
//	frame.SetRect(100, 100, 700, 500);
#ifndef flagIDERW
	LoadFromFile(*this);
#endif
	OpenWindow();
	frame.Run();
#ifndef flagIDERW
	StoreToFile(*this);
#endif
	if(!alias_map.Save(alias_map_file))
		Exclamation(NFormat("Error updating alias file [* \1%s\1].", alias_map_file));
}

void LayoutDesigner::UpdateClassList() {
	Vector<String> avail_classes;
	Append(avail_classes, ClassMap().GetKeys());
	Append(avail_classes, alias_map.obj_source);
	Sort(avail_classes, StdLess<String>());
	classname.ClearList();
	for(int i = 0; i < avail_classes.GetCount(); i++)
		classname.AddList(avail_classes[i]);
}

void LayoutDesigner::SetBar()
{
	toolbar.Set(THISBACK(Tool));
}

LayoutDesigner::LayoutDesigner() {
	int ehg = EditField::GetStdHeight();
	VSizePos(0, 0).HSizePos(0, 0);
	form.IgnoreMouse();
	form.ld = this;
	Add(form);
	apaint.VSizePos(0, 0).HSizePos(0, 0).Transparent().IgnoreMouse();
	apaint.ld = this;
	itab.NoWantFocus();
	itab.AddColumn("class");
	itab.AddColumn("var / label").SetDisplay(VarLblDisplay(false)).Add(2);
	itab.AddIndex();
	itab.WhenLeftClick = THISBACK(WhenITab);
	props.Vert(itab, label_help_split);
	props.SetPos(6000);
	class_var_lbl.NoWantFocus().Transparent();
	class_var_lbl.Add(classname.TopPos(0, ehg).HSizePos(0, 88));
	class_var_lbl.Add(variable.TopPos(0, ehg).RightPos(0, 80));
	class_var_lbl.Add(label.VSizePos(ehg + 4, 0).HSizePos());
	label_help_split.Vert(class_var_lbl, help);
//	props.Add(itab.VSizePos(0, ehg + 8 + 60 + 8 + 60 + 8).HSizePos());
//	props.Add(classname.BottomPos(60 + 8 + 60 + 8, STDSIZE).HSizePos(0, 88));
//	props.Add(variable.BottomPos(60 + 8 + 60 + 8, STDSIZE).RightPos(0, 80));
//	props.Add(label.BottomPos(60 + 8, 60).HSizePos());
//	props.Add(help.BottomPos(0, 60).HSizePos());
	classname.WhenAction = THISBACK(NewClass);
	classname.SetDropLines(24);
	variable.WhenAction = THISBACK(NewVariable);
	variable.ClickSelect();
	label.WhenAction = THISBACK(NewLabel);
	help.WhenAction = THISBACK(NewHelp);
	classset = THISBACK(NewCtrl);
	layoutlist.WhenAction = THISBACK(EnterLayout);
	layoutlist.SizePos();
//	layoutlist.LeftPos(0, 150).TopPos(ehg + 11, ehg);
	layouti = -1;

	langlist.WhenAction = THISBACK(EnterLanguage);
	langlist.SizePos();
//	langlist.LeftPos(150, 150).TopPos(ehg + 11, ehg);
	langi = 0;

	form.Hide();
	sb.WhenScroll = THISBACK(Scroll);
//	menubar.TopPos(0, ehg + 4).HSizePos(0, 0);
//	toolbar.HSizePos(300, 0).TopPos(ehg + 8, 24).SetFrame(NullFrame());
	frame.ld = this;
	frame.Sizeable().Zoomable().Icon(RwIconSmall(), RwIconLarge()); //.HelpBox(); // todo? or not to do?
	frame.AddFrame(menubar);
	frame.AddFrame(toolbar);
	frame.WhenClose = THISBACK(CloseFile);
//	status.TopPos(0, ehg + 4).HSizePos(0, 0);
	status.NoTransparent();
	menubar.AddFrame(status);
	SetBar();
	menubar.Set(THISBACK(MainBar));
	design.SetFrame(InsetFrame());
	design.AddFrame(sb);
	design.Add(*this);
	design.Add(apaint);
//	splital.VSizePos(2 * EditField::GetStdHeight() + 16, 0);
	splital.Horz(props, design);
	splital.SetPos(3000);
	frame.Add(splital.SizePos());
	SetLayout(-1);
	LruList.WhenSelect = THISBACK(LruOpen);
	grid.x <<= grid.y <<= 2;
	matrix.x <<= matrix.y <<= 1;
	matrix.vspace <<= matrix.hspace <<= 0;
	CtrlLayoutOKCancel(matrix, "Matrix duplication");
	matrix.x.MinMax(1, 10);
	matrix.y.MinMax(1, 10);
	ignoremin = false;
	qlib2_output = false;
	moved = false;
	usegrid = true;
//	frame.HelpBox().CommentBox(); // todo? or not to do?
	filesel.Type("Layout files (*.lay)", "*.lay").Type("All files (*.*)", "*.*")
		   .DefaultExt("lay");
	layoutlist.Disable();
}

static Array<LayoutDesigner>& GetDesigners() {
	static Array<LayoutDesigner> list;
	return list;
}

#ifdef flagIDERW
void LayoutDesigner::CloseDesigner() {
	Array<LayoutDesigner>& list = GetDesigners();
	for(int i = 0; i < list.GetCount(); i++)
		if(&list[i] == this)
		{
			list.Remove(i);
			return;
		}
}
#endif

bool CloseDesignLayout() {
	Array<LayoutDesigner>& list = GetDesigners();
	while(!list.IsEmpty())
	{
		if(!list[0].CloseLayoutFile())
			return false;
		list.Remove(0);
	}
	return true;
}

bool SaveDesignLayout() {
	Array<LayoutDesigner>& list = GetDesigners();
	bool is_ok = true;
	for(int i = 0; i < list.GetCount(); i++)
		if(!list[i].SaveLayoutFile())
			is_ok = false;
	return is_ok;
}

void DesignLayout(String open_file) {
	Array<LayoutDesigner>& list = GetDesigners();
	for(int i = 0; i < list.GetCount(); i++)
		if(!CompareNoCase(list[i].GetFileName(), open_file))
		{
			list[i].SetWantFocus();
			return;
		}
	LayoutDesigner& ld = list.Add();
	ld.OpenWindow();
	ld.LoadLayoutFile(open_file);
}

Image GetRwIcon() {
	return RwIconSmall();
}

#ifndef flagIDERW
void AppMain()
{
	SetDefaultCharset(CHARSET_WIN1250);
	LayoutDesigner ld;
	ld.Execute();
}
#endif//flagIDERW
