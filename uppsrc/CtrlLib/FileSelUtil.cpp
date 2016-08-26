#include "CtrlLib.h"

namespace Upp {

FileSel& GFileSel()
{
	static FileSel fs;
	ONCELOCK {
		fs.ActiveDir(GetHomeDirectory());
	}
	return fs;
}

INITBLOCK {
	RegisterGlobalConfig("GlobalFileSelector");
}

String SelectFileOpen(const char *types)
{
	FileSel& fs = GFileSel();
	LoadFromGlobal(fs, "GlobalFileSelector");
	fs.ClearTypes();
	fs.Types(types);
	bool b = fs.ExecuteOpen();
	StoreToGlobal(fs, "GlobalFileSelector");
	return b ? ~fs : String::GetVoid();
}

String SelectFileSaveAs(const char *types)
{
	FileSel& fs = GFileSel();
	LoadFromGlobal(fs, "GlobalFileSelector");
	fs.ClearTypes();
	fs.Types(types);
	bool b = fs.ExecuteSaveAs();
	StoreToGlobal(fs, "GlobalFileSelector");
	return b ? ~fs : String::GetVoid();
}

String SelectDirectory()
{
	FileSel fs;
	fs.ActiveDir(GetHomeDirectory());
	LoadFromGlobal(fs, "GlobalDirSelector");
	bool b = fs.ExecuteSelectDir();
	StoreToGlobal(fs, "GlobalDirSelector");
	return b ? ~fs : String::GetVoid();
}

SelectFileIn::SelectFileIn(const char *types)
{
	for(;;) {
		String p = SelectFileOpen(types);
		if(p.GetCount() == 0 || Open(p))
			return;
		Exclamation(t_("Unable to open [* \1") + p + t_("\1] for reading!"));
	}
}

SelectFileOut::SelectFileOut(const char *types)
{
	for(;;) {
		String p = SelectFileSaveAs(types);
		if(p.GetCount() == 0 || Open(p))
			return;
		Exclamation(t_("Unable to open [* \1") + p + t_("\1] for writing!"));
	}
}

String SelectLoadFile(const char *types)
{
	String p = SelectFileOpen(types);
	return p.GetCount() ? LoadFile(p) : String::GetVoid();
}

bool SelectSaveFile(const char *types, const String& data)
{
	String p = SelectFileSaveAs(types);
	if(p.GetCount() == 0)
		return false;
	if(!SaveFile(p, data)) {
		Exclamation(t_("Error saving the file!"));
		return false;
	}
	return true;
}

}
