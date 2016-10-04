#include <bigmailer/bigmailer.h>


GUI_APP_MAIN
{
	bigmailer mainWin ;
	mainWin.Run();
}

bigmailer *bigmailer::mailer = NULL ;

bigmailer::bigmailer()
{
	mailer = this ;

	AddFrame(mainMenu) ;
	Add(tab.SizePos()) ;

	Sizeable().MaximizeBox() ;

	//CtrlLayout(*this ) ;
	CtrlLayout(dataPage) ;

	//AddFrame(mainMenu) ;// mail addressees
	mainMenu.Add( t_("Addressees"), THISBACK( mnuMailAddr )  ) ;
	mainMenu.Add( t_("Messages"), THISBACK( mnuMsg ) ) ;
	mainMenu.Add( t_("Send"), THISBACK( mnuSend ) ) ;
	mainMenu.Add( t_("?"), THISBACK( mnuHelp ) ) ;

	theCfg.Load() ;   // cargamos la configuración

	//tab.Add(listDefinitionPage.SizePos(), t_("List definition") ) ;

	tab.Add(dataPage.SizePos(), t_("Mail Addresses") ) ;
		dataPage.btEditFields.WhenAction = THISBACK(DataPageEditFields) ;
		//dataPage.btSave.WhenAction = THISBACK(DataPageSave) ;
		dataPage.edListName.WhenAction = THISBACK(DataPageTheGridModification) ;
		dataPage.btSaveData.WhenAction = THISBACK(DataPageSave) ;
		dataPage.theGrid.WhenModification = THISBACK(DataPageTheGridModification);
		dataPage.theGrid.Absolute() ;
		dataPage.theGrid.Inserting().Removing().Appending().Absolute()
		                 .EditCell().SelectRow(true).ShowRows(true) ;


		dataPage.btCSV.WhenAction = THISBACK(DataPageImportCSV) ;
		dataPage.opErr.WhenAction = THISBACK(DataPageFill) ;

	tab.Add(messagePage.SizePos(), t_("Message's body") ) ;
	//tab.WhenSet = THISBACK(onTabSet);
	tab.WhenAction = THISBACK(onTabSet);

 	lastTabIndex = -1 ;
 	onStart() ;
}
void bigmailer::onStart()
{
	onCloseList() ;
	if (theCfg.lastList.IsEmpty())
		onNewList() ;
	else
		theDefList.Load(theCfg.lastList) ;

	if (theCfg.lastMsg.IsEmpty())
		onNewMsg() ;
	else
		messagePage.Load(theCfg.lastMsg) ;

	SetTitle() ;
}
void bigmailer::mnuMailAddr( Bar &bar )
{
	bar.Add(t_("New"), THISBACK( onNewList) ) ;
	bar.Add(t_("Open"), THISBACK( onOpenList) ) ;
	bar.Add(t_("Close"), THISBACK( onCloseList) ).Enable(theDefList.IsActive()) ;
	bar.MenuSeparator() ;
	bar.Add(t_("Print List"), THISBACK( onPrintList) ).Enable(theDefList.IsActive()) ;
	bar.MenuSeparator() ;
	bar.Add(t_("Exit"), THISBACK( onExit) ) ;
}
void bigmailer::mnuMsg( Bar &bar )
{
	bar.Add(t_("New"), THISBACK( onNewMsg) ) ;
	bar.Add(t_("Open"), THISBACK( onOpenMsg) ) ;
	bar.Add(t_("Close"), THISBACK( onCloseMsg) ) ;
	bar.MenuSeparator() ;
	bar.Add(t_("Print"), THISBACK( onPrintMsg) ) ;
}
void bigmailer::mnuSend( Bar &bar )
{
	bar.Add(t_("Send big mail"), THISBACK( onSend ) ) ;
}
void bigmailer::mnuHelp( Bar &bar )
{
	bar.Add(t_("Preferences"), THISBACK( onPreferences ))  ;
	bar.Add(t_("About..."), THISBACK( onAbout) ) ;
}

void bigmailer::SetTitle()
{
	String s = t_("The Big Mailer") ;
	if ( theDefList.IsActive() )
	{
		s << " [ " << theDefList.name << " ]" ;
		dataPage.Enable(true) ;
		DataPageFill() ;
	}
	else
	{
		dataPage.theGrid.Reset() ;
		dataPage.Enable(false) ;
	}

	if ( ! messagePage.filename.IsEmpty() )
	{
		s << " [" << messagePage.filename << " ]" ;
	}
	tab.GetItem(1).Enable( ! messagePage.filename.IsEmpty() ) ;

	// de momento el tab del cuerpo del mensaje está inactivo
	//tab.GetItem(1).Disable() ; messagePage.Show(false) ;

	Title( s ) ;
}

