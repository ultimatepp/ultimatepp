#include "ide.h"

EscValue Ide::MacroEditor()
{
	EscValue out;
	out.Escape("GetLength()", THISBACK(MacroGetLength));
	out.Escape("GetLineCount()", THISBACK(MacroGetLineCount));
	out.Escape("GetLinePos(line)", THISBACK(MacroGetLinePos));
	out.Escape("GetLineLength(line)", THISBACK(MacroGetLineLength));
	out.Escape("GetCursor()", THISBACK(MacroGetCursor));
	out.Escape("GetLine(pos)", THISBACK(MacroGetLine));
	out.Escape("GetColumn(pos)", THISBACK(MacroGetColumn));
	out.Escape("GetSelBegin()", THISBACK(MacroGetSelBegin));
	out.Escape("GetSelCount()", THISBACK(MacroGetSelCount));
	out.Escape("SetCursor(pos)", THISBACK(MacroSetCursor));
	out.Escape("SetSelection(begin, count)", THISBACK(MacroSetSelection));
	out.Escape("ClearSelection()", THISBACK(MacroClearSelection));
	out.Escape("Get(...)", THISBACK(MacroGet));
	out.Escape("Remove(...)", THISBACK(MacroRemove));
	out.Escape("Insert(...)", THISBACK(MacroInsert));
	out.Escape("Find(...)", THISBACK(MacroFind));
	out.Escape("FindMatchingBrace(pos)", THISBACK(MacroFindMatchingBrace));
    out.Escape("FindClosingBrace(pos)", THISBACK(MacroFindClosingBrace));
    out.Escape("FindOpeningBrace(pos)", THISBACK(MacroFindOpeningBrace));
    out.Escape("Replace(...)", THISBACK(MacroReplace));
	out.Escape("MoveLeft(...)", THISBACK(MacroMoveLeft));
	out.Escape("MoveRight(...)", THISBACK(MacroMoveRight));
	out.Escape("MoveWordLeft(...)", THISBACK(MacroMoveWordLeft));
	out.Escape("MoveWordRight(...)", THISBACK(MacroMoveWordRight));
	out.Escape("MoveUp(...)", THISBACK(MacroMoveUp));
	out.Escape("MoveDown(...)", THISBACK(MacroMoveDown));
	out.Escape("MoveHome(...)", THISBACK(MacroMoveHome));
	out.Escape("MoveEnd(...)", THISBACK(MacroMoveEnd));
	out.Escape("MovePageUp(...)", THISBACK(MacroMovePageUp));
	out.Escape("MovePageDown(...)", THISBACK(MacroMovePageDown));
	out.Escape("MoveTextBegin(...)", THISBACK(MacroMoveTextBegin));
	out.Escape("MoveTextEnd(...)", THISBACK(MacroMoveTextEnd));

	out.Escape("EditFile(...)", THISBACK(MacroEditFile));
	out.Escape("SaveCurrentFile()", THISBACK(MacroSaveCurrentFile));
	out.Escape("CloseFile()",THISBACK(MacroCloseFile));
	out.Escape("FileName()", THISBACK(MacroFileName));

	out.Escape("Input(...)", THISBACK(MacroInput));
	out.Escape("ClearConsole()", THISBACK(MacroClearConsole));
	out.Escape("Echo(...)", THISBACK(MacroEcho));

	out.Escape("Build(...)", THISBACK(MacroBuild));
	out.Escape("BuildProject(...)", THISBACK(MacroBuildProject));
	out.Escape("Execute(cmdline)", THISBACK(MacroExecute));
	out.Escape("Launch(cmdline)", THISBACK(MacroLaunch));

	out.Escape("MainPackage()",THISBACK(MacroMainPackage));
	out.Escape("ActivePackage()",THISBACK(MacroActivePackage));
	out.Escape("PackageDir(...)",THISBACK(MacroPackageDir));
	out.Escape("ProjectDir()", THISBACK(MacroPackageDir)); // BW compatibility
	out.Escape("Assembly()",THISBACK(MacroAssembly));
	out.Escape("BuildMethod()",THISBACK(MacroBuildMethod));
	out.Escape("BuildMode()",THISBACK(MacroBuildMode));
	out.Escape("Flags()",THISBACK(MacroFlags));
	out.Escape("PackageFiles(...)",THISBACK(MacroPackageFiles));
	out.Escape("AllPackages()",THISBACK(MacroAllPackages));
	out.Escape("Target()", THISBACK(MacroTarget));

	return out;
}

