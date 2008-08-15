#include <bigmailer/bigmailer.h>
//#include <bigmailer/csvclass/csvparser.h>
#include "csvImport.h"


void bigmailer::onNewList()
{
	onCloseList() ;

	theDefList.name = t_("new list") ;
	theDefList.fields.Add( "name" ) ;
	theDefList.fields.Add( "email" ) ;

	SetTitle() ;
}
void bigmailer::onOpenList()
{
	FileSel fs;
	fs.ActiveDir(ConfigFile("")) ;
	fs.Type("list files", "*.list") ;

	if ( ! fs.ExecuteOpen(t_("select a list file") ) )
		return;

	onCloseList() ;

	theDefList.Load(fs.Get()) ;
	SetTitle() ;
}
void bigmailer::onCloseList()
{
	DataPageSave() ;
	theDefList.Clear() ;
	SetTitle() ;
}

void bigmailer::DataPageImportCSV()
{
	FileSel fs ;
	fs.ActiveDir(ConfigFile("")) ;
	fs.Type("csv files", "*.csv") ;

	if ( ! fs.ExecuteOpen(t_("")) )
		return;

	csvResult csv ;
	if ( ! csv.ImportFrom(fs.Get()) )
		return ;


	WithcsvToDefLayout<TopWindow> dlg;
	CtrlLayout(dlg, "Import From CSV");
	for (int i = 0; i < csv.fields.GetCount(); i++)
	{
		dlg.dropName.Add( csv.fields[i] ) ;
		dlg.dropEmail.Add( csv.fields[i] ) ;
	}
	dlg.dropName.SetIndex(0) ;
	dlg.dropEmail.SetIndex(1) ;
	dlg.btOk.WhenAction = Breaker(IDOK) ;
	dlg.btCancel.WhenAction = Breaker( IDCANCEL ) ;
	if (dlg.Execute() == IDOK)
	{
		if (PromptOKCancel("Con esta operación se van a eliminar [* TODOS]") == IDOK)
		{
			// añadimos la cabecera
			theDefList.fields.Clear() ;
			theDefList.fields.Add( dlg.dropName.GetValue() ) ;
			theDefList.fields.Add( dlg.dropEmail.GetValue() ) ;

			for (int i = 0; i < csv.fields.GetCount(); i++)
			{
				String f = csv.fields[i] ;
				if (f != theDefList.fields[0] && f != theDefList.fields[1])
					theDefList.fields.Add(f) ;
			}


			theDefList.data.Clear() ;
			for (int i = 0; i < csv.values.GetCount(); i++) // para cada registro
			{
				Vector<String>& line = csv.values[i] ;
				Vector<String>& record = theDefList.data.Add() ;

				for (int j = 0; j < theDefList.fields.GetCount(); j++)
				{
					String fieldName = theDefList.fields[j] ;
					for (int k = 0; k < csv.fields.GetCount(); k++)
					{
						if (fieldName == csv.fields[k])
						{
							record.Add( line[k] ) ;
							break ;
						}
					}
				}
			}

			theDefList.Save() ;
			DataPageFill() ;

		}
	}

}



// ---------------------------------------------------------------------------------------------
class DataListDefinitionDlg : public WithDataListDefinition<TopWindow> {
public:
	EditString edCell ;

	typedef DataListDefinitionDlg CLASSNAME;
	DataListDefinitionDlg();
	void OnSave() ;
};

DataListDefinitionDlg::DataListDefinitionDlg()
{
	CtrlLayout(*this, t_("Name and Field definition"));

	edListName.SetData( theDefList.name ) ;

	labelInfo.SetQTF(
t_(
"[ $$0,0#00000000000000000000000000000000:Default]"
"[s0;1 The 2 first fields are obligatory and they are not possible "
"to be modified.&]"
"[s0;1 You can create new fields to personalize its list.]"
)) ;

	grid.Appending().Removing().EditCell().AddColumn(t_("field")).Edit(edCell) ;
	grid.AddRow(theDefList.fields.GetCount()) ;
	grid.GetRow(0).Editable(false) ;
	grid.GetRow(1).Editable(false) ;

	for (int i = 0; i < theDefList.fields.GetCount(); i++)
		grid.Set(i,0, theDefList.fields[i]) ;
	//grid.SetFixedRows(3) ;


	btCancel.WhenAction = Breaker(IDCANCEL) ;
	btSave.WhenAction = THISBACK(OnSave) ;
}
void DataListDefinitionDlg::OnSave()
{
	theDefList.name = ~edListName ;
	theDefList.fields.Clear() ;

	theDefList.fields.Add("NAME") ;
	theDefList.fields.Add("EMAIL") ;

	for (int i = 0; i < grid.GetCount(); i++)
		theDefList.fields.Add( grid.Get(i,0)) ;

	theDefList.Save() ;
	Break(IDOK) ;
}
void bigmailer::DataPageEditFields()
{
	DataListDefinitionDlg	eso ;
	int resp = eso.Execute() ;
	if (resp != IDCANCEL)
		DataPageFill() ;
}
