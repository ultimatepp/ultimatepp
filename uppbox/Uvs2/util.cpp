#include "Uvs2.h"
#pragma hdrstop

#include <plugin/z/z.h>

#ifdef _DEBUG
//#define SLOWTRANSFER 2
#endif

bool SaveFileBackup(String fn, String data, bool keep_backup)
{
	if(LoadFile(fn) == data)
		return true;
	String tfn = fn + ".$old";
	FileDelete(tfn);
	FileMove(fn, tfn);
	bool res = ::SaveFile(fn, data);
	if(!res)
		FileDelete(fn);
	else if(!keep_backup)
		FileDelete(tfn);
	return res;
}

void SplitPath(String path, String& dir, String& file)
{
	dir = Null;
	file = path;
	const char *b = path.Begin(), *e = path.End();
	while(e > b && e[-1] != '/')
		e--;
	if(int pos = e - b)
	{
		dir = path.Left(pos - 1);
		file = path.Mid(pos);
	}
}

bool SaveFileTime(const char *filename, String data, Time time)
{
	return SaveFile(filename, data) && FileSetTime(filename, time);
}

String NoCr(String in) {
	String out;
	out.Reserve(in.GetLength());
	const char *b = in.Begin(), *e = in.End();
	while(e > b && (byte)e[-1] <= ' ')
		e--;
	for(const char *s = b; *s && s < e;)
	{
		const char *l = s;
		while(*s && *s != '\n')
			s++;
		const char *le = s;
		while(le > l && (byte)le[-1] <= ' ')
			le--;
		out.Cat(l, le - l);
		if(*s == '\n')
		{
			s++;
			out.Cat('\n');
		}
	}
	return out;
}

String DoCr(String in, bool usecr) {
	String out;
	for(const char *s = in; s < in.End(); s++) {
		if(*s == '\n' && usecr) out.Cat('\r');
		if(*s != '\r') out.Cat(*s);
	}
	return out;
}

int MatchLen(const Vector<String>& a, int ai, const Vector<String>& b, int bi)
{
	int n;
	for(n = 0; n + ai < a.GetCount() && n + bi < b.GetCount(); n++)
		if(a[ai + n] != b[bi + n])
			break;
	return n;
}

String Diff(String newfile, String oldfile) {
	String diff;
	Index<String> ni = Split(NoCr(newfile), '\n', false);
	Vector<String> oi = Split(NoCr(oldfile), '\n', false);
	int i = 0;
	while(i < oi.GetCount()) {
		int li = ni.Find(oi[i]);
		if(li < 0)
			diff << '+' << oi[i++] << '\n';
		else {
			int ll = li;
			int ln = 1;
			while(li >= 0) {
				int n = MatchLen(oi, i, ni.GetKeys(), li);
				if(n > ln) {
					ln = n;
					ll = li;
				}
				li = ni.FindNext(li);
			}
			diff << '-' << ll;
			if(ln > 1)
				diff << ',' << ln;
			diff << '\n';
			i += ln;
		}
	}
	return diff;
}

String PrevVersion(String newfile, const char *diff, int *errln0) {
	if(errln0) *errln0 = -1;
	String oldfile;
	Vector<String> nl;
	const char *p = newfile;
	do
	{
		const char *b = p;
		while(*p && *p != '\n')
			p++;
		const char *e = p;
		while(e > b && (byte)e[-1] <= ' ')
			e--;
		nl.Add(String(b, e));
	}
	while(*p++);
	p = diff;
	int lineno = 0;
	while(*p && *p != '#')
	{
		const char *b = p;
		while(*p && *p != '\n')
			p++;
		String line(b, p);
		if(*p)
			p++;
		const char *lp = line;
		if(*lp == '+')
			oldfile << (lp + 1) << '\n';
		else if(*lp == '-') {
			int i = stou(lp + 1, &lp);
			int n = 1;
			if(*lp++ == ',')
				n = stou(lp, &lp);
			if(i < 0 || n <= 0)
			{
				if(errln0) *errln0 = lineno;
				return String::GetVoid();
			}
			while(n-- > 0)
			{
				if(i < 0 || i >= nl.GetCount())
				{
					if(errln0) *errln0 = lineno;
					return String::GetVoid();
				}
				oldfile << nl[i++] << '\n';
//				if(i < nl.GetCount())
//					oldfile << '\n';
			}
		}
		else {
			if(errln0) *errln0 = lineno;
			return String::GetVoid();
		}
		lineno++;
	}
	if(!oldfile.IsEmpty() && *oldfile.Last() == '\n')
		oldfile.Trim(oldfile.GetLength() - 1);
	return oldfile;
}

String PrevVersion(String newfile, const char *verfile, int verindex, int *errln0)
{
	if(errln0) *errln0 = -1;
	int line = 1;
	if(verindex <= 0)
		return newfile;
	String prevfile = newfile;
	const char *p = verfile;
	while(*p)
	{
		const char *b = p;
		int vline = line;
		while(*p && *p != '#')
		{
			while(*p && *p++ != '\n')
				;
			line++;
		}
		int verr;
		String oldfile = PrevVersion(prevfile, String(b, p), &verr);
		if(verr >= 0)
		{
			if(errln0) *errln0 = vline + verr;
			return prevfile;
		}
		prevfile = oldfile;
		while(*p && *p++ != '\n')
			;
		line++;
	}
	return prevfile;
}

String AddPrevVersion(String rep, String ver, String add, String text)
{
	const char *b = ver, *p = b;
	while(*p && *p != '#')
		while(*p && *p++ != '\n')
			;
	String prev = PrevVersion(rep, String(b, p));
	String out;
	out.Cat(Diff(rep, add));
	for(b = p; *p && *p++ != '\n';)
		;
	out.Cat(b, p - b);
	out.Cat(Diff(add, prev));
	out.Cat(text);
	out.Cat(p, ver.End() - p);
	return out;
}

bool ParseVer(const char *p, int& filetime, String& author)
{
	try
	{
		if(*p++ != '#' || !IsDigit(*p))
			return false;
		CParser parser(p);
		filetime = parser.ReadInt();
		author = parser.ReadOneString();
		return !IsNull(filetime);
	}
	catch(Exc e)
	{
		return false;
	}
}

DirInfo ReadDir(String dir) throw (Exc) {
	CParser p(dir);
	DirInfo result;
	while(!p.IsEof()) {
		FileInfo& f = result.Add(p.ReadOneString());
		f.filetime = p.IsInt() ? p.ReadInt() : ToUvsTime(GetSysTime());
		f.author = p.ReadOneString();
		if(p.Id("deleted"))
			f.deleted = true;
		p.Char(';');
	}
	return result;
}

