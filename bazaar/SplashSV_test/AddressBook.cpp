#include <CtrlLib/CtrlLib.h>
#include <Report/Report.h>

using namespace Upp;

#define LAYOUTFILE <SplashSV_test/AddressBook.lay>
#include <CtrlCore/lay.h>

#define TAG_ADDRESSBOOK "AddressBook"
#define TAG_PERSON "person"
#define TAG_NAME "name"
#define TAG_SURNAME "surname"
#define TAG_ADDRESS "address"
#define TAG_EMAIL "email"

													//
#define IMAGECLASS SVLogo							// Adding Graphic LOGO 
#define  IMAGEFILE <SplashSV_test/SVLogo.iml>		//
#include <Draw/iml.h>								//

#define TOPICFILE <SplashSV_test/app.tpp/all.i>		// Adding QTF for splash (and for other aims)
#include <Core/topic_group.h>						//

#define SPLASHQTF "SplashSV_test/app/Splash"		//
#define SPLASH_LOGO_NATIVE       SVLogo::logoENUS()	//
#define SPLASH_LOGO_INTERNATIONAL SVLogo::logoENUS()//  this is Parameter set for Splash
#define SPLASH_NATIVE_LANG "en-us"					//
#include <SplashSV/splash-sv.h>						//



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
	void FileMenu(Bar& bar);
	void MainMenu(Bar& bar);

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
	search.oname <<= search.osurname <<= search.oaddress
	             <<= search.oemail <<= THISBACK(SetupSearch);
	array.AddColumn(TAG_NAME, "Name");
	array.AddColumn(TAG_SURNAME, "Surname");
	array.AddColumn(TAG_ADDRESS, "Address");
	array.AddColumn(TAG_EMAIL, "Email");
	modify.add <<= THISBACK(Add);
	modify.change <<= THISBACK(Change);
	search.search <<= THISBACK(Search);
	SetupSearch();
	fs.AllFilesType();
	menu.Set(THISBACK(MainMenu));
}

void AddressBook::FileMenu(Bar& bar)
{
	bar.Add("Open..", CtrlImg::open(), THISBACK(Open));
	bar.Add("Save", CtrlImg::save(), THISBACK(Save));
	bar.Add("Save as..", CtrlImg::save_as(), THISBACK(SaveAs));
	bar.Separator();
	bar.Add("Print", CtrlImg::print(), THISBACK(Print));
	bar.Separator();
	bar.Add("Quit", THISBACK(Quit));
}

void AddressBook::MainMenu(Bar& bar)
{
	bar.Add("File", THISBACK(FileMenu));
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
	array.Clear();
	try {
		String d = LoadFile(filename);
		XmlNode n = ParseXML(d);
		if(n.GetCount()==0 || n[0].GetTag() != TAG_ADDRESSBOOK)
			throw XmlError("No AddressBook tag");
		const XmlNode &ab = n[0];
		array.SetCount(ab.GetCount());
		for(int i=0; i < ab.GetCount(); i++){
			const XmlNode &person = ab[i];
			for(int j=0; j < person.GetCount(); j++){
				const XmlNode &prop = person[j];
				if(prop.IsTag(TAG_NAME))
					array.Set(i, TAG_NAME, prop[0].GetText());
				else
				if(prop.IsTag(TAG_SURNAME))
					array.Set(i, TAG_SURNAME, prop[0].GetText());
				else
				if(prop.IsTag(TAG_ADDRESS))
					array.Set(i, TAG_ADDRESS, prop[0].GetText());
				else
				if(prop.IsTag(TAG_EMAIL))
					array.Set(i, TAG_EMAIL, prop[0].GetText());
			}
		}
	}
	catch(XmlError &e) {
		Exclamation("Error reading the input file:&" + DeQtf(e) );
	}
}

void AddressBook::Save()
{
	if(IsEmpty(filename)) {
		SaveAs();
		return;
	}
	XmlNode n;
	XmlNode &ab = n.Add(TAG_ADDRESSBOOK);
	for(int i=0; i < array.GetCount(); i++) {
		XmlNode &person = ab.Add(TAG_PERSON);
		person.Add(TAG_NAME).Add().CreateText(array.Get(i, TAG_NAME));
		person.Add(TAG_SURNAME).Add().CreateText(array.Get(i, TAG_SURNAME));
		person.Add(TAG_ADDRESS).Add().CreateText(array.Get(i, TAG_ADDRESS));
		person.Add(TAG_EMAIL).Add().CreateText(array.Get(i, TAG_EMAIL));
	}
	if(!SaveFile(filename, AsXML(n)))
		Exclamation("Error saving the file!");
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
	SplashInit();									// Splash Initializing and Show screen with text "Program Loading"
	ShowSplashStatus(t_("Loading Settings..."));	// Show message at bottom of splash screen
	AddressBook ab;
	LoadFromFile(ab);
	ShowSplashStatus(t_("Normal Running..."));		// Show message at bottom of splash screen
	SetSplashTimer(1000);							// Non standart splash-close timer set to 1 sec (maximum is 50 sec)
	ab.Run();
	StoreToFile(ab);
}
