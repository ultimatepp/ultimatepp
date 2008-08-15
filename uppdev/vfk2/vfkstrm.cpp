#include "vfkstrm.h"

static struct VFKLongName
{
	const char *longname;
	const char *shortname;
}
vfk_long_names[] =
{
	{ "PARCELY",              "PAR" },
	{ "BUDOVY",               "BUD" },
	{ "CASTI_BUDOV",          "CABU" },
	{ "ZP_OCHRANY_NEM",       "ZPOCHN" },
	{ "D_POZEMKU",            "DRUPOZ" },
	{ "ZP_VYUZITI_POZ",       "ZPVYPO" },
	{ "ZDROJE_PARCEL_ZE",     "ZDPAZE" },
	{ "ZP_URCENI_VYMERY",     "ZPURVY" },
	{ "T_BUDOV",              "TYPBUD" },
	{ "MAPOVE_LISTY",         "MAPLIS" },
	{ "KATASTR_UZEMI",        "KATUZE" },
	{ "CASTI_OBCI",           "CASOBC" },
	{ "R_ZPOCHR",             "RZO" },
	{ "ZP_VYUZITI_BUD",       "ZPVYBU" },
	{ "JEDNOTKY",             "JED" },
	{ "T_JEDNOTEK",           "TYPJED" },
	{ "ZP_VYUZITI_JED",       "ZPVYJE" },
	{ "BONIT_DILY_PARC",      "BDP" },
	{ "OPRAV_SUBJEKTY",       "OPSUB" },
	{ "VLASTNICTVI",          "VLA" },
	{ "CHAR_OS",              "CHAROS" },
	{ "TELESA",               "TEL" },
	{ "JINE_PRAVNI_VZTAHY",   "JPV" },
	{ "T_PRAVNICH_VZTAHU",    "TYPRAV" },
	{ "RIZENI_KU",            "RIZKU" },
	{ "OBJEKTY_RIZENI",       "OBJRIZ" },
	{ "PREDMETY_RIZENI",      "PRERIZ" },
	{ "UCASTNICI",            "UCAST" },
	{ "ADRESY",               "ADRUC" },
	{ "LISTINY",              "LISTIN" },
	{ "DALSI_UDAJE_LISTINY",  "DUL" },
	{ "LISTINY_DALSI_UDAJE",  "LDU" },
	{ "T_LISTIN",             "TYPLIS" },
	{ "T_PREDMETU_R",         "TYPPRE" },
	{ "TYPY_RIZENI",          "TYPRIZ" },
	{ "TYPY_UCASTNIKU",       "TYPUCA" },
	{ "UCASTNICI_TYP",        "UCTYP" },
	{ "R_LIST",               "RL" },
	{ "SOURADNICE_OBRAZU",    "SOBR" },
	{ "SPOJENI_B_POLOH",      "SBP" },
	{ "SPOJENI_B_MAPY",       "SBM" },
	{ "KODY_CHAR_Q_BODU",     "KODCHB" },
	{ "T_SOURAD_SYS",         "TYPSOS" },
	{ "HRANICE_PARCEL",       "HP" },
	{ "OBRAZY_PARCEL",        "OP" },
	{ "OBRAZY_BUDOV",         "OB" },
	{ "DALSI_PRVKY_MAPY",     "DPM" },
	{ "OBRAZY_BODU_BP",       "OBBP" },
	{ "T_PRVKU_P_DAT",        "TYPPPD" },
	{ "HRANICE_BPEJ",         "HBPEJ" },
	{ "OZNACENI_BPEJ",        "OBPEJ" },
	{ "NAVRHY_ZMEN_KM",       "NZ" },
	{ "NZ_ZPMZ",              "NZZP" },
	{ "SOURADNICE_POLOHY",    "SPOL" },
	{ "REZ_PARCELNI_CISLA",   "RECI" },
	{ "DOTCENA_PAR_CISLA",    "DOCI" },
	{ "DOT_HIST_PAR_CISLA",   "DOHICI" },
	{ "SROVNAVACI_SESTAVENI", "SRST" },
	{ 0 }
};

static bool IsPointMap(String name)
{
	return name == "SPOJENI_B_MAPY"
		|| name == "SOURADNICE_OBRAZU"
		|| name == "DALSI_PRVKY_MAPY"
		|| name == "OBRAZY_PARCEL"
		|| name == "OBRAZY_BUDOV"
		|| name == "OBRAZY_BODU_BP"
		|| name == "SOURADNICE_POLOHY"
		|| name == "PARCELY"
		|| name == "BUDOVY";
}

inline bool IsIdent(char c)
{
	return c >= '0' && c <= '9' || c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c == '_';
}

static String GetVTypeName(int vtype)
{
	switch(vtype) {
		default:
			case VOID_V:       return "void";
			case ERROR_V:      return "error";
			case UNKNOWN_V:    return "unknown";
			case VALUE_V:      return "Value";
			case INT_V:        return "int";
			case DOUBLE_V:     return "double";
			case INT64_V:      return "int64";
			case STRING_V:     return "String";
			case WSTRING_V:    return "WString";
			case DATE_V:       return "Date";
			case TIME_V:       return "Time";
			case VALUEARRAY_V: return "ValueArray";
			return             NFormat("unknown(%d)", vtype);
	}
}

