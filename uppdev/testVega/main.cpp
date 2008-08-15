#include "VegaMain.h"

RoundData TD; //is it seen in each *.cpp file?

void NewMask()
{	TD.NAMETORNEO = "luigi";
    NewTournamentDlg newt;
	newt.Run();
	PromptOK( TD.NAMETORNEO);
}

GUI_APP_MAIN
{  	TopWindow w;
	Button b;
	w.Add(b);
	b.SetLabel("push me").LeftPos(10, 100).TopPos(10, 30);
	b.WhenAction = callback(NewMask);
	w.Run();
}
