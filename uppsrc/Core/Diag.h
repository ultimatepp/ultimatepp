class Exc : public String {
public:
	Exc() {}
	Exc(const String& desc) : String(desc) {}
};

const char LOG_BEGIN = '\x1e';
const char LOG_END = '\x1f';

enum LogOptions {
	LOG_FILE = 1, LOG_COUT = 2, LOG_CERR = 4, LOG_DBG = 8, LOG_SYS = 16,
	LOG_TIMESTAMP = 256, LOG_TIMESTAMP_UTC = 512, LOG_APPEND = 1024, LOG_ROTATE_GZIP = 2048,
};

inline int LOG_ROTATE(int x) { return x << 24; }

void     StdLogSetup(dword options, const char *filepath = NULL,
                     int filesize_limit = 10 * 1024 * 1024);
Stream&  StdLog();

Stream&  UppLog();
void     SetUppLog(Stream& log);

Stream&  VppLog();
void     SetVppLog(Stream& log);

void     SetVppLogName(const String& file);
void     SetVppLogSizeLimit(int limit);

void     HexDumpData(Stream& s, const void *ptr, int size, bool adr, int maxsize);
void     HexDump(Stream& s, const void *ptr, int size, int maxsize = INT_MAX);

void     LogHex(const String& s);
void     LogHex(const WString& s);

String        GetTypeName(const char *type_name);
inline String GetTypeName(const ::std::type_info& tinfo)   { return GetTypeName(tinfo.name()); }

void __LOGF__(const char *format, ...);

#define STATIC_ASSERT( expr ) { struct __static_assert { unsigned __static_assert_tst:(expr); }; }

#ifdef _DEBUG

#define _DBG_

#define DEBUGCODE(x)     x

#define LOG(a)           UPP::VppLog() << a << UPP::EOL
#define LOGF             UPP::__LOGF__
#define LOGBEGIN()       UPP::VppLog() << UPP::LOG_BEGIN
#define LOGEND()         UPP::VppLog() << UPP::LOG_END
#define LOGBLOCK(n)      RLOGBLOCK(n)
#define LOGHEXDUMP(s, a) UPP::HexDump(VppLog(), s, a)
#define LOGHEX(x)        UPP::LogHex(x)
#define QUOTE(a)         { LOG(#a); a; }
#define LOGSRCPOS()      UPP::VppLog() << __FILE__ << '#' << __LINE__ << UPP::EOL
#define DUMP(a)          UPP::VppLog() << #a << " = " << (a) << UPP::EOL
#define DUMPC(c)         UPP::DumpContainer(VppLog() << #c << ':' << UPP::EOL, (c))
#define DUMPCC(c)        UPP::DumpContainer2(VppLog() << #c << ':' << UPP::EOL, (c))
#define DUMPCCC(c)       UPP::DumpContainer3(VppLog() << #c << ':' << UPP::EOL, (c))
#define DUMPM(c)         UPP::DumpMap(VppLog() << #c << ':' << UPP::EOL, (c))
#define DUMPHEX(x)       UPP::VppLog() << #x << " = "; UPP::LogHex(x)
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
#define TIMESTOP(x)      RTIMESTOP(x)

#define DLOG(x)          LOG(x)
#define DDUMP(x)         DUMP(x)
#define DDUMPC(x)        DUMPC(x)
#define DDUMPM(x)        DUMPM(x)
#define DTIMING(x)       TIMING(x)
#define DLOGHEX(x)       LOGHEX(x)
#define DDUMPHEX(x)      DUMPHEX(x)
#define DTIMESTOP(x)     TIMESTOP(x)

#else

#define DLOG(x)          @ // To clean logs after debugging, this produces error in release mode
#define DDUMP(x)         @
#define DDUMPC(x)        @
#define DDUMPM(x)        @
#define DTIMING(x)       @
#define DLOGHEX(x)       @
#define DDUMPHEX(nx)     @
#define DTIMESTOP(x)     @

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
#define CHECK(c)         (void)(c)
#define XCHECK(c, d)     (void)(c)

#define TIMING(x)           LOG_NOP
#define HITCOUNT(x)         LOG_NOP
#define ACTIVATE_TIMING()   LOG_NOP
#define DEACTIVATE_TIMING() LOG_NOP

#define TIMESTOP(x)      LOG_NOP

#endif

struct DebugLogBlock
{
	DebugLogBlock(const char *name) : name(name) { VppLog() << name << EOL << LOG_BEGIN; }
	~DebugLogBlock()                             { VppLog() << LOG_END << "//" << name << EOL; }
	const char *name;
};

#define RLOG(a)           UPP::VppLog() << a << UPP::EOL
#define RLOGBEGIN()       UPP::VppLog() << LOG_BEGIN
#define RLOGEND()         UPP::VppLog() << LOG_END
#define RLOGBLOCK(n)      UPP::DebugLogBlock MK__s(n)
#define RLOGHEXDUMP(s, a) UPP::HexDump(UPP::VppLog(), s, a)
#define RQUOTE(a)         { LOG(#a); a; }
#define RLOGSRCPOS()      UPP::VppLog() << __FILE__ << '#' << __LINE__ << UPP::EOL
#define RDUMP(a)          UPP::VppLog() << #a << " = " << (a) << UPP::EOL
#define RDUMPC(c)         UPP::DumpContainer(UPP::VppLog() << #c << ':' << UPP::EOL, (c))
#define RDUMPM(c)         UPP::DumpMap(VppLog() << #c << ':' << UPP::EOL, (c))
#define RLOGHEX(x)        UPP::LogHex(x)
#define RDUMPHEX(x)       UPP::VppLog() << #x << " = ", UPP::LogHex(x)

// Conditional logging

#define LOG_(flag, x)       do { if(flag) RLOG(x); } while(false)
#define LOGBEGIN_(flag)     do { if(flag) RLOGBEGIN(x); } while(false)
#define LOGEND_(flag)       do { if(flag) RLOGEND(x); } while(false)
#define DUMP_(flag, a)      do { if(flag) RDUMP(x); } while(false)
#define LOGHEX_(flag, x)    do { if(flag) RLOGHEX(x); } while(false)
#define DUMPHEX_(flag, x)   do { if(flag) RDUMPHEX(x); } while(false)
	
// USRLOG


struct IniBool;

namespace Ini {
extern IniBool user_log;
};

#define USRLOG(x)         LOG_(Ini::user_log, x)

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
		s << '[' << i << "] = (" << t.GetKey(i) << ") " << t[i] << EOL;
	s << LOG_END;
}

String AsString(const MemoryProfile& mem);

String CppDemangle(const char* name);

#if 0 // rare cases when release-mode DLOG/DDUMP is needed

#undef  DLOG
#undef  DDUMP
#define DLOG(x)  RLOG(x)
#define DDUMP(x) RDUMP(x)

#endif
