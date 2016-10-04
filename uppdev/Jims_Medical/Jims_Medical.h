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

	void Cancel();
	void Next();

public:
	typedef Jims_Medical CLASSNAME;
	Jims_Medical();
};

#endif
