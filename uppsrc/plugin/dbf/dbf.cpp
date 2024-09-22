#include "dbf.h"

namespace Upp {

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

inline int DeleteShareFlag()
{
#ifdef PLATFORM_WIN32
	return IsWinNT() ? FileStream::DELETESHARE : 0;
#else
	return 0;
#endif
}

struct DbfCharset {
	byte dbf_code;
	byte charset;
};

static DbfCharset _dbfCharsets[] = {
//	{ 0x01, CHARSET_UNKNOWN }, // DOS USA, CP437
//	{ 0x02, CHARSET_UNKNOWN }, // DOS multilingual, CP850
	{ 0x03, CHARSET_WIN1252 }, // Windows ANSI, CP1252
//	{ 0x04, CHARSET_UNKNOWN }, // Standard Macintosh
	{ 0x64, CHARSET_CP852   }, // EE MS-DOS, CP852
//	{ 0x65, CHARSET_UNKNOWN }, // Nordic MS-DOS, CP865
//	{ 0x66, CHARSET_UNKNOWN }, // Russian MS-DOS, CP866
//	{ 0x67, CHARSET_UNKNOWN }, // Icelandic MS-DOS
	{ 0x68, CHARSET_MJK     }, // Kamenicky (Czech) MS-DOS
//	{ 0x69, CHARSET_UNKNOWN }, // Mazovia (Polish) MS-DOS
//	{ 0x6A, CHARSET_UNKNOWN }, // Greek MS-DOS (437G)
//	{ 0x6B, CHARSET_UNKNOWN }, // Turkish MS-DOS
//	{ 0x96, CHARSET_UNKNOWN }, // Russian Macintosh
//	{ 0x97, CHARSET_UNKNOWN }, // East European Macintosh
//	{ 0x98, CHARSET_UNKNOWN }, // Greek Macintosh
	{ 0xC8, CHARSET_WIN1250 },
//	{ 0xC9, CHARSET_UNKNOWN }, // Russian Windows
//	{ 0xCA, CHARSET_UNKNOWN }, // Turkish Windows
//	{ 0xCB, CHARSET_UNKNOWN }, // Greek Windows
};

static void sStreamIL(Stream& s, int& x)
{
	if(s.IsLoading())
		x = s.Get32le();
	else
		s.Put32le(x);
}

static void sStreamIW(Stream& s, int& x)
{
	if(s.IsLoading())
		x = s.Get16le();
	else
		s.Put16le(x);
}

static int scan_int(const char *p, const char *e = 0)
{
	LTIMING("scan_int");
	bool neg = false;
	if(p != e && (*p == '+' || *p == '-'))
		neg = (*p++ == '-');
	if(p == e || (byte)(*p - '0') >= 10)
		return Null;
	int val = *p++ - '0';
	while(p != e && (byte)(*p - '0') < 10)
		val = 10 * val + *p++ - '0';
	return neg ? -val : val;
}

static double scan_dbl(const char *p, const char *e = 0)
{
	LTIMING("scan_dbl");
	bool neg = false;
	if(p != e && (*p == '+' || *p == '-'))
		neg = (*p++ == '-');
	if(p == e || (byte)(*p - '0') >= 10)
		return Null;
	double mantissa = 0;
	char c;
	int exp = 0;
	while(p != e && (byte)(*p - '0') < 10)
		if((c = *p++) != '0') {
			if(exp) { mantissa *= ipow10(exp); exp = 0; }
			mantissa = 10 * mantissa + c - '0';
		}
		else
			exp++;
	int raise = exp;
	if(p != e && *p == '.') { // decimal part
		while(++p != e && (byte)((c = *p) - '0') < 10)
			if(c != '0') {
				if(raise) { mantissa *= ipow10(raise); exp -= raise; raise = 0; }
				exp--;
				mantissa = 10 * mantissa + c - '0';
			}
			else
				raise++;
	}
	if(p != e && (*p == 'E' || *p == 'e')) { // exponent
		int vexp = scan_int(p + 1, e);
		if(!IsNull(vexp))
			exp += vexp;
	}
	if(exp) {
		double e = ipow10(tabs(exp));
		mantissa = (exp > 0 ? mantissa * e : mantissa / e);
	}
	return neg ? -mantissa : mantissa;
}

static int StrBool(char c)
{
	if(c == 'A' || c == 'a' || c == 'Y' || c == 'y' || c == 'T' || c == 't' || c == '1')
		return 1;
	if(c == 'N' || c == 'n' || c == 'F' || c == 'f' || c == '0')
		return 0;
	return (int)Null;
}

DbfStream::Field::Field(const char *_name, char type, byte width, byte decimal)
: offset(0), type(type), width(width), decimal(decimal)
{
	if(_name) {
		const char *p = reinterpret_cast<const char *>(memchr(_name, 0, 11));
		name = String(_name, p ? (int)(p - _name) : 11);
	}
	name = ToUpper(name);
}

String DbfStream::Field::Format(Value value, byte charset) const
{
	switch(type) {
		case 'C': {
			if(charset == GetDefaultCharset())
				return StdFormat(value);
			WString s;
			if(value.GetType() == WSTRING_V)
				s = value;
			else
				s = StdFormat(value).ToWString();
			return FromUnicode(s, charset);
		}

		case 'N':
		case 'F': {
			double v = value;
			if(IsNull(v))
				return Null;
			String out;
			if(decimal == 0) {
				if(width < 10)
					out = FormatInt(fround(v));
				else
					out = FormatDoubleFix(v, 0);
				if(out.GetLength() > width) {
					out = (v >= 0 ? "" : "-");
					out.Cat('9', width - out.GetLength());
				}
			}
			else {
				if(v == 0 || ilog10(fabs(v)) + 2 <= width)
					out = FormatDoubleFix(v, decimal);
				if(out.IsEmpty() || out.GetLength() > width)
					out = FormatDoubleExp(v, min<int>(decimal, width - 6));
				ASSERT(out.GetLength() <= width);
			}
			if(out.GetLength() < width)
				out = String(' ', width - out.GetLength()) + out;
			return out;
		}

		case 'D': {
			UPP::Date dt = value;
			if(IsNull(dt))
				return Null;
			char p[9];
			p[8] = 0;
			p[7] = dt.day % 10 + '0';
			p[6] = dt.day / 10 + '0';
			p[5] = dt.month % 10 + '0';
			p[4] = dt.month / 10 + '0';
			int y = dt.year;
			p[3] = y % 10 + '0'; y /= 10;
			p[2] = y % 10 + '0'; y /= 10;
			p[1] = y % 10 + '0';
			p[0] = y / 10 + '0';
			return String(p, 8);
		}

		case '!': {
			StringBuffer temp;
			temp.SetCount(4);
			Poke32le(~temp, (int)value);
			return String(temp);
		}

		case '@': {
			Time t(value);
			int dt = UPP::Date(t) - UPP::Date(-4713, 1, 1);
			int tm = 1000 * (t.second + 60 * (t.minute + 60 * t.hour));
			StringBuffer temp;
			temp.SetCount(8);
			Poke32le(~temp + 0, dt);
			Poke32le(~temp + 4, tm);
			return String(temp);
		}

		case 'O': {
			union {
				char s[8];
				double d;
			};
			d = value;
			StringBuffer temp;
			temp.SetCount(8);
#ifdef CPU_LE
			*(double *)~temp = d;
#else
			temp[0] = s[7];
			temp[1] = s[6];
			temp[2] = s[5];
			temp[3] = s[4];
			temp[4] = s[3];
			temp[5] = s[2];
			temp[6] = s[1];
			temp[7] = s[0];
#endif
			return String(temp);
		}

		case 'L': {
			int cond = value;
			if(IsNull(cond)) return "?";
			return cond ? "T" : "F";
		}

		default:  {
			NEVER();
			return Null;
		}
	}
}

DbfStream::Field DbfStream::Field::Text(const char *name, int width)
{
	ASSERT(width >= 1 && width <= 255);
	return Field(name, 'C', width);
}

DbfStream::Field DbfStream::Field::Number(const char *name, int width, int decimals)
{
	ASSERT(width >= 1 && width <= 31);
	ASSERT(decimals >= 0 && decimals <= width - 1);
	return Field(name, 'N', width, decimals);
}

DbfStream::Field DbfStream::Field::Date(const char *name)
{
	return Field(name, 'D', 8);
}

DbfStream::Field DbfStream::Field::Logical(const char *name)
{
	return Field(name, 'L', 1);
}

int DbfStream::Field::GetType() const
{
	switch(type) {
	case 'N':
	case 'F': return (width <= 9 && decimal == 0 ? INT_V : DOUBLE_V);
	case 'O': return DOUBLE_V;
	case 'M':
	case 'G':
	case 'C': return STRING_V;
	case 'D': return DATE_V;
	case 'L': return INT_V;
	default:  return VOID_V;
	}
}

void DbfStream::Field::Serialize(Stream& stream)
{
	char buffer[12];
	ZeroArray(buffer);
	if(stream.IsStoring())
		memcpy(buffer, name, min(name.GetLength(), 10));
	stream.SerializeRaw((byte *)buffer, 11);
	if(stream.IsLoading()) {
		name = buffer;
		name = ToUpper(name);
	}
	stream % type;
	sStreamIL(stream, offset);
	stream % width % decimal;
	if(stream.IsStoring())
		stream.Put(0, 14);
	else
		stream.SeekCur(14);
}

DbfStream::DbfStream()
{
	version = 0x03;
}

DbfStream::DbfStream(const char *_file, bool write, byte _charset, bool _delete_share)
{
	version = 0x03;
	Open(_file, write, _charset, _delete_share);
}

bool DbfStream::Check(const char *filename, bool write)
{
	return DbfStream().Open(filename, write, CHARSET_DEFAULT);
}

bool DbfStream::Open(const char *file, bool write, byte _charset, bool _delete_share)
{
	Close();
	LLOG("DbfStream::Open(" << file << ", write = " << write << ", charset = " << (int)_charset << ")");
	if(!dbf.Open(file, (write ? FileStream::READWRITE : FileStream::READ)
	| (_delete_share ? DeleteShareFlag() : 0))) {
		LLOG("-> open error");
		return false;
	}
	if(dbt.Open(ForceExt(file, ".dbt"),
		FileStream::READ | (_delete_share ? DeleteShareFlag() : 0))) // writing MEMO's is not supported yet
		dbt.SetLoading();
	if(fpt.Open(ForceExt(file, ".fpt"),
		FileStream::READ | (_delete_share ? DeleteShareFlag() : 0)))
		fpt.SetLoading();
	dbf.SetLoading();
	if(DoOpen(_charset))
		return true;
	LLOG("-> DoOpen error");
	Close();
	return false;
}

bool DbfStream::Create(const char *filename, const Array<Field>& _fields, byte _charset, bool _delete_share)
{
	Close();
	if(!dbf.Open(filename, FileStream::CREATE | (_delete_share ? DeleteShareFlag() : 0)))
		return false;
	if(DoCreate(_fields, _charset))
		return true;
	Close();
	return false;
}

static inline bool IsValidType(char t)
{
	return t == 'C' || t == 'N' || t == 'D' || t == 'L' || t == 'M'
	|| t == 'F' || t == 'O' || t == 'G';
}

bool DbfStream::StreamHeader(bool full)
{
	ASSERT(dbf.IsOpen());
	data_offset = 32 + fields.GetCount() * 32 + 1;
	if(dbf.IsStoring()) {
		ASSERT(!IsReadOnly());
		dbf.Put(version); // #0: version number - 03 without a DBT file
		Date date = GetSysDate();
		dbf.Put(date.year - 1900); // #1 - 3: date of last update
		dbf.Put(date.month);
		dbf.Put(date.day);
	}
	else {
		version = dbf.Get8();
		dbf.Get8();
		dbf.Get16();
	}
	sStreamIL(dbf, rows);
	if(!full)
		return true;
	sStreamIW(dbf, data_offset);
	int row_width = record.GetCount();
	if(dbf.IsStoring() && row_width == 0) { // calculate row width
		row_width = 1;
		for(int i = 0; i < fields.GetCount(); i++)
			row_width += fields[i].width;
	}
	sStreamIW(dbf, row_width);
	if(dbf.IsStoring()) {
		dbf.Put(0, 17);
		int i = __countof(_dbfCharsets);
		while(--i >= 0 && _dbfCharsets[i].charset != charset)
			;
		dbf.Put(i >= 0 ? _dbfCharsets[i].dbf_code : 0);
	//	dbf.PutIL(lang_code);
		dbf.Put16(0);
	}
	else
		dbf.SeekCur(20);
	int field_count = fields.GetCount();
	if(dbf.IsLoading()) {
		if(rows < 0)
			return false;
		if(data_offset <= 32) // || (data_offset & 0x1F) != 1)
			return false; // invalid header size
		if(dbf.GetSize() < (unsigned)data_offset)
			return false; // incomplete file header
		int rec_count = (int)((dbf.GetSize() - data_offset) / row_width);
		rows = min(rows, rec_count); // physical record #
		field_count = (data_offset >> 5) - 1;
		fields.Clear();
		has_memo = false;
	}
	int offset = 1;
	Index<String> used_names;
	for(int i = 0; i < field_count && offset < row_width; i++) {
		Field field;
		if(dbf.IsStoring()) {
			fields[i].offset = offset;
			field = fields[i];
		}
		field.Serialize(dbf);
		if(dbf.IsError())
			return false;
		if(dbf.IsLoading()) {
			if(field.type == 0)
				break;
			String uname = ToUpper(ToAscii(field.name, charset), charset);
			String outname;
			for(const char *p = uname; *p; p++)
				if(IsAlNum(*p) || *p == '_')
					outname.Cat(*p);
			if(outname.IsEmpty() || IsDigit(*outname))
				outname.Insert(0, 'F');
			if(used_names.Find(outname) >= 0) {
				int i = 0;
				while(used_names.Find(outname + '_' + IntStr(++i)) >= 0)
					;
				outname << '_' << i;
			}
			used_names.Add(field.name = outname);
		}
		if(dbf.IsLoading() && field.name != "_DBASELOCK" && IsValidType(field.type)) {
			field.offset = offset;
			fields.Add(field.name, field);
			if(field.type == 'M')
				has_memo = true;
		}
		offset += field.width;
	}
	byte b = 0x0D;
	dbf % b;
	ASSERT(dbf.GetPos() <= (unsigned)data_offset);
	record.SetCount(row_width);
	memset(record.Begin(), ' ', record.GetCount());
	if(dbf.IsError())
		return false;
	if(has_memo) {
		if(dbt.IsOpen() && dbt.IsLoading()) { // read DBT header
			dbt.Seek(16);
			int b = dbt.Get();
			dbt_block_size = 512;
			if(b != 3) {
				dbt.Seek(20);
				dbt_block_size = (short)dbt.Get16le();
				if(dbt_block_size <= 0)
					dbt.Close(); // has_memo = false;
			}
		}
		if(fpt.IsOpen() && fpt.IsLoading()) { // read FPT header
			// int nextfree = fpt.Get32be();
			fpt_block_size = fpt.Get32be();
			if(fpt_block_size <= 0 || fpt_block_size >= 1 << 24)
				fpt.Close(); // has_memo = false;
		}
	}
	return true;
}

bool DbfStream::DoOpen(byte _charset)
{
	charset = _charset;
	dirty = false;
	dirty_header = false;
	row_index = Null;
	next_row_index = 0;
	if(!StreamHeader(true))
		return false;
	DoFields();
	return true;
}

bool DbfStream::DoCreate(const Array<Field>& _fields, byte _charset)
{
	charset = _charset;
	rows = 0;
	dirty = false;
	dirty_header = false;
	row_index = 0;
	next_row_index = 0;
	has_memo = false;
	fields.Clear();
	for(int i = 0; i < _fields.GetCount(); i++)
		fields.Add(_fields[i].name, _fields[i]);
	if(!StreamHeader(true))
		return false;
	DoFields();
	return true;
}

void DbfStream::DoFields()
{
	field_read.Clear();
	field_read.SetCount(fields.GetCount(), &DbfStream::GetItemEmpty);
	for(int i = 0; i < fields.GetCount(); i++) {
		const Field& fld = fields[i];
		switch(fld.type) {
		case 'C': field_read[i] = &DbfStream::GetItemString; break;
		case 'N':
		case 'F': field_read[i] = (fld.width < 10 && fld.decimal == 0 ? &DbfStream::GetItemInt : &DbfStream::GetItemDouble); break;
		case 'O': field_read[i] = &DbfStream::GetItemDouble8; break;
		case '!':
		case '+': field_read[i] = &DbfStream::GetItemInt4; break;
		case '@': field_read[i] = &DbfStream::GetItemDateTime8; break;
		case 'D': field_read[i] = (fld.width < 10 ? &DbfStream::GetItemDateShort : &DbfStream::GetItemDateLong); break;
		case 'L': field_read[i] = &DbfStream::GetItemLogical; break;
		case 'M':
		case 'G': field_read[i] = &DbfStream::GetItemMemoString; break;
		case 'B': field_read[i] = &DbfStream::GetItemMemoBinary; break;
		}
	}
}

void DbfStream::SetRowCount(int count)
{
	if(rows > count) { // trim file
		dbf.SetSize(data_offset + count * record.GetCount());
		dirty_header = true;
	}
	else if(count > rows) { // extend file
		dbf.Seek(data_offset + rows * record.GetCount());
		dbf.Put(' ', (count - rows) * record.GetCount());
		dirty_header = true;
	}
	rows = count;
}

void DbfStream::Flush()
{
	FlushRow();
	FlushHeader();
}

void DbfStream::FlushRow()
{
	ASSERT(IsOpen());
	if(dirty) {
		ASSERT(!IsReadOnly());
		ASSERT(row_index >= 0);
		if(row_index > rows) {
			dbf.Seek(data_offset + rows * record.GetCount());
			dbf.Put(' ', (row_index - rows) * record.GetCount());
		}
		else
			dbf.Seek(data_offset + row_index * record.GetCount());
		dbf.Put(record.Begin(), record.GetCount());
		if(row_index >= rows) {
			rows = row_index + 1;
			dirty_header = true;
		}
		dirty = false;
	}
}

void DbfStream::FlushHeader()
{
	ASSERT(IsOpen());
	if(dirty_header) {
		ASSERT(!IsReadOnly());
		dbf.Seek(0);
		dbf.SetStoring();
		StreamHeader(false);
		dirty_header = false;
	}
}

bool DbfStream::Close()
{
	if(IsOpen())
		Flush();
	dbf.Close();
	dbt.Close();
	return !dbf.IsError() && (!has_memo || !dbt.IsError());
}

bool DbfStream::Fetch(int row)
{
	LTIMING("DbfStream::Fetch");
	ASSERT(IsOpen());
	if(dirty)
		FlushRow();
	next_row_index = row + 1;
	if(row == row_index)
		return *record;
	row_index = row;
	if(row < 0 || row >= rows) {
		*record.Begin() = 0;
		return false;
	}

	dbf.Seek(data_offset + row * record.GetCount());
	byte *p = record.Begin(), *e = record.End();
	int count = dbf.Get(p, (int)(e - p));
	if(count < record.GetCount())
		memset(p + count, ' ', record.GetCount() - count);
	if(count <= 0 || (*p != ' ' && *p != 0)) {
		*p = 0;
		return false;
	}
	*p = ' ';
/*
	if(codepage_cv)
		for(const char *page = codepage_map; ++p < e; *p = page[*p])
			;
*/
	dirty = false;
	return true;
}

void DbfStream::Seek(int row)
{
	if(row != row_index) {
		FlushRow();
		next_row_index = row;
	}
}

bool DbfStream::Fetch()
{
	int r = next_row_index;
	for(; r < rows; r++)
		if(Fetch(r))
			return true;
	return false;
}

Value DbfStream::GetItem(int i) const
{
	LTIMING("DbfStream::GetItem");
	ASSERT(i >= 0 && i < fields.GetCount());
	return (this ->* (field_read[i].proc))(i);
}

void DbfStream::PutItem(int i, Value value)
{
	const Field& fld = fields[i];
	dirty = true;
	byte *p = record.Begin() + fld.offset;
	if(IsNull(value)) // empty value is the same for all data types
		memset(p, ' ', fld.width);
	else {
		String s = fld.Format(value, charset);
		int l = min<int>(s.GetLength(), fld.width);
		memcpy(p, s, l);
		if(l < fld.width)
			memset(p + l, ' ', fld.width - l);
	}
}

void DbfStream::PutItem(String n, Value value)
{
	int i = GetFieldIndex(n);
	if(i >= 0)
		PutItem(i, value);
	else
		NEVER(); // column missing
}

Value DbfStream::GetItem(String name) const
{
	int index = GetFieldIndex(name);
	if(index >= 0)
		return GetItem(index);
	else {
		NEVER();
		return ErrorValue(NFormat("column '%s' not found", name));
	}
}

Vector<Value> DbfStream::FetchRow()
{
	if(!Fetch())
		return Vector<Value>();
	return FetchRow(row_index);
}

Vector<Value> DbfStream::FetchRow(int row)
{
	LTIMING("DbfStream::FetchRow");
	Vector<Value> values;
	if(!Fetch(row))
		return values;
	values.SetCount(fields.GetCount());
	for(int i = 0; i < fields.GetCount(); i++)
		values[i] = (this ->* (field_read[i].proc))(i);
	return values;
}

void DbfStream::WriteRow(int row, const Vector<Value>& values)
{
	ASSERT(row >= 0);
	FlushRow();
	if(values.GetCount() < fields.GetCount())
		Fetch(row);
	else {
		row_index = row;
		next_row_index = row_index + 1;
	}
	for(int i = 0, n = min(values.GetCount(), fields.GetCount()); i < n; i++)
		PutItem(i, values[i]);
}

void DbfStream::WriteRow(const Vector<Value>& values)
{
	WriteRow(next_row_index, values);
	Seek(row_index + 1);
}

void DbfStream::WriteRow()
{
	row_index = next_row_index;
	FlushRow();
	next_row_index = ++row_index;
}

String DbfStream::ToString() const
{
	String result;
	result
		<< fields.GetCount() << " columns\n"
		<< rows << " rows\n\nColumns:\n";

	for(int i = 0; i < GetFieldCount(); i++) {
		const Field& field = GetField(i);
		result = Format("%2i: %-12s '%c' %d", i, ~field.name, field.type, field.width);
		if(field.decimal)
			result << '.' << field.decimal;
		result << '\n';
	}
	return result << '\n';
}

void DbfStream::DumpData(Stream& stream)
{
	stream << *this; // dump layout

	// dump column headers
	stream << "ROWID ";
	int total = 6;
	Vector<int> fw;

	int i;
	for(i = 0; i < fields.GetCount(); i++) {
		const Field& field = GetField(i);
		int width = max<int>(field.width, (int)strlen(field.name));
		switch(field.type) {
		case 'D': width = max(width, 10); break;
		}
		++width;
		stream << NFormat("%*<s", width, field.name);
		fw.Add() = width;
		total += width;
	}
	stream << "\n-";
	stream.Put('-', total - 2);
	stream << '\n';

	for(i = 0; i < rows; i++) {
		stream << NFormat("%5d ", i);
 		if(Fetch(i)) { // display individual columns
			for(int j = 0; j < fields.GetCount(); j++)
				stream << NFormat("%*<vt", fw[j], GetItem(j));
		}
		stream << '\n';
	}
	stream << "\n\n";
}

Value DbfStream::GetItemDouble8(int i) const
{
	const byte *p = record.Begin() + fields[i].offset;
#ifdef CPU_BE
	union {
		char temp[8];
		double dbl;
	};
	temp[0] = p[7];
	temp[1] = p[6];
	temp[2] = p[5];
	temp[3] = p[4];
	temp[4] = p[3];
	temp[5] = p[2];
	temp[6] = p[1];
	temp[7] = p[0];
	return dbl;
#else
	return *(const double *)p;
#endif
}

Value DbfStream::GetItemInt4(int i) const
{
	const byte *p = record.Begin() + fields[i].offset;
	int v = Peek32le(p);
	v ^= 0x80000000;
	return v;
}

Value DbfStream::GetItemDateTime8(int i) const
{
	const byte *p = record.Begin() + fields[i].offset;
	int dt = Peek32le(p + 0);
	int tm = Peek32le(p + 4);
	Date d = Date(-4713, 1, 1) + dt;
	Time t = ToTime(d);
	tm /= 1000;
	t.second = tm % 60;
	tm /= 60;
	t.minute = tm % 60;
	tm /= 60;
	t.hour = tm;
	return t;
}

Value DbfStream::GetItemInt(int i) const
{
	const byte *p = record.Begin() + fields[i].offset, *e = p + fields[i].width;
	while(p < e && *p == ' ')
		p++;
	return scan_int((const char *)p, (const char *)e);
}

Value DbfStream::GetItemDouble(int i) const
{
	const byte *p = record.Begin() + fields[i].offset, *e = p + fields[i].width;
	while(p < e && *p == ' ')
		p++;
	return scan_dbl((const char *)p, (const char *)e);
}

Value DbfStream::GetItemString(int i) const
{
	const byte *p = record.Begin() + fields[i].offset, *e = p + fields[i].width;
	while(p < e && *p == ' ')
		p++;
	while(e > p && e[-1] == ' ')
		e--;
	String s(p, (int)(e - p));
	if(charset == GetDefaultCharset())
		return s;
	return ToUnicode(s, charset);
}

Value DbfStream::GetItemDateShort(int i) const
{
	const byte *p = record.Begin() + fields[i].offset;
	Date date;
	if(IsDigit(*p)) {
		if(p[2] == '.') {
			date.day = 10 * p[0] + p[1] - 11 * '0';
			date.month = 10 * p[3] + p[4] - 11 * '0';
			date.year = 10 * p[6] + p[7] + 1900 - 11 * '0';
		}
		else {
			date.year = 1000 * p[0] + 100 * p[1] + 10 * p[2] + p[3] - 1111 * '0';
			date.month = 10 * p[4] + p[5] - 11 * '0';
			date.day = 10 * p[6] + p[7] - 11 * '0';
		}
	}
	return date;
}

Value DbfStream::GetItemDateLong(int i) const
{
	const byte *p = record.Begin() + fields[i].offset;
	if(IsDigit(*p) && !IsDigit(p[2])) {
		Date date;
		date.day = 10 * p[0] + p[1] - 11 * '0';
		date.month = 10 * p[3] + p[4] - 11 * '0';
		date.year = 1000 * p[6] + 100 * p[7] + 10 * p[8] + p[9] - 1111 * '0';
		return date;
	}
	else
		return GetItemDateShort(i);
}

Value DbfStream::GetItemLogical(int i) const
{
	return StrBool(record[fields[i].offset]);
}

Value DbfStream::GetItemMemoString(int i) const
{
	return GetItemMemo(i, false);
}

Value DbfStream::GetItemMemoBinary(int i) const
{
	return GetItemMemo(i, true);
}

Value DbfStream::GetItemMemo(int i, bool binary) const
{
	if(!has_memo)
		return false;
	const byte *p = record.Begin() + fields[i].offset, *e = p + fields[i].width;
	while(p < e && *p == ' ')
		p++;
	int block = scan_int((const char *)p, (const char *)e);
	if(!IsNull(block) && block > 0) {
		String out;
		if(dbt.IsOpen()) {
			unsigned pos = block * dbt_block_size;
			if(pos >= dbt.GetSize())
				return Value();
			dbt.Seek(pos);
			if(dbt.GetIL() == 0x8FFFF) { // dBASE IV memo with explicit length
				unsigned len = dbt.GetIL();
				if(len <= 0 || len > dbt.GetLeft())
					return Value();
				Buffer<byte> buffer(len);
				if(!dbt.GetAll(buffer, len))
					return Value();
				if(!binary) {
					byte *p;
					if((p = (byte *)memchr(buffer, '\0', len))) len = (unsigned)(p - buffer);
					if((p = (byte *)memchr(buffer, '\x1A', len))) len = (unsigned)(p - buffer);
				}
				out = String(buffer, len);
			}
			else { // dBASE III memo with 1A1A separator
				Buffer<byte> buffer(dbt_block_size + 1);
				bool eof = false;
				for(int b; !eof && (b = dbt.Get(buffer, dbt_block_size + 1)) > 0;) {
					byte *p = buffer, *e = p + b;
					while((p = (byte *)memchr(p, '\x1A', e - p)) != 0)
						if(++p < e && *p++ == '\x1A') { // double EOF found
							e = p - 2;
							eof = true;
							break;
						}
					int l = (int)(e - buffer);
					if(!eof && l > dbt_block_size) {
						dbt.SeekCur(dbt_block_size - l);
						e = buffer + l;
					}
					if(l > 0)
						out.Cat(buffer, l);
				}
			}
		}
		else if(fpt.IsOpen()) {
			unsigned pos = block * fpt_block_size;
			if(pos >= fpt.GetSize())
				return Value();
			fpt.Seek(pos);
			// int fieldtype = fpt.Get32be();
			int len = fpt.Get32be();
			if(len > 0) {
				StringBuffer outbuf(len);
				if(!fpt.GetAll(outbuf, len))
					return Value();
				out = outbuf;
			}
		}
		if(binary || charset == GetDefaultCharset())
			return out;
		return ToUnicode(out, charset);
	}
	return Value();
}

Value DbfStream::GetItemEmpty(int i) const
{
	return Value();
}

}
