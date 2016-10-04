#include "uld.h"
#pragma hdrstop

#include "obj.h"

#define LTIMING(x) // RTIMING(x)
#define LDUMP(x)   // RDUMP(x)

/*
static inline unsigned MemHash(const char *b, const char *p)
{
	unsigned out = p - b;
	while(p - b >= 4)
	{
		out ^= PeekIL(b);
		b += 4;
	}
	switch(p - b)
	{
	case 3: out ^= byte(b[2]) << 16;
	case 2: out ^= byte(b[1]) << 8;
	case 1: out ^= byte(b[0]) << 0;
	}
	return out;
}
*/

/*
static inline Point ScanStabType(const char *p, const char *& endptr)
{
	ASSERT(*p == '(' && IsDigit(p[1]));
	Point out(0, 0);
	while(IsDigit(*++p))
		out.x = 10 * out.x + *p - '0';
	if(*p == ',')
		while(IsDigit(*++p))
			out.y = 10 * out.y + *p - '0';
	if(*p == ')')
		p++;
	endptr = p;
	return out;
}
*/

/*
static inline const char *ScanStab(const char *p)
{
	if(*p == '(')
		while(*p && *p++ != ')')
			;
	else if(IsDigit(*p))
		while(IsDigit(*++p))
			;
	return p;
}
*/

static inline String TrimClassName(String clss)
{
	int f = clss.Find('$');
	if(f >= 0)
		clss.Trim(f);
	return clss;
}

static String Undecorate(String s)
{
	const char *p = s;
	while(*p && *p != '$')
		p++;
	if(*p != '$')
		return s;
	const char *b = ++p;
	while(*p && *p != '@')
		p++;
	return String(b, p);
}

ObjectFile::ObjectFile(LinkJob& j, int i, String lf, String of, FILETYPE ft, int lo, Time tm, int os)
: linkjob(j), index(i), library_file(lf), object_file(of)
, filetype(ft), library_offset(lo), file_time(tm), object_size(os)
{
	autocollect = collected = used_any = false;
	dll_atom = 0;
	stab_index = -1;
	stabstr_index = -1;
	has_ctors_dtors = false;
	//!! terrible kludge
	String title = ToLower(GetFileTitle(object_file));
	export_symbols = (filetype == DIRECT && title != "dllcrt2" && title != "crtbegin");
}

String ObjectFile::ToString() const
{
	if(IsNull(library_file))
	{
		if(IsNull(object_file))
			return "(linker)";
		return object_file;
	}
	return GetFileName(library_file) + ":" + object_file;
}

