#include "binobj.h"
#include <plugin/z/z.h>
#include <plugin/bz2/bz2.h>

NAMESPACE_UPP

BinObjInfo::BinObjInfo()
{
}

void BinObjInfo::Parse(CParser& binscript, String base_dir)
{
	while(!binscript.IsEof()) {
		String binid = binscript.ReadId();
		bool ba = (binid == "BINARY_ARRAY");
		bool bm = (binid == "BINARY_MASK");
		if(binid == "BINARY" || ba || bm) {
			binscript.PassChar('(');
			Block blk;
			blk.scriptline = binscript.GetLine();
			blk.ident = binscript.ReadId();
			ArrayMap<int, Block>& brow = blocks.GetAdd(blk.ident);
			binscript.PassChar(',');
			blk.index = -1;
			if(ba) {
				blk.flags |= Block::FLG_ARRAY;
				blk.index = binscript.ReadInt();
				if(blk.index < 0 || blk.index > 1000000)
					binscript.ThrowError(NFormat("invalid array index: %d", blk.index));
				binscript.PassChar(',');
			}
			else if(bm) {
				blk.flags |= Block::FLG_MASK;
				blk.index = brow.GetCount();
			}
			String file = binscript.ReadString();
			if(binscript.Id("ZIP"))
				blk.encoding = Block::ENC_ZIP;
			else if(binscript.Id("BZ2"))
				blk.encoding = Block::ENC_BZ2;
			binscript.PassChar(')');
			FindFile ff;
			String searchpath = NormalizePath(file, base_dir);
			String searchdir = GetFileDirectory(searchpath);
			Vector<String> files;
			Vector<int64> lengths;
			if(ff.Search(searchpath))
				do
					if(ff.IsFile()) {
						files.Add(ff.GetName());
						lengths.Add(ff.GetLength());
					}
				while(ff.Next());
			if(files.IsEmpty())
				binscript.ThrowError(NFormat("'%s' not found or not a file", file));
			if(!(blk.flags & Block::FLG_MASK) && files.GetCount() > 1)
				binscript.ThrowError(NFormat("Multiple files found (e.g. %s, %s) in single file mode", files[0], files[1]));
			IndexSort(files, lengths);
			for(int i = 0; i < files.GetCount(); i++) {
				blk.file = AppendFileName(searchdir, files[i]);
				blk.length = (int)lengths[i];
				int f = brow.Find(blk.index);
				if(f >= 0)
					binscript.ThrowError(NFormat("%s[%d] already seen at line %d", blk.ident, blk.index, brow[f].scriptline));
				if(blk.index < 0 && !brow.IsEmpty() || blk.index >= 0 && !brow.IsEmpty() && brow[0].index < 0)
					binscript.ThrowError(NFormat("%s: mixing non-array and array elements", blk.ident));
				brow.Add(blk.index, blk);
				if(!(blk.flags & Block::FLG_MASK))
					break;
				blk.index++;
			}
		}
		else
			binscript.ThrowError("binary script item identifier expected");
	}
}

class BinObj : public BinObjInfo {
public:
	BinObj(Callback1<String> WhenConsole);

	void Run(String objectfile, CParser& binscript, String base_dir);

public:
	Callback1<String> WhenConsole;

private:
	void PrepareSymbolTable(String binfile);
	void PrepareMetaData();
	void FixAuxSymbols(int data_size);
	void WriteFile(String objectfile);

	int  AddSymbol(COFF_IMAGE_SYMBOL& sym, const char *name);
	int  AddMetaData(int value, int reloc_section = -1);

	static int Align(int len) { return (len + 3) & -4; }

private:
	Vector<byte> metadata;
//	int          data_size;
	Vector<byte> relocations;
	Vector<byte> symbol_table;
	Vector<byte> symbol_table_strtbl;

	enum {
		SCN_COMDAT,
		SCN_METADATA,
		SCN_DATA,
	};

	int          aux_metadata_scn;
	int          aux_data_scn;
	int          symtbl_metasec;
	int          symtbl_datasec;
};

static const char metadata_secn[] = ".data$BM";
static const char data_secn[] = ".data$BD";

