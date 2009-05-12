#include <TCtrlLib/Help/TCtrlLibHelp.h>
#pragma hdrstop

NAMESPACE_UPP

#define LLOG(x) // LOG(x)

enum
{
	MAX_LRU = 9,
};

static bool FindTemplate(const char *tmpl, const char *s)
{
	char c;
	while((c = *tmpl++) && c != ';')
		switch(c) {
		case '*':
			do
				if(FindTemplate(tmpl, s))
					return true;
			while(*s++);
			return false;

		case '?':
			if(!*s++)
				return false;
			break;

		default:
			if(ToLower(c) != ToLower(*s++))
				return false;
			break;
		}
	return !*s;
}

static bool FindMultiTemplate(const char *tmpl, const char *s)
{
	while(*tmpl) {
		if(FindTemplate(tmpl, s))
			return true;
		while(*tmpl && *tmpl++ != ';')
			;
	}
	return false;
}

static String GetDefaultExt(const char *s)
{
	while(*s) {
		while(*s && *s != '.')
			s++;
		if(*s) {
			const char *b = ++s;
			bool wild = false;
			for(; *s && *s != ';' && *s != '.'; s++)
				if(*s == '*' || *s == '?')
					wild = true;
			if(!wild && s > b)
				return String(b, s);
			if(*s)
				s++;
		}
	}
	return Null;
}

//////////////////////////////////////////////////////////////////////
// DocList::DocInfo::

void DocList::DocInfo::Serialize(Stream& stream)
{
	stream % name % type;
}

//////////////////////////////////////////////////////////////////////
// DocList::

RegisterHelpTopicObjectTitle(DocList, t_("Open windows"))

DocList::DocList()
//: Dialog(IDTLIB_WINDOWS)
: doc_counter(0)
, lock(false)
, maximize(false)
{
	app_name = GetExeTitle();
//	app_name = typeid(*AfxGetApp()).name();
	window_list.AutoHideSb().NoHeader().NoGrid();
	window_list.AddIndex();
	window_list.AddColumn(t_("Window name"));
	window_list.WhenBar = THISBACK(StdBar);
	window_list.WhenLeftDouble = THISBACK(OnActivate);

	CtrlLayoutOKCancel(*this, DocListHelpTitle());
	HelpTopic("DlgDocList");

	ok     <<= THISBACK(OnActivate);
	close  <<= THISBACK(OnRemove);
	cancel <<= THISBACK(OnClose);

	WhenClose        = THISBACK(OnClose);
//	WhenHelpContents = THISBACK(OnHelpContents);
//	WhenHelpSearch   = THISBACK(OnHelpSearch);

//	recent_path      = GetExeDirFile("");
}

void DocList::Activate()
{
	LLOG("DocList::Activate");
//	AfxGetApp() -> m_pMainWnd = GetCWnd();
	window_list.SetFocus();
	TopWindow::Activate();
}

void DocList::Serialize(Stream& stream)
{
	int version = StreamHeading(stream, 3, 1, 3, "DocList");
	if(version >= 1) {
		StreamContainer(stream, lru);
		stream % store_docs;
		if(!store_docs)
			old_docs.Clear();
		StreamContainer(stream, old_docs);
		if(!store_docs)
			old_docs.Clear();
	}
	if(version >= 2)
		stream % maximize;
	if(version >= 3)
		stream % recent_path;
}

DocList& DocList::AppName(const char *_app_name)
{
	app_name = _app_name;
	for(int i = 0; i < doc_list.GetCount(); i++)
		doc_list[i].Change(DocCtrl::APPNAME);
//	ForEachX(doc_list, XCall(&DocCtrl::Change, DocCtrl::APPNAME));
	return *this;
}

void DocList::Insert(DocCtrl *document)
{
	ASSERT(GetIndex(document) < 0);
	int index = doc_list.GetCount();
	doc_list.Add(document);
	UpdateTable(index);
}

void DocList::Remove(DocCtrl *document)
{
	int ix = GetIndex(document);
	if(ix >= 0) {
		document -> doc_list = NULL;
		doc_list.Remove(ix);
		RefreshTable();
	}
}

void DocList::Detach(DocCtrl *document)
{
	int ix = GetIndex(document);
	if(ix >= 0) {
		document -> doc_list = NULL;
		doc_list.Detach(ix);
		RefreshTable();
	}
}

void DocList::Raise(DocCtrl *document)
{
	int ix = GetIndex(document);
	ASSERT(ix >= 0);
	if(ix > 0)
		doc_list.Move(ix, 0);
	UpdateTable(0);
}

Vector<DocCtrl *> DocList::FindDoc(const DocType& type) const
{
	Vector<DocCtrl *> result;
	for(DocVector::ConstIterator p = doc_list.Begin(), e = doc_list.End(); p < e; p++)
		if(p -> GetDocType() == &type)
			result.Add(const_cast<DocCtrl *>(&*p));
	return result;
}

bool DocList::IsDocAvail(const DocType& type) const
{
	for(int i = 0; i < doc_list.GetCount(); i++)
		if(doc_list[i].GetDocType() == &type)
			return true;
	return false;
}

