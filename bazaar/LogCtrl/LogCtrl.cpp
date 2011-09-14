#include "LogCtrl.h"

LoggerCtrl::LoggerCtrl() : maxlines(1000), ignore(false)
{
	NoInitFocus();
	NoWantFocus();
	SetReadOnly();
	//Reserve(maxlines*64);
}

void LoggerCtrl::Save()
{
	FileSel fs;
	fs.Type("Text file", "*.txt");
	if(!fs.ExecuteSaveAs("Save Log to file")) return;
	String fn = fs.Get();
	if(fn.IsEmpty()) return;
	FileOut out(fn);
	Flush();
	DocEdit::Save(out);
}

void LoggerCtrl::Updated()
{
	int lc = GetLineCount();
	if(lc <= maxlines) { DocEdit::Updated(); return; }
	int lp = DocEdit::GetPos(lc/2);
	Remove(0, lp);
	DocEdit::Updated();
}

void LoggerCtrl::_Put(int w)
{
	if(ignore) return;
	String d;
	d.Cat(w);
	//String s = EscapeCh((unsigned)w);
	GuiLock _;
	Insert(GetLength(), d);
	SetCursor(GetLength());
}

void LoggerCtrl::_Put(const void *data, dword size)
{
	if(ignore) return;
	String d;
	d.Cat((const char*)data, size);
	//String s = EscapeCh(d);
	GuiLock _;
	Insert(GetLength(), d);
	SetCursor(GetLength());
}

LogCtrl::LogCtrl() : ps(NULL)
{

}

void LogCtrl::Log(bool b)
{
	if(ps == this) return;
	if(b)
	{
		ps = &VppLog();
		SetVppLog(*this);
	}
	else if(ps)
	{
		SetVppLog(*ps);
		ps = NULL;
	}
}

void LogCtrl::Reset()
{
	SetVppLog(StdLog());
	ps = NULL;
}

INITBLOCK
{
	LogLev(LDBG);
}
