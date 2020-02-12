// This is an upp conversion of qt example, see
// http://doc.trolltech.com/3.0/addressbook-example.html

#include <CtrlLib/CtrlLib.h>
#include <Report/Report.h>

using namespace Upp;

#define LAYOUTFILE <AddressBook/AddressBook.lay>
#include <CtrlCore/lay.h>

class AddressBook : public WithAddressBookLayout<TopWindow> {
	WithModifyLayout<ParentCtrl> modify;
	WithSearchLayout<ParentCtrl> search;
	FileSel fs;
	String  filename;

	void SetupSearch();
	void Add();
	void Change();
	void Search();
	void Open();
	void Save();
	void SaveAs();
	void Print();
	void Quit();

	typedef AddressBook CLASSNAME;

public:
	void Serialize(Stream& s);

	AddressBook();
};

AddressBook::AddressBook()
{
	CtrlLayout(*this, "Address book");
	CtrlLayout(modify);
	CtrlLayout(search);
	tab.Add(modify, "Modify");
	tab.Add(search, "Search");
	ActiveFocus(search.name);
	search.oname = true;
	search.oname ^= search.osurname ^= search.oaddress ^= search.oemail ^= THISFN(SetupSearch);
	array.AddColumn("Name");
	array.AddColumn("Surname");
	array.AddColumn("Address");
	array.AddColumn("Email");
	modify.add ^= THISFN(Add);
	modify.change ^= THISFN(Change);
	search.search ^= THISFN(Search);
	SetupSearch();
	fs.AllFilesType();
#ifdef PLATFORM_COCOA
	SetMainMenu([=](Bar& bar) {
		bar.Sub("File", [=](Bar& bar) {
			bar.Add("Open..", CtrlImg::open(), THISFN(Open));
			bar.Add("Save", CtrlImg::save(), THISFN(Save));
			bar.Add("Save as..", CtrlImg::save_as(), THISFN(SaveAs));
			bar.Separator();
			bar.Add("Print", CtrlImg::print(), THISFN(Print));
			bar.Separator();
			bar.Add("Quit", THISFN(Quit));
		});
	});
#else
	menu.Set([=](Bar& bar) {
		bar.Sub("File", [=](Bar& bar) {
			bar.Add("Open..", CtrlImg::open(), THISFN(Open));
			bar.Add("Save", CtrlImg::save(), THISFN(Save));
			bar.Add("Save as..", CtrlImg::save_as(), THISFN(SaveAs));
			bar.Separator();
			bar.Add("Print", CtrlImg::print(), THISFN(Print));
			bar.Separator();
			bar.Add("Quit", THISFN(Quit));
		});
	});
#endif
}

void AddressBook::SetupSearch()
{
	search.name.Enable(search.oname);
	search.surname.Enable(search.osurname);
	search.address.Enable(search.oaddress);
	search.email.Enable(search.oemail);
}

void AddressBook::Add()
{
	array.Add(~modify.name, ~modify.surname, ~modify.address, ~modify.email);
	array.GoEnd();
	modify.name <<= modify.surname <<= modify.address <<= modify.email <<= Null;
	ActiveFocus(modify.name);
}

void AddressBook::Change()
{
	if(array.IsCursor()) {
		array.Set(0, ~modify.name);
		array.Set(1, ~modify.surname);
		array.Set(2, ~modify.address);
		array.Set(3, ~modify.email);
	}
}

bool Contains(const String& text, const String& substr)
{
	for(const char *s = text; s <= text.End() - substr.GetLength(); s++)
		if(strncmp(s, substr, substr.GetLength()) == 0)
			return true;
	return false;
}

void AddressBook::Search()
{
	if(!array.GetCount()) return;
	bool sc = true;
	array.ClearSelection();
	for(int i = 0; i < array.GetCount(); i++) {
		if((!search.oname || Contains(array.Get(i, 0), ~search.name)) &&
		   (!search.osurname || Contains(array.Get(i, 1), ~search.surname)) &&
		   (!search.oaddress || Contains(array.Get(i, 2), ~search.address)) &&
		   (!search.oemail || Contains(array.Get(i, 3), ~search.email))) {
			array.Select(i);
			if(sc) {
				array.SetCursor(i);
				array.CenterCursor();
				sc = false;
			};
		}
	}
}

void AddressBook::Open()
{
	if(!fs.ExecuteOpen()) return;
	filename = fs;
	FileIn in(filename);
	if(!in) {
		Exclamation("Unable to open [* " + DeQtf(filename));
		return;
	}
	array.Clear();
	while(!in.IsEof()) {
		Vector<Value> q;
		for(int i = 0; i < 4; i++)
			q.Add(in.GetLine());
		array.Add(q);
	}
}

void AddressBook::Save()
{
	if(IsEmpty(filename)) {
		SaveAs();
		return;
	}
	FileOut out(filename);
	if(!out) {
		Exclamation("Unable to open " + filename);
		return;
	}
	for(int i = 0; i < array.GetCount(); i++)
		for(int q = 0; q < 4; q++)
			out.PutLine(String(array.Get(i, q)));
}

void AddressBook::SaveAs()
{
	if(!fs.ExecuteSaveAs()) return;
	filename = fs;
	Save();
}

void AddressBook::Print()
{
	String qtf;
	qtf = "{{1:1:1:1 Name:: Surname:: Address:: Email";
	for(int i = 0; i < array.GetCount(); i++)
		for(int q = 0; q < 4; q++)
			qtf << ":: " << DeQtf((String)array.Get(i, q));
	Report report;
	report << qtf;
	Perform(report);
}

void AddressBook::Quit()
{
	Break();
}

void AddressBook::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	s % search.oname % search.osurname % search.oaddress % search.oemail;
	s % fs;
	SetupSearch();
}

GUI_APP_MAIN
{
	AddressBook ab;
	LoadFromFile(ab);
	ab.Run();
	StoreToFile(ab);
}
