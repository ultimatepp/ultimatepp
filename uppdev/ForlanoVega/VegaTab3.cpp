#include "VegaMain.h"


VegaTab3::VegaTab3()
{
	CtrlLayout(*this);
	SizePos();
		              //id, label, width
	arrp.AddColumn("Board", "Board", 3) ;   //0
	arrp.AddColumn("WhitePlayer", "White Player", 19);         //1
	arrp.AddColumn("ScoreW", " ", 4);       //3
	arrp.AddColumn("BlackPlayer", "Black Player", 19);         //1
	arrp.AddColumn("ScoreB", " ", 4);       //3
	arrp.AddColumn("IDW", "ID white", 3);       //3
	arrp.AddColumn("IDB", "ID black", 3);       //3

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
