#include "uar.h"
#pragma hdrstop

#include "lib.h"
#include "version.h"

UARArchiveJob::UARArchiveJob()
{
	dump_flags = 0;
	delete_source_objects = false;
	keep_archive_backups = true;
	nologo = false;
}

String UARArchiveJob::Usage()
{
	return String().Cat()
		<< NFormat("uar COFF librarian, version %d.%d.r%d, release date %`\n%s",
		UAR_VERSION_MAJOR, UAR_VERSION_MINOR, UAR_VERSION_RELEASE, UAR_DATE, UAR_COPYRIGHT) << "\n"
		"\n"
		"Currently supported MS lib-compatible options:\n"
		"\n"
		"-out:<filename> .. name of lib file to produce\n"
		"-list[:<filename>] .. dump library contents\n"
		"-nologo .. suppress startup banner (ignored)\n"
		"-machine:<one-of " << COFFMachineNames() << ">\n"
		"-verbose .. verbose link process report\n"
		"   (handled as a neutral, not MS-specific option)\n"
		"\n"
		"Currently supported GNU ar-compatible options:\n"
		"\n"
		"-c .. don't complain when creating new archive\n"
		"      (automatic, warnings shown in verbose mode only)\n"
		"-d .. remove files from archive\n"
		"-m .. move files to archive (delete original files)\n"
		"-p .. print archive contents\n"
		"-q .. quick append (same as -r in uar)\n"
		"-r .. replace & insert files in archive\n"
		"-s .. create archive index (ignored, automatic)\n"
		"-t .. type archive contents\n"
		"-x .. extract archive members\n"
		"\n"
		"uar-specific options:\n"
		"\n"
		"-dumpsym .. display archive symbol table\n"
		"-demangle .. display demangled symbol table\n"
		;
}

void UARArchiveJob::ReadCommand(const char *cmd)
{
	ReadCommand(SplitCmdArgs(cmd));
}

void UARArchiveJob::ReadCommand(const Vector<String>& cmdline)
{
	for(int i = 0; i < cmdline.GetCount(); i++)
	{
		String arg = cmdline[i];
		if(*arg == '-' || *arg == '/')
		{
			const char *ptr = ~arg + 1;
			const char *end = ptr;
			while(*end && *end != ':')
				end++;
			String cmd(ptr, end);
			String lcmd = ToLower(cmd);
			String val;
			if(*end == ':')
				val = end + 1;
			bool eaten = true;
			if(lcmd == "out" && !IsNull(val))
				libraryfile = val;
			else if(lcmd == "list")
			{
				command = CMD_PRINT;
				printfile = val;
			}
			else if(lcmd == "nologo")
				nologo = true;
			else if(lcmd == "machine" && !IsNull(val))
			{
				val = ToUpper(val);
				int m = COFFMachineCode(val);
				if(m == COFF_IMAGE_FILE_MACHINE_UNKNOWN)
					PutStdOut(NFormat("Unknown machine type: %s; supported machines are: %s", val, COFFMachineNames()));
				machine = m;
			}
			else
				eaten = false;
			if(eaten)
				armode = MODE_MSLIB;
			else
			{
//				if(lcmd == "dumpobj")
//					dump_flags |= DUMP_OBJECTS;
				if(lcmd == "dumpsym")
					dump_flags |= DUMP_SYMBOLS;
				else if(lcmd == "demangle")
					dump_flags |= DUMP_SYMBOLS | DUMP_SYMBOLS_DEMANGLED;
				else if(lcmd == "verbose")
					verbose = true;
				else
				{
					for(const char *p = ~arg + 1; *p;)
						switch(*p++)
						{
						case 'c': // don't complain when creating archive
							if(verbose)
								PutStdOut("option '-c' ignored (automatic)");
							break;

						case 'd': // delete files
							command = CMD_REMOVE;
							break;

						case 'm': // move files
							command = CMD_ADD;
							delete_source_objects = true;
							break;

						case 'p': // print files
							command = CMD_PRINT;
							break;

						case 'q': // quick append
							command = CMD_ADD;
							break;

						case 'r': // replace & insert
							command = CMD_ADD;
							break;

						case 's': // create archive index
							if(verbose)
								PutStdOut("option '-s' ignored (automatic)");
							break;

						case 't': // type contents
							command = CMD_CONTENTS;
							break;

						case 'x': // extract
							command = CMD_EXTRACT;
							throw Exc("option '-x' not supported (yet)");

						default: throw Exc(NFormat("Invalid command line option: '%s'", arg));
						}

					armode = MODE_GNUAR;
				}
			}
		}
		else if(IsNull(libraryfile))
			libraryfile = AppendExt(arg, ".a");
		else
		{
			String fn = AppendExt(arg, armode == MODE_GNUAR ? ".o" : ".obj");
			if(command == CMD_ADD)
				LoadFile(fn);
			else
				object_names.Add(fn);
		}
	}
}

