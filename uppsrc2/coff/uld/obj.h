#ifndef _console_uld_obj_h_
#define _console_uld_obj_h_

static inline int CoffSectionAlignShift(int flags)
{
	int v = (flags >> 20) & 15;
	return v ? v - 1 : 2;
}

static inline int CoffGetLengthAlignment(int align, int length)
{
/*
	for(int i = 1; i < align; i <<= 1)
		if(length & i)
			return i;
*/
	return align;
}

typedef int atom_t;

class LinkJob;

class ObjSec : Moveable<ObjSec>
{
public:
	ObjSec() {}
	ObjSec(const Nuller& nuller) : object(-1), section(0) {}
	ObjSec(int object, int section) : object(object), section(section) {}

	unsigned GetHashValue() const   { return CombineHash(::GetHashValue(object), ::GetHashValue(section)); }
	bool     IsNullInstance() const { return object < 0; }

public:
	int      object;
	int      section;
};

inline bool operator == (const ObjSec& a, const ObjSec& b) { return a.object == b.object && a.section == b.section; }
inline bool operator != (const ObjSec& a, const ObjSec& b) { return !(a == b); }

namespace Upp {
NTL_MOVEABLE(COFF_IMAGE_SYMBOL)
};

class ObjectFile
{
public:
	enum FILETYPE { DIRECT, FULL_PATH, LIB_PATH, DEFAULTLIB, INTERNAL, };
	ObjectFile(LinkJob& job, int index, String library_file, String object_file,
		FILETYPE filetype, int library_offset, Time file_time, int object_size);

	void       ReadFile(const byte *mapping);
	String     ToString() const;

	void       Dump() const;

public:
	enum SEC_TYPE { SEC_ANON_COMDAT, SEC_STD, SEC_STAB, SEC_STABSTR, /*SEC_INULL,*/ SEC_RAW, SEC_DEBUG };
	enum COM_STATE { COM_UNK, COM_SELECTED, COM_TRASHED };

	struct Section
	{
		Section() : name_atom(0), sec_atom(0), size(0), raw_size(0), offset(0), flags(0)
			, ref_sec_index(0), ref_ext_index(0), sec_map_index(-1) /*, comdat_forward(-1)*/
			, used(false), autocollect(false), comdat_state(COM_UNK), type(SEC_RAW), comdat_packing(0) {}

		int          name_atom;
		int          sec_atom;
		int          size;
		int          raw_size;
		int          offset;
		int          flags;
		int          ref_sec_index;
		int          ref_ext_index;
		int          sec_map_index;
//		int          comdat_forward;
		Vector<byte> section_data;
		bool         autocollect;
		bool         used;
		COM_STATE    comdat_state;
		SEC_TYPE     type;
		char         comdat_packing;
	};

	struct Import
	{
		Import() : app_atom(0), imp_atom(0), ordinal_hint(0) {}

		int    app_atom;
		int    imp_atom;
		int    ordinal_hint;
	};

public:
	LinkJob&        linkjob;
	int             index;
	int             archive_index;
	String          library_file;
	String          object_file;
	Time            file_time;
	int             object_size;
	String          object_data;
	int             library_offset;
	int             dll_atom;
	int             stab_index;
	int             stabstr_index;
	FILETYPE        filetype;
	bool            used_any;
	bool            collected;
	bool            has_ctors_dtors;
	bool            export_symbols;
//	String          directives;
	Vector<int>     ref_sections;
	Vector<int>     ref_externals;
	Array<Section>  sections;
	Array<Import>   imports;
	Index<int>      used_stubs;
	Index<int>      used_imports;
	enum
	{
		SEC_COMDAT,
		SEC_DLL_STUBS,
		SEC_DLL_NAMES,
		SEC_DLL_IMPORTS,
		SEC_DLL_BOUND,
		SEC_DLL_DESCRIPTOR,
//		SEC_DLL_NULL_DESC,
		DLL_SECTIONS
	};
	VectorMap<int, int> comdat_assoc;
};