void DocList::SetActive(int index, bool kill_init)
{
	doc_list[index].SetActive(kill_init);
}

DocCtrl *DocList::GetActive()
{
	int index = GetActiveIndex();
	return index >= 0 ? &doc_list[index] : NULL;
}

const DocCtrl *DocList::GetActive() const
{
	int index = GetActiveIndex();
	return index >= 0 ? &doc_list[index] : NULL;
}

int DocList::GetActiveIndex() const
{
	return doc_list.IsEmpty() ? -1 : 0;
}

int DocList::FindDoc(const char *filename) const
{
	for(DocVector::ConstIterator p = doc_list.Begin(), e = doc_list.End(); p < e; p++)
		if(!p -> IsUnnamed() && !ComparePath(p -> GetName(), filename))
			return p - doc_list.Begin();
	return -1;
}

DocCtrl *DocList::OpenDoc(const char *filename, int type_index)
{
	RemoveLRU(filename);
	if(OpenExistingDoc(filename))
		return GetActive();
	const DocType *type = NULL;
	if(type_index >= 0 && type_index < DocType::doc_types.GetCount())
		type = DocType::doc_types[type_index];
	else
		type = DocType::GetDocType(filename);
	if(!type) {
		Exclamation(NFormat(t_("Document [* \1%s\1] has an unknown type."), filename));
		return NULL;
	}

	DocCtrl *document = type -> CreateDocument(*this);
	if(document) {
		if(document -> Open(filename)) { // managed to open the file
			String fn = document -> GetName();
			if(document -> IsUnnamed())
				fn = filename;
			AddLRU(fn, type);
			document -> SetActive(true);
			SetRecentPath(GetFileDirectory(filename));
		}
		else {
			Remove(document);
			Exclamation(NFormat(t_("Error opening document [* \1%s\1]."), filename));
			document = NULL;
		}
	}

	return document;
}

bool DocList::OpenExistingDoc(const char *filename)
{
	int existing = FindDoc(filename);
	if(existing >= 0) {
		DocCtrl& doc = doc_list[existing];
		doc.SetActive();
		AddLRU(&doc);
		return true;
	}
	return false;
}

void DocList::RefreshTable()
{
	if(IsOpen())
		UpdateTable();
}

void DocList::UpdateTable(int new_ix)
{
	if(!IsOpen())
		return;

//	SyncTable sync(window_list);
//	sync.SetValue(new_ix);
	Vector<String> names;
	names.SetCount(doc_list.GetCount());
	int i;
	for(i = 0; i < doc_list.GetCount(); i++)
		names[i] = doc_list[i].GetName();
	Vector<int> order = GetSortOrder(names, GetLanguageInfo());
	window_list.Clear();
	for(i = 0; i < order.GetCount(); i++)
		window_list.Add(order[i], doc_list[order[i]].GetFullName());
	ok.Enable(!doc_list.IsEmpty());
	close.Enable(!doc_list.IsEmpty());
//	ForSortOrderX(doc_list, XMethod(&DocCtrl::GetName), CsStringComp(),
//		XApply(static_cast<void (TableArray::*)(Value, Value)>(&TableArray::Add),
//		window_list, XVal<int>(), XAt(doc_list, XMethod(&DocCtrl::GetFullName))));
//	XCall(&Ctrl::Enable, !doc_list.IsEmpty()) >> ok >> close;

	Update();
}

void DocList::Destroy()
{
	SetActiveTop();
//	DestroyWindow();
}

void DocList::SetActiveTop(DocCtrl *ignore)
{
	LLOG("DocList::SetActiveTop, ignore " << ::Name(ignore));
	if(lock && IsOpen()) {
		OnWindowList();
		lock = false;
		return;
	}

	for(DocVector::Iterator p = doc_list.Begin(), e = doc_list.End(); p < e; p++)
		if(&*p != &*ignore) {
			p -> SetActive();
			return;
		}

	LLOG("//DocList::SetActiveTop");

//	if(IsOpen())
//		DestroyWindow();
//	AfxGetApp() -> m_pMainWnd = 0;
//	PostQuitMessage(0);
}

void DocList::Change(DocCtrl& doc, int change)
{
	LLOG("DocList::Change(" << change << ")");
	if(change != DocCtrl::ACTIVATE)
		RefreshTable();
}

bool DocList::Close(int index)
{
	ASSERT(index >= 0 && index < doc_list.GetCount());
	return doc_list[index].CloseDoc();
}

void DocList::ToolFileNew(Bar& bar)
{
	bar.Add(t_("New"), CtrlImg::new_doc(), THISBACK(OnFileNew))
		.Key(K_CTRL_N)
		.Help(t_("Create a new document"))
		.Topic("DocListFileNew");
}

void DocList::OnFileNew()
{
	DocCtrl *doc = DocType::CreateAnyDocument(*this);
	if(doc)
		doc -> SetActive();
}