inline Time VfkReadTime(const char *p, void *endptr)
{
	int fields[6];
	ZeroArray(fields);
	int nfld = 0;
	for(;;)
		if(IsDigit(*p)) {
			int v = ScanInt(p, &p);
			if(IsNull(v) || v < 0)
				return Null;
			fields[nfld++] = v;
			if(nfld >= 6)
				break;
		}
		else if(*p == ' ' || *p == ':' || *p == '.')
			p++;
		else
			break;
	if(endptr)
		*(const char **)endptr = p;
	if(nfld < 3)
		return Null;
	if(fields[0] >= 1 && fields[0] <= 31
	&& fields[1] >= 1 && fields[1] <= 12
	&& fields[2] >= 1900 && fields[2] <= 2900
	&& fields[3] >= 0 && fields[3] <= 23
	&& fields[4] >= 0 && fields[4] <= 59
	&& fields[5] >= 0 && fields[5] <= 59) {
		Time dt(fields[2], fields[1], fields[0], fields[3], fields[4], fields[5]);
		if(dt.IsValid())
			return dt;
	}
	return Null;
}

static String VFKParsePolvz(const char *text)
{
	for(; *text; text++)
		if(text[0] == 'P' && text[1] == 'O' && text[2] == 'L' && text[3] == 'V' && text[4] == 'Z' && text[5] == ':') {
			text += 6;
			while(*text && (byte)*text <= ' ')
				text++;
			const char *b = text;
			bool slash = false;
			while(*text && (byte)*text > ' ')
				if(*text++ == '/')
					slash = true;
			String out;
			int len = text - b;
			if(slash || len <= 4)
				out = String(b, text);
			else {
				out.Cat(b, len - 4);
				out.Cat('/');
				out.Cat(text - 4, 4);
			}
			return out;
		}
	return Null;
}

static String VFKParseDOPLKOD(String s)
{
	const char *p = s;
	while(*p && !(p[0] == '/' && p[1] == ' ' && p[2] == 'D' && p[3] == 'O'
		&& p[4] == 'P' && p[5] == 'L' && p[6] == 'K' && p[7] == 'O' && p[8] == 'D'))
		p++;
	if(!*p)
		return s;
	while(p > s.Begin() && (byte)p[-1] <= ' ')
		p--;
	return s.Left(p - s.Begin());
}

/*
void VfkStream::Table::FlushCache()
{
	scanned = scanned_sbp = scanned_sobr = false;
	row_offsets.Clear();
	segment_index.Clear();
	point_index.Clear();
	extents.Clear();
	points.Clear();
	sobr_map.Clear();
	par_ku_map.Clear();
	cobce_obec_map.Clear();
	tel_ku_map.Clear();
	bud_id_index.Clear();
	bud_id_zdb.Clear();
	bud_id_ob.Clear();
	ku_index.Clear();
	obec_kod_index.Clear();
}
*/

int VfkStream::Table::SizeOfInstance() const
{
	return columns.GetCount() * 100
/*
 + row_offsets.GetCount() * sizeof(int)
		+ segment_index.GetCount() * sizeof(int) + point_index.GetCount() * sizeof(int)
		+ extents.GetCount() * sizeof(Rectf) + points.GetCount() * sizeof(const Pointf *)
		+ sobr_map.GetCount() * (sizeof(double) + sizeof(Pointf) + 4 * sizeof(int))
		+ par_ku_map.GetCount() * (6 * sizeof(int))
		+ cobce_obec_map.GetCount() * (6 * sizeof(int))
		+ tel_ku_map.GetCount() * (6 * sizeof(int))
		+ bud_id_zdb.GetCount() * (5 * sizeof(int) + sizeof(Pointf))
		+ bud_id_ob.GetCount() * 200
		+ ku_index.GetCount() * (4 * sizeof(int))
		+ bud_id_index.GetCount() * (4 * sizeof(int))
		+ obec_kod_index.GetCount() * (4 * sizeof(int));
*/
	;
}

VfkStream::Data::Data(VfkStream *vfk_, Table *table_)
: vfk(vfk_), table(table_)
{
	header = table->header;
	row_buffer.SetCount(table->columns.GetCount());
	row_index = -1;
}

int64 VfkStream::Data::GetCount() const
{
	if(!table->IsScanned())
		vfk->ScanTable(*table);
	return table->header ? vfk->header.GetCount() : table->row_count;
}

const Vector<Value>& VfkStream::Data::operator [] (int64 row) const
{
	if(!table->IsScanned())
		vfk->ScanTable(*table);
	if(header)
		return vfk->header[(int)row];
	ASSERT(row >= 0 && row < table->row_count);
	if(row != row_index) {
		row_index = row;
		vfk->ReadRow(*table, row_index = row, row_buffer);
	}
	return row_buffer;
}

GisObject VfkStream::Data::GetPath(int64 row, double resolution) const
{
	return Null;
/*
	int bs = table->segment_index[row + 0], es = table->segment_index[row + 1];
	if(es <= bs)
		return Null;
	RefPtr<GisObjectPath> path = new GisObjectPath;
	path->extent = table->extents[row];
	if(path->extent.Width() <= resolution && path->extent.Height() <= resolution) {
		path->index.SetCount(1, 1);
		path->points.SetCount(1, path->extent.CenterPoint());
	}
	else {
		path->index.Reserve(es - bs);
		int bx = table->point_index[bs], ex = table->point_index[es];
		for(int t = bs; t < es; t++)
			path->index.Add(table->point_index[t + 1] - bx);
		path->points.Reserve(ex - bx);
		for(int t = bx; t < ex; t++)
			path->points.Add(*table->points[t]);
	}
	return GisObject(~path);
*/
}