void Ide::MacroGetLength(EscEscape& e)
{
	e = editor.GetLength();
}

void Ide::MacroGetLineCount(EscEscape& e)
{
	e = editor.GetLineCount();
}

void Ide::MacroGetLinePos(EscEscape& e)
{
	e = editor.GetPos(e.Int(0));
}

void Ide::MacroGetLineLength(EscEscape& e)
{
	e = editor.GetLineLength(e.Int(0));
}

void Ide::MacroGetCursor(EscEscape& e)
{
	e = editor.GetCursor();
}

void Ide::MacroGetLine(EscEscape& e)
{
	e = editor.GetLine(e.Int(0));
}

void Ide::MacroGetColumn(EscEscape& e)
{
	int pos = e.Int(0);
	editor.GetLinePos(pos);
	e = pos;
}

void Ide::MacroGetSelBegin(EscEscape& e)
{
	int l, h;
	if(editor.GetSelection(l, h))
		e = l;
	else
		e = editor.GetCursor();
}

void Ide::MacroGetSelEnd(EscEscape& e)
{
	int l, h;
	if(editor.GetSelection(l, h))
		e = h;
	else
		e = editor.GetCursor();
}

void Ide::MacroGetSelCount(EscEscape& e)
{
	int l, h;
	if(editor.GetSelection(l, h))
		e = h - l;
	else
		e = 0.0;
}

void Ide::MacroSetCursor(EscEscape& e)
{
	editor.SetCursor(e.Int(0));
}

void Ide::MacroSetSelection(EscEscape& e)
{
	int b = e.Int(0), c = e.Int(1);
	if(b < 0 || b > editor.GetLength() || c < 0 || c > editor.GetLength() || b + c > editor.GetLength())
		e.ThrowError(Format("invalid selection: begin = %d, count = %d (text length = %d)",
			b, c, editor.GetLength()));
	editor.SetSelection(b, b + c);
}

void Ide::MacroClearSelection(EscEscape& e)
{
	editor.ClearSelection();
}

void Ide::MacroGet(EscEscape& e)
{
	if(e.GetCount() < 1 || e.GetCount() > 2)
		e.ThrowError("wrong number of arguments in call to Get (1 or 2 expected)");
	int pos = e.Int(0);
	int count = e.GetCount() > 1 ? e.Int(1) : 1;
	if(pos < 0 || pos > editor.GetLength() || count <= 0 || pos + count > editor.GetLength())
		e.ThrowError(Format("error in Get(%d, %d), text length = %d", pos, count, editor.GetLength()));
	e = editor.GetW(pos, count);
}

void Ide::MacroRemove(EscEscape& e)
{
	int c = e.GetCount();
	if(c > 2)
		e.ThrowError("wrong number of arguments in call to Remove (0 to 2 expected)");
	int len = editor.GetLength();
	int cur = editor.GetCursor();
	if(c == 0) {
		int l, h;
		if(editor.GetSelection(l, h))
			editor.Remove(l, h - l);
		else if(cur < len)
			editor.Remove(cur, 1);
	}
	else {
		int start = (c > 1 ? e.Int(0) : cur);
		int count = e.Int(c - 1);
		if(count < 0 || count > len || start < 0 || start + count > len)
			e.ThrowError(Format("cannot remove %d character(s) at position %d, text length is only %d",
				count, start, len));
		editor.Remove(start, count);
	}
}

void Ide::MacroInsert(EscEscape& e)
{
	int c = e.GetCount();
	if(c < 1 || c > 2)
		e.ThrowError("wrong number of arguments in call to Insert (1 or 2 expected)");
	WString text = e[c - 1];
	editor.Insert(c > 1 ? e.Int(0) : editor.GetCursor(), text);
}