class LinkJob
{
public:
	enum
	{
		COFF_IMAGE_SYM_TYPE_SPECIAL = 0xFF00,
		COFF_IMAGE_SYM_TYPE_SECTION,
		COFF_IMAGE_SYM_TYPE_IMPORT_NAME,
		COFF_IMAGE_SYM_TYPE_IMPORT_ORDINAL,
		COFF_IMAGE_SYM_TYPE_IMPORT_STUB,
		COFF_IMAGE_SYM_TYPE_BASE,
		COFF_IMAGE_SYM_TYPE_CTOR_DTOR,
	};
	enum SPECIAL_OBJECTS
	{
		OBJ_MARKER,
		OBJ_CTOR,
		OBJ_EXPORT,
		OBJ_FIRST, // first user object
	};
	enum
	{
		OBJM_COMDAT,
		OBJM_DATA_BEGIN,
		OBJM_PSEUDO_RELOC_BEGIN,
		OBJM_PSEUDO_RELOC_END,
		OBJM_BSS_BEGIN,
		OBJM_BSS_END,
		OBJM_COUNT,
	};
	enum
	{
		OBJC_COMDAT,
		OBJC_CTOR_BEGIN,
		OBJC_CTOR_END,
		OBJC_DTOR_BEGIN,
		OBJC_DTOR_END,
		OBJC_IDATA_HOLE,
		OBJC_IDATA_NULL,
		OBJC_COUNT
	};
	enum
	{
		OBJE_COMDAT,
		OBJE_DIRECTORY,
		OBJE_ADDRESS_TABLE,
		OBJE_NAME_POINTERS,
		OBJE_ORDINALS,
		OBJE_NAMES,
		OBJE_COUNT,
	};

	struct Symbol
	{
		Symbol(ObjSec obj_sec = Null, int value = 0, word type = 0, bool external = false)
			: obj_sec(obj_sec), value(value), type(type)
			, linked(false), used(false), relocated(false), external(external) {}

		ObjSec obj_sec;
		int    value;
		word   type;
		bool   linked    : 1;
		bool   used      : 1;
		bool   relocated : 1;
		bool   external  : 1;

		String ToString() const;
	};

	enum
	{
		GRP_CODE,
		GRP_RDATA,
		GRP_DATA,
		GRP_UDATA,
		GRP_EDATA,
		GRP_IDATA,
		GRP_RSRC,
		GRP_DEBUG,
		GRP_STAB,
		GRP_STABSTR,
		GRP_SYMBOLS,
		GRP_COUNT,

		GRP_NONE = -1,
		GRP_TEXT_BEGIN = GRP_CODE,
		GRP_DATA_BEGIN = GRP_RDATA,
		GRP_BSS_BEGIN  = GRP_UDATA,
		GRP_BSS_END    = GRP_UDATA + 1,
	};

	static String GetGroupName(int group_id);

	struct Group
	{
		int    first_section;
		int    rva;
		int    rfa;
		int    raw_size;
	};

	struct Section
	{
		Section() : obj_sec(Null), udata(false), group(0), app_section(0), sec_atom(0), rva(-1), rfa(-1), size(0) {}

		ObjSec obj_sec;
		byte   group;
		bool   udata;
		word   app_section;
		String name;
		int    sec_atom;
		int    rva;
		int    rfa;
		int    size;
	};

	struct Stab
	{
		Stab() : textoff(0), type(0), other(0), desc(0), value(0), /*fileatom(0),*/ fixup(Null) {}

		int         textoff;
		byte        type;
		byte        other;
		word        desc;
		int         value;
//		int         fileatom;
		ObjSec      fixup;

//		unsigned    GetHashValue() const;
//		bool        Equals(const Stab& b) const;

//		friend bool operator == (const Stab& a, const Stab& b) { return a.Equals(b); }
//		friend bool operator != (const Stab& a, const Stab& b) { return !a.Equals(b); }
	};

	struct Export
	{
		Export(String export_name, int ordinal) : export_name(export_name), ordinal(ordinal) {}

		String      export_name;
		int         ordinal;
	};

	struct Cache
	{
		FileMapping mapping;
		int         lock;
	};

/*
	struct Cache
	{
		String      library;
		String      file;
		Time        time;
		int         offset;
		int         size;

		void        Serialize(Stream& stream);
	};
*/

	struct AtomOrder
	{
		AtomOrder(const LinkJob& linkjob) : linkjob(linkjob), langinfo(GetLanguageInfo()) {}
		bool operator () (int a, int b) const { return langinfo(linkjob[a], linkjob[b]); }
		const LinkJob& linkjob;
		const LanguageInfo& langinfo;
	};

	struct GlobalAddressOrder
	{
		GlobalAddressOrder(const LinkJob& linkjob) : linkjob(linkjob), langinfo(GetLanguageInfo()) {}
		bool operator () (int a, int b) const;
		const LinkJob& linkjob;
		const LanguageInfo& langinfo;
	};

	static bool Less(const Section& a, const Section& b);

public:
	LinkJob();

	static String         Usage();

	void                  ReadCommand(const Vector<String>& cmdline, bool user);
	void                  ReadCommand(const char *cmdline, bool user);
	void                  ReadDefaultLibs();

	void                  Link();

	void                  LoadFile(String file, bool defaultlib);
	void                  LoadObject(String file, const FileMapping& mapping, ObjectFile::FILETYPE filetype);
	void                  LoadDLL(String file, const FileMapping& mapping, ObjectFile::FILETYPE filetype);
	void                  LoadLibrary(String file, const FileMapping& mapping, ObjectFile::FILETYPE filetype);

