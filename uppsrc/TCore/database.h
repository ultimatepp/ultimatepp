NAMESPACE_UPP

#define CHECK_CACHE() // CheckCache()
#define CHECK_BLOCK(block, index) // CheckBlock(block, index)
#ifdef _DEBUG
//#define CHECK_INDEX // comment this line to turn index checking off
#endif

class DataClient
{
public:
	enum { DEFAULT_CACHE_SIZE = 100000 };

	DataClient();
	virtual ~DataClient();

	virtual void          Serialize(Stream& stream);

	void                  StreamMaster(Stream& stream, DataFile& df, dfid_t id);
	void                  Open(DataFile& datafile, int index, dfid_t id);
	int                   Create(DataFile& datafile, dfid_t id);
	void                  Close();
	virtual void          Unlink();

	bool                  IsOpen() const                               { return datafile; }
	DataFile&             GetDataFile() const                          { return *datafile; }
	int                   GetMasterIndex() const                       { return master_index; }
	bool                  IsReadOnly() const                           { return datafile -> IsReadOnly(); }
	dfid_t                GetID() const                                { return id; }

	virtual void          SetCacheSize(int size);
	int                   GetCacheSize() const                         { return cache_size; }
	virtual int           GetUsedCacheSize() const;
	virtual void          ShrinkCache();

	virtual void          Commit() = 0;
	virtual void          Rollback();

	static void           GetIndex(int *dest, int count, Stream& src);
	static void           SetIndex(Stream& dest, const int *src, int count);
	static void           GetIndex(Vector<int>& dest, Stream& src)       { GetIndex(dest.Begin(), dest.GetCount(), src); }
	static void           SetIndex(Stream& dest, const Vector<int>& src) { SetIndex(dest, src.Begin(), src.GetCount()); }

protected:
	enum
	{
		LOG2_INDEX_BYTES = 2,
		INDEX_BYTES      = 1 << LOG2_INDEX_BYTES,
	};

	void                  FlushMaster();

	virtual void          DoOpen();
	virtual void          DoCreate();
	virtual void          DoClose(); // NO-OP by default

protected:
	DataFile             *datafile;
	dfid_t                id;
	int                   master_index;
	int                   cache_size;
	int                   commit_index;
	mutable bool          master_dirty;
};

class DataColumn : public DataClient
{
public:
	DataColumn();
	virtual ~DataColumn();

	int                   GetCount() const                             { return row_count; }
	virtual void          SetCount(int count) = 0;

protected:
	int                   row_count;
};

class DataColumnFix : public DataColumn
{
public:
	DataColumnFix(int item_width);
	virtual ~DataColumnFix();

	virtual void          Unlink();
	virtual void          Serialize(Stream& stream);
	virtual void          Commit();
	virtual void          SetCount(int count);
	virtual int           GetUsedCacheSize() const;
	virtual void          ShrinkCache();

protected:
	const byte           *GetRaw(int index);
	void                  SetRaw(int index, const void *value);

private:
	virtual void          DoClose();

	void                  FlushCache();
	void                  ClearCache();
	void                  FetchCache(int block);

private:
	enum
	{
		ADD_STEP = 10,
	};

	struct Block
	{
		Block() : dirty(false), index(-1) {}

		int    index;
		bool   dirty;
		byte   buffer[DataFile::BLOCK_BYTES];
	};

	int                   item_width;
	unsigned              per_block;
	Vector<int>           block_index;
	ArrayMap<int, Block>  cache;
	int                   last_index;
	int                   last_index1;
	int                   last_index2;
	int                   last_index3;
	Block                *last_block;
	Block                *last_block1;
	Block                *last_block2;
	Block                *last_block3;
};

class DataColumnVar : public DataColumn
{
public:
	DataColumnVar();
	virtual ~DataColumnVar();

	virtual void          Unlink();
	virtual void          Serialize(Stream& stream);
	virtual void          Commit();
	virtual void          SetCount(int count);
	virtual int           GetUsedCacheSize() const;
	virtual void          ShrinkCache();

public:
	enum
	{
		SHORT_BYTES  = 2,
		MAX_BYTES    = DataFile::BLOCK_BYTES,
		INIT_BYTES   = MAX_BYTES / 2,
		JOIN_BYTES   = MAX_BYTES / 4,
		SINGLE_BYTES = MAX_BYTES / 4,

