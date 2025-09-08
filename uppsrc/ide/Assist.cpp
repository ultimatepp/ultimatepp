#include "ide.h"

#if 0
#define LDUMP(x)     DDUMP(x)
#define LDUMPC(x)    DDUMPC(x)
#define LLOG(x)      DLOG(x)
#else
#define LDUMP(x)
#define LDUMPC(x)
#define LLOG(x)
#endif

#define LTIMING(x) // DTIMING(x)

AssistEditor::AssistEditor()
{
	assist_display.editor = this;
	assist.NoHeader();
	assist.NoGrid();
	assist.AddRowNumColumn().Margin(0).SetDisplay(assist_display);
	assist.NoWantFocus();
	assist.WhenLeftClick = THISBACK(AssistInsert);
	type.NoHeader();
	type.NoGrid();
	type.AddColumn();
	type.WhenCursor = THISBACK(SyncAssist);
	type.NoWantFocus();
	popup.Horz(type, assist);
	popup.SetPos(2000);
	auto_assist = true;
	commentdp = false;

	SyncNavigatorPlacement();
	navigatorframe.Left(navigatorpane, HorzLayoutZoom(140));
	navigating = false;

	int cy = search.GetMinSize().cy;
	navigatorpane.Add(search.TopPos(0, cy).HSizePos(0, cy + 4));
	navigatorpane.Add(sortitems.TopPos(0, cy).RightPos(0, cy));
	navigatorpane.Add(scope_list.VSizePos(cy, 0).HSizePos());
	
	scope_list.Vert() << scope << list;
	scope_list.SetPos(1500);

	navigator = true;

	WhenAnnotationMove = THISBACK(SyncAnnotationPopup);
	WhenAnnotationClick = THISBACK1(EditAnnotation, true);
	WhenAnnotationRightClick = THISBACK1(EditAnnotation, false);
	Annotations(Zx(12));
	annotation_popup.Background(SColorPaper());
	annotation_popup.SetFrame(BlackFrame());
	annotation_popup.Margins(6);
	annotation_popup.NoSb();

	cachedpos = INT_MAX;
	cachedln = -1;

	parami = 0;

	param_info.Margins(2);
	param_info.Background(SColorPaper());
	param_info.SetFrame(BlackFrame());
	param_info.BackPaint();
	param_info.NoSb();

	include_assist = false;

	NoFindReplace();

	WhenUpdate << [=] {
		TriggerSyncFile(500);
	};
}

class IndexSeparatorFrameCls : public CtrlFrame {
	virtual void FrameLayout(Rect& r)                   { r.right -= 1; }
	virtual void FramePaint(Draw& w, const Rect& r) {
		w.DrawRect(r.right - 1, r.top, 1, r.Height(), SColorShadow);
	}
	virtual void FrameAddSize(Size& sz) { sz.cx += 2; }
};

void AssistEditor::TriggerSyncFile(int delay_ms)
{
	if(!theide)
		return;
	if(IsSourceFile(theide->editfile) || master_source.GetCount() || IsHeaderFile(theide->editfile)) {
		annotating = true;
		annotate_trigger.KillSet(delay_ms, [=] { SyncCurrentFile(); });
		ClearErrors();
	}
}

void AssistEditor::ClearErrors()
{
	errors.Clear();
	Errors(Vector<Point>());
	StatusImage(Null);
}

void AssistEditor::SyncNavigatorPlacement()
{
	int sz = navigatorframe.GetSize();
	if(navigator_right)
		navigatorframe.Right(navigatorpane, sz);
	else
		navigatorframe.Left(navigatorpane, sz);
}

AssistEditor::~AssistEditor()
{
	CancelAutoComplete();
	CancelCurrentFile();
}

void AssistEditor::CloseAssist()
{
	CancelAutoComplete();
	if(popup.IsOpen())
		popup.Close();
	if(annotation_popup.IsOpen())
		annotation_popup.Close();
	assist_item_ndx.Clear();
	assist_item.Clear();
	CloseTip();
}

void AssistEditor::PostInsert(int pos, const WString& s)
{
	if(pos < assist_cursor) {
		CancelAutoComplete();
		assist_cursor = -1;
	}
	CodeEditor::PostInsert(pos, s);
}

void AssistEditor::PostRemove(int pos, int size)
{
	if(pos < assist_cursor) {
		CancelAutoComplete();
		assist_cursor = -1;
	}
	CodeEditor::PostRemove(pos, size);
}

bool isincludefnchar(int c)
{
	return c && c != '<' && c != '>' && c != '?' &&
	       c != ' ' && c != '\"' && c != '/' && c != '\\' && c >= 32 && c < 65536;
}

