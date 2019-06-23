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

	int m = color.GetRect().top;
	int cx = color.GetRect().left - 2 * m;
	int cy = rgbactrl.GetHeight(cx);
	Rect r = GetRect();
	r.bottom = r.top + cy + 2 * m;
	rgbactrl.SetRect(m, m, cx, cy);
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

void Ide::InsertCString()
{
	if(editor.IsReadOnly())
		return;
	String txt = ReadClipboardText();
	if(txt.GetCount())
		editor.Paste(AsCString(txt).ToWString());
}

void Ide::InsertFilePath(bool c)
{
	if(editor.IsReadOnly())
		return;
	String path = SelectFileOpen("All files\t*.*");
	path.Replace("\\", "/");
	if(path.GetCount()) {
		if(c)
			path = AsCString(path);
		editor.Paste(path.ToWString());
	}
}

void Ide::InsertMenu(Bar& bar)
{
	if(bar.IsScanKeys())
		return;
	bar.Add("Insert color..", THISBACK(InsertColor));
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
	}
	if(editfile.GetCount()) {
		Parser ctx;
		editor.Context(ctx, editor.GetCursor());
		if(!IsNull(ctx.current_scope) && ctx.current_scope != "::" && !ctx.IsInBody()) {
			String s = "typedef " + ctx.current_scope + " CLASSNAME;";
			bar.Add(s, THISBACK1(InsertText, s));
		}
	}
	bar.Add("Insert clipboard as C string", THISBACK(InsertCString));
	bar.Add("Insert file path..", THISBACK1(InsertFilePath, false));
	bar.Add("Insert file path as C string..", THISBACK1(InsertFilePath, true));
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
	InsertAdvanced(bar);
	bar.MenuSeparator();
	OnlineSearchMenu(bar);
    bar.Add(IsClipboardAvailableText() && (editor.IsSelection() || editor.GetLength() < 1024*1024),
            "Compare with clipboard..", [=]() {
        DiffDlg dlg;
        dlg.diff.left.RemoveFrame(dlg.p);
        dlg.diff.Set(ReadClipboardText(), editor.IsSelection() ? editor.GetSelection()
                                                               : editor.Get());
		dlg.Title("Compare with clipboard");
        dlg.Run();
    });
	bar.MenuSeparator();
	editor.StdBar(bar);
}