void ObjectFile::ReadFile(const byte *begin)
{
 	const COFF_IMAGE_FILE_HEADER *header = (const COFF_IMAGE_FILE_HEADER *)begin;

	int i;
/*
	const COFF_IMAGE_SYMBOL *symtbl = (const COFF_IMAGE_SYMBOL *)(begin + header->PointerToSymbolTable);
	int nsym = header->NumberOfSymbols;
	const COFF_IMAGE_SYMBOL *symend = symtbl + nsym;
	const char *strtbl = (const char *)symend;
	symbols.Reserve(nsym);
	while(symtbl < symend)
	{
		String name;
		if(symtbl->N.Name.Short)
			name = MaxLenString(symtbl->N.ShortName, 8);
		else
			name = strtbl + symtbl->N.Name.Long;
		int atom = linkjob.Atomize(name);
		int naux = symtbl->NumberOfAuxSymbols;
		symbols.Add(atom, *symtbl++);
		while(--naux >= 0)
			symbols.Add(0, *symtbl++);
	}
*/
	const COFF_IMAGE_SECTION_HEADER *sechdr = (const COFF_IMAGE_SECTION_HEADER *)(begin
		+ sizeof(COFF_IMAGE_FILE_HEADER) + header->SizeOfOptionalHeader), *secptr;
	sections.SetCount(header->NumberOfSections + 1);
	Section& comdat = sections[0];
	comdat.type = SEC_ANON_COMDAT;
	comdat.name_atom = comdat.sec_atom = linkjob.bss_atom;
	comdat.flags = COFF_IMAGE_SCN_CNT_UNINITIALIZED_DATA | COFF_IMAGE_SCN_ALIGN_16BYTES
		| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE;
	comdat.comdat_packing = 0;
	for(i = 1, secptr = sechdr; i < sections.GetCount(); i++, secptr++)
	{
		Section& sec = sections[i];
		sec.raw_size = secptr->SizeOfRawData;
		if(!(sec.size = secptr->Misc.VirtualSize))
			sec.size = sec.raw_size;
		sec.offset = secptr->PointerToRawData;
		ASSERT(sec.offset >= 0);
		sec.flags = secptr->Characteristics;
		sec.comdat_packing = 0;
		sec.coff_reloc_offset = secptr->VirtualAddress;
//		sec.comdat_assoc = 0;

		String secn = MaxLenString(secptr->Name, sizeof(secptr->Name));
/*
		if(linkjob.merge_gcc_sections && secn[0] == '/' && IsDigit(secn[1]))
		{
			if(sec.flags & COFF_IMAGE_SCN_CNT_CODE)
				secn = ".text$" + secn;
			else if(sec.flags & COFF_IMAGE_SCN_CNT_INITIALIZED_DATA)
				secn = ".data$" + secn;
			else
				secn = ".bss$" + secn;
		}
*/
		sec.name_atom = linkjob.Atomize(secn);
		secn = TrimClassName(secn);
		sec.sec_atom = linkjob.Atomize(secn);
		bool ctor_dtor_section = false;
		if(sec.sec_atom == linkjob.ctors_atom || sec.sec_atom == linkjob.dtors_atom)
		{
			sec.sec_atom = linkjob.data_atom;
//			sec.flags = (sec.flags & ~COFF_IMAGE_SCN_ALIGN_MASK) | COFF_IMAGE_SCN_ALIGN_4BYTES;
			has_ctors_dtors = ctor_dtor_section = true;
		}
		if(sec.sec_atom == linkjob.idata_atom)
			linkjob.idata_last = index;
		if(sec.name_atom == linkjob.idata_inull_atom)
			linkjob.idata_null_found = true;

		sec.type = (sec.sec_atom == linkjob.stab_atom
			? SEC_STAB : sec.sec_atom == linkjob.stabstr_atom
				? SEC_STABSTR : SEC_STD);
		if(sec.type == SEC_STAB)
			stab_index = i;
		else if(sec.type == SEC_STABSTR)
			stabstr_index = i;
		sec.autocollect = sec.type == SEC_STD
			&& (sec.sec_atom == linkjob.crt_atom
				|| ctor_dtor_section
				|| sec.sec_atom == linkjob.idata_atom
				|| sec.sec_atom == linkjob.rsrc_atom
				|| !linkjob.autocollect_crt_only && (sec.flags & COFF_IMAGE_SCN_CNT_INITIALIZED_DATA));
		if(sec.name_atom == linkjob.drectve_atom && (sec.flags & COFF_IMAGE_SCN_LNK_INFO))
		{
			String directives = String(begin + secptr->PointerToRawData, secptr->SizeOfRawData);
			linkjob.ReadCommand(directives, false);
		}
	}

	if(stab_index >= 0 && stabstr_index >= 0)
		linkjob.has_stabs = true;

	coff_symbols.SetCount(header->NumberOfSymbols);

	const COFF_IMAGE_SYMBOL *symtbl = (const COFF_IMAGE_SYMBOL *)(begin + header->PointerToSymbolTable), *symptr = symtbl;
	const char *strtbl = (const char *)(symtbl + header->NumberOfSymbols);
//	int xxxatom = linkjob.Atomize("?AppMain@@YAXXZ");
	for(i = 0; i < (int)header->NumberOfSymbols; i++, symptr++)
	{
//		if(symptr->SectionNumber > 0 && linkjob.NameAtom(*symptr, strtbl) == xxxatom)
//			__asm int 3;
//		RLOG(NFormat("%30<s SEC %04d VAL %08x %s", linkjob[linkjob.NameAtom(*symptr, strtbl)],
//			symptr->SectionNumber, (int)symptr->Value, file));

		if(symptr->SectionNumber < 0)
			coff_symbols[i] = Symbol(symptr->SectionNumber, symptr->Value);
		else if(symptr->StorageClass == COFF_IMAGE_SYM_CLASS_STATIC && symptr->SectionNumber != 0
		&& symptr->Value == 0 && symptr->NumberOfAuxSymbols >= 1)
//		&& linkjob.Atomize(TrimClassName(COFFSymbolName(*symptr, strtbl))) == sections[(int)symptr->SectionNumber].name_atom)
		{ // section aux symbol
			LinkJob::Symbol lsym;
			lsym.obj_sec.object = index;
			lsym.obj_sec.section = symptr->SectionNumber;
			lsym.value = 0;
			lsym.type = LinkJob::COFF_IMAGE_SYM_TYPE_SECTION;
			Section& sec = sections[(int)symptr->SectionNumber];
			if(sec.name_atom != linkjob.ctors_atom && sec.name_atom != linkjob.dtors_atom)
			{
				String secn = COFFSymbolName(*symptr, strtbl);
				sec.name_atom = linkjob.Atomize(secn);
				secn = TrimClassName(secn);
				sec.sec_atom = linkjob.Atomize(secn);
			}
			linkjob.AddGlobal(sec.name_atom, lsym);
			const COFF_IMAGE_AUX_SYMBOL *aux = (const COFF_IMAGE_AUX_SYMBOL *)(symptr + 1);
			if(sec.flags & COFF_IMAGE_SCN_LNK_COMDAT)
			{
				sec.comdat_packing = aux->Section.Selection;
				comdat_assoc.Add(aux->Section.Number, symptr->SectionNumber);
//				sec.comdat_assoc = aux->Section.Number;
				if(sec.comdat_packing == COFF_IMAGE_COMDAT_SELECT_EXACT_MATCH)
				{
					secptr = sechdr + (symptr->SectionNumber);
					sec.section_data.Reserve(secptr->SizeOfRawData + secptr->NumberOfRelocations * sizeof(COFF_IMAGE_RELOCATION));
					CatN(sec.section_data, secptr->SizeOfRawData, begin + secptr->PointerToRawData);
					const COFF_IMAGE_RELOCATION *relptr = (const COFF_IMAGE_RELOCATION *)(begin + secptr->PointerToRelocations);
					const COFF_IMAGE_RELOCATION *relend = relptr + secptr->NumberOfRelocations;
					while(relptr < relend)
					{
						COFF_IMAGE_RELOCATION r = *relptr++;
						r.SymbolTableIndex = linkjob.NameAtom(symtbl[r.SymbolTableIndex], strtbl);
						CatN(sec.section_data, sizeof(COFF_IMAGE_RELOCATION), &r);
					}
				}
			}
			coff_symbols[i] = Symbol(lsym.obj_sec.section, 0);
		}
		else if((symptr->StorageClass == COFF_IMAGE_SYM_CLASS_EXTERNAL || symptr->StorageClass == COFF_IMAGE_SYM_CLASS_STATIC)
		&& (symptr->SectionNumber != 0 || symptr->Value != 0))
		{
			bool glo = (symptr->StorageClass == COFF_IMAGE_SYM_CLASS_EXTERNAL);
			LinkJob::Symbol lsym;
			lsym.obj_sec.object = index;
			lsym.obj_sec.section = symptr->SectionNumber;
			lsym.value = symptr->Value;
			lsym.type = symptr->Type;
			lsym.external = glo;
			int atom = linkjob.NameAtom(*symptr, strtbl);
			linkjob.AddGlobal(atom, lsym);
			coff_symbols[i] = Symbol(glo ? ~atom : lsym.obj_sec.section, lsym.value);
		}
		else if(symptr->StorageClass == COFF_IMAGE_SYM_CLASS_WEAK_EXTERNAL && symptr->NumberOfAuxSymbols >= 1)
		{
			int refsym = symptr[1].N.Name.Short;
			int sym = linkjob.NameAtom(*symptr, strtbl);
			linkjob.AddWeakExternal(sym, linkjob.NameAtom(symtbl[refsym], strtbl));
			coff_symbols[i] = Symbol(~sym, symptr->Value);
		}
		else if(symptr->StorageClass == COFF_IMAGE_SYM_CLASS_EXTERNAL)
			coff_symbols[i] = Symbol(~linkjob.NameAtom(*symptr, strtbl), 0);
		else if(symptr->SectionNumber > 0)
			coff_symbols[i] = Symbol(symptr->SectionNumber, symptr->Value);
		i += symptr->NumberOfAuxSymbols;
		symptr += symptr->NumberOfAuxSymbols;
	}

	for(i = 1, secptr = sechdr; i < sections.GetCount(); i++, secptr++)
	{
		Section& sec = sections[i];
		sec.ref_sec_index = ref_sections.GetCount();
		sec.ref_ext_index = ref_externals.GetCount();
		if(sec.type == SEC_STD)
		{
			sec.coff_reloc.SetCount(secptr->NumberOfRelocations);
			memcpy(sec.coff_reloc.Begin(), (const COFF_IMAGE_RELOCATION *)(begin + secptr->PointerToRelocations),
				secptr->NumberOfRelocations * sizeof(COFF_IMAGE_RELOCATION));
			const COFF_IMAGE_RELOCATION *reloc = sec.coff_reloc.Begin();
			Index<int> ref_sec, ref_ext;
			for(int nreloc = secptr->NumberOfRelocations; --nreloc >= 0; reloc++)
			{
				const COFF_IMAGE_SYMBOL& sym = symtbl[reloc->SymbolTableIndex];
				if(sym.SectionNumber > 0 && sym.StorageClass != COFF_IMAGE_SYM_CLASS_EXTERNAL)
					ref_sec.FindAdd(sym.SectionNumber);
				if(sym.StorageClass == COFF_IMAGE_SYM_CLASS_STATIC && sym.SectionNumber > 0
				&& sym.Value == 0 && sym.NumberOfAuxSymbols >= 1 && !(sections[sym.SectionNumber].flags & COFF_IMAGE_SCN_LNK_COMDAT))
				{} // no-op for section externals
				else if(sym.StorageClass == COFF_IMAGE_SYM_CLASS_EXTERNAL
					|| sym.StorageClass == COFF_IMAGE_SYM_CLASS_WEAK_EXTERNAL
					|| sym.StorageClass == COFF_IMAGE_SYM_CLASS_STATIC)
				{
					int atom = linkjob.NameAtom(sym, strtbl);
					if(sym.SectionNumber == 0)
						linkjob.AddExternal(atom, ObjSec(index, i));
					ref_ext.FindAdd(atom);
				}
	/*
				{
					LinkJob::Symbol symbol;
					symbol.object = index;
					symbol.value = sym.Value;
					symbol.type = sym.Type;
					symbol.section = sym.SectionNumber;
					ref_ext.FindAdd(linkjob.AddGlobal(linkjob.NameAtom(sym, strtbl), symbol));
				}
	*/
			}
			ref_sections.AppendPick(ref_sec.PickKeys());
			ref_externals.AppendPick(ref_ext.PickKeys());
		}
/*
		else if(sec.type == SEC_STAB && stabstr_index >= 0)
		{
			ASSERT(sizeof(STAB_INFO) == 12);
			int count = sec.size / sizeof(STAB_INFO);
			stabs.SetCount(count);
			const byte *p = begin + sec.offset;
#ifdef CPU_LE // && CPU_32BIT
			memcpy(stabs.Begin(), p, count * sizeof(STAB_INFO));
#else
			for(STAB_INFO *sp = stabs.Begin(), *se = stabs.End(); sp < se; sp++, p += 12)
			{
				sp->strdx = PeekIL(p + STAB_STRDXOFF);
				sp->type = p[STAB_TYPEOFF];
				sp->other = p[STAB_OTHEROFF];
				sp->desc = PeekIW(p[STAB_DESCOFF]);
				sp->value = PeekIL(p[STAB_VALUEOFF]);
			}
#endif
			stab_ref.SetCount(count, 0);
			for(int nreloc = secptr->NumberOfRelocations; --nreloc >= 0; reloc++)
			{
				int entry = reloc->VirtualAddress / sizeof(STAB_INFO);
				if(entry < 0 || entry >= count)
					continue;
				const COFF_IMAGE_SYMBOL& sym = symtbl[reloc->SymbolTableIndex];
				if(sym.SectionNumber)
					stab_ref[entry] = ~sym.SectionNumber;
				else if(sym.StorageClass == COFF_IMAGE_SYM_CLASS_EXTERNAL || sym.StorageClass == COFF_IMAGE_SYM_CLASS_STATIC)
					stab_ref[entry] = linkjob.NameAtom(sym, strtbl);
			}
			if(linkjob.dump_flags & LinkJob::DUMP_STABS)
			{
				const Section& strsec = sections[stabstr_index];
				puts(NFormat("%s: %d stabs, .stab = %d bytes, .stabstr = %d bytes",
					ToString(), count, sec.size, strsec.size));
				const byte *str = begin + strsec.offset;
				for(int i = 0; i < stabs.GetCount(); i++)
				{
					const STAB_INFO& stab = stabs[i];
					String desc;
					desc << NFormat("[%d]: offset %08x, type %02x, desc %04x, value %08x, reloc %08x",
						i, stab.strdx, stab.type, stab.desc, stab.value, stab_ref[i]);
					puts(desc);
					if(stab.strdx)
						puts((const char *)(str + stab.strdx));
				}
			}
		}
*/
	}

	ref_sections.Shrink();
	ref_externals.Shrink();
}

