#ifndef _coff_lib_h_
#define _coff_lib_h_

#ifdef PLATFORM_WIN32

class ArchiveJob
{
protected:
	class Object;
	friend class ArchiveJob::Object;

public:
	ArchiveJob();

	int            AddObject(String filename, String object_data, Time filetime);
	void           AddGlobal(int object, String symbol);

	void           SortGlobals();

	void           BuildLibrary(String libfile, bool keep_archive_backups);

protected:
	void           LoadFile(String file);
	void           LoadLibrary(String libfile);
	int            FindObject(String objname) const;
	String         TrimObjectName(String objname) const;

	void           WriteArchive(String libfile, bool keep_archive_backups);

	class Object
	{
	public:
		Object(ArchiveJob& archive, int index, String filename, String objdata, double filetime, int header_offset, bool newfile);

		void        ReadObject();

	public:
		ArchiveJob& archive;
		int         index;

		String      filename;
		String      trimmed_name;
		String      object_data;
		double      filetime;
		bool        newfile;

		int         longname_offset;
		int         header_offset;
	};

public:
	int            machine;
	bool           keep_object_paths;
	bool           use_existing_archive;
	bool           archive_must_exist;
	bool           trim_longname_objects;
	bool           skip_duplicates;
	bool           verbose;
	enum MODE { MODE_MSLIB, MODE_GNUAR };
	MODE           armode;

protected:
	Array<Object>  objects;
	Vector<String> object_names;

	Vector<int>    symbol_objects;
	Index<String>  symbol_names;

	String         longnames;
	String         symbolnames;

	int            first_index_offset;
	int            first_index_datasize;
	int            second_index_offset;
	int            second_index_datasize;
	int            longnames_offset;
	int            archive_length;

private:
	void           BuildLongNameIndex();
	void           BuildSymbolIndex();
	void           CalcArchiveOffsets();
	void           WriteIndex(byte *first, byte *second);
};

#endif

#endif
