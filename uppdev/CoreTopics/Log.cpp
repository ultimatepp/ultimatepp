#include "Core.h"

NAMESPACE_UPP

#ifdef PLATFORM_WINCE
const char *FromSysChrSet(const wchar *s)
{
	static char out[256];
	FromUnicode(out, s, wstrlen(s), CHARSET_DEFAULT);
	return out;
}

const wchar *ToSysChrSet(const char *s)
{
	static wchar out[1024];
	ToUnicode(out, s, strlen(s), CHARSET_DEFAULT);
	return out;
}
#endif

LogStream::LogStream()
{
#ifdef PLATFORM_POSIX
	hfile = -1;
#else
	hfile = INVALID_HANDLE_VALUE;
#endif
	part = 0;
	sizelimit = 0;
	*filename = 0;
	options = LOG_FILE;
	depth = 0;
	bol = false;
}

LogStream::~LogStream() {}

void LogStream::Close()
{
#ifdef PLATFORM_POSIX
	if(hfile >= 0)
		close(hfile);
	hfile = -1;
#else
	if(hfile != INVALID_HANDLE_VALUE)
		CloseHandle(hfile);
	hfile = INVALID_HANDLE_VALUE;
#endif
}

bool LogStream::Delete()
{
	Close();
	if(*filename) {
		if(!FileDelete(filename)) {
			BugLog() << "Error deleting " << filename << ": " << GetLastErrorMessage();
			return false;
		}
		*filename = 0;
	}
	return true;
}

