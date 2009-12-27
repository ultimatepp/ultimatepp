#include <CtrlLib/CtrlLib.h>
#include <Report/Report.h>
#include <RepGen/RepGen.h>		//added

using namespace Upp;

#define LAYOUTFILE <RepGenTest_AddressBookXML2/AddressBook.lay>
#include <CtrlCore/lay.h>

#define TAG_ADDRESSBOOK "AddressBook"
#define TAG_PERSON "person"
#define TAG_NAME "name"
#define TAG_SURNAME "surname"
#define TAG_ADDRESS "address"
#define TAG_EMAIL "email"

class AddressBook : public WithAddressBookLayout<TopWindow> {
	WithModifyLayout<ParentCtrl> modify;
	WithSearchLayout<ParentCtrl> search;

	FileSel fs;
	String  filename;
	
	//--------------------------//
	RepGen rep;					//
	FileSel ftpl;				// added to have 
	String report_template;		//
	int iterator;				// template control 
	void PrintStart();			//
	void PrintBody();			// print ability
	void PrintTotal();			//
	int TOTAL;					//	
	//--------------------------//

	void SetupSearch();
	void Add();
	void Change();
	void Search();
	void Open();
	void Save();
	void SaveAs();
	void OpenTemplate();
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
	bar.Add("Select Report Template", CtrlImg::open(), THISBACK(OpenTemplate));
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

// -------------------VVV Added VVV ------------------------
void AddressBook::OpenTemplate()
{
	if(!ftpl.ExecuteOpen()) return;
	report_template = ftpl;
	DUMP(report_template);
}

void AddressBook::PrintStart()
{
	iterator = 0;
}

void AddressBook::PrintBody()
{
	rep.SubstVar("##NAME",(String)array.Get(iterator, 0));
	rep.SubstVar("##SURNAME",(String)array.Get(iterator, 1));
	rep.SubstVar("##ADDRESS",(String)array.Get(iterator, 2));
	rep.SubstVar("##EMAIL",(String)array.Get(iterator, 3));
	
	if (++iterator >= array.GetCount() ) rep.LoopDone(); 
}
void AddressBook::PrintTotal()
{
	TOTAL = array.GetCount();
	rep.SubstVar("##TOTAL",Format("%`",TOTAL));
}
// -------------------^^^ Added ^^^ ------------------------

// -------------------VVV Changeded VVV ------------------------
void AddressBook::Print()
{
	DUMP(report_template);
	
	String qtf = LoadFile(report_template);
	DUMP(qtf);
	rep.RepGenCalculateStart = THISBACK(PrintStart);
	rep.RepGenCalculateBody = THISBACK(PrintBody);
	rep.RepGenReportFinish = THISBACK(PrintTotal);

	rep.SetTemplate(qtf);
	rep.Perform();
}
// -------------------^^^ Changeded ^^^ ------------------------

void AddressBook::Quit()
{
	Break();
}
// -------------------VVV Changeded VVV ------------------------
void AddressBook::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	s % search.oname % search.osurname % search.oaddress % search.oemail;
	s % fs;
	s % ftpl;
	s % report_template;
	SetupSearch();
}
// -------------------^^^ Changeded ^^^ ------------------------

GUI_APP_MAIN
{
	AddressBook ab;
	LoadFromFile(ab);
	ab.Run();
	StoreToFile(ab);
}