String AsString(const DirInfo& info) {
	String result;
	for(int i = 0; i < info.GetCount(); i++) {
		const FileInfo& f = info[i];
		result << AsCString(info.GetKey(i)) << ' '
			   << f.filetime << ' ' << AsCString(f.author);
		if(f.deleted)
			result << " deleted";
		result << ";\n";
	}
	return result;
}

FileVerInfo ReadVer(String verfile, const FileInfo& current, String curdata)
{
	FileVerInfo vinfo;
	FileVersion& curver = vinfo.Add();
	curver.filetime = current.filetime;
	curver.author = current.author;
	curver.data = curdata;
	String verdata = curdata;
	const char *p = verfile;
	while(*p)
	{
		const char *b = p;
		while(*p && *p != '#')
			while(*p && *p++ != '\n')
				;
		if(*p == '#')
		{
			verdata = PrevVersion(verdata, String(b, p));
			int ft;
			String auth;
			if(!ParseVer(p, ft, auth))
			{
				FileVersion& prevver = vinfo.Add();
				prevver.filetime = ft;
				prevver.author = auth;
				prevver.data = verdata;
			}
		}
		while(*p && *p++ != '\n')
			;
	}
	return vinfo;
}

String AsString(const FileVerInfo& verinfo)
{
	if(verinfo.IsEmpty())
		return Null;
	String ver;
	for(int i = 1; i < verinfo.GetCount(); i++)
		if(verinfo[i - 1].author != verinfo[i].author || verinfo[i - 1].filetime != verinfo[i].filetime
		|| verinfo[i - 1].data != verinfo[i].data)
			ver << Diff(verinfo[i - 1].data, verinfo[i].data)
			<< '#' << verinfo[i].filetime << ' ' << AsCString(verinfo[i].author) << '\n';
	return ver;
}

bool MergeVer(FileVerInfo& dest, const FileVerInfo& src)
{
	bool modf = false;
	for(int i = 0; i < src.GetCount(); i++)
	{
		const FileVersion& sver = src[i];
		int p = 0;
		while(p < dest.GetCount() && dest[p].filetime > sver.filetime)
			p++;
		if(p >= dest.GetCount() || sver.filetime != dest[p].filetime
		|| sver.author != dest[p].author || sver.data != dest[p].data)
		{
			dest.Insert(p, sver);
			modf = true;
		}
	}
	return modf;
}

ComplexFileInfo::ComplexFileInfo()
: tree_time(Null), tree_size(Null), host_size(Null),
//	prev_loaded(false),
	treeconflict(false), timeconflict(false), ignorehost(false), noconflict(false)
{
}

String ComplexFileInfo::GetSortKey(int sort_method, bool is_download, int64 sort_size) const
{
	String key;
	String dir, file, ext;
	SplitPath(path, dir, file);
	dir = ToUpper(dir);
	file = ToUpper(file);
	ext = GetFileExt(file);
	switch(sort_method)
	{
	case SORT_ACTION: key << (IsTreeConflict() ? "A\n" : IsTreeEdit() ? "C\n" : is_download ? "B\n" : "D\n");
	case SORT_PATH: key << dir << '\n' << file; break;
	case SORT_NAME: key << file << '\n' << dir; break;
	case SORT_EXT:  key << ext << '\n' << dir << '\n' << file; break;
	case SORT_TIME: key << FormatIntDec(INT_MAX - tree_time, 10, '0') << '\n' << dir << '\n' << file; break;
	case SORT_SIZE:
		if(!IsNull(sort_size))
		{
			key = Format("%.0f", 1e15 - sort_size);
			if(key.GetLength() < 20)
				key = String('0', 20 - key.GetLength()) + key;
		}
		key << '\n' << dir << '\n' << file; break;
	default: NEVER(); break;
	}
	return key;
}

bool ComplexFileInfo::IsTreeNew() const
{
	return !IsNull(tree_time)
		&& (IsNull(local.filetime) || local.deleted)
		&& (IsNull(download.filetime) || download.deleted);
//		(prev_loaded ? IsNull(prev.filetime) || prev.deleted : IsNull(local.filetime) || local.deleted);
}

bool ComplexFileInfo::IsTreeDeleted() const
{
	return IsNull(tree_time)
		&& !IsNull(local.filetime) && !local.deleted
		&& (IsNull(download.filetime) || !download.deleted);
//		(prev_loaded ? !IsNull(prev.filetime) && !prev.deleted : !IsNull(local.filetime) && !local.deleted);
}

bool ComplexFileInfo::IsTreeEdit() const
{
	return IsTreeDeleted() || IsTreeNew() ||
		!IsNull(tree_time) && !IsNull(local.filetime) && !local.deleted && !IsSameTime(local.filetime, tree_time);
}

bool ComplexFileInfo::IsHostNew() const
{
	const FileInfo& base = Base();
	return !IsNull(download.filetime) && !ignorehost && !download.deleted
		&& (IsNull(base.filetime) || base.deleted);
}

bool ComplexFileInfo::IsHostDeleted() const
{
	return !IsNull(download.filetime) && !ignorehost && download.deleted;
}

bool ComplexFileInfo::IsHostEdit() const
{
	return !IsNull(download.filetime) && !ignorehost;
}

bool ComplexFileInfo::IsUpload() const
{
	return !IsTreeConflict() && IsTreeEdit();
//		|| !IsNull(tree_time) && !IsNull(download.filetime) && !download.deleted
//			&& !IsSameTime(tree_time, download.filetime));
}

static const char conflict_text[] = "$uvs: PENDING CONFLICT";
enum
{
	CONFLICT_LEN = sizeof(conflict_text) - 1,
	PROGRESS_TOTAL = 1000,
};