void LogStream::Create(const char *path, bool append)
{
	Close();

	strcpy(filename, path);
	strcpy(backup, filename);
	strcat(backup, ".old");

#if defined(PLATFORM_WIN32)

	#if defined(PLATFORM_WINCE)
		wchar_t pwcs[512];
		mbstowcs(pwcs, backup, strlen(backup));
		DeleteFile(pwcs);
	#else
		DeleteFile(backup);
	#endif

#elif defined(PLATFORM_POSIX)
	unlink(backup);
#else
	#error
#endif

#if defined(PLATFORM_WIN32)
	#if defined(PLATFORM_WINCE)
		wchar_t wfilename[512];
		mbstowcs(wfilename, filename, strlen(filename));
		MoveFile(wfilename, pwcs);
	#else
		MoveFile(filename, backup);
	#endif
#elif defined(PLATFORM_POSIX)
	rename(filename, backup);
#else
	#error
#endif

	filesize = 0;

#ifdef PLATFORM_WIN32
	hfile = CreateFile(ToSysChrSet(filename),
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		append ? OPEN_ALWAYS : CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if(append)
		filesize = (int)SetFilePointer(hfile, 0, NULL, FILE_END);
#else
	hfile = open(filename, append ? O_CREAT|O_RDWR|O_APPEND : O_CREAT|O_RDWR|O_TRUNC, 0644);
	if(append)
		filesize = (int)lseek(hfile, 0, SEEK_END);
#endif
	wrlim = ptr = (byte *)this;
	p = buffer;

	Time t = GetSysTime();
#ifdef PLATFORM_WINCE
	wchar exe[512];
#else
	char exe[512];
#endif
	char user[500];
	*user = 0;

#ifdef PLATFORM_WIN32
	GetModuleFileName(AppGetHandle(), exe, 512);
#ifndef PLATFORM_WINCE
	dword w = 2048;
	GetUserName(user, &w);
#endif
#else //#
	const char *procexepath_();
	strcpy(exe, procexepath_());
	const char *uenv = getenv("USER");
	strcpy(user, uenv ? uenv : "boot");
#endif

	char h[1000];
	sprintf(h, "* %s %02d.%02d.%04d %02d:%02d:%02d, user: %s\n",
	           FromSysChrSet(exe),
	           t.day, t.month, t.year, t.hour, t.minute, t.second, user);
#ifdef PLATFORM_WIN32
	dword n;
	WriteFile(hfile, h, (dword)strlen(h), &n, NULL);
	if(part) {
		sprintf(h, ", #%d", part);
		WriteFile(hfile, h, (dword)strlen(h) , &n, NULL);
	}
	WriteFile(hfile, "\r\n", 2, &n, NULL);
#else
	write(hfile, h, strlen(h));
	if(part) {
		sprintf(h, ", #%d", part);
		write(hfile, h, strlen(h));
	}
	write(hfile, "\r\n", 2);
#endif
}

void LogStream::Flush()
{
	int count = (int)(p - buffer);
	if(count == 0) return;
	if(options & LOG_COUT)
		Cout().Put(buffer, count);
	if(options & LOG_CERR)
		Cerr().Put(buffer, count);
#ifdef PLATFORM_WIN32
	if(options & LOG_FILE)
		if(hfile != INVALID_HANDLE_VALUE) {
			dword n;
			WriteFile(hfile, buffer, count, &n, NULL);
		}
	if(options & LOG_DBG) {
		*p = 0;
		::OutputDebugString((LPCSTR)buffer);
	}
#else
	if(options & LOG_FILE)
		if(hfile >= 0)
			write(hfile, buffer, count);
	if(options & LOG_DBG)
		Cerr().Put(buffer, count);
#endif
	filesize += count;
	p = buffer;
	if(sizelimit > 0 && filesize > sizelimit)
		Create(filename, false);
}

void LogStream::Put0(int w)
{
	if(w == LOG_BEGIN)
		depth++;
	else
	if(w == LOG_END)
		depth--;
	else {
		if(bol) {
			bol = false;
			for(int q = depth; q--;)
				Put0('\t');
		}
		*p++ = w;
		if(w == '\n') {
			Flush();
			bol = true;
		}
		else
		if(p == buffer + 512)
			Flush();
	}
}

void LogStream::_Put(int w)
{
	CriticalSection::Lock __(cs);
	Put0(w);
}

void  LogStream::_Put(const void *data, dword size)
{
	CriticalSection::Lock __(cs);
	const byte *q = (byte *)data;
	while(size--)
		Put0(*q++);
}

bool LogStream::IsOpen() const
{
#ifdef PLATFORM_POSIX
	return hfile >= 0;
#else
	return hfile != INVALID_HANDLE_VALUE;
#endif
}

static void sLarge(String& text, size_t *large, int count, const char *txt)
{
	int n = min(1024, count);
	Sort(large, large + n, StdLess<size_t>());
	int i = 0;
	while(i < n) {
		size_t q = large[i];
		int nn = i++;
		while(i < n && large[i] == q) i++;
		nn = i - nn;
		if(q < 10000)
			text << Format("%4d B, %5d %s (%6d KB)\r\n", (int)(uintptr_t)q, nn, txt, (int)(uintptr_t)((nn * q) >> 10));
		else
			text << Format("%4d`KB, %5d %s (%6d KB)\r\n", (int)(uintptr_t)(q >> 10), nn, txt, (int)(uintptr_t)((nn * q) >> 10));
	}
}

String AsString(MemoryProfile& mem)
{
	String text;
	int acount = 0;
	size_t asize = 0;
	int fcount = 0;
	size_t fsize = 0;
	for(int i = 0; i < 1024; i++)
		if(mem.allocated[i]) {
			int sz = 4 * i;
			text << Format("%4d B, %6d allocated (%5d KB), %6d fragmented (%5d KB)\n",
			              sz, mem.allocated[i], (mem.allocated[i] * sz) >> 10,
			              mem.fragmented[i], (mem.fragmented[i] * sz) >> 10);
			acount += mem.allocated[i];
			asize += mem.allocated[i] * sz;
			fcount += mem.fragmented[i];
			fsize += mem.fragmented[i] * sz;
		}
	text << Format(" TOTAL, %6d allocated (%5d KB), %6d fragmented (%5d KB)\n",
	              acount, int(asize >> 10), fcount, int(fsize >> 10));
	text << "Free pages " << mem.freepages << " (" << mem.freepages * 4 << " KB)\n";
	text << "Large block count " << mem.large_count
	    << ", total size " << (mem.large_total >> 10) << " KB\n";
//	sLarge(text, mem.large_size, mem.large_count, "allocated");
	text << "Large fragments count " << mem.large_free_count
	    << ", total size " << (mem.large_free_total >> 10) << " KB\n";
//	sLarge(text, mem.large_free_size, mem.large_free_count, "fragments");
	return text;
}


#ifdef _MULTITHREADED

StaticCriticalSection sLogLock;

void LockLog()
{
	sLogLock.Enter();
}

void UnlockLog()
{
	sLogLock.Leave();
}

#endif

#ifdef flagCHECKINIT

void InitBlockBegin__(const char *fn, int line) {
	RLOG(fn << " " << line << " init block");
#ifdef HEAPDBG
	MemoryCheckDebug();
#else
	MemoryCheck();
#endif
}

void InitBlockEnd__(const char *fn, int line) {
	RLOG(fn << " " << line << " init block finished");
#ifdef HEAPDBG
	MemoryCheckDebug();
#else
	MemoryCheck();
#endif
}

#endif

END_UPP_NAMESPACE