String AssistEditor::ReadIdBackPos(int& pos, bool include)
{
	String id;
	bool (*test)(int c) = include ? isincludefnchar : iscid;
	while(pos > 0 && (*test)(GetChar(pos - 1)))
		pos--;
	int q = pos;
	while(q < GetLength64() && (*test)(GetChar(q)))
		id << (char)GetChar(q++);
	return id;
}

String AssistEditor::ReadIdBack(int q, bool include)
{
	return ReadIdBackPos(q, include);
}

void AssistEditor::DirtyFrom(int line)
{
	if(line >= cachedln) {
		cachedpos = INT_MAX;
		cachedline.Clear();
		cachedln = -1;
	}
	CodeEditor::DirtyFrom(line);
}

int AssistEditor::Ch(int i)
{
	if(i >= 0 && i < GetLength64()) {
		if(i < cachedpos || i - cachedpos > cachedline.GetCount()) {
			cachedln = GetLine(i);
			cachedline = GetWLine(cachedln);
			cachedpos = GetPos32(cachedln);
		}
		i -= cachedpos;
		return i < cachedline.GetCount() ? cachedline[i] : '\n';
	}
	return 0;
}

int AssistEditor::ParsBack(int q)
{
	int level = 1;
	--q;
	while(q > 0) {
		if(isrbrkt(Ch(q)))
			level++;
		if(islbrkt(Ch(q)))
			if(--level <= 0)
				break;
		--q;
	}
	return max(0, q);
}

bool IsSpc(int c)
{
	return c > 0 && c <= 32;
}

void AssistEditor::SkipSpcBack(int& q)
{
	while(q > 0 && IsSpc(Ch(q - 1)))
		q--;
}

String AssistEditor::IdBack(int& qq)
{
	String r;
	if(iscid(Ch(qq - 1))) {
		int q = qq;
		while(iscid(Ch(q - 1)))
			q--;
		if(iscib(Ch(q))) {
			qq = q;
			while(q < GetLength64() && iscid(Ch(q)))
				r.Cat(Ch(q++));
		}
	}
	return r;
}

String AssistEditor::CompleteIdBack(int& q, const Index<String>& locals)
{
	String id;
	for(;;) {
		SkipSpcBack(q);
		if(Ch(q - 1) == ',') {
			q--;
			id = ',' + id;
		}
		else
		if(Ch(q - 1) == '>') {
			q--;
			id = '>' + id;
		}
		else
		if(Ch(q - 1) == '<') {
			q--;
			id = '<' + id;
		}
		else
		if(Ch(q - 1) == ':' && Ch(q - 2) == ':') {
			q -= 2;
			id = "::" + id;
		}
		else {
			if(iscib(*id))
				break;
			String nid = IdBack(q);
			if(IsNull(nid))
				break;
			if(locals.Find(nid) >= 0 && findarg(*id, '<', '>') >= 0)
				return id.Mid(1);
			id = nid + id;
		}
	}
	return id;
}

void AssistEditor::SyncAssist()
{
	LTIMING("SyncAssist");
	String name = ReadIdBack(GetCursor32(), include_assist);
	String uname = ToUpper(name);
	assist_item_ndx.Clear();
	int typei = type.GetCursor() - 1;
	Buffer<bool> found(assist_item.GetCount(), false);
	VectorMap<String, int> found_name; // to accelerate resolving duplicities
	Index<String> cpp;
	for(int pass = 0; pass < 2; pass++) {
		for(int i = 0; i < assist_item.GetCount(); i++) {
			const AssistItem& m = assist_item[i];
			if(!found[i] &&
			   (typei < 0 || m.typei == typei) &&
			   ((pass ? m.uname.StartsWith(uname) : m.name.StartsWith(name)) || m.kind == KIND_ERROR)) {
			    int q = found_name.Find(m.name);
			    if(q >= 0) { // resolve duplicities
					int& ii = found_name[q];
					if(ii >= 0) { // lazy call to CppText
						const AssistItem& mm = assist_item[ii];
						cpp.FindAdd(CppText(mm.name, mm.pretty));
						ii = -1;
					}
					String g = CppText(m.name, m.pretty);
					if(cpp.Find(g) >= 0)
						continue;
					else
						cpp.Add(g);
			    }
				else
			        found_name.Add(m.name, i);
				found[i] = true;
				assist_item_ndx.Add(i);
			}
		}
	}
	assist.Clear();
	assist.SetVirtualCount(assist_item_ndx.GetCount());
}

