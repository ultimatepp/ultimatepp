#include "uld.h"
#pragma hdrstop

#include "obj.h"
#include "version.h"

#define LTIMING(x) // RTIMING(x)
#define LDUMP(x)   // RDUMP(x)

static const byte std_stub[] =
{
	0x4d, 0x5a, 0x90, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
	0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0e, 0x1f, 0xba, 0x0e, 0x00, 0xb4, 0x09, 0xcd, 0x21, 0xb8, 0x01, 0x4c, 0xcd, 0x21, 0x54, 0x68,
	0x69, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 0x61, 0x6d, 0x20, 0x63, 0x61, 0x6e, 0x6e, 0x6f,
	0x74, 0x20, 0x62, 0x65, 0x20, 0x72, 0x75, 0x6e, 0x20, 0x69, 0x6e, 0x20, 0x44, 0x4f, 0x53, 0x20,
	0x6d, 0x6f, 0x64, 0x65, 0x2e, 0x0d, 0x0d, 0x0a, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

String LinkJob::Symbol::ToString() const
{
	return NFormat("OBJ %5>d SEC %5>d VAL %8>d TYP %04x", obj_sec.object, obj_sec.section, value, type);
}

bool LinkJob::GlobalAddressOrder::operator () (int a, int b) const
{
	const Symbol& ga = linkjob.globals[a], & gb = linkjob.globals[b];
	if(ga.obj_sec.section < 0 || gb.obj_sec.section < 0) {
		if(ga.obj_sec.section != gb.obj_sec.section)
			return ga.obj_sec.section < gb.obj_sec.section;
		return ga.value < gb.value;
	}
	if(ga.obj_sec.object < 0 || gb.obj_sec.object < 0) {
		if(ga.obj_sec.object != gb.obj_sec.object)
			return ga.obj_sec.object < gb.obj_sec.object;
		return ga.value < gb.value;
	}
	int seca = linkjob.GetSection(ga.obj_sec).sec_map_index;
	int secb = linkjob.GetSection(gb.obj_sec).sec_map_index;
	if(seca < 0 || secb < 0)
		return seca > secb;
	int rva = linkjob.section_map[seca].rva - linkjob.section_map[secb].rva;
	if(rva) return rva < 0;
	if(ga.value != gb.value)
		return ga.value < gb.value;
	return langinfo(linkjob[linkjob.globals.GetKey(a)], linkjob[linkjob.globals.GetKey(b)]);
}

/*
unsigned LinkJob::Stab::GetHashValue() const
{
	CombineHash ch;
	ch.Put(::GetHashValue(text));
	ch.Put(MAKELONG(MAKEWORD(type, other), desc));
	ch.Put(value);
	ch.Put(fixup.object);
	ch.Put(fixup.section);
	ch.Put(fileatom);
	return ch;
}
*/

/*
bool LinkJob::Stab::Equals(const Stab& b) const
{
	return type == b.type && other == b.other
		&& desc == b.desc && value == b.value
		&& fixup == b.fixup && text == b.text
		&& fileatom == b.fileatom;
}
*/

/*
void LinkJob::Cache::Serialize(Stream& stream)
{
	stream % library % file % time % offset % size;
}
*/

bool LinkJob::Resource::Sort(const Resource& r, const Resource& s)
{
	if(r.type_id != s.type_id)
		return r.type_id < s.type_id;
	if(r.type != s.type)
		return r.type < s.type;
	if(r.name_id != s.name_id)
		return r.name_id < s.name_id;
	if(r.name != s.name)
		return r.name < s.name;
	return r.lang_id < s.lang_id;
}

LinkJob::LinkJob()
{
	start_time = msecs();

//	mangling_style = MANGLING_MSC;
	file_align = 0x200;
	image_align = 0x1000;
	image_base = 0;
	image_fixed = false;
	force_reloc = false;
	stack_reserve = 0x100000;
	stack_commit = 0x1000;
	heap_reserve = 0x100000;
	heap_commit = 0x1000;
	ignore_code_alignment = true;
	merge_gcc_sections = true;
	linkermode = (!CompareNoCase(GetFileTitle(GetExeFilePath()), "link") ? MODE_MSLINK : MODE_GNULD);
#ifdef _DEBUG
	dump_flags = DUMP_COMMANDLINE; //DUMP_STAT | DUMP_TIMING;
#else
	dump_flags = 0;
#endif
#if defined(CPU_IA32)
	machine = COFF_IMAGE_FILE_MACHINE_I386;
#else
	machine = COFF_IMAGE_FILE_MACHINE_UNKNOWN;
#endif
	write_mapfile = false;
	mapfile_stdout = false;
	write_xref = false;
	debug_info_raw = -1;
	debug_info = false;
	show_logo = true;
	autocollect_crt_only = true;
//	cache_objects = false;
	cache_object_data = false;
	static_libraries = true;
	make_dll = false;
	verbose = false;
	major_version = 1;
	minor_version = 1;
	auto_dll_base = false;
	subsystem = COFF_IMAGE_SUBSYSTEM_DEFAULT;
	atoms.Add(Null);
	text_atom = Atomize(".text");
	data_atom = Atomize(".data");
	edata_atom = Atomize(".edata");
	crt_atom = Atomize(".CRT");
	ctors_atom = Atomize(".ctors");
	dtors_atom = Atomize(".dtors");
	gcc_except_atom = Atomize(".gcc_except_table");
	CTOR_LIST_atom = Atomize("__CTOR_LIST__");
	_CTOR_LIST_atom = Atomize("___CTOR_LIST__");
	CTOR_LIST_END_atom = Atomize("__CTOR_LIST_END__");
	DTOR_LIST_atom = Atomize("__DTOR_LIST__");
	_DTOR_LIST_atom = Atomize("___DTOR_LIST__");
	DTOR_LIST_END_atom = Atomize("__DTOR_LIST_END__");
	idata_atom = atoms.FindAdd(".idata");
	idata_idesc_atom = atoms.FindAdd(".idata$2");
	idata_inull_atom = atoms.FindAdd(".idata$3");
	idata_iat1_atom = atoms.FindAdd(".idata$4");
	idata_iat2_atom = atoms.FindAdd(".idata$5");
	idata_names_atom = atoms.FindAdd(".idata$6");
	rdata_atom = atoms.FindAdd(".rdata");
	bss_atom = atoms.FindAdd(".bss");
	debug_atom = atoms.FindAdd(".debug");
	drectve_atom = Atomize(".drectve");
	stab_atom = Atomize(".stab");
	stabstr_atom = Atomize(".stabstr");
	rsrc_atom = Atomize(".rsrc");
	AddGlobal(Atomize("__image_base__"), Symbol(Null, 0, COFF_IMAGE_SYM_TYPE_BASE));
	AddGlobal(Atomize("__except_list"), Symbol(ObjSec(0, -1), 0));
	AddGlobal(Atomize("___safe_se_handler_count"), Symbol(ObjSec(0, -1), 2));

	stab_section = -1;
	stabstr_section = -1;
	symbol_section = -1;
	reloc_section = -1;
	has_stabs = false;
//	has_symbols = false;
//	has_relocs = false;
	idata_last = -1;
	idata_null_found = false;

	mapping_cache_limit = 100;

	{
		ASSERT(objects.GetCount() == OBJ_MARKER);
		ObjectFile& of = objects.Add(new ObjectFile(*this, OBJ_MARKER, Null, Null, ObjectFile::INTERNAL, 0, Null, 0));
		of.sections.SetCount(OBJM_COUNT);
		static const char *sec_names[] = { "..data", ".data$PSR", ".data$PSR", "..bss", ".~bss" };
		for(int i = 1; i < OBJM_COUNT; i++) {
			ObjectFile::Section& sec = of.sections[i];
			sec.name_atom = Atomize(sec_names[i - 1]);
			sec.sec_atom = (i < OBJM_BSS_BEGIN ? data_atom : bss_atom);
			sec.size = sec.raw_size = 0;
			sec.flags = (i < OBJM_BSS_BEGIN ? COFF_IMAGE_SCN_CNT_INITIALIZED_DATA : COFF_IMAGE_SCN_CNT_UNINITIALIZED_DATA)
				| (COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE | COFF_IMAGE_SCN_ALIGN_1BYTES);
			sec.type = ObjectFile::SEC_RAW;
			sec.autocollect = true;
		}
		AddGlobal(Atomize("__data_start__"), Symbol(ObjSec(OBJ_MARKER, OBJM_DATA_BEGIN)));
		AddGlobal(Atomize("__data_end__"), Symbol(ObjSec(OBJ_MARKER, OBJM_BSS_BEGIN)));
		AddGlobal(Atomize("___RUNTIME_PSEUDO_RELOC_LIST__"), Symbol(ObjSec(OBJ_MARKER, OBJM_PSEUDO_RELOC_BEGIN)));
		AddGlobal(Atomize("___RUNTIME_PSEUDO_RELOC_LIST_END__"), Symbol(ObjSec(OBJ_MARKER, OBJM_PSEUDO_RELOC_END)));
		AddGlobal(Atomize("__bss_start__"), Symbol(ObjSec(OBJ_MARKER, OBJM_BSS_BEGIN)));
		AddGlobal(Atomize("__bss_end__"), Symbol(ObjSec(OBJ_MARKER, OBJM_BSS_END)));
	}

	{
		ASSERT(objects.GetCount() == OBJ_CTOR);
		ObjectFile& of = objects.Add(new ObjectFile(*this, OBJ_CTOR, Null, Null, ObjectFile::INTERNAL, 0, Null, 0));
		of.sections.SetCount(OBJC_COUNT);
		static const char *sec_names[] = { ".ctors", ".ctors$_end", ".dtors", ".dtors$_end", ".idata$6", ".idata$3" };
		for(int i = 1; i < OBJC_COUNT; i++) {
			ObjectFile::Section& sec = of.sections[i];
			sec.name_atom = Atomize(sec_names[i - 1]);
			sec.sec_atom = (i >= OBJC_IDATA_HOLE ? idata_atom : data_atom);
			sec.size = sec.raw_size = (i == OBJC_IDATA_NULL ? sizeof(COFF_IMAGE_IMPORT_DESCRIPTOR) : 4);
			sec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA
				| COFF_IMAGE_SCN_MEM_READ | (i == OBJC_IDATA_NULL ? 0 : COFF_IMAGE_SCN_MEM_WRITE)
				| COFF_IMAGE_SCN_ALIGN_4BYTES;
			sec.section_data.SetCount(sec.size, i == OBJC_CTOR_BEGIN || i == OBJC_DTOR_BEGIN ? 0xFF : 0x00);
			sec.type = ObjectFile::SEC_RAW;
			sec.autocollect = true;
		}
	}

/*
	if(idata_last >= 0 && !idata_null_found) {
		ObjectFile& of = objects[idata_last];
		ObjectFile::Section& ofsec = of.sections.Add();
//		ObjectFile::Section& prev = of.sections[of.sections.GetCount() - 2];
		ofsec.name_atom = idata_inull_atom;
		ofsec.sec_atom = idata_atom;
		ofsec.size = sizeof(COFF_IMAGE_IMPORT_DESCRIPTOR);
		ofsec.raw_size = 0;
		ofsec.offset = 0;
		ofsec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA
			| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE;
		ofsec.ref_sec_index = of.ref_sections.GetCount();
		ofsec.ref_ext_index = of.ref_externals.GetCount();
		ofsec.sec_map_index = -1;
		of.used_any = ofsec.used = true;
		ofsec.type = ObjectFile::SEC_INULL;
	}
*/

	AddGlobal(CTOR_LIST_atom,     Symbol(ObjSec(OBJ_CTOR, OBJC_CTOR_BEGIN), 0, COFF_IMAGE_SYM_TYPE_CTOR_DTOR));
	AddGlobal(_CTOR_LIST_atom,    Symbol(ObjSec(OBJ_CTOR, OBJC_CTOR_BEGIN), 0, COFF_IMAGE_SYM_TYPE_CTOR_DTOR));
	AddGlobal(CTOR_LIST_END_atom, Symbol(ObjSec(OBJ_CTOR, OBJC_CTOR_END),   0, COFF_IMAGE_SYM_TYPE_CTOR_DTOR));
	AddGlobal(DTOR_LIST_atom,     Symbol(ObjSec(OBJ_CTOR, OBJC_DTOR_BEGIN), 0, COFF_IMAGE_SYM_TYPE_CTOR_DTOR));
	AddGlobal(_DTOR_LIST_atom,    Symbol(ObjSec(OBJ_CTOR, OBJC_DTOR_BEGIN), 0, COFF_IMAGE_SYM_TYPE_CTOR_DTOR));
	AddGlobal(DTOR_LIST_END_atom, Symbol(ObjSec(OBJ_CTOR, OBJC_DTOR_END),   0, COFF_IMAGE_SYM_TYPE_CTOR_DTOR));

	{
		ASSERT(objects.GetCount() == OBJ_EXPORT);
		ObjectFile& of = objects.Add(new ObjectFile(*this, OBJ_EXPORT, Null, Null, ObjectFile::INTERNAL, 0, Null, 0));
		of.sections.SetCount(OBJE_COUNT);
		static const char *sec_names[] = { ".edata$1dir", ".edata$2adr", ".edata$3ptr", ".edata$4ord", ".edata$5nam" };
		for(int i = 1; i < OBJE_COUNT; i++) {
			ObjectFile::Section& sec = of.sections[i];
			sec.name_atom = Atomize(sec_names[i - 1]);
			sec.sec_atom = edata_atom;
			sec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA
				| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE | COFF_IMAGE_SCN_ALIGN_4BYTES;
			sec.type = ObjectFile::SEC_RAW;
			sec.autocollect = true;
		}
	}

	{
		ASSERT(objects.GetCount() == OBJ_RSRC);
		ObjectFile& of = objects.Add(new ObjectFile(*this, OBJ_RSRC, Null, Null, ObjectFile::INTERNAL, 0, Null, 0));
		of.sections.SetCount(OBJR_COUNT);
		for(int i = 1; i < OBJR_COUNT; i++) {
			ObjectFile::Section& sec = of.sections[i];
			sec.name_atom = Atomize(".rsrc$" + FormatInt(i));
			sec.sec_atom = rsrc_atom;
			sec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_ALIGN_4BYTES;
			sec.type = ObjectFile::SEC_RAW;
			sec.autocollect = true;
		}
	}

	output_stab_info.SetCountR(STAB_STABSIZE, 0);
	output_stab_strings.SetCountR(1, 0);
}

String LinkJob::DemangleAtom(int atom) const
{
	return ::DemangleName(atoms[atom], linkermode == MODE_MSLINK ? MANGLING_MSC : MANGLING_GCC);
}

FileMapping& LinkJob::GetMapping(String filename)
{
	int f = mapping_cache.Find(filename);
	if(f >= 0)
		return mapping_cache[f].mapping;
	if(mapping_cache.GetCount() >= mapping_cache_limit) {
		for(int t = 0; t < mapping_cache.GetCount() && mapping_cache.GetCount() >= mapping_cache_limit; t++)
			if(mapping_cache[t].lock == 0)
				mapping_cache.Remove(t--);
	}
	One<Cache> nc = new Cache;
	nc->lock = 0;
	static FileMapping dummy;
	if(!nc->mapping.Open(filename))
		return dummy;
	Cache *pc = ~nc;
	mapping_cache.Add(filename, -nc);
	return pc->mapping;
}

void LinkJob::LockMapping(String filename)
{
	int f = mapping_cache.Find(filename);
	if(f >= 0)
		mapping_cache[f].lock++;
}

void LinkJob::UnlockMapping(String filename)
{
	int f = mapping_cache.Find(filename);
	if(f >= 0)
		mapping_cache[f].lock--;
}

void LinkJob::ReadCommand(const char *cmd, bool user)
{
	ReadCommand(SplitCmdArgs(cmd), user);
}

String LinkJob::Usage()
{
	return String().Cat()
		<< NFormat("uld COFF linker, version %d.%d.r%d, release date: %`\n%s",
			ULD_VERSION_MAJOR, ULD_VERSION_MINOR, ULD_VERSION_RELEASE, ULD_DATE, ULD_COPYRIGHT) << "\n"
		"\n"
		"Currently supported MS linker-compatible options:\n"
		"\n"
		"-align:<alignment factor:1,2,4,8,16,...65536>\n"
		"-alternatename:<name>=<alias name> .. define weak external\n"
		"-base:<image base address>\n"
		"-debug\n"
		"-defaultlib:<default library path>\n"
		"-fixed .. build fixed address executable\n"
		"-libpath:<library path>\n"
		"-machine:<one-of " << COFFMachineNames() << ">\n"
		"-map[:<map file>]\n"
		"-merge:<section>=<section>\n"
		"-nodefaultlib:<library to exclude from defaults>\n"
		"-disallowlib:<same as -nodefaultlib>\n"
		"-entry:<symbol> .. override default entry point\n"
		"-nologo .. suppress startup banner\n"
		"-out:<filename> .. name of exe file to produce\n"
		"-stub:<filename> .. file to use as DOS stub\n"
		"-subsystem:<one-of: NATIVE, CONSOLE, POSIX, WINDOWS,"
		" WINDOWSCE, OS2>\n"
		"-version:<major>[.<minor>] .. DLL version number\n"
		"-verbose .. display additional link information\n"
		"\n"
		"Currently supported GNU LD-compatible options:\n"
		"\n"
		"-Map <mapfile>"
		"--subsystem <one-of: windows, console>\n"
		"--map-file .. outputs on stdout\n"
		"--start-group, --end-group (ignored, automatic)\n"
		"--gc-sections (ignored, automatic)\n"
		"--major-image-version <major> .. major DLL version\n"
		"--minor-image-version <minor> .. minor DLL version\n"
		"-B <one-of: static, dynamic>\n"
		"-e <symbol> .. override default entry point\n"
		"-L <library path>\n"
		"-l <file to load as lib<file>.a>\n"
		"-M .. write mapfile on stdout\n"
		"-O ### .. optimization level (ignored)\n"
		"-o ### .. write output on file ###\n"
		"-s .. strip debug information from file\n"
		"\n"
		"uld-specific options:\n"
//		"-nocache .. do not cache link information in helper file\n"
//		"   (default = on)\n"
		"-collectdata .. automatically collect all initialized data\n"
		"   from any referenced object (default=CRT sections only)\n"
		"-aligncode .. honor alignment in code sections (default=off)\n"
		"-keepsections .. keep /xxx gcc code COMDAT section names\n"
		"   (default = convert to .text / .data / .bss)\n"
		"-dump:[stab;][global;]<exe file name> .. dump executable\n"
		"-dumpcollect .. dump object tree collector output\n"
		"-dumpcommand .. dump command line as C string constant\n"
		"-dumpdll .. dump exports from loaded dynamic libraries\n"
		"-dumpexports .. dump export systems with ordinals\n"
		"-dumpfixups .. dump fixups\n"
		"-dumpimp .. dump import libraries\n"
		"-dumpimplib .. dump contents of loaded import libraries\n"
		"-dumpobj .. dump object files\n"
		"-dumpobjlib .. dump contents of loaded object libraries\n"
		"-dumpresource .. dump parsed resource files\n"
		"-dumpsec[:all | :defines] .. dump sections\n"
		"     / all contributions / all contrib's + defined symbols\n"
		"-dumpstabs .. dump output stabs\n"
		"-dumpstabtypes .. dump stab collector output\n"
		"-dumpstat .. dump general linking statistics\n"
		"-dumptime .. dump linker phase timing\n"
		"-mapall .. write all symbols to map file\n"
		"-mapout .. write mapfile to <outputfile>.map\n"
		"-mapxref .. write cross references into map file\n"
		"-mapunused .. write unused linked in symbols to map file\n"
	;
}

void LinkJob::ReadCommand(const Vector<String>& cmdlist, bool user)
{
	if(user)
		command_args.Append(cmdlist);
	int start = msecs();
	for(int i = 0; i < cmdlist.GetCount(); i++) {
		String arg = cmdlist[i];
		bool is_opt = false;
		if(*arg == '/') {
			const char *p = arg;
			is_opt = true;
			while(*++p && *p != ':' && *p != '=' && *p != '/' && *p != '.')
				;
			is_opt = (*p == 0 || *p == ':');
		}

		if(is_opt || *arg == '-') {
			const char *ptr = ~arg + 1;
			const char *end = ptr;
			while(*end && *end != ':')
				end++;
			String cmd(ptr, end);
			String lcmd = ToLower(cmd);
			if(cmd == "Map") {
				linkermode = MODE_GNULD;
				write_mapfile = true;
				mapfile = FetchCmdArg(cmdlist, i);
				continue;
			}
			String val;
			if(*end == ':')
				val = end + 1;
			bool eaten = true;
			if(lcmd == "align") {
				int at = IsDigit(*val) ? ScanInt(val) : 4096;
				image_align = 0x100;
				while(image_align < at && image_align < 65536)
					image_align <<= 1;
				if(image_align < 0x1000)
					file_align = image_align;
			}
			else if(lcmd == "alternatename" && !IsNull(val)) {
				int q = val.Find('=');
				if(q > 0 && q < val.GetLength() - 1) {
					String name = val.Left(q), alias = val.Mid(q + 1);
					AddWeakExternal(Atomize(name), Atomize(alias));
				}
				else
					PutStdOut(NFormat("ALTERNATENAME: invalid parameter (expected ident=ident): %s", val));
			}
			else if(lcmd == "base")
				image_base = ScanCInt(val);
			else if(lcmd == "debug")
				debug_info_raw = 1;
			else if(lcmd == "defaultlib" && !IsNull(val))
				defaultlibs.FindAdd(NormalizePathCase(val));
			else if(lcmd == "entry" && !IsNull(val))
				entrypoint = val;
			else if(lcmd == "fixed")
				image_fixed = true;
			else if(lcmd == "include" && !IsNull(val))
				include_symbols.FindAdd(Atomize(val));
			else if(lcmd == "incremental") {
				PutStdOut("(INCREMENTAL: currently ignored)");
			}
			else if(lcmd == "libpath" && !IsNull(val))
				libpaths.Add(val);
			else if(lcmd == "machine" && !IsNull(val)) {
				val = ToUpper(val);
				machine = COFFMachineCode(ToUpper(val));
				if(machine == COFF_IMAGE_FILE_MACHINE_UNKNOWN)
					PutStdOut(NFormat("Unknown machine type: %s; supported machines are: %s", val, COFFMachineNames()));
			}
			else if(lcmd == "map") {
				write_mapfile = true;
//				write_xref = true;
				if(!IsNull(val))
					mapfile = val;
			}
			else if(lcmd == "merge") {
				const char *p = val;
				while(*p && *p != '=')
					p++;
				String from(~val, p);
				if(*p++ != '=' || !*p || IsNull(from))
					PutStdOut("Invalid syntax of /merge command.");
				else
					section_merge.GetAdd(Atomize(from)) = Atomize(p);
			}
			else if((lcmd == "nodefaultlib" || lcmd == "disallowlib") && !IsNull(val))
				nodefaultlibs.FindAdd(NormalizePathCase(val));
			else if(lcmd == "nologo")
				show_logo = false;
			else if(lcmd == "opt") {
				PutStdOut("(OPT: currently ignored)");
			}
			else if(lcmd == "out") {
				if(!IsNull(outputfile))
					PutStdOut(NFormat("Duplicate output file: %s (was %s)", val, outputfile));
				outputfile = val;
			}
			else if(lcmd == "pdb") {
				PutStdOut("(PDB: currently ignored)");
			}
			else if(lcmd == "stub") {
				if(!IsNull(stub_filename))
					PutStdOut(NFormat("Duplicate stub file name: %s (was %s)", val, stub_filename));
				stub_filename = val;
			}
			else if(lcmd == "subsystem" && !IsNull(val)) {
				val = ToUpper(val);
				/**/ if(val == "CONSOLE")   subsystem = COFF_IMAGE_SUBSYSTEM_WINDOWS_CUI;
				else if(val == "NATIVE")    subsystem = COFF_IMAGE_SUBSYSTEM_NATIVE;
				else if(val == "POSIX")     subsystem = COFF_IMAGE_SUBSYSTEM_POSIX_CUI;
				else if(val == "WINDOWS")   subsystem = COFF_IMAGE_SUBSYSTEM_WINDOWS_GUI;
#ifdef COMPILER_MSC
				else if(val == "WINDOWSCE") subsystem = COFF_IMAGE_SUBSYSTEM_WINDOWS_CE_GUI;
#endif
				else if(val == "OS2")       subsystem = COFF_IMAGE_SUBSYSTEM_OS2_CUI;
				else
					PutStdOut(NFormat("Unknown subsystem: %s", val));
			}
			else if(lcmd == "version" && !IsNull(val)) {
				const char *p = val;
				major_version = (word)ScanInt(p, &p);
				if(*p++ == '.' && IsDigit(*p))
					minor_version = (word)ScanInt(p);
			}
			else
				eaten = false;
			if(eaten)
				linkermode = MODE_MSLINK;
			else {
				eaten = true;
				if(lcmd == "aligncode")
					ignore_code_alignment = false;
				else if(lcmd == "collectdata")
					autocollect_crt_only = false;
//				else if(lcmd == "nocache")
//					cache_objects = false;
				else if(lcmd == "dump" && !IsNull(val)) {
					void DumpFile(String fn);
					DumpFile(val);
				}
				else if(lcmd == "dumpcollect")
					dump_flags |= DUMP_COLLECTOR;
				else if(lcmd == "dumpcommand")
					dump_flags |= DUMP_COMMANDLINE;
				else if(lcmd == "dumpdll")
					dump_flags |= DUMP_DLL_EXPORTS;
				else if(lcmd == "dumpexports")
					dump_flags |= DUMP_EXPORTS;
				else if(lcmd == "dumpfixups")
					dump_flags |= DUMP_REF_FIXUPS;
				else if(lcmd == "dumpimp")
					dump_flags |= DUMP_IMPORT;
				else if(lcmd == "dumpimplib")
					dump_flags |= DUMP_IMPORT_LIBS;
				else if(lcmd == "dumpobj")
					dump_flags |= DUMP_OBJECTS;
				else if(lcmd == "dumpobjlib")
					dump_flags |= DUMP_OBJECT_LIBS;
				else if(lcmd == "dumpresource")
					dump_flags |= DUMP_RESOURCES;
				else if(lcmd == "dumpsec")
					dump_flags |= (val == "def" ? DUMP_SEC_DEFINES : val == "all" ? DUMP_SECTIONS_ALL : DUMP_SECTIONS);
				else if(lcmd == "dumpstabs")
					dump_flags |= DUMP_STABS;
				else if(lcmd == "dumpstabtypes")
					dump_flags |= DUMP_STAB_TYPES;
				else if(lcmd == "dumpstat")
					dump_flags |= DUMP_STAT;
				else if(lcmd == "dumptime")
					dump_flags |= DUMP_TIMING;
				else if(lcmd == "mapall")
					dump_flags |= DUMP_MAP_ALL;
				else if(lcmd == "mapout")
					write_mapfile = true;
				else if(lcmd == "mapxref")
					write_xref = true;
				else if(lcmd == "mapunused")
					dump_flags |= DUMP_MAP_UNUSED;
				else if(lcmd == "verbose")
					verbose = true;
				else
					eaten = false;
			}
			if(!eaten) {
				ptr = ~arg + 1;
				const char *pfx;
				eaten = true;
				switch(*ptr++) {
				case '-':
					if(pfx = EatPrefix(ptr, "dll-search-prefix="))
						dll_search_prefix = pfx;
					else if(!strcmp(ptr, "dynamic"))
						static_libraries = false;
					else if(!strcmp(ptr, "enable-auto-image-base"))
						auto_dll_base = true;
					else if(!strcmp(ptr, "gc-sections"))
						; // automatic for the moment
					else if(!strcmp(ptr, "major-image-version"))
						major_version = (word)ScanInt(FetchCmdArg(cmdlist, i));
					else if(!strcmp(ptr, "map-file"))
						write_mapfile = mapfile_stdout = true;
					else if(!strcmp(ptr, "minor-image-version"))
						minor_version = (word)ScanInt(FetchCmdArg(cmdlist, i));
					else if(!strcmp(ptr, "start-group") || !strcmp(ptr, "end-group"))
						; // ignore these for the moment
					else if(!strcmp(ptr, "static"))
						static_libraries = true;
					else if(!strcmp(ptr, "shared"))
						make_dll = true;
					else if(!strcmp(ptr, "subsystem")) {
						String sub = FetchCmdArg(cmdlist, i);
						if(sub == "windows") {
							if(subsystem != COFF_IMAGE_SUBSYSTEM_WINDOWS_CUI)
								subsystem = COFF_IMAGE_SUBSYSTEM_WINDOWS_GUI;
						}
						else if(sub == "console")
							subsystem = COFF_IMAGE_SUBSYSTEM_WINDOWS_CUI;
						else
							PutStdOut(NFormat("Unknown --subsystem: %s", sub));
					}
					else
						eaten = false;
					break;

				case 'B': {
						String style = FetchCmdArg(cmdlist, i, ptr);
						if(style == "static")
							static_libraries = true;
						else if(style == "dynamic")
							static_libraries = true;
						else if(style == "shareable")
							make_dll = true;
						else
							eaten = false;
						break;
					}

				case 'e':
					entrypoint = FetchCmdArg(cmdlist, i, ptr);
					break;

				case 'L':
					libpaths.Add(FetchCmdArg(cmdlist, i, ptr));
					break;

				case 'l':
					LoadFile(String().Cat() << "lib" << ptr << ".a", false);
					break;

				case 'M':
					write_mapfile = true;
					mapfile_stdout = true;
					break;

				case 'O':
					ptr = FetchCmdArg(cmdlist, i, ptr);
					PutStdOut(NFormat("(option '-O %s' ignored)", ptr));
					break;

				case 'o':
					outputfile = FetchCmdArg(cmdlist, i, ptr);
					break;

				case 's':
					debug_info_raw = 0;
					break;

				default:
					eaten = false;
					break;
				}
				if(eaten)
					linkermode = MODE_GNULD;
			}
			if(!eaten)
				PutStdOut(NFormat("Unknown option: %s", arg));
		}
		else
			LoadFile(arg, false);
/*
		FindFile ff;
		if(ff.Search(arg))
			do
				if(ff.IsFile()) {
					String fn = AppendFileName(GetFileDirectory(arg), ff.GetName());
					PutStdOut(fn);
					fflush(stdout);
					LoadFile(fn);
				}
			while(ff.Next());
*/
	}
}

String FindLinkFile(String fn, String dir, bool has_ext)
{
	String natdir = NormalizePathCase(NativePath(dir));
	String nat = NormalizePathCase(NativePath(fn));
	String out;
	if(FileExists(out = AppendFileName(natdir, nat)))
		return out;
	if(has_ext)
		return Null;
	if(FileExists(out = AppendFileName(natdir, nat + ".obj"))
	|| FileExists(out = AppendFileName(natdir, nat + ".o"))
	|| FileExists(out = AppendFileName(natdir, nat + ".lib"))
	|| FileExists(out = AppendFileName(natdir, nat + ".a"))
	|| FileExists(out = AppendFileName(natdir, "lib" + nat + ".a")))
		return out;
	return Null;
}

void LinkJob::LoadFile(String fn, bool defaultlib)
{
	LTIMING("LinkJob::LoadFile");
	String relfn = NormalizeRelPath(NormalizePathCase(fn));
	if(loaded_files.Find(relfn) >= 0)
		return;
	bool has_ext = *GetFileExtPos(relfn);
	String file;
	ObjectFile::FILETYPE ft = ObjectFile::FULL_PATH;
	file = FindLinkFile(relfn, Null, has_ext);
	if(IsNull(file) && !IsNull(dll_search_prefix))
		file = FindLinkFile(AppendFileName(GetFileDirectory(relfn), dll_search_prefix + GetFileName(relfn)), Null, has_ext);
#ifdef PLATFORM_WIN32
	String cygfn = TranslateCygWinPath(fn);
	if(IsNull(file) && !IsNull(cygfn))
		file = FindLinkFile(cygfn, Null, has_ext);
#endif
	if(IsNull(file) && !IsFullPath(relfn)) {
		ft = ObjectFile::LIB_PATH;
		for(int i = 0; i < libpaths.GetCount(); i++) {
			if(!IsNull(file = FindLinkFile(relfn, libpaths[i], has_ext)))
				break;
#ifdef PLATFORM_WIN32
			if(!IsNull(cygfn = TranslateCygWinPath(libpaths[i])) && !IsNull(file = FindLinkFile(relfn, cygfn, has_ext)))
				break;
#endif
		}
	}
	if(IsNull(outputfile))
		outputfile = ForceExt(file, "");
//	if(cache_objects && IsNull(cache_file_name))
//		cache_file_name = ForceExt(outputfile, ".uld");
	if(defaultlib)
		ft = ObjectFile::DEFAULTLIB;
	FileMapping& mapping = LinkJob::GetMapping(file);
	if(!mapping.IsOpen()) {
		PutStdOut(NFormat("File '%s' not found.", fn));
		return;
	}
	if(loaded_files.Find(file) >= 0)
		return;
	loaded_files.Add(fn);
//	loaded_files.Add(full);
	loaded_files.Add(file);
	if(dump_flags & DUMP_OBJECTS)
		PutStdOut(file);
	if(mapping[0] == 'M' && mapping[1] == 'Z') {
		LoadDLL(file, mapping, ft);
		return;
	}
	if(*(const word *)~mapping == machine) {
		if(ft == ObjectFile::FULL_PATH)
			ft = ObjectFile::DIRECT;
		LoadObject(file, mapping, ft);
		return;
	}
	if(!memcmp(~mapping, "!<arch>\n", 8)) {
		LoadLibrary(file, mapping, ft);
		return;
	}
	if(mapping.GetLength() >= 8) {
		int long1 = PeekIL(&mapping[0]), long2 = PeekIL(&mapping[4]);
		if(long1 >= 0 && long2 >= 0 && long1 + long2 <= mapping.GetLength()) {
			LoadResource(file, mapping);
			return;
		}
	}
	PutStdOut(NFormat("File '%s' has unknown type.", file));
}

void LinkJob::LoadObject(String file, const FileMapping& mapping, ObjectFile::FILETYPE filetype)
{
	LTIMING("LinkJob::LoadObject");
	if(verbose)
		PutStdOut(NFormat("%s: loading object file", file));
	int nobj = objects.GetCount();
	ObjectFile& obj = objects.Add(new ObjectFile(*this, nobj, Null, file, filetype, 0, mapping.GetTime(), mapping.GetLength()));
	if(filetype == ObjectFile::DIRECT)
		obj.autocollect = true;
	if(cache_object_data) {
		obj.object_data.SetCount(obj.object_size);
		memcpy(obj.object_data.Begin(), mapping.Begin(), obj.object_size);
		obj.ReadFile(obj.object_data.Begin());
	}
	else
		obj.ReadFile(~mapping);
}

void LinkJob::LoadLibrary(String file, const FileMapping& mapping, ObjectFile::FILETYPE filetype)
{
	if(verbose)
		PutStdOut(NFormat("%s: loading static library", file));
	LTIMING("LinkJob::LoadLibrary");
	int objcount = -1;
	String libprefix = GetFileTitle(file) + ":";
	const byte *ptr = mapping.GetIter(8);
	const byte *end = mapping.End();
	const byte *longptr = NULL;
//	int objid = 0;
	Time file_time = mapping.GetTime();
	enum { HEADER_SIZE = sizeof(COFF_IMAGE_ARCHIVE_MEMBER_HEADER) };
	while(ptr + HEADER_SIZE <= end && ptr[HEADER_SIZE - 2] == COFF_IMAGE_ARCHIVE_END1
		&& ptr[HEADER_SIZE - 1] == COFF_IMAGE_ARCHIVE_END2) {
//		if(*ptr == '\n')
//			ptr++;
		int offset = ptr - mapping.Begin();
		COFF_IMAGE_ARCHIVE_MEMBER_HEADER hdr;
		memcpy(&hdr, ptr, sizeof(hdr));
		ptr += sizeof(hdr);
		int size = atoi((const char *)hdr.Size);
		const byte *brk = ptr + ((size + 1) & -2);
		if(hdr.Name[0] == '/' && hdr.Name[1] == '/') { // longnames
			longptr = ptr;
		}
		else if(hdr.Name[0] != '/' || hdr.Name[1] != ' ') { // object
			const byte *nameptr, *namelim;
			if(hdr.Name[0] != '/') {
				nameptr = hdr.Name;
				namelim = nameptr + __countof(hdr.Name);
			}
			else {
				int loff = atoi((const char *)hdr.Name + 1);
				nameptr = longptr + loff;
				namelim = end;
			}
			const byte *p = nameptr;
			while(p < namelim && *p && *p != '/')
				p++;
			String objname(nameptr, p - nameptr);
			String modname = libprefix + objname;
			const COFF_IMPORT_OBJECT_HEADER *imphdr = (const COFF_IMPORT_OBJECT_HEADER *)ptr;
			if(imphdr->Sig1 == 0 && imphdr->Sig2 == 0xFFFF) {
				if(imphdr->Machine != machine)
					throw Exc(NFormat("%s: invalid machine type (%d) in import library module", file, imphdr->Machine));
				const char *pname = (const char *)ptr + sizeof(COFF_IMPORT_OBJECT_HEADER);
				atom_t impsym_atom = Atomize(pname);
				atom_t dll_atom = Atomize(NormalizePathCase(pname + strlen(pname) + 1));
				if(dump_flags & DUMP_IMPORT_LIBS) {
					String iname = atoms[impsym_atom];
					PutStdOut(NFormat(iname == pname ? "%18<s %08x  @%5>d  %s" : "%18<s %08x  @%5>d  %s -> %s",
						atoms[dll_atom], offset, imphdr->Hint, pname, atoms[impsym_atom]));
				}
				int objn = dll_objects.Get(dll_atom, -1);
				if(objn < 0) {
					dll_objects.Add(dll_atom, objn = objects.GetCount());
					ObjectFile& obj = objects.Add(new ObjectFile(*this, objn, file, objname, filetype, 0, file_time, size));
					obj.dll_atom = dll_atom;
					obj.sections.SetCount(ObjectFile::DLL_SECTIONS);
					ObjectFile::Section& stubsec = obj.sections[ObjectFile::SEC_DLL_STUBS];
					stubsec.name_atom = stubsec.sec_atom = text_atom;
					stubsec.type = ObjectFile::SEC_RAW;
					stubsec.flags = COFF_IMAGE_SCN_CNT_CODE | COFF_IMAGE_SCN_ALIGN_1BYTES
						| COFF_IMAGE_SCN_MEM_EXECUTE | COFF_IMAGE_SCN_MEM_READ;
					ObjectFile::Section& namesec = obj.sections[ObjectFile::SEC_DLL_NAMES];
					namesec.name_atom = idata_names_atom;
					namesec.sec_atom = idata_atom;
					namesec.type = ObjectFile::SEC_RAW;
					namesec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_ALIGN_2BYTES | COFF_IMAGE_SCN_MEM_READ;
					ObjectFile::Section& impsec = obj.sections[ObjectFile::SEC_DLL_IMPORTS];
					impsec.name_atom = idata_iat1_atom;
					impsec.sec_atom = idata_atom;
					impsec.type = ObjectFile::SEC_RAW;
					impsec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_ALIGN_4BYTES
						| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE;
					ObjectFile::Section& boundsec = obj.sections[ObjectFile::SEC_DLL_BOUND];
					boundsec.name_atom = idata_iat2_atom;
					boundsec.sec_atom = idata_atom;
					boundsec.type = ObjectFile::SEC_RAW;
					boundsec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_ALIGN_4BYTES
						| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE;
					ObjectFile::Section& descsec = obj.sections[ObjectFile::SEC_DLL_DESCRIPTOR];
					descsec.name_atom = idata_idesc_atom;
					descsec.sec_atom = idata_atom;
					descsec.type = ObjectFile::SEC_RAW;
					descsec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_ALIGN_4BYTES
						| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE;
/*
					ObjectFile::Section& nullsec = obj.sections[ObjectFile::SEC_DLL_NULL_DESC];
					nullsec.name_atom = idata_inull_atom;
					nullsec.sec_atom = idata_atom;
					nullsec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_ALIGN_4BYTES
						| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE;
*/
				}
				ObjectFile::Import& dllimp = objects[objn].imports.Add();
				dllimp.app_atom = impsym_atom;
				dllimp.ordinal_hint = imphdr->Hint;
				switch(imphdr->NameType) {
				case COFF_IMPORT_OBJECT_ORDINAL:
					dllimp.ordinal = true;
					dllimp.ordinal_hint = imphdr->Ordinal;
					while(*pname == '_')
						pname++;
					break;

				case COFF_IMPORT_OBJECT_NAME:
					dllimp.imp_atom = impsym_atom;
					break;

				case COFF_IMPORT_OBJECT_NAME_NO_PREFIX:
				case COFF_IMPORT_OBJECT_NAME_UNDECORATE:
					while(*pname == '@' || *pname == '_' || *pname == '?')
						pname++; {
						const char *end = pname;
						if(imphdr->NameType == COFF_IMPORT_OBJECT_NAME_NO_PREFIX)
							end += strlen(end);
						else
							while(*end && *end != '@')
								end++;
						dllimp.imp_atom = Atomize(String(pname, end));
					}
					break;

				default:
					throw Exc(NFormat("%s: invalid import name type (%d)", modname, imphdr->NameType));
				}
				Symbol global;
				global.obj_sec.object = objn;
				global.value = objects[objn].imports.GetCount() - 1;
				if(imphdr->Type == COFF_IMPORT_CODE) {
					global.obj_sec.section = ObjectFile::SEC_DLL_STUBS;
					global.type = COFF_IMAGE_SYM_TYPE_IMPORT_STUB;
					AddGlobal(impsym_atom, global);
				}
				global.obj_sec.section = ObjectFile::SEC_DLL_BOUND;
				int import_atom = Atomize(String("__imp__") + pname);
				global.type = (imphdr->NameType == COFF_IMPORT_OBJECT_ORDINAL
					? COFF_IMAGE_SYM_TYPE_IMPORT_ORDINAL : COFF_IMAGE_SYM_TYPE_IMPORT_NAME);
				AddGlobal(import_atom, global);
//				RLOG("Import atom " << atoms[import_atom] << " in " << modname);
			}
			else {
				int nobj = objects.GetCount();
				if(objcount < 0)
					objcount = nobj;
				ObjectFile& obj = objects.Add(new ObjectFile(*this, nobj, file, objname,
					filetype, ptr - mapping.Begin(), file_time, size));
//				if(cache_object_data && filetype != ObjectFile::DEFAULTLIB)
//				{
//					obj.object_data = String(ptr, size);
//					obj.ReadFile(obj.object_data);
//				}
//				else
					obj.ReadFile(ptr);
				if(dump_flags & DUMP_OBJECT_LIBS) {
					for(int g = 0; g < globals.GetCount(); g++) {
						const Symbol& sym = globals[g];
						if(sym.obj_sec.object != nobj || sym.type >= COFF_IMAGE_SYM_TYPE_SPECIAL)
							continue;
						const ObjectFile& of = objects[sym.obj_sec.object];
						String secname;
						if(sym.obj_sec.section < 0)
							secname = "ABS";
						else if(sym.obj_sec.section == 0)
							secname = "COMDAT";
						else
							secname = atoms[of.sections[sym.obj_sec.section].name_atom];
						PutStdOut(NFormat("%15<s  %12<s  %12<s  %08x  %s",
							GetFileName(of.library_file), GetFileName(of.object_file), secname, sym.value, DemangleAtom(globals.GetKey(g))));
					}
				}
			}
		}
		ptr = brk;
	}
}

void LinkJob::LoadDLL(String file, const FileMapping& mapping, ObjectFile::FILETYPE filetype)
{
	try {
		if(verbose)
			PutStdOut(NFormat("%s: loading shared library", file));
		unsigned header;
		if(mapping.GetLength() < 0x40 || (header = PeekIL(&mapping[0x3C]))
			+ 4 + sizeof(COFF_IMAGE_FILE_HEADER) + sizeof(COFF_IMAGE_OPTIONAL_HEADER32) > (unsigned)mapping.GetLength()
		|| PeekIL(&mapping[header]) != 'P' + 256 * 'E')
			throw Exc("not a valid PE file");
		COFF_IMAGE_FILE_HEADER hdr;
		COFF_IMAGE_OPTIONAL_HEADER32 opthdr;
		memcpy(&hdr, &mapping[header + 4], sizeof(hdr));
		if(hdr.Machine != machine)
			throw Exc(NFormat("invalid machine code: %s", COFFMachineName(hdr.Machine)));
		if(!(hdr.Characteristics & IMAGE_FILE_DLL))
			throw Exc("not a dynamic library");
		if(hdr.SizeOfOptionalHeader < sizeof(opthdr))
			throw Exc(NFormat("invalid optional header size = %d", hdr.SizeOfOptionalHeader));
		memcpy(&opthdr, &mapping[header + 4 + sizeof(hdr)], sizeof(opthdr));
		if(opthdr.NumberOfRvaAndSizes < COFF_IMAGE_DIRECTORY_ENTRY_EXPORT)
			throw Exc("not enough data directories in image (EXPORT section not present)");
		unsigned export_rva = opthdr.DataDirectory[COFF_IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
		unsigned export_end = export_rva + opthdr.DataDirectory[COFF_IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
		Buffer<COFF_IMAGE_SECTION_HEADER> sections(hdr.NumberOfSections);
		memcpy(sections, &mapping[header + 4 + sizeof(hdr) + hdr.SizeOfOptionalHeader],
			hdr.NumberOfSections * sizeof(COFF_IMAGE_SECTION_HEADER));
		int rva_fpos = 0;
		bool found = false;
		for(int i = 0; i < hdr.NumberOfSections; i++) {
			const COFF_IMAGE_SECTION_HEADER& sec = sections[i];
			if(export_rva >= sec.VirtualAddress && export_end <= sec.VirtualAddress + sec.Misc.VirtualSize) {
				rva_fpos = sec.PointerToRawData - sec.VirtualAddress;
				found = true;
				break;
			}
		}
		if(!found)
			throw Exc("invalid export section RVA (not in any section)");
		COFF_IMAGE_EXPORT_DIRECTORY_TABLE expdir;
		memcpy(&expdir, &mapping[export_rva + rva_fpos], sizeof(expdir));

		atom_t dll_atom = Atomize(ToUpper(GetFileName(file)));
		int objn = objects.GetCount();
		dll_objects.Add(dll_atom, objn);
		ObjectFile& obj = objects.Add(new ObjectFile(*this, objn, Null, file, filetype, 0, mapping.GetTime(), mapping.GetLength()));
		obj.dll_atom = dll_atom;
		obj.sections.SetCount(ObjectFile::DLL_SECTIONS);
		ObjectFile::Section& stubsec = obj.sections[ObjectFile::SEC_DLL_STUBS];
		stubsec.name_atom = stubsec.sec_atom = text_atom;
		stubsec.flags = COFF_IMAGE_SCN_CNT_CODE | COFF_IMAGE_SCN_ALIGN_1BYTES
			| COFF_IMAGE_SCN_MEM_EXECUTE | COFF_IMAGE_SCN_MEM_READ;
		ObjectFile::Section& namesec = obj.sections[ObjectFile::SEC_DLL_NAMES];
		namesec.name_atom = idata_names_atom;
		namesec.sec_atom = idata_atom;
		namesec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_ALIGN_2BYTES | COFF_IMAGE_SCN_MEM_READ;
		ObjectFile::Section& impsec = obj.sections[ObjectFile::SEC_DLL_IMPORTS];
		impsec.name_atom = idata_iat1_atom;
		impsec.sec_atom = idata_atom;
		impsec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_ALIGN_4BYTES
			| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE;
		ObjectFile::Section& boundsec = obj.sections[ObjectFile::SEC_DLL_BOUND];
		boundsec.name_atom = idata_iat2_atom;
		boundsec.sec_atom = idata_atom;
		boundsec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_ALIGN_4BYTES
			| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE;
		ObjectFile::Section& descsec = obj.sections[ObjectFile::SEC_DLL_DESCRIPTOR];
		descsec.name_atom = idata_idesc_atom;
		descsec.sec_atom = idata_atom;
		descsec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_ALIGN_4BYTES
			| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE;

		if(dump_flags & DUMP_DLL_EXPORTS)
			PutStdOut(NFormat("%s: %d named export(s):", file, (int)expdir.NumberOfNamePointers));
		for(int i = 0; i < (int)expdir.NumberOfNamePointers; i++) {
			int nameptr = PeekIL(&mapping[expdir.NamePointerTableRVA + rva_fpos + 4 * i]);
			String name = (const char *)&mapping[nameptr + rva_fpos];
			word ordinal = PeekIW(&mapping[expdir.OrdinalTableRVA + rva_fpos + 2 * i]);
			if(ordinal >= (int)expdir.AddressTableEntries)
				throw Exc(NFormat("export symbol %s has invalid ordinal: %d (# address entries = %d)",
					name, ordinal, (int)expdir.AddressTableEntries));
			unsigned bound = PeekIL(&mapping[expdir.ExportAddressTableRVA + rva_fpos + 4 * ordinal]);

			if(dump_flags & DUMP_DLL_EXPORTS)
				PutStdOut(NFormat("\t(%d) %s", (int)(ordinal + expdir.OrdinalBase), name));
			ObjectFile::Import& dllimp = obj.imports.Add();
			dllimp.app_atom = Atomize("_" + name);
			dllimp.imp_atom = Atomize(name);
			dllimp.ordinal_hint = ordinal + expdir.OrdinalBase;
			Symbol global;
			global.obj_sec.object = objn;
			global.value = obj.imports.GetCount() - 1;
			unsigned sec_flags = 0;
			for(int s = 0; s < hdr.NumberOfSections; s++) {
				const COFF_IMAGE_SECTION_HEADER& sec = sections[s];
				if(bound >= sec.VirtualAddress && bound < sec.VirtualAddress + sec.Misc.VirtualSize) {
					sec_flags = sec.Characteristics;
					break;
				}
			}
			if(sec_flags & COFF_IMAGE_SCN_CNT_CODE) {
				global.obj_sec.section = ObjectFile::SEC_DLL_STUBS;
				global.type = COFF_IMAGE_SYM_TYPE_IMPORT_STUB;
				AddGlobal(dllimp.app_atom, global);
			}
			global.obj_sec.section = ObjectFile::SEC_DLL_BOUND;
			int import_atom = Atomize("__imp__" + name);
			global.type = COFF_IMAGE_SYM_TYPE_IMPORT_NAME;
			AddGlobal(import_atom, global);
		}
	}
	catch(Exc e) {
		PutStdOut(NFormat("%s: %s", file, e));
	}
}

static const byte *ResString(const byte *ptr, WString& out, int& out_id)
{
	int len = PeekIW(ptr);
	if(len == 0xFFFF) {
		out_id = PeekIW(ptr + 2);
		out = Null;
		ptr += 4;
	}
	else {
		out_id = Null;
		memcpy(out.GetBuffer(len >> 1), ptr + 2, 2 * (len >> 1));
		out.ReleaseBuffer(len >> 1);
		ptr += (len + 5) & -4;
	}
	return ptr;
}

void LinkJob::LoadResource(String file, const FileMapping& mapping)
{
	const byte *data = mapping.Begin(), *end = mapping.End();
	int resx = resource_objects.GetCount();
	while(data + 0x20 <= end) {
		int datalen = PeekIL(data);
		int hdrlen = PeekIL(data + 4);
		int skipnext = (datalen + hdrlen + 3) & -4;
		if(datalen < 0 || hdrlen < 0x20 || skipnext > end - data) {
			PutStdOut(NFormat("%s: invalid resource at offset %08x", file, data - mapping.Begin()));
			return;
		}
		Resource res;
		res.data = String(data + hdrlen, datalen);
		const byte *next = data + skipnext;
		data = ResString(data + 8, res.type, res.type_id);
		data = ResString(data, res.name, res.name_id);
		res.lang_id = PeekIW(data + 6);
		if(datalen > 0)
			resource_objects.Add(res);
		data = next;
	}

	if(dump_flags & DUMP_RESOURCES) {
		PutStdOut(NFormat("Resource dump, %d objects (%s):", resource_objects.GetCount() - resx, file));
		PutStdOut("Type      Name      Lang  Size");
		while(resx < resource_objects.GetCount()) {
			const Resource& rsrc = resource_objects[resx++];
			String r;
			if(!IsNull(rsrc.type))
				r << String(rsrc.type);
			else
				r << FormatIntHex(rsrc.type_id);
			r.Cat(' ', max(1, 10 - r.GetLength()));
			if(!IsNull(rsrc.name))
				r << String(rsrc.name);
			else
				r << FormatIntHex(rsrc.name_id);
			r.Cat(' ', max(1, 20 - r.GetLength()));
			PutStdOut(NFormat("%s%04x  %08x", r, rsrc.lang_id, rsrc.data.GetLength()));
		}
	}

/*
	ObjectFile& obj = objects.Add(new ObjectFile(*this, objects.GetCount(), Null, file, ObjectFile::INTERNAL, 0, mapping.GetTime(), mapping.GetLength()));
	obj.autocollect = true;
	obj.sections.SetCount(2);
	ObjectFile::Section& data = obj.sections[1];
	data.name_atom = data.sec_atom = rsrc_atom;
	data.autocollect = true;
	data.type = ObjectFile::SEC_RAW;
	data.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_ALIGN_4BYTES | COFF_IMAGE_SCN_MEM_READ;
	data.size = data.raw_size = mapping.GetLength();
	data.section_data.SetCount(mapping.GetLength());
	memcpy(data.section_data.Begin(), mapping.Begin(), data.size);
*/
}

void LinkJob::Dump() const
{
	RLOG("LinkJob(" << objects.GetCount() << " objects)");
	int i;
	RLOG("DefaultLibs: " << defaultlibs.GetCount());
	for(i = 0; i < defaultlibs.GetCount(); i++)
		RLOG(NFormat("defaultlib[%d] = %s, %s", i, defaultlibs[i],
			nodefaultlibs.Find(defaultlibs[i]) >= 0 ? "ignored" : "used"));
	RLOG("NoDefaultLibs: " << nodefaultlibs.GetCount());
	for(i = 0; i < nodefaultlibs.GetCount(); i++)
		RLOG(NFormat("nodefaultlib[%d] = %s", i, nodefaultlibs[i]));
	int atom_size = (16 + 16) * atoms.GetCount();
	for(i = 0; i < atoms.GetCount(); i++)
		atom_size += atoms[i].GetLength();
	RLOG("#atoms = " << atoms.GetCount() << ", size = " << atom_size);
	RLOG("#globals = " << globals.GetCount() << ", size = " << globals.GetCount() * (sizeof(Symbol) + 16));
	for(i = 0; i < globals.GetCount(); i++)
		RLOG(NFormat("[%3>d]: %30<s = %s", i, atoms[globals.GetKey(i)], globals[i].ToString()));
	RLOG("#objects = " << objects.GetCount());
//	for(i = 0; i < objects.GetCount(); i++)
//		objects[i].Dump();
}

void LinkJob::AddWeakExternal(atom_t atom, int refatom)
{
	for(int i = weak_externals.Find(atom); i >= 0; i = weak_externals.FindNext(atom))
		if(weak_externals[i] == refatom)
			return;
	weak_externals.Add(atom, refatom);
//	PutStdOut(NFormat("ALTERNATE: %s -> %s", atoms[atom], atoms[refatom]));
}

/*
int LinkJob::AddGlobal(int atom, const Symbol& symbol)
{
	globals.Add(atom, symbol);

	int pos = globals.Find(atom);
	if(pos < 0) {
		pos = globals.GetCount();
		globals.Add(atom, symbol);
		return pos;
	}
	Symbol& old_symbol = globals[pos];
//	if(old_symbol.storage_class != symbol.storage_class)
//		PutStdOut(NFormat("%s: storage class mismatch (%02x / %02x)", atoms[atom], old_symbol.storage_class, symbol.storage_class));
	if(old_symbol.section == 0)
		old_symbol = symbol;
	else if(symbol.section != 0) { // duplicate / comdat
		if(old_symbol.type != symbol.type)
			PutStdOut(NFormat("%s: symbol type mismatch (%04x / %04x)", atoms[atom], old_symbol.type, symbol.type));
		if(old_symbol.section > 0 && symbol.section > 0) {
			ObjectFile::Section& sec1 = objects[old_symbol.object].sections[old_symbol.section];
			ObjectFile::Section& sec2 = objects[symbol.object].sections[symbol.section];
			if((sec1.flags & sec2.flags) & COFF_IMAGE_SCN_LNK_COMDAT) {
				if(sec1.comdat_packing != sec2.comdat_packing)
					PutStdOut(NFormat("%s: comdat packing style mismatch: %d / %d",
						atoms[atom], sec1.comdat_packing, sec2.comdat_packing));
				switch(sec2.comdat_packing) {
				case COFF_IMAGE_COMDAT_SELECT_NODUPLICATES:
					break;

				case COFF_IMAGE_COMDAT_SELECT_ANY:
					return pos;

				case COFF_IMAGE_COMDAT_SELECT_SAME_SIZE:
					if(sec2.size == sec1.size)
						return pos;
					break;

				case COFF_IMAGE_COMDAT_SELECT_EXACT_MATCH:
					if(sec2.size == sec1.size && sec2.exact_comdat == sec1.exact_comdat)
						return pos;
					break;

				case COFF_IMAGE_COMDAT_SELECT_ASSOCIATIVE:
					break;

				case COFF_IMAGE_COMDAT_SELECT_LARGEST:
					if(sec2.size > sec1.size)
						old_symbol = symbol;
					return pos;
				}
			}
		}

		PutStdOut(NFormat("%s: duplicate (%s:%d / %s:%d)", atoms[atom],
			objects[old_symbol.object].file, old_symbol.section,
			objects[symbol.object].file, symbol.section));
	}
	return pos;
}
*/

void LinkJob::ReadDefaultLibs()
{
	LTIMING("LinkJob::ReadDefaultLibs");
	for(int i = 0; i < defaultlibs.GetCount(); i++)
		if(nodefaultlibs.Find(defaultlibs[i]) < 0) {
			if(dump_flags & DUMP_OBJECTS)
				PutStdOut(NFormat("defaultlib: %s", defaultlibs[i]));
			LoadFile(defaultlibs[i], true);
		}
}

void LinkJob::PrepareResourceTree()
{
	if(resource_objects.GetCount() == 0)
		return;
	ObjectFile& of = objects[OBJ_RSRC];
	of.autocollect = true;
	ObjectFile::Section& dirsec = of.sections[OBJR_DIRS];
	ObjectFile::Section& strsec = of.sections[OBJR_STRINGS];
	Sort(resource_objects, &LinkJob::Resource::Sort);
	Vector<int> string_relocations;
	Vector<int> end, par;
//	int stamp = time(NULL);
	end.Add(resource_objects.GetCount());
	par.Add(0);
	for(int level = 0; level < 3; level++) {
		Vector<int> new_end, new_par;
		int rix = 0;
		for(int part = 0; part < end.GetCount(); part++) {
			int eix = end[part], pix = par[part];
			COFF_IMAGE_RESOURCE_DIRECTORY imgdir;
			Zero(imgdir);
//			imgdir.TimeDateStamp = stamp;
			if(pix)
				PokeIL(&dirsec.section_data[pix],
					dirsec.section_data.GetCount() | COFF_IMAGE_RESOURCE_DATA_IS_DIRECTORY);
			int diroff = dirsec.section_data.GetCount();
			int numid = 0, numstr = 0;
			CatN(dirsec.section_data, sizeof(imgdir), &imgdir);
			const Resource *prev = &resource_objects[rix], *next;
			do {
				next = (++rix < eix ? &resource_objects[rix] : NULL);
				bool eq = next &&
					( level == 0 ? next->type_id == prev->type_id && next->type == prev->type
					: level == 1 ? next->name_id == prev->name_id && next->name == prev->name
					: level == 2 ? next->lang_id == prev->lang_id : true );
				if(!eq) {
					COFF_IMAGE_RESOURCE_DIRECTORY_ENTRY entry;
					Zero(entry);
					if(level == 2) {
						numid++;
						entry.Id = prev->lang_id;
					}
					else {
						int id = (level ? prev->name_id : prev->type_id);
						if(!IsNull(id)) {
							numid++;
							entry.Id = id;
						}
						else {
							numstr++;
							WString w = (level ? prev->name : prev->type);
							entry.Name = COFF_IMAGE_RESOURCE_NAME_IS_STRING + strsec.section_data.GetCount();
							string_relocations.Add(dirsec.section_data.GetCount()
								+ __offsetof(COFF_IMAGE_RESOURCE_DIRECTORY_ENTRY, Name));
							word len = 2 * w.GetLength();
							CatN(strsec.section_data, 2, &len);
							CatN(strsec.section_data, 2 * w.GetLength(), w);
						}
					}
					entry.OffsetToDirectory = -1;
					new_end.Add(rix);
					new_par.Add(dirsec.section_data.GetCount()
						+ __offsetof(COFF_IMAGE_RESOURCE_DIRECTORY_ENTRY, OffsetToData));
					CatN(dirsec.section_data, sizeof(entry), &entry);
				}
			}
			while(prev = next);
			PokeIW(&dirsec.section_data[diroff + __offsetof(COFF_IMAGE_RESOURCE_DIRECTORY, NumberOfIdEntries)], numid);
			PokeIW(&dirsec.section_data[diroff + __offsetof(COFF_IMAGE_RESOURCE_DIRECTORY, NumberOfNamedEntries)], numstr);
		}
		end = new_end;
		par = new_par;
	}
	dirsec.size = dirsec.raw_size = dirsec.section_data.GetCount();
	for(int i = 0; i < string_relocations.GetCount(); i++) {
		byte *ptr = &dirsec.section_data[string_relocations[i]];
		PokeIL(ptr, PeekIL(ptr) + dirsec.size);
	}
	strsec.size = strsec.raw_size = strsec.section_data.GetCount();
	int entoff = dirsec.size + strsec.size;
	ObjectFile::Section& entsec = of.sections[OBJR_ENTRIES];
	ObjectFile::Section& datsec = of.sections[OBJR_DATA];
	int pend = 0;
	for(int i = 0; i < end.GetCount(); i++) {
		const Resource& r = resource_objects[pend];
		int nend = end[i];
		if(nend - pend > 1) {
			String dupres;
			dupres << "Duplicate resource (type: "
				<< (!IsNull(r.type_id) ? FormatIntHex(r.type_id, 4) : String(r.type))
				<< ", name: "
				<< (!IsNull(r.name_id) ? FormatIntHex(r.name_id, 4) : String(r.name))
				<< ", lang: "
				<< FormatIntHex(r.lang_id, 4)
				<< ")";
			PutStdOut(dupres);
		}
		COFF_IMAGE_RESOURCE_DATA_ENTRY entry;
		Zero(entry);
		entry.OffsetToData = datsec.section_data.GetCount();
		entry.Size = r.data.GetLength();
		entry.CodePage = 0; //??
		byte *ptr = &dirsec.section_data[par[i]];
		PokeIL(ptr, entsec.section_data.GetCount() + entoff);
		CatN(datsec.section_data, r.data.GetLength(), r.data);
		CatN0(datsec.section_data, 4 - (r.data.GetLength() & 3));
		CatN(entsec.section_data, sizeof(entry), &entry);
		pend = nend;
	}
	entsec.size = entsec.raw_size = entsec.section_data.GetCount();
	datsec.size = datsec.raw_size = datsec.section_data.GetCount();
}

void LinkJob::SetEntryPoint()
{
	if(IsNull(entrypoint))
		if(make_dll)
			entrypoint = "_DllMainCRTStartup";
		else if(subsystem == COFF_IMAGE_SUBSYSTEM_WINDOWS_GUI && globals.Find(Atomize("_WinMainCRTStartup")) >= 0)
			entrypoint = "_WinMainCRTStartup";
		else
			entrypoint = "_mainCRTStartup";
	entrypoint_atom = Atomize(entrypoint);
}

void LinkJob::SetExportSymbols()
{
	if(exports.IsEmpty() && make_dll) { // collect all globals
		int ordinal = 0;
		for(int i = 0; i < globals.GetCount(); i++) {
			atom_t gatom = globals.GetKey(i);
			const Symbol& sym = globals[i];
			if(gatom != entrypoint_atom && sym.external && objects[sym.obj_sec.object].export_symbols && sym.obj_sec.section >= 0) {
				String name = atoms[gatom];
				const char *p = name;
				if(*p == '_')
					p++;
				exports.Add(gatom, Export(p, ++ordinal));
			}
		}
	}
}

void LinkJob::SetRootSymbols()
{
	AddCollect(entrypoint_atom);
	for(int i = 0; i < exports.GetCount(); i++)
		AddCollect(exports.GetKey(i));
	for(int i = 0; i < include_symbols.GetCount(); i++)
		AddCollect(include_symbols[i]);
}

void LinkJob::CollectObject(int index)
{
	const ObjectFile& of = objects[index];
	Index<ObjSec> osv;
	for(int t = 0; t < of.sections.GetCount(); t++)
		if(of.sections[t].autocollect)
			osv.Add(ObjSec(index, t));
	CollectSectionSymbols(osv, 0);
}

void LinkJob::CollectSectionSymbols(Index<ObjSec>& collect_objsec, int referer_index)
{
	LTIMING("LinkJob::CollectSectionSymbols");

	int ref = globals.GetKey(referer_index), setref = ref;
	for(int si = 0; si < collect_objsec.GetCount(); si++) {
		ObjSec objsec = collect_objsec[si];
		ObjectFile& of = objects[objsec.object];
		if(!of.collected) {
			of.collected = true;
			for(int s = 0; s < of.sections.GetCount(); s++)
				if(of.sections[s].autocollect) {
					if(dump_flags & DUMP_COLLECTOR)
						PutStdOut(NFormat("%s: autocollect section %d, %s", AsString(of), s, atoms[of.sections[s].name_atom]));
					collect_objsec.FindAdd(ObjSec(objsec.object, s));
				}
		}

		if(objsec.section < 0)
			continue;
		ObjectFile::Section& ofsec = of.sections[objsec.section];
		if(ofsec.sec_atom == debug_atom || ofsec.comdat_state == ObjectFile::COM_TRASHED) {
			if(si == 0)
				referer_index = ofsec.comdat_forward;
			continue;
		}

		bool was_used = ofsec.used;

//		LTIMING("LinkJob::CollectSectionSymbols / unused section");
		if(ofsec.flags & COFF_IMAGE_SCN_LNK_COMDAT)
			ofsec.comdat_state = ObjectFile::COM_SELECTED;
/*
		if(ofsec.comdat_state != ObjectFile::COM_SELECTED && ()) {
//			LTIMING("LinkJob::CollectSectionSymbols / COMDAT");
			Vector<int> candidates;
			for(int f = globals.Find(ofsec.name_atom); f >= 0; f = globals.FindNext(f))
				candidates.Add(f);
			if(dump_flags & DUMP_COLLECTOR)
				PutStdOut(NFormat("\tCOMDAT section (%d candidates)", candidates.GetCount()));
			int chosen = -1;
			int current = -1;
			if(candidates.GetCount() > 1) {
				for(int i = 0; i < candidates.GetCount(); i++) {
					ObjSec otheros = globals[candidates[i]].obj_sec;
					if(otheros == objsec) { // current section
						current = i;
						continue;
					}
					ObjectFile& otherof = objects[otheros.object];
					ObjectFile::Section& othersec = otherof.sections[otheros.section];
					if(othersec.used) {
						chosen = i;
						break;
					}
					if(!(othersec.flags & COFF_IMAGE_SCN_LNK_COMDAT)) {
						puts(NFormat("%s: %s is not a COMDAT, whereas %s:%d is (non-COMDAT selected)",
							atoms[ofsec.name_atom], FormatSection(otheros),
							AsString(of), objsec.section));
						chosen = i;
					}
					if(othersec.comdat_packing != ofsec.comdat_packing) {
						PutStdOut(NFormat("%s: %s has COMDAT packing style %d, %s has %d",
							atoms[ofsec.name_atom],
							FormatSection(otheros), othersec.comdat_packing,
							FormatSection(objsec), ofsec.comdat_packing));
					}
					switch(ofsec.comdat_packing) {
					case COFF_IMAGE_COMDAT_SELECT_NODUPLICATES:
						PutStdOut(NFormat("%s: duplicate COMDAT definitions %s / %s",
							atoms[ofsec.name_atom], FormatSection(otheros),
							FormatSection(objsec)));
						break;

					case COFF_IMAGE_COMDAT_SELECT_ANY:
						break;

					case COFF_IMAGE_COMDAT_SELECT_SAME_SIZE:
						if(othersec.size != ofsec.size)
							PutStdOut(NFormat("%s: COMDAT size mismatch: %s - %d B, %s - %d B",
								atoms[ofsec.name_atom],
								FormatSection(otheros), othersec.size,
								FormatSection(objsec), ofsec.size));

					case COFF_IMAGE_COMDAT_SELECT_LARGEST:
						if(chosen < 0 && othersec.size > ofsec.size
						|| chosen >= 0 && othersec.size > GetSection(globals[candidates[chosen]].obj_sec).size)
							chosen = i;
						break;

					case COFF_IMAGE_COMDAT_SELECT_EXACT_MATCH:
						if(othersec.size != ofsec.size
						|| memcmp(othersec.section_data.Begin(), ofsec.section_data.Begin(), ofsec.size))
							PutStdOut(NFormat("%s: COMDAT data mismatch: %s / %s",
								atoms[ofsec.name_atom], FormatSection(otheros),
								FormatSection(objsec)));
						break;

					case COFF_IMAGE_COMDAT_SELECT_ASSOCIATIVE:
						break;

					}
				}
				if(chosen >= 0) {
					int candidate = candidates[chosen];
					if(si == 0)
						referer_index = candidate;
					ObjSec otheros = globals[candidates[chosen]].obj_sec;
					for(int i = 0; i < candidates.GetCount(); i++)
						if(i != chosen) {
							ObjectFile::Section& othersec = GetSection(globals[candidates[i]].obj_sec);
							othersec.comdat_state = ObjectFile::COM_TRASHED;
							othersec.comdat_forward = candidate;
						}
					if(dump_flags & DUMP_COLLECTOR) {
						ObjectFile& otherof = objects[otheros.object];
						puts(NFormat("\tCOMDAT section %s discarded, %s selected",
							FormatSection(objsec), FormatSection(otheros)));
					}
					GetSection(otheros).comdat_state = ObjectFile::COM_SELECTED;
					collect_objsec.FindAdd(otheros);
					continue;
				}
				else if(current >= 0) {
					int candidate = candidates[current];
					for(int i = 0; i < candidates.GetCount(); i++)
						if(i != current) {
							ObjSec otheros = globals[candidates[i]].obj_sec;
							ObjectFile::Section& othersec = GetSection(otheros);
							othersec.comdat_state = ObjectFile::COM_TRASHED;
							othersec.comdat_forward = candidate;
						}
				}
			}
		}
*/

		of.used_any = ofsec.used = true;

		if(setref) {
			for(int rs = global_obj_sec_index.Find(objsec); rs >= 0; rs = global_obj_sec_index.FindNext(rs))
				if(globals.GetKey(rs) == setref) {
					globals[rs].linked = globals[rs].used = true;
					setref = 0;
					break;
				}
		}

		if(!was_used) {
			if(dump_flags & DUMP_COLLECTOR)
				PutStdOut(NFormat("%s: collecting section %d (%d B) %s (referer = %s)",
					AsString(of), objsec.section, ofsec.size, atoms[ofsec.name_atom], atoms[globals.GetKey(referer_index)]));
			for(int rs = global_obj_sec_index.Find(objsec); rs >= 0; rs = global_obj_sec_index.FindNext(rs)) {
				int f = collected_symbols.Find(globals.GetKey(rs));
				globals[rs].linked = true;
				if(f >= 0 && collected_symbols[f] < 0) {
					globals[rs].used = true;
					collected_symbols[f] = rs;
				}
//				bool u = (globals.GetKey(rs) == ref);
//				if(u)
//					globals[rs].used = true;
				if(dump_flags & DUMP_COLLECTOR)
					PutStdOut(NFormat("\tdefines: %-30s VAL %08x TYP %04x %[1:*]s",
						atoms[globals.GetKey(rs)], globals[rs].value, globals[rs].type, globals[rs].used));
			}
		}

		if(objsec.section <= 0)
			continue;

		if(!was_used) {
			for(int ca = of.comdat_assoc.Find(objsec.section); ca >= 0; ca = of.comdat_assoc.FindNext(ca)) {
	//			LTIMING("LinkJob::CollectSectionSymbols / COMDAT_ASSOC");
				ObjSec cca(of.index, of.comdat_assoc[ca]);
				if(collect_objsec.Find(cca) < 0) {
					collect_objsec.Add(cca);
					if(!was_used && (dump_flags & DUMP_COLLECTOR))
						PutStdOut(NFormat("\tassoc comdat: %d", cca.section));
				}
			}
		}

		int sec_end = of.ref_sections.GetCount();
		int ext_end = of.ref_externals.GetCount();
		if(objsec.section + 1 < of.sections.GetCount()) {
			sec_end = of.sections[objsec.section + 1].ref_sec_index;
			ext_end = of.sections[objsec.section + 1].ref_ext_index;
		}
		if(ofsec.type == ObjectFile::SEC_STD) {
//			LTIMING("LinkJob::CollectSectionSymbols / references + xrefs + collector");
			if(!was_used) {
//				LTIMING("LinkJob::CollectSectionSymbols / section references");
				for(int x = ofsec.ref_sec_index; x < sec_end; x++) {
					int isec = of.ref_sections[x];
					const ObjectFile::Section& sec = of.sections[isec];
					if(!sec.used) {
						collect_objsec.FindAdd(ObjSec(of.index, isec));
						if(!was_used && (dump_flags & DUMP_COLLECTOR))
							PutStdOut(NFormat("\tintra-object reference to section %d", isec));
					}
				}
//				LTIMING("LinkJob::CollectSectionSymbols / symbol references");
				for(NOMSC6(int) x = ofsec.ref_ext_index; x < ext_end; x++) {
					int sx = obj_static_global_index.Get(Point(objsec.object, of.ref_externals[x]), -1);
					if(sx >= 0) { // static global
						const ObjectFile::Section& sec = GetSection(globals[sx].obj_sec);
						if(!sec.used) {
							collect_objsec.FindAdd(globals[sx].obj_sec);
							if(!was_used && (dump_flags & DUMP_COLLECTOR))
								PutStdOut(NFormat("\tintra-object reference to static %s", atoms[globals.GetKey(sx)]));
						}
					}
					else
						AddCollect(of.ref_externals[x]);
				}
			}
			if(write_xref) {
				for(int x = ofsec.ref_ext_index; x < ext_end; x++)
					collected_referer.GetAdd(of.ref_externals[x]).FindAdd(globals.GetKey(referer_index));
			}
			if((dump_flags & DUMP_COLLECTOR) && !was_used) {
				for(int x = ofsec.ref_ext_index; x < ext_end; x++)
					PutStdOut(NFormat("\treferences: %s", atoms[of.ref_externals[x]]));
			}
		}
	}
}

void LinkJob::CollectSymbols()
{
//	int xxx = Atomize("__crtBreakAlloc");
	bool error = false;
	LTIMING("LinkJob::CollectSymbols");
	for(int i = 0; i < objects.GetCount(); i++)
		if(objects[i].autocollect)
			CollectObject(i);
/* {
			const ObjectFile& of = objects[i];
			Index<ObjSec> osv;
			for(int t = 0; t < of.sections.GetCount(); t++)
				if(of.sections[t].autocollect)
					osv.Add(ObjSec(i, t));
			CollectSectionSymbols(osv, 0);
		}
*/
	Index<int> undefined;
	Vector<ObjSec> undef_ref;
	for(NOMSC6(int) i = 0; i < collected_symbols.GetCount(); i++) {
		atom_t ref = collected_symbols.GetKey(i);
//		if(ref == CTOR_LIST_atom || ref == _CTOR_LIST_atom || ref == CTOR_LIST_END_atom
//		|| ref == DTOR_LIST_atom || ref == _DTOR_LIST_atom || ref == DTOR_LIST_END_atom)
//			__asm int 3
		if(dump_flags & DUMP_COLLECTOR)
			PutStdOut(NFormat("%s: collecting symbol #%d", atoms[ref], i));
//		if(ref == xxx)
//			__asm int 3
		int a, af = globals.Find(ref);
		if(af < 0) {
//			PutStdOut(NFormat("N/F global: %s", atoms[ref]));
			if((a = weak_externals.Find(ref)) >= 0) {
				if((af = weak_externals.FindNext(a)) >= 0)
					PutStdOut(NFormat("%s: weak external has multiple aliases (%s, %s)",
						atoms[ref], atoms[a], atoms[af]));
				AddCollect(weak_externals[a]);
				if(write_xref)
					collected_referer.GetAdd(weak_externals[a]).FindAdd(ref);
				continue;
			}
			if(undefined.Find(collected_symbols.GetKey(i)) < 0) {
				undefined.Add(collected_symbols.GetKey(i));
				undef_ref.Add(ext_obj_sec.Get(ref, Null));
			}
			continue;
		}
		int collect_global = -1;
		int collect_byfile = -1;
//		ObjectFile *collect_file = NULL;
//		Index<int> collect_sections;
		for(a = af; a >= 0; a = globals.FindNext(a)) {
			if(globals[a].used) {
				collect_global = a;
				break;
			}
			int secno = globals[a].obj_sec.section;
			if(secno > 0) {
				ObjectFile& of = objects[globals[a].obj_sec.object];
				if(of.used_any) {
					collect_byfile = a;
	//				globals[a].used = true;
	//				collected_symbols[i] = a;
	//				collect_file = &of;
	//				collect_sections.FindAdd(secno);
				}
			}
		}
		if(collect_global < 0)
			collect_global = collect_byfile;
		if(collect_global < 0) {
//			if(ref == Atomize("__acmdln"))
//				__asm int 3
			Index<int> comdat_candidates;
			Index<int> dflib_candidates;
			Index<int> lib_candidates;
			Index<int> full_candidates;
			Index<int> direct_candidates;
			Index<int> inner_candidates;
			for(a = af; a >= 0; a = globals.FindNext(a)) {
				int atom = globals.GetKey(a);
				Symbol& global = globals[a];
				if(global.obj_sec.object < OBJ_FIRST) {
					inner_candidates.Add(a);
					continue;
				}
				if(global.obj_sec.section == 0) {
					comdat_candidates.Add(a);
					continue;
				}
				const ObjectFile& of = objects[global.obj_sec.object];
				if(of.used_any)
					inner_candidates.Add(a);
				else if(of.filetype == of.DIRECT)
					direct_candidates.Add(a);
				else if(of.filetype == of.FULL_PATH)
					full_candidates.Add(a);
				else if(of.filetype == of.LIB_PATH)
					lib_candidates.Add(a);
				else
					dflib_candidates.Add(a);
			}
			if(inner_candidates.IsEmpty())
				inner_candidates = direct_candidates;
			if(inner_candidates.IsEmpty())
				inner_candidates = full_candidates;
			if(inner_candidates.IsEmpty())
				inner_candidates = lib_candidates;
			if(inner_candidates.IsEmpty())
				inner_candidates = dflib_candidates;
			if(inner_candidates.IsEmpty())
				inner_candidates = comdat_candidates;
			if(inner_candidates.GetCount() > 1) {
				for(a = 1; a < inner_candidates.GetCount(); a++) {
					bool matched = false;
					const Symbol& sym1 = globals[inner_candidates[0]];
					const Symbol& sym2 = globals[inner_candidates[a]];
					if(sym1.type != sym2.type) {
/*						PutStdOut(NFormat("%s: symbol type mismatch (%s:%d:%04x / %s:%d:%04x)",
							atoms[ref],
							sym1.obj_sec.object >= 0 ? AsString(objects[sym1.obj_sec.object]) : String("(linker)"),
							sym1.obj_sec.section, sym1.type,
							sym2.obj_sec.object >= 0 ? AsString(objects[sym2.obj_sec.object]) : String("(linker)"),
							sym2.obj_sec.section, sym2.type));
*/					}
					else if((sym1.type == COFF_IMAGE_SYM_TYPE_IMPORT_NAME || sym1.type == COFF_IMAGE_SYM_TYPE_IMPORT_ORDINAL
						|| sym1.type == COFF_IMAGE_SYM_TYPE_IMPORT_STUB) && sym2.type == sym1.type) {
						const ObjectFile::Import& imp1 = objects[sym1.obj_sec.object].imports[sym1.value];
						const ObjectFile::Import& imp2 = objects[sym2.obj_sec.object].imports[sym2.value];
						bool m = true;
						if(imp1.imp_atom != imp2.imp_atom) {
							m = false;
							PutStdOut(NFormat("%s: import name mismatch (%s, %s)", atoms[ref], atoms[imp1.imp_atom], atoms[imp2.imp_atom]));
						}
						if(sym1.type == COFF_IMAGE_SYM_TYPE_IMPORT_ORDINAL && imp1.ordinal_hint != imp2.ordinal_hint) {
							m = false;
							PutStdOut(NFormat("%s: import library ordinal mismatch (%d, %d)", atoms[ref], imp1.ordinal_hint, imp2.ordinal_hint));
						}
						if(m)
							matched = true;
					}
					else if(sym1.obj_sec.section > 0 && sym2.obj_sec.section > 0) {
						ObjectFile::Section& sec1 = GetSection(sym1.obj_sec);
						ObjectFile::Section& sec2 = GetSection(sym2.obj_sec);
						if((sec1.flags | sec2.flags) & COFF_IMAGE_SCN_LNK_COMDAT)
							matched = true;
					}
					else if(sym1.obj_sec.section == 0 && sym2.obj_sec.section == 0 && sym1.value == sym2.value)
						matched = true;
					if(!matched) {
						PutStdOut(NFormat("%s: duplicate - %s (used) / %s (discarded)", atoms[ref],
							FormatSection(sym1.obj_sec), FormatSection(sym2.obj_sec)));
					}
				}
			}
			if(!inner_candidates.IsEmpty()) {
//				collected_symbols[i] = inner_candidates[0];
				int chosen = inner_candidates[0];
				Symbol& sym = globals[chosen];
//				sym.used = true;
				if(sym.obj_sec.object >= 0) {
					ObjectFile& fo = objects[sym.obj_sec.object];
//					if(fo.dll_atom)
//						__asm int 3
//					if(fo.dll_atom)
//						PutStdOut(NFormat("collecting DLL import %s", atoms[ref]));
					if(sym.obj_sec.section > 0)
						collect_global = chosen;
//					{
//						collect_file = &fo;
//						collect_sections.FindAdd(sym.obj_sec.section);
//					}
					else if(sym.obj_sec.section == 0) { // anon comdat
						int comdat_align = 1;
						while(comdat_align < 16 && !(sym.value & comdat_align))
							comdat_align <<= 1;
						int v = (fo.sections[0].size + comdat_align - 1) & -comdat_align;
						fo.sections[0].size = v + sym.value;
						fo.used_any = fo.sections[0].used = true;
						sym.value = v;
					}
				}
				if(collect_global < 0)
					globals[collected_symbols[i] = chosen].used = true;
			}
		}
		if(collect_global >= 0) // && !collect_file->dll_atom)
		{
			ObjSec os = globals[collect_global].obj_sec;
//			if(os.object == OBJ_CTOR)
//				__asm int 3
			if(os.object >= 0) {
				Index<ObjSec> objsec;
				objsec.Add(os);
				CollectSectionSymbols(objsec, collect_global);
/*
				if(!globals[collect_global].used) {
					int i;
					for(i = globals.Find(ref); i >= 0 && !globals[i].used; i = globals.FindNext(i))
						;
					collect_global = i;
					if(i >= 0)
						while((i = globals.FindNext(i)) >= 0 && !globals[i].used)
							;
					if(i >= 0)
						puts(NFormat("%s: duplicate in %s (used, %s (discarded)",
							atoms[ref], FormatSection(globals[collect_global].obj_sec),
								FormatSection(globals[i].obj_sec)));
				}
				if(collect_global < 0)
					puts(NFormat("%s: undefined", atoms[ref]));
*/
			}
			else
				collected_symbols[i] = collect_global;
		}
	}

	for(NOMSC6(int) i = 0; i < collected_symbols.GetCount(); i++)
		if(collected_symbols[i] < 0) {
			int atom = collected_symbols.GetKey(i);
			int f = globals.Find(atom);
			while(f >= 0 && !globals[f].used)
				f = globals.FindNext(f);
			if(f >= 0) {
				int g = f;
				while((g = globals.FindNext(g)) >= 0 && !globals[g].used)
					;
				if(g >= 0)
					puts(NFormat("%s: duplicate in %s (used), %s (discarded)",
						atoms[atom], FormatSection(globals[f].obj_sec), FormatSection(globals[g].obj_sec)));
				else if(dump_flags & DUMP_COLLECTOR)
					puts(NFormat("%s: delayed collect from %s", atoms[atom], FormatSection(globals[f].obj_sec)));
				collected_symbols[i] = f;
				globals[f].used = true;
			}
		}

	for(NOMSC6(int) i = 0; i < weak_externals.GetCount(); i++) {
		int f = collected_symbols.Find(weak_externals.GetKey(i));
		if(f >= 0 && collected_symbols[f] < 0)
			collected_symbols[f] = collected_symbols.Get(weak_externals[i], -1);
	}

	if(idata_last >= 0) {
		Index<ObjSec> collect;
		collect.Add(ObjSec(OBJ_CTOR, OBJC_IDATA_HOLE));
		if(!idata_null_found)
			collect.Add(ObjSec(OBJ_CTOR, OBJC_IDATA_NULL));
		CollectSectionSymbols(collect, 0);
	}

	if(!undefined.IsEmpty()) {
		Vector<String> untext;
		untext.SetCount(undefined.GetCount());
		int u;
		for(u = 0; u < undefined.GetCount(); u++)
			untext[u] = atoms[undefined[u]];
		Vector<int> order = GetSortOrder(untext);
		PutStdOut(NFormat("%d undefined symbol(s):", untext.GetCount()));
		for(u = 0; u < order.GetCount(); u++) {
			int o = order[u];
			String ident = untext[o];
			ObjSec obj_sec = ext_obj_sec.Get(undefined[o], Null);
			if(!IsNull(obj_sec)) {
				String os_globals;
				for(int t = global_obj_sec_index.Find(obj_sec); t >= 0; t = global_obj_sec_index.FindNext(t))
					if(os_globals.GetLength() >= 80) {
						os_globals << "; ...";
						break;
					}
					else
						os_globals << "; " << DemangleName(atoms[globals.GetKey(t)], linkermode == MODE_GNULD ? MANGLING_GCC : MANGLING_MSC);
				PutStdOut(NFormat("%s (referenced from %s%s)", ident, FormatSection(obj_sec), os_globals));
			}
			else
				PutStdOut(ident);
		}
		error = true;
	}

	if(dump_flags & DUMP_STAT) {
		PutStdOut(NFormat("#objects = %d", objects.GetCount()));
		int atom_size = (16 + 16) * atoms.GetCount();
		int i;
		for(i = 0; i < atoms.GetCount(); i++)
			atom_size += atoms[i].GetLength();
		PutStdOut(NFormat("#atoms = %d, size = %d", atoms.GetCount(), atom_size));
		int used_globals = 0;
		for(i = 0; i < globals.GetCount(); i++)
			if(globals[i].used)
				used_globals++;
		PutStdOut(NFormat("#globals = %d (%d used), size = %d", globals.GetCount(), used_globals,
			(int)(globals.GetCount() * (sizeof(Symbol) + 16))));
	}

	if(error)
		throw Exc("linking aborted due to fatal errors");

	CheckDuplicates();
}

void LinkJob::CheckDuplicates()
{
	bool error = false;
	for(int i = 0; i < globals.GetCount(); i++)
		if(globals[i].type != COFF_IMAGE_SYM_TYPE_SECTION && globals.FindPrev(i) < 0) {
			int atom = globals.GetKey(i);
			const Symbol *first = NULL;
			for(int f = i; f >= 0; f = globals.FindNext(f)) {
				const Symbol& global = globals[f];
				if(!global.used || global.obj_sec.object < 0)
					continue;
				const ObjectFile& object = objects[global.obj_sec.object];
				if(!first) {
					first = &global;
					continue;
				}
				if(first -> type == COFF_IMAGE_SYM_TYPE_SECTION && global.type == COFF_IMAGE_SYM_TYPE_SECTION)
					continue;
				if((first->type == COFF_IMAGE_SYM_TYPE_IMPORT_NAME || first->type == COFF_IMAGE_SYM_TYPE_IMPORT_ORDINAL)
				&& global.type == first->type) {
					const ObjectFile::Import& imp1 = objects[first->obj_sec.object].imports[first->value];
					const ObjectFile::Import& imp2 = objects[global.obj_sec.object].imports[global.value];
					if(imp1.imp_atom != imp2.imp_atom)
						PutStdOut(NFormat("%s: import name mismatch (%s, %s)", atoms[atom], atoms[imp1.imp_atom], atoms[imp2.imp_atom]));
					if(first->type == COFF_IMAGE_SYM_TYPE_IMPORT_ORDINAL && imp1.ordinal_hint != imp2.ordinal_hint)
						PutStdOut(NFormat("%s: import ordinal mismatch (%d, %d)", atoms[atom], imp1.ordinal_hint, imp2.ordinal_hint));
					continue;
				}
				error = true;
				PutStdOut(NFormat("%s: duplicate (%s / %s)", atoms[atom],
					FormatSection(first->obj_sec), FormatSection(global.obj_sec)));
			}
		}
//	if(error)
//		throw Exc("duplicates found, linking aborted");
}

void LinkJob::CollectImports()
{
	for(int i = 0; i < dll_objects.GetCount(); i++) {
		ObjectFile& of = objects[dll_objects[i]];
		ASSERT(of.dll_atom);
		ASSERT(of.used_imports.IsEmpty());
		if(of.used_any) {
			ObjectFile::Section& stubsec = of.sections[ObjectFile::SEC_DLL_STUBS];
			ObjectFile::Section& namesec = of.sections[ObjectFile::SEC_DLL_NAMES];
			ObjectFile::Section& impsec  = of.sections[ObjectFile::SEC_DLL_IMPORTS];
			ObjectFile::Section& bimpsec = of.sections[ObjectFile::SEC_DLL_BOUND];
			ObjectFile::Section& descsec = of.sections[ObjectFile::SEC_DLL_DESCRIPTOR];
//			ObjectFile::Section& nullsec = of.sections[ObjectFile::SEC_DLL_NULL_DESC];
			String dllfn = ToUpper(GetFileName(of.object_file));
			CatN(namesec.section_data, dllfn.GetLength() + 1, dllfn);
			if(namesec.section_data.GetCount() & 1)
				namesec.section_data.Add(0);
			namesec.size = namesec.section_data.GetCount();
			int g;
			for(g = global_obj_sec_index.Find(ObjSec(of.index, ObjectFile::SEC_DLL_STUBS)); g >= 0; g = global_obj_sec_index.FindNext(g)) {
				Symbol& stubimp = globals[g];
				if(!stubimp.used)
					continue;
//				if(of.used_stubs.Find(stubimp.value) < 0)
//					PutStdOut(NFormat("stub import[%d] = %s", of.used_stubs.GetCount(), atoms[globals.GetKey(g)]));
				of.used_imports.FindAdd(stubimp.value);
				stubimp.value = IMP_STUB_SIZE * of.used_stubs.FindAdd(stubimp.value);
				stubsec.used = impsec.used = true;
			}
			for(g = global_obj_sec_index.Find(ObjSec(of.index, ObjectFile::SEC_DLL_BOUND)); g >= 0; g = global_obj_sec_index.FindNext(g)) {
				Symbol& imp = globals[g];
				if(!imp.used)
					continue;
				ObjectFile::Import& import = of.imports[imp.value];
				of.used_imports.Add(imp.value);
				imp.value = IMP_ENTRY_SIZE * (1 + of.used_imports.FindAdd(imp.value));
				impsec.used = true;
			}
			stubsec.size = IMP_STUB_SIZE * of.used_stubs.GetCount();
			if(!of.used_imports.IsEmpty()) {
				ASSERT(impsec.used);
				used_dll_objects.Add(of.index);
				impsec.used = bimpsec.used = namesec.used = true;
				impsec.size = bimpsec.size = IMP_ENTRY_SIZE * (of.used_imports.GetCount() + 1);
				impsec.section_data.SetCount(impsec.size, 0);
				for(g = 0; g < of.used_imports.GetCount(); g++) {
					const ObjectFile::Import& oi = of.imports[of.used_imports[g]];
					int ival = oi.ordinal_hint | 0x80000000;
					if(!oi.ordinal) { // create hint/name table entry
						ival = namesec.size;
						String ia = atoms[oi.imp_atom];
						int offset = CatN0(namesec.section_data, ((ia.GetLength() + 4) & -2));
						PokeIW(&namesec.section_data[offset], oi.ordinal_hint);
						memcpy(&namesec.section_data[offset + 2], ia, ia.GetLength());
						namesec.size = namesec.section_data.GetCount();
					}
					byte *ient = impsec.section_data.GetIter(IMP_ENTRY_SIZE * (g + 1));
					PokeIL(ient, ival);
				}
				bimpsec.section_data <<= impsec.section_data;
				descsec.used = true;
				descsec.size = sizeof(COFF_IMAGE_IMPORT_DESCRIPTOR);
/*
				if(used_dll_objects.GetCount() == 1) {
					nullsec.used = true;
					nullsec.size = sizeof(COFF_IMAGE_IMPORT_DESCRIPTOR);
				}
*/
				if(dump_flags & DUMP_IMPORT) {
					PutStdOut(NFormat("%s: %d imports used; %d using stub; %d bytes in name table",
						of.object_file, of.used_imports.GetCount(), of.used_stubs.GetCount(), namesec.size));
					for(g = 0; g < of.used_imports.GetCount(); g++) {
						int uimp = of.used_imports[g];
						const ObjectFile::Import& ofimp = of.imports[uimp];
						if(ofimp.app_atom == ofimp.imp_atom)
							PutStdOut(NFormat("\t(%d) %s: %[1:stub]s",
								ofimp.ordinal_hint, atoms[ofimp.imp_atom],
								of.used_stubs.Find(uimp) >= 0));
						else
							PutStdOut(NFormat("\t(%d) %s -> %s: %[1:stub]s",
								ofimp.ordinal_hint, atoms[ofimp.app_atom], atoms[ofimp.imp_atom],
								of.used_stubs.Find(uimp) >= 0));
					}
				}
			}
		}
	}
}

void LinkJob::CollectExports()
{
	if(exports.IsEmpty())
		return;
	ObjectFile& ofe = objects[OBJ_EXPORT];
	ObjectFile::Section& dir = ofe.sections[OBJE_DIRECTORY];
	ObjectFile::Section& adr = ofe.sections[OBJE_ADDRESS_TABLE];
	ObjectFile::Section& ptr = ofe.sections[OBJE_NAME_POINTERS];
	ObjectFile::Section& ord = ofe.sections[OBJE_ORDINALS];
	ObjectFile::Section& nam = ofe.sections[OBJE_NAMES];

	dir.size = dir.raw_size = sizeof(COFF_IMAGE_EXPORT_DIRECTORY_TABLE);
	COFF_IMAGE_EXPORT_DIRECTORY_TABLE expdir;
	Zero(expdir);

	int min_ordinal, max_ordinal;
	min_ordinal = max_ordinal = exports[0].ordinal;
	Vector<String> named_exports;
	Vector<int> named_ordinals;
	for(int i = 0; i < exports.GetCount(); i++) {
		int e = exports[i].ordinal;
		if(e < min_ordinal) min_ordinal = e;
		if(e > max_ordinal) max_ordinal = e;
		if(!IsNull(exports[i].export_name)) {
			named_exports.Add(exports[i].export_name);
			named_ordinals.Add(e);
		}
	}
	expdir.DateTimeStamp = time(NULL);
	expdir.MajorVersion = major_version;
	expdir.MinorVersion = minor_version;
	expdir.NameRVA = 0;
	String dll_name = ToUpper(GetFileName(outputfile));
	CatN(nam.section_data, dll_name.GetLength() + 1, dll_name);
	expdir.OrdinalBase = min_ordinal;
	expdir.AddressTableEntries = max_ordinal - min_ordinal + 1;
	expdir.NumberOfNamePointers = named_exports.GetCount();

	adr.size = adr.raw_size = 4 * (max_ordinal - min_ordinal + 1);
	adr.section_data.SetCount(adr.size, 0);

	IndexSort(named_exports, named_ordinals, StdLess<String>());
	ptr.size = ptr.raw_size = 4 * named_exports.GetCount();
	ord.size = ord.raw_size = 2 * named_exports.GetCount();
	ptr.section_data.SetCount(ptr.size);
	ord.section_data.SetCount(ord.size);

	for(int i = 0; i < named_exports.GetCount(); i++) {
		PokeIL(ptr.section_data.GetIter(4 * i), nam.section_data.GetCount());
		PokeIW(ord.section_data.GetIter(2 * i), named_ordinals[i] - min_ordinal);
		String exp = named_exports[i];
		CatN(nam.section_data, exp.GetLength() + 1, exp);
	}
	nam.size = nam.raw_size = nam.section_data.GetCount();

	CatN(dir.section_data, dir.size, &expdir);

	if(dump_flags & DUMP_EXPORTS) {
		PutStdOut("Ordinal Export");
		for(int i = 0; i < exports.GetCount(); i++) {
			String ename = exports[i].export_name;
			String sname = atoms[exports.GetKey(i)];
			if(sname == ename)
				PutStdOut(NFormat("%7<d %s", exports[i].ordinal, ename));
			else
				PutStdOut(NFormat("%7<d %30<s %s", exports[i].ordinal, ename, sname));
		}
	}

	CollectObject(OBJ_EXPORT);
}

void LinkJob::CollectSections()
{
	iat2_rva = iat2_end_rva = 0;
	idesc_rva = idesc_end_rva = 0;
	rsrc_rva = rsrc_end_rva = 0;

/*
	if(idata_last >= 0 && !idata_null_found) {
		ObjectFile& of = objects[idata_last];
		ObjectFile::Section& ofsec = of.sections.Add();
//		ObjectFile::Section& prev = of.sections[of.sections.GetCount() - 2];
		ofsec.name_atom = idata_inull_atom;
		ofsec.sec_atom = idata_atom;
		ofsec.size = sizeof(COFF_IMAGE_IMPORT_DESCRIPTOR);
		ofsec.raw_size = 0;
		ofsec.offset = 0;
		ofsec.flags = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA
			| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE;
		ofsec.ref_sec_index = of.ref_sections.GetCount();
		ofsec.ref_ext_index = of.ref_externals.GetCount();
		ofsec.sec_map_index = -1;
		of.used_any = ofsec.used = true;
		ofsec.type = ObjectFile::SEC_INULL;
	}
*/

//	for(i = 0; i < SEC_COUNT; i++)
//		section_map[i].Clear();
/*
	if(debug_info) {
		Section& se = section_map.Add();
		se.obj_sec = Null;
		se.group = GRP_SYMBOLS;
		se.name = ".symbols";
		se.rva = se.rfa = -1;
		se.size = coff_output_symbols.GetCount() * sizeof(COFF_IMAGE_SYMBOL)
			+ coff_output_symbol_strings.GetLength();
	}
*/
	for(int i = 0; i < objects.GetCount(); i++)
		if(objects[i].used_any) {
			ObjectFile& of = objects[i];
			for(int s = 0; s < of.sections.GetCount(); s++)
				if(of.sections[s].used) {
					ObjectFile::Section& ofsec = of.sections[s];
					int sec_atom = section_merge.Get(ofsec.sec_atom, ofsec.sec_atom);
					int sx = GRP_NONE;
					if(i == OBJ_EXPORT)
						sx = GRP_EDATA;
					else if(sec_atom == rsrc_atom)
						sx = GRP_RSRC;
					else if(sec_atom == debug_atom)
						sx = GRP_DEBUG;
					else if(sec_atom == idata_atom)
						sx = GRP_IDATA;
					else if(ofsec.flags & COFF_IMAGE_SCN_CNT_CODE)
						sx = GRP_CODE;
					else if(!(ofsec.flags & COFF_IMAGE_SCN_MEM_WRITE))
						sx = GRP_RDATA;
					else if(ofsec.flags & COFF_IMAGE_SCN_CNT_UNINITIALIZED_DATA)
						sx = GRP_UDATA;
					else
						sx = GRP_DATA;
					Section& se = section_map.Add();
					se.obj_sec = ObjSec(i, s);
					se.group = sx;
					se.udata = (ofsec.type == ObjectFile::SEC_ANON_COMDAT
						|| ofsec.type == ObjectFile::SEC_STD && ofsec.offset == 0
						|| (ofsec.flags & (COFF_IMAGE_SCN_CNT_INITIALIZED_DATA
							| COFF_IMAGE_SCN_CNT_UNINITIALIZED_DATA | COFF_IMAGE_SCN_CNT_CODE))
							== COFF_IMAGE_SCN_CNT_UNINITIALIZED_DATA);
/*
					PutStdOut(NFormat("%s:%d, udata = %[0:no;yes]s, flags = %08x, STD = %d, offset = %d, anon = %d",
						AsString(of), s, se.udata, ofsec.flags, ofsec.type == ObjectFile::SEC_STD, ofsec.offset,
						(ofsec.type == ObjectFile::SEC_ANON_COMDAT
						|| ofsec.type == ObjectFile::SEC_STD && ofsec.offset == 0
						|| (ofsec.flags & (COFF_IMAGE_SCN_CNT_INITIALIZED_DATA
							| COFF_IMAGE_SCN_CNT_UNINITIALIZED_DATA | COFF_IMAGE_SCN_CNT_CODE))
							== COFF_IMAGE_SCN_CNT_UNINITIALIZED_DATA)
						));
*/
					se.sec_atom = (sx == GRP_CODE ? text_atom
						: sx == GRP_DATA ? data_atom
						: sx == GRP_RDATA ? rdata_atom
						: sx == GRP_UDATA ? bss_atom : sec_atom);
					se.name = atoms[ofsec.name_atom];
					ASSERT(!se.name.IsEmpty());
					se.size = ofsec.size;
					if(ofsec.sec_atom == idata_atom || ofsec.sec_atom == crt_atom) {
						se.name << ':';
						if(!IsNull(of.library_file))
							se.name << of.library_file << ':';
						se.name << of.object_file;
					}

					if(ofsec.type == ObjectFile::SEC_STD) {
						String ofile = Nvl(of.library_file, of.object_file);
						FileMapping& mapping = GetMapping(ofile);
						if(!mapping.IsOpen())
							throw Exc(NFormat("%s: error opening file", ofile));

						const byte *begin = mapping.Begin() + of.library_offset;
//						const COFF_IMAGE_FILE_HEADER *header = (const COFF_IMAGE_FILE_HEADER *)begin;
//						const COFF_IMAGE_SECTION_HEADER *sechdr = (const COFF_IMAGE_SECTION_HEADER *)(begin
//							+ sizeof(COFF_IMAGE_FILE_HEADER) + header->SizeOfOptionalHeader);
						if(ofsec.offset) {
							ofsec.section_data.SetCount(ofsec.raw_size);
							memcpy(ofsec.section_data.Begin(), begin + ofsec.offset, ofsec.raw_size);
						}
/*
						if(debug_info) {
							const COFF_IMAGE_SYMBOL *symtbl = (const COFF_IMAGE_SYMBOL *)
								(begin + header->PointerToSymbolTable);
							const COFF_IMAGE_SYMBOL *symend = symtbl + header->NumberOfSymbols;
							const char *strtbl = (const char *)symend;
							for(const COFF_IMAGE_SYMBOL *symp = symtbl; symp < symend; symp++) {
/ *
								if(symp->StorageClass == COFF_IMAGE_SYM_CLASS_STATIC && symp->SectionNumber != 0
								&& symp->Value == 0 && symp->NumberOfAuxSymbols >= 1)
									symp += symp->NumberOfAuxSymbols; // section entry
								else
* /
								if((symp->StorageClass == COFF_IMAGE_SYM_CLASS_EXTERNAL
								|| symp->StorageClass == COFF_IMAGE_SYM_CLASS_WEAK_EXTERNAL)
								&& AddCOFFSymbol(COFFSymbolName(*symp, strtbl), of.index, *symp))
									for(int naux = symp->NumberOfAuxSymbols; --naux >= 0;)
										AddAuxCOFFSymbol(*++symp);
								else
									symp += symp->NumberOfAuxSymbols;
							}
						}
*/
					}
				}
		}

	ZeroArray(groups);
	Sort(section_map, &LinkJob::Less);
	section_object_index.Reserve(section_map.GetCount());
//	section_obj_sec_index.Reserve(section_map.GetCount());
	int rva = 0; //, rfa = 0;
	int prev_sec = 0;
	int prev_grp = 0;
	for(NOMSC6(int) i = 0; i < section_map.GetCount(); i++) {
		Section& section = section_map[i];
		int rva_align = image_align, rfa_align = image_align;
		ObjectFile::Section& ofsec = GetSection(section.obj_sec);
//		ofsec_atom = ofsec.name_atom;
		rva_align = CoffGetLengthAlignment(1 << CoffSectionAlignShift(ofsec.flags), ofsec.size);
		rfa_align = rva_align;
//			sec_size = ofsec.size;
		ofsec.sec_map_index = i;
		if(section.sec_atom != prev_sec) {
			section_name_map.Add(prev_sec = section.sec_atom, i);
			rva_align = max(rva_align, image_align);
			rfa_align = max(rfa_align, file_align);
		}
		section.app_section = section_name_map.GetCount() - 1;
		section.rva = rva = (rva + rva_align - 1) & -rva_align;
//		section.rfa = rfa = (rfa + rfa_align - 1) & -rfa_align;
		if(iat2_rva == 0 && ofsec.name_atom == idata_iat2_atom)
			iat2_rva = iat2_end_rva = rva;
		if(idesc_rva == 0 && ofsec.sec_atom == idata_atom)
			idesc_rva = idesc_end_rva = rva;
		if(rsrc_rva == 0 && ofsec.sec_atom == rsrc_atom)
			rsrc_rva = rsrc_end_rva = rva;
		section_object_index.Add(section.obj_sec.object);
//		section_obj_sec_index.Add(section.obj_sec);
		rva += section.size;
//		rfa += ofsec.size;
		if(ofsec.name_atom == idata_iat2_atom)
			iat2_end_rva = rva;
//		if(ofsec.name_atom == idata_idesc_atom || ofsec.name_atom == idata_inull_atom)
//			idesc_end_rva = rva;
		if(ofsec.sec_atom == idata_atom)
			idesc_end_rva = rva;
		if(ofsec.sec_atom == rsrc_atom)
			rsrc_end_rva = rva;
		const Section *nxt = (i + 1 >= section_map.GetCount() ? NULL : &section_map[i + 1]);
		while(prev_grp <= section.group)
			groups[prev_grp++].first_section = i;
	}
	while(prev_grp <= GRP_COUNT)
		groups[prev_grp++].first_section = section_map.GetCount();

	int rfa = 0, end_rva = 0;
	for(NOMSC6(int) i = 0; i <= GRP_COUNT; i++) {
		int first = groups[i].first_section;
		if(first >= section_map.GetCount()) {
			groups[i].rva = end_rva;
			groups[i].rfa = rfa;
			continue;
		}
		const Section& sec = section_map[first];
		int last = groups[i + 1].first_section - 1;
		const Section& lastsec = section_map[last];
//		const ObjectFile::Section& oflast = GetSection(lastsec.obj_sec);
		while(last >= first && section_map[last].udata)
			last--;
		const Section *rawsec = (last >= first ? &section_map[last] : NULL);
//		const ObjectFile::Section& ofsec = GetSection(sec.obj_sec);
		groups[i].rva = sec.rva;
		groups[i].rfa = rfa;
		groups[i].raw_size = rawsec ? rawsec->rva + GetSection(rawsec->obj_sec).size - sec.rva : 0;
		end_rva = (lastsec.rva + sec.size + image_align - 1) & -image_align;
		rfa = (rfa + groups[i].raw_size + file_align - 1) & -file_align;
	}

	for(NOMSC6(int) i = 0; i < section_map.GetCount(); i++) {
		Section& sec = section_map[i];
		Group& grp = groups[sec.group];
		if(!sec.udata)
			sec.rfa = sec.rva - grp.rva + grp.rfa;
	}

	if(dump_flags & DUMP_STAT)
		PutStdOut(NFormat("#collected sections = %d", section_map.GetCount()));
}

void LinkJob::RelocateGlobals()
{
	for(int i = 0; i < globals.GetCount(); i++) {
		Symbol& global = globals[i];
		if(global.used) {
			if(global.type != COFF_IMAGE_SYM_TYPE_BASE && global.obj_sec.section >= 0) {
				ObjSec os = global.obj_sec;
//				if(os.section == 0 && global.type < COFF_IMAGE_SYM_TYPE_SPECIAL)
//					os.object = 0; // comdat
				global.value += section_map[GetSection(os).sec_map_index].rva + code_rva;
			}
			global.relocated = true;
		}
	}
}

void LinkJob::RelocateExports()
{
	ObjectFile& ofe = objects[OBJ_EXPORT];
	if(!ofe.used_any)
		return;

	ObjectFile::Section& dir = ofe.sections[OBJE_DIRECTORY];
	ObjectFile::Section& adr = ofe.sections[OBJE_ADDRESS_TABLE];
	ObjectFile::Section& ptr = ofe.sections[OBJE_NAME_POINTERS];
	ObjectFile::Section& ord = ofe.sections[OBJE_ORDINALS];
	ObjectFile::Section& nam = ofe.sections[OBJE_NAMES];
	ASSERT(dir.sec_map_index >= 0 && adr.sec_map_index >= 0 && nam.sec_map_index >= 0);

	int nam_rva = section_map[nam.sec_map_index].rva + code_rva;
	int ptr_rva = section_map[ptr.sec_map_index].rva + code_rva;
	int adr_rva = section_map[adr.sec_map_index].rva + code_rva;

	COFF_IMAGE_EXPORT_DIRECTORY_TABLE expdir;
	memcpy(&expdir, dir.section_data.Begin(), sizeof(expdir));
	expdir.NameRVA = nam_rva;
	expdir.ExportAddressTableRVA = adr_rva;
	if(ptr.sec_map_index >= 0) {
		ASSERT(ord.sec_map_index >= 0);
		expdir.NamePointerTableRVA = ptr_rva;
		expdir.OrdinalTableRVA = section_map[ord.sec_map_index].rva + code_rva;
	}
	memcpy(dir.section_data.Begin(), &expdir, sizeof(expdir));

	for(int i = 0; i < ptr.size; i += 4) {
		byte *p = ptr.section_data.GetIter(i);
		PokeIL(p, PeekIL(p) + nam_rva);
	}

	for(int i = 0; i < exports.GetCount(); i++) {
		int f = collected_symbols.Get(exports.GetKey(i), -1);
		if(f >= 0) {
			Symbol& sym = globals[f];
			int offset = 4 * (exports[i].ordinal - expdir.OrdinalBase);
			PokeIL(adr.section_data.GetIter(offset), sym.value);
		}
	}
}

bool LinkJob::Less(const Section& a, const Section& b)
{
	int i;
	if((i = a.group - b.group) || (i = cmp(a.name, b.name)))
		return i < 0;
	if((i = a.obj_sec.object - b.obj_sec.object) || (i = a.obj_sec.section - b.obj_sec.section))
		return i < 0;
	return false;
}

void LinkJob::AddAuxCOFFSymbol(const COFF_IMAGE_SYMBOL& sym)
{
	coff_output_symbols.Add() = sym;
}

bool LinkJob::AddCOFFSymbol(String name, int object, const COFF_IMAGE_SYMBOL& sym)
{
	int value = sym.Value;
	int secno = sym.SectionNumber;
//	bool reloc_value = false;
	if(*name == '.' && sym.StorageClass != COFF_IMAGE_SYM_CLASS_FILE)
		return false; // probably also section entry
	if(sym.SectionNumber == 0 && sym.StorageClass == COFF_IMAGE_SYM_CLASS_EXTERNAL
	|| sym.StorageClass != COFF_IMAGE_SYM_CLASS_EXTERNAL) {
/*	|| sym.StorageClass == COFF_IMAGE_SYM_CLASS_BLOCK || sym.StorageClass == COFF_IMAGE_SYM_CLASS_FUNCTION
	|| sym.StorageClass == COFF_IMAGE_SYM_CLASS_END_OF_STRUCT// || sym.StorageClass == COFF_IMAGE_SYM_CLASS_FILE
	|| sym.StorageClass == COFF_IMAGE_SYM_CLASS_REGISTER_PARAM || sym.StorageClass == COFF_IMAGE_SYM_CLASS_ARGUMENT
	|| sym.StorageClass == COFF_IMAGE_SYM_CLASS_REGISTER || sym.StorageClass == COFF_IMAGE_SYM_CLASS_AUTOMATIC
	|| sym.StorageClass == COFF_IMAGE_SYM_CLASS_END_OF_FUNCTION || sym.StorageClass == COFF_IMAGE_SYM_CLASS_SECTION) { // external symbol / anon COMDAT
*/
		return false;
/*
		int global = collected_symbols.Get(Atomize(name), -1);
		if(global < 0)
			return false;
		value = globals[global].value;
		if(globals[global].obj_sec.section > 0)
			reloc_value = true;
*/
	}

	int appsec = sym.SectionNumber;
	if(sym.SectionNumber >= 0) {
		int sec_map_index = GetSection(ObjSec(object, sym.SectionNumber)).sec_map_index;
		if(sec_map_index < 0)
			return false;
		appsec = section_map[sec_map_index].app_section;
		value += section_map[sec_map_index].rva - section_map[section_name_map[appsec]].rva;
		appsec++;
//		reloc_value = true;
	}

	int offset = coff_output_symbols.GetCount() * sizeof(COFF_IMAGE_SYMBOL);
	COFF_IMAGE_SYMBOL& outsym = coff_output_symbols.Add();
	Zero(outsym);
	if(name.GetLength() <= 8)
		memcpy(outsym.N.ShortName, name, name.GetLength());
	else {
		outsym.N.Name.Long = coff_output_symbol_strings.GetLength() + 4;
		coff_output_symbol_strings.Cat(name, name.GetLength() + 1);
//		coff_reloc_offsets.Add(offset + __offsetof(COFF_IMAGE_SYMBOL, N.Name.Long));
	}
//	if(reloc_value)
//	{
//		value += image_base;
//		coff_reloc_offsets.Add(offset + __offsetof(COFF_IMAGE_SYMBOL, Value));
//	}
	outsym.Value = value;
	outsym.SectionNumber = appsec;
	outsym.StorageClass = sym.StorageClass;
	outsym.Type = sym.Type;
	outsym.NumberOfAuxSymbols = sym.NumberOfAuxSymbols;
	return true;
}

void LinkJob::AddGlobal(int atom, const Symbol& symbol)
{
	if(!symbol.external)
		obj_static_global_index.Add(Point(symbol.obj_sec.object, atom), globals.GetCount());
	globals.Add(atom, symbol);
	global_obj_sec_index.Add(symbol.obj_sec);
}

void LinkJob::PrepareImageHeader()
{
	output_image_header.Clear();
	String stub_data;
	if(!stub_filename.IsEmpty()) {
		stub_data = ::LoadFile(stub_filename);
		if(stub_data.IsVoid())
			PutStdOut(NFormat("%s: error loading stub file", stub_filename));
	}
	if(IsNull(stub_data))
		stub_data = String(std_stub, sizeof(std_stub));

	CatN(output_image_header, stub_data.GetLength(), stub_data);
	if(output_image_header.GetCount() < 0x40)
		output_image_header.SetCountR(0x40, 0);
	PokeIL(output_image_header.GetIter(0x3C), output_image_header.GetCount());
	CatN(output_image_header, 4, "PE\0\0");
	ifhdr_pos = CatN0(output_image_header, sizeof(COFF_IMAGE_FILE_HEADER));
	iohdr_pos = CatN0(output_image_header, sizeof(COFF_IMAGE_OPTIONAL_HEADER32));
	section_count = section_name_map.GetCount();
	if(has_stabs && debug_info) {
		stab_section = section_count++;
		stabstr_section = section_count++;
	}
	if(debug_info) {
//		has_symbols = true;
		symbol_section = section_count++;
	}
	if(!image_fixed) {
//		has_relocs = true;
		reloc_section = section_count++;
	}
	secmap_pos = CatN0(output_image_header, sizeof(COFF_IMAGE_SECTION_HEADER) * section_count);
//	impmap_pos = CatN0(output_image, sizeof(COFF_IMAGE_IMPORT_DESCRIPTOR) * (used_dll_objects.GetCount() + 1));
	header_size = output_image_header.GetCount();
	code_rfa = (header_size + file_align - 1) & -file_align;
	code_rva = (header_size + image_align - 1) & -image_align;
	output_image_header.SetCount(code_rfa, 0); // + groups[GRP_COUNT].rfa, 0);
//	memset(output_image_header.GetIter(code_rfa), 0x90, groups[GRP_DATA_BEGIN].rfa - groups[GRP_TEXT_BEGIN].rfa);
}

static bool SymTblLess(const COFF_IMAGE_SYMBOL& a, const COFF_IMAGE_SYMBOL& b)
{
	if(a.SectionNumber != b.SectionNumber) return a.SectionNumber < b.SectionNumber;
	return a.Value < b.Value;
}

void LinkJob::PrepareSymbolTable()
{
	LTIMING("LinkJob::PrepareSymbolTable");

	for(int i = 0; i < globals.GetCount(); i++) {
		const Symbol& sym = globals[i];
		if(sym.used && sym.obj_sec.object >= 0) {
			COFF_IMAGE_SYMBOL cis;
			Zero(cis);
			cis.Value = sym.value;
			cis.SectionNumber = -1;
			cis.StorageClass = COFF_IMAGE_SYM_CLASS_EXTERNAL;
			cis.Type = sym.type;
			cis.NumberOfAuxSymbols = 0;
			if(sym.obj_sec.section >= 0) {
				int sec = GetSection(sym.obj_sec).sec_map_index;
				if(sec < 0)
					continue;
				cis.SectionNumber = section_map[sec].app_section;
				cis.Value -= section_map[section_name_map[cis.SectionNumber]].rva;
			}
			String name = atoms[globals.GetKey(i)];
			if(name.GetLength() <= 8)
				memcpy(cis.N.ShortName, name, name.GetLength());
			else {
				cis.N.Name.Long = coff_output_symbol_strings.GetLength() + 4;
				coff_output_symbol_strings.Cat(name, name.GetLength() + 1);
			}
			coff_output_symbols.Add(cis);
		}
	}

/*
	for(int i = 0; i < section_name_map.GetCount(); i++) { // generate section beginning symbols
		COFF_IMAGE_SYMBOL secsym;
		Zero(secsym);
		strncpy((char *)secsym.N.ShortName, atoms[section_name_map.GetKey(i)], 8);
		secsym.SectionNumber = i + 1;
		secsym.Value = 0;
		secsym.StorageClass = COFF_IMAGE_SYM_CLASS_EXTERNAL;
		coff_output_symbols.Add(secsym);
	}
*/

//	Sort(coff_output_symbols, &SymTblLess);
	int rawsize = coff_output_symbols.GetCount() * sizeof(COFF_IMAGE_SYMBOL)
		+ 4 + coff_output_symbol_strings.GetLength();
	int imagesize = (rawsize + image_align - 1) & -image_align;
	int filesize = (rawsize + file_align - 1) & -file_align;
	for(int g = GRP_SYMBOLS; ++g <= GRP_COUNT;) {
		groups[g].rva += imagesize;
		groups[g].rfa += filesize;
	}
//	for(int g = 0; g < coff_output_symbols.GetCount(); g += 1 + coff_output_symbols[g].NumberOfAuxSymbols)
//		if(coff_output_symbols[g].N.Name.Short == 0)
//			coff_output_symbols[g].N.Name.Long += groups[GRP_SYMBOLS].rva + code_rva + image_base;
}

#ifdef CPU_LE // && CPU_32BIT
	#define GET_STAB(dest, ptr) dest = (STAB_INFO *)(ptr);
#else
	#define GET_STAB(dest, ptr) \
		dest->strdx = PeekIL(ptr + STAB_STRDXOFF); \
		dest->type = ptr[STAB_TYPEOFF]; \
		dest->other = ptr[STAB_OTHEROFF]; \
		dest->desc = PeekIW(ptr + STAB_DESCOFF); \
		dest->value = PeekIL(ptr + STAB_VALUEOFF);
#endif

void LinkJob::PrepareStabBlocks()
{
	LTIMING("LinkJob::PrepareStabBlocks");

	int count_stabs = output_stab_info.GetCount() / STAB_STABSIZE;
	if(dump_flags & DUMP_STABS) {
		STAB_INFO aux_stab, *stab = &aux_stab;
		for(int i = 0; i < count_stabs; i++) {
			GET_STAB(stab, output_stab_info.GetIter(i * STAB_STABSIZE));
			String desc;
			desc << NFormat("[%d]: type %02x, desc %04x, value %08x, %s",
				i, stab->type, stab->desc, stab->value, output_stab_strings.GetIter(stab->strdx));
			puts(desc);
		}
		puts(NFormat("Total %d stabs, .stab = %d B, .stabstr = %d B",
			count_stabs, output_stab_info.GetCount(), output_stab_strings.GetCount()));
	}
/*
	for(int f = 0; f < output_stab_fixup_pos.GetCount(); f++) {
		int sec_index = GetSection(output_stab_fixups[f]).sec_map_index;
		if(sec_index >= 0) {
			int pos = output_stab_fixup_pos[f];
			byte *p = output_stab_info.GetIter(STAB_STABSIZE * pos + STAB_VALOFF);
			PokeIL(p, PeekIL(p) + section_map[sec_index].rva + code_rva + image_base);
			highlow_fixup_rva.Add(pos * STAB_STABSIZE + STAB_VALOFF + groups[GRP_STAB].rva + code_rva);
		}
	}
*/
	PokeIW(output_stab_info.GetIter(STAB_DESCOFF), count_stabs);
	PokeIL(output_stab_info.GetIter(STAB_VALOFF), output_stab_strings.GetCount());

	groups[GRP_STABSTR].rva = groups[GRP_STAB].rva + ((output_stab_info.GetCount() + image_align - 1) & -image_align);
	groups[GRP_STABSTR].rfa = groups[GRP_STAB].rfa + ((output_stab_info.GetCount() + file_align - 1) & -file_align);
	groups[GRP_COUNT].rva = groups[GRP_SYMBOLS].rva = groups[GRP_STABSTR].rva + ((output_stab_strings.GetCount() + image_align - 1) & -image_align);
	groups[GRP_COUNT].rfa = groups[GRP_SYMBOLS].rfa = groups[GRP_STABSTR].rfa + ((output_stab_strings.GetCount() + file_align - 1) & -file_align);
}

void LinkJob::PrepareFixupBlocks()
{
	Sort(highlow_fixup_rva);
	int first = -1;
	Vector<byte> block;
	for(int i = 0; i < highlow_fixup_rva.GetCount(); i++) {
		int fp = highlow_fixup_rva[i];
		ASSERT(i == 0 || fp > highlow_fixup_rva[i - 1]); // duplicate relocations
		if(block.IsEmpty() || ((fp ^ first) & -4096)) {
			if(!block.IsEmpty()) {
				ASSERT(first >= 0);
				if(block.GetCount() & 2) {
					block.Add(0);
					block.Add(0);
				}
				int o = CatN(output_fixups, 8, NULL);
				PokeIL(&output_fixups[o], first);
				PokeIL(&output_fixups[o + 4], 8 + block.GetCount());
				output_fixups.AppendPick(block);
				block.Clear();
			}
			first = fp & -4096;
		}
		fp -= first;
		block.Add(fp);
		block.Add((fp >> 8) + 16 * COFF_IMAGE_REL_BASED_HIGHLOW);
	}
	if(!block.IsEmpty()) {
		ASSERT(first >= 0);
		if(block.GetCount() & 2) {
			block.Add(0);
			block.Add(0);
		}
		int o = CatN(output_fixups, 8, NULL);
		PokeIL(&output_fixups[o], first);
		PokeIL(&output_fixups[o + 4], 8 + block.GetCount());
		output_fixups.AppendPick(block);
	}
}

void LinkJob::PrepareImageInfo()
{
	CheckUnresolved();
	if(debug_info && stab_section >= 0)
		PrepareStabBlocks();
//	if(has_symbols)
	if(symbol_section >= 0)
		PrepareSymbolTable();
	if(!image_fixed)
		PrepareFixupBlocks();

	if(dump_flags & DUMP_SECTIONS) {
		PutStdOut(NFormat("Sections (%d total, %d parts):", section_name_map.GetCount(), section_map.GetCount()));
		for(int i = 0; i < section_name_map.GetCount(); i++) {
			const Section& sec = section_map[section_name_map[i]];
			int last_index = (i + 1 < section_name_map.GetCount()
				? section_name_map[i + 1] : section_map.GetCount()) - 1;
			const Section& last = section_map[last_index];
			const ObjectFile::Section& oflast = GetSection(last.obj_sec);
			PutStdOut(NFormat("%10<s RVA %08x RFA %08x LEN %08x",
				atoms[section_name_map.GetKey(i)], sec.rva, sec.rfa,
				last.rva - sec.rva + oflast.size));
		}
		PutStdOut(NFormat("Section groups (%d total):", GRP_COUNT));
		for(NOMSC6(int) i = 0; i <= GRP_COUNT; i++)
			PutStdOut(NFormat("[%d] RVA: %08x RFA %08x RAWSIZE %08x", i, groups[i].rva, groups[i].rfa, groups[i].raw_size));
	}

	if(dump_flags & DUMP_SECTIONS_ALL) {
		PutStdOut("Section map:");
		for(int i = 0; i < section_map.GetCount(); i++) {
			const Section& sec = section_map[i];
			const ObjectFile& of = objects[sec.obj_sec.object];
			const ObjectFile::Section& ofsec = of.sections[sec.obj_sec.section];
			PutStdOut(NFormat("RVA %08x, RFA %08x (%08x): %10<s %s: %s", sec.rva, sec.rfa,
				ofsec.size, atoms[ofsec.sec_atom], atoms[ofsec.name_atom], FormatSection(sec.obj_sec)));
			if(dump_flags & DUMP_SEC_DEFINES)
				for(int s = global_obj_sec_index.Find(sec.obj_sec); s >= 0; s = global_obj_sec_index.FindNext(s))
					if(globals[s].type != COFF_IMAGE_SYM_TYPE_SECTION)
						PutStdOut(NFormat("\tdefines: %s", atoms[globals.GetKey(s)]));
		}
//		for(i = 0; i < section_name_map.GetCount(); i++)
///			PutStdOut(NFormat("%s
	}

	int i;
	for(i = 0; i < section_name_map.GetCount(); i++) {
		COFF_IMAGE_SECTION_HEADER sechdr;
		Zero(sechdr);
		String name = atoms[section_name_map.GetKey(i)];
		memcpy(sechdr.Name, name, min(name.GetLength(), 8));
		int first_part = section_name_map[i];
		Section& first = section_map[first_part];
		int last_part;
		if(i + 1 < section_name_map.GetCount())
			last_part = section_name_map[i + 1] - 1;
		else
			last_part = section_map.GetCount() - 1;
		const Section& last = section_map[last_part];
		sechdr.Misc.VirtualSize = last.rva - first.rva + GetSection(last.obj_sec).size;
		sechdr.VirtualAddress = code_rva + first.rva;
		sechdr.SizeOfRawData = 0;
		for(; last_part >= first_part; last_part--) {
			const Section& chk = section_map[last_part];
			if(!chk.udata) {
				const ObjectFile::Section& ofsec = GetSection(chk.obj_sec);
				sechdr.SizeOfRawData = chk.rva - first.rva + ofsec.size;
				break;
			}
		}
		sechdr.SizeOfRawData = (sechdr.SizeOfRawData + file_align - 1) & -file_align;
		sechdr.PointerToRawData = (sechdr.SizeOfRawData ? code_rfa + first.rfa : 0);
		sechdr.PointerToRelocations = 0;
		sechdr.PointerToLinenumbers = 0;
		sechdr.Characteristics = GetSection(first.obj_sec).flags
			& (COFF_IMAGE_SCN_CNT_CODE | COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_CNT_UNINITIALIZED_DATA
				| COFF_IMAGE_SCN_MEM_EXECUTE | COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE
				| COFF_IMAGE_SCN_MEM_SHARED | COFF_IMAGE_SCN_MEM_DISCARDABLE | COFF_IMAGE_SCN_MEM_NOT_CACHED
				| COFF_IMAGE_SCN_MEM_NOT_PAGED);
		memcpy(&output_image_header[secmap_pos + (int)sizeof(COFF_IMAGE_SECTION_HEADER) * i], &sechdr, sizeof(sechdr));
	}

	enum {
//		STAB_SEC_STYLE = COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_MEM_READ
//			| COFF_IMAGE_SCN_MEM_SHARED | COFF_IMAGE_SCN_MEM_DISCARDABLE
		STAB_SEC_STYLE = COFF_IMAGE_SCN_MEM_DISCARDABLE | COFF_IMAGE_SCN_MEM_READ
		| COFF_IMAGE_SCN_TYPE_NOLOAD | COFF_IMAGE_SCN_LNK_REMOVE
	};

	if(stab_section >= 0) {
		COFF_IMAGE_SECTION_HEADER stab;
		Zero(stab);
		strcpy((char *)stab.Name, ".stab");
		stab.Misc.VirtualSize = output_stab_info.GetCount();
		stab.SizeOfRawData = (stab.Misc.VirtualSize + file_align - 1) & -file_align;
		stab.VirtualAddress = groups[GRP_STAB].rva + code_rva;
		stab.PointerToRawData = groups[GRP_STAB].rfa + code_rfa;
		stab.Characteristics = STAB_SEC_STYLE;
		memcpy(&output_image_header[secmap_pos + (int)sizeof(COFF_IMAGE_SECTION_HEADER) * stab_section],
			&stab, sizeof(stab));
	}

	if(stabstr_section >= 0) {
		COFF_IMAGE_SECTION_HEADER stabstr;
		Zero(stabstr);
		memcpy((char *)stabstr.Name, ".stabstr", 8);
		stabstr.Misc.VirtualSize = output_stab_strings.GetCount();
		stabstr.SizeOfRawData = (stabstr.Misc.VirtualSize + file_align - 1) & -file_align;
//		stabstr.Misc.VirtualSize = stabstr.SizeOfRawData = groups[GRP_COUNT].rva - groups[GRP_STABSTR].rva;
		stabstr.VirtualAddress = groups[GRP_STABSTR].rva  + code_rva;
		stabstr.PointerToRawData = groups[GRP_STABSTR].rfa + code_rfa;
		stabstr.Characteristics = STAB_SEC_STYLE;
		memcpy(&output_image_header[secmap_pos + (int)sizeof(COFF_IMAGE_SECTION_HEADER) * stabstr_section],
			&stabstr, sizeof(stabstr));
	}

	if(symbol_section >= 0) {
		COFF_IMAGE_SECTION_HEADER sym;
		Zero(sym);
		strcpy((char *)sym.Name, ".symbol");
		sym.Misc.VirtualSize = coff_output_symbols.GetCount() * sizeof(COFF_IMAGE_SYMBOL)
			+ 4 + coff_output_symbol_strings.GetLength();
		sym.SizeOfRawData = (sym.Misc.VirtualSize + file_align - 1) & -file_align;
		sym.VirtualAddress = groups[GRP_SYMBOLS].rva + code_rva;
		sym.PointerToRawData = groups[GRP_SYMBOLS].rfa + code_rfa;
		sym.Characteristics = COFF_IMAGE_SCN_LNK_REMOVE | COFF_IMAGE_SCN_MEM_DISCARDABLE
			| COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_TYPE_NOLOAD;
		memcpy(&output_image_header[secmap_pos + (int)sizeof(COFF_IMAGE_SECTION_HEADER) * symbol_section],
			&sym, sizeof(sym));
	}

	output_fixup_size = 0;
	if(reloc_section >= 0) { // fill in relocation segment header
		COFF_IMAGE_SECTION_HEADER reloc;
		Zero(reloc);
		strcpy((char *)reloc.Name, ".reloc");
		reloc.SizeOfRawData = reloc.Misc.VirtualSize = output_fixups.GetCount();
		output_fixup_size = (reloc.Misc.VirtualSize + file_align - 1) & -file_align;
		reloc.VirtualAddress = groups[GRP_COUNT].rva + code_rva;
		reloc.PointerToRawData = groups[GRP_COUNT].rfa + code_rfa;
		reloc.Characteristics = COFF_IMAGE_SCN_MEM_DISCARDABLE | COFF_IMAGE_SCN_MEM_READ;
		memcpy(&output_image_header[secmap_pos + (int)sizeof(COFF_IMAGE_SECTION_HEADER) * reloc_section],
			&reloc, sizeof(reloc));
	}

/*
	for(i = 0; i < used_dll_objects.GetCount(); i++) {
		const ObjectFile& of = objects[used_dll_objects[i]];
		COFF_IMAGE_IMPORT_DESCRIPTOR idesc;
		Zero(idesc);
		idesc.OriginalFirstThunk = section_map[of.sections[ObjectFile::SEC_DLL_IMPORTS].sec_map_index].rva;
		idesc.ForwarderChain = -1;
		idesc.Name = section_map[of.sections[ObjectFile::SEC_DLL_NAMES].sec_map_index].rva;
		idesc.FirstThunk = section_map[of.sections[ObjectFile::SEC_DLL_BOUND].sec_map_index].rva;
		memcpy(&output_image[impmap_pos + i * (int)sizeof(idesc)], &idesc, sizeof(idesc));
	}
*/
	COFF_IMAGE_OPTIONAL_HEADER32 opthdr;
	Zero(opthdr);
	opthdr.Magic = 0x10b; // PE32; PE32+ = 0x20b
	opthdr.MajorLinkerVersion = ULD_VERSION_MAJOR * 10 + ULD_VERSION_MINOR;
	opthdr.MinorLinkerVersion = ULD_VERSION_RELEASE;
	opthdr.SizeOfCode = groups[GRP_DATA_BEGIN].rva;
	opthdr.SizeOfInitializedData = groups[GRP_BSS_BEGIN].rva - groups[GRP_DATA_BEGIN].rva;
	opthdr.SizeOfUninitializedData = groups[GRP_BSS_END].rva - groups[GRP_BSS_BEGIN].rva;
	int entry = collected_symbols[0];
	if(entry >= 0) {
		const Symbol& sym = globals[entry];
		if(sym.obj_sec.section < 0 && !image_fixed)
			PutStdOut(NFormat("%s: absolute entry point in non-fixed application, application may not work", atoms[globals.GetKey(entry)]));
		opthdr.AddressOfEntryPoint = sym.value;
	}
	opthdr.BaseOfCode = code_rva;
	opthdr.BaseOfData = code_rva + groups[GRP_DATA_BEGIN].rva;

	opthdr.ImageBase = image_base;
	opthdr.SectionAlignment = image_align;
	opthdr.FileAlignment = file_align;
	opthdr.MajorOperatingSystemVersion = 4;
	opthdr.MinorOperatingSystemVersion = 0;
	opthdr.MajorImageVersion = major_version;
	opthdr.MinorImageVersion = minor_version;
	opthdr.MajorSubsystemVersion = 4;
	opthdr.MinorSubsystemVersion = 0;
	opthdr.Win32VersionValue = 0;
//	opthdr.SizeOfImage = (code_rva + groups[GRP_COUNT].rva + output_fixups.GetCount() + file_align - 1) & -file_align;
	opthdr.SizeOfImage = code_rva + groups[GRP_COUNT].rva + ((output_fixup_size + image_align - 1) & -image_align);
	opthdr.SizeOfHeaders = code_rfa;
	opthdr.CheckSum = 0;
	opthdr.Subsystem = subsystem;
	opthdr.DllCharacteristics;
	opthdr.SizeOfStackReserve = stack_reserve;
	opthdr.SizeOfStackCommit = stack_commit;
	opthdr.SizeOfHeapReserve = heap_reserve;
	opthdr.SizeOfHeapCommit = heap_commit;
	opthdr.LoaderFlags = 0;
	opthdr.NumberOfRvaAndSizes = __countof(opthdr.DataDirectory);
	opthdr.DataDirectory[COFF_IMAGE_DIRECTORY_ENTRY_BASERELOC].Size = output_fixups.GetCount();
	if(!output_fixups.IsEmpty())
		opthdr.DataDirectory[COFF_IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress = groups[GRP_COUNT].rva + code_rva;
	if(opthdr.DataDirectory[COFF_IMAGE_DIRECTORY_ENTRY_EXPORT].Size = groups[GRP_EDATA].raw_size)
		opthdr.DataDirectory[COFF_IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress = groups[GRP_EDATA].rva + code_rva;
	if(opthdr.DataDirectory[COFF_IMAGE_DIRECTORY_ENTRY_IMPORT].Size = idesc_end_rva - idesc_rva)
		opthdr.DataDirectory[COFF_IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = idesc_rva + code_rva;
	if(opthdr.DataDirectory[COFF_IMAGE_DIRECTORY_ENTRY_IAT].Size = iat2_end_rva - iat2_rva)
		opthdr.DataDirectory[COFF_IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress = iat2_rva + code_rva;
	if(opthdr.DataDirectory[COFF_IMAGE_DIRECTORY_ENTRY_RESOURCE].Size = rsrc_end_rva - rsrc_rva)
		opthdr.DataDirectory[COFF_IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress = code_rva + rsrc_rva;
	memcpy(&output_image_header[iohdr_pos], &opthdr, sizeof(opthdr));

	COFF_IMAGE_FILE_HEADER ifhdr;
	Zero(ifhdr);
	ifhdr.Machine = machine;
	ifhdr.NumberOfSections = section_count;
	ifhdr.TimeDateStamp = timestamp = time(NULL);
	ifhdr.PointerToSymbolTable = 0;
	ifhdr.NumberOfSymbols = 0;
	if(symbol_section >= 0) {
		ifhdr.PointerToSymbolTable = code_rfa + groups[GRP_SYMBOLS].rfa;
		ifhdr.NumberOfSymbols = coff_output_symbols.GetCount();
	}
	ifhdr.SizeOfOptionalHeader = sizeof(opthdr); //!! temporary
	ifhdr.Characteristics = COFF_IMAGE_FILE_EXECUTABLE_IMAGE
		| COFF_IMAGE_FILE_32BIT_MACHINE
//		| COFF_IMAGE_FILE_BYTES_REVERSED_LO
		| COFF_IMAGE_FILE_LINE_NUMS_STRIPPED
		| (debug_info ? 0 : COFF_IMAGE_FILE_LOCAL_SYMS_STRIPPED)
		| (debug_info ? 0 : COFF_IMAGE_FILE_DEBUG_STRIPPED)
		| (image_fixed ? COFF_IMAGE_FILE_RELOCS_STRIPPED : 0)
		| (make_dll ? COFF_IMAGE_FILE_DLL : 0)
		// COFF_IMAGE_FILE_AGGRESSIVE_WS_TRIM
		// COFF_IMAGE_FILE_LARGE_ADDRESS_AWARE
		// COFF_IMAGE_FILE_16BIT_MACHINE
		// COFF_IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP
		// COFF_IMAGE_FILE_SYSTEM
		// COFF_IMAGE_FILE_DLL
		// COFF_IMAGE_FILE_UP_SYSTEM_ONLY
		// COFF_IMAGE_FILE_BYTES_REVERSED_HI
		;
	memcpy(&output_image_header[ifhdr_pos], &ifhdr, sizeof(ifhdr));

	if(dump_flags & DUMP_STAT) {
		PutStdOut(NFormat("Preferred image base: %08x", image_base));
		PutStdOut(NFormat("Entry point RVA:      %08x", (int)opthdr.AddressOfEntryPoint));
	}
}

void LinkJob::WriteImageFile()
{
	outputfile = AppendExt(outputfile, ".exe");
	FileOut fo;
	if(!fo.Open(outputfile))
		throw Exc(NFormat("%s: error creating file", outputfile));
	if(verbose)
		PutStdOut(NFormat("%s: writing output file (%d bytes)",
			outputfile, groups[GRP_COUNT].rfa + code_rfa + output_fixups.GetCount()));
	fo.Put(output_image_header.Begin(), output_image_header.GetCount());
	byte fill = 0x90;
	for(int i = 0; i < section_map.GetCount(); i++) {
		const Section& sec = section_map[i];
		if(sec.udata || sec.rfa < 0)
			continue;
		int offset = sec.rfa + code_rfa;
		if((unsigned)offset > fo.GetPos())
			fo.Put(fill, offset - fo.GetPos());
		const ObjectFile::Section& ofsec = GetSection(sec.obj_sec);
		fo.Put(ofsec.section_data.Begin(), min(ofsec.section_data.GetCount(), sec.size));
		if(sec.group != GRP_CODE)
			fill = 0x00;
	}
	if(stab_section >= 0) {
		fo.Put(0, int(groups[GRP_STAB].rfa + code_rfa - fo.GetPos()));
		fo.Put(output_stab_info.Begin(), output_stab_info.GetCount());
	}
	if(stabstr_section >= 0) {
		fo.Put(0, int(groups[GRP_STABSTR].rfa + code_rfa - fo.GetPos()));
		fo.Put(output_stab_strings.Begin(), output_stab_strings.GetCount());
	}
	if(symbol_section >= 0) {
		fo.Put(0, int(groups[GRP_SYMBOLS].rfa + code_rfa - fo.GetPos()));
		fo.Put(coff_output_symbols.Begin(), coff_output_symbols.GetCount() * sizeof(COFF_IMAGE_SYMBOL));
		fo.PutIL(coff_output_symbol_strings.GetLength() + 4);
		fo.Put(coff_output_symbol_strings.Begin(), coff_output_symbol_strings.GetLength());
	}
	fo.Put(0, int(code_rfa + groups[GRP_COUNT].rfa - fo.GetPos()));
	if(!image_fixed)
		fo.Put(output_fixups.Begin(), output_fixups.GetCount());
//	fo.Put(0, int(opthdr.SizeOfImage - fo.GetPos()));
	fo.Close();
	if(fo.IsError()) {
		FileDelete(outputfile);
		throw Exc(NFormat("%s: error writing output file", outputfile));
	}

	if(write_mapfile)
		WriteMapFile();
}

/*
static const char cachefile_header[32] = "ULD Link Cache File v. 1\r\n\x1A\0";
enum
{
	CACHE_TAG_SIZE    = 32,
	CACHE_DIROFF_SIZE = 4,
	CACHE_DATA_POS    = CACHE_TAG_SIZE + CACHE_DIROFF_SIZE,
	CACHE_DIR_HEAD    = 0x02468ACE,
	CACHE_DIR_TAIL    = 0x13579BDF,
	CACHE_MAX_SIZE    = 500000000,
};
*/

/*
void LinkJob::OpenCacheFile()
{
	cache_dirty = false;
	if(!cache_objects || IsNull(cache_file_name))
		return;
	if(cache_file.Open(cache_file_name, FileStream::READWRITE)) {
		char buffer[CACHE_TAG_SIZE + CACHE_DIROFF_SIZE];
		if(!cache_file.GetAll(buffer, sizeof(buffer)) || memcmp(buffer, cachefile_header, CACHE_TAG_SIZE))
			cache_file.SetError();
		else {
			int offset = PeekIL(buffer + CACHE_TAG_SIZE); ASSERT(CACHE_DIROFF_SIZE == 4);
			if(offset < CACHE_DATA_POS || offset >= (int)cache_file.GetSize() - 8)
				cache_file.SetError();
			else {
				cache_file.Seek(offset);
				if(cache_file.GetIL() != CACHE_DIR_HEAD)
					cache_file.SetError();
				else {
					cache_file.SetLoading();
					cache_file % object_cache;
					int tail = cache_file.GetIL();
					if(tail != CACHE_DIR_TAIL || cache_file.GetLeft() != 0)
						cache_file.SetError();
				}
			}
		}
		if(cache_file.IsError()) {
			PutStdOut(NFormat("Rewriting cache file '%s'", cache_file_name));
			object_cache.Clear();
			cache_file.ClearError();
			cache_file.Seek(0);
			cache_file.SetSize(0);
		}
		else
			PutStdOut(NFormat("Open cache file '%s', %d objects", cache_file_name, object_cache.GetCount()));
		cache_name_index.Clear();
		cache_library_index.Clear();
		for(int i = 0; i < object_cache.GetCount(); i++) {
			const Cache& obj = object_cache[i];
			cache_name_index.Add(String().Cat() << obj.library << ':' << obj.file);
			cache_library_index.Add(obj.library);
		}
	}
}
*/

/*
void LinkJob::SaveCacheFile()
{
	if(!cache_objects)
		return;
	if(cache_dirty) {
		if(!cache_file.IsError()) {
			int first_pos = CACHE_DATA_POS, used_size = 0;
			for(int i = 0; i < object_cache.GetCount(); i++) {
				used_size += object_cache[i].size;
				first_pos = object_cache[i].offset + object_cache[i].size;
			}
			if(cache_file.GetSize() >= 2u * used_size)
				cache_file.SetError();
			else {
				cache_file.Seek(first_pos);
				cache_file.PutIL(CACHE_DIR_HEAD);
				cache_file.SetStoring();
				cache_file % object_cache;
				cache_file.PutIL(CACHE_DIR_TAIL);
				cache_file.SetSize(cache_file.GetSize());
				if(!cache_file.IsError()) {
					cache_file.Seek(CACHE_TAG_SIZE);
					cache_file.PutIL(first_pos);
				}
			}
		}
	}
	cache_file.Close();
	if(cache_file.IsError())
		FileDelete(cache_file_name);
}
*/

void LinkJob::WriteImageSections()
{
	LTIMING("LinkJob::WriteImageSections");

//	OpenCacheFile();

	for(int i = 0; i < objects.GetCount(); i++) {
		ObjectFile& of = objects[i];
		if(!of.used_any)
			continue;
		if(of.dll_atom)
			RelocateImport(of);
		else if(i == OBJ_RSRC)
			RelocateResource(of);
		else if(of.filetype == ObjectFile::INTERNAL)
			RelocateInternal(of);
		else {
			LTIMING("relocating objects");
			RelocateObject(of);
			if(debug_info) {
				String ofile = Nvl(of.library_file, of.object_file);
				FileMapping& mapping = GetMapping(ofile);
				if(!mapping.IsOpen())
					throw Exc(NFormat("%s: error opening file", ofile));
				const byte *begin = mapping.Begin() + of.library_offset;
				RelocateStabs(of, begin);
			}
		}
	}

//	SaveCacheFile();
}

void LinkJob::RelocateImport(ObjectFile& of)
{
	const ObjectFile::Section& stubsec = of.sections[ObjectFile::SEC_DLL_STUBS];
	const ObjectFile::Section& impsec = of.sections[ObjectFile::SEC_DLL_IMPORTS];
	int impsec_rva = section_map[impsec.sec_map_index].rva + code_rva;
	const ObjectFile::Section& bimpsec = of.sections[ObjectFile::SEC_DLL_BOUND];
	int bimpsec_rva = section_map[bimpsec.sec_map_index].rva + code_rva;
	const ObjectFile::Section& namesec = of.sections[ObjectFile::SEC_DLL_NAMES];
	int namesec_rva = section_map[namesec.sec_map_index].rva + code_rva;
	const ObjectFile::Section& descsec = of.sections[ObjectFile::SEC_DLL_DESCRIPTOR];
	int descsec_rva = section_map[descsec.sec_map_index].rva + code_rva;
	for(int s = section_object_index.Find(of.index); s >= 0; s = section_object_index.FindNext(s)) {
		const Section& sec = section_map[s];
		ASSERT(sec.obj_sec.object == of.index);
		int sec_rva = code_rva + sec.rva;
//		byte *sbegin = &output_image[code_rfa + sec.rfa];
		ObjectFile::Section& ofsec = of.sections[sec.obj_sec.section];
		ofsec.section_data.SetCount(ofsec.size, 0);
		byte *sbegin = ofsec.section_data.Begin();
//		memcpy(sbegin, ofsec.section_data.Begin(), ofsec.section_data.GetCount());
		if(sec.obj_sec.section == ObjectFile::SEC_DLL_STUBS) {
			ASSERT(ofsec.size == IMP_STUB_SIZE * of.used_stubs.GetCount());
			for(int i = 0; i < of.used_stubs.GetCount(); i++, sbegin += IMP_STUB_SIZE) {
				sbegin[0] = 0xFF; // JMP dword ptr [12345678]
				sbegin[1] = 0x25;
				int imp = (1 + of.used_imports.Find(of.used_stubs[i])) * IMP_ENTRY_SIZE;
				ASSERT(imp >= 0);
				PokeIL(sbegin + 2, image_base + bimpsec_rva + imp);
				highlow_fixup_rva.Add(sec_rva + 2 + i * IMP_STUB_SIZE);
			}
		}
		else if(sec.obj_sec.section == ObjectFile::SEC_DLL_IMPORTS || sec.obj_sec.section == ObjectFile::SEC_DLL_BOUND) {
			for(int i = 0; i < of.used_imports.GetCount(); i++) {
				sbegin += IMP_ENTRY_SIZE;
				int imptr = PeekIL(sbegin);
				if(!(imptr & 0x80000000)) {
					ASSERT(namesec_rva >= 0);
					imptr += namesec_rva;
					PokeIL(sbegin, imptr);
//					highlow_fixup_rva.Add(sec_rva + IMP_ENTRY_SIZE * (i + 1));
				}
			}
		}
		else if(sec.obj_sec.section == ObjectFile::SEC_DLL_DESCRIPTOR) {
			COFF_IMAGE_IMPORT_DESCRIPTOR idesc;
			Zero(idesc);
			idesc.OriginalFirstThunk = impsec_rva + IMP_ENTRY_SIZE;
			idesc.ForwarderChain = 0;
			idesc.Name = namesec_rva;
			idesc.FirstThunk = bimpsec_rva + IMP_ENTRY_SIZE;
			memcpy(sbegin, &idesc, sizeof(idesc));
//			highlow_fixup_rva.Add(sec_rva + __offsetof(COFF_IMAGE_IMPORT_DESCRIPTOR, OriginalFirstThunk));
//			highlow_fixup_rva.Add(sec_rva + __offsetof(COFF_IMAGE_IMPORT_DESCRIPTOR, Name));
//			highlow_fixup_rva.Add(sec_rva + __offsetof(COFF_IMAGE_IMPORT_DESCRIPTOR, FirstThunk));
		}
	}
}

void LinkJob::RelocateInternal(ObjectFile& of)
{
	LTIMING("LinkJob::RelocateInternal");

#ifdef _DEBUG
	for(int s = section_object_index.Find(of.index); s >= 0; s = section_object_index.FindNext(s)) {
		const Section& sec = section_map[s];
		if(sec.obj_sec.object == 0 && sec.obj_sec.section == 0)
			continue; // comdat
		ASSERT(sec.obj_sec.object == of.index);
//		int destseg_rva = code_rva + sec.rva;
//		byte *sbegin = &output_image[code_rfa + sec.rfa];
		const ObjectFile::Section& ofsec = GetSection(sec.obj_sec);
//		byte *sbegin = ofsec.section_data.Begin();
		if(/*ofsec.type == ObjectFile::SEC_INULL ||*/ ofsec.type == ObjectFile::SEC_ANON_COMDAT)
			continue;
		ASSERT(ofsec.type == ObjectFile::SEC_STD || ofsec.type == ObjectFile::SEC_RAW);
//		int datalen = min(ofsec.section_data.GetCount(), ofsec.size);
//		memcpy(sbegin, ofsec.section_data.Begin(), datalen);
	}
#endif
}

void LinkJob::RelocateResource(ObjectFile& of)
{
	ObjectFile::Section& entsec = of.sections[OBJR_ENTRIES];
	int trg = section_map[entsec.sec_map_index].rva + code_rva;
	int sec = section_map[of.sections[OBJR_DATA].sec_map_index].rva + code_rva;
	for(int i = __offsetof(COFF_IMAGE_RESOURCE_DATA_ENTRY, OffsetToData);
	i < entsec.section_data.GetCount(); i += sizeof(COFF_IMAGE_RESOURCE_DATA_ENTRY)) {
		byte *ptr = &entsec.section_data[i];
		PokeIL(ptr, PeekIL(ptr) + sec);
		if(!image_fixed)
			highlow_fixup_rva.Add(trg + i);
	}
}

void LinkJob::RelocateObject(ObjectFile& of)
{
	LTIMING("LinkJob::RelocateObject");

//	const COFF_IMAGE_FILE_HEADER *header = (const COFF_IMAGE_FILE_HEADER *)begin;
//	const COFF_IMAGE_SECTION_HEADER *sechdr = (const COFF_IMAGE_SECTION_HEADER *)(begin
//		+ sizeof(COFF_IMAGE_FILE_HEADER) + header->SizeOfOptionalHeader);
//	const COFF_IMAGE_SYMBOL *symtbl = (const COFF_IMAGE_SYMBOL *)(begin + header->PointerToSymbolTable);
//	const char *strtbl = (const char *)(symtbl + header->NumberOfSymbols);
	for(int s = section_object_index.Find(of.index); s >= 0; s = section_object_index.FindNext(s)) {
		const Section& sec = section_map[s];
		if(sec.obj_sec.object == 0 && sec.obj_sec.section == 0 || sec.udata)
			continue; // comdat
		ASSERT(sec.obj_sec.object == of.index);
		int destseg_rva = code_rva + sec.rva;
//		byte *sbegin = &output_image[code_rfa + sec.rfa];
		ObjectFile::Section& ofsec = GetSection(sec.obj_sec);
		ASSERT(ofsec.type == ObjectFile::SEC_STD);
//		const COFF_IMAGE_SECTION_HEADER& secdata = sechdr[sec.obj_sec.section - 1];
//		ofsec.section_data.SetCount(ofsec.raw_size);
		byte *sbegin = ofsec.section_data.Begin();
//		if(ofsec.offset)
//			memcpy(ofsec.section_data.Begin(), begin + ofsec.offset, ofsec.raw_size);
//		const COFF_IMAGE_RELOCATION *relptr = (const COFF_IMAGE_RELOCATION *)(begin + secdata.PointerToRelocations);
//		const COFF_IMAGE_RELOCATION *relend = relptr + secdata.NumberOfRelocations;
		for(const COFF_IMAGE_RELOCATION *relptr = ofsec.coff_reloc.Begin(),
		*relend = ofsec.coff_reloc.End(); relptr < relend; relptr++) {
			int delta_rva = relptr->VirtualAddress - ofsec.coff_reloc_offset;
			int target_rva = destseg_rva + delta_rva;
			byte *target = sbegin + delta_rva; //&output_image[(int)(code_rfa + sec.rfa + delta_rva)];
			int value = 0;
			ObjSec obj_sec = Null;
			ObjectFile::Symbol relsym = of.coff_symbols[(int)relptr->SymbolTableIndex];
			if(relsym.section_atom == 0) {
				value = relsym.value;
				obj_sec.section = -1;
			}
			else if(relsym.section_atom < 0) {
/*
			else if(relsym.StorageClass == COFF_IMAGE_SYM_CLASS_EXTERNAL
//			|| relsym.SectionNumber > 0 && of.sections[relsym.SectionNumber].flags & COFF_IMAGE_SCN_LNK_COMDAT
			) {
*/
				int coll = collected_symbols.Find(~relsym.section_atom);
				if(coll < 0 || collected_symbols[coll] < 0) {
					unresolved.FindAdd(~relsym.section_atom);
					continue;
				}
				const Symbol& sym = globals[collected_symbols[coll]];
				if(!sym.relocated) {
					unresolved.FindAdd(~relsym.section_atom);
					continue;
				}
				value = sym.value;
				obj_sec = sym.obj_sec;
			}
			else { // if(relsym.SectionNumber > 0) {
				int sec_map_index = of.sections[relsym.section_atom].sec_map_index;
				if(sec_map_index >= 0) {
					obj_sec.object = of.index;
					obj_sec.section = relsym.section_atom;
					value = relsym.value + section_map[sec_map_index].rva + code_rva;
				}
				else
					PutStdOut(NFormat("%s(%d): invalid static relocation to section %d",
						AsString(of), sec.obj_sec.section, relsym.section_atom));
			}
//			else
//				PutStdOut(NFormat("%s: invalid relocation to %s (%d)",
//					AsString(of), atoms[ref_atom], relsym.SectionNumber));
			switch(relptr->Type) {
			case COFF_IMAGE_REL_I386_ABSOLUTE:
				break;
//				case COFF_IMAGE_REL_I386_DIR16:
//				case COFF_IMAGE_REL_I386_REL16:
			case COFF_IMAGE_REL_I386_DIR32:
				PokeIL(target, PeekIL(target) + value + (obj_sec.section >= 0 ? image_base : 0));
				if(obj_sec.section >= 0 && !image_fixed)
					highlow_fixup_rva.Add(target_rva);
				break;

			case COFF_IMAGE_REL_I386_DIR32NB:
				PokeIL(target, PeekIL(target) + value);
				break;

//				case COFF_IMAGE_REL_I386_SEG12:
			case COFF_IMAGE_REL_I386_SECTION:
			case COFF_IMAGE_REL_I386_SECREL: {
					int xsec = obj_sec.section >= 0 ? section_map[GetSection(obj_sec).sec_map_index].app_section : -1;
					if(relptr->Type == COFF_IMAGE_REL_I386_SECTION)
						PokeIW(target, PeekIW(target) + xsec);
					else
						PokeIL(target, PeekIL(target) + code_rva + sec.rva - (xsec >= 0 ? section_map[section_name_map[xsec]].rva : 0));
				}
				break;

			case COFF_IMAGE_REL_I386_REL32:
				PokeIL(target, PeekIL(target) + value - target_rva - 4);
				break;

			default:
				PutStdOut(NFormat("%s: invalid relocation type %d", AsString(of), relptr->Type));
				break;
			}
		}
	}
}

void LinkJob::RelocateStabs(ObjectFile& of, const byte *begin)
{
	LTIMING("LinkJob::RelocateStabs");

	if(of.stab_index < 0 || of.stabstr_index < 0)
		return;
	if(dump_flags & DUMP_STAB_TYPES)
		puts(NFormat("Relocating STABs for object '%s':", AsString(of)));
	const char *stabstrtbl = (const char *)begin + of.sections[of.stabstr_index].offset;
	const ObjectFile::Section& sec = of.sections[of.stab_index];
	ASSERT(sizeof(STAB_INFO) == 12);
	int count = sec.size / sizeof(STAB_INFO);
	STAB_INFO aux_stab, *stab = &aux_stab;
	STAB_INFO aux_stab2, *stab2 = &aux_stab2;
//	int match_inc = -1;
//	int begin_inc = -1;
	int fileatom = 0;
	Vector<bool> incl_stack;
	bool incl_use = true;
	int func_use = -1;
//	Stab filestab;
	const COFF_IMAGE_FILE_HEADER *header = (const COFF_IMAGE_FILE_HEADER *)begin;
	const COFF_IMAGE_SECTION_HEADER *secptr = (const COFF_IMAGE_SECTION_HEADER *)(begin
		+ sizeof(COFF_IMAGE_FILE_HEADER) + header->SizeOfOptionalHeader) + (of.stab_index - 1);
	const COFF_IMAGE_RELOCATION *reloc = (const COFF_IMAGE_RELOCATION *)(begin + secptr->PointerToRelocations);
	const COFF_IMAGE_SYMBOL *symtbl = (const COFF_IMAGE_SYMBOL *)(begin + header->PointerToSymbolTable), *symptr = symtbl;
	const char *strtbl = (const char *)(symtbl + header->NumberOfSymbols);
	Vector<int> stab_ref;
	stab_ref.SetCount(count, 0);

	for(int nreloc = secptr->NumberOfRelocations; --nreloc >= 0; reloc++) {
		int entry = reloc->VirtualAddress / sizeof(STAB_INFO);
		if(entry < 0 || entry >= count)
			continue;
		const COFF_IMAGE_SYMBOL& sym = symtbl[reloc->SymbolTableIndex];
		if(sym.SectionNumber > 0 && !(of.sections[sym.SectionNumber].flags & COFF_IMAGE_SCN_LNK_COMDAT))
			stab_ref[entry] = ~sym.SectionNumber;
		else if(sym.StorageClass == COFF_IMAGE_SYM_CLASS_EXTERNAL || sym.StorageClass == COFF_IMAGE_SYM_CLASS_STATIC)
			stab_ref[entry] = NameAtom(sym, strtbl);
	}

	Index<String> types_found;
	Index<String> type_stab_ident;
	Vector<int> type_stab_index;
	Vector<const char *> type_stab_def;

	LTIMING("LinkJob::RelocateStabs / stepping through stabs");
	int lastfile = 0;
	const byte *pstab = begin + sec.offset;
	int out_index = output_stab_info.GetCount() / STAB_STABSIZE;
	for(int istab = 0; istab < count; istab++, pstab += sizeof(STAB_INFO)) {
		GET_STAB(stab, pstab)
		if(stab->type == 0)
			continue;
		bool use_stab = !!func_use;
		Stab outstab;
		outstab.type = stab->type;
		if(stab->type == N_BINCL) { // include matching
			LTIMING("LinkJob::RelocateStabs / checking includes");
			use_stab = true;
			int checksum = 0;
			int nesting = 0;
			const char *textptr = stabstrtbl + stab->strdx;
			const byte *npstab = pstab;
			int nstab = istab;
			while(++nstab < count) {
				npstab += sizeof(STAB_INFO);
				GET_STAB(stab2, npstab);
				if(stab2->type == N_EINCL) {
					if(--nesting < 0)
						break;
				}
				else if(stab2->type == N_BINCL)
					nesting++;
				else if(nesting == 0) {
					LTIMING("LinkJob::RelocateStabs / calculating checksums");
					const char *str = stabstrtbl + stab2->strdx;
					for(byte c; c = *str++;)
						if(c == '(') {
							while((byte)(*str++ - '0') < 10)
								;
							str--;
						}
						else if(*str == '(') {
							checksum += c;
							str++;
							while((byte)(*str++ - '0') < 10)
								;
							str--;
						}
						else if(*str == 0) {
							checksum += c;
							break;
						}
						else {
							checksum += c;
							str++;
						}
				}
			}
			String hash;
			int len = strlen(textptr);
			hash.Reserve(len + sizeof(checksum));
			hash.Cat(textptr, len);
			hash.Cat((const char *)&checksum, sizeof(checksum));
			incl_stack.Add(incl_use);
			int sc = stab_bincl_map.GetCount();
			incl_use = (stab_bincl_map.FindAdd(hash) >= sc);
//			incl_use = true;
			if(!incl_use)
				outstab.type = N_EXCL;
		}
		else if(stab->type == N_EINCL) {
			use_stab = incl_use;
			if(!incl_stack.IsEmpty())
				incl_use = incl_stack.Pop();
		}
		else if(stab->type == N_FUN) {
			if(stab->strdx) { // beginning
				int sr = stab_ref[istab];
				func_use = (sr < 0 ? of.sections[~sr].used : collected_symbols.Find(sr) >= 0) ? 1 : 0;
				use_stab = !!func_use;
			}
			else {
				func_use = -1;
			}
		}
		else if(func_use < 0 && (stab->type == N_STSYM || stab->type == N_LCSYM)) {
			int sr = stab_ref[istab];
			use_stab = (sr < 0 ? of.sections[~sr].used : collected_symbols.Find(sr) >= 0);
		}
		else
			use_stab &= incl_use;

		if(use_stab) {
			LTIMING("LinkJob::RelocateStabs / used stab");
			int value = stab->value;
			int ref = stab_ref[istab];
			int xsec;
			if(ref < 0 && (xsec = of.sections[~ref].sec_map_index) >= 0) {
				value += section_map[xsec].rva + code_rva + image_base;
//				highlow_fixup_rva.Add(out_index * STAB_STABSIZE + STAB_VALOFF + groups[GRP_STAB].rva + code_rva);
			}
			else if(ref > 0) {
				int c = collected_symbols.Find(ref);
				if(c >= 0) {
					const Symbol& sym = globals[collected_symbols[c]];
					value += sym.value + image_base;
//					highlow_fixup_rva.Add(out_index * STAB_STABSIZE + STAB_VALOFF + groups[GRP_STAB].rva + code_rva);
				}
			}
			int outdx = 0;
			if(stab->strdx) {
				const char *text = stabstrtbl + stab->strdx;
				int len = strlen(text) + 1;
				unsigned hash = memhash(text, len - 1);
				for(int f = output_stab_string_hash.Find(hash); f >= 0; f = output_stab_string_hash.FindNext(f)) {
					int pos = output_stab_string_hash[f];
					if(!memcmp(output_stab_strings.GetIter(pos), text, len)) {
						outdx = pos;
						break;
					}
				}
				if(!outdx) {
					outdx = output_stab_strings.GetCount();
					output_stab_strings.SetCountR(outdx + len);
					memcpy(output_stab_strings.GetIter(outdx), text, len);
					output_stab_string_hash.Add(hash, outdx);
				}
			}

			int ipos = output_stab_info.GetCount();
			output_stab_info.SetCountR(ipos + STAB_STABSIZE);
			byte *outstab = output_stab_info.GetIter(ipos);
			PokeIL(outstab + STAB_STRDXOFF, outdx);
			outstab[STAB_TYPEOFF] = stab->type;
			outstab[STAB_OTHEROFF] = stab->other;
			PokeIW(outstab + STAB_DESCOFF, stab->desc);
			PokeIL(outstab + STAB_VALOFF, value);
			out_index++;
		}
	}
}

void LinkJob::CheckUnresolved()
{
	if(unresolved.IsEmpty())
		return;
	PutStdOut(NFormat("%d unresolved externals:", unresolved.GetCount()));
	Vector<int> unrn = unresolved.PickKeys();
	Sort(unrn, AtomOrder(*this));
	for(int i = 0; i < unrn.GetCount(); i++) {
		int uatom = unrn[i];
//		PutStdOut(NFormat("%s (%d)", atoms[uatom], collected_symbols.Find(uatom)));
		PutStdOut(atoms[uatom]);
#ifdef _DEBUG
		int w = weak_externals.Find(uatom);
		if(w >= 0)
			PutStdOut(NFormat("\t! alias %s", atoms[weak_externals[w]]));
#endif
		int e = ext_obj_sec.Find(uatom);
		if(e >= 0)
			PutStdOut(NFormat("\t? referenced from %s", AsString(objects[ext_obj_sec[e].object])));
		for(int g = globals.Find(uatom); g >= 0; g = globals.FindNext(g)) {
			ObjSec os = globals[g].obj_sec;
			PutStdOut(NFormat("\t= defined in %s", FormatSection(os)));
		}
	}
}

String LinkJob::GetGroupName(int group)
{
	switch(group) {
	case GRP_CODE:    return "CODE";
	case GRP_RDATA:   return "RDATA";
	case GRP_EDATA:   return "EDATA";
	case GRP_IDATA:   return "IDATA";
	case GRP_DATA:    return "DATA";
	case GRP_UDATA:   return "BSS";
	case GRP_DEBUG:   return "DEBUG";
	case GRP_RSRC:    return "RSRC";
	case GRP_STAB:    return "STAB";
	case GRP_STABSTR: return "STABSTR";
	case GRP_SYMBOLS: return "SYMBOLS";
	default: NEVER(); return NFormat("(%d)", group);
	}
}

void LinkJob::WriteMapGlobals(String& map, const Vector<int>& used, String name)
{
	map <<
		"Section Offset   RVA      FPOS     Library      Object       " << name << "\n";

	for(int i = 0; i < used.GetCount(); i++) {
		const Symbol& sym = globals[used[i]];
		const ObjectFile *of = (sym.obj_sec.object >= 0 ? &objects[sym.obj_sec.object] : NULL);
		int atom = globals.GetKey(used[i]);
		String section;
		int value = sym.value;
		int section_rva = 0;
		int section_rfa = 0;
		int delta_rva = 0;
		int delta_rfa = 0;
		if(sym.obj_sec.object < 0)
			section = "LINKER";
		else if(sym.obj_sec.section < 0)
			section = (sym.obj_sec.section == -1 ? "ABS" : "DEBUG");
		else {
//			int secx = GetSection0(sym.obj_sec).sec_map_index;
			int secx = GetSection(sym.obj_sec).sec_map_index;
			if(secx >= 0) {
				const Section& sec = section_map[secx];
				section = atoms[section_name_map.GetKey(sec.app_section)];
				const Section& secbegin = section_map[section_name_map[sec.app_section]];
				section_rva = secbegin.rva + code_rva;
				section_rfa = secbegin.rfa + code_rfa;
	//			delta_rva = code_rva;
	//			delta_rfa = code_rfa + section_rfa;
				if(!sym.used)
					value += section_rva;
			}
			else
				section = "UNUSED";
		}
		map << NFormat("%7<s %08x %08x %08x %12<s %12<s %s\n",
			section, value - section_rva, value, value - section_rva + section_rfa,
			of ? GetFileName(of->library_file) : String(Null),
			of ? GetFileName(of->object_file) : String(Null),
			DemangleAtom(atom));
	}
}

void LinkJob::WriteMapFile()
{
	int time = msecs();

	String map;
	map <<
		"Output file:  " << outputfile << "\n"
		"Timestamp:    " << FormatIntHex(timestamp) << ", " << ctime(&timestamp) <<
		"Load address: " << FormatIntHex(image_base) << "\n"
		"\n"
		"Segment  #   Class RVA       FPOS      Length\n";

	int i;
	for(i = 0; i < section_name_map.GetCount(); i++) {
		const Section& sec = section_map[section_name_map[i]];
		int lastid = (i + 1 >= section_name_map.GetCount() ? section_map.GetCount() : section_name_map[i + 1]) - 1;
		const Section& lastsec = section_map[lastid];
		String secname = atoms[section_name_map.GetKey(i)];
		map << NFormat("%8<s %3<d %5<s %08x  %08x  %08x\n",
			secname, i + 1, GetGroupName(sec.group), code_rva + sec.rva, code_rfa + sec.rfa, lastsec.rva - sec.rva
			+ GetSection(lastsec.obj_sec).size);
	}
	map << NFormat(".reloc %20>08x  %08x  %08x\n", groups[GRP_COUNT].rva, groups[GRP_COUNT].rfa, output_fixups.GetCount())
		<< "\n";

	Vector<int> used;
	for(i = 0; i < globals.GetCount(); i++) {
		bool u = globals[i].used || (dump_flags & DUMP_MAP_ALL);
		if(!u && (dump_flags & DUMP_MAP_UNUSED)) {
			const Symbol& sym = globals[i];
			if(sym.obj_sec.object >= 0 && sym.obj_sec.section >= 0 && GetSection(sym.obj_sec).used) {
				int f = i;
				while((f = globals.FindPrev(f)) >= 0) {
					const Symbol& prev = globals[f];
					if(prev.used || prev.obj_sec.object >= 0 && prev.obj_sec.section >= 0
						&& GetSection(prev.obj_sec).used)
						break;
				}
				if(f < 0)
					u = true;
			}
		}
		if(u)
			used.Add(i);
	}

	Sort(used, GlobalAddressOrder(*this));
	WriteMapGlobals(map, used, "Publics by address");
	map << "\n";
	Vector<String> demangled;
	demangled.SetCount(used.GetCount());
	for(i = 0; i < used.GetCount(); i++)
		demangled[i] = DemangleAtom(globals.GetKey(used[i]));
	IndexSort(demangled, used, GetLanguageInfo());
	WriteMapGlobals(map, used, "Publics by name");

	if(write_xref) {
		map << "\n"
			"Cross-reference map (Target: <- Referer)\n";
		for(int i = 0; i < used.GetCount(); i++) {
			int atom = globals.GetKey(used[i]);
			map << DemangleAtom(atom) << ":\n";
			Vector<String> referer;
			int f = collected_referer.Find(atom);
			if(f >= 0) {
				for(int r = 0; r < collected_referer[f].GetCount(); r++)
					referer.Add(DemangleAtom(collected_referer[f][r]));
				Sort(referer, GetLanguageInfo());
			}
			for(int r = 0; r < referer.GetCount(); r++)
				map << "\t<- " << referer[r] << "\n";
			if(referer.IsEmpty())
				map << "\t(root symbol)\n";
		}
	}

	map << "\n"
		"DLL Imports:\n";
	for(i = 0; i < used_dll_objects.GetCount(); i++) {
		const ObjectFile& of = objects[used_dll_objects[i]];
		map << NFormat("%s (%d imports)\n", of.object_file, of.used_imports.GetCount());
		for(int s = 0; s < of.used_imports.GetCount(); s++) {
			const ObjectFile::Import& imp = of.imports[of.used_imports[s]];
			map << NFormat("%5>d %s\n", imp.ordinal_hint, atoms[imp.imp_atom]);
		}
	}

	if(mapfile_stdout)
		PutStdOut(map);
	else {
		if(IsNull(mapfile)) {
			if(mapfile_stdout)
				return;
			mapfile = ForceExt(outputfile, ".map");
		}
		if(!SaveFile(mapfile, map)) {
			FileDelete(mapfile);
			throw Exc(NFormat("%s: error writing map file", mapfile));
		}

		PutStdOut(NFormat("%s(1): mapfile written in %d msecs", mapfile, msecs(time)));
	}
}

void LinkJob::Link()
{
	if(dump_flags & DUMP_ENVIRONMENT) {
		PutStdOut("Environment: <<");
		const VectorMap<String, String>& env = Environment();
		for(int i = 0; i < env.GetCount(); i++)
			PutStdOut(NFormat("%s=%s", env.GetKey(i), env[i]));
		PutStdOut(">> End of environment");
	}

	if(dump_flags & DUMP_COMMANDLINE) {
		PutStdOut(NFormat("Current directory: %s", GetCurrentDirectory()));
		PutStdOut("Command line: <<");
		for(int i = 0; i < command_args.GetCount(); i++)
			if(!IsNull(command_args[i]))
				PutStdOut(AsCString(command_args[i] + ' '));
		PutStdOut(">> End of command line");
	}

	if(objects.GetCount() <= OBJ_FIRST && IsNull(outputfile))
		return;

	if(!make_dll && !force_reloc)
		image_fixed = true;

	if(!image_base)
		if(!make_dll)
			image_base = 0x400000;
		else if(!auto_dll_base)
			image_base = 0x10000000;
		else
			image_base = 0x10000000 + ((GetHashValue(outputfile) % 0x6000) << 16);

	if(subsystem == COFF_IMAGE_SUBSYSTEM_DEFAULT)
#ifdef PLATFORM_WIN32
		subsystem = COFF_IMAGE_SUBSYSTEM_WINDOWS_CUI;
#else
		subsystem = COFF_IMAGE_SUBSYSTEM_POSIX;
#endif

	debug_info = (debug_info_raw > 0 || debug_info_raw != 0 && linkermode == MODE_GNULD);
	int start = msecs();
	ReadDefaultLibs();
	if(dump_flags & DUMP_TIMING) {
		PutStdOut(NFormat("Default libraries:   %d msecs", msecs(start)));
		PutStdOut(NFormat("Total load time:     %d msecs", msecs(start_time)));
	}
	start = msecs();
	SetEntryPoint();
	SetExportSymbols();
	SetRootSymbols();
	PrepareResourceTree();
	CollectSymbols();
	if(dump_flags & DUMP_TIMING)
		PutStdOut(NFormat("Collecting symbols:  %d msecs", msecs(start)));
	start = msecs();
	CollectImports();
	if(dump_flags & DUMP_TIMING)
		PutStdOut(NFormat("Collecting imports:  %d msecs", msecs(start)));
	start = msecs();
	CollectExports();
	if(dump_flags & DUMP_TIMING)
		PutStdOut(NFormat("Collecting exports:  %d msecs", msecs(start)));
	start = msecs();
	CollectSections();
	if(dump_flags & DUMP_TIMING)
		PutStdOut(NFormat("Collecting sections: %d msecs", msecs(start)));
	start = msecs();
	PrepareImageHeader();
	if(dump_flags & DUMP_TIMING)
		PutStdOut(NFormat("Preparing header:    %d msecs", msecs(start)));
	start = msecs();
	RelocateGlobals();
	RelocateExports();
	if(dump_flags & DUMP_TIMING)
		PutStdOut(NFormat("Relocating globals:  %d msecs", msecs(start)));
	start = msecs();
	WriteImageSections();
	if(dump_flags & DUMP_TIMING)
		PutStdOut(NFormat("Composing sections:  %d msecs", msecs(start)));
	start = msecs();
	PrepareImageInfo();
	if(dump_flags & DUMP_TIMING)
		PutStdOut(NFormat("Composing image:     %d msecs", msecs(start)));
	start = msecs();
	WriteImageFile();
	if(dump_flags & DUMP_TIMING)
		PutStdOut(NFormat("Writing image file:  %d msecs", msecs(start)));
	if(dump_flags & DUMP_TIMING)
		PutStdOut(NFormat("Total link time:     %d msecs", msecs(start_time)));
}

String LinkJob::FormatSection(ObjSec os) const
{
	if(IsNull(os))
		return "(null)";
	return NFormat("%s:%d", AsString(objects[os.object]), os.section);
}
