#include "Core.h"

NAMESPACE_UPP

StaticCriticalSection log_mutex;

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

struct LogOut {
	dword options;
	int   sizelimit;

#ifdef PLATFORM_WIN32
	HANDLE hfile;
#endif
#ifdef PLATFORM_POSIX
	enum { INVALID_HANDLE_VALUE = -1 };
	int   hfile;
#endif

	char  filepath[512];
	int   filesize;

	int   part;
	
	bool  line_begin;
	
	void  Create(bool append);
	void  Create()                                     { Create(options & LOG_APPEND); }
	void  Close();
	void  Line(const char *buffer, int len, int depth);
	bool  IsOpen() const;
	void  Rotate();
};

bool LogOut::IsOpen() const
{
#ifdef PLATFORM_POSIX
	return hfile >= 0;
#else
	return hfile != INVALID_HANDLE_VALUE;
#endif
}

void LogOut::Rotate()
{
}

void LogOut::Create(bool append)
{
	Close();
	
	line_begin = true;
	
	int rotn = options >> 24;
	if(rotn) {
		char next[512];
		for(int rot = rotn; rot >= 0; rot--) {
			char current[512];
			if(rot == 0)
				strcpy(current, filepath);
			else
				sprintf(current, rot > 1 && (options & LOG_ROTATE_GZIP) ? "%s.%d.gz" : "%s.%d",
				        filepath, rot);
			if(FileExists(current)) {
				if(rot == rotn)
					FileDelete(current);
				else
				if((options & LOG_ROTATE_GZIP) && rot == 1 && !IsPanicMode()) {
					GZCompressFile(next, current); // Should be OK to use heap in Create...
				}
				else
					FileMove(current, next);
			}
			strcpy(next, current);
		}
	}
	
	filesize = 0;

#ifdef PLATFORM_WIN32
	hfile = CreateFile(ToSysChrSet(filepath),
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
	hfile = open(filepath, append ? O_CREAT|O_RDWR|O_APPEND : O_CREAT|O_RDWR|O_TRUNC, 0644);
	if(append)
		filesize = (int)lseek(hfile, 0, SEEK_END);
#endif
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
	::GetUserNameA(user, &w);
#endif
#else //#
	const char *procexepath_(void);
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
	IGNORE_RESULT(
		write(hfile, h, strlen(h))
	);
	if(part) {
		sprintf(h, ", #%d", part);
		IGNORE_RESULT(
			write(hfile, h, strlen(h))
		);
	}
	IGNORE_RESULT(
		write(hfile, "\r\n", 2)
	);
#endif
}

void LogOut::Close()
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

void LogOut::Line(const char *s, int len, int depth)
{
	Mutex::Lock __(log_mutex);

	char h[600];
	char *p = h;
	int   ll = 0;
	if((options & (LOG_TIMESTAMP|LOG_TIMESTAMP_UTC)) && line_begin) {
		Time t = (options & LOG_TIMESTAMP_UTC) ? GetUtcTime() : GetSysTime();
		ll = sprintf(h, "%02d.%02d.%04d %02d:%02d:%02d ",
		                t.day, t.month, t.year, t.hour, t.minute, t.second);
		if(ll < 0)
			return;
		p += ll;
	}
	char *beg = p; 
	for(int q = min(depth, 99); q--;)
		*p++ = '\t';
	line_begin = len && s[len - 1] == '\n';
	memcpy(p, s, len);
	p += len;
	*p = '\0';
	int count = (int)(p - h);
	if(count == 0) return;
	if(options & LOG_COUT)
		Cout().Put(h, count);
	if(options & LOG_CERR)
		Cerr().Put(h, count);
#ifdef PLATFORM_WIN32
	if(options & LOG_FILE)
		if(hfile != INVALID_HANDLE_VALUE) {
			dword n;
			WriteFile(hfile, h, count, &n, NULL);
		}
	if(options & LOG_DBG) {
		*p = 0;
		::OutputDebugString((LPCSTR)h);
	}
#else
	if(options & LOG_FILE)
		if(hfile >= 0)
			IGNORE_RESULT(
				write(hfile, h, count)
			);
	if(options & LOG_DBG)
		Cerr().Put(h, count);
	if(options & LOG_SYS)
		syslog(LOG_INFO|LOG_USER, "%s", beg);
#endif
	filesize += count;
	if(sizelimit > 0 && filesize > sizelimit)
		Create(false);
}

#ifdef PLATFORM_POSIX
static LogOut sLog = { LOG_FILE, 10 * 1024 * 1024, -1 };
#else
static LogOut sLog = { LOG_FILE, 10 * 1024 * 1024 };
#endif

struct ThreadLog {
	char  buffer[512];
	int   len;
	int   line_depth;
	int   depth;
	
	void  Put(int w);
};

static thread__ ThreadLog sTh;

void ThreadLog::Put(int w)
{
	if(w == LOG_BEGIN)
		depth = min(depth + 1, 20);
	else
	if(w == LOG_END)
		depth = max(depth - 1, 0);
	else {
		buffer[len++] = w;
		if(w == '\n' || len > 500) {
			sLog.Line(buffer, len, line_depth);
			len = 0;
		}
		if(w != '\r')
			line_depth = depth;
	}
}

class LogStream : public Stream {
protected:
	virtual void    _Put(int w);
	virtual void    _Put(const void *data, dword size);
	virtual int64   GetSize() const;

public:
	virtual   bool  IsOpen() const;
};

int64 LogStream::GetSize() const
{
	return sLog.filesize;
}

bool LogStream::IsOpen() const
{
	return sLog.IsOpen();
}

void LogStream::_Put(int w)
{
	sTh.Put(w);
}

void  LogStream::_Put(const void *data, dword size)
{
	const byte *q = (byte *)data;
	while(size--)
		sTh.Put(*q++);
}

String AsString(const MemoryProfile& mem)
{
	String text;
#ifdef UPP_HEAP
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
	text << "Free 4KB pages " << mem.freepages << " (" << mem.freepages * 4 << " KB)\n";
	text << "Large block count " << mem.large_count
	     << ", total size " << (mem.large_total >> 10) << " KB\n";
	text << "Large fragments count " << mem.large_free_count
	     << ", total size " << (mem.large_free_total >> 10) << " KB\n";
	text << "Large free 64KB pages " << mem.large_empty
	     << ", total size " << 64 * mem.large_empty << " KB\n";
	text << "Big block count " << mem.big_count
	     << ", total size " << int(mem.big_size >> 10) << " KB\n";
#endif
	return text;
}

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

#ifdef PLATFORM_WIN32
static void sLogFile(char *fn, const char *app = ".log")
{
#ifdef PLATFORM_WINCE
	wchar wfn[256];
	::GetModuleFileName(NULL, wfn, 512);
	strcpy(fn, FromSysChrSet(wfn));
#else
	::GetModuleFileName(NULL, fn, 512);
#endif
	char *e = fn + strlen(fn), *s = e;
	while(s > fn && *--s != '\\' && *s != '.')
		;
	strcpy(*s == '.' ? s : e, app);
}
#endif

#ifdef PLATFORM_POSIX
const char *procexepath_();
extern char Argv0__[_MAX_PATH + 1];

static void sLogFile(char *fn, const char *app = ".log")
{
	char *path = fn;
	const char *ehome = getenv("HOME");
	strcpy(fn, ehome ? ehome : "/root");
	if(!*fn || (fn += strlen(fn))[-1] != '/')
		*fn++ = '/';
	*fn = '\0';
	strcat(path, ".upp/");
	const char *exe = procexepath_();
	if(!*exe)
		exe = Argv0__;
	const char *q = strrchr(exe, '/');
	if(q)
		exe = q + 1;
	if(!*exe)
		exe = "upp";
	strcat(path, exe);
	mkdir(path, 0755);
	strcat(path, "/");
	strcat(path, exe);
	strcat(path, app);
}
#endif

static Stream *__logstream;

void SetVppLogSizeLimit(int limit) { sLog.sizelimit = limit; }
void SetVppLogNoDeleteOnStartup()  { sLog.options |= LOG_APPEND; }

LogStream& StdLogStream()
{
	static LogStream *s;
	ReadMemoryBarrier();
	if(!s) {
		static StaticCriticalSection lock;
		lock.Enter();
		if(!s) {
			static byte lb[sizeof(LogStream)];
			LogStream *strm = new(lb) LogStream;
			if(*sLog.filepath == '\0')
				sLogFile(sLog.filepath);
			sLog.Create();
			WriteMemoryBarrier();
			s = strm;
		}
		lock.Leave();
	}
	return *s;
}

void CloseStdLog()
{
	StdLogStream().Close();
}

void ReopenLog()
{
	if(sLog.IsOpen()) {
		sLog.Close();
		sLog.Create();
	}
}

void StdLogSetup(dword options, const char *filepath, int filesize_limit)
{
	sLog.options = options;
	sLog.sizelimit = filesize_limit;
	if(filepath)
		strcpy(sLog.filepath, filepath);
	ReopenLog();
}

Stream& StdLog()
{
	return StdLogStream();
}

void SetVppLog(Stream& log) {
	__logstream = &log;
}

void SetUppLog(Stream& log) {
	__logstream = &log;
}

Stream& UppLog() {
	if(!__logstream) __logstream = &StdLog();
	return *__logstream;
}

Stream& VppLog() {
	return UppLog();
}

void SetVppLogName(const String& file) {
	strcpy(sLog.filepath, file);
	ReopenLog();
}

namespace Ini {
	INI_BOOL(user_log, false, "Activates logging of user actions");
};

END_UPP_NAMESPACE