		ADD_STEP     = 100,
	};

protected:
	String                GetRaw(int index);
	void                  SetRaw(int index, String value);

private:
	struct Block
	{
		Block() : raw_dirty(false), total(SHORT_BYTES), offsets(SHORT_BYTES) {}

		int                    offsets;
		int                    total;
		bool                   raw_dirty;
		VectorMap<int, String> dirty;
		String                 buffer;
	};

	virtual void          DoClose();

	int                   FindBlock(int index) const;

	void                  FlushCache();
	void                  FlushCache(int index, int set_span = -1);
	void                  ClearCache();
	int                   FetchCache(int block);

	int                   GetDataLength(int index) const;
	bool                  CanJoin(int index, int length) const;
	void                  JoinBlock(int index, char *buffer, int offset, int count);
	int                   AddBlock(int index, char *buffer, int offset, int count);

	void                  AdjustLastIndex(int i)
	{
		if(last_index < 0 || i < index_index[last_index] || i >= index_index[last_index + 1])
			last_index = FindBlock(i);
	}

	void                  CheckCache() const;
	void                  CheckBlock(const Block& b, int index) const;

private:
	Vector<int>           index_index;
	Vector<int>           block_index;
	Index<int>            cache_index;
	Array<Block>          cache;
	int                   last_index;
	int                   current_size; // size of all cached objects
};

class DataRowIndex : public DataClient
{
public:
	DataRowIndex();

	virtual void          Unlink();
	virtual void          Serialize(Stream& stream);
	virtual void          Commit();
	virtual int           GetUsedCacheSize() const;
	virtual void          ShrinkCache();

	int                   GetCount() const                             { return exact_count; }
	int                   GetFirstEmpty() const                        { return first_empty; }
	int                   GetEndIndex() const                          { return end_index; }

	int                   Add();
	Vector<int>           Add(int count);
	void                  AddAt(int index);
	void                  Remove(int index);
	void                  Remove(const Vector<int>& index);

	Vector<int>           GetIndex();
	bool                  IsEmpty(int index);

private:
	struct Block
	{
		Block() : dirty(false) {}
		bool        dirty;
		Vector<int> data;

		int         FindIndex(int offset) const;
	};

	virtual void          DoClose();

	void                  FlushCache();
	void                  ClearCache();
	int                   FetchCache(int block);

	void                  CalcFirstEmpty();
	void                  CalcEndIndex();
	void                  CalcExactCount();

	void                  CheckBlock(const Block& block);
	void                  UnCollapse();

private:
	enum
	{
		LOG2_BLOCK_ITEMS = DataFile::LOG2_BLOCK_BYTES - LOG2_INDEX_BYTES - 1,
		BLOCK_ITEMS      = 1 << LOG2_BLOCK_ITEMS,
	};

	Vector<int>           block_index;
	ArrayMap<int, Block>  cache;
	int                   exact_count;
	int                   end_index;
	int                   first_empty;
	int                   current_size;
	bool                  collapsed;
};

class DataSource
{
public:
	DataSource()                                              {}
	virtual ~DataSource()                                     {}

	virtual void   SetRowIndex(DataRowIndex *index)           {}

	virtual Value  Get(int index) = 0;
	Value          operator [] (int index)                    { return Get(index); }

	virtual void   Add(int index, Value value)                { Set(index, value); }
	virtual void   Set(int index, Value value) = 0;
	virtual void   Remove(int index)                          { Set(index, Value()); }

	virtual void   CheckAdd(int index, Value value)           { CheckSet(index, value); }
	virtual void   CheckSet(int index, Value value)           {}

	virtual void   Commit()                                   {}
	virtual void   Rollback()                                 {}
	virtual int    GetUsedCacheSize() const                   { return 0; }
	virtual void   ShrinkCache()                              {}

	virtual String Describe() const = 0;
};

template <class C>
C& DataFetch(C& container, DataSource& source, const Vector<int>& index)
{
	typedef typename C::ValueType CVT;
	for(const int *p = index.Begin(), *e = index.End(); p != e; ++p)
		container.Add(CalcType<CVT>::ValueTo(source.Get(*p)));
	return container;
}

template <class C>
C DataFetch(DataSource& source, const Vector<int>& index, C * = 0)
{
	C container;
	return DataFetch(container, source, index);
}

template <class C, class L>
C& DataFetchSorted(C& container, DataSource& source, const Vector<int>& index, const L& sort, C * = 0)
{
	DataFetch(container, source, index);
	Sort(container, sort);
	return container;
}