void DocList::ToolFileNewDefault(Bar& bar)
{
	bar.Add(t_("New"), CtrlImg::new_doc(), THISBACK(OnFileNewDefault))
		.Key(K_CTRL_N)
		.Help(t_("Create a new document"))
		.Topic("DocListFileNewDefault");
}

void DocList::OnFileNewDefault()
{
	DocCtrl *doc = DocType::CreateDefaultDocument(*this);
	if(doc)
		doc -> SetActive();
}

void DocList::ToolFileOpen(Bar& bar)
{
	bar.Add(t_("Open"), CtrlImg::open(), THISBACK(OnFileOpen))
		.Key(K_CTRL_O)
		.Help(t_("Open an existing document"))
		.Topic("DocListFileOpen");
}

void DocList::OnFileOpen()
{
	FileSelector fsel;
	DocType::SetOpenFileDlg(fsel);
	fsel <<= recent_path;
	fsel.AllFilesType();
	fsel.Multi();
	if(fsel.ExecuteOpen(t_("Open file")))
		for(int i = 0; i < fsel.GetCount(); i++)
			OpenDoc(fsel[i]);
}

void DocList::ToolFileSaveAll(Bar& bar)
{
	bar.Add(!this || !doc_list.IsEmpty(),
		t_("Save al&l"), THISBACK(OnFileSaveAll))
		.Help(t_("Save all open documents to the disk"))
		.Topic("DocListFileSaveAll");
}

void DocList::OnFileSaveAll()
{
	SaveAll();
}

bool DocList::ToolFileLru(Bar& bar)
{
	if(this && bar.IsMenuBar() && !bar.IsScanKeys() && !lru.IsEmpty()) {
		if(lru.GetCount() > MAX_LRU)
			lru.SetCount(MAX_LRU);
		bar.Separator();
		for(int i = 0; i < lru.GetCount(); i++)
			bar.Add('&' + FormatInt(i + 1) + ' ' + GetFileName(lru[i].name), THISBACK1(OnFileLru, i))
			.Help(NFormat(t_("Open file '%s'"), lru[i].name));
		return true;
	}
	return false;
}

void DocList::OnFileLru(int i)
{
	if(i >= 0 && i < lru.GetCount()) {
		DocInfo doc = lru[i];
		int d;
		for(d = 0; d < DocType::doc_types.GetCount(); d++)
			if(DocType::doc_types[d]->GetStreamName() == doc.type
			|| DocType::doc_types[d]->doc_name == doc.type) {
				OpenDoc(doc.name, d);
				break;
			}
//		OpenDoc(doc.name, FindXIndex(DocType::doc_types,
//			XDeref(XMethod(&DocType::GetStreamName)), doc.type));
	}
}

void DocList::ToolFileQuit(Bar& bar)
{
	bar.Add(t_("Quit"), THISBACK(OnFileQuit))
		.Help(t_("Quit application"))
		.Topic("DocListFileQuit")
		.Topic( "DocListFileQuit");
}

void DocList::OnFileQuit()
{
	if(CloseAll()) {
//		PostQuitMessage(0);
	}
}

void DocList::ToolFileIo(Bar& bar, DocCtrl *doc)
{
	ToolFileNew(bar);
	ToolFileOpen(bar);
	if(doc) {
		doc -> ToolFileSave(bar);
		doc -> ToolFileSaveAs(bar);
	}
	if(!this || !doc_list.IsEmpty())
		ToolFileSaveAll(bar);
	if(doc)
		doc -> ToolFileClose(bar);
}

void DocList::ToolFileMisc(Bar& bar)
{
	ToolFileLru(bar);
	bar.MenuSeparator();
	ToolFileQuit(bar);
}

void DocList::ToolWindowActivate(Bar& bar)
{
	if(this && !bar.IsScanKeys() && !doc_list.IsEmpty()) { // activations don't have any hotkeys so we won't bother expanding in scan mode
		Vector<DocCtrl *> docs;
		Vector<String> names;
		docs.SetCount(doc_list.GetCount());
		names.SetCount(doc_list.GetCount());
		int i;
		for(i = 0; i < doc_list.GetCount(); i++) {
			docs[i] = &doc_list[i];
			names[i] = doc_list[i].GetName();
		}
		IndexSort(names, docs, GetLanguageInfo());
//		SortX(docs, XDeref(XMethod(&DocCtrl::GetName)), CsStringComp());
		int active = FindIndex(docs, &doc_list[0]);
		if(docs.GetCount() > 9) { // remove extra documents
			int rem = max(active - 4 - min(docs.GetCount() - active, 4), 0);
			docs.Remove(0, rem);
			active -= rem;
			rem = max(docs.GetCount() - active - min(active, 4), 0) ;
			docs.SetCount(docs.GetCount() - rem);
		}

		for(i = 0; i < docs.GetCount(); i++) {
			DocCtrl *doc = docs[i];
			bar.Add(FormatInt(i + 1) + ' ' + doc -> GetShortName(), callback(doc, &DocCtrl::OnWindowActivate))
				.Check(i == active)
				.Help(NFormat(t_("Activate document '%s'?"), doc -> GetFullName()));
		}
		if(!docs.IsEmpty())
			ToolWindowList(bar);
	}
}