GisObject VfkStream::Data::GetArea(int64 row, double resolution) const
{
	return Null;
/*
	int bs = table->segment_index[row + 0], es = table->segment_index[row + 1];
	if(es <= bs)
		return Null;
	RefPtr<GisObjectArea> area = new GisObjectArea;
	area->extent = table->extents[row];
	if(area->extent.Width() <= resolution && area->extent.Height() <= resolution) {
		area->index.SetCount(1, 1);
		area->points.SetCount(1, area->extent.CenterPoint());
	}
	else {
		area->index.Reserve(es - bs);
		int bx = table->point_index[bs], ex = table->point_index[es];
		for(int t = bs; t < es; t++)
			area->index.Add(table->point_index[t + 1] - bx);
		area->points.Reserve(ex - bx);
		for(int t = bx; t < ex; t++)
			area->points.Add(*table->points[t]);
	}
	return GisObject(~area);
*/
}

VfkStream::VfkStream()
{
	charset = CHARSET_DEFAULT;
}

void VfkStream::Open(const VectorMap<int, String>& fns)
{
	Close();
	charset = CHARSET_ISO8859_2;
	file_groups <<= fns;
	streams.SetCount(file_groups.GetCount());
	for(int i = 0; i < streams.GetCount(); i++)
		if(!streams[i].Open(file_groups[i]))
			throw Exc(NFormat("cannot open file '%s'", file_groups[i]));
	indexfilename = GetTempFileName("vfk");
	if(!indexfile.Open(indexfilename, FileStream::CREATE))
		throw Exc(NFormat("cannot create indexfile '%s'", indexfilename));
	String hdrname = "X_HEADER";
	Table& hdr = tables.Add(hdrname);
	hdr.name = hdr.rawname = hdrname;
	hdr.header = true;
	hdr.row_count = 0;
	ASSERT(hdr.columns.GetCount() == HDR_ID);
	hdr.AddColumn(Column("ID", STRING_V, 30));
	ASSERT(hdr.columns.GetCount() == HDR_ORD);
	hdr.AddColumn(Column("ORD", INT_V));
	ASSERT(hdr.columns.GetCount() == HDR_STR);
	hdr.AddColumn(Column("STR", STRING_V, 1000));
	ASSERT(hdr.columns.GetCount() == HDR_NUM);
	hdr.AddColumn(Column("NUM", DOUBLE_V));
	ASSERT(hdr.columns.GetCount() == HDR_DTM);
	hdr.AddColumn(Column("DTM", TIME_V));
	ASSERT(hdr.columns.GetCount() == HDR_COUNT);
	Scan();
}

void VfkStream::Close()
{
	streams.Clear();
	if(indexfile.IsOpen()) {
		indexfile.Close();
		FileDelete(indexfilename);
	}
	tables.Clear();
	header.Clear();
}

int VfkStream::SizeOfInstance() const
{
	int si = sizeof(*this);
	for(int i = 0; i < tables.GetCount(); i++)
		si += SizeOf(tables[i]);
	return si;
}

/*
int VfkStream::MappingSizeOfInstance() const
{
	return mapping.IsOpen() ? mapping.GetLength() : 0;
}
*/

Vector<String> VfkStream::GetTables() const
{
	Vector<String> out;
	out <<= tables.GetKeys();
	return out;
}

VfkStream::Data VfkStream::GetData(String table)
{
	int f = tables.Find(table);
	ASSERT(f >= 0);
/*
	if(table == "HRANICE_PARCEL")
		ScanSBP(table, "HP_ID");
	else if(table == "OBRAZY_BUDOV")
		ScanSBP(table, "OB_ID");
	else if(table == "DALSI_PRVKY_MAPY")
		ScanSBP(table, "DPM_ID");
*/
	return GetDataRaw(f);
}

void VfkStream::Dump()
{
	puts(NFormat("Index file: %s", indexfilename));
	puts(NFormat("%d file group(s):", file_groups.GetCount()));
	for(int i = 0; i < file_groups.GetCount(); i++)
		puts(NFormat("\t[%d] = %d / %s (%n B)", i, file_groups.GetKey(i), file_groups[i],
			streams[i].IsOpen() ? streams[i].GetSize() : 0));
	puts("Tables:");
	for(int t = 0; t < tables.GetCount(); t++) {
		const Table& tbl = tables[t];
		puts(NFormat("\tTable(%d, %s (%s)) sections:\n", t, tbl.name, tbl.rawname));
		for(int s = 0; s < tbl.file_index.GetCount(); s++)
			puts(NFormat("\t\t[%d]: file index [%d], range %n .. %n, %n bytes, %n rows",
				s, tbl.file_index[s], tbl.begin_offset[s], tbl.end_offset[s],
				tbl.end_offset[s] - tbl.begin_offset[s], tbl.row_count));
/*
		for(int c = 0; c < tbl.columns.GetCount(); c++) {
			const Column& col = tbl.columns[c];
			puts(NFormat("\t\t(%d, %s): %s %~d", c, col.name, GetVTypeName(col.vtype), col.width));
		}
*/
		Data data = GetData(tbl.name);
		puts(NFormat("%d row(s):", data.GetCount()));
		for(int i = 0; i < 10 && i < data.GetCount(); i++) {
			const Vector<Value>& row = data[i];
			String out;
			for(int v = 0; v < row.GetCount(); v++)
				out << (v ? ";" : "") << StdFormat(row[v]);
			puts(out);
		}
	}
}

