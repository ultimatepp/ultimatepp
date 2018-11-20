class String;

#if defined(PLATFORM_POSIX) || defined(PLATFORM_WINCE)
inline int tmGetTime() {
	return GetTickCount();
}
#else
inline int tmGetTime() {
	return timeGetTime();
}
#endif

class TimingInspector {
protected:
	static bool active;

	const char *name;
	int         call_count;
	dword       total_time;
	dword       min_time;
	dword       max_time;
	int         max_nesting;
	int         all_count;
	StaticMutex mutex;

public:
	TimingInspector(const char *name = NULL); // Not String !!!
	~TimingInspector();

	void   Add(dword time, int nesting);

	String Dump();

	class Routine {
	public:
		Routine(TimingInspector& stat, int& nesting)
		: nesting(nesting), stat(stat) {
			start_time = tmGetTime();
			nesting++;
		}

		~Routine() {
			nesting--;
			stat.Add(start_time, nesting);
		}

	protected:
		dword start_time;
		int& nesting;
		TimingInspector& stat;
	};

	static void Activate(bool b)                    { active = b; }
};

class HitCountInspector
{
public:
	HitCountInspector(const char *name, int hitcount = 0) : name(name), hitcount(hitcount) {}
	~HitCountInspector();

	void Step()              { hitcount++; }
	void Add(int i)          { hitcount += i; }
	void operator ++ ()      { Step(); }
	void operator += (int i) { Add(i); }

private:
	const char *name;
	int         hitcount;
};

#define RTIMING(x) \
	static UPP::TimingInspector COMBINE(sTmStat, __LINE__)(x); \
	static thread_local int COMBINE(sTmStatNesting, __LINE__); \
	UPP::TimingInspector::Routine COMBINE(sTmStatR, __LINE__)(COMBINE(sTmStat, __LINE__), COMBINE(sTmStatNesting, __LINE__))

#define RACTIVATE_TIMING()    TimingInspector::Activate(true);
#define RDEACTIVATE_TIMING()  TimingInspector::Activate(false);

#define RHITCOUNT(n) \
	{ static HitCountInspector hitcount(n); hitcount.Step(); }
