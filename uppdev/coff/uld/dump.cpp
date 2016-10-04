#include "uld.h"
#pragma hdrstop

class FileDump
{
public:
	FileDump() : dump_stabs(false), dump_globals(false) {}

	void        Run(const char *fn);

private:
	void        RunCOFF();
	void        RunSymbols(int symoff, int symcnt);
	void        RunStab(int staboff, int stablen, int stroff, int strlen);

private:
	String      filename;
	FileMapping mapping;
	int         ne_offset;
	bool        dump_stabs;
	bool        dump_globals;
};

void FileDump::RunSymbols(int symoff, int symcnt)
{
	const COFF_IMAGE_SYMBOL *sym = (COFF_IMAGE_SYMBOL *)&mapping[symoff];
	const COFF_IMAGE_SYMBOL *end = sym + symcnt;
	const char *strtbl = (const char *)end;
	puts(NFormat("Global symbol table (%d entries)", symcnt));
	puts("Sect Value    Type Cl Name");
	for(; sym < end; sym++)
	{
		puts(NFormat("%4>04x %08x %04x %02x %s",
			sym->SectionNumber, (int)sym->Value, sym->Type, sym->StorageClass, COFFSymbolName(*sym, strtbl)));
		if(sym->NumberOfAuxSymbols)
		{
			if(sym->StorageClass == COFF_IMAGE_SYM_CLASS_FILE)
				puts(NFormat("\tFile: %s", (const char *)(sym + 1)));
		}
		sym += sym->NumberOfAuxSymbols;
	}
}

void FileDump::RunStab(int staboff, int stablen, int stabstroff, int stabstrlen)
{
	int entries = stablen / STAB_STABSIZE;
	puts(NFormat("STAB debug info (%d entries)", entries));
	const STAB_INFO *info = (const STAB_INFO *)&mapping[staboff];
	const char *str = (const char *)&mapping[stabstroff];
	int counts[256], sizes[256];
	ZeroArray(counts);
	ZeroArray(sizes);
	int strtotal = 0;
	static VectorMap<String, int> old_histogram;
	VectorMap<String, int> histogram;
	for(int i = 0; i < entries; i++, info++)
	{
		counts[info->type]++;
		int sl = strlen(str + info->strdx) + 1;
		sizes[info->type] += sl;
		strtotal += sl;
		puts(NFormat("%06x %02x %02x %04x %08x %s",
			i, info->type, info->other, info->desc, info->value, str + info->strdx));
		String hentry = NFormat("%02x %02x %04x %08x %s", info->type, info->other, info->desc, 0/*info->value*/, str + info->strdx);
		histogram.GetAdd(hentry, 0)++;
	}
	puts(NFormat("%d B, compressed %d B = %d%%",
		strtotal, stabstrlen, 100 * (1 - stabstrlen / double(max(strtotal, 1)))));
	for(NOMSC6(int) i = 0; i < 256; i++)
		if(counts[i])
			puts(NFormat("[%02x]: %6>d times, %8>d B", i, counts[i], sizes[i]));
	puts("\nHistogram:");
	Vector<int> order = GetSortOrder(histogram.GetValues(), StdGreater<int>());
	for(NOMSC6(int) i = 0; i < order.GetCount(); i++)
		puts(NFormat("%4>d * %s", histogram[order[i]], histogram.GetKey(order[i])));
	if(!old_histogram.IsEmpty())
	{
		puts("\nDifferential histogram:");
		VectorMap<String, int> diff;
		diff <<= histogram;
		for(int i = 0; i < old_histogram.GetCount(); i++)
			diff.GetAdd(old_histogram.GetKey(i), 0) -= old_histogram[i];
		for(NOMSC6(int) i = diff.GetCount(); --i >= 0;)
			if(!diff[i])
				diff.Remove(i);
		order = GetSortOrder(diff.GetValues(), StdGreater<int>());
		for(NOMSC6(int) i = 0; i < order.GetCount(); i++)
		{
			String key = diff.GetKey(order[i]);
			puts(NFormat("(%4>d) %4>~d <- %4>~d * %s", diff[order[i]], histogram.Get(key, Null), old_histogram.Get(key, Null), key));
		}
	}
	old_histogram = histogram;
}