void VfkStream::SpeedTest()
{
	for(int i = 0; i < tables.GetCount(); i++) {
		int begin = msecs();
		Data data = GetData(tables[i].name);
		int get = msecs();
		if(data.GetCount() > 0)
			data[0];
		int first = msecs();
		for(int r = 1; r < data.GetCount(); r++)
			data[r];
		int nth = msecs();
		puts(NFormat("%s: get(%d), first(%d), nth(%2v)",
			tables[i].name, get - begin, first - get, (nth - first) / max<double>((double)data.GetCount() - 1, 1)));
	}
}

bool VfkStream::IsDateTime(String field) const
{
	return field == "PLATNOST" || field == "VYTVORENO";
}

WString VfkStream::ReadString(const char *b, const char **endptr) const
{
	if(*b++ != '\"')
		throw Exc("'\"' expected");
	StringBuffer out;
	for(;;) {
		const char *b0 = b;
		while(*b && *b != '\"')
			b++;
		out.Cat(b0, b - b0);
		if(*b != '\"' || *++b != '\"')
			break;
		out.Cat(*b++);
	}
	if(endptr)
		*endptr = b;
	return ToUnicode((String)out, charset);
}

void VfkStream::ScanHeader(const char *b, Table& table)
{
	int colid = 0;
	while(*b && *b != '\n') {
		if(IsAlpha(*b)) {
			const char *id = b;
			while(IsIdent(*++b))
				;
			Column col;
			col.name = String(id, b);
			try {
				while(*b == ' ')
					b++;
				col.vtype = ERROR_V;
				col.width = Null;
				switch(*b++) {
					case 'N': {
						col.vtype = DOUBLE_V;
						break;
					}
					case 'T': {
						col.vtype = STRING_V;
						if(!IsDigit(*b))
							throw Exc("missing string width");
						col.width = ScanInt(b, &b);
						break;
					}
					case 'D': {
						col.vtype = TIME_V;
						break;
					}
					default: {
						throw Exc(NFormat("invalid data type '%c'", *--b));
					}
				}
				if(colid < table.columns.GetCount()) {
					const Column& old = table.columns[colid];
					if(old.name != col.name)
						throw Exc(NFormat("column name mismatch (%s / %s)", old.name, col.name));
					if(old.vtype != col.vtype)
						throw Exc(NFormat("column type mismatch (%d / %d)", old.vtype, col.vtype));
					if(old.width != col.width)
						throw Exc(NFormat("column width mismatch (%~d / %~d)", old.width, col.width));
				}
				else
					table.AddColumn(col);
				colid++;
			}
			catch(Exc e) {
				throw Exc(NFormat("column '%s': %s", colid, e));
			}
		}
		else
			b++;
	}
}

void VfkStream::SkipRow(Stream& strm)
{
	RTIMING("VfkStream::SkipRow");
	enum { BATCH = 100 };
	char buffer[BATCH + 1];
	int count = strm.Get(buffer, BATCH);
	buffer[count] = '\n';
	const char *p = buffer;
	for(;;)
		if(*p == '\n') {
			if(p - buffer < count) {
				strm.SeekCur((p - buffer) - count  + 1);
				return;
			}
			buffer[count = strm.Get(buffer, BATCH)] = '\n';
			p = buffer;
			if(!count)
				return;
		}
		else if(*p++ == '\"') {
			for(;;) {
				if(*p == '\n' && p - buffer >= count) {
					buffer[count = strm.Get(buffer, BATCH)] = '\n';
					p = buffer;
					if(!count)
						return;
				}
				else if(*p++ == '\"') {
					if(*p == '\n' && p - buffer >= count) {
						buffer[count = strm.Get(buffer, BATCH)] = '\n';
						p = buffer;
						if(!count)
							return;
					}
					if(*p != '\"')
						break;
					p++;
				}
			}
		}
}

/*
void VfkStream::ScanRow(Table& table) const
{
	table.row_offsets.Add(b - (const char *)mapping.Begin() - 1);
	return SkipRow(b);
}
*/

void VfkStream::ReadRow(const Table& table, int64 index, Vector<Value>& row)
{
	RTIMING("VfkStream::ReadRow");
	indexfile.Seek(table.row_offsets_xpos + 12 * index);
	int64 off = indexfile.Get64le();
	int len = indexfile.Get16le();
	int ixf = indexfile.Get16le();
	if(off < 0 || len < 0 || ixf < 0 || ixf > streams.GetCount())
		return;
	Buffer<char> rowbuf(len + 1);
	Stream& strm = streams[ixf];
	strm.Seek(off);
	strm.Get(rowbuf, len);
	rowbuf[len] = 0;
	const char *b = rowbuf;
	for(int i = 0; *b++ == ';' && i < row.GetCount(); i++) {
		switch(table.columns[i].vtype) {
			case DOUBLE_V: {
				row[i] = ScanDouble(b, &b);
				break;
			}

			case TIME_V: {
				if(*b == '\"') {
					row[i] = VfkReadTime(b + 1, &b);
					if(*b == '\"')
						b++;
				}
				break;
			}

			case STRING_V: {
				row[i] = ReadString(b, &b);
				break;
			}

			default: NEVER(); return;
		}
	}
}

