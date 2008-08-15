#include <bigmailer/bigmailer.h>


// funcines del menú principal
void bigmailer::onNewMsg()
{
	onCloseMsg() ;
	messagePage.filename = ConfigFile("newMessage.qtf") ;
	SetTitle() ;
	if (tab.Get() != 1)
		tab.Set(1) ;

	theCfg.lastMsg = messagePage.filename ;
	theCfg.Save() ;

}
void bigmailer::onOpenMsg()
{
	FileSel fs;
	fs.ActiveDir(ConfigFile("")) ;
	fs.Type("list files", "*.msg") ;

	if ( ! fs.ExecuteOpen(t_("Select a message file") ) )
		return;

	onCloseMsg() ;

	messagePage.Load(fs.Get()) ;
	SetTitle() ;
	if (tab.Get() != 1)
		tab.Set(1) ;

}
void bigmailer::onCloseMsg()
{
	messagePage.editor.Clear() ;
	messagePage.editor.ClearModify() ;
	messagePage.filename = "" ;
}
void bigmailer::onPrintMsg()
{
	//messagePage.editor.Print() ;
}


// clase para editar el mensaje

MessageEditor::MessageEditor()
{
	CtrlLayout( *this ) ;
	editor.ClearModify();
	//editor.WhenRefreshBar= THISBACK(SetBar);
	SetBar();
	editor.WhenRefreshBar = THISBACK(SetBar);

  btSave.WhenAction = THISBACK(Save) ;
  edFrom.WhenAction = edSubject.WhenAction = THISBACK(CheckModify) ;


}
void MessageEditor::FillFields()
{
	fields.Clear() ;
	for (int i = 0; i < theDefList.fields.GetCount(); i++)
		fields.Add(theDefList.fields[i]) ;
	fields.WhenAction = THISBACK( OnInsertField ) ;
}

void MessageEditor::OnInsertField()
{
	if (editor.IsReadOnly())
		return ;
	String sel = fields.GetData() ;
	if (sel.IsEmpty())
		return ;
	sel = "[%" +  sel + "%]" ;
	editor.PasteText(AsRichText(sel.ToWString())) ;
}

void MessageEditor::CheckModify()
{
	btSave.Enable( editor.IsModified() || edFrom.IsModified() || edSubject.IsModified() ) ;
}

void MessageEditor::MainBar(Bar& bar)
{

	CheckModify() ;

	//bar.Add(editor.IsModified(), "Save", CtrlImg::save(), THISBACK(Save))
	//   .Key(K_CTRL_S)
	//   .Help(t_("Save current document") );

	bar.Add("SaveAs", CtrlImg::save_as(), THISBACK(SaveAs))
	   .Help(t_("Save current document with a new name" ) );

	bar.Gap(2) ;
	editor.CutTool(bar) ;
	editor.CopyTool(bar);
	editor.PasteTool(bar);

	bar.Gap(2);
	editor.FaceTool(bar);
	bar.Gap(2) ;

	editor.HeightTool(bar);
	bar.Gap(2);
	editor.BoldTool(bar);
	editor.UnderlineTool(bar) ;
	editor.ItalicTool(bar);

	bar.Gap(2);
	editor.InkTool(bar);
	editor.PaperTool(bar);

	bar.Gap(2) ;
	editor.LeftTool(bar);
	editor.CenterTool(bar);
	editor.RightTool(bar);
	editor.JustifyTool(bar);

	bar.Gap(2)	 ;
	editor.ObjectTool(bar) ;

	bar.Gap(2) ;
	bar.Add(true, fields, 100) ;
		fields.HelpLine( t_("Insert field data in text") ) ;


}

void MessageEditor::SetBar()
{
	toolbar.Set(THISBACK(MainBar));
}

void MessageEditor::Load(const String& name)
{
	theMsg.Load(name) ;
	edFrom.SetData(theMsg.from) ;
	edSubject.SetData(theMsg.subject) ;
	editor.SetQTF(theMsg.qtfBody) ;
	/*
	VectorMap<String,String> data =  LoadIniFile(name + ".ini") ;
	edFrom.SetData( data.Get("from") ) ;       edFrom.ClearModify() ;
	edSubject.SetData( data.Get("subject") ) ; edSubject.ClearModify() ;

	String qtf, s ;
	for (int i = 0; true; i++)
	{
		s = data.Get("body" + AsString(i), "") ;
		if (s.IsEmpty())
			break ;
		qtf += s + "\r\n" ;
	}
	editor.SetQTF(qtf) ;

	//editor.SetQTF(LoadFile(name));
*/
	filename = name;
	editor.ClearModify();
	SetBar() ;
	bigmailer::Mailer().SetTitle() ;
}

void MessageEditor::Save()
{
	//if(!editor.IsModified() ) return;
	if(filename.IsEmpty())
		SaveAs();
	else
	{
		theMsg.from = ~edFrom ;
		theMsg.subject = ~edSubject ;
		theMsg.qtfBody = editor.GetQTF() ;
		theMsg.Save() ;
		ClearModify();
	}
	SetBar() ;
	bigmailer::Mailer().SetTitle() ;
}

void MessageEditor::SaveAs()
{
	/*
	String pngStr = LoadFile("/home/nico/dobleDorje4.png") ;
	RichObject pngObj("PNG", pngStr) ;
  RichPara p ;
  RichPara::CharFormat f ;
  p.Cat(pngObj,f) ;
  //editor.ObjectTool(Bar& bar¸ dword key = 0)

	RichObject CreateImageObject(const Image& img);
	String AsQTF(const RichObject& obj);

	*/

	FileSel fs ;
	fs.Type(t_("Message files"), "*.msg") ;
	fs.ActiveDir(ConfigFile("")) ;
	if(fs.ExecuteSaveAs())
	{
		filename = fs.Get();
		Save() ;
	}
	SetBar() ;
	bigmailer::Mailer().SetTitle() ;

}