bool AssistEditor::IncludeAssist()
{
	if(!theide)
		return false;
	Vector<String> include;
	String ln = GetUtf8Line(GetCursorLine());
	CParser p(ln);
	try {
		if(!p.Char('#') || !p.Id("include"))
			return false;
		if(p.Char('\"')) {
			include.Add(GetFileFolder(theide->editfile));
			include_local = true;
		}
		else {
			p.Char('<');
			include = SplitDirs(theide->GetIncludePath());
			include_local = false;
		}
		include_path.Clear();
		include_back = 0;
		if(include_local)
			while(p.Char3('.', '.', '/') || p.Char3('.', '.', '\\')) {
				include.Top() = GetFileFolder(include.Top());
				include_back++;
			}
		for(;;) {
			String dir;
			while(isincludefnchar(p.PeekChar()))
				dir.Cat(p.GetChar());
			if(dir.GetCount() && (p.Char('/') || p.Char('\\'))) {
				if(include_path.GetCount())
					include_path << '/';
				include_path << dir;
			}
			else
				break;
		}
	}
	catch(CParser::Error) {
		return false;
	}
	Vector<String> folder, upper_folder, file, upper_file;
	Index<String> done; // avoid duplicates
	for(int i = 0; i < include.GetCount(); i++) {
		FindFile ff(AppendFileName(AppendFileName(include[i], include_path), "*.*"));
		while(ff) {
			String fn = ff.GetName();
			if(done.Find(fn) < 0 && !ff.IsHidden()) {
				done.Add(fn);
				if(ff.IsFolder()) {
					folder.Add(fn);
					upper_folder.Add(ToUpper(fn));
				}
				else {
					static Index<String> ext(Split(".h;.hpp;.hh;.hxx;.i;.lay;.iml;.t;.dli", ';'));
					String fext = GetFileExt(fn);
					if(fext.GetCount() == 0 || ext.Find(ToLower(fext)) >= 0) {
						file.Add(fn);
						upper_file.Add(ToUpper(fn));
					}
				}
			}
			ff.Next();
		}
	}
	IndexSort(upper_folder, folder);
	Index<String> fnset;
	for(int i = 0; i < folder.GetCount(); i++) {
		String fn = folder[i];
		if(fnset.Find(fn) < 0) {
			fnset.Add(fn);
			AssistItem& f = assist_item.Add();
			f.name = f.pretty = fn;
			f.uname = ToUpper(f.name);
			f.kind = KIND_INCLUDEFOLDER;
		}
	}
	IndexSort(upper_file, file);
	for(int i = 0; i < file.GetCount(); i++) {
		String fn = file[i];
		AssistItem& f = assist_item.Add();
		f.name = f.pretty = fn;
		f.uname = ToUpper(f.name);
		static Index<String> hdr(Split(".h;.hpp;.hh;.hxx", ';'));
		String fext = GetFileExt(fn);
		f.kind = hdr.Find(ToLower(GetFileExt(fn))) >= 0 || fext.GetCount() == 0 ? KIND_INCLUDEFILE
		                                                                        : KIND_INCLUDEFILE_ANY;
		f.priority = 0;
	}
	include_assist = true;
	if(include_path.GetCount())
		include_path << "/";
	PopUpAssist();
	return true;
}

CurrentFileContext AssistEditor::CurrentContext(int pos)
{
	CurrentFileContext cfx;
	if(!theide)
		return cfx;
	cfx.filename = cfx.real_filename = NormalizePath(theide->editfile);
	cfx.includes = theide->GetCurrentIncludePath();
	cfx.defines = theide->GetCurrentDefines();
	if(!IsView() && GetLength() < 4000000) {
		cfx.content = Get(0, min(GetLength(), pos));
		if(!IsSourceFile(cfx.filename)) {
			if(master_source.GetCount()) {
				MakeIncludeTrick(cfx);
				cfx.filename = master_source;
			}
			else
			if(!IsHeaderFile(cfx.filename)) {
				cfx.content.Clear();
				cfx.is_source_file = false;
			}
		}
	}
	return cfx;
}

void AssistEditor::SetAnnotations(const CppFileInfo& f)
{
	ClearAnnotations();
	annotations.Clear();
	references.Clear();
	for(const AnnotationItem& m : f.items) {
		annotations.Add(m);
		SetAnnotation(m.pos.y,
		              GetRefLinks(m.id).GetCount() ? IdeImg::tpp_doc()
		                                           : IdeImg::tpp_pen(),
		              TppId(m.id));
	}
	references = clone(f.refs);
	locals = clone(f.locals);
	annotating = false;
	if(!navigator_global)
		Search();
	SyncCursor();
	SyncTip();
}

bool IgnoredError(const String& s)
{
	return s.Find("in file included from") >= 0 || s.Find("to match this '{'") >= 0;
}