void VfkStream::Scan()
{
	RTIMING("VfkStream::Scan");
	for(int i = 0; i < file_groups.GetCount(); i++)
		ScanFile(i);
}

void VfkStream::ScanFile(int fx)
{
	RTIMING("VfkStream::ScanFile");
	Stream& strm = streams[fx];
	int64 last_line = strm.GetSize();
	while(last_line > 0) {
		strm.Seek(last_line - 1);
		if(strm.Get() == '\n')
			break;
		last_line--;
	}
	strm.Seek(0);
	try {
		int c;
		int64 rowpos = strm.GetPos();
		while((c = strm.Get()) == '&' && ((c = strm.Get()) == 'H' || c == 'D') && IsAlpha(strm.Term())) {
			char type = c;
			int64 begin = strm.GetPos();
			SkipRow(strm);
			rowpos = strm.GetPos();
			int len = (int)(strm.GetPos() - begin);
			StringBuffer linebuf(len + 1);
			strm.Seek(begin);
			strm.Get(linebuf, len);
			linebuf[len] = 0;
			const char *b = linebuf;
			const char *id = b;
			while(IsIdent(*++b))
				;
			String ident(id, b);
			if(*b++ != ';')
				throw Exc(NFormat("';' expected after '%s' (found: '%c', %2:02x)", ident, *b));
			if(type == 'D') {
				String fident = "X_" + ident;
				int f = tables.Find(fident);
				if(f < 0)
					throw Exc(NFormat("unexpected data for filter table '%s'", ident));
//				b = ScanRow(b, tables[f]);
			}
			else if(IsAlpha(*b)) {
				String fident = "X_" + ident;
				Table& tbl = tables.GetAdd(fident);
				tbl.name = tbl.rawname = fident;
				tbl.row_count = 0;
				ScanHeader(b, tbl);
			}
			else {
				do {
					Vector<Value> row;
					row.SetCount(HDR_COUNT);
					if(*b == '\"') {
						WString text = ReadString(b, &b);
						if(IsDateTime(ident) && !IsNull(text)) {
							Time dt = VfkReadTime(text.ToString(), NULL);
							if(IsNull(dt))
								throw Exc(NFormat("invalid date/time value %s", AsCString(text.ToString())));
							row[HDR_DTM] = dt;
						}
						else {
							row[HDR_STR] = text;
							if(ident == "CODEPAGE")
								if(text == WString("EE8MSWIN1250")) charset = CHARSET_WIN1250;
						}
					}
					else {
						double num = ScanDouble(b, &b);
						if(IsNull(num))
							throw Exc("invalid numeric value");
						row[HDR_NUM] = num;
					}
					int l = header.FindLast(ident);
					row[HDR_ID] = ident;
					row[HDR_ORD] = (l >= 0 ? (int)header[l][HDR_ORD] + 1 : 0);
					header.Add(ident) = row;
				}
				while(*b++ == ';');
				b--;
			}
		}
		strm.Seek(rowpos);
		while(strm.Get() == '&' &&  strm.Get() == 'B' && IsAlpha(strm.Term())) {
			int64 header_offset = strm.GetPos();
			SkipRow(strm);
			int64 begin_offset = strm.GetPos();
			int len = (int)(begin_offset - header_offset);
			Buffer<char> linebuf(len + 1);
			strm.Seek(header_offset);
			strm.Get(linebuf, len);
			linebuf[len] = 0;
			const char *b = linebuf;
			const char *id = b;
			while(IsIdent(*++b))
				;
			int idlen = b - id;
			String ident(id, b);
			if(*b++ != ';')
				throw Exc(NFormat("';' expected after '%s' (found: '%c', %2:02x)", ident, *b));
			String name = ident;
			for(const VFKLongName *ln = vfk_long_names; ln->shortname; ln++)
				if(name == ln->shortname) {
					name = ln->longname;
					break;
				}
			Table& tbl = tables.GetAdd(name);
			tbl.name = name;
			tbl.rawname = ident;
			ScanHeader(b, tbl);
			int64 p = begin_offset, e = last_line;
			Buffer<char> idbuf(idlen + 3);
			while(p < e) {
				int64 m = (p + e) >> 1;
				while(m > p) {
					char part[100];
					int partsize = (int)min<int64>(m - p, sizeof(part));
					strm.Seek(m - partsize);
					strm.Get(part, partsize);
					const char *x = &part[partsize];
					while(x > part && x[-1] != '\n')
						x--;
					int lfpos = x - part;
					if(x > part && --x > part && x[-1] == '\r')
						x--;
					m -= partsize - lfpos;
					if(x <= part)
						continue;
					if(*--x != '\xA4')
						break;
					m -= lfpos - (x - part);
				}
				strm.Seek(m);
				if(strm.Get(idbuf, idlen + 3) != idlen + 3 || idbuf[0] != '&' || idbuf[1] != 'D'
				|| memcmp(~idbuf + 2, id, idlen) || idbuf[idlen + 2] != ';')
					e = m;
				else {
					SkipRow(strm);
					p = strm.GetPos();
				}
			}
			int xgrp = file_groups.GetKey(fx);
			int f;
			for(f = 0; f < tbl.file_index.GetCount(); f++)
				if(file_groups.GetKey(tbl.file_index[f]) == xgrp)
					break;
			if(f >= tbl.file_index.GetCount()) {
				tbl.file_index.Add(fx);
				tbl.begin_offset.Add(begin_offset);
				tbl.end_offset.Add(p);
			}
			strm.Seek(p);
		}
	}
	catch(Exc e) {
		throw Exc(NFormat("%s (offset %n): %s", file_groups[fx], strm.GetPos(), e));
	}
}