void FileDump::RunCOFF()
{
	const COFF_IMAGE_FILE_HEADER *hdr = (const COFF_IMAGE_FILE_HEADER *)&mapping[ne_offset + 4];
	puts(NFormat("File:                 %s", filename));
	puts(NFormat("File size:            %d B", mapping.GetLength()));
	const MachineInfo *mach = COFFMachineList();
	while(mach->name && mach->code != hdr->Machine)
		mach++;
	puts(NFormat("Machine:              0x%04x (%s)", hdr->Machine, mach->name));
	puts(NFormat("NumberOfSections:     %d", hdr->NumberOfSections));
	time_t stamp = hdr->TimeDateStamp;
	puts(NFormat("TimeDateStamp:        0x%08x, (%s)", (int)hdr->TimeDateStamp, asctime(localtime(&stamp))));
	puts(NFormat("PointerToSymbolTable: 0x%08x", (int)hdr->PointerToSymbolTable));
	puts(NFormat("NumberOfSymbols:      %d", (int)hdr->NumberOfSymbols));
	puts(NFormat("SizeOfOptionalHeader: %d", hdr->SizeOfOptionalHeader));
	puts(NFormat("Characteristics:      0x%08x", hdr->Characteristics));
	if(hdr->SizeOfOptionalHeader < sizeof(COFF_IMAGE_OPTIONAL_HEADER32))
		return;

	const COFF_IMAGE_OPTIONAL_HEADER32 *opthdr = (const COFF_IMAGE_OPTIONAL_HEADER32 *)
			&mapping[ne_offset + 4 + sizeof(COFF_IMAGE_FILE_HEADER)];

	puts(NFormat("Magic:                       0x%08x", opthdr->Magic));
	puts(NFormat("MajorLinkerVersion:          %d", opthdr->MajorLinkerVersion));
	puts(NFormat("MinorLinkerVersion:          %d", opthdr->MinorLinkerVersion));
	puts(NFormat("SizeOfCode:                  0x%08x", (int)opthdr->SizeOfCode));
	puts(NFormat("SizeOfInitializedData:       0x%08x", (int)opthdr->SizeOfInitializedData));
	puts(NFormat("SizeOfUninitializedData:     0x%08x", (int)opthdr->SizeOfUninitializedData));
	puts(NFormat("AddressOfEntryPoint:         0x%08x", (int)opthdr->AddressOfEntryPoint));
	puts(NFormat("BaseOfCode:                  0x%08x", (int)opthdr->BaseOfCode));
	puts(NFormat("BaseOfData:                  0x%08x", (int)opthdr->BaseOfData));
	puts(NFormat("ImageBase:                   0x%08x", (int)opthdr->ImageBase));
	puts(NFormat("SectionAlignment:            0x%08x", (int)opthdr->SectionAlignment));
	puts(NFormat("FileAlignment:               0x%08x", (int)opthdr->FileAlignment));
	puts(NFormat("MajorOperatingSystemVersion: %d", opthdr->MajorOperatingSystemVersion));
	puts(NFormat("MinorOperatingSystemVersion: %d", opthdr->MinorOperatingSystemVersion));
	puts(NFormat("MajorImageVersion:           %d", opthdr->MajorImageVersion));
	puts(NFormat("MinorImageVersion:           %d", opthdr->MinorImageVersion));
	puts(NFormat("MajorSubsystemVersion:       %d", opthdr->MajorSubsystemVersion));
	puts(NFormat("MinorSubsystemVersion:       %d", opthdr->MinorSubsystemVersion));
	puts(NFormat("Win32VersionValue:           %d", (int)opthdr->Win32VersionValue));
	puts(NFormat("SizeOfImage:                 0x%08x", (int)opthdr->SizeOfImage));
	puts(NFormat("SizeOfHeaders:               0x%08x", (int)opthdr->SizeOfHeaders));
	puts(NFormat("CheckSum:                    0x%08x", (int)opthdr->CheckSum));
	puts(NFormat("Subsystem:                   %d", opthdr->Subsystem));
	puts(NFormat("DllCharacteristics:          0x%08x", opthdr->DllCharacteristics));
	puts(NFormat("SizeOfStackReserve:          0x%08x", (int)opthdr->SizeOfStackReserve));
	puts(NFormat("SizeOfStackCommit:           0x%08x", (int)opthdr->SizeOfStackCommit));
	puts(NFormat("SizeOfHeapReserve:           0x%08x", (int)opthdr->SizeOfHeapReserve));
	puts(NFormat("SizeOfHeapCommit:            0x%08x", (int)opthdr->SizeOfHeapCommit));
	puts(NFormat("LoaderFlags:                 0x%08x", (int)opthdr->LoaderFlags));
	puts(NFormat("NumberOfRvaAndSizes:         %d", (int)opthdr->NumberOfRvaAndSizes));
	const COFF_IMAGE_DATA_DIRECTORY *dir = opthdr->DataDirectory;
	puts("Index RVA        Size");
	int i;
	for(i = 0; i < (int)opthdr->NumberOfRvaAndSizes; i++, dir++)
		if(dir->VirtualAddress || dir->Size)
			puts(NFormat("0x%02x  0x%08x 0x%08x", i, (int)dir->VirtualAddress, (int)dir->Size));
	const COFF_IMAGE_SECTION_HEADER *sechdr = (const COFF_IMAGE_SECTION_HEADER *)dir;
	puts("#  Section  RVA        RFA        Size       PtrData    PtrReloc   NumReloc   PtrLineNo  NumLineNo  Char");
	int stab_rfa = 0, stabstr_rfa = 0, stab_len = 0, stabstr_len;
	for(i = 0; i < hdr->NumberOfSections; i++, sechdr++)
	{
		char temp[9];
		temp[8] = 0;
		memcpy(temp, sechdr->Name, COFF_IMAGE_SIZEOF_SHORT_NAME);
		if(!strcmp(temp, ".stab"))
		{
			stab_rfa = sechdr->PointerToRawData;
			stab_len = sechdr->Misc.VirtualSize;
		}
		else if(!strcmp(temp, ".stabstr"))
		{
			stabstr_rfa = sechdr->PointerToRawData;
			stabstr_len = sechdr->Misc.VirtualSize;
		}
		String line;
		line
		<< NFormat("%2<d %8<s 0x%08x 0x%08x 0x%08x ",
			i + 1,
			temp,
			(int)sechdr->VirtualAddress,
			(int)sechdr->PointerToRawData,
			(int)sechdr->Misc.VirtualSize)
		<< NFormat("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x",
			(int)sechdr->PointerToRawData,
			(int)sechdr->PointerToRelocations, (int)sechdr->NumberOfRelocations,
			(int)sechdr->PointerToLinenumbers, (int)sechdr->NumberOfLinenumbers,
			(int)sechdr->Characteristics);
		puts(line);
	}
	if(dump_globals && hdr->PointerToSymbolTable)
		RunSymbols(hdr->PointerToSymbolTable, hdr->NumberOfSymbols);
	if(dump_stabs && stab_rfa > 0 && stabstr_rfa > 0)
		RunStab(stab_rfa, stab_len, stabstr_rfa, stabstr_len);
}

void FileDump::Run(const char *fn)
{
	for(;;)
		if(!memicmp(fn, "stab;", 5))
		{
			fn += 5;
			dump_stabs = true;
		}
		else if(!memicmp(fn, "global;", 7))
		{
			fn += 7;
			dump_globals = true;
		}
		else
			break;
	filename = fn;
	if(!mapping.Open(filename = fn))
		throw Exc("file failed to open");
	if(mapping.GetCount() >= 0x40 && mapping[0] == 'M' && mapping[1] == 'Z'
	&& (ne_offset = PeekIL(&mapping[0x3C])) >= 0x40 && ne_offset < mapping.GetLength()
	&& PeekIL(&mapping[ne_offset]) == 'P' + 256 * 'E')
	{
		RunCOFF();
		return;
	}
	throw Exc("unknown file format");
}

void DumpFile(String fn)
{
	try
	{
		FileDump().Run(fn);
	}
	catch(Exc e)
	{
		throw Exc(NFormat("%s: %s", fn, e));
	}
}
