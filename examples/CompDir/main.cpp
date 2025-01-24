#include "CompDir.h"
#pragma hdrstop

#define LAYOUTFILE <CompDir/CompDir.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE  <CompDir/CompDir.iml>
#define IMAGECLASS CompDirImg
#include <Draw/iml.h>

String NormalizePathCase(String fn)
{
#ifdef PLATFORM_WIN32 // !PATH_CASE
	return ToLower(fn);
#else
	return fn;
#endif
}

static String ExpandTabs(String line, int tabsize = 4)
{
	String out;
	int pos = 0;
	for(const char *p = line; *p; p++)
		if(*p == '\t') {
			int left = tabsize - pos % tabsize;
			out.Cat(' ', left);
			pos += left;
		}
		else {
			out.Cat(*p);
			pos++;
		}
	return out;
}

class DlgCompareDir : public WithCompareDirLayout<TopWindow> {
public:
	DlgCompareDir();

	void Run();

	void Serialize(Stream& stream);

private:
	void CmdRefresh();
	void DoTreeCursor();
	int  Refresh(String rel_path, int parent);
	void DoBrowse(Ctrl *field);
	void ToolTree(Bar& bar);
	String GetTreePath() const;

private:
	struct FileInfo : Moveable<FileInfo>
	{
		FileInfo() {}
		FileInfo(String name, int64 size, Time time) : name(name), size(size), time(time) {}

		String name;
		int64  size;
		Time   time;
	};

	bool FetchDir(String dir, VectorMap<String, FileInfo>& files, VectorMap<String, String>& dirs);

	FrameRight<Button> browse_a, browse_b;
	TreeCtrl tree;
	StaticRect editor;
	LineEdit lineedit;
	RichTextCtrl qtf;
	String pa, pb, fm;
};

DlgCompareDir::DlgCompareDir()
{
	CtrlLayout(*this, "Compare directories");
	Sizeable().Zoomable();
	refresh << [=] { CmdRefresh(); };
	splitter.Vert(tree, editor);
	editor << lineedit.SizePos() << qtf.SizePos();
	qtf.Background(White());
	qtf.SetFrame(InsetFrame());
	path_a.AddFrame(browse_a);
	browse_a.SetImage(CtrlImg::right_arrow());
	browse_a << [=] { DoBrowse(&path_a); };
	path_b.AddFrame(browse_b);
	browse_b.SetImage(CtrlImg::right_arrow());
	browse_b <<  [=] { DoBrowse(&path_b); };
	file_mask <<= "*.cpp *.h *.hpp *.c *.C *.cxx *.cc *.lay *.iml *.upp *.sch *.dph";
	tree.WhenCursor = [=] { DoTreeCursor(); };
	lineedit.SetReadOnly();
	lineedit.SetFont(Courier(14));
}

void DlgCompareDir::Run()
{
	TopWindow::Run();
}

void DlgCompareDir::Serialize(Stream& stream)
{
	int version = 1;
	stream / version;
	stream % path_a % path_b % file_mask;
	SerializePlacement(stream);
	stream % splitter;
}

void DlgCompareDir::CmdRefresh()
{
	pa = ~path_a;
	pb = ~path_b;
	fm = ~file_mask;
	tree.Clear();
	Image icon;
	switch(Refresh(Null, 0)) {
	case 0: icon = CtrlImg::Dir(); break;
	case 1: icon = CompDirImg::a_dir(); break;
	case 2: icon = CompDirImg::b_dir(); break;
	case 3: icon = CompDirImg::ab_dir(); break;
	}
	tree.SetRoot(icon, "Root");
}

bool DlgCompareDir::FetchDir(String dir, VectorMap<String, FileInfo>& files, VectorMap<String, String>& dirs)
{
	FindFile ff;
	if(!ff.Search(AppendFileName(dir, "*")))
		return false;
	do
		if(ff.IsFile() && PatternMatchMulti(fm, ff.GetName()))
			files.Add(NormalizePathCase(ff.GetName()), FileInfo(ff.GetName(), ff.GetLength(), ff.GetLastWriteTime()));
		else if(ff.IsFolder())
			dirs.Add(NormalizePathCase(ff.GetName()), ff.GetName());
	while(ff.Next());
	return true;
}

