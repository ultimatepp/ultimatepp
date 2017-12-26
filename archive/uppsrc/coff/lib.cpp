#include "coff.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

enum { HEADER_SIZE = sizeof(COFF_IMAGE_ARCHIVE_MEMBER_HEADER) };

static double ToLibraryTime(Time time)
{
	if(IsNull(time))
		return Null;
	SYSTEMTIME tm, tb;
	Zero(tm); Zero(tb);
	tm.wYear   = time.year;   tb.wYear   = 1970;
	tm.wMonth  = time.month;  tb.wMonth  = 1;
	tm.wDay    = time.day;    tb.wDay    = 1;
	tm.wHour   = time.hour;   tb.wHour   = 2;
	tm.wMinute = time.minute;
	tm.wSecond = time.second;
	FileTime /*ftl,*/ ftg;
	SystemTimeToFileTime(&tm, &ftg);
//	LocalFileTimeToFileTime(&ftl, &ftg);
	FileTime fbl, fbg;
	SystemTimeToFileTime(&tb, &fbl);
	LocalFileTimeToFileTime(&fbl, &fbg);
#ifdef PLATFORM_WIN32
	int64 tg = ftg.dwLowDateTime + (int64(1) << 32) * ftg.dwHighDateTime;
	int64 bg = fbg.dwLowDateTime + (int64(1) << 32) * fbg.dwHighDateTime;
	return (double)((tg - bg) / 10000000);
#else
	return tg - bg;
#endif
}

ArchiveJob::Object::Object(ArchiveJob& archive, int index, String fn, String od, double ft, int ho, bool nf)
: archive(archive), index(index), filename(fn), object_data(od), filetime(ft), newfile(nf), header_offset(ho)
{
	trimmed_name = archive.TrimObjectName(filename);
	longname_offset = -1;
	header_offset = -1;
}

void ArchiveJob::Object::ReadObject()
{
	if(archive.verbose)
		PutStdOut(NFormat("%s: reading object file (%d B)", filename, object_data.GetLength()));
	const byte *begin = object_data;

 	const COFF_IMAGE_FILE_HEADER *header = (const COFF_IMAGE_FILE_HEADER *)begin;
	const COFF_IMAGE_SYMBOL *symtbl = (const COFF_IMAGE_SYMBOL *)(begin + header->PointerToSymbolTable), *symptr = symtbl;
	const char *strtbl = (const char *)(symtbl + header->NumberOfSymbols);
	for(int i = 0; i < (int)header->NumberOfSymbols; i++, symptr++)
	{
		if(symptr->StorageClass == COFF_IMAGE_SYM_CLASS_STATIC && symptr->SectionNumber != 0
		&& symptr->Value == 0 && symptr->NumberOfAuxSymbols >= 1)
			; // ignore section symbols
		else if((symptr->StorageClass == COFF_IMAGE_SYM_CLASS_EXTERNAL || symptr->StorageClass == COFF_IMAGE_SYM_CLASS_STATIC)
		&& (symptr->SectionNumber != 0 || symptr->Value != 0))
			archive.AddGlobal(index, COFFSymbolName(*symptr, strtbl));
		i += symptr->NumberOfAuxSymbols;
		symptr += symptr->NumberOfAuxSymbols;
	}
}

ArchiveJob::ArchiveJob()
{
	keep_object_paths = false;
	use_existing_archive = true;
	trim_longname_objects = false;
	skip_duplicates = true;
	verbose = false;
#if defined(CPU_IA32)
	machine = COFF_IMAGE_FILE_MACHINE_I386;
#else
	machine = COFF_IMAGE_FILE_MACHINE_UNKNOWN;
#endif
}

void ArchiveJob::LoadFile(String file)
{
	FileMapping mapping;
	if(!mapping.Open(file))
		throw Exc(NFormat("%s: file failed to open", file));
	objects[AddObject(file, mapping.GetData(0, (dword)mapping.GetFileSize()), mapping.GetTime())].ReadObject();
}

int ArchiveJob::AddObject(String filename, String object_data, Time filetime)
{
	return objects.Add(new Object(*this, objects.GetCount(), filename, object_data,
		ToLibraryTime(filetime), -1, true)).index;
}

