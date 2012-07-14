#include "Core.h"

namespace Upp {

VectorMap<String, String> LoadIniStream(Stream &sin) {
	Stream *in = &sin;
	FileIn fin;
	VectorMap<String, String> key;
	int c;
	if((c = in->Get()) < 0) return key;
	for(;;) {
		String k, v;
		for(;;) {
			if(IsAlNum(c) || c == '_')
				k.Cat(c);
			else
				break;
			if((c = in->Get()) < 0) return key;
		}
		for(;;) {
			if(c != '=' && c != ' ') break;
			if((c = in->Get()) < 0) return key;
		}
		for(;;) {
			if(c < ' ') break;
			v.Cat(c);
			if((c = in->Get()) < 0) break;
		}
		if(!k.IsEmpty())
			key.Add(k, v);
		if(k == "LINK") {
			if(in == &fin)
				fin.Close();
			if(!fin.Open(v) || (c = in->Get()) < 0) return key;
			in = &fin;
		}
		else
			for(;;) {
				if(IsAlNum(c) || c == '_') break;
				if((c = in->Get()) < 0) return key;
			}
	}
}

VectorMap<String, String> LoadIniFile(const char *filename) {
	FileIn in(filename);
	if(!in) return VectorMap<String, String>();
	return LoadIniStream(in);
}

static StaticMutex sMtx;
static char  sIniFile[256];
static int64 s_ini_version = 1;

void ReloadIniFile()
{
	Mutex::Lock __(sMtx);
	s_ini_version++;
}

void SetIniFile(const char *name) {
	Mutex::Lock __(sMtx);
	strcpy(sIniFile, name);
	ReloadIniFile();
}

static
void sIniSet(int64& version)
{
	version = s_ini_version;
}


static
bool sIniChanged(int64& version)
{
	if(version != s_ini_version) {
		version = s_ini_version;
		return true;
	}
	return false;
}

String GetIniKey(const char *id, const String& def) {
	Mutex::Lock __(sMtx);
	static VectorMap<String, String> key;
	static int64 version;
	if(sIniChanged(version)) {
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

IniString::operator String()
{
	String x;
	{
		Mutex::Lock __(sMtx);
		String& s = (*ref_fn)();
		if(sIniChanged(version)) {
			s = TrimBoth(GetIniKey(id));
			if(IsNull(s))
				s = (*def)();
		}
		x = s;
	}
	return x;
}

String IniString::operator=(const String& s)
{
	Mutex::Lock __(sMtx);
	(*ref_fn)() = s;
	sIniSet(version);
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

IniInt::operator int() {
	Mutex::Lock __(sMtx);
	if(sIniChanged(version)) {
		value = (int)ReadIniInt(id);
		if(IsNull(value))
			value = (*def)();
	}
	return value;
}

int IniInt::operator=(int b) {
	Mutex::Lock __(sMtx);
	sIniSet(version);
	return value = b;
}

String IniInt::ToString() const
{
	return AsString((int)const_cast<IniInt&>(*this));
}

IniInt64::operator int64()
{
	Mutex::Lock __(sMtx);
	if(sIniChanged(version)) {
		value = ReadIniInt(id);
		if(IsNull(value))
			value = (*def)();
	}
	return value;
}

int64 IniInt64::operator=(int64 b)
{
	Mutex::Lock __(sMtx);
	sIniSet(version);
	return value = b;
}

String IniInt64::ToString() const
{
	return AsString((int64)const_cast<IniInt64&>(*this));
}

IniDouble::operator double()
{
	Mutex::Lock __(sMtx);
	if(sIniChanged(version)) {
		value = ScanDouble(TrimBoth(ToLower(GetIniKey(id))));
		if(IsNull(value))
			value = (*def)();
	}
	return value;
}

double IniDouble::operator=(double b)
{
	Mutex::Lock __(sMtx);
	sIniSet(version);
	return value = b;
}

String IniDouble::ToString() const
{
	return AsString((double)const_cast<IniDouble&>(*this));
}

IniBool::operator bool() {
	Mutex::Lock __(sMtx);
	if(sIniChanged(version)) {
		String h = TrimBoth(ToLower(GetIniKey(id)));
		if(h.GetCount())
			value = h == "1" || h == "yes" || h == "true" || h == "y";
		else
			value = (*def)();
	}
	return value;
}

bool IniBool::operator=(bool b) {
	Mutex::Lock __(sMtx);
	sIniSet(version);
	return value = b;
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