#include "LogCtrl.h"

LoggerCtrl::LoggerCtrl() : maxlines(1000), ignore(false)
{
	NoInitFocus();
	NoWantFocus();
	SetReadOnly();
	Reserve(512);
	
	SetTimeCallback(-200, THISBACK(Flush));
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
	StringStream::_Put(w);
}

void LoggerCtrl::_Put(const void *data, dword size)
{
	if(ignore) return;
	StringStream::_Put(data, size);
}

void LoggerCtrl::Flush()
{
	if(StringStream::GetSize() > 0)
	{
		Insert(GetLength(), GetResult());
		SetCursor(GetLength());
		StringStream::Create(); //empty
		Reserve(512);
	}
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