void AssistEditor::SyncCurrentFile(const CurrentFileContext& cfx)
{
	if(!theide)
		return;
	if(cfx.content.GetCount() && HasLibClang())
		SetCurrentFile(cfx, [=](const CppFileInfo& f, const Vector<Diagnostic>& ds) {
			SetAnnotations(f);

			ClearErrors();
			if(!IsCurrentFileDirty() && theide && theide->IsProjectFile(theide->editfile)) {
				errors = clone(ds);
	
				Vector<Point> err;
	
				int di = 0;
				String path = NormalizePath(theide->editfile);
				if(path.TrimEnd(".icpp"))
					path << ".cpp";
				while(di < ds.GetCount() && err.GetCount() < 30) {
					int k = ds[di].kind;
					bool group_valid = false;
					auto Do = [&](const Diagnostic& d) {
						if(d.pos.y < 0 || IgnoredError(d.text) || path != d.path)
							return;
						Point pos = d.pos;
						FromUtf8x(pos);
						bool error = true;
						if(!IsSourceFile(path) && pos.y > GetLineCount() - 10) { // ignore errors after the end of header (e.g. missing })
							int line = pos.y;
							error = false;
							int xpos = pos.x;
							while(line < GetLineCount()) {
								String l = GetUtf8Line(line++);
								String s = TrimBoth(l);
								if(s.GetCount() && xpos < l.GetCount() && *s != '#')
									error = true;
								xpos = 0;
							}
						}
						if(!error)
							return;

						err.Add(pos);

						if(d.path == path)
							group_valid = true;
					};
					int q = err.GetCount();
					if(IsWarning(k) || IsError(k)) {
						Do(ds[di++]);
						while(di < ds.GetCount() && ds[di].detail)
							Do(ds[di++]);
					}
					if(!group_valid)
						err.Trim(q);
				}
				if(show_errors_status)
					StatusImage(err.GetCount() ? IdeImg::CurrentErrors() : IdeImg::CurrentOK());
				if(show_errors)
					Errors(pick(err));
			}
		});
	else {
		ClearErrors();
		CppFileInfo none;
		SetAnnotations(none);
	}
}

void AssistEditor::SetQTF(CodeEditor::MouseTip& mt, const String& qtf)
{
	mt.value = qtf;
	mt.display = &QTFDisplay();

	RichText txt = ParseQTF(qtf);
	txt.ApplyZoom(GetRichTextStdScreenZoom());
	mt.sz.cx = min(mt.sz.cx, txt.GetWidth() + DPI(2));
	mt.sz.cy = txt.GetHeight(Upp::Zoom(1, 1), mt.sz.cx) + DPI(2);
}

bool AssistEditor::DelayedTip(CodeEditor::MouseTip& mt)
{
	if(annotating || !theide)
		return false;
	if(GetChar(mt.pos) <= 32)
		return false;
	String name;
	Point ref_pos;
	String ref_id = theide->GetRefId(mt.pos, name, ref_pos);
	if(ref_id.GetCount() == 0 || IsNull(name))
		return false;

	int lp = mt.pos;
	int li = GetLinePos(lp);

	bool found_local = false;
	AnnotationItem m, m1;

	AnnotationItem cm = FindCurrentAnnotation(); // what function body are we in?
	if(IsFunction(cm.kind)) { // do local variables
		for(const AnnotationItem& lm : locals) {
			int ppy = -1;
			if(lm.id == ref_id && lm.pos.y >= cm.pos.y && lm.pos.y <= li && lm.pos.y > ppy) {
				ppy = lm.pos.y;
				m = lm;
				found_local = true;
				if(ref_pos == lm.pos)
					break;
			}
		}
	}

	if(!found_local) {
		for(const auto& f : ~CodeIndex())
			for(const AnnotationItem& q : f.value.items)
				if(q.id == ref_id)
					(q.definition ? m1 : m) = q;
		
		if(m.id.GetCount() == 0)
			m = m1;
	}
	
	if(m.id.GetCount() == 0)
		return false;
	
	String qtf = "[g ";
	if(m.nest.GetCount())
		qtf << "[@b* \1" << m.nest << "::\1]&";

	String tl = BestTopic(GetRefLinks(ref_id));
	if(tl.GetCount()) {
		RichText txt = GetCodeTopic(tl, ref_id);
		qtf << AsQTF(txt);
	}
	else
		qtf << SignatureQtf(m.name, m.pretty);

	SetQTF(mt, qtf);
	mt.background = AdjustIfDark(Color(245, 255, 221));

	return true;
}