	void                  SetEntryPoint();
	void                  SetExportSymbols();
	void                  SetRootSymbols();
	void                  CollectSymbols();
	void                  CollectSectionSymbols(Index<ObjSec>& collect_objsec, int referer_index);
	void                  CollectObject(int index);
	void                  FixupAnonComdats();
//	void                  CheckDuplicates();
	void                  CollectImports();
	void                  CollectExports();
	void                  CollectSections();
	void                  PrepareImageHeader();
	void                  RelocateGlobals();
	void                  RelocateExports();
	void                  PrepareImageInfo();
	void                  WriteImageFile();
//	void                  OpenCacheFile();
//	void                  SaveCacheFile();
	void                  WriteImageSections();
	void                  RelocateImport(const ObjectFile& of);
	void                  RelocateObject(const ObjectFile& of, const byte *object_ptr);
	void                  RelocateInternal(const ObjectFile& of);
	void                  RelocateStabs(const ObjectFile& of, const byte *object_ptr);
	void                  CheckUnresolved();
	void                  PrepareSymbolTable();
	void                  PrepareStabBlocks();
	void                  PrepareFixupBlocks();
	void                  WriteMapFile();
	void                  WriteMapGlobals(String& map, const Vector<int>& used, String name);
	void                  BuildLibrary();
	void                  BuildDefFiles();

	atom_t                Atomize(String atom)            { return atoms.FindAdd(atom); }
	String                operator [] (atom_t atom) const { return atoms[atom]; }
	atom_t                NameAtom(const COFF_IMAGE_SYMBOL& isym, const char *strtbl) { return Atomize(COFFSymbolName(isym, strtbl)); }
	String                DemangleAtom(atom_t atom) const;
	FileMapping&          GetMapping(String filename);
	void                  LockMapping(String filename);
	void                  UnlockMapping(String filename);

	ObjectFile::Section&  GetSection(ObjSec os)             { return objects[os.object].sections[os.section]; }
	const ObjectFile::Section& GetSection(ObjSec os) const  { return objects[os.object].sections[os.section]; }
//	const ObjectFile::Section& GetSection0(ObjSec os) const { return objects[os.section ? os.object : 0].sections[os.section]; }
	String                FormatSection(ObjSec os) const;

	void                  AddExternal(atom_t atom, ObjSec obj_sec)  { ext_obj_sec.Add(atom, obj_sec); }
	void                  AddWeakExternal(atom_t atom, int refatom);
	void                  AddGlobal(atom_t atom, const Symbol& symbol);
	void                  AddCollect(atom_t atom)                   { collected_symbols.FindAdd(atom, -1); }

	bool                  AddCOFFSymbol(String name, int object, const COFF_IMAGE_SYMBOL& sym);
	void                  AddAuxCOFFSymbol(const COFF_IMAGE_SYMBOL& sym);

	void                  Dump() const;

public:
	enum
	{
		DUMP_IMPORT_LIBS    = 0x00000001,
		DUMP_OBJECT_LIBS    = 0x00000002,
		DUMP_COLLECTOR      = 0x00000004,
		DUMP_REF_FIXUPS     = 0x00000008,
		DUMP_SECTIONS       = 0x00000010,
		DUMP_SECTIONS_ALL   = 0x00000020,
		DUMP_SEC_DEFINES    = 0x00000040,
		DUMP_STAT           = 0x00000080,
		DUMP_IMPORT         = 0x00000100,
		DUMP_OBJECTS        = 0x00000200,
		DUMP_TIMING         = 0x00000400,
		DUMP_MAP_UNUSED     = 0x00000800,
		DUMP_STABS          = 0x00001000,
		DUMP_STAB_TYPES     = 0x00002000,
		DUMP_MAP_ALL        = 0x00004000,
		DUMP_EXPORTS        = 0x00008000,
		DUMP_DLL_EXPORTS    = 0x00010000,
		DUMP_COMMANDLINE    = 0x00020000,
		DUMP_ENVIRONMENT    = 0x00040000,
		DUMP_COMDAT_REMAPS  = 0x00080000,
	};
	int                   dump_flags;
	Vector<String>        libpaths;
	Index<String>         defaultlibs;
	Index<String>         nodefaultlibs;
	String                dll_search_prefix;
	int                   subsystem;
	int                   machine;
	String                outputfile;
	String                mapfile;
	bool                  write_mapfile;
	bool                  mapfile_stdout;
	bool                  write_xref;
	signed char           debug_info_raw;
	bool                  debug_info;
	bool                  show_logo;
	bool                  autocollect_crt_only;
	bool                  ignore_code_alignment;
	bool                  static_libraries;
	bool                  make_lib;
	bool                  make_dll;
	bool                  make_def;
	bool                  auto_dll_base;
//	bool                  cache_objects;
	bool                  cache_object_data;
	bool                  verbose;
	word                  major_version;
	word                  minor_version;
	word                  major_subsystem_version;
	word                  minor_subsystem_version;
	enum MODE { MODE_MSLINK, MODE_GNULD };
	MODE                  linkermode;
	int                   file_align;
	int                   image_align;
	int                   image_base;
	bool                  image_fixed;
	int                   stack_reserve;
	int                   stack_commit;
	int                   heap_reserve;
	int                   heap_commit;