void UARArchiveJob::Build()
{
	if(IsNull(libraryfile))
		throw Exc("Archive file not specified.");

	archive_must_exist = (command != CMD_ADD && use_existing_archive);
	if(use_existing_archive)
		LoadLibrary(libraryfile);

	SortGlobals();

	if(dump_flags & DUMP_SYMBOLS)
	{
		PutStdOut(NFormat("%d symbol(s) in %d object file(s)", symbol_names.GetCount(), objects.GetCount()));
		PutStdOut("Object          Symbol");
		for(int i = 0; i < symbol_names.GetCount(); i++)
		{
			String symname = symbol_names[i];
			if(dump_flags & DUMP_SYMBOLS_DEMANGLED)
				symname = DemangleName(symname, armode == MODE_MSLIB ? MANGLING_MSC : MANGLING_GCC);
			PutStdOut(NFormat("%15<s %s", objects[symbol_objects[i]].trimmed_name, symname));
		}
	}

	if(command == CMD_CONTENTS || command == CMD_PRINT)
	{
		Print();
		return;
	}

	if(command == CMD_EXTRACT)
	{
		Extract();
		return;
	}

	if(command == CMD_REMOVE)
		Remove();

	if(objects.IsEmpty())
		PutStdOut(NFormat("%s: empty archive", libraryfile));

	BuildLibrary(libraryfile, keep_archive_backups);
	if(delete_source_objects) {
		if(verbose)
			PutStdOut("Deleting source objects:");
		for(int i = 0; i < objects.GetCount(); i++)
			if(objects[i].newfile) {
				String fn = objects[i].filename;
				if(verbose)
					PutStdOut(NFormat("%d: deleting object file", fn));
				if(!FileDelete(fn))
					PutStdOut(NFormat("%s: error deleting file", fn));
			}
	}
}

void UARArchiveJob::Remove()
{
	for(int i = 0; i < object_names.GetCount(); i++)
	{
		String n = TrimObjectName(object_names[i]);
		int t = FindFieldIndex(objects, &Object::trimmed_name, n);
		if(t >= 0)
			objects.Remove(t);
		else
			PutStdOut(NFormat("%s: not found in archive (trimmed name = %s)", object_names[i], n));
	}
}

void UARArchiveJob::Extract()
{
	for(int i = 0; i < object_names.GetCount(); i++)
	{
		String fn = object_names[i], n = TrimObjectName(fn);
		int t = FindFieldIndex(objects, &Object::trimmed_name, n);
		if(t >= 0)
		{
			if(!SaveFile(fn, objects[t].object_data))
				throw Exc(NFormat("%s: error writing extracted file (trimmed name = %s)", fn, n));
			if(verbose)
				PutStdOut(NFormat("%s: extracted member %s (%d B)", fn, n, objects[t].object_data.GetLength()));
		}
		else
			PutStdOut(NFormat("%s: not found in archive (trimmed name = %s)", fn, n));
	}
}

void UARArchiveJob::Print()
{
	PutStdOut("Offset  Size   Time     Object");
	for(int i = 0; i < objects.GetCount(); i++)
	{
		const Object& o = objects[i];
		PutStdOut(NFormat("%10>d %10>d %12>0n %s", o.header_offset, o.object_data.GetLength(), o.filetime, o.filename));
		if(command == CMD_PRINT)
			for(int s = 0; s < symbol_names.GetCount(); s++)
				if(symbol_objects[s] == i)
					PutStdOut("\t" + symbol_names[s]);
	}
}
