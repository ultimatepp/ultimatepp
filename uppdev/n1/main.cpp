#include <CtrlLib/CtrlLib.h>

#include <global.h>
#include <osfile.h>
#include <nsfdb.h>
#include <nsfdata.h>
#include <names.h>
#include <osmisc.h>

struct MyItem {
	virtual void   Process(XmlParser& p);
	virtual void   AddTo(Ctrl& parent);
	virtual void   Remove();
	virtual String GetResults();
};

struct MyOptionItem : MyItem {
	Option x;

	virtual String GetResults() {
		return x ? "on" : "off";
	}
};

struct MyButton : MyItem {
	Button button;

	virtual void   Process(XmlParser& p) {

	}
	virtual void   AddTo(Ctrl& parent) {
		parent.Add(button);
	}
	virtual void   Remove() {
		button.Remove();
	}

	virtual String GetResults() {
		return Null;
	}
}

struct TheGui {
	Index<String> ids;
	Array<MyItem> items;
	Index<String> parent_ids;

	int selected;

	void RemoveWithParent(const String& id, Vector<int>& remove) {
		for(;;) {
			int q = parent_ids.Find(id);
			if(q < 0)
				break;
			RemoveWithParent(ids[q]);
		}
		ids.Unlink(q);
		remove.Add(q);
		parent_ids.Unlinke(q);
	}

public:
	void Select(const String& id) {
		selected = ids.Find(id);
	}

	void Remove(const String& parent);

	void RemoveSelected() {
		if(selected < 0)
			RemoveWithParent(ids[selected]);
	}

	TheGui() {
		selected = -1;
	}
};


void NotesCheck(STATUS nError)
{
	if(nError == NOERROR)
		return;
	char h[256];
	OSLoadString(GetModuleHandle(NULL), ERR(nError), h, 255);
	Exclamation(h);
	abort();
}

GUI_APP_MAIN
{
	if (NotesInitExtended(__argc, __argv) != NOERROR) {
		Exclamation("Unable to Initialize Notes!");
		return;
	}

	char h[256];

//	OSPathNetConstruct(NULL, "Notes3/MU_HP", "DOKUMENT\\LIB_test.nsf", h);

	DUMP(h);

	DBHANDLE hNotesDB;
	NotesCheck(NSFDbOpen("c:/notesapi/notedata/intro.nsf", &hNotesDB));
    NotesCheck(NSFDbInfoGet(hNotesDB, h));

	DUMP(h);
	NotesCheck(NSFDbClose(hNotesDB));

    NotesTerm();
}