template <class C, class L>
C DataFetchSorted(DataSource& source, const Vector<int>& index, const L& sort, C * = 0)
{
	C container;
	return DataFetchSorted(container, source, index);
}

class DataFilter : public DataSource
{
public:
	DataFilter() : source(0) {}

	void           SetSource(DataSource *_source)          { source = _source; }
	DataSource    *GetSource() const                       { return source; }

	virtual void   SetRowIndex(DataRowIndex *index)        { source -> SetRowIndex(index); }

	virtual Value  Get(int index)                          { ASSERT(source); return source -> Get(index); }
	virtual void   Add(int index, Value value)             { ASSERT(source); source -> Add(index, value); }
	virtual void   Set(int index, Value value)             { ASSERT(source); source -> Set(index, value); }
	virtual void   Remove(int index)                       { ASSERT(source); source -> Remove(index); }

	virtual void   CheckAdd(int index, Value value)        { ASSERT(source); source -> CheckAdd(index, value); }
	virtual void   CheckSet(int index, Value value)        { ASSERT(source); source -> CheckSet(index, value); }

	virtual void   Commit()                                { ASSERT(source); source -> Commit(); }
	virtual void   Rollback()                              { ASSERT(source); source -> Rollback(); }
	virtual int    GetUsedCacheSize() const                { ASSERT(source); return source->GetUsedCacheSize(); }
	virtual void   ShrinkCache()                           { ASSERT(source); source->ShrinkCache(); }

protected:
	DataSource    *source;
};

class DataConvertFilter : public DataFilter
{
public:
	DataConvertFilter(const Convert& convert = NoConvert()) : convert(&convert) {}

	void           SetConvert(const Convert& _convert)    { convert = &_convert; }
	const Convert& operator <<= (const Convert& _convert) { convert = &_convert; return _convert; }

	virtual Value  Get(int index);
	virtual void   Set(int index, Value value);

	virtual void   CheckSet(int index, Value value);

protected:
	const Convert *convert;
};

class DataMapFilter : public DataFilter
{
public:
	DataMapFilter()                                             {}

	void                SetMap(const Index<Value>& _map)        { map <<= _map; }
	const Index<Value>& operator <<= (const Index<Value>& _map) { return map <<= _map; }

	void                PickMap(Index<Value>& _map)             { map = _map; }
	void                operator |= (Index<Value>& _map)        { map = _map; }

	virtual void        CheckSet(int index, Value value);

protected:
	Index<Value>        map;
};

class DataMinMaxFilter : public DataFilter
{
public:
	DataMinMaxFilter(double min = Null, double max = Null, bool not_null = false)
		: min(min), max(max), not_null(not_null) {}

	void                MinMax(double _min, double _max) { min = _min; max = _max; }
	double              GetMin() const                   { return min; }
	double              GetMax() const                   { return max; }

	void                NotNull(bool _nn = true)         { not_null = true; }
	void                NoNotNull()                      { not_null = false; }
	bool                IsNotNull() const                { return not_null; }

	virtual void        CheckSet(int index, Value value);

protected:
	double              min, max;
	bool                not_null;
};

class DataOrderedIndexBase : public DataFilter, public DataClient
{
public:
	DataOrderedIndexBase(bool lazy);

	virtual void   Serialize(Stream& stream);
	virtual void   Add(int index, Value value);
	virtual void   Set(int index, Value value);
	virtual void   Remove(int index);
	virtual int    GetUsedCacheSize() const;
	virtual void   ShrinkCache();

#ifdef CHECK_INDEX
	virtual void   CheckIndex() = 0;
#endif
	virtual void   SetRowIndex(DataRowIndex *ri)      { dirty = true; row_index = ri; DataFilter::SetRowIndex(ri); }

	virtual void   Commit();
	virtual void   Rollback();

	Vector<int>    GetRange(Value min, Value max);
	Vector<int>    GetEqual(const Index<Value>& values);
	void           GetMinMax(Ref min, Ref max);
	int            GetFirstEqual(Value value, int exclude_row = -1);

	bool           IsEnabled() const                 { return !disabled; }
	void           Disable();
	void           Enable();

	virtual String Describe() const;

protected:
	virtual void   DoClose();