	Index<String>         atoms;
	atom_t                text_atom;
	atom_t                data_atom;
	atom_t                edata_atom;
	atom_t                crt_atom;
	atom_t                ctors_atom;
	atom_t                dtors_atom;
	atom_t                gcc_except_atom;
	atom_t                CTOR_LIST_atom;
	atom_t                _CTOR_LIST_atom;
	atom_t                CTOR_LIST_END_atom;
	atom_t                DTOR_LIST_atom;
	atom_t                _DTOR_LIST_atom;
	atom_t                DTOR_LIST_END_atom;
	atom_t                idata_atom;
	atom_t                idata_idesc_atom;
	atom_t                idata_inull_atom;
	atom_t                idata_iat1_atom;
	atom_t                idata_iat2_atom;
	atom_t                idata_names_atom;
	atom_t                rdata_atom;
	atom_t                bss_atom;
	atom_t                debug_atom;
	atom_t                drectve_atom;
	atom_t                stab_atom;
	atom_t                stabstr_atom;
	atom_t                rsrc_atom;
//	int                   mangling_style;

	enum {
		I386_IMP_STUB_SIZE = 6,
		ARM_IMP_STUB_SIZE = 12,
		I386_IMP_ENTRY_SIZE = 4,
		ARM_IMP_ENTRY_SIZE = 4,
	};

//	Array<Cache>          object_cache;
//	Index<String>         cache_name_index;
//	Index<String>         cache_library_index;
//	FileStream            cache_file;
//	String                cache_file_name;
//	bool                  cache_dirty;

	Vector<String>        command_args;
	Vector<String>        user_command_args;
	Vector<String>        files_to_load;
	Index<String>         loaded_files;
	Array<ObjectFile>     objects;
	VectorMap<atom_t, int> dll_objects; // dll name -> index into objects
	Vector<atom_t>        used_dll_objects;
	int                   stab_section;
	int                   stabstr_section;
	int                   symbol_section;
	int                   reloc_section;
	int                   section_count;
	bool                  has_stabs;
//	bool                  has_symbols;
//	bool                  has_relocs;

	String                            entrypoint;
	atom_t                            entrypoint_atom;
	VectorMap<atom_t, int>            collected_symbols;
	VectorMap<atom_t, Index<atom_t> > collected_referer;
	VectorMap<atom_t, atom_t>         weak_externals;
	VectorMap<atom_t, atom_t>         section_merge;
	ArrayMap<atom_t, Symbol>          globals;
	Index<ObjSec>                     global_obj_sec_index;
	VectorMap<Point, int>             obj_static_global_index;
	VectorMap<atom_t, ObjSec>         ext_obj_sec;

	ArrayMap<String, Cache>           mapping_cache;
	int                               mapping_cache_limit;

	Segtor<Section, 256>              section_map;
	Index<int>                        section_object_index;
//	Index<ObjSec>                     section_obj_sec_index;
	VectorMap<atom_t, int>            section_name_map; // atom -> sec_map_index
	Group                             groups[GRP_COUNT + 1];
	Index<atom_t>                     unresolved;

	Vector<COFF_IMAGE_SYMBOL>         coff_output_symbols;
	String                            coff_output_symbol_strings;
//	Vector<int>                       coff_reloc_offsets;

	SegtorMap<atom_t, Export, 256>    exports;

	Vector<byte>                      output_image;
	Vector<byte>                      output_fixups;
	int                               output_fixup_size;
	VectorMap<String, int>            stab_bincl_map;
	Vector<byte>                      output_stab_info;
	Vector<char>                      output_stab_strings;
	VectorMap<unsigned, int>          output_stab_string_hash;
	Vector<int>                       highlow_fixup_rva;
	time_t                            timestamp;
	int                               ifhdr_pos;
	int                               iohdr_pos;
	int                               secmap_pos;
	int                               header_size;
	int                               code_rva;
	int                               code_rfa;
	int                               idesc_rva;
	int                               idesc_end_rva;
	int                               rsrc_rva;
	int                               rsrc_end_rva;
	int                               iat2_rva;
	int                               iat2_end_rva;
	int                               idata_last;
	bool                              idata_null_found;
	int                               start_time;
	String                            stub_filename;
	int                               imp_stub_size;
	int                               imp_entry_size;
};

#endif//_console_uld_obj_h_
