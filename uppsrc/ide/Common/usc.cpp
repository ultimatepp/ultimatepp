#include "Common.h"

ArrayMap<String, EscValue>& UscGlobal()
{
	static ArrayMap<String, EscValue> global;
	return global;
}

IdeMacro::IdeMacro()
{
	hotkey = 0;
}

GLOBAL_VAR(Array<IdeMacro>, UscMacros)

void ESC_cout(EscEscape& e)
{
	if(e[0].IsArray())
		PutConsole((String) e[0]);
	else
	if(e[0].IsNumber()) {
		String str;
		str << e[0].GetNumber();
		PutConsole(str);
	}
	else
	if(!e[0].IsVoid())
		e.ThrowError("invalid argument to 'cout'" + e.DumpType(0));
}

void ESC_dump(EscEscape& e)
{
	PutConsole(e[0].ToString());
}

void CleanUsc()
{
	UscGlobal().Clear();
	UscMacros().Clear();
	for(int i = 0; i < GetIdeModuleCount(); i++)
		GetIdeModule(i).CleanUsc();
	StdLib(UscGlobal());
	Escape(UscGlobal(), "cout(x)", ESC_cout);
	Escape(UscGlobal(), "dump(x)", ESC_dump);
}

bool IdeModuleUsc(CParser& p)
{
	for(int i = 0; i < GetIdeModuleCount(); i++)
		if(GetIdeModule(i).ParseUsc(p))
			return true;
	return false;
}

static void ReadMacro(CParser& p)
{
	IdeMacro macro;
	if(p.IsString()) {
		macro.menu = p.ReadString();
		if(p.Char(':'))
			macro.submenu = p.ReadString();
	}
	if(!p.IsChar('{'))
		macro.hotkey = ParseKeyDesc(p);
	EscLambda& l = macro.code.CreateLambda();
	const char *t = p.GetPtr();
	l.filename = p.GetFileName();
	l.line = p.GetLine();
	if(!p.Char('{'))
		p.ThrowError("missing '{'");
	SkipBlock(p);
	l.code = String(t, p.GetPtr());
	Array<IdeMacro>& mlist = UscMacros();
	if(macro.hotkey) {
		int f = FindFieldIndex(mlist, &IdeMacro::hotkey, macro.hotkey);
		if(f >= 0) {
			PutConsole(NFormat("%s(%d): duplicate macro hotkey %s\n", l.filename, l.line, GetKeyDesc(macro.hotkey)));
			const EscLambda& lambda = UscMacros()[f].code.GetLambda();
			PutConsole(NFormat("%s(%d): previously defined here\n", lambda.filename, lambda.line));
		}
	}
	if(!IsNull(macro.menu)) {
		for(int i = 0; i < mlist.GetCount(); i++)
			if(mlist[i].menu == macro.menu && mlist[i].submenu == macro.submenu) {
				PutConsole(NFormat("%s(%d): duplicate macro menu item (%s:%s)\n",
					l.filename, l.line, macro.menu, macro.submenu));
				const EscLambda& lambda = UscMacros()[i].code.GetLambda();
				PutConsole(NFormat("%s(%d): previously defined here\n", lambda.filename, lambda.line));
				break;
			}
	}
	mlist.Add(macro);
}

void ParseUscFile(const char *filename) throw(CParser::Error)
{
	String d = LoadFile(filename);
	CParser p(d, filename);
	while(!p.IsEof()) {
		if(p.Id("fn")) {
			EscValue& v = UscGlobal().GetPut(p.ReadId());
			v = ReadLambda(p);
		}
		else
		if(p.Id("macro"))
			ReadMacro(p);
		else
		if(!IdeModuleUsc(p)) {
			EscValue& v = UscGlobal().GetPut(p.ReadId());
			v = ReadLambda(p);
		}
	}
}
