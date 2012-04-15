#include "Web.h"

NAMESPACE_UPP

String FormatIP(dword _ip)
{
	byte ip[4];
	Poke32be(ip, _ip);
	return Format("%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}

static const char hex_digits[] = "0123456789ABCDEF";

String UrlEncode(const String& s, const char *specials)
{
	int l = (int)strlen(specials);
	const char *p = s, *e = s.End();
	String out;
	for(; p < e; p++)
	{
		const char *b = p;
		while(p < e && (byte)*p > ' ' && (byte)*p < 127 && memchr(specials, *p, l) == 0)
			p++;
		if(p > b)
			out.Cat(b, int(p - b));
		if(p >= e)
			break;
		if(*p == ' ')
			out << '+';
		else
			out << '%' << hex_digits[(*p >> 4) & 15] << hex_digits[*p & 15];
	}
	return out;
}


bool IsSameTextFile(const char *p, const char *q)
{
	for(;;)
	{
		if(*p == 0)
		{
			while(*q && (byte)*q <= ' ')
				q++;
			return *q == 0;
		}
		if(*q == 0)
		{
			while(*p && (byte)*p <= ' ')
				p++;
			return *p == 0;
		}
		bool pcr = false;
		if(*p == '\r' || *p == '\n')
		{
			if(p[1] == (*p == '\r' ? '\n' : '\r'))
				p++;
			p++;
			pcr = true;
		}
		bool qcr = false;
		if(*q == '\r' || *q == '\n')
		{
			if(q[1] == (*q == '\r' ? '\n' : '\r'))
				q++;
			q++;
			qcr = true;
		}
		if(pcr || qcr)
		{
			if(pcr != qcr)
				return false;
		}
		else if(*p++ != *q++)
			return false;
	}
}

String StringSample(const char *s, int limit)
{
	const char *e = (const char *)memchr(s, 0, limit + 1);
	if(e)
		return String(s, e);
	return String(s, limit) + "...";
}

static String ToQuotedHtml(const String& s)
{
	String out;
	for(const char *p = s.Begin(), *e = s.End(); p < e; p++)
		switch(*p)
		{
		case 31:   out.Cat("&nbsp;"); break;
		case '<':  out.Cat("&lt;");   break;
		case '>':  out.Cat("&gt;");   break;
		case '&':  out.Cat("&amp;");  break;
		case '\"': out.Cat("&quot;"); break;
		case '\r': break;
		case '\t': out.Cat("&#9;");   break;
		case '\n': out.Cat("&#10;");  break;
		default:   out.Cat(*p);       break;
		}
	return out;
}

String GetRandomIdent(int length)
{
	StringBuffer ident(length);
	char *p = ident;
	for(int i = 0; i < length; i++)
		*p++ = 'A' + rand() % ('Z' - 'A' + 1);
	return ident;
}

static unsigned AddCRC(unsigned old, byte next, unsigned crc)
{
	for(int i = 0; i < 8; i++)
	{
		old = (old << 1) ^ (old & 0x80000000u ? crc : 0) ^ (next & 1);
		next >>= 1;
	}
	return old;
}

enum { CRC = 0x84030625 };

String OtpEncode(const String& password, const String& otp_seed)
{
	if(IsNull(password))
		return Null;
	unsigned crc_sum[4];
	memset(crc_sum, 0, sizeof(crc_sum));
	for(int j = 0; j < otp_seed.GetLength(); j++)
	{
		unsigned sum = crc_sum[j & 3];
		sum = AddCRC(sum, otp_seed[j], CRC);
		for(int i = 0; i < password.GetLength(); i++)
			sum = AddCRC(sum, password[i], CRC);
		crc_sum[j & 3] = sum;
	}
	char h[100];
	sprintf(h, "%08X%08X%08X%08X", crc_sum[0], crc_sum[1], crc_sum[2], crc_sum[3]);
	return h;
}

String EncryptString(const String& password, const String& otp_key)
{
	unsigned crc = 0;
	int i;
	for(i = 0; i < otp_key.GetLength(); i++)
		crc = AddCRC(crc, otp_key[i], CRC);
	int pl = password.GetLength();
	StringBuffer out(pl);
	char *p = out;
	for(i = 0; i < pl; i++)
	{
		*p++ = password[i] ^ crc;
		crc = AddCRC(crc, otp_key[(int)(crc % otp_key.GetLength())], CRC);
	}
	return out;
}

String BinHexEncode(const char *s, const char *e)
{
	static const char bh[] = "0123456789ABCDEF";
	int l = int(e - s);
	StringBuffer out(2 * l);
	char *p = out;
	for(; s < e; s++) {
		*p++ = bh[(*s >> 4) & 0xF];
		*p++ = bh[*s & 0xF];
	}
	return out;
}

String BinhexEncode(const char *s, const char *e)
{
	static const char bh[] = "0123456789abcdef";
	int l = int(e - s);
	StringBuffer out(2 * l);
	char *p = out;
	for(; s < e; s++) {
		*p++ = bh[(*s >> 4) & 0xF];
		*p++ = bh[*s & 0xF];
	}
	return out;
}

String BinHexDecode(const char *p, const char *e)
{
	StringBuffer out;
	if((e - p) & 1)
		e--;
	for(; p < e; p++)
		if(IsXDigit(*p) && IsXDigit(p[1])) {
			out.Cat(ctoi(p[0]) * 16 + ctoi(p[1]));
			p++;
		}
	return out;
}

String ASCII85Encode(const byte *p, int length)
{
	StringBuffer out;
	out.Reserve((length >> 2) * 5 + (length & 3) + 1);
	while(length >= 4)
	{
		dword chunk = (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3];
		if(chunk == 0)
			out.Cat('z');
		else {
			out.Cat(33 + chunk / (85 * 85 * 85 * 85));
			out.Cat(33 + (chunk / (85 * 85 * 85)) % 85);
			out.Cat(33 + (chunk / (85 * 85)) % 85);
			out.Cat(33 + (chunk / 85) % 85);
			out.Cat(33 + chunk % 85);
		}
		p += 4;
		length -= 4;
	}
	if(length)
	{
		dword chunk = *p++ << 24;
		if(length > 1) chunk += *p++ << 16;
		if(length > 2) chunk += *p++ << 8;
		out.Cat(33 + chunk / (85 * 85 * 85 * 85));
		out.Cat(33 + (chunk / (85 * 85 * 85)) % 85);
		if(length > 1)
			out.Cat(33 + (chunk / (85 * 85)) % 85);
		if(length > 2)
			out.Cat(33 + (chunk / 85) % 85);
	}
	return out;
}

String ASCII85Decode(const byte *p, int length)
{
	byte buffer[5];
	int pos = 0;
	String out;
	for(const byte *e = p + length; p < e; p++)
		if(*p >= 33 && *p < 33 + 85)
		{
			buffer[pos++] = *p - 33;
			if(pos >= 5)
			{
				unsigned v = buffer[4] + 85 * (buffer[3] + 85 * (buffer[2] + 85 * (buffer[1] + 85 * buffer[0])));
				out.Cat((byte)(v >> 24));
				out.Cat((byte)(v >> 16));
				out.Cat((byte)(v >> 8));
				out.Cat((byte)v);
				pos = 0;
			}
		}
		else if(*p == 'z')
		{
			out.Cat("\0\0\0", 4);
			pos = 0;
		}
	switch(pos)
	{
	case 2:
		{
			unsigned v = 84 + 84 * 85 + 84 * 85 * 85 + 85 * 85 * 85 * (buffer[1] + 85 * buffer[0]);
			out.Cat((byte)(v >> 24));
		}
		break;

	case 3:
		{
			unsigned v = 84 + 84 * 85 + 85 * 85 * (buffer[2] + 85 * (buffer[1] + 85 * buffer[0]));
			out.Cat((byte)(v >> 24));
			out.Cat((byte)(v >> 16));
		}
		break;

	case 4:
		{
			unsigned v = 84 + 85 * (buffer[3] + 85 * (buffer[2] + 85 * (buffer[1] + 85 * buffer[0])));
			out.Cat((byte)(v >> 24));
			out.Cat((byte)(v >> 16));
			out.Cat((byte)(v >> 8));
		}
		break;
	}
	return out;
}

static const dword *GetCRCTable()
{
	enum { CTL = 0xDAA97221 };
	static dword table[256];
	if(table[1] == 0)
	{
		for(int i = 1; i < __countof(table); i++)
		{
			dword v = i << 24;
			for(int t = 0; t < 8; t++)
				v = (v << 1) ^ (v & (1 << 31) ? CTL : 0);
			table[i] = v;
		}
	}
	return table;
}

dword AddCRC(dword crc, const byte *data, int count)
{
	const dword *t = GetCRCTable();
	for(const byte *e = data + count; data < e; data++)
		crc = ((crc << 8) | (*data & 0xFF)) ^ t[(crc >> 24) & 0xFF];
	return crc;
}

static void GetLineIndex(String file, HashBase& hash, Vector<String>& lines)
{
	const char *p = file;
	while(*p)
	{
		while(*p && *p != '\n' && (byte)*p <= ' ')
			p++;
		const char *b = p;
		while(*p && *p++ != '\n')
			;
		const char *e = p;
		while(e > b && (byte)e[-1] <= ' ')
			e--;
		String s(b, e);
		hash.Add(GetHashValue(s));
		lines.Add(s);
	}
}

int LocateLine(String old_file, int old_line, String new_file)
{
	HashBase old_hash, new_hash;
	Vector<String> old_lines, new_lines;
	GetLineIndex(old_file, old_hash, old_lines);
	GetLineIndex(new_file, new_hash, new_lines);
	if(old_line <= 0)
		return 0;
	if(old_line >= old_lines.GetCount())
		return new_lines.GetCount();
	String line = old_lines[old_line];
	//int hash = old_hash[old_line]; Mirek: unused
	//int fore_count = old_lines.GetCount() - old_line - 1;
	int best_match = 0, best_value = 0;
	for(int r = 0; r < 10 && !best_value; r++)
	{
		int src = (r & 1 ? old_line + (r >> 1) + 1 : old_line - (r >> 1));
		if(src < 0 || src >= old_lines.GetCount())
			continue;
		dword hash = old_hash[src];
		for(int i = new_hash.Find(hash); i >= 0; i = new_hash.FindNext(i))
			if(new_lines[i] == old_lines[src])
			{
				int max_back = min(i, src);
				int max_fore = min(new_lines.GetCount() - i, old_lines.GetCount() - src) - 1;
				if(max_back + max_fore <= best_value)
					continue;
				int back = 1;
				while(back <= max_back && new_hash[i - back] == old_hash[src - back]
					&& new_lines[i - back] == old_lines[src - back])
					back++;
				int fore = 1;
				while(fore < max_fore && new_hash[i + fore] == old_hash[src + fore]
					&& new_lines[i + fore] == old_lines[src + fore])
					fore++;
				if(back + fore > best_value)
				{
					best_value = back + fore;
					best_match = minmax(i, 0, new_lines.GetCount());
				}
			}
	}
	return best_match;
}

void AppVersion(const char *ver)
{
#ifdef PLATFORM_WIN32
	String out = "set RAW_VERSION=";
	out.Cat(ver);
	out.Cat("\r\n"
		"set APP_VERSION=");
	for(; *ver; ver++)
		out.Cat(IsAlNum(*ver) ? *ver : '_');
	out.Cat("\r\n");
	SaveFile("ver.bat", out);
#else
	fputs(ver, stdout);
#endif
}

//////////////////////////////////////////////////////////////////////
// HttpQuery::

RefPtr<HttpQuery::Data> HttpQuery::Empty()
{
	static RefPtr<Data> emp = new Data;
	return emp;
}

void HttpQuery::Serialize(Stream& stream)
{
	int version = StreamHeading(stream, 1, 1, 1, "HttpQuery");
	if(version >= 1)
	{
		if(stream.IsLoading())
			Clear();
		int count = GetCount();
		stream / count;
		for(int i = 0; i < count; i++)
		{
			String key, value;
			if(stream.IsStoring())
			{
				key = data -> map.GetKey(i);
				value = data -> map[i];
			}
			stream % key % value;
			if(stream.IsLoading())
				Set(key, value);
		}
	}
}

HttpQuery& HttpQuery::SetURL(const String& url)
{
	const char *p = url;
	while(*p && *p != '\1' && *p != '?')
		p++;
	p = (*p == '?' ? p + 1 : ~url);

	// const char *e = url.End(); Mirek:unused
	while(*p)
	{
		const char *last = p;
		while(*p && *p != '=' && *p != '&')
			p++;
		String key = case_sensitive ? UrlDecode(last, p) : ToUpper(UrlDecode(last, p));
		if(*p == '=')
			p++;
		last = p;
		while(*p && *p != '&')
			p++;
		Set(key, UrlDecode(last, p));
		if(*p)
			p++;
	}
	return *this;
}

String HttpQuery::GetHidden() const
{
	String out;
	for(int i = 0; i < GetCount(); i++)
		if(!IsInternal(i))
		{
			out << "<INPUT TYPE=HIDDEN NAME=\"" << ToHtml(GetKey(i))
			<< "\" VALUE=\"" << ToQuotedHtml(GetValue(i)) << "\">\n";
		}
	return out;
}

String HttpQuery::GetQuery(bool empty) const
{
	String result;
	for(int i = 0, n = GetCount(); i < n; i++)
		if(!IsInternal(i))
		{
			String key = GetKey(i), value = GetValue(i);
			if(empty || !value.IsEmpty())
			{
				if(!result.IsEmpty()) result.Cat('&');
				result.Cat(UrlEncode(key));
				result.Cat('=');
				result.Cat(UrlEncode(value));
			}
		}
	return result;
}

String HttpQuery::GetQuery(HttpQuery patch, bool empty) const
{
	String result;
	int i, n;
	for(i = 0, n = GetCount(); i < n; i++)
		if(!IsInternal(i))
		{
			String key = GetKey(i), value = GetValue(i);
			if(patch.Find(key) < 0)
			{
				if(empty || !value.IsEmpty())
				{
					if(!result.IsEmpty()) result.Cat('&');
					result.Cat(UrlEncode(key));
					result.Cat('=');
					result.Cat(UrlEncode(value));
				}
			}
		}
	for(i = 0, n = patch.GetCount(); i < n; i++)
		if(!patch.IsInternal(i))
		{
			String key = patch.GetKey(i), value = patch.GetValue(i);
			if(empty || !value.IsEmpty())
			{
				if(!result.IsEmpty()) result.Cat('&');
				result.Cat(UrlEncode(key));
				result.Cat('=');
				result.Cat(UrlEncode(value));
			}
		}

	return result;
}

bool HttpQuery::IsEmpty(String key) const
{
	int f = Find(key);
	return f < 0 || data -> map[f].IsEmpty();
}

bool HttpQuery::IsInternal(int i) const
{
	String key = GetKey(i);
	return key[0] == '$' && key[1] == '$';
}

void HttpQuery::Get(const String& key, Ref result) const
{
	switch(result.GetType())
	{
	case BOOL_V:   result.SetValue(GetBool(key)); break;
	case INT_V:    RefInt(result)    = GetInt(key); break;
	case DOUBLE_V: RefDouble(result) = GetDouble(key); break;
	case STRING_V: RefString(result) = GetString(key); break;
	case DATE_V:   RefDate(result)   = GetDate(key); break;
	case TIME_V:   RefTime(result)   = GetTime(key); break;
	default: NEVER();
	}
}

bool HttpQuery::GetBool(const String& key) const
{
	int i = Find(key);
	return i >= 0 && data -> map[i] != "0";
}

int HttpQuery::GetInt(const String& key) const
{
	int i = Find(key);
	return i < 0 || data->map[i].IsEmpty() ? (int)Null : atoi(data->map[i]);
}

double HttpQuery::GetDouble(const String& key) const
{
	int i = Find(key);
	return i < 0 || data->map[i].IsEmpty() ? (double)Null : atof(data->map[i]);
}

String HttpQuery::GetString(const String& key) const
{
	int i = Find(key);
	return i >= 0 ? data -> map[i] : String::GetVoid();
}

Date HttpQuery::GetDate(const String& key) const
{
	int i = Find(key);
	return i >= 0 ? Date(StdConvertDate().Scan(data -> map[i])) : Date(Null);
}

Time HttpQuery::GetTime(const String& key) const
{
	int i = Find(key);
	return i >= 0 ? Time(StdConvertTime().Scan(data -> map[i])) : Time(Null);
}

Color HttpQuery::GetColor(const String& key) const
{
	String s = GetString(key);
	if(s.GetLength() >= 6)
	{
		int digit[6];
		int i = 0;
		while(i < 6 && (digit[i] = ctoi(s[i])) < 16)
			i++;
		if(i >= 6)
			return Color(digit[0] * 16 + digit[1], digit[2] * 16 + digit[3], digit[4] * 16 + digit[5]);
	}
	return Null;
}

HttpQuery& HttpQuery::SetColor(const String& key, Color c)
{
	if(IsNull(c))
		SetString(key, Null);
	else
		SetString(key, NFormat("%02x%02x%02x", c.GetR(), c.GetG(), c.GetB()));
	return *this;
}

bool HttpQuery::GetBool(const String& key, bool dflt) const
{
	int i = Find(key);
	return i >= 0 ? data -> map[i] != "0" : dflt;
}

int HttpQuery::GetInt(const String& key, int min, int max, int dflt) const
{
	int v = GetInt(key);
	return !IsNull(v) ? minmax(v, min, max) : dflt;
}

double HttpQuery::GetDouble(const String& key, double min, double max, double dflt) const
{
	double v = GetDouble(key);
	return !IsNull(v) ? minmax(v, min, max) : dflt;
}

String HttpQuery::GetString(const String& key, const String& dflt) const
{
	int i = Find(key);
	return i >= 0 && !data -> map[i].IsEmpty() ? data -> map[i] : dflt;
}

HttpQuery& HttpQuery::SetBool(const String& key, bool b)
{
	return Set(key, b ? "1" : "");
}

HttpQuery& HttpQuery::SetInt(const String& key, int i)
{
	return Set(key, IntStr(i));
}

HttpQuery& HttpQuery::SetDouble(const String& key, double f)
{
	return Set(key, DblStr(f));
}

HttpQuery& HttpQuery::Set(const String& key, const String& s)
{
	int i = Find(key);
	if(i < 0 || data -> map[i] != s)
	{
		Clone();
		if(i < 0)
			data -> map.Add(key, s);
		else
			data -> map[i] = s;
	}
	return *this;
}

HttpQuery& HttpQuery::SetDate(const String& key, Date d)
{
	return Set(key, Format(d));
}

HttpQuery& HttpQuery::SetTime(const String& key, Time t)
{
	return Set(key, Format(t));
}

HttpQuery& HttpQuery::SetValue(const String& key, const Value& v)
{
	return Set(key, StdFormat(v));
}

HttpQuery& HttpQuery::Set(HttpQuery query)
{
	if(int n = query.GetCount())
	{
		Clone();
		RefCon<Data> s = query.data;
		for(int i = 0; i < n; i++)
			Set(s -> map.GetKey(i), s -> map[i]);
	}
	return *this;
}

HttpQuery& HttpQuery::Remove(const String& key)
{
	int i = Find(key);
	if(i < 0)
		return *this;
	if(data -> map.GetCount() == 1)
		Clear();
	else
	{
		Clone();
		data -> map.Remove(i);
	}
	return *this;
}

HttpQuery& HttpQuery::Remove(const Vector<String>& keys)
{
	if(!data)
		return *this;
	Index<int> todo;
	for(int i = 0; i < keys.GetCount(); i++)
	{
		int f = data -> map.Find(keys[i]);
		if(f >= 0)
			todo.FindAdd(f);
	}
	if(todo.IsEmpty())
		return *this;
	Clone();
	Vector<int> todo_us = todo.PickKeys();
	Sort(todo_us);
	while(!todo_us.IsEmpty())
		data -> map.Remove(todo_us.Pop());
	return *this;
}

HttpQuery& HttpQuery::Remove(const Vector<Id>& keys)
{
	if(!data)
		return *this;
	Index<int> todo;
	for(int i = 0; i < keys.GetCount(); i++)
	{
		int f = data -> map.Find(~keys[i]);
		if(f >= 0)
			todo.FindAdd(f);
	}
	if(todo.IsEmpty())
		return *this;
	Clone();
	Vector<int> todo_us = todo.PickKeys();
	Sort(todo_us);
	while(!todo_us.IsEmpty())
		data -> map.Remove(todo_us.Pop());
	return *this;
}

String HttpQuery::ToString() const
{
	String out;
	out << "HttpQuery (" << GetCount() << " items)\n";
	for(int i = 0; i < GetCount(); i++)
	{
		out << "[" << i;
		if(IsInternal(i))
			out << ":internal";
		out << "]: <" << GetKey(i) << "> = <";
		String value = GetValue(i);
		if(value.GetLength() <= 1000)
			out << value;
		else
		{
			out.Cat(value, 1000);
			out << " ... (length = " << value.GetLength() << ")";
		}
		out << ">\n";
	}
	return out;
}

END_UPP_NAMESPACE
