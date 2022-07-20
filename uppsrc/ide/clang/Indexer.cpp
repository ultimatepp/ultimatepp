#include "clang.h"

#define LTIMING(x)
#define LTIMESTOP(x)
#define LLOG(x)

struct BlitzMaker {
	Time           time;
	String         blitz;
	Vector<String> individual;

	void Do(const String& pk_name, const Vector<Tuple<String, bool>>& file, Hdepend& hdepend);
};

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

void BlitzMaker::Do(const String& pk_name, const Vector<Tuple<String, bool>>& file, Hdepend& hdepend)
{
	blitz.Clear();
	individual.Clear();
	time = Null;
	int index = 1;
	for(const auto& m : file) {
		if(IsCSourceFile(m.a)) {
			Time filetime = hdepend.FileTime(m.a);
			time = max(Nvl(time, filetime), filetime);
			if(hdepend.BlitzApproved(m.a) && !m.b)
				BlitzFile(blitz, m.a, hdepend, index++);
			else
				individual.Add(m.a);
		}
	}
}

ArrayMap<String, FileAnnotation>& WriteCodeIndex()
{
	DTIMING("WriteCodeIndex");
	static ArrayMap<String, FileAnnotation> m;
	return m;
}

const ArrayMap<String, FileAnnotation>& CodeIndex()
{
	return WriteCodeIndex();
}

static std::atomic<int> indexer_pkgi;
static std::atomic<int> running_indexers;
static Index<String>    visited_files;
static Mutex            visited_files_mutex;

bool IsIndexing()
{
	return running_indexers;
}

void Indexer(const String& includes, const String& defines)
{
	static Hdepend hdepend; // shared between threads
	static Mutex   hdepend_mutex;
	ONCELOCK {
		hdepend.NoConsole();
	};
	Clang clang;
	int tm0 = msecs();
	running_indexers++;
	for(;;) {
		BlitzMaker m;
		String pk_name;
		Vector<Tuple<String, bool>> pk_files;
		Index<String> pk_files_index;
		{
			GuiLock __;
			const Workspace& wspc = GetIdeWorkspace();
			int pkgi = indexer_pkgi++;
			if(pkgi >= wspc.GetCount())
				break;
			LTIMING("Indexer Fetch");
			pk_name = wspc[pkgi];
			const Package& pk = wspc.GetPackage(pkgi);
			for(int i = 0; i < pk.GetCount(); i++) {
				String path = SourcePath(pk_name, pk[i]);
				pk_files.Add({ path, pk[i].noblitz });
				pk_files_index.Add(path);
			}
		}
		{
			Mutex::Lock __(hdepend_mutex);
			m.Do(pk_name, pk_files, hdepend);
		}
		// TODO: Check time
		
		auto ProcessResults = [&] {
			ClangVisitor v;
			Index<String> doing_files;
			v.WhenFile = [&](const String& path) {
				return pk_files_index.Find(path) >= 0;
			};
			{
				DTIMING("Visitor");
				v.Do(clang.tu);
			}

			{
				GuiLock __;
				DTIMING("Process files");
				for(int i = 0; i < v.item.GetCount(); i++) {
					DTIMING("Process file");
					FileAnnotation& f = WriteCodeIndex().GetAdd(v.item.GetKey(i));
					f.defines = defines;
					f.includes = includes;
					DTIMING("Process file pick");
					f.items = pick(v.item[i]);
				}
			}
		};
		
		LTIMESTOP("======= Indexing " + pk_name);
		for(const String& fn : m.individual) {
			LTIMESTOP("Indexing " + fn);
			clang.Parse(fn, Null, includes, defines,
			            CXTranslationUnit_DetailedPreprocessingRecord|
			            CXTranslationUnit_KeepGoing|
			            PARSE_FILE);
			if(clang.tu)
				ProcessResults();
			if(Thread::IsShutdownThreads())
				break;
		}
		if(Thread::IsShutdownThreads())
			break;
		LTIMESTOP("Indexing BLITZ " + pk_name);
		clang.Parse(ConfigFile(pk_name + "$$$blitz.cpp"), m.blitz, includes, defines,
		            CXTranslationUnit_DetailedPreprocessingRecord|
		            CXTranslationUnit_KeepGoing);
		if(clang.tu)
			ProcessResults();

		if(Thread::IsShutdownThreads())
			break;
	}
	if(--running_indexers == 0)
		LLOG("Done everything " << (msecs() - tm0) / 1000.0 << " s");
	LLOG("Done");
}

void StartIndexing(const String& includes, const String& defines)
{
	{
		Mutex::Lock __(visited_files_mutex);
		visited_files.Clear();
	}
	indexer_pkgi = 0;
	if(!IsIndexing()) {
		for(int i = 0; i < CPU_Cores(); i++) // TODO: CPU_Cores?
			Thread::StartNice([=] { Indexer(includes, defines); });
	}
}
