class FileMapping {
public:
	FileMapping(const char *file = NULL);
	~FileMapping() { Close(); }

#ifdef PLATFORM_WIN32
	bool        Open(const char *filename, dword mode = FileStream::READ, int64 filesize = 0);
#else
	bool        Open(const char *filename, dword mode = FileStream::READ, int64 filesize = 0, mode_t acm = 0644);
#endif

	bool        Create(const char *file, int64 filesize) { return Create(file, filesize, false); }

	int64       GetFileSize() const       { return filesize; }
	Time        GetTime() const;

	byte       *Map(int64 mapoffset, size_t maplen);
	byte       *Map()                     { ASSERT((size_t)GetFileSize() == GetFileSize()); return Map(0, (size_t)GetFileSize()); }
	bool        Unmap();
	bool        Close();

	bool        IsOpen() const            { return hfile != INVALID_HANDLE_VALUE; }

	int64       GetOffset() const         { return offset; }
	size_t      GetCount() const          { return size; }

	const byte *operator~() const         { ASSERT(IsOpen()); return base; }
	const byte *begin() const             { ASSERT(IsOpen()); return base; }
	const byte *end() const               { ASSERT(IsOpen()); return base + size; }
	const byte& operator [] (int i) const { ASSERT(IsOpen() && i >= 0 && (size_t)i < size); return base[i]; }

	byte       *operator ~ ()             { ASSERT(IsOpen()); return base; }
	byte       *begin()                   { ASSERT(IsOpen()); return base; }
	byte       *end()                     { ASSERT(IsOpen()); return base + size; }
	byte&       operator [] (int i)       { ASSERT(IsOpen() && i >= 0 && (size_t)i < size); return base[i]; }

// deprecated:
	bool        Create(const char *file, int64 filesize, bool delete_share);

	int64       GetRawOffset() const      { return rawoffset; }
	size_t      GetRawCount() const       { return rawsize; }

	const byte *Begin() const             { ASSERT(IsOpen()); return base; }
	const byte *End() const               { ASSERT(IsOpen()); return base + size; }
	const byte *GetIter(int i) const      { ASSERT(IsOpen() && i >= 0 && (size_t)i <= size); return base + i; }

	byte       *Begin()                   { ASSERT(IsOpen()); return base; }
	byte       *End()                     { ASSERT(IsOpen()); return base + size; }
	byte       *GetIter(int i)            { ASSERT(IsOpen() && i >= 0 && (size_t)i <= size); return base + i; }

private:
#ifdef PLATFORM_WIN32
	HANDLE      hfile;
	HANDLE      hmap;
#endif
#ifdef PLATFORM_POSIX
	enum { INVALID_HANDLE_VALUE = -1 };
	int	        hfile;
#endif
	byte       *base;
	byte       *rawbase;
	int64       filesize;
	int64       offset;
	int64       rawoffset;
	size_t      size;
	size_t      rawsize;
	bool        write;

	static int MappingGranularity();
};
