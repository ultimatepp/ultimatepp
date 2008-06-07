#include <DocEdit/DocEdit.h>

int  PosOf(const String& str, const String& sub)
{
	if(sub.GetLength() > str.GetLength()) return -1;
	const char *s = str;
	const char *e = s + str.GetLength() - sub.GetLength();
	while(s <= e) {
		if(memcmp(s, sub, sub.GetLength()) == 0)
			return s - ~str;
		s++;
	}
	return -1;
}

void DocBrowser::Nameing()
{
	nesting.Clear();
	if(IsNull(nameing)) {
		Nesting();
		return;
	}
	const VectorMap<String, ArrayMap<String, DocItem> >& ns = set.Get(~nameing);
	nesting.Clear();
	for(int i = 0; i < ns.GetCount(); i++)
		nesting.Add(ns.GetKey(i));
	nesting.Sort(0, CompareNests);
	nesting.GoBegin();
	Nesting();
}

void DocBrowser::Nesting()
{
	item.Clear();
	if(!nesting.IsCursor()) {
		Item();
		return;
	}
	LoadItems(set, ~nameing, nesting.GetKey(), item);
	if(sort)
		item.Sort(CompareItems);
	item.GoBegin();
	edit.Enable(item.IsCursor());
}

void DocBrowser::Item()
{
	Flush();
	EnterItem();
}

void DocBrowser::EnterItem()
{
	String lbl;
	target = current = DocKey(~nameing, nesting.GetKey(), item.GetKey(), (int)~lang);
	if(doc_dir.GetStatus(current) == DocDir::LINK)
		if(!ParseLink(doc_dir.GetLink(current), target, lbl)) {
		   View();
		   view.SetQTF("[R9&&&=@3* Invalid link");
		   return;
		}
	String txt = LoadFile(CommonPath("dppstyles.qtf")) + doc_dir.GetText(target);
	package <<= doc_dir.GetPackage(target);
	if(edit.IsShown()) {
		edit.SetPage(Size(3968, INT_MAX));
		edit.SetQTF(txt);
		edit.SetFocus();
		String p = doc_dir.GetFilePath(target);
		if(!IsNull(p)) {
			int q = editstate.Find(p);
			if(q >= 0) {
				FileInfo& fi = editstate[q];
				if(fi.time == GetFileTime(p)) {
					edit.SetPickUndoInfo(fi.info);
					fi.time = Time(1, 1, 1);
				}
			}
		}
		int q = posinfo.Find(current);
		if(q >= 0)
			edit.SetPosInfo(posinfo[q]);
		else
		if(!lbl.IsEmpty())
			edit.GotoLabel(lbl);
		edit.ClearModify();
	}
	else {
		view.SetQTF(txt);
		if(!lbl.IsEmpty())
			view.GotoLabel(lbl);
	}
	trect.Color(SLtBlue);
	title.SetInk(SWhite);
	String s = " dpp://" + current.nameing + "/" + current.nesting + "/" + current.item;
	switch(GetCurrentStatus()) {
	case DocDir::UNDOCUMENTED:
		trect.Color(SGray);
		break;
	case DocDir::LINK:
		s << " -> " << target.nameing + "/" + target.nesting + "/" + target.item;
		trect.Color(SCyan);
		break;
	case DocDir::OBSOLETE:
		trect.Color(SRed);
		break;
	case DocDir::EXTERNAL:
		trect.Color(SGreen);
		break;
	}
	title = s;
	int status = item.Get(4);
	int kind = item.Get(2);
	String name = item.Get(1);
	String text = item.Get(0);
	String post;
	if(status == DocDir::EXTERNAL)
		post = " (external)";
	if(!name.IsEmpty() && status != DocDir::EXTERNAL) {
		int q = PosOf(text, name);
		if(q >= 0) {
			if(current.nesting.IsEmpty() || kind == STRUCT || kind == STRUCTTEMPLATE)
				Title(text.Mid(0, q) + current.nameing + text.Mid(q) + post);
			else
				Title(text.Mid(0, q) + current.nameing + current.nesting + "::" + text.Mid(q)
				      + post);
			return;
		}
	}
	Title(current.nameing + "::" + current.nesting + "::" + text + post);
}

