/*
#include <bigmailer/bigmailer.h>


ListDataPage::ListDataPage()
{
	CtrlLayout(*this);

	btLoad.WhenAction = THISBACK(Fill) ;
	btSave.WhenAction = THISBACK(Save) ;

}


void ListDataPage::Fill()
{
	theGrid.Reset() ;
	for (int col = 0; col < theDefList.fields.GetCount(); col++)
	{
		theGrid.AddColumn(theDefList.fields[col]).Edit(theEdits[col]) ;
	}
	theGrid.EditCell().SelectRow(true).Removing(true).ShowRows(true) ;


	for (int row = 0; row < theDefList.data.GetCount(); row++)
	{
		Vector<String> d = Split(theDefList.data[row], ',') ;
		for (int col = 0; col < d.GetCount(); col++)
		{
			theGrid.Set(row, col, d[col]) ;
    }
	}
	theGrid.ClearModify() ;

}
void ListDataPage::Save()
{
	if ( ! theGrid.IsModified() )
		return ;

	String line ;
	theDefList.data.Clear() ;
	for (int row = 0; row < theGrid.GetCount(); row++)
	{
		line = "" ;
		for (int col = 0; col < theDefList.fields.GetCount(); col++)
		{
			if (col > 0)
				line += "," ;
			line << theGrid.Get(row, col) ;
		}
		theDefList.data.Add(line) ;
	}
}
*/