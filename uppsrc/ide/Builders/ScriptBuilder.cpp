#include "Builders.h"

#include <Esc/Esc.h>

EscValue ScriptBuilder::ExecuteIf(const char *fn, Vector<EscValue>& args)
{
	CheckParse();
	EscValue out;
	int f = globals.Find(fn);
	if(f < 0)
		return out;
	try
	{
		out = ::Execute(globals, NULL, globals[f], args, 50000);
	}
	catch(Exc e)
	{
		script_error = true;
		PutConsole(e);
	}
	return out;
}

EscValue ScriptBuilder::ExecuteIf(const char *fn)
{
	Vector<EscValue> args;
	return ExecuteIf(fn, args);
}

EscValue ScriptBuilder::ExecuteIf(const char *fn, EscValue arg)
{
	Vector<EscValue> args;
	args.Add(arg);
	return ExecuteIf(fn, args);
}

EscValue ScriptBuilder::ExecuteIf(const char *fn, EscValue arg1, EscValue arg2)
{
	Vector<EscValue> args;
	args.Add(arg1);
	args.Add(arg2);
	return ExecuteIf(fn, args);
}

EscValue ScriptBuilder::ExecuteIf(const char *fn, EscValue arg1, EscValue arg2, EscValue arg3)
{
	Vector<EscValue> args;
	args.Add(arg1);
	args.Add(arg2);
	args.Add(arg3);
	return ExecuteIf(fn, args);
}

void ScriptBuilder::ESC_Execute(EscEscape& e)
{
	e = Execute(String(e[0])) ? 0 : 1;
}

void ScriptBuilder::ESC_PutConsole(EscEscape& e)
{
	PutConsole(String(e[0]));
}

void ScriptBuilder::ESC_PutVerbose(EscEscape& e)
{
	PutVerbose(String(e[0]));
}

void ScriptBuilder::CheckParse()
{
	if(is_parsed)
		return;
	script_error = false;
	is_parsed = true;
	StdLib(globals);
	Escape(globals, "Execute(cmdline)", THISBACK(ESC_Execute));
	Escape(globals, "PutConsole(text)", THISBACK(ESC_PutConsole));
	Escape(globals, "PutVerbose(text)", THISBACK(ESC_PutVerbose));
	EscValue inclist;
	inclist.SetEmptyArray();
	for(int i = 0; i < include.GetCount(); i++)
		inclist.ArrayAdd(GetPathQ(include[i]));
	globals.GetAdd("INCLUDE") = inclist;
	EscValue liblist;
	liblist.SetEmptyArray();
	for(int i = 0; i < libpath.GetCount(); i++)
		liblist.ArrayAdd(GetPathQ(libpath[i]));
	globals.GetAdd("LIBPATH") = liblist;

	try
	{
		String sdata = LoadFile(script);
		if(IsNull(sdata))
			throw Exc(Format("%s: not found or empty", script));
		CParser parser(sdata, script, 1);
		while(!parser.IsEof()) {
			String id = parser.ReadId();
			globals.GetAdd(id) = ReadLambda(parser);
		}
	}
	catch(Exc e)
	{
		script_error = true;
		PutConsole(e);
	}
}

bool ScriptBuilder::BuildPackage(const String& package, Vector<String>& linkfile, Vector<String>&, String& linkoptions,
	const Vector<String>& all_uses, const Vector<String>& all_libraries, int)
{
	int i;
	Package pkg;
	pkg.Load(PackageFile(package));
	String packagedir = PackageDirectory(package);
	ChDir(packagedir);
	PutVerbose("cd " + packagedir);
	Vector<String> obj;
	script_error = false;

	String gfl = Gather(pkg.option, config.GetKeys());

	Vector<String> sfile;
	Vector<String> soptions;
	bool           error = false;

	for(i = 0; i < pkg.GetCount(); i++) {
		if(!IdeIsBuilding())
			return false;
		if(!pkg[i].separator) {
			String gop = Gather(pkg[i].option, config.GetKeys());
			Vector<String> srcfile = CustomStep(pkg[i], package, error);
			if(srcfile.GetCount() == 0)
				error = true;
			for(int j = 0; j < srcfile.GetCount(); j++) {
				String fn = srcfile[j];
				String ext = ToLower(GetFileExt(fn));
				if(ext == ".c" || ext == ".cpp" || ext == ".cc" || ext == ".cxx" ||
				   (ext == ".rc" && HasFlag("WIN32"))) {
					sfile.Add(fn);
					soptions.Add(gfl + " " + gop);
				}
				else
				if(ext == ".o")
					obj.Add(fn);
				else
				if(ext == ".a" || ext == ".so")
					linkfile.Add(fn);
			}
		}
	}

/*
	if(HasFlag("BLITZ")) {
		Blitz b = BlitzStep(sfile, soptions, obj, ".o");
		if(b.build) {
			PutConsole("BLITZ:" + b.info);
			int time = msecs();
			if(Execute(cc + " " + GetPathQ(b.path) + " -o " + GetPathQ(b.object)) == 0)
				PutCompileTime(time, b.count);
			else
				error = true;
		}
	}
*/
	int time = msecs();
	int ccount = 0;
	for(i = 0; i < sfile.GetCount() && !script_error; i++) {
		if(!IdeIsBuilding())
			return false;
		String fn = sfile[i];
		String ext = ToLower(GetFileExt(fn));
//		bool rc = ext == ".rc";
		String objfile = ExecuteIf("objectfile", fn);
		if(script_error)
			return false;
		if(IsNull(objfile))
			objfile = CatAnyPath(outdir, GetFileTitle(fn) + ".o");
		if(HdependFileTime(fn) > GetFileTime(objfile)) {
			PutConsole(GetFileName(fn));
			int time = msecs();
			if(!ExecuteIf("compile", GetPathQ(fn), GetPathQ(objfile), soptions[i]).GetNumber()) {
				DeleteFile(objfile);
				error = true;
			}
			PutVerbose("compiled in " + GetPrintTime(time));
			ccount++;
		}
		obj.Add(objfile);
	}
	if(ccount)
		PutCompileTime(time, ccount);

	if(error || script_error)
		return false;

	linkoptions << Gather(pkg.link, config.GetKeys());

	Vector<String> libs = Split(Gather(pkg.library, config.GetKeys()), ' ');
	linkfile.Append(libs);

	time = msecs();
	if(!HasFlag("MAIN")) {
		if(HasFlag("NOLIB")) {
			linkfile.Append(obj);
			return true;
		}
		String product = ExecuteIf("libraryfile", package);
		if(IsNull(product))
			product = CatAnyPath(outdir, GetAnyFileName(package) + ".a");
		Time producttime = GetFileTime(product);
		if(obj.GetCount())
			linkfile.Add("*" + product); //!! ugly
		for(int i = 0; i < obj.GetCount(); i++)
			if(GetFileTime(obj[i]) > producttime) {
				PutConsole("Creating library...");
				DeleteFile(product);
				EscValue objlist;
				objlist.SetEmptyArray();
				for(int i = 0; i < obj.GetCount(); i++)
					objlist.ArrayAdd(GetPathQ(obj[i]));
				if(!ExecuteIf("library", objlist, product).GetNumber()) {
					DeleteFile(product);
					error = true;
					return false;
				}
				PutConsole(String().Cat() << product << " (" << GetFileInfo(product).length
				           << " B) created in " << GetPrintTime(time));
				break;
			}
		return true;
	}

	obj.Append(linkfile);
	linkfile = pick(obj);
	return true;
}