void Ide::MacroFind(EscEscape& e)
{
	int n = e.GetCount();
	if(n < 1 || n > 6)
		e.ThrowError("wrong number of arguments in call to Find (1 to 5 expected)");
	CodeEditor::FindReplaceData d = editor.GetFindReplaceData();
	bool down = (n <= 1 || e.Int(1) > 0);
	d.wholeword = (n > 2 && e.Int(2) > 0);
	d.ignorecase = (n > 3 && e.Int(3) > 0);
	d.wildcards = (n > 4 && e.Int(4) > 0);
	d.find = e[0];
	editor.SetFindReplaceData(d);
	e = editor.Find(!down, false);
}

void Ide::MacroReplace(EscEscape& e)
{
	int n = e.GetCount();
	if(n < 2 || n > 5)
		e.ThrowError("wrong number of arguments in call to Find (2 to 6 expected)");
	CodeEditor::FindReplaceData d = editor.GetFindReplaceData();
	d.find = e[0];
	d.replace = e[1];
	d.wholeword = (n > 2 && e.Int(2) > 0);
	d.ignorecase = (n > 3 && e.Int(3) > 0);
	d.wildcards = (n > 4 && e.Int(4) > 0);
	d.samecase = (n > 5 && e.Int(5) > 0);
	editor.SetFindReplaceData(d);
	e = editor.BlockReplace();
}

void Ide::MacroFindMatchingBrace(EscEscape& e)
{
    int FindMatchingBrace(int pos);
}

void Ide::MacroFindClosingBrace(EscEscape& e)
{
    int FindClosingBrace(int pos);
}

void Ide::MacroFindOpeningBrace(EscEscape& e)
{
    int FindOpeningBrace(int pos);
}

void Ide::MacroMoveLeft(EscEscape& e)
{
	if(e.GetCount() > 1) e.ThrowError("MoveLeft(sel = false) takes at most 1 parameter");
	editor.MoveLeft(e.GetCount() > 0 && e.Int(0) > 0);
}

void Ide::MacroMoveRight(EscEscape& e)
{
	if(e.GetCount() > 1) e.ThrowError("MoveRight(sel = false) takes at most 1 parameter");
	editor.MoveRight(e.GetCount() > 0 && e.Int(0) > 0);
}

void Ide::MacroMoveUp(EscEscape& e)
{
	if(e.GetCount() > 1) e.ThrowError("MoveUp(sel = false) takes at most 1 parameter");
	editor.MoveUp(e.GetCount() > 0 && e.Int(0) > 0);
}

void Ide::MacroMoveDown(EscEscape& e)
{
	if(e.GetCount() > 1) e.ThrowError("MoveDown(sel = false) takes at most 1 parameter");
	editor.MoveDown(e.GetCount() > 0 && e.Int(0) > 0);
}

void Ide::MacroMoveHome(EscEscape& e)
{
	if(e.GetCount() > 1) e.ThrowError("MoveHome(sel = false) takes at most 1 parameter");
	editor.MoveHome(e.GetCount() > 0 && e.Int(0) > 0);
}

void Ide::MacroMoveEnd(EscEscape& e)
{
	if(e.GetCount() > 1) e.ThrowError("MoveEnd(sel = false) takes at most 1 parameter");
	editor.MoveEnd(e.GetCount() > 0 && e.Int(0) > 0);
}

void Ide::MacroMovePageUp(EscEscape& e)
{
	if(e.GetCount() > 1) e.ThrowError("MovePageUp(sel = false) takes at most 1 parameter");
	editor.MovePageUp(e.GetCount() > 0 && e.Int(0) > 0);
}

void Ide::MacroMovePageDown(EscEscape& e)
{
	if(e.GetCount() > 1) e.ThrowError("MovePageDown(sel = false) takes at most 1 parameter");
	editor.MovePageDown(e.GetCount() > 0 && e.Int(0) > 0);
}

void Ide::MacroMoveTextBegin(EscEscape& e)
{
	if(e.GetCount() > 1) e.ThrowError("MoveTextBegin(sel = false) takes at most 1 parameter");
	editor.MoveTextBegin(e.GetCount() > 0 && e.Int(0) > 0);
}