int DlgCompareDir::Refresh(String rel_path, int parent)
{
	FindFile ff;
	VectorMap<String, FileInfo> afile, bfile;
	VectorMap<String, String> adir, bdir;
	String arel = AppendFileName(pa, rel_path);
	String brel = AppendFileName(pb, rel_path);
	int done = 0;
	if(!FetchDir(arel, afile, adir))
		done |= 2;
	if(!FetchDir(brel, bfile, bdir))
		done |= 1;

	Index<String> dir_index;
	dir_index <<= adir.GetIndex();
	FindAppend(dir_index, bdir.GetKeys());
	Vector<String> dirs(dir_index.PickKeys());
	Sort(dirs, GetLanguageInfo());
	for(int i = 0; i < dirs.GetCount(); i++) {
		int fa = adir.Find(dirs[i]), fb = bdir.Find(dirs[i]);
		String dn = (fb >= 0 ? bdir[fb] : adir[fa]);
		int dirpar = tree.Add(parent, CtrlImg::Dir(), dn);
		int dirdone = Refresh(AppendFileName(rel_path, dirs[i]), dirpar);
		done |= dirdone;
		switch(dirdone) {
		case 0: tree.Remove(dirpar); break;
		case 1: tree.SetNode(dirpar, TreeCtrl::Node().SetImage(CompDirImg::a_dir()).Set(dn)); break;
		case 2: tree.SetNode(dirpar, TreeCtrl::Node().SetImage(CompDirImg::b_dir()).Set(dn)); break;
		case 3: tree.SetNode(dirpar, TreeCtrl::Node().SetImage(CompDirImg::ab_dir()).Set(dn)); break;
		}
	}
	Index<String> name_index;
	name_index <<= afile.GetIndex();
	FindAppend(name_index, bfile.GetKeys());
	Vector<String> names(name_index.PickKeys());
	Sort(names, GetLanguageInfo());
	for(int i = 0; i < names.GetCount(); i++) {
		int fa = afile.Find(names[i]), fb = bfile.Find(names[i]);
		if(fa < 0) {
			tree.Add(parent, CompDirImg::b_file(), NFormat("%s: B (%`, %0n)", bfile[fb].name, bfile[fb].time, bfile[fb].size));
			done |= 2;
		}
		else if(fb < 0) {
			tree.Add(parent, CompDirImg::a_file(), NFormat("%s: A (%`, %0n)", afile[fa].name, afile[fa].time, afile[fa].size));
			done |= 1;
		}
		else if(afile[fa].size != bfile[fb].size
		|| LoadFile(AppendFileName(arel, names[i])) != LoadFile(AppendFileName(brel, names[i]))) {
			tree.Add(parent, CompDirImg::ab_file(), NFormat("%s: A (%`, %0n), B (%`, %0n)",
				bfile[fb].name, afile[fa].time, afile[fa].size, bfile[fb].time, bfile[fb].size));
			done |= 3;
		}
	}
	return done;
}

String DlgCompareDir::GetTreePath() const
{
	int i = tree.GetCursor();
	if(i < 0)
		return String::GetVoid();
	if(i == 0)
		return Null;
	String s = tree.Get(i);
	int f = s.Find(':');
	if(f >= 0)
		s.Trim(f);
	while((i = tree.GetParent(i)) != 0)
		s = AppendFileName(String(tree.Get(i)), s);
	return s;
}

void DlgCompareDir::DoTreeCursor()
{
	String s = GetTreePath();
	if(IsNull(s))
		return;
	String fa = AppendFileName(pa, s), fb = AppendFileName(pb, s);
	String da = LoadFile(fa), db = LoadFile(fb);
	if(!IsNull(da) || !IsNull(db)) {
		if(IsNull(da) || IsNull(db)) {
			qtf.Hide();
			lineedit.Show();
			lineedit <<= Nvl(db, da);
		}
		else {
			lineedit.Hide();
			qtf.Show();
			String comptext = "[C2 ";
			Vector<String> la = GetStringLineMap(da), lb = GetStringLineMap(db);
			Array<TextSection> sections = CompareLineMaps(la, lb);
			for(int s = 0; s < sections.GetCount(); s++) {
				const TextSection& sec = sections[s];
				if(sec.same) {
					comptext << "[@(0.0.0) \1";
					if(sec.count1 <= 6)
						for(int i = 0; i < sec.count1; i++)
							comptext << ExpandTabs(la[i + sec.start1]) << '\n';
					else {
						for(int i = 0; i < 3; i++)
							comptext << ExpandTabs(la[i + sec.start1]) << '\n';
						comptext << "...\n";
						for(int i = -3; i < 0; i++)
							comptext << ExpandTabs(la[i + sec.start1 + sec.count1]) << '\n';
					}
					comptext << "\1]";
				}
				else {
					if(sec.count1) {
						comptext << "[@(0.160.0) \1";
						for(int i = 0; i < sec.count1; i++)
							comptext << ExpandTabs(la[sec.start1 + i]) << '\n';
						comptext << "\1]";
					}
					if(sec.count2) {
						comptext << "[@(0.0.255) \1";
						for(int i = 0; i < sec.count2; i++)
							comptext << ExpandTabs(lb[sec.start2 + i]) << '\n';
						comptext << "\1]";
					}
				}
			}
			qtf.SetQTF(comptext);
		}
	}
}

void DlgCompareDir::DoBrowse(Ctrl *field)
{
	FileSel fsel;
	fsel.AllFilesType();
	static String recent_dir;
	fsel <<= Nvl((String)~*field, recent_dir);
	if(fsel.ExecuteSelectDir())
		*field <<= recent_dir = ~fsel;
}

void DlgCompareDir::ToolTree(Bar& bar)
{
}

GUI_APP_MAIN
{
	DlgCompareDir cmpdlg;
	LoadFromFile(cmpdlg, ConfigFile());
	cmpdlg.Run();
	StoreToFile(cmpdlg, ConfigFile());
}