	virtual void   RefreshIndex() = 0;
	virtual int    FindIndex(int index, Value value) = 0;

protected:
	DataRowIndex  *row_index;
	bool           disabled;
	bool           lazy;
	bool           dirty;
	Vector<int>    ordered_index;
};

template <class T>
class DataOrderedIndex : public DataOrderedIndexBase
{
public:
	DataOrderedIndex(bool lazy) : DataOrderedIndexBase(lazy) {}

#ifdef CHECK_INDEX
	virtual void   CheckIndex();
#endif

private:
	virtual void   RefreshIndex();
	virtual int    FindIndex(int index, Value value);

private:
	struct LessSort
	{
		LessSort(const Vector<T>& values) : values(values) {}
		bool operator () (int i, int j) const
		{
			return values[i] < values[j] || values[i] == values[j] && i < j;
		}
		const Vector<T>& values;
	};

	struct LessFind
	{
		LessFind(DataSource *source, int rindex) : source(source), rindex(rindex) {}

		bool operator () (int index, const T& value) const
		{
			T t = CalcType<T>::ValueTo(source -> Get(index));
			return t < value || t == value && index < rindex;
		}

		DataSource *source;
		int         rindex;
	};
};

template <class T>
void DataOrderedIndex<T>::RefreshIndex()
{
	if(!disabled && dirty) {
		CDB_TIMING("DataOrderedIndex::RefreshIndex");
		int start = msecs();
		ASSERT(row_index);
		if(!lazy)
			master_dirty = !datafile -> IsReadOnly();
		Vector<int> all = row_index->GetIndex();
		Vector<T> values;
		ordered_index.SetCount(all.GetCount());
		values.SetCount(all.GetCount());
		for(int i = 0; i < ordered_index.GetCount(); i++) {
			ordered_index[i] = i;
			values[i] = CalcType<T>::ValueTo(source->Get(all[i]));
		}
		Sort(ordered_index, LessSort(values));
		for(int i = 0; i < ordered_index.GetCount(); i++)
			ordered_index[i] = all[ordered_index[i]];
		dirty = false;
		RLOG(msecs(start) << " msec / DataOrderedIndex<" << typeid(T).name() << ">::RefreshIndex(" << source->Describe() << ")");
	}
}

#ifdef CHECK_INDEX
template <class T>
void DataOrderedIndex<T>::CheckIndex()
{
	if(disabled)
		return;
	for(int i = 1; i < ordered_index.GetCount(); i++)
		if(LessFind(source, ordered_index[i - 1])(ordered_index[i], CalcType<T>::ValueTo(source->Get(ordered_index[i - 1]))))
		{
			String s;
			s << "CheckIndex error at " << i << "\n"
				"[" << (i - 1) << "] = " << StdFormat(Get(ordered_index[i - 1]))
				<< " (" << ordered_index[i - 1] << "), [" << i << "] = "
				<< StdFormat(Get(ordered_index[i])) << " (" << ordered_index[i] << ")\n";
			RLOG(s);
			NEVER();
		}
}
#endif

template <class T>
int DataOrderedIndex<T>::FindIndex(int index, Value value)
{
	return BinFindIndex(ordered_index, CalcType<T>::ValueTo(value), LessFind(source, index));
}

template <class T, int item_size, class C = T>
class DataColumnFixSimple : public DataColumnFix, public DataSource
{
public:
	typedef T ValueType;

	DataColumnFixSimple() : DataColumnFix(item_size) {}

	virtual void   Commit()                            { DataColumnFix::Commit(); }
	virtual void   Rollback()                          { DataColumnFix::Rollback(); }
	virtual int    GetUsedCacheSize() const            { return DataColumnFix::GetUsedCacheSize(); }
	virtual void   ShrinkCache()                       { DataColumnFix::ShrinkCache(); }

	virtual void   CheckSet(int index, Value v);

	virtual String Describe() const                    { return GetTypeName(typeid(T).name()); }
};

template <class T, int item_size, class C>
void DataColumnFixSimple<T, item_size, C>::CheckSet(int index, Value v)
{
	if(!IsNull(v) && !CalcType<C>::IsType(v))
		throw ObjectExc('<' + StdFormat(v) + ">: oèekávána hodnota typu '" + CalcType<C>::Describe() + '\'');
}

template <class T, class C = T>
class DataColumnVarSimple : public DataColumnVar, public DataSource
{
public:
	typedef T ValueType;

