#include <CodeEditor/CodeEditor.h>

using namespace Upp;

struct CodeIndex : public TopWindow {
	virtual bool Key(dword key, int count);

	FileList   files;
	CodeEditor view;
	ToolBar    toolbar;
	Splitter   split;
	EditString search;

	Index<String>                   exts;
	Index<String>                   file;
	VectorMap<String, Vector<int> > index;

	void ScanFile(const String& f);
	void Scan(const String& dir, Progress& pi, int64& amount);
	void Scan();
	void Serialize(Stream& s);
	void ToolBar(Bar& bar);
	void File();

public:
	typedef CodeIndex CLASSNAME;

	CodeIndex();
};

void CodeIndex::ScanFile(const String& fn) {
	DUMP(fn);
	int fi = file.GetCount();
	file.Add(fn);
	String f = LoadFile(fn);
	const char *q = f;
	for(;;) {
		int c = *q;
		if(IsAlpha(c) || c == '_') {
			const char *b = q++;
			while(IsAlNum(*q) || *q == '_') q++;
			Vector<int>& mf = index.GetAdd(String(b, q));
			if(mf.GetCount() == 0 || mf.Top() != fi)
				mf.Add(fi);
		}
		else {
			if(!c) break;
			q++;
		}
	}
}

int NoP(int c)
{
	return c == '%' ? 0 : c;
}

void CodeIndex::Scan(const String& dir, Progress& pi, int64& amount)
{
	FindFile ff(AppendFileName(dir, "*.*"));
	while(ff) {
		String p = AppendFileName(dir, ff.GetName());
		if(ff.IsFile()) {
			String ext = ToUpper(GetFileExt(ff.GetName()));
			if(pi.StepCanceled())
				throw Exc();
			if(ext.GetCount() && exts.Find(ext.Mid(1)) >= 0)
				ScanFile(p);
			amount += ff.GetLength();
		}
		if(ff.IsFolder()) {
			pi.SetText(AsString(amount >> 20) + " MB: " + Filter(p, NoP));
			Scan(p, pi, amount);
		}
		ff.Next();
	}
}

void CodeIndex::Scan()
{
	try {
		Progress pi;
		int64 amount = 0;
		pi.AlignText(ALIGN_LEFT);
		Scan("f:/exsrc", pi, amount);
	}
	catch(Exc) {}
}

void CodeIndex::ToolBar(Bar& bar)
{
	bar.Add(search, INT_MAX);
}

bool CodeIndex::Key(dword key, int count)
{
	if(key == K_ENTER) {
		files.Clear();
		int q = index.Find(~search);
		if(q >= 0) {
			Vector<int>& f = index[q];
			for(int i = 0; i < f.GetCount(); i++)
				files.Add(file[f[i]]);
		}
		return true;
	}
	return false;
}

void CodeIndex::File()
{
	view.Clear();
	if(files.IsCursor())
		view <<= LoadFile(files.GetCurrentName());
}

CodeIndex::CodeIndex()
{
	Add(split.Horz(files, view));
	split.SetPos(2000);
	files.AddFrame(toolbar);
	view.SetReadOnly();
	toolbar.Set(THISBACK(ToolBar));
	exts = Split("C;CPP;H;HPP", ';');
	files.WhenSel = THISBACK(File);
}

void CodeIndex::Serialize(Stream& s)
{
	s.Magic(0x76129812);
	s % file;
	s % index;
}

GUI_APP_MAIN
{
	CodeIndex app;
	LoadFromFile(app);
	if(app.index.GetCount() == 0)
		app.Scan();
	app.Run();
	StoreToFile(app);
//	Progress pi;
//	b.Scan("f:/exsrc", &pi);
}