void SetTree(ComplexDirInfo& cfi, String physdir, String logdir,
	Gate2<int, int> progress, int offset, int total)
{
	progress(offset, PROGRESS_TOTAL);
	Vector<String> files, folders;
	Vector<int64> lengths;
	Vector<int> times;

	{
		FindFile ff;
		if(ff.Search(AppendFileName(physdir, "*")))
			do
				if(ff.IsFolder())
					folders.Add(ff.GetName());
				else if(ff.IsFile())
				{
					files.Add(ff.GetName());
					lengths.Add(ff.GetLength());
					times.Add(ToUvsTime(ff.GetLastWriteTime()));
				}
			while(ff.Next());
	}

	int i;
	for(i = 0; i < files.GetCount(); i++) {
		String rpath;
		if(!IsNull(logdir))
			rpath << logdir << '/';
		rpath << files[i];
		String ixrpath = ToLower(rpath);
		int f = cfi.Find(ixrpath);
		int ltime = times[i];
		if(f < 0) {
			f = cfi.GetCount();
			cfi.Add(ixrpath);
		}
		cfi[f].path = rpath;
		cfi[f].tree_time = ltime;
		cfi[f].tree_size = lengths[i];
		bool conflict = false;
		if(lengths[i] > CONFLICT_LEN) {
			FileIn fi;
			fi.SetBufferSize(CONFLICT_LEN);
			if(fi.Open(AppendFileName(physdir, files[i]))) {
				char buffer[CONFLICT_LEN];
				conflict = (fi.GetAll(buffer, CONFLICT_LEN) && !memcmp(buffer, conflict_text, CONFLICT_LEN));
			}
		}
		cfi[f].treeconflict = conflict;
	}
	for(i = 0; i < folders.GetCount(); i++) {
		int pb = offset + iscale(total, i, folders.GetCount());
		int pe = offset + iscale(total, i + 1, folders.GetCount());
		String rpath;
		if(!IsNull(logdir))
			rpath << logdir << '/';
		rpath << folders[i];
		SetTree(cfi, AppendFileName(physdir, folders[i]), rpath, progress, pb, pe - pb);
	}
	progress(offset + total, PROGRESS_TOTAL);
}

void SetTree(ComplexDirInfo& cfi, String dir, Gate2<int, int> progress)
{
	SetTree(cfi, dir, Null, progress, 0, PROGRESS_TOTAL);
	progress(PROGRESS_TOTAL, PROGRESS_TOTAL);
}

void SetLocal(ComplexDirInfo& cfi, const DirInfo& localinfo)
{
	for(int i = 0; i < localinfo.GetCount(); i++)
	{
		String rpath = localinfo.GetKey(i), ixrpath = ToLower(rpath);
		int f = cfi.Find(ixrpath);
		if(f < 0)
		{
			f = cfi.GetCount();
			cfi.Add(ixrpath);
			cfi[f].path = rpath;
		}
		cfi[f].local = localinfo[i];
	}
}

void SetHost(ComplexDirInfo& cfi, const DirInfo& download)
{
	for(int i = 0; i < download.GetCount(); i++)
	{
		String rpath = download.GetKey(i), ixrpath = ToLower(rpath);
		int f = cfi.Find(ixrpath);
		if(f < 0)
		{
			f = cfi.GetCount();
			cfi.Add(ixrpath);
			cfi[f].path = rpath;
		}
		cfi[f].download = download[i];
		cfi[f].ignorehost = false;
	}
}

/*
void SetPrev(ComplexDirInfo& cfi, const DirInfo& prev)
{
	if(!prev.IsEmpty())
	{
		int i;
		for(i = 0; i < prev.GetCount(); i++)
		{
			String rpath = prev.GetKey(i), ixrpath = ToLower(rpath);
			int f = cfi.Find(ixrpath);
			if(f < 0)
			{
				f = cfi.GetCount();
				cfi.Add(ixrpath);
				cfi[f].path = rpath;
			}
			cfi[f].prev = prev[i];
		}
		for(i = 0; i < cfi.GetCount(); i++)
			cfi[i].prev_loaded = true;
	}
}
*/

int ToUvsTime(Time time) {
	if(IsNull(time))
		return Null;
	int uvstime = (int)(time - Time(1990, 1, 1, 0, 0, 0));
	double diff = (double)(FromUvsTime(uvstime) - time);
	ASSERT(diff == 0);
	return uvstime;
/*
	SYSTEMTIME tm, tb;
	Zero(tm); Zero(tb);
	tm.wYear   = time.year;   tb.wYear   = 1989;
	tm.wMonth  = time.month;  tb.wMonth  = 12;
	tm.wDay    = time.day;    tb.wDay    = 31;
	tm.wHour   = time.hour;   tb.wHour   = 22;
	tm.wMinute = time.minute;
	tm.wSecond = time.second;
	FileTime / *ftl, * /ftg;
	SystemTimeToFileTime(&tm, &ftg);
//	LocalFileTimeToFileTime(&ftl, &ftg);
	FileTime / *fbl,* / fbg;
	SystemTimeToFileTime(&tb, &fbg);
//	LocalFileTimeToFileTime(&fbl, &fbg);
#ifdef PLATFORM_WIN32
	__int64 tg = ftg.dwLowDateTime + (1i64 << 32) * ftg.dwHighDateTime;
	__int64 bg = fbg.dwLowDateTime + (1i64 << 32) * fbg.dwHighDateTime;
	int uvstime = (int)((tg - bg) / 10000000);
#else
	int uvstime = (int)(tg - bg);
#endif
#ifdef _DEBUG
	double diff = FromUvsTime(uvstime) - time;
//	ASSERT(diff == 0);
#endif
	return uvstime;
*/
}

Time FromUvsTime(int time) {
	if(IsNull(time))
		return Null;
	return Time(1990, 1, 1, 0, 0, 0) + time;
/*
	SYSTEMTIME tb;
	Zero(tb);
	tb.wYear   = 1990;
	tb.wMonth  = 1;
	tb.wDay    = 1;
	tb.wHour   = 0;
	FileTime fbg/ *, fbl* /;
	SystemTimeToFileTime(&tb, &fbg);
//	LocalFileTimeToFileTime(&fbl, &fbg);
#ifdef PLATFORM_WIN32
	__int64 bg = fbg.dwLowDateTime + (1i64 << 32) * fbg.dwHighDateTime;
	bg += 10000000i64 * time;
	FileTime ftg, ftl;
	ftg.dwLowDateTime = (dword)bg;
	ftg.dwHighDateTime = (dword)(bg >> 32);
//	FileTimeToLocalFileTime(&ftg, &ftl);
	FileTimeToSystemTime(&ftg, &tb);
	Time tm(tb.wYear, tb.wMonth, tb.wDay, tb.wHour, tb.wMinute, tb.wSecond);
#else
	ftl = fbg + time;
	Time tm(ftl);
#endif
//	RDUMP(ToUvsTime(tm) - time);
	return tm;
*/
}