void DocBrowser::Flush()
{
	if(edit.IsShown() && target && item.IsCursor()) {
		posinfo.GetAdd(current) = edit.GetPosInfo();
		if(edit.IsModified()) {
			DocItem& im = set.Get(target.nameing).Get(target.nesting).Get(target.item);
			const RichText& txt = edit.Get();
			SaveFile(CommonPath("dppstyles.qtf"), StylesAsQTF(txt));
			if(edit.GetLength()) {
				doc_dir.SaveText(GetItemPackage(), target, BodyAsQTF(txt),
				                 doc_dir.GetStatus(target) == DocDir::EXTERNAL);
				im.status = DocDir::NORMAL;
			}
			else {
				doc_dir.Remove(target);
				im.status = DocDir::UNDOCUMENTED;
			}
			String p = doc_dir.GetFilePath(target);
			FileInfo& fi = editstate.GetAdd(p);
			fi.time = GetFileTime(p);
			fi.info = edit.PickUndoInfo();
		}
		target.Clear();
		if(edit.IsModified())
			ReloadItems();
		edit.ClearModify();
	}
}

void DocBrowser::Query()
{
	DocQuery q;
	q.lang = (int)~lang;
	q.undocumented = q.normal = q.external = q.obsolete = q.ignored = false;
	switch(query.status) {
	case 4:
		q.ignored = true;
	case 0:
		q.undocumented = q.normal = q.external = q.obsolete = true;
		break;
	case 1:
		q.undocumented = true;
		break;
	case 2:
		q.obsolete = true;
		break;
	case 3:
		q.ignored = true;
		break;
	}
	q.name = query.name;
	q.text = query.text;
	q.header = ~query.header;
	set = doc_dir.Select(q);
	nameing.ClearList();
	for(int i = 0; i < set.GetCount(); i++)
		nameing.Add(set.GetKey(i));
	if(nameing.GetCount())
		nameing.SetIndex(0);
	Nameing();
}

void DocBrowser::Select()
{
	int c;
	query.header.ClearList();
	Vector<String> h = doc_base.GetHeaders();
	for(int i = 0; i < h.GetCount(); i++)
		query.header.Add(h[i], GetFileName(h[i]));
	query.header.Add(Null, "<any>");
	for(;;) {
		c = query.Run();
		if(c == IDCANCEL)
			break;;
		if(c == IDYES) {
			query.status = 0;
			query.name <<= Null;
			query.text <<= Null;
		}
		if(c == IDOK)
			break;
	}
	query.Close();
	query.name.AddHistory();
	query.text.AddHistory();
	if(c == IDOK)
		Query();
}

bool DocBrowser::Key(dword key, int)
{
	switch(key) {
	case K_F6:
		Select();
		break;
	default:
		return false;
	}
	return true;
}

void DocBrowser::Edit()
{
	Flush();
	edit.Show();
	view.Hide();
	EnterItem();
}

void DocBrowser::View()
{
	view.Show();
	edit.Hide();
	Item();
}

void DocBrowser::Sort()
{
	sort = !sort;
	Nesting();
}

int DocBrowser::GetCurrentStatus()
{
	return item.IsCursor() ? doc_dir.GetStatus(current) : Null;
}

void DocBrowser::ReloadItems()
{
	int c = item.GetCursor();
	int a = item.GetCursorSc();
	Nesting();
	if(c >= 0)
		item.SetCursor(c);
	item.ScCursor(a);
}

void DocBrowser::SetCurrentStatus(int status)
{
	set.GetAdd(current.nameing).GetAdd(current.nesting).GetAdd(current.item).status = status;
	ReloadItems();
}

void DocBrowser::Link()
{
	int q = GetCurrentStatus();
	if(q != DocDir::UNDOCUMENTED && q != DocDir::LINK || !item.IsCursor())
		return;
	String pk = GetItemPackage();
	String h;
	if(GetCurrentStatus() == DocDir::LINK)
		h = doc_dir.GetLink(current);
	if(linkdlg.Perform(set, sort, h, (int)~lang, ~nameing, nesting.GetKey())) {
		int c = item.GetCursor();
		item.KillCursor();
		doc_dir.SaveLink(pk, current, h);
		item.SetCursor(c);
		SetCurrentStatus(DocDir::LINK);
	}
}