bool AssistEditor::AssistTip(CodeEditor::MouseTip& mt)
{
	if(assist.IsOpen() || !theide)
		return false;
	int p = mt.pos;
	int line = GetLinePos(p);
	Point pos(p, line);
	ToUtf8x(pos);
	int di = 0;
	String path = NormalizePath(theide->editfile);
	if(path.TrimEnd(".icpp"))
		path << ".cpp";
	while(di < errors.GetCount()) {
		int k = errors[di].kind;
		int ii0 = di;
		bool found = false;
		if(IsWarning(k) || IsError(k)) {
			if(errors[di].path == path && errors[di].pos == pos && !IgnoredError(errors[di].text))
				found = true;
			di++;
			while(di < errors.GetCount() && errors[di].detail) {
				if(errors[di].path == path && errors[di].pos == pos && !IgnoredError(errors[di].text))
					found = true;
				di++;
			}
		}
		if(found) {
			String qtf = "[g ";
			for(int i = ii0; i < di; i++) {
				Diagnostic& d = errors[i];
				if(d.pos.y < 0 || IgnoredError(d.text))
					continue;
				if(i > ii0)
					qtf << "&";
				qtf << "[";
				if(d.path == path && d.pos == pos)
					qtf << "*";
				qtf << " ";
				qtf << "[@B \1" << GetFileName(d.path) << "\1] " << d.pos.y << ": ";
				if(IsWarning(d.kind))
					qtf << "[@m warning: ]";
				if(IsError(d.kind))
					qtf << "[@r error: ]";
				qtf << "\1" << d.text << "\1";
				qtf << "]";
			}
			
			SetQTF(mt, qtf);
			mt.background = AdjustIfDark(Color(255, 234, 207));
			return true;
		}
	}
	return false;
}

void AssistEditor::SyncCurrentFile()
{
	if(is_source_file) {
		CurrentFileContext cfx = CurrentContext();
		SyncCurrentFile(cfx);
	}
}

void AssistEditor::NewFile(bool reloading)
{
	annotations.Clear();
	references.Clear();
	Search();
	SyncMaster();
	CurrentFileContext cfx = CurrentContext();
	ClearErrors();

	if(!reloading)
		is_source_file = cfx.is_source_file;

	if(is_source_file) {
		annotating = true;
		int q = CodeIndex().Find(cfx.real_filename);
		if(q >= 0) {
			const FileAnnotation& f = CodeIndex()[q];
			SetAnnotations(f);
			if(f.defines == cfx.defines && f.includes == cfx.includes && f.time >= GetFileTime(cfx.real_filename)) {
				annotating = false;
				PutAssist(cfx.real_filename + " annotations loaded from index");
			}
		}
		SyncCurrentFile(cfx);
	}
}

int  AssistEditor::ToUtf8x(int line, int pos)
{ // libclang treats utf-8 bytes as whole characters
	if(line < GetLineCount() && GetLineLength(line) < 1000) {
		WString w = GetWLine(line);
		if(pos <= w.GetCount())
	        return w.Mid(0, pos).ToString().GetCount();
	}
	return pos;
}

int AssistEditor::FromUtf8x(int line, int pos)
{ // libclang treats utf-8 bytes as whole characters
	if(line < GetLineCount() && GetLineLength(line) < 1000) {
		const String& h = GetUtf8Line(line);
		if(pos <= h.GetCount())
			for(const char ch : h)
				if((byte)ch >= 128)
					return h.Mid(0, pos).ToWString().GetCount();
	}
	return pos;
}

void AssistEditor::Assist(bool macros)
{
	LTIMING("Assist");
	CloseAssist();
	assist_type.Clear();
	assist_item.Clear();
	include_assist = false;

	int pos = GetCursor();
	ReadIdBackPos(pos, false); // libclang does not work well if file is not truncated for autocomplete (?)
	assist_cursor = pos;

	if(IncludeAssist())
		return;

	CurrentFileContext cfx = CurrentContext(pos);
	int line = GetLinePos(pos);
	if(cfx.content.GetCount() && HasLibClang())
		StartAutoComplete(cfx, line + cfx.line_delta + 1, ToUtf8x(line, pos) + 1, macros, [=](const Vector<AutoCompleteItem>& items) {
			bool has_globals = false;
			bool has_macros = false;
			for(const AutoCompleteItem& m : items) {
				if(m.kind == CXCursor_MacroDefinition)
					has_macros = true;
				else
				if(IsNull(m.parent))
					has_globals = true;
			}
			if(has_macros)
				assist_type.Add("<macros>");
			if(has_globals)
				assist_type.Add(Null);
			for(const AutoCompleteItem& m : items)
				if(!m.name.StartsWith("dv___")) {
					AssistItem& f = assist_item.Add();
					(AutoCompleteItem&)f = m;
					f.uname = ToUpper(f.name);
					f.typei = assist_type.FindAdd(f.kind == CXCursor_MacroDefinition ? "<macros>" : f.parent);
				}
			PopUpAssist();
		});
}

