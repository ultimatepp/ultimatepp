#include "clang.h"

#define LTIMING(x)   //TIMING(x)
#define LTIMESTOP(x) //TIMESTOP(x)
#define LLOG(x)      //DLOG(x)
#define LDUMP(x)     //DDUMP(x)
#define LDUMPM(x)    //DDUMPM(x)

struct Timest { // TODO remove
	String name;
	int tm;
	Timest(const String& name) : name(name) { tm = msecs(); }
	~Timest() {
		tm = msecs() - tm;
		if(tm > 500)
			DLOG(name << " " << tm / 1000.0);
	}
};

#define ITIMESTOP(x)  Timest COMBINE(sTmStop, __LINE__)(x);


String FindMasterSource(Hdepend& hdepend, const Workspace& wspc, const String& header_file)
{
	String master_source;
	for(int pass = 0; pass < 2; pass++) { // all packages in second pass
		for(int i = 0; i < wspc.GetCount(); i++) { // find package of included file
			const Package& pk = wspc.GetPackage(i);
			String pk_name = wspc[i];

			auto Chk = [&] {
				for(int i = 0; i < pk.file.GetCount(); i++) {
					String path = SourcePath(pk_name, pk.file[i]);
					if(!PathIsEqual(header_file, path) && IsSourceFile(path)) {
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
	  % pos
	  % definition
	  % name
	  % id
	  % pretty
	  % nspace
	  % uname
	  % nest
	  % unest
	;
}

void ReferenceItem::Serialize(Stream& s)
{
	s % id
	  % pos;
}

void FileAnnotation::Serialize(Stream& s)
{
	s % defines
	  % includes
	  % time
	  % items
	  % refs;
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
	ITIMESTOP("DumpIndex");
	GuiLock __;
	ITIMESTOP("DumpIndex2");
	FileOut out(ConfigFile("current_index.dump"));
	ArrayMap<String, FileAnnotation>& x = CodeIndex();
	for(const auto& m : ~x) {
		out << m.key << "\n";
		for(const auto& n : m.value.items)
			out << '\t' << n.name << "   " << n.id << "   " << n.pretty << "\n";
		for(const auto& n : m.value.refs)
			out << '\t' << n.pos << "   " << n.id << "\n";
	}
}

CoEvent              Indexer::event;
CoEvent              Indexer::scheduler;
Mutex                Indexer::mutex;
Vector<Indexer::Job> Indexer::jobs;
int                  Indexer::jobi;
std::atomic<int>     Indexer::running_indexers;
VectorMap<String, String> Indexer::master_file;
String               Indexer::main;
String               Indexer::includes;
String               Indexer::defines;

void Indexer::IndexerThread()
{
	while(!Thread::IsShutdownThreads()) {
		Clang clang;
		int tm0 = msecs();
		bool was_job = false; // for diagnostics
		++running_indexers;
		while(!Thread::IsShutdownThreads()) {
			Job job;
			{
				ITIMESTOP("Acquire job");
				Mutex::Lock __(mutex);
				if(jobi < jobs.GetCount())
					job = jobs[jobi++];
				else
					break;
				was_job = true;
			}

			if(Thread::IsShutdownThreads())
				break;
			
		//	if(job.path != "C:\\upp\\ide$$$blitz.cpp")
		//		continue;
			
			{
				ITIMESTOP("Parsing " + job.path + " " + AsString(job.file_times));
				clang.Parse(job.path, job.blitz, job.includes, job.defines,
				            CXTranslationUnit_DetailedPreprocessingRecord|
				            CXTranslationUnit_KeepGoing|
			//	            CXTranslationUnit_SkipFunctionBodies|
				            (job.blitz.GetCount() ? 0 : PARSE_FILE));
			//	DumpDiagnostics(clang.tu);
			}
			
			if(Thread::IsShutdownThreads())
				break;
	
			ClangVisitor v;
			if(clang.tu) {
			//	DumpDiagnostics(clang.tu);
	
				String current_file;
				bool   do_file = false;
				VectorMap<String, bool> do_file_cache;
	
				v.WhenFile = [&](const String& path) {
					DTIMING("WhenFile");
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
								DTIMING("WhenFile 2");
								Mutex::Lock __(mutex);
								do_file = job.file_times.Find(master_file.Get(NormalizePath(path), Null)) >= 0;
								do_file_cache.Add(path, do_file);
							}
							else
								do_file = do_file_cache[q];
						}
					}
					return do_file;
				};

				ITIMESTOP("Visitor " + job.path + " " + AsString(job.file_times));
				v.Do(clang.tu);
			}
	
			if(Thread::IsShutdownThreads())
				break;
			
			for(const auto& m : ~job.file_times) // in create entries even if there are no items to avoid recompiling
				v.info.GetAdd(NormalizePath(m.key));
			
			for(const auto& m : ~v.info) {
				String path = NormalizePath(m.key);
				FileAnnotation f;
				f.defines = job.defines;
				f.includes = job.includes;
				(CppFileInfo&)f = pick(m.value);
				f.time = job.file_times.Get(path, Time::Low());
				ITIMESTOP("Save");
				SaveChangedFile(CachedAnnotationPath(path, f.defines, f.includes, master_file.Get(path, Null)), StoreAsString(f), true);
				ITIMESTOP("Set");
				GuiLock __;
				CodeIndex().GetAdd(path) = pick(f);
			}
			
		}
		bool last = false;
		{
			Mutex::Lock __(mutex);
			if(--running_indexers == 0 && jobs.GetCount()) {
				DLOG("Done everything " << (msecs() - tm0) / 1000.0 << " s");
				jobs.Clear();
				scheduler.Broadcast();
				last = true;
			}
		}
	#ifdef _DEBUG
		if(last)
			DumpIndex(); // TODO remove
	#endif
		if(Thread::IsShutdownThreads())
			break;
		event.Wait();
		LLOG("Indexers Thread::IsShutdownThreads() " << Thread::IsShutdownThreads());
	}
	LLOG("Exiting IndexerThread");
}

void Indexer::Start(const String& main, const String& includes, const String& defines)
{
	ONCELOCK {
		MemoryIgnoreNonMainLeaks();
		MemoryIgnoreNonUppThreadsLeaks(); // clangs leaks static memory in threads
		Thread::AtShutdown([] {
			LLOG("Shutdown indexers");
			event.Broadcast();
			scheduler.Broadcast();
		});
		for(int i = 0; i < CPU_Cores(); i++) // TODO: CPU_Cores?
			Thread::StartNice([] { Indexer::IndexerThread(); });
		Thread::StartNice([] { SchedulerThread(); });
	}
	
	GuiLock __;
	Indexer::main = main;
	Indexer::includes = includes;
	Indexer::defines = defines;
	if(jobs.GetCount() == 0)
		scheduler.Broadcast();
}

void Indexer::SchedulerThread()
{
	Hdepend hdepend;
	while(!Thread::IsShutdownThreads()) {
		scheduler.Wait();

		Mutex::Lock __(mutex);
		String includes, defines;
		
		VectorMap<String, Vector<Tuple<String, bool>>> sources;
		{
			GuiLock __;
			ITIMESTOP("Load workspace");
			Workspace wspc;
			wspc.Scan(main);

			for(int pi = 0; pi < wspc.GetCount(); pi++) {
				String pk_name = wspc[pi];
				Vector<Tuple<String, bool>>& ps = sources.GetAdd(pk_name);
				const Package& pk = wspc.GetPackage(pi);
				for(int i = 0; i < pk.GetCount(); i++) {
					String path = SourcePath(pk_name, pk[i]);
					if(IsCSourceFile(path))
						ps.Add({ NormalizePath(path), pk[i].noblitz });
				}
			}
			includes = Merge(";", Indexer::includes, GetClangInternalIncludes());
			defines = Indexer::defines;
		}
		
		{
			ITIMESTOP("Create indexer jobs");
			Mutex::Lock __(mutex);
			ITIMESTOP("Create indexer jobs after lock");
			hdepend.NoConsole();
			LDUMP(includes);
			hdepend.SetDirs(includes);
			hdepend.TimeDirty();
			jobs.Clear();

			{
				ITIMESTOP("Master files"); // <<<< this is slow
				master_file.Clear();
				for(int i = sources.GetCount() - 1; i >= 0; i--) {
					const auto& pk = sources[i];
					for(const auto& f : pk) {
						master_file.Add(f.a, f.a);
						for(String p : hdepend.GetDependencies(f.a)) {
							p = NormalizePath(p);
							if(master_file.Find(p) < 0)
								master_file.Add(p, f.a);
						}
					}
				}
			}
			
			DUMPM(master_file);
			
			{
				ITIMESTOP("Loading from cache");
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
			}
			
			{ // remove files that are not in project anymore
				ITIMESTOP("Removing files");
				GuiLock __;
				for(int i = 0; i < CodeIndex().GetCount(); i++)
					if(master_file.Find(CodeIndex().GetKey(i)) < 0)
						CodeIndex().Unlink(i);
				CodeIndex().Sweep();
			}
	
			ITIMESTOP("Create indexer jobs2");
			jobi = 0;
			for(const auto& pkg : ~sources) {
				String blitz;
				VectorMap<String, Time> blitz_files;
				auto AddJob = [&](const String& path) -> Job& {
					Job& job = jobs.Add();
					job.includes = includes;
					job.defines = defines;
					job.path = path;
					return job;
				};
				int i = 0;
				for(const auto& pf : pkg.value) {
					Time time = hdepend.FileTime(pf.a);
					FileAnnotation0 f;
					{
						GuiLock __;
						f = CodeIndex().GetAdd(pf.a);
					}
					if(f.defines != defines || f.includes != includes || f.time != time) {
						if(hdepend.BlitzApproved(pf.a) && !pf.b) {
							BlitzFile(blitz, pf.a, hdepend, i++);
							blitz_files.Add(pf.a, time);
						}
						else
							AddJob(pf.a).file_times.Add(pf.a, time);
					}
				}
				if(blitz.GetCount()) {
					Job& job = AddJob(ConfigFile(pkg.key + "$$$blitz.cpp")); // the path is fake, file does not exist
					job.blitz = blitz;
					job.file_times = pick(blitz_files);
				}
			}
		}
		if(jobs.GetCount()) {
			DLOG("======= Unleash indexers");
			event.Broadcast();
		}
	}
}