void DocBrowser::Ignore()
{
	int q = GetCurrentStatus();
	if(q != DocDir::UNDOCUMENTED || !item.IsCursor()) return;
	int c = item.GetCursor();
	String pk = GetItemPackage();
	item.KillCursor();
	doc_dir.Ignore(pk, current);
	item.SetCursor(c);
	SetCurrentStatus(DocDir::IGNORED);
}

void DocBrowser::Delete()
{
	int q = GetCurrentStatus();
	if(q == DocDir::UNDOCUMENTED || !item.IsCursor()) return;
	int c = item.GetCursor();
	item.KillCursor();
	switch(q) {
	case DocDir::OBSOLETE:
	case DocDir::OBSOLETELINK:
		break;
	default:
		if(!PromptYesNo("Remove current current ?")) return;
	}
	doc_dir.Remove(current);
	item.SetCursor(c);
	SetCurrentStatus(DocDir::UNDOCUMENTED);
}

void DocBrowser::New()
{

	external.package.Add("Core"); //!!!
	external.package <<= doc_dir.GetPackage(current);
	external.nameing <<= ~nameing;
	external.nesting <<= nesting.GetKey();
again:
	if(external.Execute() != IDOK) return;
	if(IsNull(external.nameing)) {
		Exclamation("Missing namespace !");
		goto again;
	}
	if(IsNull(external.item)) {
		Exclamation("Missing item !");
		goto again;
	}
	external.nameing.AddHistory();
	external.nesting.AddHistory();
	DocKey k = DocKey(~external.nameing, ~external.nesting, ~external.item, (int)~lang);
	doc_dir.SaveText(~external.package, k, "", true);
	Query();
	nameing <<= k.nameing;
	Nameing();
	if(nesting.FindSetCursor(k.nesting))
		item.FindSetCursor(k.item);
}

void DocBrowser::EditLink(String& s)
{
	linkdlg.Perform(set, sort, s, (int)~lang, ~nameing, nesting.GetKey());
}

void DocBrowser::FollowLink(const String& s)
{
	if(memcmp(s, "dpp://", 6)) {
		Exclamation("Not a dpp link !");
		return;
	}
	Vector<String> l = Split(~s + 6, '/', false);
	if(l.GetCount() != 3) {
		Exclamation("Invalid link !");
		return;
	}
	if(nameing.HasKey(l[0])) {
		nameing.SetData(l[0]);
		Nameing();
		if(nesting.FindSetCursor(l[1])) {
			String n = l[2];
			String l;
			int q = n.Find('#');
			if(q >= 0) {
				l = n.Mid(q + 1);
				n = n.Mid(0, q);
			}
			if(item.FindSetCursor(n))
				return;
		}
	}
	Exclamation("Link not found");
}

void DocBrowser::BrowseMenu(Bar& bar)
{
	bar.Add("Select..", THISBACK(Select))
	   .Key(K_F6);
	bar.Separator();
	bar.Add("Edit", THISBACK(Edit))
	   .Radio(edit.IsShown())
	   .Key(K_CTRL_O);
	bar.Add("View", THISBACK(View))
	   .Radio(view.IsShown())
	   .Key(edit.IsShown() ? K_ESCAPE : 0);
	bar.Separator();
	bar.Add("Sort by types and names", THISBACK(Sort))
	   .Check(sort);
	bar.Separator();
}

void DocBrowser::ItemMenu(Bar& bar)
{
	int q = GetCurrentStatus();
	bar.Add(q == DocDir::UNDOCUMENTED || q == DocDir::LINK,
	        "Link..", THISBACK(Link))
	   .Key(K_CTRL_L);
	bar.Add(q == DocDir::UNDOCUMENTED, "Ignore", THISBACK(Ignore))
	   .Key(K_CTRL_I);
	bar.Add(q != DocDir::UNDOCUMENTED, "Delete", THISBACK(Delete))
	   .Key(K_CTRL_DELETE);
	bar.Add("New..", THISBACK(New));
}

