#include <ide/ide.h>

#define LTIMING(x)   //RTIMING(x)
#define LTIMESTOP(x) //RTIMESTOP(x)
#define LLOG(x)      //DLOG(x)
#define LDUMP(x)     //DDUMP(x)
#define LDUMPM(x)    //DDUMPM(x)

String FindMasterSource(PPInfo& ppi, const Workspace& wspc, const String& header_file_)
{
	LTIMING("FindMasterSource");
	String header_file = NormalizePath(header_file_);

	for(int speculative = 0; speculative < 2; speculative++) {
		VectorMap<String, Time> deps;
		ArrayMap<String, Index<String>> dics;
		for(int i : wspc.use_order) {
			const Package& pk = wspc.GetPackage(i);
			String pk_name = wspc[i];
			for(int i = 0; i < pk.file.GetCount(); i++) {
				String path = SourcePath(pk_name, pk.file[i]);
				if(!pk.file[i].separator && ppi.FileExists(path) && !PathIsEqual(header_file, path) &&
				   IsCppSourceFile(path) && GetFileLength(path) < 4000000) {
					ppi.GatherDependencies(path, deps, dics, speculative);
					if(deps.Find(header_file) >= 0)
						return path;
				}
			}
		}
	}
	return Null;
}

bool MasterSourceCacheRecord::CheckTimes(PPInfo& ppi) const
{
	for(int i = 1; i < chain.GetCount(); i++)
		if(ppi.GetFileTime(chain.GetKey(i)) != chain[i])
			return false;
	return true;
}

void MasterSourceCacheRecord::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	s % master % chain;
}

const VectorMap<String, Time>& FindMasterSourceCached(PPInfo& ppi, const Workspace& wspc, const String& header_file_,
                                                      VectorMap<String, MasterSourceCacheRecord>& cache)
{
	String header_file = NormalizePath(header_file_);
	
	if(cache.GetCount() > 2000) // 2000 cached headers is enough for everybody, right?
		cache.Clear();

	int q = cache.Find(header_file);
	if(q >= 0 && cache[q].CheckTimes(ppi))
		return cache[q].chain;

	String master = FindMasterSource(ppi, wspc, header_file);
	if(master.GetCount()) {
		for(int speculative = 0; speculative < 2; speculative++) {
			Vector<String> chain;
			bool found = false;
			VectorMap<String, Time> deps;
			ArrayMap<String, Index<String>> dics;
			Vector<Tuple<String, String, int>> flags;
			ppi.GatherDependencies(master, deps, dics, flags, speculative, header_file, chain, found);
			
			MasterSourceCacheRecord& m = cache.GetAdd(header_file);
			m.chain.Clear();
			m.master = master;
			for(const String& f : chain)
				m.chain.Add(f, ppi.GetFileTime(f));
			
			if(found)
				return m.chain;
		}
	}

	static VectorMap<String, Time> empty;
	return empty;
}

void AnnotationItem::Serialize(Stream& s)
{
	s % kind
	  % pos
	  % definition
	  % isvirtual
	  % isstatic
	  % name
	  % type
	  % id
	  % pretty
	  % nspace
	  % uname
	  % nest
	  % unest
	  % bases
	;
}

void ReferenceItem::Serialize(Stream& s)
{
	s % id
	  % pos
	  % ref_pos
	;
}

void FileAnnotation::Serialize(Stream& s)
{
	s % defines
	  % includes
	  % master_file
	  % time
	  % items
	  % refs
	;
}

String CachedAnnotationPath(const String& source_file, const String& defines, const String& includes, const String& master_file)
{
	Sha1Stream s;
	s << source_file
	  << defines
	  << includes
	  << master_file
#ifdef _DEBUG
	  << "debug" // to have different codebase for development
#endif
	;
	return CacheFile(GetFileTitle(source_file) + "$" + s.FinishString() + ".code_index");
}

ArrayMap<String, FileAnnotation>& CodeIndex()
{
	static ArrayMap<String, FileAnnotation> m;
	return m;
}

void DumpIndex(const char *file, const String& what_file)
{
	GuiLock __;
	FileOut out(file);
	out << GetSysTime() << "\n";
	ArrayMap<String, FileAnnotation>& x = CodeIndex();
	for(const auto& m : ~x)
		if(IsNull(what_file) || m.key == what_file) {
			out << m.key << "\n";
			out << "\t=== Globals:\n";
			for(const auto& n : m.value.items)
				out << '\t' << n.pos.y << ": " << n.id << " -> " << n.pretty << ", bases: " << n.bases << "\n";
			out << "\t=== References:\n";
			for(const auto& n : m.value.refs)
				out << '\t' << n.pos << "   " << n.id << " -> " << n.ref_pos << "\n";
		}
}

CoEvent              Indexer::event;
CoEvent              Indexer::scheduler;
Mutex                Indexer::mutex;
Vector<Indexer::Job> Indexer::jobs;
std::atomic<int>     Indexer::jobi;
std::atomic<int>     Indexer::jobs_done;
std::atomic<int>     Indexer::jobs_count;
bool                 Indexer::running_scheduler;
std::atomic<int>     Indexer::running_indexers;
String               Indexer::main;
String               Indexer::includes;
String               Indexer::defines;
bool                 Indexer::relaxed;