void ObjectFile::Dump() const
{
	int sym_size = (ref_sections.GetCount() + ref_externals.GetCount()) * sizeof(int);
	int sec_size = sections.GetCount() * sizeof(Section);
	int mapped_size = sizeof(ObjectFile) + sym_size + sec_size;

	RLOG("ObjectFile(" << library_file << ":" << object_file << "), mapped size = " << mapped_size
		<< " (" << sym_size << " in symbols, " << sec_size << " in sections)");
//	RLOG("Linker directives: " << directives);
	RLOG("#sections = " << sections.GetCount());
/*
	for(int i = 0; i < sections.GetCount(); i++)
	{
		const Section& sec = sections[i];
		RLOG(NFormat("[%d]: %s", i, linkjob[sec.name_atom]));
		int end_ext = (i + 1 < sections.GetCount() ? sections[i + 1].ref_ext_index : ref_externals.GetCount());
		int end_sec = (i + 1 < sections.GetCount() ? sections[i + 1].ref_sec_index : ref_sections.GetCount());
		RLOG("#external references = " << (end_ext - sec.ref_ext_index));
		int r;
		for(r = sec.ref_ext_index; r < end_ext; r++)
			RLOG(linkjob[linkjob.globals.GetKey(ref_externals[r])]);
		RLOG("#section references = " << (end_sec - sec.ref_sec_index));
		for(r = sec.ref_sec_index; r < end_sec; r++)
			RLOG(NFormat("%04x", ref_sections[r]));
	}
*/
}
