#include "clang.h"

#define LTIMING(x)   //TIMING(x)
#define LTIMESTOP(x) //DTIMESTOP(x)
#define LLOG(x)      //DLOG(x)
#define LDUMP(x)     //DDUMP(x)
#define LDUMPM(x)    //DDUMPM(x)

String FindMasterSource(PPInfo& ppi, const Workspace& wspc, const String& header_file_)
{
	LTIMING("FindMasterSource");
	String master_source;
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

void AnnotationItem::Serialize(Stream& s)
{
	s % kind
	  % pos
	  % definition
	  % isvirtual
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

String CachedAnnotationPath(const String& source_file, const String& defines, const String& includes, const String& master_file)
{
	Sha1Stream s;
	s << source_file
	  << defines
	  << includes
	  << master_file
	;
	return CacheFile(GetFileTitle(source_file) + "$" + s.FinishString() + ".code_index");
}

void BlitzFile(String& blitz, const String& sourceFile, PPInfo& ppi, int index)
{
	blitz << "\r\n"
	      << "#define BLITZ_INDEX__ F" << index << "\r\n"
	      << "#include \"" << sourceFile << "\"\r\n";
	const VectorMap<String, String>& d = ppi.GetFileDefines(sourceFile);
	for(const String& s : d.GetKeys())
		blitz << "#ifdef " << s << "\r\n"
		      << "#undef " << s << "\r\n"
		      << "#endif\r\n";
	blitz << "#undef BLITZ_INDEX__\r\n";
}

ArrayMap<String, FileAnnotation>& CodeIndex()
{
	static ArrayMap<String, FileAnnotation> m;
	return m;
}

void DumpIndex(const char *file)
{
	GuiLock __;
	FileOut out(file);
	out << GetSysTime() << "\n";
	ArrayMap<String, FileAnnotation>& x = CodeIndex();
	for(const auto& m : ~x) {
		out << m.key << "\n";
		for(const auto& n : m.value.items)
			out << '\t' << n.pos.y << ": " << n.id << " -> " << n.pretty << ", bases: " << n.bases << "\n";
		for(const auto& n : m.value.refs)
			out << '\t' << n.pos << "   " << n.id << "\n";
	}
}

CoEvent              Indexer::event;
CoEvent              Indexer::scheduler;
Mutex                Indexer::mutex;
Vector<Indexer::Job> Indexer::jobs;
int                  Indexer::jobi;
int                  Indexer::jobs_done;
bool                 Indexer::running_scheduler;
std::atomic<int>     Indexer::running_indexers;
String               Indexer::main;
String               Indexer::includes;
String               Indexer::defines;

void Indexer::IndexerThread()
{
	while(!Thread::IsShutdownThreads()) {
		Clang clang;
		clang_CXIndex_setGlobalOptions(clang.index, CXGlobalOpt_ThreadBackgroundPriorityForIndexing);
		int tm0 = msecs();
		bool was_job = false; // for diagnostics
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
				was_job = true;
			}

			if(Thread::IsShutdownThreads())
				break;

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
				LLOG("Storing " << path);
				// TODO: Compress ?
				SaveChangedFile(CachedAnnotationPath(path, f.defines, f.includes, job.master_files.Get(path, Null)), StoreAsString(f), true);
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
		for(int i = 0; i < IndexerThreads; i++) // TODO: CPU_Cores?
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
	PPInfo ppi;
	while(!Thread::IsShutdownThreads()) {
		scheduler.Wait();

		{
			LTIMESTOP("Scheduler");
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
					LTIMING("Load workspace");
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
				LTIMING("Dependencies");
				for(int speculative = 0; speculative < 2; speculative++) {
					files.Clear();
					ArrayMap<String, Index<String>> dics;
					for(const Vector<Tuple<String, bool>>& pk : sources)
						for(const Tuple<String, bool>& m : pk) {
							if(IsCSourceFile(m.a)) {
								int n = files.GetCount();
								ppi.GatherDependencies(m.a, files, dics, speculative);
								for(int i = n; i < files.GetCount(); i++) {
									String p = files.GetKey(i);
									if(!IsCSourceFile(p) && header.Find(p) < 0 && IsCppSourceFile(m.a)) {
										master.Add(m.a);
										header.Add(p);
									}
								}
							}
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
						GuiLock __;
						f = CodeIndex().GetAdd(path);
					}
					if(f.includes != includes || f.defines != defines || f.master_file != master_file) {
						String h = LoadFile(CachedAnnotationPath(path, defines, includes, master_file));
						if(h.GetCount()) {
							FileAnnotation lf;
							if(LoadFromString(lf, h)) {
								GuiLock __;
								f = lf;
								CodeIndex().GetAdd(path) = pick(lf);
							}
						}
					}
					if(f.defines != defines || f.includes != includes || f.time != m.value)
						dirty_files.FindAdd(path);
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
							if(ppi.BlitzApproved(pf.a) && !pf.b) {
								BlitzFile(blitz_job.blitz, pf.a, ppi, blitz_index++);
								JobAdd(blitz_job, pf.a);
							}
							else {
								Job& job = jobs.Add();
								job.includes = includes;
								job.defines = defines;
								job.path = pf.a;
								JobAdd(job, pf.a);
							}
						}
					}

					if(blitz_job.blitz.GetCount()) {
						Job& job = jobs.Add();
						job = blitz_job;
						job.path = ConfigFile(pkg.key + "$$$blitz.cpp"); // the path is fake, file does not exist
					}
				}
			}
			if(jobs.GetCount()) {
				LLOG("======= Unleash indexers");
				event.Broadcast();
			}
			running_scheduler = false;
		}
		ReduceCache(); // good place to do this
	}
}

bool Indexer::IsRunning()
{
	if(running_scheduler)
		return true;
	Mutex::Lock __(mutex);
	return jobs.GetCount();
}

double Indexer::Progress()
{
	if(running_scheduler)
		return 0;
	Mutex::Lock __(mutex);
	if(jobs.GetCount() == 0)
		return 1;
	return (double)(jobs_done + jobi) / (2 * jobs.GetCount());
}
