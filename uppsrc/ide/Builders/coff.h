#ifndef _console_coff_defs_h_
#define _console_coff_defs_h_

// compatibility COFF headers

#ifdef COMPILER_MSC
#pragma pack(push, 1)
#endif

const int  COFF_IMAGE_ARCHIVE_START_SIZE  = 8;
#define    COFF_IMAGE_ARCHIVE_START                  "!<arch>\n"
const char COFF_IMAGE_ARCHIVE_PAD = '\n';
const char COFF_IMAGE_ARCHIVE_END1 = '`';
const char COFF_IMAGE_ARCHIVE_END2 = '\n';

const int COFF_IMAGE_FILE_RELOCS_STRIPPED          = 0x0001;  // Relocation info stripped from file.
const int COFF_IMAGE_FILE_EXECUTABLE_IMAGE         = 0x0002;  // File is executable  (i.e. no unresolved externel references).
const int COFF_IMAGE_FILE_LINE_NUMS_STRIPPED       = 0x0004;  // Line nunbers stripped from file.
const int COFF_IMAGE_FILE_LOCAL_SYMS_STRIPPED      = 0x0008;  // Local symbols stripped from file.
const int COFF_IMAGE_FILE_AGGRESIVE_WS_TRIM        = 0x0010;  // Agressively trim working set
const int COFF_IMAGE_FILE_LARGE_ADDRESS_AWARE      = 0x0020;  // App can handle >2gb addresses
const int COFF_IMAGE_FILE_BYTES_REVERSED_LO        = 0x0080;  // Bytes of machine word are reversed.
const int COFF_IMAGE_FILE_32BIT_MACHINE            = 0x0100;  // 32 bit word machine.
const int COFF_IMAGE_FILE_DEBUG_STRIPPED           = 0x0200;  // Debugging info stripped from file in .DBG file
const int COFF_IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP  = 0x0400;  // If Image is on removable media, copy and run from the swap file.
const int COFF_IMAGE_FILE_NET_RUN_FROM_SWAP        = 0x0800;  // If Image is on Net, copy and run from the swap file.
const int COFF_IMAGE_FILE_SYSTEM                   = 0x1000;  // System File.
const int COFF_IMAGE_FILE_DLL                      = 0x2000;  // File is a DLL.
const int COFF_IMAGE_FILE_UP_SYSTEM_ONLY           = 0x4000;  // File should only be run on a UP machine
const int COFF_IMAGE_FILE_BYTES_REVERSED_HI        = 0x8000;  // Bytes of machine word are reversed.

const int COFF_IMAGE_FILE_MACHINE_UNKNOWN   = 0;
const int COFF_IMAGE_FILE_MACHINE_I386      = 0x014c;  // Intel 386.
const int COFF_IMAGE_FILE_MACHINE_R3000     = 0x0162;  // MIPS little-endian; 0x160 big-endian
const int COFF_IMAGE_FILE_MACHINE_R4000     = 0x0166;  // MIPS little-endian
const int COFF_IMAGE_FILE_MACHINE_R10000    = 0x0168;  // MIPS little-endian
const int COFF_IMAGE_FILE_MACHINE_WCEMIPSV2 = 0x0169;  // MIPS little-endian WCE v2
const int COFF_IMAGE_FILE_MACHINE_ALPHA     = 0x0184;  // Alpha_AXP
const int COFF_IMAGE_FILE_MACHINE_POWERPC   = 0x01F0;  // IBM PowerPC Little-Endian
const int COFF_IMAGE_FILE_MACHINE_SH3       = 0x01a2;  // SH3 little-endian
const int COFF_IMAGE_FILE_MACHINE_SH3E      = 0x01a4;  // SH3E little-endian
const int COFF_IMAGE_FILE_MACHINE_SH4       = 0x01a6;  // SH4 little-endian
const int COFF_IMAGE_FILE_MACHINE_ARM       = 0x01c0;  // ARM Little-Endian
const int COFF_IMAGE_FILE_MACHINE_THUMB     = 0x01c2;
const int COFF_IMAGE_FILE_MACHINE_IA64      = 0x0200;  // Intel 64
const int COFF_IMAGE_FILE_MACHINE_MIPS16    = 0x0266;  // MIPS
const int COFF_IMAGE_FILE_MACHINE_MIPSFPU   = 0x0366;  // MIPS
const int COFF_IMAGE_FILE_MACHINE_MIPSFPU16 = 0x0466;  // MIPS
const int COFF_IMAGE_FILE_MACHINE_ALPHA64   = 0x0284;  // ALPHA64
const int COFF_IMAGE_FILE_MACHINE_AXP64     = COFF_IMAGE_FILE_MACHINE_ALPHA64;

struct MachineInfo
{
	const char *name;
	int code;
};

const MachineInfo *COFFMachineList();
String             COFFMachineNames();
String             COFFMachineName(int code);
int                COFFMachineCode(String name);