bool ScriptBuilder::Link(const Vector<String>& linkfile, const String& linkoptions, bool)
{
	PutLinking();
	int time = msecs();
	for(int i = 0; i < linkfile.GetCount(); i++)
		if(GetFileTime(linkfile[i]) >= targettime) {
			EscValue objlist;
			objlist.SetEmptyArray();
			EscValue liblist;
			liblist.SetEmptyArray();
			for(i = 0; i < linkfile.GetCount(); i++)
				if(*linkfile[i] == '*')
					liblist.ArrayAdd(GetPathQ(linkfile[i].Mid(1)));
				else
					objlist.ArrayAdd(GetPathQ(linkfile[i]));
			Vector<EscValue> linkargs;
			linkargs.Add(objlist);
			linkargs.Add(liblist);
			linkargs.Add(GetPathQ(target));
			linkargs.Add(linkoptions);
			PutConsole("Linking...");
			bool error = false;
			CustomStep(".pre-link", Null, error);
			if(!error && !ExecuteIf("link", linkargs).GetNumber()) {
				DeleteFile(target);
				return false;
			}
			CustomStep(".post-link", Null, error);
			PutConsole(String().Cat() << target << " (" << GetFileInfo(target).length
				<< " B) linked in " << GetPrintTime(time));
			return !error;
		}
	PutConsole(String().Cat() << target << " (" << GetFileInfo(target).length
	           << " B) is up to date.");
	return true;
}

bool ScriptBuilder::Preprocess(const String& package, const String& file, const String& target, bool)
{
	return ExecuteIf("preprocess", file, target).GetNumber();
}

Builder *CreateScriptBuilder()
{
	return new ScriptBuilder;
}

INITIALIZER(ScriptBuilder)
{
	RegisterBuilder("SCRIPT", CreateScriptBuilder);
}

/*
EscValue LayoutItem::CreateEsc() const
{
	EscValue ctrl;
	String tp = type;
	String tm;
	if(ParseTemplate(tp, tm)) {
		CreateMethods(ctrl, tp, true);
		ctrl("CtrlPaint") = ctrl("Paint");
		CreateMethods(ctrl, tm, false);
	}
	else
		CreateMethods(ctrl, tp, false);
	for(int q = 0; q < property.GetCount(); q++) {
		EscValue& w = ctrl(property[q].name);
		const Value& v = ~property[q];
		if(IsType<Font>(v))
			w = EscFont(v);
		if(IsString(v))
			w = (WString)v;
		if(IsNumber(v))
			w = (double)v;
		if(IsType<Color>(v))
			w = EscColor(v);
	}
	ctrl("type") = (WString)type;
	ctrl("GetSize") = ReadLambda(Format("() { return Size(%d, %d); }",
	                                    csize.cx, csize.cy));
	ctrl("GetRect") = ReadLambda(Format("() { return Rect(0, 0, %d, %d); }",
	                                    csize.cx, csize.cy));
	return ctrl;
}

EscValue LayoutItem::ExecuteMethod(const char *method, Vector<EscValue>& arg) const
{
	try {
		EscValue self = CreateEsc();
		return ::Execute(UscGlobal(), &self, method, arg, 50000);
	}
	catch(CParser::Error& e) {
		PutConsole(e + "\n");
	}
	return EscValue();
}

EscValue LayoutItem::ExecuteMethod(const char *method) const
{
	Vector<EscValue> arg;
	return ExecuteMethod(method, arg);
}

Size LayoutItem::GetMinSize()
{
	return SizeEsc(ExecuteMethod("GetMinSize"));
}
*/