/*
int UvsLocalTimeDelta(int uvstime)
{
	FileTime a, b;
	a.dwLowDateTime = a.dwHighDateTime = 0;
	b.dwLowDateTime = b.dwHighDateTime = 0;
	FileTimeToLocalFileTime(&b, &b);
	__int64 diff = __int64(a.dwLowDateTime) - __int64(b.dwLowDateTime) + (1i64 << 32) * (a.dwHighDateTime - b.dwHighDateTime);
	return (int)(diff / 10000000);
}
*/

/*
String FormatUvsTime(int uvstime)
{
	if(IsNull(uvstime)) return Null;
	return Format(FromUvsTime(uvstime) + UvsLocalTimeDelta());
}
*/

struct NextArea {
	int ni;
	int oi;
};

int MatchLenNN(const Vector<String>& a, int ai, const Vector<String>& b, int bi) {
	int n = MatchLen(a, ai, b, bi);
	int nn = 0;
	for(int i = 0; i < n; i++)
		if(!a[ai + i].IsEmpty()) nn++;
	return nn;
}

NextArea FindNextArea(const Index<String>& ol, int oi, const Vector<String>& nl, int ni) {
	NextArea na;
	na.ni = nl.GetCount();
	na.oi = ol.GetCount();
	for(int i = ni; i < nl.GetCount(); i++) {
		int ii = ol.Find(nl[i]);
		while(ii >= 0 && ii < oi)
			ii = ol.FindNext(ii);
		while(ii >= 0) {
			if(MatchLenNN(ol.GetKeys(), ii, nl, i) > 1) {
				na.ni = i;
				na.oi = ii;
				return na;
			}
			ii = ol.FindNext(ii);
		}
	}
	return na;
}

bool TestDiff(const Index<String>& ol, int& oi,
			  const Vector<String>& al, int& ai, const char *av,
			  const Vector<String>& bl, int& bi, const char *bv,
			  String& out) {
	if(ai < al.GetCount() && oi < ol.GetCount() && al[ai] != ol[oi]) {
		NextArea na = FindNextArea(ol, oi, al, ai);
		if(na.ni > ai) {
			out << "$uvs: " << av << " INSERT\n";
			while(ai < na.ni)
				out << al[ai++] << '\n';
			out << "$uvs: END " << av << " INSERT\n";
		}
		if(oi < na.oi) {
			out << "$uvs: " << av << " DELETE\n";
			while(oi < ol.GetCount() && oi < na.oi) {
				if(bi < bl.GetCount() && oi < ol.GetCount() && bl[bi] != ol[oi]) {
					NextArea na = FindNextArea(ol, oi, bl, bi);
					if(na.ni > bi) {
						out << "$uvs: " << bv << " INSERT (" << av << " DELETE)\n";
						while(bi < na.ni)
							out << bl[bi++] << '\n';
						out << "$uvs: END " << bv << " INSERT (" << av << " DELETE)\n";
					}
					if(oi < na.oi) {
						out << "$uvs: " << bv << " DELETE (" << av << " DELETE)\n";
						while(oi < ol.GetCount() && oi < na.oi)
							out << ol[oi++] << '\n';
						out << "$uvs: END " << bv << " DELETE (" << av << " DELETE)\n";
					}
				}
				else {
					out << ol[oi++] << '\n';
					bi++;
				}
			}
			out << "$uvs: END " << av << " DELETE\n";
		}
		return false;
	}
	return true;
}

String ConflictDiff(String org, String my, String rep) {
	Index<String> ol = Split(NoCr(org), '\n', false);
	Vector<String> ml = Split(NoCr(my), '\n', false);
	Vector<String> rl = Split(NoCr(rep), '\n', false);
	int oi = 0;
	int mi = 0;
	int ri = 0;
	String out;
	out << "$uvs: PENDING CONFLICT\n";
	while(oi < ol.GetCount()) {
		if(TestDiff(ol, oi, ml, mi, "YOUR", rl, ri, "REPOSITORY", out) &&
		   TestDiff(ol, oi, rl, ri, "REPOSITORY", ml, mi, "YOUR", out)) {
			out << ol[oi++] << '\n';
			mi++;
			ri++;
		}
	}
	if(mi < ml.GetCount()) {
		out << "$uvs: YOUR INSERT\n";
		while(mi < ml.GetCount())
			out << ml[mi++] << '\n';
		out << "$uvs: END YOUR INSERT\n";
	}
	if(ri < rl.GetCount()) {
		out << "$uvs: REPOSITORY INSERT\n";
		while(ri < rl.GetCount())
			out << rl[ri++] << '\n';
		out << "$uvs: END REPOSITORY INSERT\n";
	}
	return out;
}

/*
bool Ftp::Connect(const char *host, const char *user, const char *password) {
	Close();
	hinternet = InternetOpen(GetExeTitle(),
		                     INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!hinternet) return false;
	hftp = InternetConnect(hinternet, host, INTERNET_DEFAULT_FTP_PORT, user, password,
		                   INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if(!hftp) {
		Close();
		return false;
	}
	return true;
}
*/

/*
void Ftp::Close() {
	if(hftp) InternetCloseHandle(hftp);
	if(hinternet) InternetCloseHandle(hinternet);
	hinternet = hftp = NULL;
}
*/

/*
Ftp::~Ftp() {
	Close();
}
*/

/*
String Ftp::Load(const char *path, Gate1<String> progress) {
	HINTERNET hrl = FtpOpenFile(hftp, path, GENERIC_READ,
		FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD | INTERNET_FLAG_RESYNCHRONIZE
		| INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NEED_FILE, 0);
	if(hrl == NULL)
		return String::GetVoid();
	String result;
	int p = 0;
	for(;;)
	{
		byte buffer[1024];
		dword n;
		if(!InternetReadFile(hrl, buffer, 1024, &n)) {
			result = String::GetVoid();
			break;
		}
		if(n == 0) {
			result.Shrink();
			break;
		}
		result.Cat(buffer, n);
#ifdef SLOWTRANSFER
		int end = GetTickCount() + SLOWTRANSFER;
		for(int d; (d = end - GetTickCount()) > 0; Sleep(d))
			;
#endif
		if(progress(result))
			break;
	}
	InternetCloseHandle(hrl);
	return result;
}
*/

