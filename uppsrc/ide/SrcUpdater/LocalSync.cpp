#include "SrcUpdater.h"
using namespace Upp;

LocalSync::LocalSync(const VectorMap<String,int>& changed,const String& localsrc):localsrc(localsrc){
	CtrlLayoutOKCancel(*this, "Synchronize local sources");
	list.AddIndex();
	list.AddColumn("Action");
	list.AddColumn("File");
	list.AddColumn("Changes");
	list.ColumnWidths("170 450 150");
	list.NoCursor().EvenRowColor();
	list.SetLineCy(max(Draw::GetStdFontCy(), 20));
	list.WhenLeftClick = THISBACK(ShowFile);
	Populate(changed);
	globalsrc=UpdaterCfg().globalsrc;
	Sizeable().Zoomable();
	setup.Hide();
	BackPaint();
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

void LocalSync::DoShowFile(int ii,int state){
	String f = list.Get(ii, 0);
	switch(state){
		case FS_CHANGED: {
			ShowFileDlg(AppendFileName(localsrc,f),AppendFileName(globalsrc,f)).Execute();
			break;
		}
		case FS_DELETED: {
			ShowFileDlg(AppendFileName(globalsrc,f)).Execute();
			break;
		}
		case FS_ADDED:{
			ShowFileDlg(AppendFileName(localsrc,f)).Execute();
			break;
		}
	}
}

void LocalSync::ShowFile(){
	int i = list.GetClickRow();
	if(i >= 0) btns[i].WhenAction();
}

void LocalSync::Perform(){
	if(list.Get(0,0)=="") return;
	for(int i = 0; i < list.GetCount(); i++){
		String file(list.Get(i,0));
		switch(switches[i]){
			case FS_ADDED:{
				file=AppendFileName(localsrc,file);
				if(!FileDelete(file))
					Exclamation("Could not delete file "+file+".");
				break;
			}
			case FS_CHANGED:
			case FS_DELETED:{
				String orig=AppendFileName(globalsrc,file);
				file=AppendFileName(localsrc,file);
				if(!RealizePath(file))
					Exclamation("Could not create directory "+GetFileDirectory(file)+".");
				if(!FileCopy(orig,file))
					Exclamation("Could not copy file "+orig+" to "+file+".");
				break;
			}
		}
	}
}

void LocalSync::Populate(VectorMap<String,int> changed){
	if(changed.GetCount()==0){
		list.Add("", Null, AttrText("No changes in the local copy").SetFont(StdFont().Italic()));
		return;
	}
	for(int i = 0; i < changed.GetCount(); i++){
		static Color color[] = {LtBlue, LtRed, Green};
		static const char* label[] = {"Show changes", "Show file", "Show file"};
		static const char* actions[] = {"Skip\nUpdate", "Skip\nAdd", "Skip\nDelete"};
		
		list.Add(changed.GetKey(i),Value(true),AttrText("  " + changed.GetKey(i)).Ink(color[changed[i]]));
		list.SetCtrl(i, 0, switches.Add().SetLabel(actions[changed[i]]).Set(0,-1).Set(1,changed[i]).NoWantFocus());
		switches.Top()=changed[i];
		Ctrl& b = btns.Add().SetLabel(label[changed[i]]).SizePos().NoWantFocus();
		b <<= THISBACK2(DoShowFile, i, changed[i]);
		list.SetCtrl(i, 2, b);
	}
}