	virtual void   Commit()                            { DataColumnVar::Commit(); }
	virtual void   Rollback()                          { DataColumnVar::Rollback(); }
	virtual int    GetUsedCacheSize() const            { return DataColumnVar::GetUsedCacheSize(); }
	virtual void   ShrinkCache()                       { DataColumnVar::ShrinkCache(); }

	virtual void   CheckSet(int index, Value v);

	virtual String Describe() const                    { return GetTypeName(typeid(T).name()); }
};

template <class T, class C>
void DataColumnVarSimple<T, C>::CheckSet(int index, Value v)
{
	if(!IsNull(v) && !CalcType<C>::IsType(v))
		throw ObjectExc('<' + StdFormat(v) + ">: oèekávána hodnota typu '" + CalcType<C>::Describe() + '\'');
}

template <class T, class C = T>
class DataColumnFixOf : public DataColumnFixSimple<T, sizeof(T)>
{
public:
	virtual Value Get(int index)                     { return (C)*(const T *)this->GetRaw(index); }
	virtual void  Set(int index, Value value)        { T t = (C)value; this->SetRaw(index, &t); }
};

template <class T>
class DataColumnOf;

enum
{
	LOG2_BOOL_BLOCK_BITS = 5,
	BOOL_BLOCK_BITS = 1 << LOG2_BOOL_BLOCK_BITS,
	BOOL_BLOCK_BYTES = sizeof(int)
};

template <>
class DataColumnOf<bool> : public DataColumnFixSimple<bool, BOOL_BLOCK_BYTES>
{
public:
	virtual Value Get(int index);
	virtual void  Set(int index, Value value);

	virtual void  SetCount(int count);
};

template <> class DataColumnOf<byte>   : public DataColumnFixOf<byte, int> {};
template <> class DataColumnOf<short>  : public DataColumnFixOf<short, int> {};
template <> class DataColumnOf<int>    : public DataColumnFixOf<int> {};
template <> class DataColumnOf<double> : public DataColumnFixOf<double> {};

template <>
class DataColumnOf<String> : public DataColumnVarSimple<String>
{
public:
	virtual Value Get(int index)                     { return GetRaw(index); }
	virtual void  Set(int index, Value value)        { SetRaw(index, value); }
};

template <> class DataColumnOf<Date> : public DataColumnFixSimple<Date, 3>
{
public:
	virtual Value Get(int index);
	virtual void  Set(int index, Value value);
};

template <>
class DataColumnOf<Time> : public DataColumnFixSimple<Time, 5>
{
public:
	virtual Value Get(int index);
	virtual void  Set(int index, Value value);
};

template <>
class DataColumnOf<Value> : public DataColumnVarSimple<Value>
{
public:
	virtual Value Get(int index);
	virtual void  Set(int index, Value value);
};

class DataTable;
class DataLinkFilter;

struct DataSet : DeepCopyOption<DataSet>
{
	DataSet(String table = Null) : table(table) {}
	DataSet(String table, int i) : table(table) { set.SetCount(1, i); }
	DataSet(const DataSet& d, int deep) : table(d.table), set(d.set, deep) {}
	DataSet(String table, pick_ Vector<int>& set) : table(table), set(set) { Sort(); }
	DataSet(String table, pick_ Vector<int>& set, int deep) : table(table), set(set, deep) { Sort(); }

	DataSet& Sort();
	DataSet  Copy() const                  { return DataSet(*this, 0); }

	DataSet& operator &= (const DataSet& s);
	DataSet& operator |= (const DataSet& s);
	DataSet& operator -= (const DataSet& s);

	bool     IsEmpty() const               { return set.IsEmpty(); }
	int      GetCount() const              { return set.GetCount(); }
	int      operator [] (int index) const { return set[index]; }

	String table;
	Vector<int> set;
};

DataSet operator && (const DataSet& a, const DataSet& b);
DataSet operator || (const DataSet& a, const DataSet& b);
DataSet operator - (const DataSet& a, const DataSet& b);

class DataTableColumn : public DataClient
{
public:
	DataTableColumn();

	enum // special valuetypes
	{
		FIRST = -100,
		SHORT_V,
		BYTE_V,
		BOOL_V,
	};

	enum // flags
	{
		NUMBER_INDEX   = 0x0001,
		DATETIME_INDEX = 0x0002,
		STRING_INDEX   = 0x0004,
		INTEGER_INDEX  = 0x0008,
//		HASH_INDEX     = 0x0008,
//		ATOM_INDEX     = 0x0010,
	};

