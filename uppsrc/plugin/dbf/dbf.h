#ifndef __dbf_dbf__
#define __dbf_dbf__

#include <Core/Core.h>

namespace Upp {

#define DBFCOL(x) (#x, x)

int StrBool(const char* s);

class DbfStream
{
public:
	class Field : Moveable<Field>
	{
	public:
		Field(const char *name = 0, char type = 0, byte width = 0, byte decimal = 0);

		String       Format(Value value, byte charset) const;

		static Field Text(const char *name, int width);
		static Field Number(const char *name, int width = 19, int decimals = 10);
		static Field Integer(const char *name, int width = 10) { return Number(name, width, 0); }
		static Field Date(const char *name);
		static Field Logical(const char *name);

		int          GetType() const;
		void         Serialize(Stream& stream);

		int          offset;
		String       name;
		char         type;
		byte         width;
		byte         decimal;
	};

	DbfStream();
	DbfStream(const char* filename, bool write = false, byte charset = CHARSET_CP852, bool delete_share = false);
	~DbfStream() { Close(); }

	bool            Open(const char *filename, bool write = false, byte charset = CHARSET_CP852, bool delete_share = false);
	bool            Create(const char *filename, const Array<Field>& fields, byte charset = CHARSET_CP852, bool delete_share = false);
	bool            Close();

	static bool     Check(const char *filename, bool write = false);

	bool            IsOpen() const                           { return dbf.IsOpen(); }
	Stream&         GetDBF()                                 { return dbf; }
	Stream&         GetDBT()                                 { return dbt; }
	bool            IsReadOnly() const                       { return !(dbf.GetStyle() & STRM_WRITE); }
	bool            IsError() const                          { return dbf.IsError(); }
	Time            GetFileTime() const                      { return dbf.GetTime(); }
	int64           GetFileSize() const                      { return dbf.GetSize(); }

	int             GetFieldCount() const                    { return fields.GetCount(); }
	const Field&    GetField(int index) const                { return fields[index]; }
	int             GetFieldIndex(String field) const        { return fields.Find(field); }

	const ArrayMap<String, Field>& GetFields() const         { return fields; }

	void            SetRowCount(int rows);
	int             GetRowCount() const                      { return rows; }

	byte            GetCharset() const                       { return charset; }

	void            Seek(int row);
	int             GetPos() const                           { return row_index; }
	int             GetNextPos() const                       { return next_row_index; }

	bool            Fetch(int row); // true = valid, false = deleted
	bool            Fetch();        // true = end of file
	bool            IsValid() const                          { return record[0] == ' '; }
	bool            IsDeleted() const                        { return !IsValid(); }

	void            Flush();        // flush current record and record count

	Value           GetItem(int i) const;
	Value           GetItem(String column) const;

	void            PutItem(int i, Value value);
	void            PutItem(String column, Value value);

	Vector<Value>   FetchRow(int row);
	Vector<Value>   FetchRow();
	void            WriteRow(int row, const Vector<Value>& values);
	void            WriteRow(const Vector<Value>& values);
	void            WriteRow();

	Value           operator [] (int i) const                { return GetItem(i); }
	Value           operator [] (String column) const        { return GetItem(column); }

	String          ToString() const;
	void            DumpData(Stream& stream);

protected:
	Value           GetItemInt(int i) const;
	Value           GetItemDouble(int i) const;
	Value           GetItemString(int i) const;
	Value           GetItemDateShort(int i) const;
	Value           GetItemDateLong(int i) const;
	Value           GetItemLogical(int i) const;
	Value           GetItemMemoString(int i) const;
	Value           GetItemMemoBinary(int i) const;
	Value           GetItemMemo(int i, bool binary) const;
	Value           GetItemEmpty(int i) const;
	Value           GetItemDouble8(int i) const;
	Value           GetItemInt4(int i) const;
	Value           GetItemDateTime8(int i) const;

	void            SetCharset(byte charset);
	void            FlushRecord();
	bool            DoOpen(byte charset);
	bool            DoCreate(const Array<Field>& _fields, byte charset);
	bool            StreamHeader(bool full);
	void            FlushRow();
	void            FlushHeader();
	void            DoFields();

	FileStream      dbf;
	mutable FileStream dbt;
	mutable FileStream fpt;
	ArrayMap<String, Field> fields;
	struct FieldProc : Moveable<FieldProc>
	{
		FieldProc(Value (DbfStream::*proc)(int) const = NULL) : proc(proc) {}
		Value (DbfStream::*proc)(int) const;
	};

	Vector<FieldProc> field_read;
	Vector<byte>    record; // current record

	int             rows;
	int             data_offset;
	int             row_index; // current row
	int             next_row_index; // row to fetch next
	int             dbt_block_size; // memo block size
	int             fpt_block_size;
	byte            charset;
//	word            codepage_uni[256];
//	char            codepage_map[256];
//	char            codepage_rev[256];
//	bool            codepage_cv;
	bool            dirty;
	bool            dirty_header;
	bool            has_memo;
	byte            version;
};

}

#endif
