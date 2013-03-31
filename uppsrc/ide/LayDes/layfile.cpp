#include "LayDes.h"

ArrayMap<String, LayDesEditPos>& LayFileDataVar()
{
	static ArrayMap<String, LayDesEditPos> x;
	return x;
}

void SerializeLayEditPos(Stream& s)
{
	ArrayMap<String, LayDesEditPos>& filedata = LayFileDataVar();
	int dummy;
	if(s.IsStoring()) {
		for(int i = 0; i < filedata.GetCount(); i++) {
			String fn = filedata.GetKey(i);
			if(!fn.IsEmpty()) {
				FindFile ff(fn);
				LayDesEditPos& ep = filedata[i];
				if(ff && ff.GetLastWriteTime() == ep.filetime) {
					s % fn;
					s % ep.filetime;
					s % ep.layouti;
					s % dummy;
				}
			}
		}
		String h;
		s % h;
	}
	else {
		String fn;
		filedata.Clear();
		for(;;) {
			s % fn;
			if(fn.IsEmpty()) break;
			LayDesEditPos& ep = filedata.GetAdd(fn);
			ep.undo.Clear();
			ep.redo.Clear();
			ep.cursor.Clear();
			s % ep.filetime;
			s % ep.layouti;
			s % dummy;
		}
	}

}

void LayDes::SaveEditPos()
{
	LayDesEditPos& p = LayFileDataVar().GetAdd(filename);
	p.filetime = filetime;
	p.undo.SetCount(layout.GetCount());
	p.redo.SetCount(layout.GetCount());
	for(int i = 0; i < layout.GetCount(); i++) {
		p.undo[i] = layout[i].undo;
		p.redo[i] = layout[i].redo;
	}
	p.layouti = layoutlist.GetCursor();
	p.cursor = cursor;
}

void LayDes::RestoreEditPos()
{
	if(IsNull(filetime)) return;
	int q = LayFileDataVar().Find(filename);
	if(q < 0)
		return;
	LayDesEditPos& p = LayFileDataVar()[q];
	if(p.filetime != filetime) return;
	if(layout.GetCount() == p.undo.GetCount()) {
		for(int i = 0; i < layout.GetCount(); i++) {
			layout[i].undo = p.undo[i];
			layout[i].redo = p.redo[i];
		}
	}
	if(p.layouti >= 0 && p.layouti < layoutlist.GetCount()) {
		layoutlist.SetCursor(p.layouti);
		LayoutCursor();
	}
	cursor = p.cursor;
	SyncItems();
}

void LayDes::FindLayout(const String& name, const String& item_name)
{
	if(layoutlist.FindSetCursor(name) && !IsNull(item_name)) {
		int q = item.Find(item_name, 1);
		if(q >= 0)
			SelectOne(q, 0);
	}
}

bool LayDes::Load(const char *file, byte _charset)
{
	charset = _charset;
	layout.Clear();
	filename = file;
	FileIn in(file);
	if(in) {
		layfile = LoadStream(in);
		newfile = false;
		filetime = in.GetTime();
		fileerror.Clear();
		try {
			CParser p(layfile);
			if(p.Char('#') && p.Id("ifdef")) {
				if(!p.Id("LAYOUTFILE"))
					p.Id("LAYOUT");
			}
			while(!p.IsEof()) {
				LayoutData& ld = layout.Add();
				ld.SetCharset(charset);
				ld.Read(p);
				if(p.Char('#'))
					p.Id("endif");
			}
		}
		catch(CParser::Error& e) {
			PutConsole(file + e);
			return false;
		}
	}
	else {
		newfile = true;
		filetime = Null;
	}
	SyncLayoutList();
	if(layout.GetCount()) {
		layoutlist.SetCursor(0);
		LayoutCursor();
	}
	RestoreEditPos();
	return true;
}

void LayDes::Save()
{
	StoreToGlobal(*this, "laydes-ctrl");

	if(!IsNull(fileerror))
		return;
	String r;
	for(int i = 0; i < layout.GetCount(); i++) {
		layout[i].SetCharset(charset);
		r << layout[i].Save() << "\r\n";
	}
	layfile = r;
	if(!SaveChangedFileFinish(filename, r))
		return;
	FindFile ff(filename);
	if(ff) {
		newfile = false;
		filetime = ff.GetLastWriteTime();
	}
}