void VfkStream::ScanTable(Table& tbl)
{
	RTIMING("VfkStream::ScanTable");
	if(tbl.row_count >= 0)
		return;
	tbl.row_count = 0;
	tbl.row_offsets_xpos = indexfile.GetPos();
	int idlen = tbl.rawname.GetLength();
	enum { READBUF = 10000000 };
	Buffer<char> readbuf(READBUF + 1);
	for(int i = 0; i < tbl.file_index.GetCount(); i++) {
		RTIMING("VfkStream::ScanTable / file");
		int fx = tbl.file_index[i];
		Stream& strm = streams[fx];
		int64 pos = tbl.begin_offset[i], end = tbl.end_offset[i];
		while(pos < end) {
			RTIMING("VfkStream::ScanTable / buffer");
			strm.Seek(pos);
			int part = strm.Get(readbuf, (int)min<int64>(end - pos, READBUF));
			readbuf[part] = '\n';
			const char *b = readbuf, *e = b + part;
			for(;;) {
				RTIMING("VfkStream::ScanTable / row");
				int64 boff = pos + (b - ~readbuf);
				if(e - b < idlen + 3) {
					pos = boff;
					break;
				}
				if(b[0] != '&' || b[1] != 'D' || memcmp(b + 2, tbl.rawname, idlen) || b[2 + idlen] != ';') {
					pos = end;
					break;
				}
				b += 3 + idlen;
				int64 eoff = -1;
				while(b < e) {
					if(*b == '\n') {
						if(b < e)
							eoff = pos + (++b - ~readbuf);
						break;
					}
					else if(*b++ == '\"') {
						for(;;)
							if(*b == '\n' && b >= e)
								break;
							else if(*b++ == '\"') {
								if(*b == '\n' && b >= e)
									break;
								if(*b != '\"')
									break;
								b++;
							}
					}
				}
				if(eoff < 0) {
					pos = boff;
					break;
				}
				RTIMING("VfkStream::ScanTable / index stream write");
				indexfile.Put64le(boff);
				indexfile.Put16le((int)(eoff - boff));
				indexfile.Put16le(fx);
				tbl.row_count++;
			}
		}
	}
}

VfkStream::Data VfkStream::GetDataRaw(int t)
{
	RTIMING("VfkStream::GetData");
	return Data(this, &tables[t]);
}

/*
void VfkStream::ScanSOBR()
{
	int f = tables.Find("SOURADNICE_OBRAZU");
	if(f < 0 || !tables[f].sobr_map.IsEmpty())
		return;
	Table& sobr = tables[f];
	int id = sobr.columns.Find("ID");
	int px = sobr.columns.Find("SOURADNICE_Y");
	int py = sobr.columns.Find("SOURADNICE_X");
	if(id < 0 || px < 0 || py < 0)
		return;
	Data data = GetDataRaw(f);
	for(int i = 0, dc = data.GetCount(); i < dc; i++) {
		const Vector<Value>& row = data[i];
		double x = row[px], y = row[py];
		if(!IsNull(x) && !IsNull(y))
			sobr.sobr_map.Add(row[id], Pointf(-x, -y));
	}
}
*/

/*
void VfkStream::ScanSBP(String table, String sbp_id)
{
	int tf = tables.Find(table), sf = tables.Find("SPOJENI_B_POLOH"), of = tables.Find("SOURADNICE_OBRAZU");
	if(tf < 0 || sf < 0 || of < 0)
		return;
	Table& tbl = tables[tf];
	int tid = tbl.columns.Find("ID");
	if(tid < 0 || tbl.scanned_sbp)
		return;
	Data tdata = GetDataRaw(tf);
	tbl.segment_index.SetCount(tdata.GetCount() + 1, 0);
	tbl.point_index.Add(0);
	tbl.extents.SetCount(tdata.GetCount(), Null);
	tbl.scanned_sbp = true;

	Table& sbp = tables[sf];
	int rid = sbp.columns.Find(sbp_id);
	int por = sbp.columns.Find("PORADOVE_CISLO_BODU");
	int pid = sbp.columns.Find("BP_ID");
	if(rid < 0 || por < 0 || pid < 0)
		return;

	ScanSOBR();
	Table& sobr = tables[of];
	Index<double> rid_index;
	Vector< Vector<int> > por_lists;
	Vector< Vector<double> > pid_lists;

	Data sdata = GetDataRaw(sf);
	for(int i = 0, sc = sdata.GetCount(); i < sc; i++) {
		const Vector<Value>& row = sdata[i];
		if(!IsNull(row[rid])) {
			int n = rid_index.FindAdd(row[rid]);
			por_lists.At(n).Add(row[por]);
			pid_lists.At(n).Add(row[pid]);
		}
	}

	for(int i = 0, tc = tdata.GetCount(); i < tc; i++) {
		Rectf ext = Null;
		int f = rid_index.Find(tdata[i][tid]);
		if(f >= 0) {
			Vector<int>& por = por_lists[f];
			Vector<double>& pid = pid_lists[f];
			IndexSort(por, pid);
			for(int j = 0; j < pid.GetCount(); j++) {
				int p = sobr.sobr_map.Find(pid[j]);
				if(p >= 0)
					ext |= *(tbl.points.Add() = &sobr.sobr_map[p]);
			}
			if(!pid.IsEmpty())
				tbl.point_index.Add(tbl.points.GetCount());
		}
		tbl.extents[i] = ext;
		tbl.segment_index[i + 1] = tbl.point_index.GetCount() - 1;
	}
}
*/

