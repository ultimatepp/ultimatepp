#include "Core.h"

namespace Upp {

static void LoadIniStream(Stream &sin, VectorMap<String, String>& ret, const char *sfile);

static void LoadIniFile(const char *filename, VectorMap<String, String>& ret)
{
	FileIn in(filename);
	if(in) LoadIniStream(in, ret, filename);
}

static void LoadIniStream(Stream& in, VectorMap<String, String>& key, const char *sfile)
{
	while(!in.IsEof()) {
		String line = in.GetLine();
		CParser p(line);
		if(p.IsId()) {
			String k = p.ReadId();
			if(p.Char('='))
				key.Add(k, p.GetPtr());
		}
		else
		if(p.Char('@')) {
			if(p.Id("include")) {
				String fn = p.GetPtr();
				if(!IsFullPath(fn) && sfile)
					fn = AppendFileName(GetFileFolder(GetFullPath(sfile)), fn);
				LoadIniFile(fn, key);
			}
			else
			if(p.Id("end"))
				return;
		}
	}
}

VectorMap<String, String> LoadIniStream(Stream &sin)
{
    VectorMap<String, String> ret;
    LoadIniStream(sin, ret, NULL);
    return ret;
}

VectorMap<String, String> LoadIniFile(const char *filename)
{
    VectorMap<String, String> ret;
    LoadIniFile(filename, ret);
    return ret;
}

static StaticMutex sMtx;
static char  sIniFile[256];

int ini_version__ = 1;

void ReloadIniFile()
{
	Mutex::Lock __(sMtx);
	ini_version__++;
}

void SetIniFile(const char *name) {
	Mutex::Lock __(sMtx);
	strcpy(sIniFile, name);
	ReloadIniFile();
}

void IniSet__(int& version)
{
	BarrierWrite(version, ini_version__);
}

#ifdef flagSO
bool IniChanged__(int version)
{
	return version != ReadWithBarrier(ini_version__);
}
#endif

String GetIniKey(const char *id, const String& def) {
	ASSERT_(IsMainRunning(), "GetIniKey is allowed only after APP_MAIN has started");
	Mutex::Lock __(sMtx);
	static VectorMap<String, String> key;
	static int version;
	if(version != ini_version__) {
		version = ini_version__;
		key = LoadIniFile(*sIniFile ? sIniFile : ~ConfigFile("q.ini"));
	#ifdef PLATFORM_WIN32
		if(key.GetCount() == 0)
			key = LoadIniFile(~GetExeDirFile("q.ini"));
		if(key.GetCount() == 0)
			key = LoadIniFile("c:\\q.ini");
	#endif
	#ifdef PLATFORM_POSIX
		if(key.GetCount() == 0)
			key = LoadIniFile(GetHomeDirFile("q.ini"));
	#endif
	}
	return key.Get(id, def);
}

String GetIniKey(const char *id)
{
	return GetIniKey(id, String());
}

static StaticMutex strMtx;

IniString::operator String()
{
	String x;
	{
		Mutex::Lock __(strMtx);
		String& s = (*ref_fn)();
		if(IniChanged__(version)) {
			s = TrimBoth(GetIniKey(id));
			if(IsNull(s))
				s = (*def)();
		}
		x = s;
		IniSet__(version);
	}
	return x;
}

String IniString::operator=(const String& s)
{
	Mutex::Lock __(strMtx);
	(*ref_fn)() = s;
	IniSet__(version);
	return s;
}

String IniString::ToString() const
{
	return (String)const_cast<IniString&>(*this);
}

int64 ReadIniInt(const char *id)
{
	String s = GetIniKey(id);
	CParser p(s);
	int64 num;
	int sgn = 1;
	if(p.Char('-'))
		sgn = -1;
	else
		p.Char('+');
	if(p.Char2('0', 'x') || p.Char2('0', 'X'))
		num = p.ReadNumber64(16);
	else
	if(p.IsNumber())
		num = p.ReadNumber();
	else
		return Null;
	num = sgn * num;
	if(p.Char('K'))
		num <<= 10;
	else
	if(p.Char('M'))
		num <<= 20;
	else
	if(p.Char('G'))
		num <<= 30;
	else
	if(p.Char('T'))
		num <<= 40;
	return num;
}

int IniInt::Load() {
	Mutex::Lock __(sMtx);
	if(IniChanged__(version)) {
		int64 v = ReadIniInt(id);
		value = IsNull(v) ? (*def)() : (int)v;
		IniSet__(version);
	}
	return value;
}

int IniInt::operator=(int b) {
	Mutex::Lock __(sMtx);
	BarrierWrite(version, -1);
	value = b;
	IniSet__(version);
	return b;
}

String IniInt::ToString() const
{
	return AsString((int)const_cast<IniInt&>(*this));
}

static StaticMutex si64Mtx;

IniInt64::operator int64()
{
	Mutex::Lock __(si64Mtx);
	if(IniChanged__(version)) {
		value = ReadIniInt(id);
		if(IsNull(value))
			value = (*def)();
		IniSet__(version);
	}
	return value;
}

int64 IniInt64::operator=(int64 b)
{
	Mutex::Lock __(si64Mtx);
	value = b;
	return b;
}

String IniInt64::ToString() const
{
	return AsString((int64)const_cast<IniInt64&>(*this));
}

double IniDouble::Load()
{
	Mutex::Lock __(sMtx);
	if(IniChanged__(version)) {
		value = ScanDouble(TrimBoth(ToLower(GetIniKey(id))));
		if(IsNull(value))
			value = (*def)();
		IniSet__(version);
	}
	return value;
}

double IniDouble::operator=(double b)
{
	Mutex::Lock __(sMtx);
	BarrierWrite(version, -1);
	value = b;
	IniSet__(version);
	return b;
}

String IniDouble::ToString() const
{
	return AsString((double)const_cast<IniDouble&>(*this));
}

bool IniBool::Load() {
	Mutex::Lock __(sMtx);
	if(IniChanged__(version)) {
		String h = TrimBoth(ToLower(GetIniKey(id)));
		if(h.GetCount())
			value = h == "1" || h == "yes" || h == "true" || h == "y";
		else
			value = (*def)();
		IniSet__(version);
	}
	return value;
}

bool IniBool::operator=(bool b) {
	Mutex::Lock __(sMtx);
	BarrierWrite(version, -1);
	value = b;
	IniSet__(version);
	return b;
}

String IniBool::ToString() const
{
	return AsString((bool)const_cast<IniBool&>(*this));
}

Array<IniInfo>& sIniInfo()
{
	static Array<IniInfo> s;
	return s;
}

void AddIniInfo(const char *id, String (*current)(), String (*def)(), const char *info)
{
	IniInfo& f = sIniInfo().Add();
	f.id = id;
	f.current = current;
	f.def = def;
	f.info = info;
}

const Array<IniInfo> GetIniInfo()
{
	return sIniInfo();
}

String GetIniInfoFormatted()
{
	String r;
	for(int i = 0; i < sIniInfo().GetCount(); i++) {
		IniInfo& f = sIniInfo()[i];
		r << f.id << " = " << (*f.current)() << " [default: " << (*f.def)() << "]\r\n"
		  << "       " << f.info << "\r\n";
	}
	return r;
}

void TextSettings::Load(const char *filename)
{
	FileIn in(filename);
	int themei = 0;
	settings.Add("");
	while(!in.IsEof()) {
		String ln = in.GetLine();
		const char *s = ln;
		if(*s == '[') {
			s++;
			String theme;
			while(*s && *s != ']')
				theme.Cat(*s++);
			themei = settings.FindAdd(theme);
		}
		else {
			if(themei >= 0) {
				String key;
				while(*s && *s != '=') {
					key.Cat(*s++);
				}
				if(*s == '=') s++;
				String value;
				while(*s) {
					value.Cat(*s++);
				}
				if(!IsEmpty(key))
					settings[themei].GetAdd(TrimBoth(key)) = TrimBoth(value);
			}
		}
	}
}

String TextSettings::Get(const char *group, const char *key) const
{
	int itemi = settings.Find(group);
	return itemi < 0 ? Null : settings.Get(group).Get(key, Null);
}

String TextSettings::Get(int groupIndex, const char *key) const
{
	return groupIndex >= 0 && groupIndex < settings.GetCount() ?
	              settings[groupIndex].Get(key, Null) : Null;
}

String TextSettings::Get(int groupIndex, int keyIndex) const
{
	if (groupIndex >= 0 && groupIndex < settings.GetCount())
		return keyIndex >= 0 && keyIndex < settings[groupIndex].GetCount() ?
		          settings[groupIndex][keyIndex] : Null;
	else
		return Null;
}

};