/*
bool Ftp::Save(const char *path, String data, Gate2<int, int> progress) {
	HINTERNET hrl = FtpOpenFile(hftp, path, GENERIC_WRITE, FTP_TRANSFER_TYPE_BINARY, 0);
	if(hrl == NULL)
		return false;
	const byte *p = data;
	int sz = data.GetLength();
	while(sz > 0) {
		dword w;
		int chunk = min(sz, 1024);
		InternetWriteFile(hrl, p, chunk, &w);
		if(chunk != w || progress(p - data, data.GetLength())) {
			InternetCloseHandle(hrl);
			return false;
		}
		sz -= chunk;
		p += chunk;
#ifdef SLOWTRANSFER
		int end = GetTickCount() + SLOWTRANSFER;
		for(int d; (d = end - GetTickCount()) > 0; Sleep(d))
			;
#endif
	}
	InternetCloseHandle(hrl);
	return true;
}
*/

/*
bool  Ftp::Exists(const char *path) {
	String nn = String(path) + ".$x$";
	if(Rename(path, nn)) {
		Rename(nn, path);
		return true;
	}
	return false;
}
*/

/*
bool Ftp::Rename(const char *oldpath, const char *newpath) {
	return FtpRenameFile(hftp, oldpath, newpath);
}
*/

/*
bool Ftp::Cd(const char *path) {
	return FtpSetCurrentDirectory(hftp, path);
}
*/

/*
bool Ftp::MkDir(const char *path) {
	return FtpCreateDirectory(hftp, path);
}
*/

/*
bool Ftp::Delete(const char *path) {
	return FtpDeleteFile(hftp, path);
}
*/

/*
String Ftp::GetError() {
	if(GetLastError() != ERROR_INTERNET_EXTENDED_ERROR)
		return GetLastErrorMessage();
	char buffer[2048];
	dword w = 2048;
	dword e;
	InternetGetLastResponseInfo(&e, buffer, &w);
	return String(buffer, w);
}
*/

/*
void Ftp::ThrowError() throw (Exc) {
	throw Exc(GetError());
}
*/

/*
void Ftp::RealizePath(const char *path) {
	const char *s = path;
	if(*s == '\0') return;
	for(;;) {
		s = strchr(s + 1, '/');
		if(!s) break;
		MkDir(String(path, s));
	}
}
*/

//////////////////////////////////////////////////////////////////////
// UvsDiffEntry::

bool UvsDiffEntry::AddLine(Array<UvsDiffEntry>& list, String line)
{
	const char *p = line, *e = line.End();
	switch(*p++)
	{
	case '+':
		if(list.IsEmpty() || list.Top().count)
			list.Add();
		list.Top().insert.Add(String(p, e));
		return true;

	case '-':
		{
			int index = stou(p, &p);
			int count = 1;
			if(*p == ',')
				count = stou(p + 1);
			if(count > 0 && index >= 0)
			{
				UvsDiffEntry& e = list.Add();
				e.index = index;
				e.count = count;
			}
		}
		return true;

	default:
		return false;
	}
}

void UvsDiffEntry::AddStream(Array<UvsDiffEntry>& list,	Stream& stream)
{
	for(int t; (t = stream.Term()) == '+' || t == '-'; AddLine(list, stream.GetLine()))
		;
}

void UvsDiffEntry::Order(Array<UvsDiffEntry>& list)
{
	bool err = false;
	int ok = 0;
	for(int i = 0; !err && i < list.GetCount(); i++)
		if(list[i].count > 0)
		{
			if(list[i].index < ok)
			{
				err = true;
				break;
			}
			ok = list[i].index + list[i].count;
		}

	if(err)
		Order(list, 0, list.GetCount(), 0, INT_MAX);
}

int UvsDiffEntry::Order(Array<UvsDiffEntry>& list, int blist, int elist, int bline, int eline)
{
	int delta = 0;
	while(elist - blist >= 2)
	{
		int i, b = -1, blen = 0;
		for(i = blist; i < elist; i++)
		{
			UvsDiffEntry& e = list[i];
			if(e.count > 0)
			{
				int within = min(e.index + e.count, eline) - max(e.index, bline);
				if(within <= 0)
					e.count = -e.count;
				else if(e.count > blen)
				{
					b = i;
					blen = e.count;
				}
			}
		}
		if(blen == 0)
			break;
		UvsDiffEntry& e = list[b];
		if(e.index < bline)
		{ // split before
			UvsDiffEntry& before = list.Insert(b++);
			elist++;
			delta++;
			before.count = bline - (before.index = e.index);
			e.count = -(e.index + e.count - bline);
			e.index = bline;
		}
		if(e.index + e.count > eline)
		{ // split after
			UvsDiffEntry& after = list.Insert(b + 1);
			elist++;
			delta++;
			after.count = -(e.index + e.count - eline);
			after.index = eline;
			e.count = eline - e.index;
		}
		if(2 * b >= blist + elist)
		{ // 2nd half - recurse for end, loop for begin
			delta += Order(list, b + 1, elist, e.index + e.count, eline);
			elist = b;
			eline = e.index;
		}
		else
		{ // 1st half - recurse for begin, loop for end
			int shift = Order(list, blist, b, bline, e.index);
			blist = b + 1 + shift;
			elist += shift;
			bline = e.index + e.count;
			delta += shift;
		}
	}
	return delta;
}

//////////////////////////////////////////////////////////////////////
// UvsDiffBlock::

UvsDiffBlock::UvsDiffBlock()
{
	newest_version = Null;
}

void UvsDiffBlock::SetInserts(Array<UvsDiffEntry>& elist, const Array<UvsDiffBlock>& file, int version_index)
{
	Vector<int> index;
	index.Add(0);
	for(int i = 0; i < elist.GetCount(); i++)
		if(elist[i].count < 0)
		{
			UvsDiffEntry& e = elist[i];
			while(e.index >= index.Top() && index.GetCount() <= file.GetCount())
			{
				const UvsDiffBlock& blk = file[index.GetCount() - 1];
				int n = index.Top();
				if(blk.newest_version <= version_index
				&& blk.newest_version + blk.line_index.GetCount() > version_index)
					n += blk.lines.GetCount();
				index.Add(n);
			}
			int p = BinFindIndex(index, e.index, StdLess<int>());
			if(p < index.GetCount() && index[p] > e.index)
				p--;
			int lind = e.index, lend = e.index + tabs(e.count);
			e.count = 0;
			for(; p < file.GetCount(); p++)
			{
				const UvsDiffBlock& blk = file[p];
				bool full = (blk.newest_version <= version_index
					&& blk.newest_version + blk.line_index.GetCount() > version_index);
				if(index.GetCount() <= p)
					index.Add(index.Top() + (full ? blk.lines.GetCount() : 0));
				if(index[p] > lind)
					break;
				if(full)
				{
					int pind = lind - index[p];
					int pcnt = min(blk.lines.GetCount() - pind, lend - lind);
					if(pcnt > 0)
					{
						e.insert.Append(blk.lines, pind, pcnt);
						lind += pcnt;
					}
				}
			}
		}
}