void Indexer::BuildingPause()
{
	while(TheIde() && TheIde()->idestate == Ide::BUILDING)
		Sleep(200);
}

void Indexer::IndexerThread()
{
//	Thread::DumpDiagnostics();
	while(!Thread::IsShutdownThreads()) {
		Clang clang;
		clang_CXIndex_setGlobalOptions(clang.index, CXGlobalOpt_ThreadBackgroundPriorityForIndexing);
		int tm0 = msecs();
		++running_indexers;
		while(!Thread::IsShutdownThreads()) {
			Job job;
			{
				LTIMESTOP("Acquire job");
				Mutex::Lock __(mutex);
				if(jobi < jobs.GetCount())
					job = jobs[jobi++];
				else
					break;
			}
			
			if(Thread::IsShutdownThreads())
				break;

			BuildingPause();

			int tm = msecs();

			clang.Parse(job.path, job.blitz, job.includes, job.defines,
			            CXTranslationUnit_KeepGoing|
		                CXTranslationUnit_DetailedPreprocessingRecord|
			            (job.blitz.GetCount() ? 0 : PARSE_FILE));

			if(Thread::IsShutdownThreads())
				break;

			ClangVisitor v;

			if(clang.tu) {
				v.WhenFile = [&](const String& path) {
				#if 0
					if(path.Find("fileapi.h") >= 0) {
						INTERLOCKED {
							DLOG("===============");
							DDUMP(path);
							DDUMP(job.path);
							DDUMP(job.file_times.Find(NormalizePath(path)));
							DDUMPM(job.file_times);
						}
					}
				#endif
					LTIMING("WhenFile");
					if(IsNull(path) || path.EndsWith("$$$blitz.cpp"))
						return false;
					if(IsCSourceFile(path))
						return true;
					return job.file_times.Find(NormalizePath(path)) >= 0;
				};

				LTIMESTOP("Visitor " + job.path + " " + AsString(job.file_times));
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
				f.master_file = job.master_files.Get(path, Null);
				LLOG("Storing " << path);
				SaveChangedFile(CachedAnnotationPath(path, f.defines, f.includes, f.master_file), StoreAsString(f), true);
				GuiLock __;
				CodeIndex().GetAdd(path) = pick(f);
			}

			PutAssist(String() << job.path << " indexed in " << msecs() - tm << " ms");
			Mutex::Lock __(mutex);
			jobs_done++;
		}
		bool last = false;
		{
			Mutex::Lock __(mutex);
			if(--running_indexers == 0 && jobs.GetCount()) {
				jobs.Clear();
				jobs_count = 0;
				scheduler.Broadcast();
				last = true;
			}
		}
		if(last)
			PutAssist(String() << "Indexing finished in " << (msecs() - tm0) / 1000.0 << " s");
		if(Thread::IsShutdownThreads())
			break;
		event.Wait();
		LLOG("Indexers Thread::IsShutdownThreads() " << Thread::IsShutdownThreads());
	}
	LLOG("Exiting IndexerThread");
}

