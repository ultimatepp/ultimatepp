#ifndef _messages_h_
#define _messages_h_

struct MessageEditor : public WithMessageLayout<ParentCtrl> {

	//RichEdit   editor;
	//ToolBar    toolbar;
	DropList   fields ;
	String     filename;

	void Load(const String& filename);
	void OpenFile(const String& fn);
	void Save();
	void SaveAs();

	void SetBar();
	void MainBar(Bar& bar);

	void OnInsertField() ;
	void FillFields() ;

	void CheckModify() ;

public:
	typedef MessageEditor CLASSNAME;

	MessageEditor();
};






#endif
