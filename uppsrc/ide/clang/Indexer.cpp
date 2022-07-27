#include "clang.h"

#define LTIMING(x)
#define LTIMESTOP(x) TIMESTOP(x)
#define LLOG(x) DLOG(x)

String FindMasterSource(Hdepend& hdepend, const Workspace& wspc, const String& header_file)
{
	DDUMP(header_file);
	String master_source;
	for(int pass = 0; pass < 2; pass++) { // all packages in second pass
		for(int i = 0; i < wspc.GetCount(); i++) { // find package of included file
			const Package& pk = wspc.GetPackage(i);
			String pk_name = wspc[i];

			auto Chk = [&] {
				for(int i = 0; i < pk.file.GetCount(); i++) {
					String path = SourcePath(pk_name, pk.file[i]);
					if(!PathIsEqual(header_file, path) && IsSourceFile(path)) {
						DDUMP(hdepend.GetDependencies(path));
						if(FindIndex(hdepend.GetDependencies(path), header_file) >= 0 && GetFileLength(path) < 200000) {
							master_source = path;
							return true;
						}
					}
				}
				return false;
			};

			if(pass) {
				if(Chk())
					return master_source;
			}
			else
			for(int i = 0; i < pk.file.GetCount(); i++) {
				if(PathIsEqual(header_file, SourcePath(pk_name, pk.file[i]))) {
					if(Chk())
						return master_source;
					break;
				}
			}
		}
	}
	return master_source;
}

void AnnotationItem::Serialize(Stream& s)
{
	s % kind
	  % line
	  % definition
	  % name
	  % id
	  % pretty
	  % nspace
	  % uname
	  % nest
	  % unest;
}

void FileAnnotation::Serialize(Stream& s)
{
	s % defines
	  % includes
	  % time
	  % items;
}

String CachedAnnotationPath(const String& source_file, const String& defines, const String& includes, const String& master)
{
	// TODO: master file?
	Sha1Stream s;
	s << source_file
	  << defines
	  << includes
	  << master
	;
	return CacheFile(GetFileTitle(source_file) + "$" + s.FinishString() + ".code_index");
}

void BlitzFile(String& blitz, const String& sourceFile, Hdepend& hdepend, int index)
{
	blitz << "\r\n"
	      << "#define BLITZ_INDEX__ F" << index << "\r\n"
	      << "#include \"" << sourceFile << "\"\r\n";
	const Vector<String>& d = hdepend.GetDefines(sourceFile);
	for(int i = 0; i < d.GetCount(); i++)
		blitz << "#ifdef " << d[i] << "\r\n"
		      << "#undef " << d[i] << "\r\n"
		      << "#endif\r\n";
	blitz << "#undef BLITZ_INDEX__\r\n";
}

ArrayMap<String, FileAnnotation>& CodeIndex()
{
	static ArrayMap<String, FileAnnotation> m;
	return m;
}

void DumpIndex()
{
	GuiLock __;
	FileOut out(ConfigFile("current_index.dump"));
	ArrayMap<String, FileAnnotation>& x = CodeIndex();
	for(const auto& m : ~x) {
		out << m.key << "\n";
		for(const auto& n : m.value.items)
			out << '\t' << n.name << "   " << n.id << "   " << n.pretty << "\n";
	}
}

CoEvent              Indexer::event;
Hdepend              Indexer::hdepend;
Mutex                Indexer::mutex;
Vector<Indexer::Job> Indexer::jobs;
int                  Indexer::jobi;
std::atomic<int>     Indexer::running_indexers;
Workspace            Indexer::wspc;
VectorMap<String, String> Indexer::master_file;

void Indexer::IndexerThread()
{
	while(!Thread::IsShutdownThreads()) {
		Clang clang;
		int tm0 = msecs();
		running_indexers++;
		bool was_job = false; // for diagnostics
		for(;;) {
			Job job;
			{
				Mutex::Lock __(mutex);
				if(jobi >= jobs.GetCount())
					break;
				job = jobs[jobi++];
				was_job = true;
			}
			
			{
				LTIMESTOP("Parsing " + job.path + " " + AsString(job.file_times));
				clang.Parse(job.path, job.blitz, job.includes, job.defines,
				            CXTranslationUnit_DetailedPreprocessingRecord|
				            CXTranslationUnit_KeepGoing|
				            CXTranslationUnit_SkipFunctionBodies|
				            (job.blitz.GetCount() ? 0 : PARSE_FILE));
				DumpDiagnostics(clang.tu);
			}
			
			if(Thread::IsShutdownThreads())
				break;
	
			bool ide_h = false;

			ClangVisitor v;
			if(clang.tu) {
				DumpDiagnostics(clang.tu);
	
				String current_file;
				bool   do_file = false;
				VectorMap<String, bool> do_file_cache;
	
				v.WhenFile = [&](const String& path) {
					if(IsNull(path))
						return false;
					if(current_file != path) {
						current_file = path;
						if(IsNull(path) || path.EndsWith("$$$blitz.cpp"))
							do_file = false;
						else
						if(IsCSourceFile(path))
							do_file = true;
						else {
							current_file = path;
							int q = do_file_cache.Find(path);
							if(q < 0) {
								Mutex::Lock __(mutex);
								do_file = job.file_times.Find(master_file.Get(NormalizePath(path), Null)) >= 0;
								do_file_cache.Add(path, do_file);
								if(path.EndsWith("istream")) { _DBG_
									DDUMP(path);
									DDUMP(job.file_times);
									DDUMP(master_file.Get(NormalizePath(path), Null));
									DDUMP(job.file_times.GetKeys());
									DDUMP(do_file);
									ide_h = do_file;
								}
							}
							else
								do_file = do_file_cache[q];
						}
					}
					return do_file;
				};
	
				v.Do(clang.tu);
			}
	
			if(ide_h) {
				DDUMPM(job.file_times);
			}
	
			for(const auto& m : ~job.file_times) // in create entries even if there are no items to avoid recompiling
				v.item.GetAdd(NormalizePath(m.key));
			
			for(const auto& m : ~v.item) {
				String path = NormalizePath(m.key);
				FileAnnotation f;
				f.defines = job.defines;
				f.includes = job.includes;
				f.items = pick(m.value);
				f.time = job.file_times.Get(path, Time::Low());
				SaveChangedFile(CachedAnnotationPath(path, f.defines, f.includes, master_file.Get(path, Null)), StoreAsString(f), true);
				GuiLock __;
				CodeIndex().GetAdd(path) = pick(f);
			}
		}
	#ifdef _DEBUG
		if(--running_indexers == 0 && was_job) {
			LLOG("Done everything " << (msecs() - tm0) / 1000.0 << " s");
			DumpIndex(); // TODO remove
		}
	#endif
		event.Wait();
		DLOG("Indexers Thread::IsShutdownThreads() " << Thread::IsShutdownThreads());
	}
	LLOG("Done");
}