void bigmailer::onPrintList()
{
}
void bigmailer::onExit()
{
	onCloseList() ;
	onCloseMsg() ;
	Close() ;
}


void bigmailer::onAbout()
{

	PromptOK(
	  "[A5 The Big Emailer]&"
		"Copyleft Nicolás Mesas Plaza&&"
		"Released under the GPL license version 2 o higther&"
		"[^bigmailer.sourceforge.net^ bigmailer.sourceforge.net]"
		"]"
	) ;


}

void bigmailer::onTabSet()
{
	if (tab.Get() != lastTabIndex)
	{
		switch(lastTabIndex)
		{
		case 0:
			DataPageSave() ;
			break ;
		case 1:
			messagePage.Save() ;
			break ;
		}

		lastTabIndex = tab.Get() ;
		switch(lastTabIndex)
		{
		case 0:
			DataPageFill() ;
			break ;
		case 1:
			messagePage.FillFields() ;
			break ;
		}
	}

}

void bigmailer::DataPageFill()
{
	dataPage.edListName.SetData(theDefList.name) ;
	dataPage.theGrid.Reset() ;

	if (dataPage.opErr.GetData() == false)
	{
		for (int col = 0; col < theDefList.fields.GetCount(); col++)
			dataPage.theGrid.AddColumn(theDefList.fields[col],150).Edit(dataPage_theEdits[col]) ;
		//dataPage.theGrid.AddRow( theDefList.data.GetCount() ) ;
		for (int row = 0; row < theDefList.data.GetCount(); row++)
		{
			Vector<String> &d = theDefList.data[row] ;
			for (int col = 0; col < d.GetCount(); col++)
			{
				dataPage.theGrid.Set(row, col, d[col]) ;
	    }
		}
		if (dataPage.theGrid.GetColumnCount() > 4)
			dataPage.theGrid.Absolute() ;
		else
			dataPage.theGrid.Proportional() ;

		if (theDefList.data.GetCount() == 0)
			dataPage.theGrid.Append() ;
	}
	else
	{
		dataPage.theGrid.AddColumn("NAME", 150) ;
		dataPage.theGrid.AddColumn("EMAIL", 150) ;
		dataPage.theGrid.AddColumn("ERROR MESSAGE",500).Edit(dataPage_editError) ;
		//dataPage.theGrid.AddRow(theDefList.data.GetCount()) ;
		for (int row = 0; row < theDefList.data.GetCount(); row++)
		{
			dataPage.theGrid.Set(row, 0, theDefList.data[row][0]) ;
			dataPage.theGrid.Set(row, 1, theDefList.data[row][1]) ;
			if ( row < theDefList.errors.GetCount() )
				dataPage.theGrid.Set(row, 2, theDefList.errors[row]) ;
		}
		dataPage.theGrid.Absolute() ;
	}


	dataPage.theGrid.ClearModify() ;
	dataPage.edListName.ClearModify() ;

	dataPage.btSaveData.Enable(false) ;

	theCfg.lastList = theDefList.path ;
	theCfg.Save() ;
}
void bigmailer::DataPageSave()
{
	if ( dataPage.theGrid.IsModified() || dataPage.edListName.IsModified())
	{
		theDefList.name = ~dataPage.edListName ;
		dataPage.edListName.ClearModify() ;

		if (dataPage.opErr.GetData() == false)
		{
			theDefList.data.Clear() ;
			for (int row = 0; row < dataPage.theGrid.GetCount(); row++)
			{
				Vector<String> &v = theDefList.data.Add() ;
				for (int col = 0; col < theDefList.fields.GetCount(); col++)
				{
					v.Add( dataPage.theGrid.Get(row, col) ) ;
				}
			}
		}
		else
		{
			theDefList.errors.Clear() ;
			for (int row = 0; row < dataPage.theGrid.GetCount(); row++)
				theDefList.errors.Add(dataPage.theGrid.Get(row, 2)) ;
		}

		if (theDefList.path.IsEmpty())
		{
			FileSel fs;
			fs.ActiveDir(ConfigFile("")) ;
			fs.Type("list files", "*.list") ;
			fs.file.SetData(theDefList.name) ;

			if ( ! fs.ExecuteSelectDir(t_("select a folder to save") ) )
				return;

			theDefList.path = fs.Get() ;
		}

		theDefList.Save() ;
		dataPage.theGrid.ClearModify() ;
	}
	dataPage.btSaveData.Enable(false) ;
}
void bigmailer::DataPageTheGridModification()
{
	dataPage.btSaveData.Enable(true) ;
}
