
const char LOG_BEGIN = '\x1e';
const char LOG_END = '\x1f';

class LogStream : public Stream {
#ifdef PLATFORM_WIN32
	HANDLE hfile;
#endif
#ifdef PLATFORM_POSIX
	enum { INVALID_HANDLE_VALUE = -1 };
	int   hfile;
#endif

	CriticalSection cs;
	char  filename[512];
	char  backup[512];
	byte  buffer[512];
	int   filesize;
	byte *p;

	int   sizelimit;
	int   part;
	dword options;
	int16 depth;
	bool  bol;

	void  Flush();
	void  Put0(int w);

protected:
	virtual void    _Put(int w);
	virtual void    _Put(const void *data, dword size);
	virtual int64   GetSize() const { return filesize; }

public:
	virtual   bool  IsOpen() const;
	void            Create(const char *path, bool append);
	void            SetLimit(int maxsize)                    { sizelimit = maxsize; }

	void            SetOptions(dword _options)               { options = _options; }

	bool            Delete();
	void            Close();

	LogStream();
	~LogStream();
};

void     ActivateUsrLog();
void     ActivatePersistentUsrLog();
bool     IsUsrLog();
Stream&  UsrLog();
Stream&  UsrLog(const char *line);
Stream&  UsrLogT(const char *line);
Stream&  UsrLogT(int indent, const char *line);
void     DeleteUsrLog();

Stream&  BugLog();
void     DeactivateBugLog();

Stream&  StdLog();
enum LogOptions {
	LOG_FILE = 1, LOG_COUT = 2, LOG_CERR = 4, LOG_DBG = 8, LOG_TIMESTAMP = 16
};
void     StdLogSetup(dword options);

Stream&  VppLog();
void     SetVppLog(Stream& log);
void     SetVppLogName(const String& file);
void     SetVppLogSizeLimit(int filesize);
void     SetVppLogNoDeleteOnStartup();
void     CloseStdLog();

void     HexDump(Stream& s, const void *ptr, int size, int maxsize = INT_MAX);

void     LogHex(const String& s);
void     LogHex(const WString& s);

String        GetTypeName(const char *type_name);
inline String GetTypeName(const ::std::type_info& tinfo)   { return GetTypeName(tinfo.name()); }

void __LOGF__(const char *format, ...);

#ifdef _MULTITHREADED
void LockLog();
void UnlockLog();
#else
inline void LockLog() {}
inline void UnlockLog() {}
#endif

#define STATIC_ASSERT( expr ) { struct __static_assert { unsigned static_assert:(expr); }; }

#ifdef _DEBUG

#define _DBG_

#define DEBUGCODE(x)     x

#define LOG(a)           UPP::LockLog(), UPP::VppLog() << a << UPP::EOL, UPP::UnlockLog()
#define LOGF             UPP::__LOGF__
#define LOGBEGIN()       UPP::LockLog(), UPP::VppLog() << UPP::LOG_BEGIN
#define LOGEND()         UPP::VppLog() << UPP::LOG_END, UPP::UnlockLog()
#define LOGBLOCK(n)      RLOGBLOCK(n)
#define LOGHEXDUMP(s, a) UPP::HexDump(VppLog(), s, a)
#define LOGHEX(x)        UPP::LockLog(), UPP::LogHex(x), UPP::UnlockLog()
#define QUOTE(a)         { LOG(#a); a; }
#define LOGSRCPOS()      UPP::LockLog(), UPP::VppLog() << __FILE__ << '#' << __LINE__ << UPP::EOL, UPP::UnlockLog()
#define DUMP(a)          UPP::LockLog(), UPP::VppLog() << #a << " = " << (a) << UPP::EOL, UPP::UnlockLog()
#define DUMPC(c)         UPP::LockLog(), UPP::DumpContainer(VppLog() << #c << ':' << UPP::EOL, (c)), UPP::UnlockLog()
#define DUMPCC(c)        UPP::LockLog(), UPP::DumpContainer2(VppLog() << #c << ':' << UPP::EOL, (c)), UPP::UnlockLog()
#define DUMPCCC(c)       UPP::LockLog(), UPP::DumpContainer3(VppLog() << #c << ':' << UPP::EOL, (c)), UPP::UnlockLog()
#define DUMPM(c)         UPP::LockLog(), UPP::DumpMap(VppLog() << #c << ':' << UPP::EOL, (c)), UPP::UnlockLog()
#define DUMPHEX(x)       UPP::LockLog(), UPP::VppLog() << #x << " = "; UPP::LogHex(x), UPP::UnlockLog()
#define XASSERT(c, d)    if(!bool(c)) { LOG("XASSERT failed"); LOGSRCPOS(); LOG(d); ASSERT(0); } else
#define NEVER()          ASSERT(0)
#define NEVER_(msg)      ASSERT_(0, msg)
#define XNEVER(d)        if(1) { LOG("NEVER failed"); LOGSRCPOS(); LOG(d); ASSERT(0); } else
#define CHECK(c)         if(!bool(c)) { ASSERT(0); } else
#define XCHECK(c, d)     if(!bool(c)) { LOG("XCHECK failed"); LOGSRCPOS(); LOG(d); ASSERT(0); } else

#define TIMING(x)        RTIMING(x)
#define HITCOUNT(x)      RHITCOUNT(x)
#define ACTIVATE_TIMING()    UPP::TimingInspector::Activate(true);
#define DEACTIVATE_TIMING()  UPP::TimingInspector::Activate(false);