Ptr<Ctrl> AssistEditor::assist_ptr;

bool AssistEditor::WheelHook(Ctrl *, bool inframe, int event, Point p, int zdelta, dword keyflags)
{
	if(!inframe && event == MOUSEWHEEL && assist_ptr && assist_ptr->IsOpen()) {
		assist_ptr->MouseWheel(p, zdelta, keyflags);
		return true;
	}
	return false;
}

void AssistEditor::PopUpAssist(bool auto_insert)
{
	LTIMING("PopUpAssist");
	int pos = GetCursor();
	ReadIdBackPos(pos, false);
	if(pos != assist_cursor)
		return;
		
	if(assist_item.GetCount() == 0) {
		if(no_empty_autocomplete)
			return;
		AssistItem& m = assist_item.Add();
		m.kind = KIND_ERROR;
		m.pretty = "No relevant autocomplete info found";
	}
	Upp::Sort(assist_item, [=](const AssistItem& a, const AssistItem& b) {
		return CombineCompare(a.priority, b.priority)(a.uname, b.uname) < 0;
	});
	int lcy = max(16, BrowserFont().Info().GetHeight());
	type.Clear();
	type.Add(AttrText("<all>").Ink(SColorHighlight()));
	if(assist_type.GetCount() == 0)
		popup.Zoom(1);
	else {
		for(int i = 0; i < assist_type.GetCount(); i++) {
			String s = assist_type[i];
			if(s[0] == ':' && s[1] == ':')
				s = s.Mid(2);
			s = Nvl(s, "<globals>");
			if(s[0] == '<')
				type.Add(AttrText(s).Ink(SColorMark()));
			else
				type.Add(s);
		}
		popup.NoZoom();
	}
	type.SetCursor(0);
	LTIMING("PopUpAssist2");
	int cy = VertLayoutZoom(304);
	cy += HeaderCtrl::GetStdHeight();
	assist.SetLineCy(lcy);
	Point p = GetCaretPoint() + GetScreenView().TopLeft();
	Rect wa = GetWorkArea();
	int cx = min(wa.Width() - 100, Zx(800));
	if(p.x + cx > wa.right)
		p.x = wa.right - cx;
	popup.SetRect(RectC(p.x, p.y + cy + GetFontSize().cy < wa.bottom ? p.y + GetFontSize().cy : p.y - cy, cx, cy) & wa);
	popup.BackPaint();
	if(auto_insert && assist.GetCount() == 1) {
		assist.GoBegin();
		AssistInsert();
	}
	else {
		popup.Ctrl::PopUp(this, false, false, true);
		assist_ptr = &assist;
		ONCELOCK {
			InstallMouseHook(AssistEditor::WheelHook);
		}
	}
}

bool sILess(const String& a, const String& b)
{
	return ToUpper(a) < ToUpper(b);
}

void AssistEditor::Complete()
{
	if(!is_source_file)
		return;

	CloseAssist();

	int c = GetCursor32();
	String q = IdBack(c);

	Index<String> ids;
	int64 len = 0;
	for(int i = 0; i < GetLineCount() && len < 1000000; i++) {
		String x = GetUtf8Line(i);
		len += x.GetLength();
		CParser p(x);
		try {
			while(!p.IsEof())
				if(p.IsId()) {
					String h = p.ReadId();
					if(h != q)
						ids.FindAdd(h);
				}
				else
					p.SkipTerm();
		}
		catch(CParser::Error) {
			return;
		}
	}

	Vector<String> id = ids.PickKeys();
	Upp::Sort(id, sILess);

	if(q.GetCount()) {
		String h;
		for(int i = 0; i < id.GetCount(); i++) {
			String s = id[i];
			if(s.StartsWith(q)) {
				if(IsNull(h))
					h = s;
				else {
					s.Trim(min(s.GetCount(), h.GetCount()));
					for(int j = 0; j < s.GetCount(); j++)
						if(s[j] != h[j]) {
							h.Trim(j);
							break;
						}
				}
			}
		}
		if(h.GetCount() > q.GetCount())
			Paste(h.Mid(q.GetCount()).ToWString());
	}
	for(int i = 0; i < id.GetCount(); i++) {
		AssistItem& f = assist_item.Add();
		f.name = f.pretty = id[i];
		f.kind = KIND_COMPLETE;
	}
	assist_type.Clear();
	PopUpAssist(true);
}