void DocList::ToolWindowList(Bar& bar)
{
	bar.Add(t_("Windows..."), THISBACK(OnWindowList))
		.Help(t_("Show open documents"))
		.Topic("DocListWindowList");
}

void DocList::OnWindowList()
{
	if(!IsOpen()) {
		Zoomable().Sizeable().Icon(CtrlImg::menu_window());
		TopWindow::OpenMain();
		UpdateTable(0);
	}
//	ActivateTopmostOwnedPopup(GetHWND());
}

void DocList::ToolHelpContents(Bar& bar)
{
	bar.Add(t_("Contents"), THISBACK(OnHelpContents))
		.Help(t_("Display alphabetic help contents"))
		.Topic("DocListHelpContents");
}

void DocList::OnHelpContents()
{
	OpenHelpTopicIndex(Null, false);
}

void DocList::ToolHelpSearch(Bar& bar)
{
	bar.Add(t_("Search"), THISBACK(OnHelpSearch))
		.Help(t_("Display help index for keyword searching"))
		.Topic("DocListHelpSearch");
}

void DocList::OnHelpSearch()
{
	OpenHelpTopicIndex(Null, true);
}

void DocList::ToolHelpBook(Bar& bar)
{
	bar.Add(t_("User manual"), THISBACK(OnHelpBook))
		.Help(t_("Typeset complete user documentation"))
		.Topic("DocListHelpBook");
}

void DocList::OnHelpBook()
{
	CreateHelpBook();
}

void DocList::OnWindowPrev()
{
	if(doc_list.GetCount() < 2) {
		DocCtrl *doc = GetActive();
		if(doc)
			doc -> info_bar.Temporary(t_("This is the only open window."));
		BeepExclamation();
	}
	else {
		doc_list.Move(0, doc_list.GetCount() - 1); // beware of Fidler's bug!!!
		SetActiveTop();
	}
}

void DocList::OnWindowNext()
{
	if(doc_list.GetCount() < 2) {
		DocCtrl *doc = GetActive();
		if(doc)
			doc -> info_bar.Temporary(t_("This is the only open window."));
		BeepExclamation();
	}
	else
		SetActive(GetDocCount() - 1);
}

void DocList::OnRemove()
{
	if(window_list.IsCursor()) {
		lock = true;
		if(Close(window_list.Get(0)))
			window_list.SetFocus();
		else
			lock = false;
	}
}

void DocList::OnActivate()
{
	if(window_list.IsCursor()) {
		SetActive(window_list.Get(0));
		window_list.SetFocus();
	}
}

void DocList::OnClose()
{
	TopWindow::Close();
}

void DocList::StdBar(Bar& bar)
{
	DocCtrl *doc = (window_list.IsCursor() ? &doc_list[window_list.Get(0)] : NULL);
	if(doc) {
		bar.Add(t_("Activate document '%s'?"), THISBACK(OnActivate));
		bar.Separator();
	}
	ToolFileIo(bar, doc);
	ToolFileMisc(bar);
}

int DocList::SaveAll()
{
	for(int i = 0; i < doc_list.GetCount(); i++)
		if(!doc_list[i].FlushDirty())
			return i;
	return -1;
}

RegisterHelpTopicObjectTitle(DlgDocListSave, t_("Close all documents"))

bool DocList::CloseAll(bool save)
{
	if(save) {
		WithDocListSaveLayout<TopWindow> dialog;
		CtrlLayoutOKCancel(dialog, DlgDocListSaveHelpTitle());
		dialog.HelpTopic("DlgDocListSave");
		ArrayOption save;
//		TableCheck save(0, 1);
		dialog.list.AddIndex();
		dialog.list.AddColumn(t_("Document"), 25);
		save.AddColumn(dialog.list, t_("Save"), 5);
		for(int i = 0; i < doc_list.GetCount(); i++)
			if(doc_list[i].IsDirty())
				dialog.list.Add(i, doc_list[i].GetShortName(), 1);

		if(dialog.list.GetCount() == 1) {
			if(SaveAll() >= 0)
				return false;
		}
		else if(dialog.list.GetCount() > 1) {
			switch(dialog.Run()) {
			case IDOK: {
					for(int i = 0; i < dialog.list.GetCount(); i++)
						if((int)dialog.list.Get(i, 2)) {
							int j = dialog.list.Get(i, 0);
							if(!doc_list[j].SaveAs())
								return false;
						}
				}
				break;

			case IDNO:
				break;

			case IDCANCEL:
				return false;
			}
		}
	}

	bool all = true;
	for(int i = 0; i < doc_list.GetCount(); i++) {
		DocCtrl& doc = doc_list[i];
		doc.Untouch();
		all &= doc.CloseDoc();
	}
	return all;
}

void DocList::AddLRU(const DocCtrl *doc)
{
	ASSERT(doc);
	if(!doc -> IsUnnamed() && !IsNull(doc -> GetName()))
		AddLRU(doc -> GetName(), doc -> GetDocType());
}