void Ide::MacroMoveTextEnd(EscEscape& e)
{
	if(e.GetCount() > 1) e.ThrowError("MoveTextEnd(sel = false) takes at most 1 parameter");
	editor.MoveTextEnd(e.GetCount() > 0 && e.Int(0) > 0);
}

void Ide::MacroMoveWordRight(EscEscape& e)
{
	if(e.GetCount() > 1) e.ThrowError("MoveWordRight(sel = false) takes at most 1 parameter");
	int p = editor.GetCursor();
	int b = p;
	int l = editor.GetLength();
	if(iscid(editor.GetChar(p)))
		while(p < l && iscid(editor.GetChar(p))) p++;
	else
		while(p < l && !iscid(editor.GetChar(p))) p++;
	if(e.GetCount() > 0 && e.Int(0) > 0)
		editor.SetSelection(b, p);
	else
		editor.SetCursor(p);
}

void Ide::MacroMoveWordLeft(EscEscape& e)
{
	if(e.GetCount() > 1) e.ThrowError("MoveWordLeft(sel = false) takes at most 1 parameter");
	int p = editor.GetCursor();
	if(p == 0) return;
	int b = p;
	if(iscid(editor.GetChar(p - 1)))
		while(p > 0 && iscid(editor.GetChar(p - 1))) p--;
	else
		while(p > 0 && !iscid(editor.GetChar(p - 1))) p--;
	if(e.GetCount() > 0 && e.Int(0) > 0)
		editor.SetSelection(p, b);
	else
		editor.SetCursor(p);
}

void Ide::MacroInput(EscEscape& e)
{
	TopWindow dialog;
	Vector<String> tags;
	tags.SetCount(max(e.GetCount(), 1));
	int xdim = 0;
	enum {
		SIDE_GAP = 4,
		LINE_DIST = 22,
		LINE_HEIGHT = 19,
		BUTTON_HEIGHT = 22,
		BUTTON_WIDTH = 80,
	};
	for(int i = 0; i < e.GetCount(); i++)
		xdim = max(xdim, GetTextSize(tags[i] = e[i], StdFont()).cx + 10);
	dialog.HCenterPos(xdim + 200, 0).VCenterPos(LINE_DIST * tags.GetCount() + 2 * SIDE_GAP + BUTTON_HEIGHT, 0);
	Array<Label> label;
	Array<EditField> editors;
	for(int i = 0; i < tags.GetCount(); i++) {
		Label& lbl = label.Add();
		lbl.SetLabel(tags[i]);
		lbl.LeftPos(SIDE_GAP, xdim).TopPos(SIDE_GAP + LINE_DIST * i, LINE_HEIGHT);
		dialog << lbl;
		EditField& fld = editors.Add();
		fld.HSizePos(SIDE_GAP + xdim, SIDE_GAP).TopPos(SIDE_GAP + LINE_DIST * i, LINE_HEIGHT);
		dialog << fld;
	}
	Button ok, cancel;
	ok.SetLabel("OK") <<= dialog.Acceptor(IDOK);
	cancel.SetLabel("Cancel") <<= dialog.Rejector(IDCANCEL);
	dialog << ok.Ok().BottomPos(SIDE_GAP, BUTTON_HEIGHT).RightPos(2 * SIDE_GAP + BUTTON_WIDTH, BUTTON_WIDTH)
		<< cancel.Cancel().BottomPos(SIDE_GAP, BUTTON_HEIGHT).RightPos(SIDE_GAP, BUTTON_WIDTH);
	dialog.Title("Macro input box");
	if(dialog.Run() != IDOK) {
		e = EscValue();
		return;
	}
	if(tags.GetCount() == 1)
		e = (WString)~editors[0];
	else {
		EscValue out;
		for(int i = 0; i < tags.GetCount(); i++)
			out.ArrayAdd((WString)~editors[i]);
		e = out;
	}
}

void Ide::MacroBuild(EscEscape& e)
{
	String outfile;
	String maincfg = mainconfigparam;
	switch(e.GetCount()) {
		case 2: outfile = e[1]; break;
		case 1: maincfg = e[0]; break;
		case 0: break;
		default: e.ThrowError("Build: 0 to 2 arguments expected ([maincfg[, outfile]])");
	}
	e = Build(IdeWorkspace(), maincfg, outfile, false);
}

