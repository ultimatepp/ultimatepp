#ifndef _console_vfk2_vfkstrm_h_
#define _console_vfk2_vfkstrm_h_

#include <gis3/gis3.h>

class VfkStream : public RefBase
{
public:
	struct Column {
		Column(String name = Null, int vtype = ERROR_V, int width = 0) : name(name), vtype(vtype), width(width) {}

		String name;
		int    vtype;
		int    width;
	};

	struct TableDeep {
		TableDeep() : header(false), row_offsets_xpos(0), row_count(-1) /*, scanned_sbp(false), scanned_sobr(false)*/ {}

		String name, rawname;
		bool header;
		int64 row_offsets_xpos;
		int64 row_count;
//		bool scanned_sbp;
//		bool scanned_sobr;
	};

	struct Table : TableDeep, DeepCopyOption<Table> {
		Table() {}
		Table(const Table& t, int deep)
		: TableDeep(t), columns(t.columns, deep)
		, file_index(t.file_index, deep)
		, begin_offset(t.begin_offset, deep)
		, end_offset(t.end_offset, deep) {}

		bool IsScanned() const          { return row_count >= 0; }
		void AddColumn(const Column& c) { columns.Add(c.name, c); }
//		void FlushCache();

		int  SizeOfInstance() const;

		Index<int> file_index;
		Vector<int64> begin_offset;
		Vector<int64> end_offset;
		ArrayMap<String, Column> columns;
/*
		Segtor<int, 256> row_offsets;
		Segtor<int, 256> segment_index;
		Segtor<int, 256> point_index;
		Segtor<Rectf, 256> extents;
		Segtor<const Pointf *, 256> points;
		ArrayMap<double, Pointf> sobr_map;
		VectorMap<int, int> par_ku_map;
		VectorMap<int, int> cobce_obec_map;
		VectorMap<int, int> tel_ku_map;
		VectorMap<int, Pointf> bud_id_zdb;
		VectorMap< int, Vector<Pointf> > bud_id_ob;
		Index<int> ku_index;
		Index<int> bud_id_index;
		Index<int> obec_kod_index;
*/
	};

	enum { HDR_ID, HDR_ORD, HDR_STR, HDR_NUM, HDR_DTM, HDR_COUNT };

	class Data
	{
		friend class VfkStream;

	public:
		Data() : vfk(0), table(0) {}
		Data(VfkStream *vfk, Table *table);

		bool                  IsOpen() const                { return !!vfk; }
		String                TableName() const             { return table->name; }
		String                RawName() const               { return table->rawname; }

		int64                 GetCount() const;
		const Vector<Value>&  operator [] (int64 row) const;
		GisObject             GetPath(int64 row, double resolution) const;
		GisObject             GetArea(int64 row, double resolution) const;

		int                   GetColumnCount() const        { return table->columns.GetCount(); }
		const Column&         GetColumn(int i) const        { return table->columns[i]; }
		int                   FindColumn(String name) const { return table->columns.Find(name); }

	public:
		VfkStream            *vfk;
		Table                *table;
		bool                  header;
		int                   gmask;
		mutable int64         row_index;
		mutable Vector<Value> row_buffer;
	};

public:
	typedef VfkStream CLASSNAME;
	VfkStream();
	~VfkStream() { Close(); }

	void        Open(const VectorMap<int, String>& file_groups);
	void        Close();
	bool        IsOpen() const              { return !streams.IsEmpty(); }
	int         SizeOfInstance() const;
//	int         MappingSizeOfInstance() const;

	const VectorMap<int, String>& GetFileGroups() const { return file_groups; }
	Vector<String> GetTables() const;
	Table&      GetTable(String name)       { return tables.Get(name); }
	Table      *FindTable(String name)      { int f = tables.Find(name); return f >= 0 ? &tables[f] : NULL; }

	void        Dump();
	void        SpeedTest();

	bool        HasTable(String table) const { return tables.Find(table) >= 0; }
	Data        GetData(String table);

	void        ScanSOBR();
	void        ScanSBP(String table, String sbp_id);
	void        ScanPAR();
	void        ScanParKUMap();
	void        ScanCastiObciMap();
	void        ScanTelKUMap();
	void        ScanKuIndex();
	void        ScanBudIndex();
	void        ScanObceIndex();
	void        ScanObrBud();

//	void        FlushCache();

private:
	Data        GetDataRaw(int n);
	void        Scan();
	void        ScanFile(int fx);
	void        ScanTable(Table& table);
	WString     ReadString(const char *b, const char **endptr) const;
	bool        IsDateTime(String field) const;
	void        ScanHeader(const char *b, Table& table);
	void        SkipRow(Stream& strm);
//	void        ScanRow(const char *b, Table& table) const;
	void        ReadRow(const Table& table, int64 index, Vector<Value>& row);

private:
	VectorMap<int, String> file_groups;
	Array<FileIn> streams;
	String      indexfilename;
	FileStream  indexfile;
	byte        charset;
	const char *last_line;
	ArrayMap<String, Table> tables;
	VectorMap< String, Vector<Value> > header;
};

#endif