void UvsDiffBlock::AddStream(Array<UvsDiffBlock>& file, Stream& stream, int version_index)
{
	if(stream.IsEof())
		return;
	UvsDiffBlock& block = file.Add();
	block.newest_version = version_index;
	block.line_index.Add(0);
	do
		block.lines.Add(stream.GetLine());
	while(!stream.IsEof());
}

void UvsDiffBlock::AddVersion(Array<UvsDiffBlock>& blocks, Stream& stream, int version_index)
{
	Array<UvsDiffEntry> elist;
	UvsDiffEntry::AddStream(elist, stream);
	UvsDiffEntry::Order(elist);
	int old_ver = version_index - 1;
	SetInserts(elist, blocks, old_ver);
	int bline = 0;
	int bitem = 0;
	int oindex = 0;
	for(int i = 0; i < elist.GetCount();)
	{
		Vector<String> insert;
		while(i < elist.GetCount() && elist[i].count <= 0)
			insert.Append(elist[i++].insert);
		int close = 1000000000;
		int copy = 0;
		if(i < elist.GetCount())
		{
			close = elist[i].index;
			if(insert.IsEmpty())
			{
				copy = elist[i].count;
				i++;
			}
		}
		for(; bitem < blocks.GetCount(); bitem++)
		{
			UvsDiffBlock *blk = &blocks[bitem];
			if(blk -> newest_version > old_ver
			|| blk -> newest_version + blk -> line_index.GetCount() <= old_ver)
				continue;
			int avail = (bline + blk -> lines.GetCount() - close);
			if(avail <= 0)
			{ // close all
				oindex += blk -> lines.GetCount();
				continue;
			}
			if(avail < blk -> lines.GetCount())
			{ // split block before
				int delta = close - bline;
				Split(blocks, bitem, delta);
				oindex += delta;
				bline = close;
				blk = &blocks[++bitem];
				avail = blk -> lines.GetCount();
			}
			if(!copy)
				break;
			if(copy < avail)
			{ // split block after
				Split(blocks, bitem, copy);
				avail = copy;
				blk = &blocks[bitem];
			}
			if(avail)
			{
				blk -> line_index.Add(oindex);
				oindex += avail;
				close = (bline += avail);
				copy -= avail;
			}
		}
		if(!insert.IsEmpty())
		{
			UvsDiffBlock& ins = blocks.Insert(bitem++);
			ins.lines = insert;
			insert.Clear();
			ins.newest_version = version_index;
			ins.line_index.Add(oindex);
		}
	}
}

void UvsDiffBlock::Split(Array<UvsDiffBlock>& blocks, int index, int count_lines)
{
	ASSERT(index >= 0 && index < blocks.GetCount());
	UvsDiffBlock& oblk = blocks[index];
	UvsDiffBlock& nblk = blocks.Insert(index);
	count_lines = min(count_lines, oblk.lines.GetCount());
	nblk.lines.Append(oblk.lines, 0, count_lines);
	nblk.newest_version = oblk.newest_version;
	nblk.line_index <<= oblk.line_index;
	for(int i = 0; i < oblk.line_index.GetCount(); i++)
		oblk.line_index[i] += count_lines;
	oblk.lines.Remove(0, count_lines);
}

void UvsDiffBlock::Merge(Array<UvsDiffBlock>& file)
{
	for(int i = file.GetCount(); --i > 0;)
	{
		UvsDiffBlock& prev = file[i - 1];
		UvsDiffBlock& curr = file[i];
		if(curr.newest_version != prev.newest_version
		   || curr.line_index.GetCount() != prev.line_index.GetCount())
		   continue;
/*		int t;
		for(t = curr.line_index.GetCount(); --t >= 0;)
			if(curr.line_index[t] != prev.line_index[t] + prev.lines.GetCount())
				break;
*/
//		if(t < 0)
		{ // may coalesce items
			prev.lines.Append(curr.lines);
			file.Remove(i);
		}
	}
}

static const char block_key[] = "UVSBLOCK";

enum
{
	COMPRESS_VERSION = 2,
	BLOCK_KEY_LENGTH = 8,
	MAX_BLOCK_COUNT  = 100000,
};

static void CatIL(String& s, int value)
{
	char temp[4];
	Poke32le(temp, value);
	s.Cat(temp, 4);
}

void UvsDataBlock::Cleanup()
{
	if(!IsNull(temp_filename))
		FileDelete(temp_filename);
}

String UvsDataBlock::Compress(Gate2<int, int> progress) const
{
	String out;
	out.Cat(block_key, BLOCK_KEY_LENGTH);
	CatIL(out, COMPRESS_VERSION);
	CatIL(out, data.GetCount());
	String dirs = AsString(dir);
	String downs = AsString(download);
	CatIL(out, dirs.GetLength());
	CatIL(out, GetCRC(dirs));
	CatIL(out, downs.GetLength());
	CatIL(out, GetCRC(downs));
	String all_data;
	bool is_ver = !version.IsEmpty();
	for(int i = 0; i < data.GetCount(); i++)
	{
		CatIL(out, data[i].GetLength());
		CatIL(out, GetCRC(data[i]));
		CatIL(out, is_ver ? version[i].GetLength() : -1);
		CatIL(out, is_ver ? GetCRC(version[i]) : 0);
		all_data.Cat(data[i]);
		if(is_ver)
			all_data.Cat(version[i]);
	}
	all_data = ZCompress(all_data, progress);
	String zdir = ZCompress(dirs);
	String zdown = ZCompress(downs);
	CatIL(out, zdir.GetLength());
	CatIL(out, zdown.GetLength());
	CatIL(out, all_data.GetLength());
	out.Cat(zdir);
	out.Cat(zdown);
	out.Cat(all_data);
	return out;
}

