#include "ide.h"

class InsertColorDlg : public WithInsertColorLayout<TopWindow> {
	typedef InsertColorDlg CLASSNAME;

	String r[5];
	bool canceled = true;
	
	void Sync();
	void Select(int i);
	
	RGBACtrl rgbactrl;

public:
	String result;

	InsertColorDlg();
	bool IsCanceled();
};

void InsertColorDlg::Select(int i)
{
	result = r[i];
	canceled = false;
	Break(IDOK);
}

void InsertColorDlg::Sync()
{
	RGBA c0 = rgbactrl.GetColor();
	RGBA c = rgbactrl.Get();
	r[0] = Format("%d, %d, %d, %d", c.a, c.r, c.g, c.b);
	rgba.SetLabel(r[0]);
	r[1] = c.a == 255 ? Format("Color(%d, %d, %d)", c.r, c.g, c.b)
	                  : Format("%d * Color(%d, %d, %d)", c.a, c0.r, c0.g, c0.b);
	color.SetLabel(r[1]);
	r[2] = Format("%02x%02x%02x%02x", c.a, c.r, c.g, c.b);
	ahex.SetLabel(r[2]);
	r[3] = Format("%02x%02x%02x", c.r, c.g, c.b);
	hex.SetLabel(r[3]);
	r[4] = Format("(%d.%d.%d)", c.r, c.g, c.b);
	qtf.SetLabel(r[4]);
}

InsertColorDlg::InsertColorDlg()
{
	CtrlLayoutCancel(*this, "Insert color");
	rgbactrl <<= THISBACK(Sync);
	rgba <<= THISBACK1(Select, 0);
	color <<= THISBACK1(Select, 1);
	ahex <<= THISBACK1(Select, 2);
	hex <<= THISBACK1(Select, 3);
	qtf <<= THISBACK1(Select, 4);
	Sync();

	int m = color.GetPos().y.GetA();
	int cx = color.GetPos().x.GetA();
	int cy = rgbactrl.GetHeight(cx - 2 * m);
	Rect r = GetRect();
	r.bottom = r.top + cy + 2 * m;
	rgbactrl.SetRect(m, m, cx - 2 * m, cy);
	Add(rgbactrl);
	SetMinSize(r.GetSize());
	SetRect(r);
}

bool InsertColorDlg::IsCanceled()
{
	return canceled;
}

void Ide::InsertColor()
{
	if(editor.IsReadOnly())
		return;
	InsertColorDlg dlg;
	dlg.Execute();
	if (!dlg.IsCanceled())
		editor.Paste(dlg.result.ToWString());
}

void Ide::InsertLay(const String& fn)
{
	if(editor.IsReadOnly())
		return;
	String s;
	s << "#define LAYOUTFILE <" << fn << ">\n"
	  << "#include <CtrlCore/lay.h>\n";
	editor.Paste(s.ToWString());
}

void Ide::InsertIml(const Package& pkg, const String& fn, String classname)
{
	if(editor.IsReadOnly())
		return;
	if(!EditText(classname, "Insert .iml include", "Img class"))
		return;
	String h;
	h << "#define IMAGECLASS " << classname << "\n"
	  << "#define IMAGEFILE <" << fn << ">\n";
	editor.Paste((h + "#include <Draw/iml_header.h>\n").ToWString());
	ClearClipboard();
	int q = fn.ReverseFind('.');
	if(q >= 0) {
		String fn0 = fn.Mid(0, q);

		Index<String> done;
		auto Variant = [&](const char *add, const char *m) {
			if(done.Find(m) >= 0)
				return;
			String fn = fn0 + add + ".iml";
			for(int i = 0; i < pkg.GetCount(); i++) {
				if(fn.EndsWith('/' + pkg[i])) {
					h << "#define IMAGEFILE" << m << " <" << fn << ">\n";
					done.Add(m);
					break;
				}
			}
		};
		
		Variant("HD_DARK", "_DARK_UHD");
		Variant("HD_DK", "_DARK_UHD");
		Variant("HDDK", "_DARK_UHD");
		Variant("HDK", "_DARK_UHD");

		Variant("DARK_UHD", "_DARK_UHD");
		Variant("DK_HD", "_DARK_UHD");
		Variant("DK_UHD", "_DARK_UHD");
		Variant("DKHD", "_DARK_UHD");
		Variant("DKUHD", "_DARK_UHD");
	
		Variant("HD", "_UHD");
		
		Variant("DK", "_DARK");
		Variant("DARK", "_DARK");
	}
	
	h << "#include <Draw/iml_source.h>\n";
	
	AppendClipboardText(h);
	PromptOK("The .cpp part was saved to clipboard");
}

