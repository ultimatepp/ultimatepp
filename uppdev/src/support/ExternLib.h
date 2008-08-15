#ifndef _src_support_ExternLib_h_
#define _src_support_ExternLib_h_

class PeFile {
public:
	PeFile(const char *data = 0) { Open(data); }

	bool                        Open(const char *data);

	int                         GetExportCount() const;
	const char                 *GetExport(int index) const;
	const char                 *FindExportRaw(const char *name, bool case_sensitive = true) const;
	void												LoadFuncsName(Vector<String>& names);

	int                         GetSectionIndex(const char *name) const;

private:
	const char                 *data;
	const IMAGE_NT_HEADERS     *headers;
	const IMAGE_SECTION_HEADER *sections;
	const IMAGE_EXPORT_DIRECTORY *exports;
};

class ExternLib {
	public:
	HMODULE hmod;
	Vector<String> funcsName;

	HMODULE Load(const char *fn);
	FARPROC WINAPI GetProc(const String& funcName);

	void Close()						 							{ FreeLibrary(hmod); }
	bool IsOpen()													{ return hmod != 0; }

	Vector<String> GetFuncsList()					{ return funcsName; }

	ExternLib();
	ExternLib(const char* fn)							{ Load(fn); }
	~ExternLib()													{ Close(); }

};

typedef Value (*ExternFunction)(Vector<Value> *args);

#endif