UvsDataBlock UvsDataBlock::Decompress(String data, bool headers_only, Gate2<int, int> progress)
{
	UvsDataBlock out;
	const char *p = data.Begin(), *e = data.End();
	static const char incomplete[] = "Neúplná nebo poškozená data, ofset ";
	int dirlen = 0, datalen = 0;
	if(e - p < BLOCK_KEY_LENGTH + 6 * 4)
		throw Exc(String().Cat() << incomplete << 0);
	if(memcmp(p, block_key, BLOCK_KEY_LENGTH))
		throw Exc("Neplatný identifikaèní kód datového bloku.");
	p += BLOCK_KEY_LENGTH;
	int version = Peek32le(p);
	if(version <= 0 || version > COMPRESS_VERSION)
		throw Exc(NFormat("Neplatná verze komprimovaných dat: %d", version));
	int down_count = Peek32le(p + 4);
	if(down_count < 0 || down_count >= MAX_BLOCK_COUNT)
		throw Exc(NFormat("Neplatný poèet záznamù v komprimovaném souboru: %d", down_count));
	p += 8;
	int dir_length = -1;
	dword dir_crc = 0;
	if(version >= 2)
	{
		dir_length = Peek32le(p);
		dir_crc = Peek32le(p + 4);
		p += 8;
	}
	int down_length = Peek32le(p);
	dword down_crc = Peek32le(p + 4);
	p += 8;
	int item_bytes = (version <= 1 ? 8 : 16);
	if(e - p < item_bytes * down_count)
		throw Exc(String().Cat() << incomplete << (p - ~data));
	Vector<int> data_length;
	Vector<dword> data_crc;
	Vector<int> version_length;
	Vector<dword> version_crc;
	int total_length = 0;
	data_length.SetCount(down_count);
	data_crc.SetCount(down_count);
	version_length.SetCount(down_count, -1);
	version_crc.SetCount(down_count, 0);
	for(int i = 0; i < down_count; i++)
	{
		total_length += (data_length[i] = Peek32le(p));
		data_crc[i] = Peek32le(p + 4);
		p += 8;
		if(version >= 2)
		{
			total_length += max(0, version_length[i] = Peek32le(p));
			version_crc[i] = Peek32le(p + 4);
			p += 8;
		}
	}
	int zdir_length = -1;
	if(version >= 2)
	{
		zdir_length = Peek32le(p);
		p += 4;
	}
	int zdown_length = Peek32le(p);
	int zdata_length = Peek32le(p + 4);
	p += 8;
	if(zdir_length > e - p)
		throw Exc(NFormat("Neplatná délka komprimovaného adresáøe: %d", zdir_length));
	if(zdown_length < 0 || zdown_length > e - p)
		throw Exc(NFormat("Neplatná délka komprimovaného adresáøe zmìnového bloku: %d", zdown_length));
	if(zdir_length >= 0 && zdir_length + zdown_length > e - p)
		throw Exc("Délka komprimovaných adresáøù pøesahuje konec souboru.");
	if(!headers_only)
	{
		if(zdata_length < 0 || zdata_length > e - p)
			throw Exc(NFormat("Neplatná délka komprimovaných dat: %d", zdata_length));
		if(zdir_length + zdata_length > e - p)
			throw Exc("Délka komprimovaného adresáøe a dat pøesahuje konec souboru.");
	}
	if(zdir_length >= 0)
	{
		String dir = ZDecompress(String(p, zdir_length), progress);
		p += zdir_length;
		if(dir.GetLength() != dir_length)
			throw Exc(NFormat("Délka dekomprimovaného adresáøe (%d) nesouhlasí s hlavièkou (%d)",
				dir.GetLength(), dir_length));
		dword diruncrc = GetCRC(dir);
		if(diruncrc != dir_crc)
			throw Exc(NFormat("CRC dekomprimovaného adresáøe (%s) nesouhlasí s hlavièkou (%s)",
				FormatIntHex(diruncrc, 8), FormatIntHex(dir_crc, 8)));
		try
		{
			out.dir = ReadDir(dir);
		}
		catch(Exc e)
		{
			throw Exc(NFormat("Chyba pøi naèítání dekomprimovaného adresáøe: %s", e));
		}
	}
	String down = ZDecompress(String(p, zdown_length), progress);
	p += zdown_length;
	if(down.GetLength() != down_length)
		throw Exc(NFormat("Délka dekomprimovaného adresáøe (%d) nesouhlasí s hlavièkou (%d)",
			down.GetLength(), down_length));
	dword downuncrc = GetCRC(down);
	if(downuncrc != down_crc)
		throw Exc(NFormat("CRC dekomprimovaného adresáøe (%s) nesouhlasí s hlavièkou (%s)",
			FormatIntHex(downuncrc, 8), FormatIntHex(down_crc, 8)));
	try
	{
		out.download = ReadDir(down);
	}
	catch(Exc e)
	{
		throw Exc(NFormat("Chyba pøi naèítání dekomprimovaného adresáøe: %s", e));
	}
	if(out.download.GetCount() != down_count)
		throw Exc(NFormat("Poèet záznamù dekomprimovaného adresáøe zmìnového bloku (%d) nesouhlasí s hlavièkou (%d)",
			out.download.GetCount(), down_count));
	if(!headers_only && down_count > 0)
	{
		String data = ZDecompress(String(p, zdata_length));
		if(data.GetLength() != total_length)
			throw Exc(NFormat("Délka dekomprimovaného datového bloku (%d) nesouhlasí s celkovou délkou dat z hlavièky (%d)",
				data.GetLength(), total_length));
		out.data.SetCount(down_count);
		out.ignore.SetCount(down_count, false);
		if(version_length[0] >= 0)
			out.version.SetCount(down_count);
		int pos = 0;
		for(int i = 0; i < down_count; i++)
		{
			dword part_crc = GetCRC(out.data[i] = data.Mid(pos, data_length[i]));
			if(part_crc != data_crc[i])
				throw Exc(NFormat("CRC záznamu #%d (%s) neodpovídá záznamu v hlavièce (%s)",
					i, FormatIntHex(part_crc, 8), FormatIntHex(data_crc[i], 8)));
			pos += data_length[i];
			if(version_length[i] >= 0)
			{
				part_crc = GetCRC(out.version[i] = data.Mid(pos, version_length[i]));
				if(part_crc != version_crc[i])
					throw Exc(NFormat("CRC záznamu verze #%d (%s) neodpovídá záznamu v hlavièce (%s)",
						i, FormatIntHex(part_crc, 8), FormatIntHex(version_crc[i], 8)));
				pos += version_length[i];
			}
		}
	}
	return out;
}