void DocList::AddLRU(const char *doc_name, const DocType *type)
{
	ASSERT(type);
	String type_name = type -> GetStreamName();
	if(!IsNull(type_name)) {
		RemoveLRU(doc_name);
		lru.Insert(0, DocInfo().Name(doc_name).Type(type_name));
		if(lru.GetCount() > MAX_LRU)
			lru.SetCount(MAX_LRU);
	}
}

void DocList::RemoveLRU(const char *doc_name)
{
	for(int i = lru.GetCount(); --i >= 0;)
		if(!CompareNoCase(lru[i].name, doc_name))
			lru.Remove(i);
}

void DocList::Run()
{
	if(doc_list.IsEmpty())
		Open();
	Ctrl::EventLoop();
/*
	while(IsOpen() && IsVisible() || !doc_list.IsEmpty()) {
		Ctrl::GuiSleep(1000);
		Ctrl::ProcessEvents();
	}
*/
}

//////////////////////////////////////////////////////////////////////
// DocType::

Vector<const DocType *> DocType::doc_types;

DocType::DocType(DocCtrl *(*create_doc)())
: create_doc(create_doc)
, detect_doc(NULL)
, doc_name(t_("Document"))
, default_name(t_("new"))
, paper(SGray)
, new_list(true)
{
}

int DocType::GetIndex() const
{
	return FindIndex(doc_types, this);
}

static String EncodeFilter(const String& filter, const String& ext)
{
	String f = filter;
	bool first = true;
	const char *s = ext;
	while(*s) {
		const char *p = s;
		while(*s && *s != ';')
			s++;
		if(s != p) {
			f << (first ? " (" : ", ");
			f.Cat(p, s - p);
			first = false;
		}
		if(*s)
			s++;
	}
	if(!first)
		f << ')';
	return f;
}

void DocType::SetOpenFileDlg(FileSelector& fsel)
{
	Index<String> exts;
	int i;
	for(i = 0; i < doc_types.GetCount(); i++) {
		const char *s = doc_types[i] -> extensions;
		while(*s) {
			const char *p = s;
			while(*s && *s != ';')
				s++;
			if(s != p)
				exts.FindAdd(String(p, s));
			if(*s)
				s++;
		}
	}
	if(exts.GetCount() > 1) {
		String list;
		for(const String *p = exts.Begin(), *e = exts.End(); p < e; p++)
			list << *p << ';';
		if(!list.IsEmpty())
			list.Trim(list.GetLength() - 1);
		fsel.Type(EncodeFilter(t_("Supported types"), list), list);
	}

	for(i = 0; i < doc_types.GetCount(); i++) {
		const DocType& dt = *doc_types[i];
		if(!IsNull(dt.filter))
			fsel.Type(EncodeFilter(dt.filter, dt.extensions), dt.extensions);
	}
}

void DocType::SetSaveFileDlg(FileSelector& fsel) const
{
	String ext = extensions;
	int f = ext.Find(';');
	if(f >= 0)
		ext.Trim(f);
	fsel.Type(EncodeFilter(filter, ext), ext);
	f = ext.Find('.');
	if(f >= 0)
		ext.Remove(0, f + 1);
	fsel.DefaultExt(ext);
}

DocType& DocType::DocName(const char *_ndn)
{
	doc_name = _ndn;
	return *this;
}

DocType& DocType::Filter(const char *_filter, const char *_defext)
{
	filter = _filter;
	extensions = _defext;
	return *this;
}

DocType& DocType::HelpTopic(const char *_ht)
{
	help_topic = _ht;
	return *this;
}

DocCtrl *DocType::CreateDocument(DocList& list) const
{
	ASSERT(create_doc);
	DocCtrl *doc = create_doc();
	if(!doc)
		return doc;
	doc -> SetDocType(this);
	doc -> SetDocList(&list);
	doc -> HelpTopic(help_topic);
	if(!doc -> New()) {
		delete doc;
		doc = NULL;
	}
	return doc;
}

DocType& DocType::StreamName(const char *sn)
{
	stream_name = sn;
	return *this;
}

String DocType::GetStreamName() const
{
	return !IsNull(stream_name)  ? stream_name
		:  !IsNull(help_topic)   ? help_topic
		:  doc_name;
}

const DocType *DocType::FindStreamName(String sn)
{
	int index = doc_types.GetCount();
	while(--index >= 0 && doc_types[index] -> GetStreamName() != sn)
		;
	return index >= 0 ? doc_types[index] : NULL;
}

const DocType *DocType::GetDocType(const char *filename)
{
	if(!filename || !*filename)
		return NULL;
	String fn = UPP::GetFileName(filename);
	for(const DocType *const *p = doc_types.Begin(), *const *e = doc_types.End(); p < e; p++)
		if(FindMultiTemplate((*p) -> GetExt(), fn))
			return *p;
	return NULL;
}

DocCtrl *DocType::CreateDefaultDocument(DocList& list)
{
	ASSERT(!doc_types.IsEmpty());
	return doc_types[0] -> CreateDocument(list);
}

RegisterHelpTopicObjectTitle(DlgDocListNew, t_("New..."))