void Indexer::Start(const String& main, const String& includes_, const String& defines)
{
	DLOG("Indexer::Start =============================== ");

	ONCELOCK {
		MemoryIgnoreNonMainLeaks();
		MemoryIgnoreNonUppThreadsLeaks(); // clangs leaks static memory in threads
		Thread::AtShutdown([] {
			DLOG("Shutdown indexers");
			event.Broadcast();
		});
		for(int i = 0; i < CPU_Cores(); i++) // TODO: CPU_Cores?
			Thread::StartNice([] { Indexer::IndexerThread(); });
	}
	
	Thread::Start([=] {
		String includes = Merge(";", includes_, GetClangInternalIncludes());

		{
			GuiLock __;
			DTIMING("Load workspace");
			wspc.Scan(main);
		}
		
		{
			DTIMING("Create indexer jobs");
			Mutex::Lock __(mutex);
			hdepend.NoConsole();
			DDUMP(includes);
			hdepend.SetDirs(includes);
			hdepend.TimeDirty();
			jobs.Clear();
	
			master_file.Clear();
			for(int pi = 0; pi < wspc.GetCount(); pi++) {
				String pk_name = wspc[pi];
				const Package& pk = wspc.GetPackage(pi);
				for(int i = 0; i < pk.GetCount(); i++) {
					String path = NormalizePath(SourcePath(pk_name, pk[i]));
					if(IsCSourceFile(path)) {
						master_file.Add(path, path);
						DDUMP(hdepend.GetDependencies(path));
						for(String p : hdepend.GetDependencies(path)) {
							p = NormalizePath(p);
							if(master_file.Find(p) < 0)
								master_file.Add(p, path);
						}
					}
				}
			}
			
			DDUMPM(master_file);
			
			for(String path : master_file.GetKeys()) {
				FileAnnotation0 f;
				{
					GuiLock __;
					f = CodeIndex().GetAdd(path);
				}
				if(f.includes != includes || f.defines != defines) {
					String h = LoadFile(CachedAnnotationPath(path, defines, includes, master_file.Get(path, Null)));
					if(h.GetCount()) {
						FileAnnotation m;
						if(LoadFromString(m, h)) {
							m.time = m.time;
							GuiLock __;
							CodeIndex().GetAdd(path) = pick(m);
						}
					}
				}
			}
			
			{ // remove files that are not in project anymore
				GuiLock __;
				for(int i = 0; i < CodeIndex().GetCount(); i++)
					if(master_file.Find(CodeIndex().GetKey(i)) < 0)
						CodeIndex().Unlink(i);
				CodeIndex().Sweep();
			}
	
			jobi = 0;
			for(int pi = 0; pi < wspc.GetCount(); pi++) {
				String blitz;
				VectorMap<String, Time> blitz_files;
				String pk_name = wspc[pi];
				const Package& pk = wspc.GetPackage(pi);
				auto AddJob = [&](const String& path) -> Job& {
					Job& job = jobs.Add();
					job.includes = includes;
					job.defines = defines;
					job.path = path;
					return job;
				};
				for(int i = 0; i < pk.GetCount(); i++) {
					String path = NormalizePath(SourcePath(pk_name, pk[i]));
					if(IsCSourceFile(path)) {
						Time time = hdepend.FileTime(path);
						FileAnnotation0 f;
						{
							GuiLock __;
							f = CodeIndex().GetAdd(path);
						}
						DLOG("===========");
						DDUMP(path);
						DDUMP(time);
						DDUMP(f.time);
						if(f.defines != defines || f.includes != includes || f.time != time) {
							if(hdepend.BlitzApproved(path) && !pk[i].noblitz) {
								BlitzFile(blitz, path, hdepend, i);
								blitz_files.Add(path, time);
							}
							else
								AddJob(path).file_times.Add(path, time);
						}
					}
				}
				if(blitz.GetCount()) {
					Job& job = AddJob(ConfigFile(pk_name + "$$$blitz.cpp"));
					job.blitz = blitz;
					job.file_times = pick(blitz_files); // the path is fake, file does not exist
				}
			}
		}
		
		event.Broadcast();
	});
}
