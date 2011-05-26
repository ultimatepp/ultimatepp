#include "SrcUpdater.h"
using namespace Upp;

enum{USER=0,ORIG=1,NEW=2};

void GetFiles(const String& dir,int prefix,VectorMap<String,synchashes>& dest,int c, Progress& p){
	p.Title("Calculating hashes ...");
	p.SetText(dir);
	FindFile ff(AppendFileName(dir,"*"));
	for(; ff; ff.Next()){
		String path=AppendFileName(dir,ff.GetName());
		if(ff.IsFile()) {
			p.Step();
			dest.GetAdd(path.Mid(prefix)).a[c]=CRC32(LoadFile(path));
		} else if(ff.IsFolder()){
			p.SetText(path);
			GetFiles(path,prefix,dest,c,p);
		}
	}
}

void LoadHashes(const String& fn,VectorMap<String,synchashes>& dest, int c, Progress& p){
	p.Title("Loading hashes ...");
	FileIn f(fn);
	for(int i=0;i<dest.GetCount();i++){
		dest[i].a[c]=0;
	}
	while(!f.IsEof()){
		p.Step();
		dword hash=f.Get32();
		dest.GetAdd(f.GetLine()).a[c]=hash;
	}
	f.Close();
}

void SaveHashes(const String& fn,VectorMap<String,synchashes>& src, int c){
	FileOut f(fn);
	for(int i=0;i<src.GetCount();i++)
		if(src[i].a[c]!=0){
			f.Put32(src[i].a[c]);
			f.PutLine(src.GetKey(i));
		}
	f.Close();
}

LocalSync::LocalSync() : row(0){
	CtrlLayoutOKCancel(*this, "Synchronize local sources");
	list.AddColumn("State");
	list.AddColumn("Actions");
	list.AddColumn("File");
	list.AddColumn("Changes");
	list.AddIndex();
	list.ColumnWidths("225 170 325 150");
	list.NoCursor().EvenRowColor();
	list.SetLineCy(max(Draw::GetStdFontCy(), 20));
	list.WhenLeftClick = THISBACK(ShowFile);
	list.WhenBar = THISBACK(Menu);

	globalsrc=UpdaterCfg().globalsrc;
	localsrc=UpdaterCfg().localsrc;
	Progress p;
	p.AlignText(ALIGN_LEFT);
	GetFiles(localsrc,strlen(localsrc)+(localsrc.EndsWith("/")?0:1),files,USER,p);
	GetFiles(globalsrc,strlen(globalsrc)+(globalsrc.EndsWith("/")?0:1),files,NEW,p);
	String h=ConfigFile("hashes.crc");
	if(FileExists(h))
		LoadHashes(h,files,ORIG,p);
	p.Close();
	Populate(files);

	Sizeable().Zoomable();
	setup.Hide();
	BackPaint();
}

void LocalSync::Menu(Bar& bar){
	row = list.GetClickRow();
	if(row>=list.GetCount() || row<0) return;
	row = list.Get(row,4);
	if(row<0) return;
	String file = files.GetKey(row);
	
	for(int j=0; j<file.GetCount(); j++)
		if(file[j]=='/'){
			String s = file.Left(j) + "/*";
			bar.Add("Skip/Keep " + s, THISBACK2(SetAction, s, 0));
			bar.Add("Process " + s, THISBACK2(SetAction, s, 1));
			bar.Add("Ignore " + s, THISBACK1(Ignore, s));
			bar.Separator();
		}

	bar.Add("Skip/Keep " + file, THISBACK2(SetAction, file, 0));
	bar.Add("Process " + file, THISBACK2(SetAction, file, 1));
	bar.Add("Ignore " + file, THISBACK1(Ignore,file));
	bar.Add("Process all", THISBACK2(SetAction, "*", 1));
	bar.Separator();
	bar.Add("Manage ignores ...", THISBACK(Manage));
}

void LocalSync::Ignore(const String& str){
	UpdaterCfg().ignores.Add(str);
	Populate(files);
}

void LocalSync::SetAction(const String& str, int action){
	for(int i=0; i<list.GetCount(); i++){
		String file = files.GetKey(list.Get(i,4));
		if(PatternMatch(str, file)){
			Switch& s = *(Switch*)list.GetCtrl(i, 1);
			const Array<Switch::Case>& cs = s.GetCases();
			s<<=cs[action].value;
		}
	}
	list.ScrollInto(row);
}

void LocalSync::Manage(){
	TopWindow dlg;
	dlg.Title("Ignored files");
	LineEdit edit;
	edit <<= Join(UpdaterCfg().ignores,"\n");
	dlg.Add(edit.SizePos());
	dlg.Sizeable().Run(true);
	UpdaterCfg().ignores = Split(AsString(~edit),"\n");
	Populate(files);
}

class ShowFileDlg:public TopWindow{
	TextDiffCtrl diff;
	LineEdit plain;
public:
	ShowFileDlg(const String& local,const String& global){
		diff.Set(LoadFile(local),LoadFile(global));
		Add(diff.SizePos());
		Title(local+" vs. "+global);
	}
	ShowFileDlg(const String& file){
		plain.SetData(LoadFile(file));
		Add(plain.SizePos());
		Title(file);
	}
};

void LocalSync::DoShowFile(const String& l, const String& g){
	if(l.IsEmpty()&&g.IsEmpty())
		Remove();
	else if(!(l.IsEmpty()||g.IsEmpty()))
		ShowFileDlg(l,g).Execute();
	else
		ShowFileDlg(l+g).Execute();
}

