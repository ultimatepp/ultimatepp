#include "Core.h"

ArrayMap<String, EscValue>& UscGlobal()
{
	static ArrayMap<String, EscValue> global;
	return global;
}

IdeMacro::IdeMacro()
{
	hotkey = 0;
}

Array<IdeMacro>& UscMacros()
{
	static Array<IdeMacro> h;
	return h;
}

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

static void (*sCleanModules)();

void UscSetCleanModules(void (*CleanModules)())
{
	sCleanModules = CleanModules;
}

void CleanUsc()
{
	UscGlobal().Clear();
	UscMacros().Clear();
	if(sCleanModules)
		sCleanModules();
	StdLib(UscGlobal());
	Escape(UscGlobal(), "cout(x)", ESC_cout);
	Escape(UscGlobal(), "dump(x)", ESC_dump);
}

static bool (*sIdeModuleUsc)(CParser& p, String&);
static void (*sReadMacro)(CParser& p);

void SetIdeModuleUsc(bool (*IdeModuleUsc)(CParser& p,String&))
{
	sIdeModuleUsc = IdeModuleUsc;
}

void UscSetReadMacro(void (*ReadMacro)(CParser& p))
{
	sReadMacro = ReadMacro;
}

void ParseUscFile(const char *filename)
{
	String d = LoadFile(filename);
	try {
		String current_namespace;
		CParser p(d, filename);
		while(!p.IsEof()) {
			if(p.Id("fn")) {
				EscValue& v = UscGlobal().GetPut(p.ReadId());
				v = ReadLambda(p);
			}
			else
			if(p.Id("macro") && sReadMacro)
				sReadMacro(p);
			else
			if(!sIdeModuleUsc || !sIdeModuleUsc(p, current_namespace)) {
				EscValue& v = UscGlobal().GetPut(p.ReadId());
				v = ReadLambda(p);
			}
		}
	}
	catch(CParser::Error) {}
}
