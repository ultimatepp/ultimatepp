#include "Designers.h"

void TreeDesPos::Save(Time afiletime, TreeCtrl& tree)
{
	filetime = afiletime;
	scroll = tree.GetScroll();
	cursor = tree.GetCursor();
	openid = tree.GetOpenIds();
}

void TreeDesPos::Restore(Time afiletime, TreeCtrl& tree)
{
	if(afiletime == filetime) {
		tree.OpenIds(openid);
		if(tree.IsValid(cursor))
			tree.SetCursor(cursor);
		tree.ScrollTo(scroll);
	}
	tree.SetFocus();
}

void Serialize(Stream& s, ArrayMap<String, TreeDesPos>& pos)
{
	int version = 0;
	s / version;
	s % pos;
}

TreeViewDes::TreeViewDes()
{
	tree.NoRoot();

	error.SetFont(Arial(20)).SetInk(Red);
	errorbg.Height(Zy(25)).Add(error.SizePos());

	Add(tree.SizePos());

	tree.WhenLeftDouble = [=] { CopyPath(); };
}

ArrayMap<String, TreeDesPos> TreeViewDes::pos;

bool TreeViewDes::Load(const String& fn)
{
	RemoveFrame(errorbg);
	tree.Clear();

	filename = fn;

	FileIn in(filename);
	if(!in)
		return false;

	filetime = in.GetTime();
	
	const int limit = 20000000;
	
	String txt = in.Get(limit);
	
	String parsingError = Load0(txt);
	if(in.GetSize() >= limit)
		MergeWith(parsingError, ", ", "file was longer than 20MB and was truncated");
	if(parsingError.GetCount()) {
		error = String() << "XML parsing error: " << parsingError << ".";
		AddFrame(errorbg);
	}

	return tree.GetChildCount(0);
}

void TreeViewDes::EditMenu(Bar& menu)
{
	menu.Add(tree.IsCursor(), "Copy current path to clipboard", [=] { CopyPath(); });
}

void TreeViewDes::RestoreEditPos()
{
	pos.GetAdd(filename + "\n" + GetId()).Restore(filetime, tree);
}

void TreeViewDes::SaveEditPos()
{
	pos.GetAdd(filename + "\n" + GetId()).Save(filetime, tree);
}

INITIALIZER(TreeViewDes)
{
	RegisterGlobalSerialize("TreeViewDes", [](Stream& s) {
		int version = 0;
		s / version;
		s % TreeViewDes::pos;
	});
}