void Ide::InsertText(const String& text)
{
	if(editor.IsReadOnly())
		return;
	editor.Paste(text.ToWString());
}

String SelectInsertFile()
{
	return SelectFileOpen(
		"All files (*.*)\t*.*\n"
		"Graphics files (*.png *.bmp *.jpg *.jpeg *.gif *.ico *.svg)\t*.png *.bmp *.jpg *.jpeg *.gif *.ico *.svg\n"
		"Source files (*.cpp *.h *.hpp *.c *.C *.cc *.cxx *.icpp *.diff *.patch *.lay *.py *.pyc *.pyd *.pyo *.iml *.java *.lng *.sch *.usc *.dbg *.rc *.brc *.upt *.witz *.js)\t"
			"*.cpp *.h *.hpp *.c *.C *.cc *.cxx *.icpp *.diff *.patch *.lay *.py *.pyc *.pyd *.pyo *.iml *.java *.lng *.sch *.usc *.dbg *.rc *.brc *.upt *.witz *.js\n"
		"Web files (*.js *.css *.html *.htm *.htmls)\t*.js *.css *.html *.htm *.htmls\n"
		"Data files (*.csv *.xml *.json)\t*.csv *.xml *.json\n"
		"Text files (*.txt *.log *.info)\t*.txt *.log *.info\n"
		"Document files (*.xlsx *.xls *.doc *.qtf *.odt *.ods *.pdf)\t*.xlsx *.xls *.doc *.qtf *.odt *.ods *.pdf\n"
		"Compressed files (*.zip *.7z *.gz *.xz)\t*.zip *.7z *.gz *.xz\n"
	);
}

void Ide::InsertFilePath(bool c)
{
	if(editor.IsReadOnly())
		return;
	String path = SelectInsertFile();
	path.Replace("\\", "/");
	if(path.GetCount()) {
		if(c)
			path = AsCString(path);
		editor.Paste(path.ToWString());
	}
}


void Ide::InsertAs(const String& data)
{
	WithInsertAsLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Insert data");
	if(data.GetCount() > 20*1024)
		Exclamation("Data size is too big!&(Limit is 20KB.)");
	String f[6];
	f[0] = data;
	f[1] = Encode64(data);
	f[2] = data;
	f[3] = LZ4Compress(data);
	f[4] = ZCompress(data);
	f[5] = LZMACompress(data);
	for(int i = 0; i < 6; i++)
		dlg.format.SetLabel(i, dlg.format.GetLabel(i) + " (" + AsString(f[i].GetCount()) + ")");
	dlg.format <<= 0;
	if(dlg.Execute() != IDOK)
		return;
	int i = ~dlg.format;
	if(i < 0 || i >= 6)
		return;
	String d = f[i];
	WriteClipboardText(AsString(d.GetCount()));
	if(i == 0 || i == 1)
		editor.Paste(AsCString(d).ToWString());
	else {
		for(int i = 0; i < d.GetCount(); i += 256) {
			int e = min(i + 256, d.GetCount());
			String h;
			for(int j = i; j < e; j++)
				h << AsString((int)(byte)d[j]) << ',';
			h << '\n';
			editor.Paste(h.ToWString());
		}
	}
}

void Ide::InsertAs()
{
	if(editor.IsReadOnly())
		return;
	String txt = ReadClipboardText();
	if(txt.GetCount())
		InsertAs(txt);
}

void Ide::InsertFileBase64()
{
	if(editor.IsReadOnly())
		return;
	String path = SelectInsertFile();
	path.Replace("\\", "/");
	if(path.GetCount()) {
		if(GetFileLength(path) >= 20*1024) {
			Exclamation("File is too big!&(Limit is 20KB.)");
			return;
		}
		InsertAs(LoadFile(path));
	}
}