void Ide::MacroBuildProject(EscEscape& e)
{
	String outfile;
	switch(e.GetCount()) {
		case 3: outfile = e[2];
		case 2: break;
		default:  e.ThrowError("BuildProject: 2 or 3 arguments expected (uppfile, maincfg[, outfile])");
	}
	String uppfile = e[0];
	String maincfg = e[1];
	Workspace wspc;
	wspc.Scan(uppfile);
	e = Build(wspc, maincfg, outfile, false);
}

void Ide::MacroExecute(EscEscape& e)
{
	int time = msecs();
	String cmdline = e[0];
	Host h;
	CreateHostRunDir(h);
	h.ChDir(Nvl(rundir, GetFileFolder(target)));
	ShowConsole();
	PutConsole(String().Cat() << "MacroExecute: " << cmdline);
	console.Sync();
	e = h.Execute(cmdline);
	PutVerbose("Finished in " + GetPrintTime(time));
}

void Ide::MacroLaunch(EscEscape& e)
{
	String cmdline = e[0];
	Host h;
	CreateHostRunDir(h);
	h.ChDir(Nvl(rundir, GetFileFolder(target)));
	h.Launch(cmdline);
}

void Ide::MacroClearConsole(EscEscape& e)
{
	console.Clear();
}

void Ide::MacroEditFile(EscEscape& e)
{
	String filename;
	if(e.GetCount() == 1)
		filename = e[0];
	else if(e.GetCount() == 2)
		filename = SourcePath(e[0], e[1]);
	EditFile(filename);
}

void Ide::MacroSaveCurrentFile(EscEscape& e)
{
	SaveFile();
}

void Ide::MacroFileName(EscEscape& e)
{
	e = editfile;
}

void Ide::MacroMainPackage(EscEscape& e)
{
	e = GetMain();
}

void Ide::MacroActivePackage(EscEscape& e)
{
	e = GetActivePackage();
}

void Ide::MacroPackageDir(EscEscape& e)
{
	String pkg;
	if(e.GetCount() == 0)
		pkg = GetActivePackage();
	else
		pkg = e[0];
	if(!FileExists(PackageFileA(pkg)))
		e.ThrowError("PackageDir: Package not found.");
	e = PackageDirA(pkg);
}

void Ide::MacroAssembly(EscEscape& e)
{
	e = GetVarsName();
}

void Ide::MacroBuildMethod(EscEscape& e)
{
	e = method;
}

void Ide::MacroBuildMode(EscEscape& e)
{
	e = double(targetmode);
}

void Ide::MacroFlags(EscEscape& e)
{
	Vector<String> v = Split(mainconfigparam," ");
	EscValue ret;
	for(int i = 0; i < v.GetCount(); i++){
		ret.ArrayAdd(v[i]);
	}
	e = ret;
}

void Ide::MacroEcho(EscEscape& e)
{
	ShowConsole();
	for(int i = 0; i < e.GetCount(); i++){
		PutConsole(String(e[i]));
	}
}

void Ide::MacroCloseFile(EscEscape& e)
{
	int n = tabs.GetCursor();
	if(n>=0)
		tabs.Close(n,true);
}

void Ide::MacroPackageFiles(EscEscape& e)
{
	String pp;
	Package pkg;
	if(e.GetCount()==0)
		pp = GetActivePackageFile();
	else
		pp = PackageFileA(String(e[0]));
	if(!FileExists(pp))
		e.ThrowError("PackageFiles: Package not found.");
	pkg.Load(pp);
	EscValue ret;
	for(int i = 0; i < pkg.file.GetCount(); i++){
		ret.ArrayAdd(pkg.file[i]);
	}
	e = ret;
}

void Ide::MacroAllPackages(EscEscape& e)
{
	EscValue ret;
	for(int i = 0; i < package.GetCount(); i++) {
		String p = package.Get(i).name;
		if(!IsAux(p))
			ret.ArrayAdd(p);
	}
	e = ret;
}

void Ide::MacroTarget(EscEscape& e)
{
	e = target;
}