BinObj::BinObj(Callback1<String> WhenConsole_)
: WhenConsole(WhenConsole_)
{
	symbol_table_strtbl.SetCountR(4);
}

void BinObj::Run(String objectfile, CParser& binscript, String base_dir)
{
	Parse(binscript, base_dir);
	PrepareSymbolTable(binscript.GetFileName());
	PrepareMetaData();
	WriteFile(objectfile);
}

void BinObj::PrepareMetaData()
{
//	data_size = 0;
	for(int i = 0; i < blocks.GetCount(); i++) {
		String ident;
		ident << '_' << blocks.GetKey(i);
		ArrayMap<int, Block>& belem = blocks[i];
		int flags = belem[0].flags;
		if(flags & (Block::FLG_ARRAY | Block::FLG_MASK)) {
			int count = Max(belem.GetKeys()) + 1;
			Vector<Block *> blockref;
			blockref.SetCount(count, 0);
			for(int a = 0; a < belem.GetCount(); a++) {
				Block& b = belem[a];
//				b.offset = data_size;
				blockref[b.index] = &b;
//				data_size += Align(b.length + 1);
			}
			COFF_IMAGE_SYMBOL countsym;
			Zero(countsym);
			countsym.Value = AddMetaData(count);
			countsym.SectionNumber = SCN_METADATA;
			countsym.StorageClass = COFF_IMAGE_SYM_CLASS_EXTERNAL;
			countsym.Type = 4;
			AddSymbol(countsym, ident + "_count");
			COFF_IMAGE_SYMBOL lensym;
			Zero(lensym);
			lensym.Value = metadata.GetCount();
			lensym.SectionNumber = SCN_METADATA;
			lensym.StorageClass = COFF_IMAGE_SYM_CLASS_EXTERNAL;
			lensym.Type = COFF_IMAGE_SYM_TYPE_INT + 256 * COFF_IMAGE_SYM_DTYPE_ARRAY;
			AddSymbol(lensym, ident + "_length");
			for(int a = 0; a < count; a++) {
				if(blockref[a])
					blockref[a]->len_meta_offset = metadata.GetCount();
				AddMetaData(-1);
			}
			COFF_IMAGE_SYMBOL ptrsym;
			Zero(ptrsym);
			ptrsym.Value = metadata.GetCount();
			ptrsym.SectionNumber = SCN_METADATA;
			ptrsym.StorageClass = COFF_IMAGE_SYM_CLASS_EXTERNAL;
			ptrsym.Type = COFF_IMAGE_SYM_TYPE_INT + 256 * COFF_IMAGE_SYM_DTYPE_ARRAY;
			AddSymbol(ptrsym, ident);
			for(int a = 0; a < count; a++) {
				if(blockref[a])
					blockref[a]->off_meta_offset = metadata.GetCount();
				AddMetaData(0, blockref[a] ? symtbl_datasec : 0);
			}
			if(flags & Block::FLG_MASK) {
				Vector<int> fn_offsets;
				fn_offsets.SetCount(count);
				for(int a = 0; a < count; a++) {
					String fn;
					if(blockref[a])
						fn = GetFileName(blockref[a]->file);
					int pos = metadata.GetCount();
					fn_offsets[a] = pos;
					metadata.SetCountR(pos + fn.GetLength() + 1);
					memcpy(&metadata[pos], fn, fn.GetLength() + 1);
				}
				metadata.SetCountR(Align(metadata.GetCount()), 0);
				COFF_IMAGE_SYMBOL filesym;
				Zero(filesym);
				filesym.Value = metadata.GetCount();
				filesym.SectionNumber = SCN_METADATA;
				filesym.StorageClass = COFF_IMAGE_SYM_CLASS_EXTERNAL;
				filesym.Type = COFF_IMAGE_SYM_TYPE_INT + 256 * COFF_IMAGE_SYM_DTYPE_ARRAY;
				AddSymbol(filesym, ident + "_files");
				for(int a = 0; a < count; a++)
					AddMetaData(fn_offsets[a], symtbl_metasec);
			}
		}
		else {
			Block& b = belem[0];
			COFF_IMAGE_SYMBOL lensym;
			Zero(lensym);
			b.len_meta_offset = metadata.GetCount();
			lensym.Value = AddMetaData(b.length);
			lensym.SectionNumber = SCN_METADATA;
			lensym.StorageClass = COFF_IMAGE_SYM_CLASS_EXTERNAL;
			lensym.Type = COFF_IMAGE_SYM_TYPE_INT;
			AddSymbol(lensym, ident + "_length");
			COFF_IMAGE_SYMBOL datasym;
			Zero(datasym);
			b.off_meta_offset = metadata.GetCount();
			datasym.Value = AddMetaData(0, symtbl_datasec);
			datasym.SectionNumber = SCN_METADATA;
			datasym.StorageClass = COFF_IMAGE_SYM_CLASS_EXTERNAL;
			datasym.Type = COFF_IMAGE_SYM_TYPE_INT;
			AddSymbol(datasym, ident);
		}
	}
}