void ArchiveJob::LoadLibrary(String libfile)
{
	FileMapping mapping;
	if(!mapping.Open(libfile))
	{
		if(archive_must_exist)
			throw Exc(NFormat("%s: archive file failed to open", libfile));
		if(use_existing_archive && verbose)
			PutStdOut(NFormat("%s: archive not found", libfile));
		return;
	}
	if(memcmp(~mapping, "!<arch>\n", 8))
		throw Exc(NFormat("%s: not a valid library file (missing !<arch>\\n header)", libfile));
	if(verbose)
		PutStdOut(NFormat("%s: reading archive (%d B)", libfile, mapping.GetFileSize()));

	const byte *ptr = mapping.GetIter(8);
	const byte *end = mapping.End();
	const byte *longptr = NULL;
	while(ptr + HEADER_SIZE <= end)
	{
//		if(*ptr == '\n')
//			ptr++;
		int offset = int(ptr - mapping.Begin());
		COFF_IMAGE_ARCHIVE_MEMBER_HEADER hdr;
		memcpy(&hdr, ptr, sizeof(hdr));
		ptr += sizeof(hdr);
		int size = atoi((const char *)hdr.Size);
		const byte *brk = ptr + ((size + 1) & -2);
		if(hdr.Name[0] == '/' && hdr.Name[1] == '/')
		{ // longnames
			longptr = ptr;
		}
		else if(hdr.Name[0] != '/' || hdr.Name[1] != ' ')
		{ // object
			const byte *nameptr, *namelim;
			if(hdr.Name[0] != '/')
			{
				nameptr = hdr.Name;
				namelim = nameptr + __countof(hdr.Name);
			}
			else
			{
				int loff = atoi((const char *)hdr.Name + 1);
				nameptr = longptr + loff;
				namelim = end;
			}
			const byte *p = nameptr;
			while(p < namelim && *p && *p != '/')
				p++;
			String objname(nameptr, int(p - nameptr));
			String objdata(ptr, size);
			char temp[13];
			memcpy(temp, hdr.Date, 12);
			temp[12] = 0;
			double membertime = ScanDouble(temp);
			const COFF_IMPORT_OBJECT_HEADER *imphdr = (const COFF_IMPORT_OBJECT_HEADER *)ptr;
			int oindex = objects.GetCount();
			if(skip_duplicates && FindObject(objname) >= 0)
			{
				if(verbose)
					PutStdOut(NFormat("%s: skipping object '%s' (inserting object with the same name)", libfile, objname));
			}
			else
			{
				if(imphdr->Sig1 == 0 && imphdr->Sig2 == 0xFFFF)
				{
					if(imphdr->Machine != machine)
						throw Exc(NFormat("%s:%s: invalid machine type (%d) in import library module",
							libfile, objname, imphdr->Machine));
					const char *pname = (const char *)ptr + sizeof(COFF_IMPORT_OBJECT_HEADER);
					AddGlobal(oindex, pname);
					objects.Add(new Object(*this, oindex, objname, objdata, membertime, offset, false));
				}
				else
					objects.Add(new Object(*this, oindex, objname, objdata, membertime, offset, false)).ReadObject();
			}
		}
		ptr = brk;
	}
}

String ArchiveJob::TrimObjectName(String objname) const
{
	String out = objname;
	if(!keep_object_paths)
		out = GetFileName(out);
	out = ToLower(out);
	if(trim_longname_objects && out.GetLength() > 15)
	{
		for(int f; out.GetLength() > 15 && ((f = out.Find('/')) >= 0 || (f = out.Find('\\')) >= 0); out.Remove(0, f + 1))
			;
		if(out.GetLength() > 15)
			out.Trim(15);
	}
	return out;
}

int ArchiveJob::FindObject(String objname) const
{
	return FindFieldIndex(objects, &Object::trimmed_name, TrimObjectName(objname));
}

void ArchiveJob::BuildLibrary(String libfile, bool keep_archive_backups)
{
	SortGlobals();
	BuildSymbolIndex();
	BuildLongNameIndex();
	CalcArchiveOffsets();
	WriteArchive(libfile, keep_archive_backups);
}

void ArchiveJob::AddGlobal(int object, String symbol)
{
	if(symbol_names.FindAdd(symbol) >= symbol_objects.GetCount())
		symbol_objects.Add(object);
}

void ArchiveJob::SortGlobals()
{
	Vector<String> symn = symbol_names.PickKeys();
	IndexSort(symn, symbol_objects, StdLess<String>());
	symbol_names = symn;
}

void ArchiveJob::BuildLongNameIndex()
{
	for(int i = 0; i < objects.GetCount(); i++)
	{
		String n = objects[i].trimmed_name;
		if(n.GetLength() >= 16)
		{
			objects[i].longname_offset = longnames.GetLength();
			longnames.Cat(n, n.GetLength() + 1);
		}
	}
}

void ArchiveJob::BuildSymbolIndex()
{
	for(int i = 0; i < symbol_names.GetCount(); i++)
		symbolnames.Cat(symbol_names[i], symbol_names[i].GetLength() + 1);
}