/*
void VfkStream::ScanParKUMap()
{
	int pf = tables.Find("PARCELY");
	if(pf < 0)
		return;
	Table& par = tables[pf];
	if(!par.par_ku_map.IsEmpty())
		return;
	Data pdata = GetDataRaw(pf);
	int pid = par.columns.Find("BUD_ID");
	if(pid < 0)
		return;
	int kid = par.columns.Find("KATUZE_KOD");
	if(kid < 0)
		return;
	for(int i = 0, pc = pdata.GetCount(); i < pc; i++) {
		const Vector<Value>& row = pdata[i];
		par.par_ku_map.Add(row[pid], row[kid]);
	}
}
*/

/*
void VfkStream::ScanTelKUMap()
{
	int tf = tables.Find("TELESA");
	if(tf < 0)
		return;
	Table& tel = tables[tf];
	if(!tel.tel_ku_map.IsEmpty())
		return;
	Data tdata = GetDataRaw(tf);
	int tid = tel.columns.Find("ID");
	if(tid < 0)
		return;
	int kid = tel.columns.Find("KATUZE_KOD");
	if(kid < 0)
		return;
	for(int i = 0, tc = tdata.GetCount(); i < tc; i++) {
		const Vector<Value>& row = tdata[i];
		tel.tel_ku_map.Add(row[tid], row[kid]);
	}
}
*/

/*
void VfkStream::ScanKuIndex()
{
	int kf = tables.Find("KATASTR_UZEMI");
	if(kf < 0)
		return;
	Table& kat = tables[kf];
	if(!kat.ku_index.IsEmpty())
		return;
	Data tdata = GetDataRaw(kf);
	int kod = kat.columns.Find("KOD");
	if(kod < 0)
		return;
	int tc = tdata.GetCount();
	kat.ku_index.Reserve(tc);
	for(int i = 0; i < tc; i++)
		kat.ku_index.Add(tdata[i][kod]);
}
*/

/*
void VfkStream::ScanBudIndex()
{
	int bf = tables.Find("BUDOVY");
	if(bf < 0)
		return;
	Table& bud = tables[bf];
	if(!bud.bud_id_index.IsEmpty())
		return;
	Data tdata = GetDataRaw(bf);
	int bid = bud.columns.Find("ID");
	if(bid < 0)
		return;
	int tc = tdata.GetCount();
	bud.bud_id_index.Reserve(tc);
	for(int i = 0; i < tc; i++)
		bud.bud_id_index.Add(tdata[i][bid]);
}
*/

/*
void VfkStream::ScanObrBud()
{
	int of = tables.Find("OBRAZY_BUDOV");
	if(of < 0)
		return;
	Table& obr = tables[of];
	if(!obr.bud_id_ob.IsEmpty() || !obr.bud_id_zdb.IsEmpty())
		return;
	Data tdata = GetDataRaw(of);
	int bid = obr.columns.Find("BUD_ID");
	int sx = obr.columns.Find("SOURADNICE_X");
	int sy = obr.columns.Find("SOURADNICE_Y");
	int obt = obr.columns.Find("OBRBUD_TYPE");
	if(bid < 0 || sx < 0 || sy < 0 || obt < 0)
		return;
	int tc = tdata.GetCount();
	for(int i = 0; i < tc; i++) {
		const Vector<Value>& row = tdata[i];
		int budid = row[bid];
		Pointf pt(row[sy], row[sx]);
		String tp = row[obt];
		if(!IsNull(pt.x) && !IsNull(pt.y))
			if(tp == "ZDB")
				obr.bud_id_zdb.Add(budid, -pt);
			else if(tp == "OB")
				obr.bud_id_ob.GetAdd(budid).Add(-pt);
	}
}
*/

/*
void VfkStream::ScanObceIndex()
{
	int of = tables.Find("OBCE");
	if(of < 0)
		return;
	Table& obce = tables[of];
	if(!obce.obec_kod_index.IsEmpty())
		return;
	Data tdata = GetDataRaw(of);
	int kid = obce.columns.Find("KOD");
	if(kid < 0)
		return;
	int tc = tdata.GetCount();
	obce.obec_kod_index.Reserve(tc);
	for(int i = 0; i < tc; i++)
		obce.obec_kod_index.Add(tdata[i][kid]);
}
*/