void BinObj::PrepareSymbolTable(String binfile)
{
	{ // FILE
		COFF_IMAGE_SYMBOL filesym;
		Zero(filesym);
		filesym.SectionNumber = -2;
		filesym.StorageClass = COFF_IMAGE_SYM_CLASS_FILE;
		filesym.NumberOfAuxSymbols = int(binfile.GetLength() / sizeof(COFF_IMAGE_SYMBOL) + 1);
		memcpy(symbol_table.GetIter(AddSymbol(filesym, ".file")), binfile, binfile.GetLength() + 1);
	}

	{ // metadata section header
		symtbl_metasec = symbol_table.GetCount() / COFF_IMAGE_SIZEOF_SYMBOL;
		COFF_IMAGE_SYMBOL msecsym;
		Zero(msecsym);
		msecsym.SectionNumber = 1;
		msecsym.StorageClass = COFF_IMAGE_SYM_CLASS_STATIC;
		msecsym.NumberOfAuxSymbols = 1;
		aux_metadata_scn = AddSymbol(msecsym, metadata_secn);
	}

	{ // data section header
		symtbl_datasec = symbol_table.GetCount() / COFF_IMAGE_SIZEOF_SYMBOL;
		COFF_IMAGE_SYMBOL dsecsym;
		Zero(dsecsym);
		dsecsym.SectionNumber = 2;
		dsecsym.StorageClass = COFF_IMAGE_SYM_CLASS_STATIC;
		dsecsym.NumberOfAuxSymbols = 1;
		aux_data_scn = AddSymbol(dsecsym, data_secn);
	}
}

void BinObj::FixAuxSymbols(int data_size)
{
	{ // metadata section info symbol
		COFF_IMAGE_AUX_SYMBOL msecaux;
		Zero(msecaux);
		msecaux.Section.Length = metadata.GetCount();
		msecaux.Section.NumberOfRelocations = int(relocations.GetCount() / sizeof(COFF_IMAGE_RELOCATION));
		msecaux.Section.Number = SCN_METADATA;
		memcpy(symbol_table.GetIter(aux_metadata_scn), &msecaux, COFF_IMAGE_SIZEOF_SYMBOL);
	}

	{ // data section info symbol
		COFF_IMAGE_AUX_SYMBOL dsecaux;
		Zero(dsecaux);
		dsecaux.Section.Length = data_size;
		dsecaux.Section.Number = SCN_DATA;
		memcpy(symbol_table.GetIter(aux_data_scn), &dsecaux, COFF_IMAGE_SIZEOF_SYMBOL);
	}

	Poke32le(symbol_table_strtbl.Begin(), symbol_table_strtbl.GetCount());
}

