#ifndef _Jims_Medical_Jims_Medical_h
#define _Jims_Medical_Jims_Medical_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <Jims_Medical/Jims_Medical.lay>
#include <CtrlCore/lay.h>



class Jims_Medical : public WithJims_MedicalLayout<TopWindow> {
	MenuBar menu;
	StatusBar status;

	void Save_to_Text();
	void FileMenu(Bar& bar);

	void About();
	void HelpMenu(Bar& bar);
public:
	typedef Jims_Medical CLASSNAME;
	Jims_Medical();
};

void Jims_Medical::Save_to_Text()
{
	PromptOK("This is an imcomplete feature. "
	"When completed, this feature will convert input data to a nicely formatted RichText file.");
}

void Jims_Medical::FileMenu(Bar& bar)
{
	bar.Add("Save to text...", THISBACK(Save_to_Text));
	bar.Separator();
	bar.Add("Exit\tAlt+F4", THISBACK(Close));
}

void Jims_Medical::About()
{
	PromptOK("When completed, this program will take sets of medical input values and "
	"convert them into a medical document which will be nicely formatted for "
	"efficiency and legibility.");
}

void Jims_Medical::HelpMenu(Bar& bar)
{
	bar.Add("About", THISBACK(About));
}

Jims_Medical::Jims_Medical()
{
	CtrlLayout(*this, "Medical Information Input");
	Sizeable().MinimizeBox().MaximizeBox();

	AddFrame(menu);
	AddFrame(status);

	menu.Add("File", THISBACK(FileMenu));
	menu.Add("Help", THISBACK(HelpMenu));
	status="Medical information input utility...";
}


#endif
