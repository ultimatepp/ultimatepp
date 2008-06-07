// #define CDB_DEBUG // comment this macro to turn various error checking options off

NAMESPACE_UPP

#ifdef CDB_DEBUG
#define CHECK_INTEGRITY()        CheckIntegrity()
#define CHECK_AVAILABILITY(a, b) CheckAvailability(a, b)
#define CDB_ASSERT(x)            ASSERT(x)
#define CDB_CALL(x)              x
#else
#define CHECK_INTEGRITY()
#define CHECK_AVAILABILITY(a, b)
#define CDB_ASSERT(x)
#define CDB_CALL(x)
#endif

#define CRASH()                  __asm int 3
#define CDB_TIMING(n)            RTIMING(n)

typedef int foff_t;
typedef int dfid_t;

class DataFile : Moveable<DataFile>
{
public:
	typedef DataFile CLASSNAME;
	DataFile();
	~DataFile();

	bool                   Open(const char *filename, bool read_only = false);
	bool                   Create(const char *filename);
	bool                   Close();
	bool                   SaveAs(const char *new_file_name);

	bool                   IsReadOnly() const             { return read_only; }
	bool                   IsOpen() const                 { return stream.IsOpen(); }
	const String&          GetFileName() const            { return filename; }
	Time                   GetLastWriteTime() const;

	int64                  GetFileSize() const            { return stream.GetSize(); }
	int                    GetOverhead() const;
	bool                   Compact(const char *backup_file_name = 0);

	void                   Commit(dfid_t id);
	void                   Rollback(dfid_t id);

	void                   BeginBlock();
	void                   EndBlock();

	bool                   Import(Stream& stream);
	void                   Export(Stream& stream);

	void                   SetCache(int large_block_size, int small_cache_size, int large_cache_size);
	int                    GetLargeBlockSize() const      { return large_block_size; }
	int                    GetLargeCacheSize() const      { return large_cache.size; }
	int                    GetSmallCacheSize() const      { return small_cache.size; }

	int                    GetCount() const               { return index.GetCount(); }
	void                   SetCount(int count);
	bool                   IsValidIndex(int object) const { return object >= 0 && object < GetCount(); }

	int                    GetLength(int object) const    { return write_objects[object].length; }
	bool                   IsEmpty(int object) const      { return GetLength(object) == 0; }

	String                 Get(int object);
	String                 operator [] (int object)       { return Get(object); }

	void                   Set(int object, const String& data, dfid_t id);

	int                    Add(dfid_t id);
	void                   Remove(int object, dfid_t id);
	bool                   IsVoid(int object) const       { return write_objects[object].length < 0; }

	void                   CheckIntegrity();
	void                   CheckAvailability(foff_t start, int length);
	Vector<int>            FindObjects(foff_t start, int length);
	void                   DumpObjects(const Vector<int>& index, bool dump_data = false);

	static void            FileTest();

public:
	enum
	{
		LOG2_OFFSET_BYTES  = 2,
		OFFSET_BYTES       = 1 << (int)LOG2_OFFSET_BYTES,
		LOG2_BLOCK_BYTES   = 12,
		BLOCK_BYTES        = 1 << (int)LOG2_BLOCK_BYTES,
		LOG2_BLOCK_OFFSETS = LOG2_BLOCK_BYTES - LOG2_OFFSET_BYTES,
		BLOCK_OFFSETS      = 1 << (int)LOG2_BLOCK_OFFSETS,
		LOG2_OBJECT_BYTES  = LOG2_OFFSET_BYTES + 1,
		OBJECT_BYTES       = 1 << (int)LOG2_OBJECT_BYTES,
		LOG2_BLOCK_OBJECTS = LOG2_BLOCK_BYTES - LOG2_OBJECT_BYTES,
		BLOCK_OBJECTS      = 1 << (int)LOG2_BLOCK_OBJECTS,

		MIN_VERSION        = 1,
		VERSION            = 1,
		MAX_VERSION        = 1,

		LOG2_GRANULARITY   = 2,
		GRANULARITY        = 1 << (int)LOG2_GRANULARITY,

		// defaults
		LARGE_BLOCK_SIZE   = 4 * BLOCK_BYTES,
		SMALL_CACHE_SIZE   = 1 << 20,
		LARGE_CACHE_SIZE   = 1 << 20,
	};

private:
	struct Object
	{
		Object(foff_t offset = 0, int length = -1) : offset(offset), length(length) {}

		bool        IsAlive() const { return offset && length > 0; }

		static int  PtrLessLength(const Object *object, int length) { return object -> length < length; }
		static int  LessOffset(const Object& object, foff_t offset) { return object.offset < offset; }
		static bool PtrLess(const Object *a, const Object *b)
		{ return (a -> length != b -> length ? a -> length < b -> length : a -> offset < b -> offset); }
		static bool OffsetLess(const Object& a, const Object& b)    { return a.offset < b.offset; }

		foff_t      offset;
		int         length;
	};

private:
	bool                   StreamHeader(bool write);
	bool                   ReadIndex();
	String                 ReadBlock(foff_t offset, int length);
	foff_t                 AllocatePart(int block_size);
	void                   FreePart(foff_t offset, int length);
	foff_t                 WriteBlock(const String& data);
	void                   EnumBlock(Array<Object>& dest, foff_t offset, int length);
	void                   FreeBlock(foff_t offset, int length);
	void                   FreeBlock(const Object& o)             { if(o.IsAlive()) FreeBlock(o.offset, o.length); }
	void                   FlushSmallCacheFor(dfid_t id);
	void                   FlushSmallCache()                      { FlushSmallCacheFor(0); }
	void                   FlushLargeCacheFor(dfid_t id);
	void                   FlushLargeCache()                      { FlushLargeCacheFor(0); }
	void                   FlushIndex();
	static int             GetBlockSize(int length);
	void                   Read(void *dest, foff_t offset, int length);
	void                   Write(const void *src, foff_t offset, int length);
	void                   ThrowRead() const;
	void                   ThrowWrite() const;
	void                   CreateFreeList();
	void                   ReorderFreeItem(int pos);
	void                   Clear();
	void                   CommitFile();

private:
	struct Header
	{
		Header();

		bool Serialize(Stream& stream, bool write);

		enum { TOTAL = 256 };

		char   tag[128];
		int    version;
		int    object_count;
		foff_t index_offset;
	};

	friend struct Header;

	class Cache
	{
	public:
		Cache(int size);

		void                   Clear();

		int                    Find(int object) const    { return data.Find(object); }
		const String&          operator [] (int i) const { return data[i]; }

		void                   Put(int object, const String& value, bool write);
		void                   Remove(int object);
		void                   RemoveAbove(int object);

	public:
		Callback               WhenFlush;

		VectorMap<int, String> data;
		Index<int>             dirty;
		int                    size;
		int                    total;
	};

	FileStream             stream;
	String                 filename;
	bool                   read_only;
	Header                 header;

	int                    large_block_size;

	Array<Object>          index;
	Index<dfid_t>          write_id;
	Index<int>             write_index;
	Array<Object>          write_objects;

	int                    block_level;
	Segtor<Object, 512>    free_upon_commit;

	Index<int>             free_objects;
	Array<Object>          free_list;
	Vector<Object *>       free_list_index;
	foff_t                 free_offset;

	mutable Cache          large_cache;
	mutable Cache          small_cache;
};

class ObjectExc : public Exc
{
public:
	ObjectExc(const String& s) { String::operator = (s); }
};

template <class T>
inline void CopyBackward(T *dst, const T *src, const T *lim) {
	while(src > lim)
		*--dst = *--src;
}

END_UPP_NAMESPACE