void DocBrowser::MainMenu(Bar& bar)
{
	bar.Add("Browse", THISBACK(BrowseMenu));
	bar.Add("Item", THISBACK(ItemMenu));
}

void DocBrowser::EditBar(Bar& bar)
{
	edit.StyleTool(bar);
	bar.Gap();
	edit.FontTools(bar);
	bar.Gap();
	edit.InkTool(bar);
	edit.PaperTool(bar);
	bar.Gap();
	edit.LanguageTool(bar);
	edit.SpellCheckTool(bar);
	bar.Break();

	edit.HyperlinkTool(bar, 300);
	bar.Gap();
	edit.LabelTool(bar);
	bar.Gap();
	edit.IndexEntryTool(bar);
	bar.Break();

	edit.ParaTools(bar);
	bar.Gap();
	edit.EditTools(bar);
	bar.Gap();
	edit.PrintTool(bar);
}

void DocBrowser::SetEditBar()
{
	editbar.Set(THISBACK(EditBar));
}

void DocBrowser::SetMainBar()
{
	menu.Set(THISBACK(MainMenu));
}

void DocBrowser::Close()
{
	Flush();
	TopWindow::Close();
}

DocBrowser::DocBrowser()
{
	Zoomable().Sizeable().Icon(DppImg::SmallIcon);
	ni_split.Vert(nesting, item);
	ni_split.SetPos(3000);
	int q = nameing.GetMinSize().cy;
	larea.Add(nameing.HSizePos().TopPos(0, q));
	larea.Add(ni_split.HSizePos().VSizePos(q + 4, 0));
	rarea.AddFrame(trect.Height(q + 4));
	trect.SetFrame(ThinInsetFrame());
	trect.Add(title.SizePos());
	trect.Add(package.RightPos(0, 100).VSizePos());
	rarea.Add(view.SizePos());
	rarea.Add(edit.SizePos());
	vi_split.Horz(larea, rarea);
	vi_split.SetPos(3000);
	Add(vi_split);
	nesting.AddColumn().SetDisplay(Single<DocNestDisplay>());
	nesting.NoHeader().NoGrid();
	nameing <<= THISBACK(Nameing);
	nesting.WhenEnterRow = THISBACK(Nesting);
	InitItemArray(item);
	item.WhenEnterRow = THISBACK(Item);
	item.WhenKillCursor = THISBACK(Flush);
	item.WhenBar = THISBACK(ItemMenu);
	item.WhenLeftDouble = THISBACK(Edit);

	view.Show();
	edit.Hide();
	edit.SetPage(Size(3968, INT_MAX));

	CtrlLayoutOKCancel(query, "Select");
	query.clear <<= query.Breaker(IDYES);
	query.status = 0;

	AddFrame(menu);
	SetMainBar();
	menu.Ctrl::Add(lang.RightPos(10, 70).TopPos(0, EditField::GetStdHeight()));
	lang.Add((int)LNG_ENGLISH, "EN-US");
	lang.Add((int)LNGFromText("EN-GB"), "EN-GB");
	lang.Add((int)LNGFromText("DE-DE"), "DE-DE");
	lang.Add((int)LNG_CZECH, "CS-CZ");
	lang <<= (int)LNGFromText("EN-US");
	lang <<= THISBACK(Query);

	edit.InsertFrame(1, editbar);
	edit.WhenRefreshBar = THISBACK(SetEditBar);
	edit.WhenHyperlink = THISBACK(EditLink);

	view.WhenLink = THISBACK(FollowLink);

	sort = false;

	CtrlLayoutOKCancel(external, "New external item");
	external.ActiveFocus(external.item);
}

void AppMain()
{
	doc_base.ignore.Add("pick_");
	FindFile ff("f:/uppsrc/Core/*.h");
	while(ff) {
		String fn = String("f:/uppsrc/Core/") + ff.GetName();
		try {
			doc_base.ParseFile(fn, "Core");
		}
		catch(Parser::Error e) {
			LOG(e);
		}
		ff.Next();
	}
	doc_dir.Refresh("Core");
	TimeStop();
	RDUMP(TimeStop());
	DocBrowser d;
	d.Query();
	d.Run();
}