int UvsDataBlock::GetLength(String data, bool headers_only)
{
	int l = data.GetLength();
	if(l < BLOCK_KEY_LENGTH + 6 * 4)
		return Null;
	if(memcmp(data, block_key, BLOCK_KEY_LENGTH))
		return 0;
	int version = Peek32le(~data + BLOCK_KEY_LENGTH);
	if(version > COMPRESS_VERSION)
		return 0;
	int hdrlen = BLOCK_KEY_LENGTH + (version <= 1 ? 6 : 9) * 4;
	if(l < hdrlen)
		return 0;
	int block_count = Peek32le(~data + BLOCK_KEY_LENGTH + 4);
	if(block_count < 0 || block_count >= MAX_BLOCK_COUNT)
		return 0;
	hdrlen += (version <= 1 ? 8 : 16) * block_count;
	if(l < hdrlen)
		return Null;
	int xlen = (version >= 2 ? Peek32le(~data + hdrlen - 12) : 0);
	int dlen = Peek32le(~data + hdrlen - 8);
	int blen = Peek32le(~data + hdrlen - 4);
	return hdrlen + xlen + dlen + (headers_only ? 0 : blen);
}

bool Var(CParser& p, const char *key, String& var) throw(Exc) {
	if(p.Id(key)) {
		p.Char('=');
		var = p.ReadString();
		p.Char(';');
		return true;
	}
	return false;
}

void Var(String& out, const char *key, String var) {
	out << key << " = " << AsCString(var) << ";\n";
}

String UvsHostIndex::Compress() const
{
	String out;
	Var(out, "last_update", FormatInt(last_update));
	Var(out, "last_full", FormatInt(last_full));
	return out;
}

UvsHostIndex UvsHostIndex::Decompress(String data)
{
	UvsHostIndex hostindex;
	CParser p(data);
	if(p.IsInt())
	{
		hostindex.last_update = p.ReadInt();
		return hostindex;
	}
	while(!p.IsEof())
	{
		String tmp;
		if(Var(p, "last_update", tmp))
		{
			hostindex.last_update = stou(~tmp);
			if(hostindex.last_update < 0 || hostindex.last_update >= (int)1e9)
				throw Exc(NFormat("Neplatný index poslední zmìny: %d", hostindex.last_update));
		}
		else if(Var(p, "last_full", tmp))
		{
			hostindex.last_full = stou(~tmp);
			if(hostindex.last_full < 0 || hostindex.last_full >= (int)1e9)
				throw Exc(NFormat("Neplatný index poslední plné zmìny: %d", hostindex.last_full));
		}
		else
			throw Exc(NFormat("Neplatný pøíkaz v indexovém souboru: %s", StringSample(p.GetPtr(), 20)));
	}
	return hostindex;
}

bool UvsJob::Read(String s) {
	usecr = false;
	anonymous = false;
	fullblock = false;
	try {
		CParser p(s);
		while(!p.IsEof()) {
			if(p.Id("usecr")) {
				p.Char('=');
				usecr = (bool) p.ReadInt();
				p.Char(';');
				continue;
			}
			if(p.Id("anonymous")) {
				p.Char('=');
				anonymous = (bool) p.ReadInt();
				p.Char(';');
				continue;
			}
			if(p.Id("fullblock")) {
				p.Char('=');
				fullblock = (bool) p.ReadInt();
				p.Char(';');
				continue;
			}
			if(p.Id("passive")) {
				p.Char('=');
				passive = (bool) p.ReadInt();
				p.Char(';');
				continue;
			}
			if(Var(p, "dir", dir)) continue;
			if(Var(p, "excludedirs", excludedirs)) continue;
			if(Var(p, "binaryfiles", binaryfiles)) continue;
			if(Var(p, "excludefiles", excludefiles)) continue;
			if(Var(p, "author", author)) continue;
			if(Var(p, "localrepository", localrepository)) continue;
			String dummy;
			if(Var(p, "messagefile", dummy)) continue;
			if(Var(p, "backup", dummy)) continue;
			if(Var(p, "tree_backup", tree_backup)) continue;
			if(Var(p, "local_backup", local_backup)) continue;
			if(Var(p, "host", host)) continue;
			if(Var(p, "user", user)) continue;
			if(Var(p, "password", password)) continue;
			if(Var(p, "hostrepository", hostrepository)) continue;
			p.ThrowError(NFormat("job parameter expected, found: ", StringSample(p.GetPtr(), 20)));
		}
	}
	catch(Exc e) {
		return false;
	}
	return true;
}

String UvsJob::AsString() const {
	String out;
	Var(out, "dir", dir);
	out << "usecr = " << (usecr ? "1" : "0") << ";\n";
	out << "anonymous = " << (anonymous ? "1" : "0") << ";\n";
	out << "passive = " << (passive ? "1" : "0") << ";\n";
	out << "fullblock = " << (fullblock ? "1" : "0") << ";\n";
	Var(out, "excludedirs", excludedirs);
	Var(out, "binaryfiles", binaryfiles);
	Var(out, "excludefiles", excludefiles);
	Var(out, "author", author);
	Var(out, "localrepository", localrepository);
//	Var(out, "messagefile", messagefile);
//	Var(out, "backup", backup);
	Var(out, "tree_backup", tree_backup);
	Var(out, "local_backup", local_backup);
	Var(out, "host", host);
	Var(out, "user", user);
	Var(out, "password", password);
	Var(out, "hostrepository", hostrepository);
	return out;
}

void UvsJob::Clear() {
	usecr = false;
	anonymous = false;
	fullblock = false;
	passive = true;
	excludedirs.Clear();
	binaryfiles.Clear();
	excludefiles.Clear();
	author.Clear();
	localrepository.Clear();
//	backup.Clear();
	host.Clear();
	user.Clear();
	password.Clear();
	hostrepository.Clear();
}

bool UvsJob::IsExcluded(String dir, String file) const
{
	return (PatternMatchMulti(excludefiles, file) || PatternMatchMulti(excludedirs, dir));
}

bool UvsJob::IsExcluded(String path) const
{
	String dir, name;
	SplitPath(path, dir, name);
	return IsExcluded(dir, name);
}

bool UvsJob::IsBinary(String path) const
{
	return PatternMatchMulti(binaryfiles, GetFileNamePos(path));
}

String UvsJob::NoCr(String data, String filename) const
{
	return IsBinary(filename) ? data : ::NoCr(data);
}

String UvsJob::DoCr(String data, String filename) const
{
	return IsBinary(filename) ? data : ::DoCr(data, usecr);
}

String UvsJob::GetTreePath(String relpath) const
{
	return AppendFileName(NativePath(dir), NativePath(relpath));
}

String UvsJob::GetLocalPath(String relpath) const
{
	return AppendFileName(NativePath(localrepository), NativePath(relpath));
}