struct COFF_IMAGE_SYMBOL
{
	union {
		byte    ShortName[8];
		struct {
			dword   Short;     // if 0, use LongName
			dword   Long;      // offset into string table
		} Name;
		byte  *LongName[2];
	} N;
	dword   Value;
	short   SectionNumber;
	word    Type;
	byte    StorageClass;
	byte    NumberOfAuxSymbols;
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct COFF_IMAGE_FILE_HEADER
{
	word    Machine;
	word    NumberOfSections;
	dword   TimeDateStamp;
	dword   PointerToSymbolTable;
	dword   NumberOfSymbols;
	word    SizeOfOptionalHeader;
	word    Characteristics;
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

const int COFF_IMAGE_NUMBEROF_DIRECTORY_ENTRIES = 16;

struct COFF_IMAGE_DATA_DIRECTORY
{
	dword   VirtualAddress;
	dword   Size;
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct COFF_IMAGE_OPTIONAL_HEADER32
{
	//
	// Standard fields.
	//

	word    Magic;
	byte    MajorLinkerVersion;
	byte    MinorLinkerVersion;
	dword   SizeOfCode;
	dword   SizeOfInitializedData;
	dword   SizeOfUninitializedData;
	dword   AddressOfEntryPoint;
	dword   BaseOfCode;
	dword   BaseOfData;

	//
	// NT additional fields.
	//

	dword   ImageBase;
	dword   SectionAlignment;
	dword   FileAlignment;
	word    MajorOperatingSystemVersion;
	word    MinorOperatingSystemVersion;
	word    MajorImageVersion;
	word    MinorImageVersion;
	word    MajorSubsystemVersion;
	word    MinorSubsystemVersion;
	dword   Win32VersionValue;
	dword   SizeOfImage;
	dword   SizeOfHeaders;
	dword   CheckSum;
	word    Subsystem;
	word    DllCharacteristics;
	dword   SizeOfStackReserve;
	dword   SizeOfStackCommit;
	dword   SizeOfHeapReserve;
	dword   SizeOfHeapCommit;
	dword   LoaderFlags;
	dword   NumberOfRvaAndSizes;
	COFF_IMAGE_DATA_DIRECTORY DataDirectory[COFF_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct COFF_IMPORT_OBJECT_HEADER
{
	word    Sig1;                       // Must be IMAGE_FILE_MACHINE_UNKNOWN
	word    Sig2;                       // Must be IMPORT_OBJECT_HDR_SIG2.
	word    Version;
	word    Machine;
	dword   TimeDateStamp;              // Time/date stamp
	dword   SizeOfData;                 // particularly useful for incremental links

	union {
		word    Ordinal;                // if grf & IMPORT_OBJECT_ORDINAL
		word    Hint;
	};

	word    Type : 2;                   // IMPORT_TYPE
	word    NameType : 3;               // IMPORT_NAME_TYPE
	word    Reserved : 11;              // Reserved. Must be zero.
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

enum COFF_IMPORT_TYPE
{
	COFF_IMPORT_CODE = 0,
	COFF_IMPORT_DATA = 1,
	COFF_IMPORT_CONST = 2,
};

enum COFF_IMPORT_OBJECT_NAME_TYPE
{
	COFF_IMPORT_OBJECT_ORDINAL = 0,          // Import by ordinal
	COFF_IMPORT_OBJECT_NAME = 1,             // Import name == public symbol name.
	COFF_IMPORT_OBJECT_NAME_NO_PREFIX = 2,   // Import name == public symbol name skipping leading ?, @, or optionally _.
	COFF_IMPORT_OBJECT_NAME_UNDECORATE = 3,  // Import name == public symbol name skipping leading ?, @, or optionally _
	                                    // and truncating at first @
};

struct COFF_IMAGE_EXPORT_DIRECTORY_TABLE
{
	dword       ExportFlags;
	dword       DateTimeStamp;
	word        MajorVersion;
	word        MinorVersion;
	dword       NameRVA;
	dword       OrdinalBase;
	dword       AddressTableEntries;
	dword       NumberOfNamePointers;
	dword       ExportAddressTableRVA;
	dword       NamePointerTableRVA;
	dword       OrdinalTableRVA;
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

const size_t COFF_IMAGE_SIZEOF_SHORT_NAME = 8;

struct COFF_IMAGE_SECTION_HEADER
{
	byte    Name[COFF_IMAGE_SIZEOF_SHORT_NAME];
	union {
			dword   PhysicalAddress;
			dword   VirtualSize;
	} Misc;
	dword   VirtualAddress;
	dword   SizeOfRawData;
	dword   PointerToRawData;
	dword   PointerToRelocations;
	dword   PointerToLinenumbers;
	word    NumberOfRelocations;
	word    NumberOfLinenumbers;
	dword   Characteristics;
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

const dword COFF_IMAGE_SCN_TYPE_REG                  = 0x00000000;  // Reserved.
const dword COFF_IMAGE_SCN_TYPE_DSECT                = 0x00000001;  // Reserved.
const dword COFF_IMAGE_SCN_TYPE_NOLOAD               = 0x00000002;  // Reserved.
const dword COFF_IMAGE_SCN_TYPE_GROUP                = 0x00000004;  // Reserved.
const dword COFF_IMAGE_SCN_TYPE_NO_PAD               = 0x00000008;  // Reserved.
const dword COFF_IMAGE_SCN_TYPE_COPY                 = 0x00000010;  // Reserved.

const dword COFF_IMAGE_SCN_CNT_CODE                  = 0x00000020;  // Section contains code.
const dword COFF_IMAGE_SCN_CNT_INITIALIZED_DATA      = 0x00000040;  // Section contains initialized data.
const dword COFF_IMAGE_SCN_CNT_UNINITIALIZED_DATA    = 0x00000080;  // Section contains uninitialized data.

const dword COFF_IMAGE_SCN_LNK_OTHER                 = 0x00000100;  // Reserved.
const dword COFF_IMAGE_SCN_LNK_INFO                  = 0x00000200;  // Section contains comments or some other type of information.
const dword COFF_IMAGE_SCN_TYPE_OVER                 = 0x00000400;  // Reserved.
const dword COFF_IMAGE_SCN_LNK_REMOVE                = 0x00000800;  // Section contents will not become part of image.
const dword COFF_IMAGE_SCN_LNK_COMDAT                = 0x00001000;  // Section contents comdat.
//                                            = 0x00002000;  // Reserved.
//const dword COFF_IMAGE_SCN_MEM_PROTECTED - Obsolete=   0x00004000
const dword COFF_IMAGE_SCN_NO_DEFER_SPEC_EXC         = 0x00004000;  // Reset speculative exceptions handling bits in the TLB entries for this section.
const dword COFF_IMAGE_SCN_GPREL                     = 0x00008000;  // Section content can be accessed relative to GP
const dword COFF_IMAGE_SCN_MEM_FARDATA               = 0x00008000;
//const dword COFF_IMAGE_SCN_MEM_SYSHEAP  - Obsolete =   0x00010000
const dword COFF_IMAGE_SCN_MEM_PURGEABLE             = 0x00020000;
const dword COFF_IMAGE_SCN_MEM_16BIT                 = 0x00020000;
const dword COFF_IMAGE_SCN_MEM_LOCKED                = 0x00040000;
const dword COFF_IMAGE_SCN_MEM_PRELOAD               = 0x00080000;

const dword COFF_IMAGE_SCN_ALIGN_1BYTES              = 0x00100000;  //
const dword COFF_IMAGE_SCN_ALIGN_2BYTES              = 0x00200000;  //
const dword COFF_IMAGE_SCN_ALIGN_4BYTES              = 0x00300000;  //
const dword COFF_IMAGE_SCN_ALIGN_8BYTES              = 0x00400000;  //
const dword COFF_IMAGE_SCN_ALIGN_16BYTES             = 0x00500000;  // Default alignment if no others are specified.
const dword COFF_IMAGE_SCN_ALIGN_32BYTES             = 0x00600000;  //
const dword COFF_IMAGE_SCN_ALIGN_64BYTES             = 0x00700000;  //
const dword COFF_IMAGE_SCN_ALIGN_128BYTES            = 0x00800000;  //
const dword COFF_IMAGE_SCN_ALIGN_256BYTES            = 0x00900000;  //
const dword COFF_IMAGE_SCN_ALIGN_512BYTES            = 0x00A00000;  //
const dword COFF_IMAGE_SCN_ALIGN_1024BYTES           = 0x00B00000;  //
const dword COFF_IMAGE_SCN_ALIGN_2048BYTES           = 0x00C00000;  //
const dword COFF_IMAGE_SCN_ALIGN_4096BYTES           = 0x00D00000;  //
const dword COFF_IMAGE_SCN_ALIGN_8192BYTES           = 0x00E00000;  //

const dword COFF_IMAGE_SCN_ALIGN_MASK                = 0x00F00000;

// Unused                                     = 0x00F00000;

const dword COFF_IMAGE_SCN_LNK_NRELOC_OVFL           = 0x01000000;  // Section contains extended relocations.
const dword COFF_IMAGE_SCN_MEM_DISCARDABLE           = 0x02000000;  // Section can be discarded.
const dword COFF_IMAGE_SCN_MEM_NOT_CACHED            = 0x04000000;  // Section is not cachable.
const dword COFF_IMAGE_SCN_MEM_NOT_PAGED             = 0x08000000;  // Section is not pageable.
const dword COFF_IMAGE_SCN_MEM_SHARED                = 0x10000000;  // Section is shareable.
const dword COFF_IMAGE_SCN_MEM_EXECUTE               = 0x20000000;  // Section is executable.
const dword COFF_IMAGE_SCN_MEM_READ                  = 0x40000000;  // Section is readable.
const dword COFF_IMAGE_SCN_MEM_WRITE                 = 0x80000000;  // Section is writeable.

const int COFF_IMAGE_SYM_TYPE_NULL                 = 0x0000;  // no type.
const int COFF_IMAGE_SYM_TYPE_VOID                 = 0x0001;  //
const int COFF_IMAGE_SYM_TYPE_CHAR                 = 0x0002;  // type character.
const int COFF_IMAGE_SYM_TYPE_SHORT                = 0x0003;  // type short integer.
const int COFF_IMAGE_SYM_TYPE_INT                  = 0x0004;  //
const int COFF_IMAGE_SYM_TYPE_LONG                 = 0x0005;  //
const int COFF_IMAGE_SYM_TYPE_FLOAT                = 0x0006;  //
const int COFF_IMAGE_SYM_TYPE_DOUBLE               = 0x0007;  //
const int COFF_IMAGE_SYM_TYPE_STRUCT               = 0x0008;  //
const int COFF_IMAGE_SYM_TYPE_UNION                = 0x0009;  //
const int COFF_IMAGE_SYM_TYPE_ENUM                 = 0x000A;  // enumeration.
const int COFF_IMAGE_SYM_TYPE_MOE                  = 0x000B;  // member of enumeration.
const int COFF_IMAGE_SYM_TYPE_BYTE                 = 0x000C;  //
const int COFF_IMAGE_SYM_TYPE_WORD                 = 0x000D;  //
const int COFF_IMAGE_SYM_TYPE_UINT                 = 0x000E;  //
const int COFF_IMAGE_SYM_TYPE_DWORD                = 0x000F;  //
const int COFF_IMAGE_SYM_TYPE_PCODE                = 0x8000;  //
//
// Type (derived) values.
//

const int COFF_IMAGE_SYM_DTYPE_NULL                = 0;       // no derived type.
const int COFF_IMAGE_SYM_DTYPE_POINTER             = 1;       // pointer.
const int COFF_IMAGE_SYM_DTYPE_FUNCTION            = 2;       // function.
const int COFF_IMAGE_SYM_DTYPE_ARRAY               = 3;       // array.

//
// TLS Chaacteristic Flags
//
const int COFF_IMAGE_SCN_SCALE_INDEX               = 0x00000001; // Tls index is scaled

const int COFF_IMAGE_SYM_CLASS_END_OF_FUNCTION     = (byte )-1;
const int COFF_IMAGE_SYM_CLASS_NULL                = 0x0000;
const int COFF_IMAGE_SYM_CLASS_AUTOMATIC           = 0x0001;
const int COFF_IMAGE_SYM_CLASS_EXTERNAL            = 0x0002;
const int COFF_IMAGE_SYM_CLASS_STATIC              = 0x0003;
const int COFF_IMAGE_SYM_CLASS_REGISTER            = 0x0004;
const int COFF_IMAGE_SYM_CLASS_EXTERNAL_DEF        = 0x0005;
const int COFF_IMAGE_SYM_CLASS_LABEL               = 0x0006;
const int COFF_IMAGE_SYM_CLASS_UNDEFINED_LABEL     = 0x0007;
const int COFF_IMAGE_SYM_CLASS_MEMBER_OF_STRUCT    = 0x0008;
const int COFF_IMAGE_SYM_CLASS_ARGUMENT            = 0x0009;
const int COFF_IMAGE_SYM_CLASS_STRUCT_TAG          = 0x000A;
const int COFF_IMAGE_SYM_CLASS_MEMBER_OF_UNION     = 0x000B;
const int COFF_IMAGE_SYM_CLASS_UNION_TAG           = 0x000C;
const int COFF_IMAGE_SYM_CLASS_TYPE_DEFINITION     = 0x000D;
const int COFF_IMAGE_SYM_CLASS_UNDEFINED_STATIC    = 0x000E;
const int COFF_IMAGE_SYM_CLASS_ENUM_TAG            = 0x000F;
const int COFF_IMAGE_SYM_CLASS_MEMBER_OF_ENUM      = 0x0010;
const int COFF_IMAGE_SYM_CLASS_REGISTER_PARAM      = 0x0011;
const int COFF_IMAGE_SYM_CLASS_BIT_FIELD           = 0x0012;

const int COFF_IMAGE_SYM_CLASS_FAR_EXTERNAL        = 0x0044;  //

const int COFF_IMAGE_SYM_CLASS_BLOCK               = 0x0064;
const int COFF_IMAGE_SYM_CLASS_FUNCTION            = 0x0065;
const int COFF_IMAGE_SYM_CLASS_END_OF_STRUCT       = 0x0066;
const int COFF_IMAGE_SYM_CLASS_FILE                = 0x0067;
// new
const int COFF_IMAGE_SYM_CLASS_SECTION             = 0x0068;
const int COFF_IMAGE_SYM_CLASS_WEAK_EXTERNAL       = 0x0069;

const int COFF_IMAGE_SIZEOF_SYMBOL = 18;

union COFF_IMAGE_AUX_SYMBOL
{
	struct {
		dword    TagIndex;                      // struct, union, or enum tag index
		union {
			struct {
				word    Linenumber;             // declaration line number
				word    Size;                   // size of struct, union, or enum
			} LnSz;
			dword    TotalSize;
		} Misc;
		union {
			struct {                            // if ISFCN, tag, or .bb
				dword    PointerToLinenumber;
				dword    PointerToNextFunction;
			} Function;
			struct {                            // if ISARY, up to 4 dimen.
				word     Dimension[4];
			} Array_;
		} FcnAry;
		word    TvIndex;                        // tv index
	} Sym;
	struct {
		byte    Name[COFF_IMAGE_SIZEOF_SYMBOL];
	} File;
	struct {
		dword   Length;                         // section length
		word    NumberOfRelocations;            // number of relocation entries
		word    NumberOfLinenumbers;            // number of line numbers
		dword   CheckSum;                       // checksum for communal
		short   Number;                         // section number to associate with
		byte    Selection;                      // communal selection type
	} Section;
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

const int COFF_IMAGE_COMDAT_SELECT_NODUPLICATES   = 1;
const int COFF_IMAGE_COMDAT_SELECT_ANY            = 2;
const int COFF_IMAGE_COMDAT_SELECT_SAME_SIZE      = 3;
const int COFF_IMAGE_COMDAT_SELECT_EXACT_MATCH    = 4;
const int COFF_IMAGE_COMDAT_SELECT_ASSOCIATIVE    = 5;
const int COFF_IMAGE_COMDAT_SELECT_LARGEST        = 6;
const int COFF_IMAGE_COMDAT_SELECT_NEWEST         = 7;

struct COFF_IMAGE_RELOCATION : Moveable<COFF_IMAGE_RELOCATION>
{
	union {
		dword   VirtualAddress;
		dword   RelocCount;             // Set to the real count when IMAGE_SCN_LNK_NRELOC_OVFL is set
	};
	dword   SymbolTableIndex;
	word    Type;
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

const int COFF_IMAGE_SUBSYSTEM_DEFAULT             = -1;  // system-specific default (ULD-specific)

const int COFF_IMAGE_SUBSYSTEM_UNKNOWN             = 0;   // Unknown subsystem.
const int COFF_IMAGE_SUBSYSTEM_NATIVE              = 1;   // Image doesn't require a subsystem.
const int COFF_IMAGE_SUBSYSTEM_WINDOWS_GUI         = 2;   // Image runs in the Windows GUI subsystem.
const int COFF_IMAGE_SUBSYSTEM_WINDOWS_CUI         = 3;   // Image runs in the Windows character subsystem.
const int COFF_IMAGE_SUBSYSTEM_OS2_CUI             = 5;   // image runs in the OS/2 character subsystem.
const int COFF_IMAGE_SUBSYSTEM_POSIX_CUI           = 7;   // image runs in the Posix character subsystem.
const int COFF_IMAGE_SUBSYSTEM_NATIVE_WINDOWS      = 8;   // image is a native Win9x driver.
const int COFF_IMAGE_SUBSYSTEM_WINDOWS_CE_GUI      = 9;   // Image runs in the Windows CE subsystem.

struct COFF_IMAGE_ARCHIVE_MEMBER_HEADER
{
	byte     Name[16];                          // File member name - `/' terminated.
	byte     Date[12];                          // File member date - decimal.
	byte     UserID[6];                         // File member user id - decimal.
	byte     GroupID[6];                        // File member group id - decimal.
	byte     Mode[8];                           // File member mode - octal.
	byte     Size[10];                          // File member size - decimal.
	byte     EndHeader[2];                      // String to end header.
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct COFF_IMAGE_IMPORT_DESCRIPTOR
{
	union {
		dword   Characteristics;            // 0 for terminating null import descriptor
		dword   OriginalFirstThunk;         // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
	};
	dword   TimeDateStamp;                  // 0 if not bound,
	                                        // -1 if bound, and real date\time stamp
	                                        //     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
	                                        // O.W. date/time stamp of DLL bound to (Old BIND)

	dword   ForwarderChain;                 // -1 if no forwarders
	dword   Name;
	dword   FirstThunk;                     // RVA to IAT (if bound this IAT has actual addresses)
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

//
// I386 relocation types.
//
const word COFF_IMAGE_REL_I386_ABSOLUTE         = 0x0000;  // Reference is absolute, no relocation is necessary
const word COFF_IMAGE_REL_I386_DIR16            = 0x0001;  // Direct 16-bit reference to the symbols virtual address
const word COFF_IMAGE_REL_I386_REL16            = 0x0002;  // PC-relative 16-bit reference to the symbols virtual address
const word COFF_IMAGE_REL_I386_DIR32            = 0x0006;  // Direct 32-bit reference to the symbols virtual address
const word COFF_IMAGE_REL_I386_DIR32NB          = 0x0007;  // Direct 32-bit reference to the symbols virtual address, base not included
const word COFF_IMAGE_REL_I386_SEG12            = 0x0009;  // Direct 16-bit reference to the segment-selector bits of a 32-bit virtual address
const word COFF_IMAGE_REL_I386_SECTION          = 0x000A;
const word COFF_IMAGE_REL_I386_SECREL           = 0x000B;
const word COFF_IMAGE_REL_I386_REL32            = 0x0014;  // PC-relative 32-bit reference to the symbols virtual address

//
// MIPS relocation types.
//

const word COFF_IMAGE_REL_MIPS_ABSOLUTE         = 0x0000;  // Reference is absolute, no relocation is necessary
const word COFF_IMAGE_REL_MIPS_REFHALF          = 0x0001;
const word COFF_IMAGE_REL_MIPS_REFWORD          = 0x0002;
const word COFF_IMAGE_REL_MIPS_JMPADDR          = 0x0003;
const word COFF_IMAGE_REL_MIPS_REFHI            = 0x0004;
const word COFF_IMAGE_REL_MIPS_REFLO            = 0x0005;
const word COFF_IMAGE_REL_MIPS_GPREL            = 0x0006;
const word COFF_IMAGE_REL_MIPS_LITERAL          = 0x0007;
const word COFF_IMAGE_REL_MIPS_SECTION          = 0x000A;
const word COFF_IMAGE_REL_MIPS_SECREL           = 0x000B;
const word COFF_IMAGE_REL_MIPS_SECRELLO         = 0x000C;  // Low 16-bit section relative referemce (used for >32k TLS)
const word COFF_IMAGE_REL_MIPS_SECRELHI         = 0x000D;  // High 16-bit section relative reference (used for >32k TLS)
const word COFF_IMAGE_REL_MIPS_JMPADDR16        = 0x0010;
const word COFF_IMAGE_REL_MIPS_REFWORDNB        = 0x0022;
const word COFF_IMAGE_REL_MIPS_PAIR             = 0x0025;

//
// Alpha Relocation types.
//

const word COFF_IMAGE_REL_ALPHA_ABSOLUTE        = 0x0000;
const word COFF_IMAGE_REL_ALPHA_REFLONG         = 0x0001;
const word COFF_IMAGE_REL_ALPHA_REFQUAD         = 0x0002;
const word COFF_IMAGE_REL_ALPHA_GPREL32         = 0x0003;
const word COFF_IMAGE_REL_ALPHA_LITERAL         = 0x0004;
const word COFF_IMAGE_REL_ALPHA_LITUSE          = 0x0005;
const word COFF_IMAGE_REL_ALPHA_GPDISP          = 0x0006;
const word COFF_IMAGE_REL_ALPHA_BRADDR          = 0x0007;
const word COFF_IMAGE_REL_ALPHA_HINT            = 0x0008;
const word COFF_IMAGE_REL_ALPHA_INLINE_REFLONG  = 0x0009;
const word COFF_IMAGE_REL_ALPHA_REFHI           = 0x000A;
const word COFF_IMAGE_REL_ALPHA_REFLO           = 0x000B;
const word COFF_IMAGE_REL_ALPHA_PAIR            = 0x000C;
const word COFF_IMAGE_REL_ALPHA_MATCH           = 0x000D;
const word COFF_IMAGE_REL_ALPHA_SECTION         = 0x000E;
const word COFF_IMAGE_REL_ALPHA_SECREL          = 0x000F;
const word COFF_IMAGE_REL_ALPHA_REFLONGNB       = 0x0010;
const word COFF_IMAGE_REL_ALPHA_SECRELLO        = 0x0011;  // Low 16-bit section relative reference
const word COFF_IMAGE_REL_ALPHA_SECRELHI        = 0x0012;  // High 16-bit section relative reference
const word COFF_IMAGE_REL_ALPHA_REFQ3           = 0x0013;  // High 16 bits of 48 bit reference
const word COFF_IMAGE_REL_ALPHA_REFQ2           = 0x0014;  // Middle 16 bits of 48 bit reference
const word COFF_IMAGE_REL_ALPHA_REFQ1           = 0x0015;  // Low 16 bits of 48 bit reference
const word COFF_IMAGE_REL_ALPHA_GPRELLO         = 0x0016;  // Low 16-bit GP relative reference
const word COFF_IMAGE_REL_ALPHA_GPRELHI         = 0x0017;  // High 16-bit GP relative reference


//
// IBM PowerPC relocation types.
//

const word COFF_IMAGE_REL_PPC_ABSOLUTE          = 0x0000;  // NOP
const word COFF_IMAGE_REL_PPC_ADDR64            = 0x0001;  // 64-bit address
const word COFF_IMAGE_REL_PPC_ADDR32            = 0x0002;  // 32-bit address
const word COFF_IMAGE_REL_PPC_ADDR24            = 0x0003;  // 26-bit address, shifted left 2 (branch absolute)
const word COFF_IMAGE_REL_PPC_ADDR16            = 0x0004;  // 16-bit address
const word COFF_IMAGE_REL_PPC_ADDR14            = 0x0005;  // 16-bit address, shifted left 2 (load doubleword)
const word COFF_IMAGE_REL_PPC_REL24             = 0x0006;  // 26-bit PC-relative offset, shifted left 2 (branch relative)
const word COFF_IMAGE_REL_PPC_REL14             = 0x0007;  // 16-bit PC-relative offset, shifted left 2 (br cond relative)
const word COFF_IMAGE_REL_PPC_TOCREL16          = 0x0008;  // 16-bit offset from TOC base
const word COFF_IMAGE_REL_PPC_TOCREL14          = 0x0009;  // 16-bit offset from TOC base, shifted left 2 (load doubleword)

const word COFF_IMAGE_REL_PPC_ADDR32NB          = 0x000A;  // 32-bit addr w/o image base
const word COFF_IMAGE_REL_PPC_SECREL            = 0x000B;  // va of containing section (as in an image sectionhdr)
const word COFF_IMAGE_REL_PPC_SECTION           = 0x000C;  // sectionheader number
const word COFF_IMAGE_REL_PPC_IFGLUE            = 0x000D;  // substitute TOC restore instruction iff symbol is glue code
const word COFF_IMAGE_REL_PPC_IMGLUE            = 0x000E;  // symbol is glue code; virtual address is TOC restore instruction
const word COFF_IMAGE_REL_PPC_SECREL16          = 0x000F;  // va of containing section (limited to 16 bits)
const word COFF_IMAGE_REL_PPC_REFHI             = 0x0010;
const word COFF_IMAGE_REL_PPC_REFLO             = 0x0011;
const word COFF_IMAGE_REL_PPC_PAIR              = 0x0012;
const word COFF_IMAGE_REL_PPC_SECRELLO          = 0x0013;  // Low 16-bit section relative reference (used for >32k TLS)
const word COFF_IMAGE_REL_PPC_SECRELHI          = 0x0014;  // High 16-bit section relative reference (used for >32k TLS)
const word COFF_IMAGE_REL_PPC_GPREL             = 0x0015;

const word COFF_IMAGE_REL_PPC_TYPEMASK          = 0x00FF;  // mask to isolate above values in IMAGE_RELOCATION.Type

// Flag bits in IMAGE_RELOCATION.TYPE

const word COFF_IMAGE_REL_PPC_NEG               = 0x0100;  // subtract reloc value rather than adding it
const word COFF_IMAGE_REL_PPC_BRTAKEN           = 0x0200;  // fix branch prediction bit to predict branch taken
const word COFF_IMAGE_REL_PPC_BRNTAKEN          = 0x0400;  // fix branch prediction bit to predict branch not taken
const word COFF_IMAGE_REL_PPC_TOCDEFN           = 0x0800;  // toc slot defined in file (or, data in toc)

//
// Hitachi SH3 relocation types.
//
const word COFF_IMAGE_REL_SH3_ABSOLUTE          = 0x0000;  // No relocation
const word COFF_IMAGE_REL_SH3_DIRECT16          = 0x0001;  // 16 bit direct
const word COFF_IMAGE_REL_SH3_DIRECT32          = 0x0002;  // 32 bit direct
const word COFF_IMAGE_REL_SH3_DIRECT8           = 0x0003;  // 8 bit direct, -128..255
const word COFF_IMAGE_REL_SH3_DIRECT8_WORD      = 0x0004;  // 8 bit direct .W (0 ext.)
const word COFF_IMAGE_REL_SH3_DIRECT8_LONG      = 0x0005;  // 8 bit direct .L (0 ext.)
const word COFF_IMAGE_REL_SH3_DIRECT4           = 0x0006;  // 4 bit direct (0 ext.)
const word COFF_IMAGE_REL_SH3_DIRECT4_WORD      = 0x0007;  // 4 bit direct .W (0 ext.)
const word COFF_IMAGE_REL_SH3_DIRECT4_LONG      = 0x0008;  // 4 bit direct .L (0 ext.)
const word COFF_IMAGE_REL_SH3_PCREL8_WORD       = 0x0009;  // 8 bit PC relative .W
const word COFF_IMAGE_REL_SH3_PCREL8_LONG       = 0x000A;  // 8 bit PC relative .L
const word COFF_IMAGE_REL_SH3_PCREL12_WORD      = 0x000B;  // 12 LSB PC relative .W
const word COFF_IMAGE_REL_SH3_STARTOF_SECTION   = 0x000C;  // Start of EXE section
const word COFF_IMAGE_REL_SH3_SIZEOF_SECTION    = 0x000D;  // Size of EXE section
const word COFF_IMAGE_REL_SH3_SECTION           = 0x000E;  // Section table index
const word COFF_IMAGE_REL_SH3_SECREL            = 0x000F;  // Offset within section
const word COFF_IMAGE_REL_SH3_DIRECT32_NB       = 0x0010;  // 32 bit direct not based

const word COFF_IMAGE_REL_ARM_ABSOLUTE          = 0x0000;  // No relocation required
const word COFF_IMAGE_REL_ARM_ADDR32            = 0x0001;  // 32 bit address
const word COFF_IMAGE_REL_ARM_ADDR32NB          = 0x0002;  // 32 bit address w/o image base
const word COFF_IMAGE_REL_ARM_BRANCH24          = 0x0003;  // 24 bit offset << 2 & sign ext.
const word COFF_IMAGE_REL_ARM_BRANCH11          = 0x0004;  // Thumb: 2 11 bit offsets
const word COFF_IMAGE_REL_ARM_SECTION           = 0x000E;  // Section table index
const word COFF_IMAGE_REL_ARM_SECREL            = 0x000F;  // Offset within section

//
// IA64 relocation types.
//

const word COFF_IMAGE_REL_IA64_ABSOLUTE         = 0x0000;
const word COFF_IMAGE_REL_IA64_IMM14            = 0x0001;
const word COFF_IMAGE_REL_IA64_IMM22            = 0x0002;
const word COFF_IMAGE_REL_IA64_IMM64            = 0x0003;
const word COFF_IMAGE_REL_IA64_DIR32            = 0x0004;
const word COFF_IMAGE_REL_IA64_DIR64            = 0x0005;
const word COFF_IMAGE_REL_IA64_PCREL21B         = 0x0006;
const word COFF_IMAGE_REL_IA64_PCREL21M         = 0x0007;
const word COFF_IMAGE_REL_IA64_PCREL21F         = 0x0008;
const word COFF_IMAGE_REL_IA64_GPREL22          = 0x0009;
const word COFF_IMAGE_REL_IA64_LTOFF22          = 0x000A;
const word COFF_IMAGE_REL_IA64_SECTION          = 0x000B;
const word COFF_IMAGE_REL_IA64_SECREL22         = 0x000C;
const word COFF_IMAGE_REL_IA64_SECREL64I        = 0x000D;
const word COFF_IMAGE_REL_IA64_SECREL32         = 0x000E;
const word COFF_IMAGE_REL_IA64_LTOFF64          = 0x000F;
const word COFF_IMAGE_REL_IA64_DIR32NB          = 0x0010;
const word COFF_IMAGE_REL_IA64_RESERVED_11      = 0x0011;
const word COFF_IMAGE_REL_IA64_RESERVED_12      = 0x0012;
const word COFF_IMAGE_REL_IA64_RESERVED_13      = 0x0013;
const word COFF_IMAGE_REL_IA64_RESERVED_14      = 0x0014;
const word COFF_IMAGE_REL_IA64_RESERVED_15      = 0x0015;
const word COFF_IMAGE_REL_IA64_RESERVED_16      = 0x0016;
const word COFF_IMAGE_REL_IA64_ADDEND           = 0x001F;

const word COFF_IMAGE_REL_BASED_ABSOLUTE       = 0;
const word COFF_IMAGE_REL_BASED_HIGH           = 1;
const word COFF_IMAGE_REL_BASED_LOW            = 2;
const word COFF_IMAGE_REL_BASED_HIGHLOW        = 3;
const word COFF_IMAGE_REL_BASED_HIGHADJ        = 4;
const word COFF_IMAGE_REL_BASED_MIPS_JMPADDR   = 5;
const word COFF_IMAGE_REL_BASED_SECTION        = 6;
const word COFF_IMAGE_REL_BASED_REL32          = 7;

const word COFF_IMAGE_REL_BASED_MIPS_JMPADDR16 = 9;
const word COFF_IMAGE_REL_BASED_IA64_IMM64     = 9;
const word COFF_IMAGE_REL_BASED_DIR64          = 10;
const word COFF_IMAGE_REL_BASED_HIGH3ADJ       = 11;

const int COFF_IMAGE_DIRECTORY_ENTRY_EXPORT         =  0;   // Export Directory
const int COFF_IMAGE_DIRECTORY_ENTRY_IMPORT         =  1;   // Import Directory
const int COFF_IMAGE_DIRECTORY_ENTRY_RESOURCE       =  2;   // Resource Directory
const int COFF_IMAGE_DIRECTORY_ENTRY_EXCEPTION      =  3;   // Exception Directory
const int COFF_IMAGE_DIRECTORY_ENTRY_SECURITY       =  4;   // Security Directory
const int COFF_IMAGE_DIRECTORY_ENTRY_BASERELOC      =  5;   // Base Relocation Table
const int COFF_IMAGE_DIRECTORY_ENTRY_DEBUG          =  6;   // Debug Directory
//        COFF_IMAGE_DIRECTORY_ENTRY_COPYRIGHT      =  7;   // (X86 usage)
const int COFF_IMAGE_DIRECTORY_ENTRY_ARCHITECTURE   =  7;   // Architecture Specific Data
const int COFF_IMAGE_DIRECTORY_ENTRY_GLOBALPTR      =  8;   // RVA of GP
const int COFF_IMAGE_DIRECTORY_ENTRY_TLS            =  9;   // TLS Directory
const int COFF_IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    = 10;   // Load Configuration Directory
const int COFF_IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   = 11;   // Bound Import Directory in headers
const int COFF_IMAGE_DIRECTORY_ENTRY_IAT            = 12;   // Import Address Table
const int COFF_IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   = 13;   // Delay Load Import Descriptors
const int COFF_IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR = 14;   // COM Runtime descriptor

struct COFF_IMAGE_RESOURCE_DIRECTORY
{
	dword   Characteristics;
	dword   TimeDateStamp;
	word    MajorVersion;
	word    MinorVersion;
	word    NumberOfNamedEntries;
	word    NumberOfIdEntries;
//  IMAGE_RESOURCE_DIRECTORY_ENTRY DirectoryEntries[];
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct COFF_IMAGE_RESOURCE_DIRECTORY_ENTRY
{
	union {
		struct {
			dword NameOffset:31;
			dword NameIsString:1;
		};
		dword   Name;
		word    Id;
	};
	union {
		dword   OffsetToData;
		struct {
			dword   OffsetToDirectory:31;
			dword   DataIsDirectory:1;
		};
	};
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct COFF_IMAGE_RESOURCE_DATA_ENTRY
{
	dword   OffsetToData;
	dword   Size;
	dword   CodePage;
	dword   Reserved;
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

#define COFF_IMAGE_RESOURCE_NAME_IS_STRING        0x80000000
#define COFF_IMAGE_RESOURCE_DATA_IS_DIRECTORY     0x80000000

// GNU stab-specific information

/* Stabs entries use a 12 byte format:
     4 byte string table index
     1 byte stab type
     1 byte stab other field
     2 byte stab desc field
     4 byte stab value
   FIXME: This will have to change for a 64 bit object format.

   The stabs symbols are divided into compilation units.  For the
   first entry in each unit, the type of 0, the value is the length of
   the string table for this unit, and the desc field is the number of
   stabs symbols for this unit.  */

const int STAB_STRDXOFF = 0;
const int STAB_TYPEOFF  = 4;
const int STAB_OTHEROFF = 5;
const int STAB_DESCOFF  = 6;
const int STAB_VALOFF   = 8;
const int STAB_STABSIZE = 12;

struct STAB_INFO : Moveable<STAB_INFO>
{
	int  strdx;
	byte type;
	byte other;
	word desc;
	int  value;
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

/*
#define __define_stab(NAME, CODE, STRING) NAME=CODE,
#define __define_stab_duplicate(NAME, CODE, STRING) NAME=CODE,

enum STAB_DEBUG_CODE
{
#include "stab.def"
LAST_UNUSED_STAB_CODE
};

#undef __define_stab
*/

/* Definitions of "desc" field for N_SO stabs in Solaris2.  */

const int STAB_N_SO_AS      = 1;
const int STAB_N_SO_C       = 2;
const int STAB_N_SO_ANSI_C  = 3;
const int STAB_N_SO_CC      = 4; /* C++ */
const int STAB_N_SO_FORTRAN = 5;
const int STAB_N_SO_PASCAL  = 6;

/* Solaris2: Floating point type values in basic types.  */

const int STAB_NF_NONE      = 0;
const int STAB_NF_SINGLE    = 1; /* IEEE 32-bit */
const int STAB_NF_DOUBLE    = 2; /* IEEE 64-bit */
const int STAB_NF_COMPLEX   = 3; /* Fortran complex */
const int STAB_NF_COMPLEX16 = 4; /* Fortran double complex */
const int STAB_NF_COMPLEX32 = 5; /* Fortran complex*16 */
const int STAB_NF_LDOUBLE   = 6; /* Long double (whatever that is) */

#ifdef COMPILER_MSC
#pragma pack(pop)
#endif

String COFFSymbolName(const COFF_IMAGE_SYMBOL& sym, const char *strtbl);

#endif