DocCtrl *DocType::CreateAnyDocument(DocList& list)
{
	const DocType *type = NULL;
	static String recent_type;
 {
		Vector<const DocType*> avail_types;
		Vector<String> avail_names;
		int i;
		for(i = 0; i < doc_types.GetCount(); i++) {
			const DocType& doc = *doc_types[i];
			if(!IsNull(doc.GetDocName()) && doc.IsNewListEnabled() && doc.GetCreateDoc()) {
				avail_types.Add(&doc);
				avail_names.Add(doc.GetDocName());
			}
		}
		switch(avail_types.GetCount()) {
		case 0:  NEVER(); return NULL; // no document types available
		case 1:  return avail_types[0] -> CreateDocument(list);
		}

		IndexSort(avail_names, avail_types, GetLanguageInfo());
		WithDocListNewLayout<TopWindow> dialog;
		CtrlLayoutOKCancel(dialog, DlgDocListNewHelpTitle());
		dialog.HelpTopic("DlgDocListNew");
		dialog.types.NoHeader().AutoHideSb();
		dialog.types.WhenLeftDouble = dialog.Acceptor(IDOK);
		dialog.types.AddIndex();
		dialog.types.AddColumn(t_("Document type"));
		int x = -1;
		for(i = 0; i < avail_types.GetCount(); i++) {
			dialog.types.Add(i, avail_types[i] -> GetDocName());
			if(avail_types[i] -> GetStreamName() == recent_type)
				x = i;
		}
		dialog.types.SetCursor(max(0, x));
		if(dialog.Run() != IDOK || dialog.types.GetCursor() < 0)
			return NULL;
		type = avail_types[dialog.types.GetCursor()];
		recent_type = type -> GetStreamName();
	}

	return type -> CreateDocument(list);
}

int DocType::Add(const DocType& doctype)
{
	int index = doc_types.GetCount();
	doc_types.Add(&doctype);
	return index;
}

//////////////////////////////////////////////////////////////////////
// DocCtrl::

DocCtrl::DocCtrl()
: doc_type(NULL)
, doc_list(NULL)
, is_dirty(false)
, is_unnamed(true)
, is_read_only(false)
, is_closing(false)
{
	WhenClose = THISBACK(OnWindowClose);
	AddFrame(menu_bar);
	AddFrame(info_bar);
	AddFrame(tool_bar);
	Rescan();
//	AddIdleCallback(callback(&(Bar&)tool_bar, &Bar::Rescan));
}

DocCtrl::~DocCtrl()
{
	if(doc_list)
		doc_list -> Detach(this);
//	KillIdleCallback(callback(&(Bar&)tool_bar, &Bar::Rescan));
}

DocCtrl& DocCtrl::SetDocList(DocList *list)
{
	if(doc_list != list) {
		ASSERT(!doc_list);
		// currently we don't support document grafting
		// because it's complicated and it seems absolutely useless
		if(doc_list)
			doc_list -> Remove(this);
		if(doc_list = list)
			list -> Insert(this);
	}
	return *this;
}

void DocCtrl::Activate()
{
	LLOG("DocCtrl::Activate for " << ::Name(this));
	ASSERT(doc_list);
//	AfxGetApp() -> m_pMainWnd = GetCWnd();
	TopWindow::Activate();
//	info_bar.HelpDefault();
	doc_list -> Raise(this);
}

void DocCtrl::Serialize(Stream& stream)
{
}

bool DocCtrl::New()
{
	ASSERT(doc_list);
	is_unnamed = true;

	int i = doc_list -> GetNextDocIndex();
	filename = FormatInt(i);

	is_dirty = false;
	Change(NEW);
	return true;
}

bool DocCtrl::Open(const char *doc, bool readonly)
{
	FileIn file(doc);
	if(!file.IsOpen())
		return false;
	Serialize(file);
	Untouch();
	if(file.IsError())
		return false;
	SetName(doc, true);
	SetReadOnly(readonly);
	Change(OPEN);
	return true;
}

bool DocCtrl::SaveAs(const char *as)
{
	String fn;
	bool known;
	if(as)
		known = !IsNull(fn = as);
	else { // if(!IsUnnamed())
		fn = GetName();
		known = !IsUnnamed();
	}
	if(!known && IsNull(fn = QueryFileName()))
		return false;
	try {
		if(!Save(fn)) {
			Exclamation(t_("Error saving document. Check free disk space."));
			return false;
		}
	}
	catch(Exc e) {
		ShowExc(e);
		return false;
	}
	SetName(fn, true);
	Untouch();
	return true;
}

String DocCtrl::QueryFileName()
{
	FileSelector fsel;
	doc_type -> SetSaveFileDlg(fsel);
	fsel <<= Nvl(GetName(), doc_list -> GetRecentPath());
	if(!fsel.ExecuteSaveAs(t_("Save as")))
		return Null;
	return ~fsel;
}

bool DocCtrl::Save(const char *fn)
{
	FileOut file(fn);
	if(!file.IsOpen())
		return false;
	Serialize(file);
	if(file.IsError())
		return false;
	doc_list -> SetRecentPath(GetFileDirectory(fn));
	return true;
}