/*
void VfkStream::ScanCastiObciMap()
{
	int cf = tables.Find("CASTI_OBCI");
	if(cf < 0)
		return;
	Table& cobce = tables[cf];
	if(!cobce.cobce_obec_map.IsEmpty())
		return;
	Data cdata = GetDataRaw(cf);
	int ck = cobce.columns.Find("KOD");
	if(ck < 0)
		return;
	int ok = cobce.columns.Find("OBCE_KOD");
	if(ok < 0)
		return;
	for(int i = 0, cc = cdata.GetCount(); i < cc; i++) {
		const Vector<Value>& row = cdata[i];
		cobce.cobce_obec_map.Add(row[ck], row[ok]);
	}
}
*/

/*
void VfkStream::ScanPAR()
{
	int pf = tables.Find("PARCELY");
	if(pf < 0)
		return;
	Table& par = tables[pf];
	if(par.scanned_sbp)
		return;
	par.scanned_sbp = true;
	Data pdata = GetDataRaw(pf);
	par.segment_index.SetCount(pdata.GetCount() + 1, 0);
	par.point_index.Add(0);
	par.extents.SetCount(pdata.GetCount(), Null);
	int pid = par.columns.Find("ID");
	if(pid < 0)
		return;
	int hf = tables.Find("HRANICE_PARCEL");
	if(hf < 0)
		return;
	Table& hp = tables[hf];
	int hp1 = hp.columns.Find("PAR_ID_1");
	int hp2 = hp.columns.Find("PAR_ID_2");
	if(hp1 < 0 || hp2 < 0)
		return;
	ScanSBP("HRANICE_PARCEL", "HP_ID");
	Index<double> hp1ix, hp2ix;
	Data hdata = GetDataRaw(hf);
	hp1ix.Reserve(hdata.GetCount());
	hp2ix.Reserve(hdata.GetCount());
	for(int i = 0, dc = hdata.GetCount(); i < dc; i++) {
		const Vector<Value>& row = hdata[i];
		hp1ix.Add(row[hp1]);
		hp2ix.Add(row[hp2]);
	}
	for(int i = 0, pc = pdata.GetCount(); i < pc; i++) {
		Rectf ext = Null;
		double id = pdata[i][pid];
		Index<const Pointf *, PtrHash> sb, se;
		Vector<int> sh;
		for(int i1 = hp1ix.Find(id); i1 >= 0; i1 = hp1ix.FindNext(i1)) {
			int bi = hp.point_index[hp.segment_index[i1]], li = hp.point_index[hp.segment_index[i1 + 1]] - 1;
			if(bi < li) {
				sh.Add(i1);
				sb.Add(hp.points[bi]);
				se.Add(hp.points[li]);
			}
		}
		for(int i2 = hp2ix.Find(id); i2 >= 0; i2 = hp2ix.FindNext(i2)) {
			int bi = hp.point_index[hp.segment_index[i2]], li = hp.point_index[hp.segment_index[i2 + 1]] - 1;
			if(bi < li) {
				sh.Add(i2);
				sb.Add(hp.points[bi]);
				se.Add(hp.points[li]);
			}
		}
		for(;;) {
			int f;
			for(f = 0; f < sh.GetCount() && sb.IsUnlinked(f); f++)
				;
			if(f >= sh.GetCount())
				break;
			int begin = par.points.GetCount();
			for(int bi = hp.point_index[hp.segment_index[sh[f]]],
				ei = hp.point_index[hp.segment_index[sh[f] + 1]]; bi < ei; bi++)
				ext |= *(par.points.Add() = hp.points[bi]);
			sb.Unlink(f);
			se.Unlink(f);
			const Pointf *close = sb[f];
			const Pointf *next = se[f];
			while(next != close) {
				bool direct = true;
				if((f = sb.Find(next)) >= 0)
					direct = true;
				else if((f = se.Find(next)) >= 0)
					direct = false;
				else {
					double d, b = 1e100;
					for(int x = 0; x < sh.GetCount(); x++)
						if(!sb.IsUnlinked(x)) {
							if((d = Distance(*sb[x], *next)) < b) {
								f = x;
								direct = true;
								b = d;
							}
							if((d = Distance(*se[x], *next)) < b) {
								f = x;
								direct = false;
								b = d;
							}
						}
					if(f < 0)
						break;
				}
				int bi = hp.point_index[hp.segment_index[sh[f]]];
				int ei = hp.point_index[hp.segment_index[sh[f] + 1]];
				if(direct) {
					if(hp.points[bi] != next)
						ext |= *(par.points.Add() = hp.points[bi]);
					while(++bi < ei)
						ext |= *(par.points.Add() = hp.points[bi]);
					next = hp.points[bi - 1];
				}
				else {
					if(hp.points[--ei] != next)
						ext |= *(par.points.Add() = hp.points[ei]);
					while(--ei >= bi)
						ext |= *(par.points.Add() = hp.points[ei]);
					next = hp.points[ei + 1];
				}
				sb.Unlink(f);
				se.Unlink(f);
			}
			if(par.points.GetCount() > begin + 1 && par.points.Top() == par.points[begin])
				par.points.Drop();
			par.point_index.Add(par.points.GetCount());
		}
		par.segment_index[i + 1] = par.point_index.GetCount() - 1;
		par.extents[i] = ext;
	}
}
*/