void LocalSync::ShowFile(){
	int i = list.GetClickRow();
	if(i >= 0) btns[i].WhenAction();
}

void LocalSync::Perform(){
	SaveHashes(ConfigFile("hashes.crc"),files,NEW);
	if(list.Get(0,0)=="") return;
	for(int i = 0; i < list.GetCount(); i++){
		int n = list.Get(i,4);
		ASSERT(n>=0);
		String file = files.GetKey(n);
		switch(switches[i]){
			case FA_INSTALL:  //incoming update
			case FA_ADD:      //added to upstream
			case FA_REVERT:   //local change or deletion
			{
				String orig=AppendFileName(globalsrc,file);
				file=AppendFileName(localsrc,file);
				if(!RealizePath(file))
					Exclamation("Could not create directory "+GetFileDirectory(file)+".");
				if(!FileCopy(orig,file))
					Exclamation("Could not copy file "+orig+" to "+file+".");
				break;
			}
			case FA_DELETE:  //file not present in upstream
			{
				file=AppendFileName(localsrc,file);
				if(!FileDelete(file))
					Exclamation("Could not delete file "+file+".");
				break;
			}
			case FA_SKIP:
			case FA_KEEP:{} //do nothing
		}
	}
}

void LocalSync::Populate(const VectorMap<String,synchashes>& files){
	int r = 0;
	for(int i=0; i<files.GetCount(); i++){
		Vector<String> ignores(UpdaterCfg().ignores, 0);
		bool ignore = false;
		ignores.Add("uppsrc/ide/version.h");
		ignores.Add("*.bm");
		for(int j=0; j<ignores.GetCount(); j++)
			if(PatternMatch(ignores[j], files.GetKey(i))){
				ignore = true;
				break;
			}
		int pos = list.Find(i, 4);
		if(ignore){
			if(pos>=0)
				list.Remove(pos);
			continue;
		}
		if(pos>=0){
			r++;
			continue;
		}
		dword u = files[i].a[USER];
		dword o = files[i].a[ORIG];
		dword n = files[i].a[NEW];
		bool ue = FileExists(AppendFileName(localsrc, files.GetKey(i)));
		bool oe = o!=0;
		bool ne = FileExists(AppendFileName(globalsrc, files.GetKey(i)));
		FileState state;
		if(!(oe&&ue&&ne)){
			if(oe&&ne){
				if(o==n) state(FA_SKIP|FA_REVERT,"Local copy deleted", LtRed(),1);
				else state(FA_SKIP|FA_INSTALL,"Local copy deleted & incoming update", LtRed(),1);
			}else if(oe&&ue){
				if(o==u) state(FA_SKIP|FA_DELETE,"Upstream deleted",Red(),1);
				else state(FA_KEEP|FA_DELETE,"Local change & upstream deleted",Magenta(),1);
			}else if(ue&&ne){
				if(u==n) state(FA_NOOP,"",Null,0); //Local and upstream added 
				else state(FA_KEEP|FA_INSTALL,"Local and upstream added (different)",Magenta(),0);
			}else if(ue)
				state(FA_KEEP|FA_DELETE,"Local added",Green(),1);
			else if(ne)
				state(FA_SKIP|FA_ADD,"Upstream added",LtGreen(),1);
			else if(oe)
				state(FA_NOOP,"",Null,0); //Local and upstream deleted
		}else{
			if(u==n){
				// u==n==o -> unchanged
				// u==n!=o -> user updated manualy
				state(FA_NOOP,"",Null,0);
			}else{
				if(u==o)
					state(FA_SKIP|FA_INSTALL,"Incomming update",LtBlue(),0);
				else if(n==o)
					state(FA_KEEP|FA_REVERT,"Local change",Blue(),0);
				else // if(o!=n)
					state(FA_KEEP|FA_INSTALL,"Local change & incoming update",LtBlue(),0);
			}
		}
		if(state.n!=FA_NOOP){
			Switch& s = switches.Add();
			Button& b = btns.Add();
			int sel=0xffff;
			#define _ACTION(FA,LBL) if(state.n&FA){ s.Add(FA,LBL); sel=min(sel,int(FA)); }
			_ACTION(FA_SKIP,"Skip");
			_ACTION(FA_KEEP,"Keep");
			_ACTION(FA_REVERT,"Revert");
			_ACTION(FA_INSTALL,"Install new");
			_ACTION(FA_ADD,"Add");
			_ACTION(FA_DELETE,"Delete");
			#undef _ACTION
			b.SetLabel("Examine");
			b <<= THISBACK2(DoShowFile,
							ue?AppendFileName(localsrc,files.GetKey(i)):"",
			                ne?AppendFileName(globalsrc,files.GetKey(i)):"");
			list.Insert(r);
			list.Set(r,0,state.desc);
			list.Set(r,1,sel);
			list.SetCtrl(r, 1, s.NoWantFocus());
			list.Set(r,2,AttrText(files.GetKey(i)).Ink(state.c).SetFont(state.f));
			list.SetCtrl(r, 3, b.NoWantFocus());
			list.Set(r,4,i);
			r++;
		}
	}
	if(r==0){
		list.Add("", Null, AttrText("No changes in the installed nests").SetFont(StdFont().Italic()),Null,Null);
	}
	list.ScrollInto(row);
}
