#include "ide.h"

class InsertColorDlg : public WithInsertColorLayout<TopWindow> {
	typedef InsertColorDlg CLASSNAME;

	String r[5];
	bool canceled;
	
	void Sync();
	void Select(int i);

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
	RGBA c = rgbactrl.Get();
	r[0] = Format("RGBA(%d, %d, %d, %d)", c.a, c.r, c.g, c.b);
	rgba.SetLabel(r[0]);
	r[1] = Format("Color(%d, %d, %d)", c.r, c.g, c.b);
	color.SetLabel(r[1]);
	r[2] = Format("%02x%02x%02x%02x", c.a, c.r, c.g, c.b);
	ahex.SetLabel(r[2]);
	r[3] = Format("%02x%02x%02x", c.r, c.g, c.b);
	hex.SetLabel(r[3]);
	r[4] = Format("(%d.%d.%d)", c.r, c.g, c.b);
	qtf.SetLabel(r[4]);
}

InsertColorDlg::InsertColorDlg() : canceled(true)
{
	CtrlLayoutCancel(*this, "Insert color");
	rgbactrl <<= THISBACK(Sync);
	rgba <<= THISBACK1(Select, 0);
	color <<= THISBACK1(Select, 1);
	ahex <<= THISBACK1(Select, 2);
	hex <<= THISBACK1(Select, 3);
	qtf <<= THISBACK1(Select, 4);
	Sync();
}

bool InsertColorDlg::IsCanceled()
{
	return canceled;
}

void Ide::InsertColor()
{
	InsertColorDlg dlg;
	dlg.Execute();
	if (!dlg.IsCanceled())
		editor.Paste(dlg.result.ToWString());
}

void Ide::InsertLay(const String& fn)
{
	String s;
	s << "#define LAYOUTFILE <" << fn << ">\n"
	  << "#include <CtrlCore/lay.h>\n";
	editor.Paste(s.ToWString());
}

void Ide::InsertIml(const String& fn, String classname)
{
	if(!EditText(classname, "Insert .iml include", "Img class"))
		return;
	String h;
	h << "#define IMAGECLASS " << classname << "\n"
	  << "#define IMAGEFILE <" << fn << ">\n"
	  << "#include <Draw/iml";
	editor.Paste((h + "_header.h>\n").ToWString());
	ClearClipboard();
	AppendClipboardText((h + "_source.h>\n"));
	PromptOK("The .cpp part was saved to clipboard");
}

void Ide::InsertText(const String& text)
{
	editor.Paste(text.ToWString());
}

void Ide::InsertCString()
{
	String txt = ReadClipboardText();
	if(txt.GetCount())
		editor.Paste(AsCString(txt).ToWString());
}

void Ide::InsertFilePath()
{
	String path = SelectFileOpen("All files\t*.*");
	path.Replace("\\", "/");
	if(path.GetCount())
		editor.Paste(path.ToWString());
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
				bar.Add(fn + " include", THISBACK2(InsertIml, pp, c.EndsWith("Img") ? c : c + "Img"));
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
	bar.Add("Insert file path..", THISBACK(InsertFilePath));
}

void Ide::EditorMenu(Bar& bar)
{
	bar.Add("Insert", THISBACK(InsertMenu));
	bar.MenuSeparator();
	editor.StdBar(bar);
}