void AssistEditor::Abbr()
{
	CloseAssist();
	int c = GetCursor32();
	int ch;
	String s;
	while(IsAlpha(ch = Ch(c - 1))) {
		s.Insert(0, ch);
		--c;
	}
	int len = s.GetCount();
	s = theide->abbr.Get(s, String());
	if(IsNull(s))
		return;
	NextUndo();
	SetCursor(c);
	Remove(c, len);
	int linepos = c;
	int line = GetLinePos32(linepos);
	WString h = GetWLine(line).Mid(0, linepos);
	for(int i = 0; i < s.GetCount(); i++) {
		ch = s[i];
		switch(ch) {
		case '@':
			c = GetCursor32();
			break;
		case '\n':
			InsertChar('\n');
			for(int j = 0; j < h.GetCount(); j++)
				InsertChar(h[j]);
			break;
		default:
			if((byte)s[i] >= ' ' || s[i] == '\t')
				InsertChar(s[i]);
			break;
		}
	}
	SetCursor(c);
}

void AssistEditor::AssistInsert()
{
	if(assist.IsCursor()) {
		int ii = assist.GetCursor();
		if(ii < 0 || ii >= assist_item_ndx.GetCount())
			return;
		ii = assist_item_ndx[ii];
		if(ii >= assist_item.GetCount()) {
			CloseAssist();
			IgnoreMouseUp();
			return;
		}
		const AssistItem& f = assist_item[ii];
		if(include_assist) {
			int ln = GetLine(GetCursor32());
			int pos = GetPos32(ln);
			Remove(pos, GetLineLength(ln));
			SetCursor(pos);
			String h;
			for(int i = 0; i < include_back; i++)
				h << "../";
			Paste(ToUnicode(String().Cat() << "#include "
			                << (include_local ? "\"" : "<")
			                << h << include_path
			                << f.name
			                << (f.kind == KIND_INCLUDEFOLDER ? "/" : include_local ? "\"" : ">")
			                , CHARSET_WIN1250));
			if(f.kind == KIND_INCLUDEFOLDER) {
				Assist(false);
				IgnoreMouseUp();
				return;
			}
			else {
				String pkg = include_path.Left(include_path.GetCount() - 1);
				Vector<String> nests = GetUppDirs();
				for(int i = 0; i < nests.GetCount(); i++){
					if(FileExists(nests[i] + "/" + include_path + GetFileName(pkg) + ".upp")) {
						Ide *ide = dynamic_cast<Ide *>(TheIde());
						if(ide)
							ide->AddPackage(pkg);
						break;
					}
				}
			}
		}
		else {
			String txt = f.name;
			int param_count;
			ParsePretty(f.name, f.pretty, &param_count);
			if(param_count >= 0)
				txt << "()";
			if(f.pretty.EndsWith("::"))
				txt << "::";
			int cl = GetCursor32();
			int ch = cl;
			while(iscid(Ch(cl - 1)))
				cl--;
			while(iscid(Ch(ch)))
				ch++;
			Remove(cl, ch - cl);
			SetCursor(cl);
			Paste(ToUnicode(txt, CHARSET_WIN1250));
			if(param_count > 0) {
				SetCursor(GetCursor32() - 1);
				StartParamInfo(f, cl);
			}
		}
	}
	CloseAssist();
	IgnoreMouseUp();
}

bool AssistEditor::InCode()
{
	int pos = GetCursor32();
	int line = GetLinePos32(pos);
	One<EditorSyntax> st = GetSyntax(line);
	WString l = GetWLine(line);
	st->ScanSyntax(l, ~l + pos, line, GetTabSize());
	return st->CanAssist();
}

bool isaid(int c)
{
	return c == '~' || iscid(c);
}

