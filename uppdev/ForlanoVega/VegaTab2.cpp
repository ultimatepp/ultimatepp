#include "VegaMain.h"


VegaTab2::VegaTab2()
{
	CtrlLayout(*this);
	SizePos();
	              //id, label, width
	arrp.AddColumn("Board", "Board", 5) ;   //0
	arrp.AddColumn("WhitePlayer", "White Player", 15);         //1
	arrp.AddColumn("ScoreW", " ", 5);       //3
	arrp.AddColumn("BlackPlayer", "Black Player", 15);         //1
	arrp.AddColumn("ScoreB", " ", 5);       //3
	arrp.AddColumn("IDW", "ID white", 5);       //3
	arrp.AddColumn("IDB", "ID black", 5);       //3

	//arr.WhenLeftClick = THISBACK(PromptCell);
	//arr.WhenLeftDouble = THISBACK(SortRecord);

	// color even rows
	arrp.EvenRowColor();
	//avoid two identical row
	//arr.NoDuplicating();  //doesn't work!

	//set the callback for the Add_Player button
	//btnAdd <<= THISBACK(AddPlayer);
/*    btnAdd <<= THISBACK(AddPlayer);
    btnModify <<= THISBACK(ModifyPlayer);
    btnClear <<= THISBACK(MaskDefaultValue);
*/
}