	DataTableColumn&  SetTable(DataTable *_owner)           { owner = _owner; return *this; }
	DataTable        *GetTable() const                      { return owner; }
	String            GetTableName() const;

	void              SetStyle(int vtype, int flags);
	int               GetType() const                       { return vtype; }
	int               GetFlags() const                      { return flags; }

	void              ModifyFlags(int flags);
	void              Link(DataTable& table);
	void              NoLink();

	DataTableColumn&  SetRowIndex(DataRowIndex *_rx)        { row_index = _rx; return *this; }
	DataRowIndex     *GetRowIndex() const                   { return row_index; }
	DataSet           GetIndex() const;

	DataTableColumn&  SetName(String _name)                 { column_name = _name; return *this; }
	String            GetName() const                       { return column_name; }

	String            GetDotName() const;

	virtual void      Unlink();
	virtual void      Commit();
	void              Serialize(Stream& stream);
	virtual int       GetUsedCacheSize() const;
	virtual void      ShrinkCache();

	DataSource&       GetSource()                           { ASSERT(source); return *source; }
	DataSource&       operator ~()                          { return GetSource(); }

	void              Set(int index, Value value);
	Value             Get(int index);
	Value             operator [] (int index);
	Vector<Value>     Get(const Vector<int>& rowset);
	Vector<int>       GetInt(const Vector<int>& rowset);

	DataSet           GetRange(Value min, Value max);
	DataSet           GetLike(WString mask);
	DataSet           GetEqual(Value value);
	DataSet           GetEqual(const Index<Value>& set);
	DataSet           GetEqual(const Vector<int>& set);
	int               GetFirstEqual(Value value, int exlude_row = -1);
	bool              Exists(Value value)            { return GetFirstEqual(value) >= 0; }
	void              GetMinMax(double& min, double& max);
	void              GetMinMax(Time& min, Time& max);

	DataSet           operator == (Value v)               { return GetEqual(v); }
	DataSet           operator == (const Index<Value>& v) { return GetEqual(v); }
	DataSet           operator == (const Vector<int>& v)  { return GetEqual(v); }
	DataSet           operator != (Value v);
	DataSet           operator != (const Index<Value>& v);
	DataSet           operator != (const Vector<int>& v);
	DataSet           operator >= (Value v)               { return GetRange(v, Null); }
	DataSet           operator <= (Value v)               { return GetRange(Null, v); }

	void              Describe(Stream& stream, int flags = 0);

public:
	DataTable        *owner;

	String            column_name;
	int               vtype;
	int               flags;

	DataRowIndex     *row_index;

	DataSource       *source;
	DataOrderedIndex<int>    *iindex;
	DataOrderedIndex<double> *nindex;
	DataOrderedIndex<Time>   *tindex;
	DataOrderedIndex<String> *sindex;
//	DataHashIndex    *hindex;
//	DataAtomIndex    *aindex;

	Array<DataFilter> filters;
	Array<DataFilter> indexes;
	One<DataLinkFilter> link;
	One<DataClient>   client;
	DataSource       *client_source;

protected:
	void              RefreshSource();
	virtual void      DoClose();

private:
	template <class T>
	void              IndexHelper(bool create, T *& index)
	{
		if(create) {
			if(!index) {
				filters.Add(index = new T(true));
				index->Create(*datafile, id);
				index->SetSource(&GetSource());
				index->SetRowIndex(row_index);
			}
		}
		else {
			if(index) {
				index->Unlink();
				int i = indexes.GetIndex(*index);
				if(i >= 0)
					indexes.Remove(i);
				index = 0;
			}
		}
		master_dirty = true;
	}
};

inline dword ValueTypeNo(const byte&)     { return DataTableColumn::BYTE_V; }
inline dword ValueTypeNo(const short&)    { return DataTableColumn::SHORT_V; }
//inline dword ValueTypeNo(const bool&)     { return DataTableColumn::BOOL_V; }

class DataTable : public DataClient
{
public:
	DataTable();
	virtual ~DataTable();

	void             Serialize(Stream& stream);
	virtual void     Unlink();
	virtual void     Commit();
	virtual void     Rollback();
	virtual int      GetUsedCacheSize() const;
	virtual void     ShrinkCache();

