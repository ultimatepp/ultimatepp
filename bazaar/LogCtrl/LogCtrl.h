#ifndef _LogCtrl_LogCtrl_h
#define _LogCtrl_LogCtrl_h

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

enum LOGLEV
{
	LNONE = 0,
	LMUTE = LNONE,
	
	LEMRG = 1,
	LALRT = 2,
	LCRIT = 3,
	LERR  = 4,
	LWARN = 5,
	LNOTE = 6,
	LINFO = LNOTE,
	LDBG  = 7,
};

inline int LogLev() { return Single<LOGLEV>(); }
inline void LogLev(int l) { Single<LOGLEV>() = (LOGLEV)l;}

#define ONLL(l) if(LogLev()>=l)
#define LL(l,x) ONLL(l) RLOG(x)

#define EMRG(x) LL(LEMRG, x)
#define ALRT(x) LL(LALRT, x)
#define CRIT(x) LL(LCRIT, x)
#define ERR(x)  LL(LERR,  x)
#define WARN(x) LL(LWARN, x)
#define NOTE(x) LL(LNOTE, x)
#define INFO(x) LL(LINFO, x)
#define DBG(x)  LL(LDBG,  x)

#define L1(x) LL(1, x)
#define L2(x) LL(2, x)
#define L3(x) LL(3, x)
#define L4(x) LL(4, x)
#define L5(x) LL(5, x)
#define L6(x) LL(6, x)
#define L7(x) LL(7, x)

///

class LoggerCtrl : public DocEdit, public StringStream
{
public:
	typedef LoggerCtrl CLASSNAME;
	LoggerCtrl();
	
	void Save();
	void Ignore(bool b = true) { ignore = b; }
	bool IsIgnore() const { return ignore; }

	void MaxLines(int c = 1000) { maxlines = c; Update(); }
	int GetMaxLines() const { return maxlines; }
	virtual void Updated();

	virtual void Flush();

protected:
	virtual  void  _Put(int w);
	virtual  void  _Put(const void *data, dword size);

	int maxlines;
	bool ignore;
#if 0	
	DocEdit::Style st;
#endif
};

class LogCtrl : public LoggerCtrl
{
public:
	typedef LogCtrl CLASSNAME;
	LogCtrl();

	void Log(bool b = true);
	bool IsLogging() const { return ps == this && !IsIgnore(); }
	void Reset();

protected:
	Stream* ps;
};

#endif