void Ide::InsertMenu(Bar& bar)
{
	if(bar.IsScanKeys())
		return;
	int pi = GetPackageIndex();
	const Workspace& wspc = IdeWorkspace();
	if(pi >= 0 && pi < wspc.GetCount()) {
		String pn = wspc[pi];
		const Package& p = wspc.GetPackage(pi);
		int n = 0;
		for(int i = 0; i < p.GetCount() && n < 12; i++) {
			String fn = p[i];
			String ext = ToLower(GetFileExt(fn));
			String pp = pn + '/' + fn;
			if(ext == ".lay") {
				bar.Add(fn + " include", THISBACK1(InsertLay, pp));
				n++;
			}
			if(ext == ".iml") {
				String c = GetFileTitle(fn);
				c.Set(0, ToUpper(c[0]));
				bar.Add(fn + " include", [=] { InsertIml(IdeWorkspace().GetPackage(pi), pp, c.EndsWith("Img") ? c : c + "Img"); });
				n++;
			}
			if(ext == ".tpp") {
				String s;
				s << "#define TOPICFILE <" << pp << "/all.i>\n"
				  << "#include <Core/topic_group.h>\n";
				bar.Add(fn + " include", THISBACK1(InsertText, s));
				n++;
			}
		}
		if(n > 0)
			bar.Separator();
	}
	bar.Add("Insert color..", THISBACK(InsertColor));
	bar.Add("Insert .iml Image..", [=] { InsertImage(); });
	bar.Add("Insert sequence..", THISBACK(InsertSequence));
	bar.Add("Insert file path..", THISBACK1(InsertFilePath, false));
	bar.Add("Insert file path as C string..", THISBACK1(InsertFilePath, true));
	bar.Add("Insert clipboard as..", [=] { InsertAs(); });
	bar.Add("Insert file as..", THISBACK(InsertFileBase64));
	bar.Add(IdeKeys::AK_INSERTDATE, [=] {
		Date d = GetSysDate();
		InsertText(Format("%d-%02d-%02d", d.year, d.month, d.day));
	});
	bar.Add(IdeKeys::AK_INSERTTIME, [=] {
		Time d = GetSysTime();
		InsertText(Format("%d-%02d-%02d %02d:%02d:%02d", d.year, d.month, d.day, d.hour, d.minute, d.second));
	});
	bar.Add(IdeKeys::AK_INSERTGUID, [=] {
		Uuid uuid;
		uuid.New();
		InsertText(Format(uuid));
	});
	bar.Add(IdeKeys::AK_INSERTGUID2, [=] {
		Uuid uuid;
		uuid.New();
		InsertText(FormatWithDashes(uuid));
	});
}

void Ide::InsertInclude(Bar& bar)
{
	if(bar.IsScanKeys()) // this takes time and contains no keys
		return;
	const Workspace& w = GetIdeWorkspace();
	String all;
	for(int i = 0; i < w.GetCount(); i++) {
		const Package& p = w.GetPackage(i);
		for(int j = 0; j < p.GetCount(); j++)
			if(findarg(ToLower(GetFileExt(p[j])), ".h", ".hpp") >= 0) {
				String h; h << "#include <" << w[i] << "/" << p[j] << '>';
				bar.Add(h, THISBACK1(InsertText, h + '\n'));
				all << h << '\n';
				break;
			}
	}
	bar.Add("All #includes", THISBACK1(InsertText, all));
}

void Ide::ToggleWordwrap()
{
	wordwrap = !wordwrap;
	SetupEditor();
}

void Ide::EditorMenu(Bar& bar)
{
	bar.Sub("Assist", [=](Bar& bar) { AssistMenu(bar); });
	Reformat(bar);
	InsertAdvanced(bar);
	bar.MenuSeparator();
	OnlineSearchMenu(bar);
    bar.Add(IsClipboardAvailableText() && (editor.IsSelection() || editor.GetLength() < 1024*1024),
            "Compare with clipboard..", [=]() {
        DiffDlg& dlg = CreateNewWindow<DiffDlg>();
        dlg.diff.left.RemoveFrame(dlg.p);
        dlg.diff.Set(ReadClipboardText(), editor.IsSelection() ? editor.GetSelection()
                                                               : editor.Get());
		dlg.Title("Compare with clipboard");
        dlg.OpenMain();
    });
	bar.MenuSeparator();
	editor.StdBar(bar);
}