void BinObj::WriteFile(String objectfile)
{
	FileOut fo;
	if(!fo.Open(objectfile))
		throw Exc(NFormat("error creating file '%s'", objectfile));

	int section_map_offset = sizeof(COFF_IMAGE_FILE_HEADER);
	int symtbl_offset = section_map_offset + 2 * sizeof(COFF_IMAGE_SECTION_HEADER);
	int strtbl_offset = symtbl_offset + symbol_table.GetCount();
	int reloc_offset = strtbl_offset + symbol_table_strtbl.GetCount();
	int sec1_offset = reloc_offset + relocations.GetCount();
	int sec2_offset = sec1_offset + metadata.GetCount();

	fo.Put('\0', sec2_offset);

	int data_size = 0;

	{ // data section
		ASSERT(fo.GetPos() == sec2_offset);
		for(int i = 0; i < blocks.GetCount(); i++) {
			const ArrayMap<int, Block>& belem = blocks[i];
			for(int a = 0; a < belem.GetCount(); a++) {
				const Block& b = belem[a];
				String data = LoadFile(b.file);
				if(data.IsVoid())
					throw Exc(NFormat("error reading file '%s'", b.file));
				if(data.GetLength() != b.length)
					throw Exc(NFormat("length of file '%s' changed (%d -> %d) during object creation",
						b.file, b.length, data.GetLength()));
				switch(b.encoding) {
					case Block::ENC_ZIP: data = ZCompress(data); break;
					case Block::ENC_BZ2: data = BZ2Compress(data); break;
				}
				int offset = (int)fo.GetPos() - sec2_offset;
				fo.Put(data, data.GetLength());
				int align = Align(data.GetLength() + 1);
				fo.Put('\0', align - data.GetLength());
				ASSERT(b.len_meta_offset >= 0);
				Poke32le(&metadata[b.len_meta_offset], data.GetLength());
				Poke32le(&metadata[b.off_meta_offset], offset);
				data_size += align;
			}
		}
	}

	fo.Seek(0);

	FixAuxSymbols(data_size);

	{ // file header
		ASSERT(fo.GetPos() == 0);
		COFF_IMAGE_FILE_HEADER hdr;
		Zero(hdr);
		hdr.Machine = COFF_IMAGE_FILE_MACHINE_I386;
		hdr.NumberOfSections = 2;
		hdr.TimeDateStamp = (dword)time(NULL);
		hdr.PointerToSymbolTable = symtbl_offset;
		hdr.NumberOfSymbols = symbol_table.GetCount() / COFF_IMAGE_SIZEOF_SYMBOL;
		hdr.Characteristics = COFF_IMAGE_FILE_LINE_NUMS_STRIPPED
			| COFF_IMAGE_FILE_LOCAL_SYMS_STRIPPED
			| COFF_IMAGE_FILE_BYTES_REVERSED_LO
			| COFF_IMAGE_FILE_32BIT_MACHINE;
		fo.Put(&hdr, sizeof(COFF_IMAGE_FILE_HEADER));
	}

	{ // section map
		ASSERT(fo.GetPos() == section_map_offset);
		COFF_IMAGE_SECTION_HEADER meta;
		Zero(meta);
		strncpy((char *)meta.Name, metadata_secn, 8);
		meta.PointerToRawData = sec1_offset;
		meta.SizeOfRawData = metadata.GetCount();
		meta.PointerToRelocations = (relocations.IsEmpty() ? 0 : reloc_offset);
		meta.NumberOfRelocations = int(relocations.GetCount() / sizeof(COFF_IMAGE_RELOCATION));
		meta.Characteristics = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA
			| COFF_IMAGE_SCN_ALIGN_4BYTES
			| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE;
		fo.Put(&meta, sizeof(COFF_IMAGE_SECTION_HEADER));

		COFF_IMAGE_SECTION_HEADER data;
		Zero(data);
		strncpy((char *)data.Name, data_secn, 8);
		data.PointerToRawData = sec2_offset;
		data.SizeOfRawData = data_size;
		data.Characteristics = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA
			| COFF_IMAGE_SCN_ALIGN_4BYTES
			| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE;
		fo.Put(&data, sizeof(COFF_IMAGE_SECTION_HEADER));
	}

	{ // symbol table
		ASSERT(fo.GetPos() == symtbl_offset);
		fo.Put(symbol_table.Begin(), symbol_table.GetCount());
		ASSERT(fo.GetPos() == strtbl_offset);
		fo.Put(symbol_table_strtbl.Begin(), symbol_table_strtbl.GetCount());
	}

	{ // relocations
		ASSERT(fo.GetPos() == reloc_offset);
		fo.Put(relocations.Begin(), relocations.GetCount());
	}

	{ // metadata section
		ASSERT(fo.GetPos() == sec1_offset);
		fo.Put(metadata.Begin(), metadata.GetCount());
	}

	fo.Close();
	if(fo.IsError())
		throw Exc(NFormat("error writing file '%s'", objectfile));
}