	int              Add(int index = -1);
	int              Add(const Vector<Value>& row, int index = -1);
	void             Set(int index, const Vector<Value>& row);
	void             Remove(int index);
	void             Remove(const DataSet& index);
	Vector<Value>    Get(int index);
	Vector<Value>    operator [] (int index)              { return Get(index); }
	Vector<Value>    GetAlways(int index); // returns empty row when index is invalid

	String           GetUserInfo() const                  { return user_info; }
	void             SetUserInfo(String u)                { user_info = u; master_dirty = true; }

	Vector<Value>    EmptyRow() const;

	bool             IsEmpty(int index)                   { return row_index.IsEmpty(index); }
	DataRowIndex&    GetRowIndex()                        { return row_index; }

	DataSet          GetIndex()                           { return DataSet(table_name, row_index.GetIndex()); }
	DataSet          operator ~ ()                        { return DataSet(table_name, row_index.GetIndex()); }

	int              GetCount() const                     { return row_index.GetCount(); }
	bool             IsEmpty() const                      { return row_index.GetCount() == 0; }

	DataTableColumn& Column(int index)                    { return columns[index]; }
	DataTableColumn& Column(String name);

	DataTableColumn& operator () (int index)              { return Column(index); }
	DataTableColumn& operator () (String name)            { return Column(name); }

	Value            operator () (int index, int row)     { return Column(index)[row]; }
	Value            operator () (String name, int row)   { return Column(name)[row]; }

	int              GetColumns() const                   { return columns.GetCount(); }
	String           GetName(int i) const                 { return columns.GetKey(i); }

	int              FindColumn(String name) const        { return columns.Find(name); }
	int              operator / (String name) const       { return columns.Find(name); }

	int              AddColumn(String name, int vtype, int flags = 0);
	void             RemoveColumn(String name);

	void             AddLink(DataLinkFilter& link);
	void             RemoveLink(DataLinkFilter& link);

	void             SetName(String _name)                { table_name = _name; }
	String           GetName() const                      { return table_name; }

	enum
	{
		DUMP_ROWS     = 0x00000001,
		EXPORT_FORMAT = 0x00000002,
	};

	void             Describe(Stream& stream, int flags = 0);
	void             Export(Stream& stream);
	String           Import(Stream& stream, bool import_id);

protected:
	String           table_name;
	DataRowIndex     row_index;
	ArrayMap<String, DataTableColumn> columns;
	Vector<DataLinkFilter *> links;
	String           user_info;

private:
	virtual void     DoClose();
};

class DataLinkFilter : public DataFilter
{
public:
	DataLinkFilter(DataTableColumn& column, DataTable& table);
	virtual ~DataLinkFilter();

	void                Kill() { table = 0; column = 0; }

	bool                Exists(int index)     { ASSERT(column); return column -> Exists(index); }
	String              GetColumnName() const { ASSERT(column); return column -> GetDotName(); }

	virtual void        CheckSet(int index, Value value);
	virtual String      Describe() const;

protected:
	DataTable          *table;
	DataTableColumn    *column;
};

class DataBase : public DataClient
{
public:
	DataBase();

	virtual void  Commit();
	virtual void  Rollback();
	virtual int   GetUsedCacheSize() const;
	virtual void  ShrinkCache();

	void          Serialize(Stream& stream);

	int           GetCount() const                 { return tables.GetCount(); }
	String        GetName(int i) const             { return tables.GetKey(i); }
	int           Find(String name) const          { return tables.Find(name); }
	DataTable&    Get(int i)                       { return tables[i]; }
	DataTable&    Get(String name);

	DataTable&    operator [] (int i)              { return tables[i]; }
	DataTable&    operator [] (String name)        { return Get(name); }

	bool          Exists(String tbl) const         { return Find(tbl) >= 0; }

	DataTable&    GetAdd(String name);
	DataTable&    Add(String name);
	void          Remove(int i);
	void          Remove(String name);
	bool          TryRemove(String name);

	void          Describe(Stream& stream, int flags = 0);

	void          BeginBlock()                     { block_level++; }
	void          EndBlock()                       { ASSERT(block_level > 0); block_level--; }
	bool          InBlock() const                  { return block_level > 0; }

	static void   AllocTest();
	static void   ColumnTest();
	static void   StructuredTest();

	static void   FullTest();

protected:
	virtual void  DoClose();

protected:
	ArrayMap<String, DataTable> tables;
	int           block_level;
};

END_UPP_NAMESPACE