#define DLOG(x)          LOG(x)
#define DDUMP(x)         DUMP(x)
#define DDUMPC(x)        DUMPC(x)
#define DDUMPM(x)        DUMPM(x)
#define DTIMING(x)       TIMING(x)
#define DLOGHEX(x)       LOGHEX(x)
#define DDUMPHEX(x)      DUMPHEX(x)

#else

#define DLOG(x)          @ // To clean logs after debugging, this produces error in release mode
#define DDUMP(x)         @
#define DDUMPC(x)        @
#define DDUMPM(x)        @
#define DTIMING(x)       @
#define DLOGHEX(x)       @
#define DDUMPHEX(nx)     @

#define DEBUGCODE(x)     LOG_NOP

inline void LOGF(const char *format, ...) {}

#define LOG(a)           LOG_NOP
#define LOGBEGIN()       LOG_NOP
#define LOGEND()         LOG_NOP
#define LOGBLOCK(n)      LOG_NOP
#define LOGHEXDUMP(s, a) LOG_NOP
#define LOGHEX(x)        LOG_NOP
#define QUOTE(a)         a
#define LOGSRCPOS()      LOG_NOP
#define DUMP(a)          LOG_NOP
#define DUMPC(a)         LOG_NOP
#define DUMPCC(a)        LOG_NOP
#define DUMPCCC(a)       LOG_NOP
#define DUMPM(a)         LOG_NOP
#define DUMPHEX(nx)      LOG_NOP
#define XASSERT(c, d)    LOG_NOP
#define NEVER()          LOG_NOP
#define NEVER_(msg)      LOG_NOP
#define XNEVER(d)        LOG_NOP
#define CHECK(c)         (c)
#define XCHECK(c, d)     (c)

#define TIMING(x)           LOG_NOP
#define HITCOUNT(x)         LOG_NOP
#define ACTIVATE_TIMING()   LOG_NOP
#define DEACTIVATE_TIMING() LOG_NOP

#endif

struct DebugLogBlock
{
	DebugLogBlock(const char *name) : name(name) { VppLog() << name << EOL << LOG_BEGIN; }
	~DebugLogBlock()                             { VppLog() << LOG_END << "//" << name << EOL; }
	const char *name;
};

#define RLOG(a)           UPP::LockLog(), UPP::VppLog() << a << UPP::EOL, UPP::UnlockLog()
#define RLOGBEGIN()       UPP::LockLog(), UPP::VppLog() << LOG_BEGIN
#define RLOGEND()         UPP::VppLog() << LOG_END, UPP::UnlockLog()
#define RLOGBLOCK(n)      UPP::DebugLogBlock MK__s(n)
#define RLOGHEXDUMP(s, a) UPP::HexDump(UPP::VppLog(), s, a)
#define RQUOTE(a)         { LOG(#a); a; }
#define RLOGSRCPOS()      UPP::LockLog(), UPP::VppLog() << __FILE__ << '#' << __LINE__ << UPP::EOL
#define RDUMP(a)          UPP::LockLog(), UPP::VppLog() << #a << " = " << (a) << UPP::EOL, UPP::UnlockLog()
#define RDUMPC(c)         UPP::LockLog(), UPP::DumpContainer(UPP::VppLog() << #c << ':' << UPP::EOL, (c)), UPP::UnlockLog()
#define RDUMPM(c)         UPP::LockLog(), UPP::DumpMap(VppLog() << #c << ':' << UPP::EOL, (c)), UPP::UnlockLog()
#define RLOGHEX(x)        UPP::LockLog(), UPP::LogHex(x), UPP::UnlockLog()
#define RDUMPHEX(x)       UPP::LockLog(), UPP::VppLog() << #x << " = ", UPP::LogHex(x), UPP::UnlockLog()

// Crash support

void InstallCrashDump(const char *app_info = NULL);
void SetCrashFileName(const char *cfile);

// Dumping templates

template <class T>
void DumpContainer(Stream& s, T ptr, T end) {
	int i = 0;
	s << LOG_BEGIN;
	while(ptr != end)
		s << '[' << i++ << "] = " << *ptr++ << EOL;
	s << LOG_END;
}

template <class C>
void DumpContainer(Stream& s, const C& c) {
	DumpContainer(s, c.Begin(), c.End());
}

template <class T>
void DumpContainer2(Stream& s, T ptr, T end) {
	int i = 0;
	s << LOG_BEGIN;
	while(ptr != end) {
		s << '[' << i++ << "] =" << EOL;
		DumpContainer(s, (*ptr).Begin(), (*ptr).End());
		ptr++;
	}
	s << LOG_END;
}

template <class C>
void DumpContainer2(Stream& s, const C& c) {
	DumpContainer2(s, c.Begin(), c.End());
}

template <class T>
void DumpContainer3(Stream& s, T ptr, T end) {
	int i = 0;
	s << LOG_BEGIN;
	while(ptr != end) {
		s << '[' << i++ << "] =" << EOL;
		DumpContainer2(s, (*ptr).Begin(), (*ptr).End());
		ptr++;
	}
	s << LOG_END;
}

template <class C>
void DumpContainer3(Stream& s, const C& c) {
	DumpContainer3(s, c.Begin(), c.End());
}

template <class T>
void DumpMap(Stream& s, const T& t) {
	s << LOG_BEGIN;
	for(int i = 0; i < t.GetCount(); i++)
		s << '[' << i << "] = ("<< t.GetKey(i) << ") " << t[i] << EOL;
	s << LOG_END;
}

String AsString(const MemoryProfile& mem);