bool DocCtrl::IsTopWindow() const
{
	return IsEnabled(); // && GetHWND() && IsWindowEnabled(GetHWND());
}

bool DocCtrl::CloseDoc()
{
	if(is_closing)
		return true;
	if(!IsTopWindow() || !IsVisible() || !IsEnabled()) { // don't close disabled window, a dialog might be over it
		Exclamation(NFormat(t_("Error closing document [* \1%s\1]."), GetName()));
		return false;
	}
	is_closing = true;
	if(FlushDirty()) {
		DoClose();
		return true;
	}
	else {
		is_closing = false;
		return false;
	}
}

void DocCtrl::DoClose()
{
	Disable();
	is_closing = true;
	Change(CLOSE);
	Close();
	if(doc_list)
		doc_list -> Remove(this);
//	KillTimeCallback();
//	PostCallback(THISBACK(PostClose));
}

//void DocCtrl::PostClose()
//{
//	if(doc_list)
//		doc_list -> SetActiveTop(this);
//	DestroyWindow();
//	if(doc_list)
//		doc_list -> Remove(this);
//}

/*
bool DocCtrl::BeginApplModal()
{
	ASSERT(doc_list);
	return doc_list -> BeginApplModal();
}
*/

/*
void DocCtrl::EndApplModal()
{
	ASSERT(doc_list);
	doc_list -> EndApplModal();
}
*/

/*
bool DocCtrl::RunApplModal(Callback cb)
{
	ASSERT(doc_list);
	return doc_list -> RunApplModal(cb);
}
*/

void DocCtrl::State(int reason)
{
/*
	if(reason == SHOW || reason == ACTIVATE) {
		WINDOWPLACEMENT wp;
		wp.length = sizeof(wp);
		VERIFY(GetWindowPlacement(GetHWND(), &wp));
		doc_list -> Maximize(wp.showCmd == SW_SHOWMAXIMIZED);
		RefreshFrame();
	}
	else if(reason == DEACTIVATE)
		RefreshFrame();
*/
	Ctrl::State(reason);
//	Rescan();
}

void DocCtrl::Change(int reason)
{
	if(!doc_list)
		return;
	if(IsOpen()) {
		String name;
		if(IsOpenDoc())
			name = GetDirtyName();
		name << " - " << doc_list -> GetAppName();
		Title(name.ToWString());
	}
	doc_list -> Change(*this, reason);
	Rescan();
}

DocCtrl& DocCtrl::SetReadOnly(bool _readonly)
{
	if(is_read_only != _readonly)
		Change((is_read_only = _readonly) ? READ_ONLY : WRITEABLE);
	return *this;
}

void DocCtrl::Touch(bool _touch)
{
	if(is_dirty != _touch) {
		Change((is_dirty = _touch) ? TOUCH : UNTOUCH);
		Rescan();
	}
}

bool DocCtrl::FlushDirty()
{
	if(!IsDirty())
		return true;
	switch(PromptYesNoCancel(NFormat(t_("%s [* \1%s\1] has been edited. Save changes to the disk?"),
		Nvl(doc_type -> GetDocName(), t_("Document")), filename))) {
	case 1:  return SaveAs();
	case 0:  return true;
	default: return false;
	}
}

void DocCtrl::Paint(Draw& draw)
{
	draw.DrawRect(GetSize(), doc_type -> paper);
}

void DocCtrl::ToolFileSave(Bar& bar)
{
	bar.Add(/*IsOpen() && IsTopWindow(),*/ t_("Save"), CtrlImg::save(), THISBACK(OnFileSave))
		.Key(K_CTRL_S)
		.Help(t_("Save document to the disk\f; you should close all dialog windows first"))
		.Topic("DocCtrlFileSave");
}

void DocCtrl::OnFileSave()
{
	SaveAs();
}

void DocCtrl::ToolFileSaveAs(Bar& bar)
{
	bar.Add(/*IsOpen() && IsTopWindow(),*/ t_("Save &as"), CtrlImg::save_as(), THISBACK(OnFileSaveAs))
		.Help(t_("Save document to another disc file\f; you should close all dialog windows first"))
		.Topic("DocCtrlFileSaveAs");
}

void DocCtrl::OnFileSaveAs()
{
	SaveAs("");
}

void DocCtrl::ToolFileClose(Bar& bar)
{
	bar.Add(IsTopWindow(), t_("Close"), THISBACK(OnFileClose))
		.Help(t_("Close active document\f; you should close all dialog windows first"))
		.Topic("DocCtrlFileClose");
}

void DocCtrl::OnFileClose()
{
	CloseDoc();
}

void DocCtrl::ToolFile(Bar& bar)
{
	doc_list -> ToolFileIo(bar, this);
	doc_list -> ToolFileMisc(bar);
}

void DocCtrl::ToolWindowToolBar(Bar& bar)
{
	bar.Add(t_("Toolbar"), THISBACK(OnWindowToolBar))
		.Check(tool_bar.IsVisible())
		.Help(t_("Show / hide window toolbar"))
		.Topic("DocCtrlWindowToolBar");
}

