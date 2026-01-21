#include <CtrlLib/CtrlLib.h>

namespace Upp {

LineEdit *dlog = NULL;
int dlev = 0;

static int pos = 0;
static constexpr int buff_size = 1024;

void LogCon(const char *fmt, ...)
{
	char buffer[buff_size];
	va_list argptr;
	va_start(argptr, fmt);
	vsnprintf(buffer, buff_size, fmt, argptr);
	va_end(argptr);
	LOG(buffer);
}

void LogCon(int level, const char *fmt, ...)
{
	char buffer[buff_size];
	va_list argptr;
	va_start(argptr, fmt);
	vsnprintf(buffer, buff_size, fmt, argptr);
	va_end(argptr);
	LOG(buffer);
}

void LogGui(const char *fmt, ...)
{
	if(!dlog)
		return;

	pos = dlog->GetLength();
	char buffer[buff_size];
	va_list argptr;
	va_start(argptr, fmt);
	int l = vsnprintf(buffer, buff_size, fmt, argptr);
	va_end(argptr);

	dlog->Insert(pos, buffer);
	pos += l;
	dlog->Insert(pos, "\n");
	pos++;
	dlog->SetCursor(pos);
}

void LogGui(int level, const char *fmt, ...)
{
	if(!dlog || level < dlev)
		return;

	pos = dlog->GetLength();
	char buffer[buff_size];
	va_list argptr;
	va_start(argptr, fmt);
	int l = vsnprintf(buffer, buff_size, fmt, argptr);
	va_end(argptr);

	dlog->Insert(pos, buffer);
	pos += l;
	dlog->Insert(pos, "\n");
	pos++;
	dlog->SetCursor(pos);
}

}