bool AssistEditor::Key(dword key, int count)
{
	CloseTip();
	dword *k = IdeKeys::AK_DELLINE().key;
	if(key == k[0] || key == k[1]) {
		DeleteLine();
		return true;
	}
	k = IdeKeys::AK_CUTLINE().key;
	if(key == k[0] || key == k[1]) {
		CutLine();
		return true;
	}
#ifdef _DEBUG
	if(key == K_F12) {
		DLOG("==================");
		PPInfo ppi;
		VectorMap<String, Time> result;
		ArrayMap<String, Index<String>> define_includes;
		String includes = theide->GetCurrentIncludePath() + ";" + GetClangInternalIncludes();
		ppi.SetIncludes(includes);
		ppi.GatherDependencies(theide->editfile, result, define_includes);
		DDUMP(includes);
		DDUMPM(result);
		DDUMPM(define_includes);
	}
#endif
	if(popup.IsOpen()) {
		int k = key & ~K_CTRL;
		ArrayCtrl& kt = key & K_CTRL ? type : assist;
		if(k == K_UP || k == K_PAGEUP || k == K_CTRL_PAGEUP || k == K_CTRL_END) {
			if(kt.IsCursor())
				return kt.Key(k, count);
			else {
				kt.SetCursor(kt.GetCount() - 1);
				return true;
			}
		}
		if(k == K_DOWN || k == K_PAGEDOWN || k == K_CTRL_PAGEDOWN || k == K_CTRL_HOME) {
			if(kt.IsCursor())
				return kt.Key(k, count);
			else {
				kt.SetCursor(0);
				return true;
			}
		}
		if(findarg(key, K_ENTER, K_TAB) >= 0 && assist.GetCount()) {
			if(!assist.IsCursor())
				assist.GoBegin();
			AssistInsert();
			return true;
		}
	}
	int c = GetCursor32();
	int cc = GetChar(c);
	int bcc = c > 0 ? GetChar(c - 1) : 0;
	bool b = CodeEditor::Key(key, count);
	if(b && search.HasFocus())
		SetFocus();
	if(IsReadOnly() || IsRectSelection())
		return b;
	if(assist.IsOpen()) {
		bool (*test)(int c) = include_assist ? isincludefnchar : isaid;
		if(!(*test)(key) &&
		   !((*test)(cc) && (key == K_DELETE || key == K_RIGHT)) &&
		   !((*test)(bcc) && (key == K_LEFT || key == K_BACKSPACE))) {
			if(b) {
				CloseAssist();
				if(include_assist ? (key == '/' || key == '\\') : key == '.')
					Assist(false);
			}
		}
		else
			SyncAssist();
	}
	else
	if(auto_assist) {
		if(InCode()) {
			if(key == '>' && Ch(GetCursor32() - 2) == '-' || key == ':' && Ch(GetCursor32() - 2) == ':')
				Assist(false);
			else
			if(key == '.')  {
				String id;
				int pos = GetCursor() - 2;
				int n = 50;
				while(pos >= 0 && n-- >= 0) {
					int c = GetChar(pos);
					if(c > ' ') {
						if(iscib(*ReadIdBack(pos)) || c == ')' || c == ']') // where we can realistically expect autocomplete...
							Assist(false);
						break;
					}
					pos--;
				}
			}
		}
		if((key == '\"' || key == '<' || key == '/' || key == '\\') && GetUtf8Line(GetCursorLine()).StartsWith("#include"))
			Assist(false);
	}
	return b;
}

void AssistEditor::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(keyflags & K_CTRL)
		WhenFontScroll(sgn(zdelta));
	else
		if(assist.IsOpen())
			assist.MouseWheel(p, zdelta, keyflags);
		else
			CodeEditor::MouseWheel(p, zdelta, keyflags);
}

void AssistEditor::LeftDown(Point p, dword keyflags)
{
	CloseAssist();
	CodeEditor::LeftDown(p, keyflags);
}

void AssistEditor::ChildLostFocus()
{
	search.AddHistory();
}

void AssistEditor::LostFocus()
{
	CloseAssist();
}

bool AssistEditor::Esc()
{
	bool r = false;
	if(assist.IsOpen()) {
		CloseAssist();
		r = true;
	}
	for(int i = 0; i < PARAMN; i++)
		param[i].line = -1;
	if(param_info.IsOpen()) {
		param_info.Close();
		r = true;
	}
	return r;
}

void AssistEditor::SyncNavigatorShow()
{
	navigatorframe.Show(navigator && theide && !theide->designer && !theide->IsEditorMode());
}

void AssistEditor::Navigator(bool nav)
{
	navigator = nav;
	SyncNavigatorShow();
	if(IsNavigator())
		SetFocus();
	SyncNavigator();
	SyncCursor();
}

void AssistEditor::SyncNavigator()
{
	if(navigating)
		return;
	if(IsNavigator()) {
		Search();
		SyncCursor();
	}
	SyncNavigatorShow();
}

void AssistEditor::SelectionChanged()
{
	CodeEditor::SelectionChanged();
	SyncCursor();
	SyncParamInfo();
	WhenSelectionChanged();
}

void AssistEditor::SerializeNavigator(Stream& s)
{
	int version = 10;
	s / version;
	s % navigatorframe;
	s % navigator;
	if(version == 1 || version == 3) {
		Splitter dummy;
		s % dummy;
	}
	if(version >= 4 && version < 6) {
		Splitter dummy;
		s % dummy;
	}
	if(version >= 5)
		s % navigator_right;
	Navigator(navigator);

	if(s.IsLoading())
		SyncNavigatorPlacement();
	
	if(version >= 7)
		s % show_errors % show_errors_status;

	if(version >= 8)
		s % no_empty_autocomplete;
	
	if(version >= 9)
		s % ms_cache;
	
	if(version >= 10)
		s % scope_list;
}

void AssistEditor::SerializeNavigatorWorkspace(Stream& s)
{
	int version = 0;
	s / version;
	search.SerializeList(s);
}