void ArchiveJob::CalcArchiveOffsets()
{
	first_index_offset = 8; // header length
	first_index_datasize = 4 + 4 * symbol_names.GetCount() + symbolnames.GetLength();
	second_index_offset = first_index_offset + HEADER_SIZE + ((first_index_datasize + 1) & -2);
	second_index_datasize = 4 + 4 * objects.GetCount() + 4 + 2 * symbol_names.GetCount() + symbolnames.GetLength();
	longnames_offset = second_index_offset + HEADER_SIZE + ((second_index_datasize + 1) & -2);
	archive_length = longnames_offset + HEADER_SIZE + ((longnames.GetLength() + 1) & -2);
	for(int i = 0; i < objects.GetCount(); i++)
	{
		objects[i].header_offset = archive_length;
		archive_length += HEADER_SIZE + ((objects[i].object_data.GetLength() + 1) & -2);
	}
}

static void WriteMemberHeader(byte *out, String name, int longname_offset, double time, int size)
{
	memset(out, ' ', sizeof(COFF_IMAGE_ARCHIVE_MEMBER_HEADER));
	if(longname_offset < 0)
	{
		memcpy(out, name, name.GetLength());
		out[name.GetLength()] = '/';
	}
	else
	{
		String tmp = FormatInt(longname_offset);
		out[0] = '/';
		memcpy(out + 1, tmp, tmp.GetLength());
	}
	String tmp = FormatDoubleFix(time, 0);
	memcpy(out + 16, tmp, min(tmp.GetLength(), 12));
	out[28] = '0';
	out[34] = '0';
	memcpy(out + 40, "100666", 6);
	tmp = FormatInt(size);
	memcpy(out + 48, tmp, tmp.GetLength());
	out[58] = COFF_IMAGE_ARCHIVE_END1;
	out[59] = COFF_IMAGE_ARCHIVE_END2;
	if(size & 1)
		out[HEADER_SIZE + size] = COFF_IMAGE_ARCHIVE_PAD;
}

void ArchiveJob::WriteArchive(String libraryfile, bool keep_archive_backups)
{
	Vector<byte> archive;
	archive.SetCount(archive_length);
	memcpy(archive.Begin(), "!<arch>\n", 8);
	double nulltime = ToLibraryTime(GetSysTime());
	WriteMemberHeader(archive.GetIter(first_index_offset), "", -1, nulltime, first_index_datasize);
	WriteMemberHeader(archive.GetIter(second_index_offset), "", -1, nulltime, second_index_datasize);
	WriteIndex(archive.GetIter(first_index_offset + HEADER_SIZE), archive.GetIter(second_index_offset + HEADER_SIZE));
	WriteMemberHeader(archive.GetIter(longnames_offset), "/", -1, nulltime, longnames.GetLength());
	memcpy(archive.GetIter(longnames_offset + HEADER_SIZE), longnames, longnames.GetLength());
	for(int i = 0; i < objects.GetCount(); i++)
	{
		const Object& o = objects[i];
		WriteMemberHeader(archive.GetIter(o.header_offset), o.trimmed_name, o.longname_offset, o.filetime, o.object_data.GetLength());
		memcpy(archive.GetIter(o.header_offset + HEADER_SIZE), o.object_data, o.object_data.GetLength());
	}
	String tmpfile = libraryfile + ".tmp";
	String oldfile = libraryfile + ".old";
	FileOut fo;
	if(!fo.Open(tmpfile))
		throw Exc(NFormat("%s: failed to create file", tmpfile));
	if(verbose)
		PutStdOut(NFormat("%s: saving updated archive (%d B)", libraryfile, archive_length));
	fo.Put(archive.Begin(), archive_length);
	fo.Close();
	if(fo.IsError())
	{
		FileDelete(tmpfile);
		throw Exc(NFormat("%s: error writing file", tmpfile));
	}
	FileDelete(oldfile);
	FileMove(libraryfile, oldfile);
	if(!FileMove(tmpfile, libraryfile))
		throw Exc(NFormat("%s: error replacing %s", tmpfile, libraryfile));
	if(!keep_archive_backups)
		FileDelete(oldfile);
}

void ArchiveJob::WriteIndex(byte *first, byte *second)
{
	int nsym = symbol_names.GetCount();
	Poke32be(first, nsym);
	first += 4;
	for(int i = 0; i < nsym; i++, first += 4)
		Poke32be(first, objects[symbol_objects[i]].header_offset);
	memcpy(first, symbolnames, symbolnames.GetLength());
	Poke32le(second, objects.GetCount());
	second += 4;
	for(int i = 0; i < objects.GetCount(); i++, second += 4)
		Poke32le(second, objects[i].header_offset);
	Poke32le(second, nsym);
	second += 4;
	for(int i = 0; i < symbol_names.GetCount(); i++, second += 2)
		Poke16le(second, symbol_objects[i] + 1);
	memcpy(second, symbolnames, symbolnames.GetLength());
}

#endif

END_UPP_NAMESPACE