void Indexer::Start(const String& main, const String& includes, const String& defines)
{
	if(!HasLibClang())
		return;

	ONCELOCK {
		MemoryIgnoreNonMainLeaks();
		MemoryIgnoreNonUppThreadsLeaks(); // clangs leaks static memory in threads
		Thread::AtShutdown([] {
			LLOG("Shutdown indexers");
			event.Broadcast();
			scheduler.Broadcast();
		});
		for(int i = 0; i < IndexerThreads; i++) {
			Thread t;
			t.StackSize(8192*1024);
			t.RunNice([] { Indexer::IndexerThread(); });
			t.Detach();
		}
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
	PPInfo ppi;
	while(!Thread::IsShutdownThreads()) {
		scheduler.Wait();

		{
			LTIMESTOP("Scheduler");
			BuildingPause();

			Mutex::Lock __(mutex);
			running_scheduler = true;

			String includes, defines;

			VectorMap<String, Time>   files; // all files of project, including external headers

			Index<String>             workspace_headers;

			Index<String>             master; // bidirectional map, including files
			Index<String>             header; // included files

			VectorMap<String, Vector<Tuple<String, bool>>> sources; // bool is "noblitz"
			{
				GuiLock __;

				defines = Indexer::defines;
				includes = Merge(";", Indexer::includes, GetClangInternalIncludes());

				ppi.SetIncludes(includes);
				ppi.Dirty();

				{
					LTIMESTOP("Load workspace");
					Workspace wspc;
					wspc.Scan(main);

					for(int pi : wspc.use_order) {
						String pk_name = wspc[pi];
						Vector<Tuple<String, bool>>& ps = sources.GetAdd(pk_name);
						const Package& pk = wspc.GetPackage(pi);
						for(int i = 0; i < pk.GetCount(); i++) {
							String path = NormalizePath(SourcePath(pk_name, pk[i]));
							if(!pk.file[i].separator && ppi.FileExists(path)) {
								if(IsCSourceFile(path))
									ps.Add({ NormalizePath(path), pk[i].noblitz });
								else
									workspace_headers.FindAdd(path);
							}
						}
					}
				}
			}

			{
				LTIMESTOP("Dependencies");
				for(int speculative = 0; speculative < 2; speculative++) {
					files.Clear();
					ArrayMap<String, Index<String>> dics;
					for(const Vector<Tuple<String, bool>>& pk : sources)
						for(const Tuple<String, bool>& m : pk) {
							if(IsCppSourceFile(m.a)) {
								int n = files.GetCount();
								ppi.GatherDependencies(m.a, files, dics, speculative);
								for(int i = n; i < files.GetCount(); i++) {
									String p = files.GetKey(i);
									if(!IsCSourceFile(p) && header.Find(p) < 0) {
										master.Add(m.a);
										header.Add(p);
									}
									if(RelaxedIndexerDependencies)
										files[i] = ppi.GetFileTime(p);
								}
							}
							else
							if(IsCSourceFile(m.a))
								files.GetAdd(m.a) = ppi.GetFileTime(m.a);
						}
				}
			}

//			DDUMPC(header);

			Index<String> dirty_files; // files that need to be recompiled (including headers)

//			DDUMPC(dirty_files);
//			DDUMPM(files);


			{
				LTIMESTOP("Loading from cache, checking filetimes");
				for(const auto& m : ~files) {
					String path = m.key;
					String master_file;
					int q = header.Find(path);
					if(q >= 0)
						master_file = master[q];
					FileAnnotation0 f;
					{
						LTIMING("GuiLock 1");
						GuiLock __;
						f = CodeIndex().GetAdd(path);
					}
					if(f.includes != includes || f.defines != defines || f.master_file != master_file) {
						LTIMING("LoadFile");
						String h = LoadFile(CachedAnnotationPath(path, defines, includes, master_file));
						if(h.GetCount()) {
							FileAnnotation lf;
							if(LoadFromString(lf, h)) {
								LTIMING("GuiLock 2");
								GuiLock __;
								f = lf;
								CodeIndex().GetAdd(path) = pick(lf);
							}
						}
					}
					if(f.defines != defines || f.includes != includes || f.time != m.value) {
						dirty_files.FindAdd(Nvl(master_file, path));
					}
				}
			}


			{ // remove files that are not in project anymore
				LTIMESTOP("Removing files");
				GuiLock __;
				for(int i = 0; i < CodeIndex().GetCount(); i++)
					if(files.Find(CodeIndex().GetKey(i)) < 0)
						CodeIndex().Unlink(i);
				CodeIndex().Sweep();
			}

			{
				LTIMESTOP("Create indexer jobs");
				jobs.Clear();
				jobi = 0;
				jobs_done = 0;
				jobs_count = 0;
				for(const auto& pkg : ~sources) {
					Job blitz_job;
					blitz_job.includes = includes;
					blitz_job.defines = defines;
					auto JobAdd = [&](Job& job, const String& path) {
						job.file_times.Add(path, files.Get(path, Time::Low()));
						for(int q = master.Find(path); q >= 0; q = master.FindNext(q)) {
							String hpath = header[q];
							job.file_times.Add(hpath, files.Get(hpath, Time::Low()));
							job.master_files.Add(header[q], path);
						}
						return job;
					};
					int blitz_index = 0;
					for(const auto& pf : pkg.value) {
						FileAnnotation0 f;
						{
							GuiLock __;
							f = CodeIndex().GetAdd(pf.a);
						}
						if(dirty_files.Find(pf.a) >= 0) {
							if(ppi.BlitzApproved(pf.a) && !pf.b && IsCppSourceFile(pf.a)) {
								BlitzFile(blitz_job.blitz, pf.a, ppi.GetFileDefines(pf.a).GetKeys(), blitz_index++);
								JobAdd(blitz_job, pf.a);
							}
							else {
								Job& job = jobs.Add();
								jobs_count = jobs.GetCount();
								job.includes = includes;
								job.defines = defines;
								job.path = pf.a;
								JobAdd(job, pf.a);
							}
						}
					}

					if(blitz_job.blitz.GetCount()) {
						Job& job = jobs.Add();
						jobs_count = jobs.GetCount();
						job = blitz_job;
						job.path = ConfigFile(pkg.key + "$$$blitz.cpp"); // the path is fake, file does not exist
					}
				}
			}
			if(jobs.GetCount()) {
				LLOG("======= Unleash indexers");
				jobs_count = jobs.GetCount();
				event.Broadcast();
			}
			running_scheduler = false;
		}
		ReduceCache(); // good place to do this
		while(jobs_count && !Thread::IsShutdownThreads()) // wait for all jobs to finish so that files are not rescheduled before parsed
			Sleep(100);
	}
}

bool Indexer::IsRunning()
{
	return running_scheduler || jobs_count;
}

double Indexer::Progress()
{
	if(running_scheduler || jobs_count == 0)
		return 0;
	return (double)(jobs_done + jobi) / (2 * jobs_count);
}
