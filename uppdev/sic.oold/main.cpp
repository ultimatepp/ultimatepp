#include "Sic.h"

SVal SIC_Print(Sic& sic, Vector<Sic::SRVal>& arg)
{
	if(arg[0]->IsString())
		VppLog() << arg[0]->GetString();
	else
	if(arg[0]->IsNumber())
		VppLog() << arg[0]->GetNumber();
	else
	if(!arg[0]->IsVoid())
		sic.ThrowError("invalid argument to 'print'");

	return Null;
}

SVal SIC_DumpLocals(Sic& sic, Vector<Sic::SRVal>& arg)
{
	LOG("--- DUMP of SIC local variables -------------------------");
	for(int i = 0; i < sic.var.GetCount(); i++) {
		const Sic::Var& v = sic.var[i];
		RLOG(sic.var.GetKey(i) << " = " << (v.ref ? v.ref->Dump() : v.var.Dump()));
	}
	LOG("---------------------------------------------------------");
	return Null;
}

void Main()
{
	String code = LoadFile("E:\\sic.sic");
	ArrayMap<String, SVal> global;

	Escape(global, "print(x)", SIC_Print);
	Escape(global, "dump_locals()", SIC_DumpLocals);

	StdLib(global);

	try {
		Scan(global, code);
		TimeStop();
		SVal ret = Execute(global, "main");
		RLOG(TimeStop());
		RLOG("RETURN VALUE: " << ret.Dump());
	}
	catch(CParser::Error& e)
	{
		RLOG(e);
	}
}
