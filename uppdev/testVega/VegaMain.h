#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE      <testVega/Vega.lay>
#include <CtrlCore/lay.h>

class NewTournamentDlg : public WithNewTournamentLayout<TopWindow> {
public:

	typedef NewTournamentDlg CLASSNAME;

	NewTournamentDlg();
	~NewTournamentDlg() {;}
};


class RoundData {
public:
 	String NAMETORNEO;

 	RoundData();
	~RoundData(){;}
};

// maybe the problem is the line below?
extern RoundData TD; //is it seen in each *.cpp file?