void DocCtrl::OnWindowToolBar()
{
	tool_bar.Show(!tool_bar.IsVisible());
}

void DocCtrl::ToolWindowInfoBar(Bar& bar)
{
	bar.Add(t_("Info bar"), THISBACK(OnWindowInfoBar))
		.Check(info_bar.IsVisible())
		.Help(t_("Show / hide window information bar"))
		.Topic("DocCtrlWindowInfoBar");
}

void DocCtrl::OnWindowInfoBar()
{
	info_bar.Show(!info_bar.IsVisible());
	info_bar.Layout();
}

void DocCtrl::AddToolWindow(Bar& bar)
{
	if(bar.IsMenuBar())
		bar.Add(t_("Window"), THISBACK(ToolWindow));
}

void DocCtrl::ToolWindow(Bar& bar)
{
	ToolWindowToolBar(bar);
	ToolWindowInfoBar(bar);
	bar.Separator();
	doc_list -> ToolWindowActivate(bar);
}

void DocCtrl::ToolHelp(Bar& bar)
{
//	ToolHelpDoc(bar);
	doc_list -> ToolHelpContents(bar);
	doc_list -> ToolHelpSearch(bar);
}

void DocCtrl::BarView(Bar& bar)
{
}

void DocCtrl::BarTool(Bar& bar)
{
	bar.Add(t_("File"), THISBACK(ToolFile));
	BarView(bar);
	AddToolWindow(bar);
	if(bar.IsMenuBar())
		bar.Add(t_("Help"), THISBACK(ToolHelp));
}

void DocCtrl::Rescan()
{
	menu_bar.Set(THISBACK(BarTool));
	tool_bar.Set(THISBACK(BarTool));
}

void DocCtrl::OnWindowActivate()
{
	SetActive();
}

void DocCtrl::OnWindowClose()
{
	CloseDoc();
}

void DocCtrl::SetActive(bool kill_init)
{
	LOG("DocCtrl::SetActive in " << UPP::Name(this));

	if(!IsEnabled())
		return;

	ASSERT(doc_list);

	bool max = doc_list -> IsMaximize();

	DocCtrl *active = doc_list -> GetActive();
	if(kill_init && active && active != this && active -> IsUnnamed() && !active -> IsDirty()) { // kill empty document
		LOGBLOCK("-> KillInit");
		Rect rc = active -> GetRect();
		SetRect(rc);
		max = active -> IsMaximized();
		active -> CloseDoc();
	}

	if(!IsOpen()) {
		LOGBLOCK("-> open window");
		LOG("Sizeable, Zoomable, Icon");
		Sizeable().Zoomable().Icon(doc_type -> GetSmallIcon(), doc_type -> GetLargeIcon());
		LOG("Layout");
		Layout();
		LOG("OpenMain");
		TopWindow::OpenMain();
		if(max)
			Maximize();
		LOG("OPENWINDOW");
		Change(OPENWINDOW);
	}

	menu_bar.WhenHelp = info_bar;
	LOG("Activate");
	Activate();
//	SetWantFocus();
//	ActivateTopmostOwnedPopup(GetHWND());
}

void DocCtrl::SetName(String name, bool clear_unnamed)
{
	if(!IsNull(name) && name != filename) {
		filename = name;
		if(clear_unnamed) {
			is_unnamed = false;
			doc_list -> AddLRU(this);
		}
		Change(RENAME);
	}
}

String DocCtrl::GetDirtyName() const
{
	String name = doc_type -> GetDocName();
	bool doc = !IsNull(name);
	if(doc)
		name << " (";
	name << GetFileName(GetName());
	if(IsDirty())
		name << " *";
	if(doc)
		name << ')';
	return name;
}

String DocCtrl::GetShortName() const
{
	String name = GetName();
	if(!IsUnnamed())
		name = GetFileName(name);
	return doc_type -> GetDocName() + " (" + name + ')';
}

String DocCtrl::GetFullName() const
{
	return doc_type -> GetDocName() + " (" + GetName() + ')';
}

bool DocCtrl::HotKey(dword key)
{
	if(Ctrl::HotKey(key))
		return true;
	switch(key) {
	case K_TAB: case K_SHIFT_TAB: return true; // do nothing on tab
	case K_CTRL_TAB:       doc_list -> OnWindowNext(); return true;
	case K_SHIFT_CTRL_TAB: doc_list -> OnWindowPrev(); return true;
	}
	return false;
}

#if 0
DocPokus::DocPokus()
{
}

DocPokus::~DocPokus()
{
}

void DocPokus::RegisterType()
{
	static DocType pokus = DocType(CreateDoc)
		.DocName("Pokusný dokument")
		.Filter("Pokusy", "*.pok");

	static DocType other = DocType(CreateDoc)
		.DocName("Ten samy dokument")
		.Filter("Prdy", "*.prd");

	DocType::Add(pokus);
	DocType::Add(other);
}

DocCtrl *DocPokus::CreateDoc()
{
	return new DocPokus;
}
#endif

END_UPP_NAMESPACE
