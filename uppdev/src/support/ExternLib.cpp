#include "support.h"

bool PeFile::Open(const char *_data) {
	data = _data;
	if(!data || data[0] != 'M' || data[1] != 'Z')
		return false;
	int pe = PeekIL(data + 0x3C);
	if(IsBadReadPtr(data + pe, sizeof(IMAGE_NT_HEADERS)))
		return false;
	if(memcmp(data + pe, "PE\0\0", 4))
		return false;
	headers = (const IMAGE_NT_HEADERS *)(data + pe);
	sections = (const IMAGE_SECTION_HEADER *)(headers + 1);
	exports = (const IMAGE_EXPORT_DIRECTORY *)(data
		+ headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	return true;
}

int PeFile::GetSectionIndex(const char *name) const {
	for(int i = 0, n = headers->FileHeader.NumberOfSections; i < n; i++)
		if(!strcmp((const char *)sections[i].Name, name))
			return i;
	return -1;
}

int PeFile::GetExportCount() const {
	return exports ? exports->NumberOfNames : 0;
}

const char *PeFile::GetExport(int index) const {
	if(!exports || index < 0 || index >= (int) exports->NumberOfNames)
		return 0;
	return data + ((const dword *)(data + (dword)exports->AddressOfNames))[index];
}

static bool EqualMem(const char *a, const char *b, int len, bool case_sensitive) {
	if(case_sensitive)
		return !memcmp(a, b, len);
	else
		return !memicmp(a, b, len);
}

const char *PeFile::FindExportRaw(const char *name, bool case_sensitive) const {
	if(!exports || !name || !*name)
		return 0;
	int len = strlen(name);
	const dword *pnames = (const dword *)(data + (dword)exports->AddressOfNames);
	for(int i = 0; i < (int) exports->NumberOfNames; i++) {
		const char *exp = data + pnames[i];
		int elen = strlen(exp);
		if(elen < len)
			continue;
		if(elen == len && EqualMem(exp, name, len, case_sensitive))
			return exp;
		if(EqualMem(exp, name, len, case_sensitive) && exp[len] == '@')
			return exp;
		if(exp[0] == '_' && EqualMem(exp + 1, name, len, case_sensitive)
			&& (exp[len + 1] == '@' || exp[len + 1] == 0))
			return exp;
	}
	return 0;
}

void PeFile::LoadFuncsName(Vector<String>& names) {

	if(!exports) return;
	const dword *pnames = (const dword *)(data + (dword)exports->AddressOfNames);
	for(int i = 0; i < (int) exports->NumberOfNames; i++) {
		const char *exp = data + pnames[i];
		names.Add(String(exp));
	}
	return;
}

HMODULE ExternLib::Load(const char *fn) {
	hmod = LoadLibrary(fn);
	if(!hmod) return 0;

	PeFile pe;
	if(!pe.Open((const char *)hmod)) {
		FreeLibrary(hmod);
		return 0;
	}

	funcsName.Clear();
	pe.LoadFuncsName(funcsName);

	if(funcsName.GetCount()<1) {
		FreeLibrary(hmod);
		return 0;
	}
	return hmod;
}

FARPROC WINAPI ExternLib::GetProc(const String& funcName) {
	for(int i=0; i<funcsName.GetCount(); i++) {
		String fn = funcsName[i];
		for(int j=0; j<=(fn.GetLength()-funcName.GetLength()); j++) {
			if(memcmp((char*)~fn.Mid(j, funcName.GetLength()), (char*)~funcName, funcName.GetLength())	== 0) {
				return GetProcAddress(hmod, (char*)~fn);
			}
		}
	}
	return 0;
}
