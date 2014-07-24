#include <Core/Core.h>
#include <CppBase/CppBase.h>

using namespace Upp;

StringStream ss;

void Error(int line, const String& error)
{
	ss << "ERROR at " << line << ": " << error << "\n";
}


CONSOLE_APP_MAIN
{
	FindFile ff(GetDataFile("*.tc"));
	while(ff) {
		FileIn in(ff.GetPath());
		ss.Create();
		CppBase base;
		Parse(in, Vector<String>(), base, "test.cpp", callback(Error));
		Qualify(base);
		for(int i = 0; i < base.GetCount(); i++) {
			ss << "======== " << Nvl(base.GetKey(i), "GLOBALS") << "\n";
			const Array<CppItem>& am = base[i];
			for(int j = 0; j < am.GetCount(); j++) {
				const CppItem& m = am[j];
				m.Dump(ss);
			}
		}
		String result = ss.GetResult();
		LOG("****** FILE: " << ff.GetName());
		VppLog() << result;
		LOG("******\n\n");
		String h = LoadFile(ForceExt(ff.GetPath(), ".r"));
		result.Replace("\r", "");
		h.Replace("\r", "");
		if(h.GetCount())
			ASSERT(h == result);
		ff.Next();
	}
}
