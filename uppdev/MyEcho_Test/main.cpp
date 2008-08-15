#include "MyEcho.h"
String MyDocEdit::tipString(">");
MyApp app;

MyDocEdit::MyDocEdit()
{
		ShowTip();
}

static int value = 0;

void MyDocEdit::InsertData()
{
	char tmp[32];
	sprintf(tmp, "value: %d\n", value++);
	Insert(GetCursor(), tmp);
	SetCursor(GetCursor()+strlen(tmp)+1);
}

void MyDocEdit::ShowTip()
{
	dir = GetCurrentDirectory();
	Insert(cursor, dir+tipString);
	this->SetCursor(cursor+(dir+tipString).GetLength());
	inputString.Clear();
}
char str[32];
bool MyDocEdit::Key(dword key, int count)
{
	switch(key & ~(K_CTRL | K_ALT | K_SHIFT)) {
		case K_ENTER:
			RunCommand(inputString);
			inputString.Clear();
			if(GetLineCount() > MAX_CACHE_LINE)
				Remove(0, GetPos(GetLineCount() - MAX_CACHE_LINE));
			ShowTip();
			return true;
		default:
			if(!(key & 0x10000) &&  !(key & 0x110000)) { 
				inputString +=(char)key;
			}
			break;
	}
	
	return DocEdit::Key(key, count);
}

int MyDocEdit::RunCommand(String user_input)
{
	FindFile ff;
	Insert(GetCursor(), "\n");
	cursor++;
	if(!ff.Search(AppendFileName(dir, "*")))
		return false;
	do {
		Insert(GetCursor(), ff.GetName()+"\n");
		
		SetCursor(GetCursor()+ff.GetName().GetLength()+1);
	}
	while(ff.Next());
	return 0;
}

GUI_APP_MAIN
{
	app.Run();
}