int BinObj::AddMetaData(int value, int reloc_symbol)
{
	int len = metadata.GetCount();
	metadata.SetCountR(len + 4);
	Poke32le(metadata.GetIter(len), value);
	if(reloc_symbol >= 0) {
		COFF_IMAGE_RELOCATION reloc;
		reloc.VirtualAddress = len;
		reloc.SymbolTableIndex = reloc_symbol;
		reloc.Type = COFF_IMAGE_REL_I386_DIR32;
		int off = relocations.GetCount();
		relocations.InsertN(off, sizeof(COFF_IMAGE_RELOCATION));
		memcpy(relocations.GetIter(off), &reloc, sizeof(COFF_IMAGE_RELOCATION));
	}
	return len;
}

int BinObj::AddSymbol(COFF_IMAGE_SYMBOL& sym, const char *name)
{
	sym.N.Name.Short = sym.N.Name.Long = 0;
	int nl = (int)strlen(name);
	if(nl <= 8)
		memcpy(sym.N.ShortName, name, nl);
	else {
		int symlen = symbol_table_strtbl.GetCount();
		sym.N.Name.Long = symlen;
		symbol_table_strtbl.InsertN(symlen, nl + 1);
		memcpy(symbol_table_strtbl.GetIter(symlen), name, nl + 1);
	}
	int len = symbol_table.GetCount();
	symbol_table.InsertN(len, COFF_IMAGE_SIZEOF_SYMBOL * (1 + sym.NumberOfAuxSymbols));
	memcpy(symbol_table.GetIter(len), &sym, COFF_IMAGE_SIZEOF_SYMBOL);
	return (sym.NumberOfAuxSymbols ? len + COFF_IMAGE_SIZEOF_SYMBOL : -1);
}

void BinaryToObject(String objectfile, CParser& binscript, String base_dir, Callback1<String> WhenConsole)
{
	BinObj(WhenConsole).Run(objectfile, binscript, base_dir);
}

#ifdef flagMAIN

#define BINOBJ_VERSION   "1.0.r1"
#define BINOBJ_DATE      Date(2004, 10, 2)
#define BINOBJ_COPYRIGHT "Copyright (c) 2004 Tomas Rylek"

static String Usage()
{
	String out;
	out <<
	"BINOBJ: binary to object converter\n"
	"Version:      " BINOBJ_VERSION "\n"
	"Release date: " << BINOBJ_DATE << "\n"
	BINOBJ_COPYRIGHT "\n"
	"\n"
	"Usage: binobj <definition file> [<output object file>]\n"
	"\n"
	"Binary definition file syntax:\n"
	"BINARY(ident, \"abc.def\") // expose file abc.def as identifier ident\n"
	"BINARY_ARRAY(array, 0, \"abc.def\") // expose file abc.def as array[0]\n"
	"\n"
	"Interfacing declarations:\n"
	"\n"
	"extern const unsigned char ident[];\n"
	"extern const int ident_length;\n"
	"extern const unsigned char *array[];\n"
	"extern const int array_length[];\n"
	"extern const int array_count;\n"
	;
	return out;
}

static void TryMain()
{
	const Vector<String>& cmdline = CommandLine();
	String binfile, outfile;
	for(int i = 0; i < cmdline.GetCount(); i++) {
		String arg = cmdline[i];
		if(IsNull(binfile))
			binfile = AppendExt(arg, ".bin");
		else if(IsNull(outfile))
			outfile = AppendExt(arg, ".obj");
		else
			throw Exc(NFormat("no use for argument '%s'", arg));
	}
	if(IsNull(binfile)) {
		puts(Usage());
		SetExitCode(-1);
		return;
	}
	if(IsNull(outfile))
		outfile = ForceExt(binfile, ".obj");
	String bindata = LoadFile(binfile);
	if(bindata.IsVoid())
		throw Exc(NFormat("error reading file '%s'", binfile));
	CParser parser(bindata, binfile);
	BinaryToObject(outfile, parser, GetFileDirectory(binfile));
}

CONSOLE_APP_MAIN
{
	try {
		TryMain();
	}
	catch(Exc e) {
		puts("BINOBJ: " + e);
	}
}
#endif

END_UPP_NAMESPACE
