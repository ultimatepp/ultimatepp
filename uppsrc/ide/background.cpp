#include "ide.h"

Mutex                     s_allfiles_lock;
VectorMap<String, String> s_allfiles;
Vector<String>            s_allnests;

void ForAllSourceFiles(Event<const VectorMap<String, String>&> fn)
{
	Mutex::Lock __(s_allfiles_lock);
	fn(s_allfiles);
}

void ForAllNests(Event<const Vector<String>&> fn)
{
	Mutex::Lock __(s_allfiles_lock);
	fn(s_allnests);
}

void GatherAllFiles(const String& path, Index<String>& filei, VectorMap<String, String>& file)
{
	if(path.GetCount() == 0)
		return;
	Sleep(0); // This is supposed to be superlazy
	for(FindFile ff(path + "/*.*"); ff && !Thread::IsShutdownThreads(); ff.Next())
		if(ff.IsFolder() && *ff.GetName() != '.')
			GatherAllFiles(ff.GetPath(), filei, file);
		else
		if(ff.IsFile()) {
			String p = NormalizePath(ff.GetPath());
			String lp = ToLower(p);
			if(filei.Find(lp) < 0) {
				filei.Add(lp);
				file.Add(GetFileName(p), p);
			}
		}
}

CoEvent ide_bg_scheduler;

Index<String> GetAllNests(bool sleep)
{
	Index<String> dir;
	for(FindFile ff(ConfigFile("*.var")); ff && !Thread::IsShutdownThreads(); ff.Next()) {
		VectorMap<String, String> var;
		LoadVarFile(ff.GetPath(), var);
		for(String d : Split(var.Get("UPP", ""), ';'))
			dir.FindAdd(NormalizePath(d));
		if(sleep)
			Sleep(0);
	}
	return dir;
}

void IdeBackgroundThread()
{
	while(!Thread::IsShutdownThreads()) {
		VectorMap<String, String> file;
		Index<String> filei;

		Index<String> dir = GetAllNests(true);
		
		for(String d : dir)
			GatherAllFiles(d, filei, file);

		if(TheIde() && TheIde()->search_downloads)
			GatherAllFiles(GetDownloadFolder(), filei, file);

		{
			Mutex::Lock __(s_allfiles_lock);
			s_allfiles = pick(file);
			s_allnests = dir.PickKeys();
		}
	
		ide_bg_scheduler.Wait();
	}
}

void StartIdeBackgroundThread()
{
	Thread::AtShutdown([] {
		ide_bg_scheduler.Broadcast();
	});
	Thread::StartNice(IdeBackgroundThread);
}

void TriggerIdeBackgroundThread(int delay)
{
	static TimeCallback tm;
	tm.KillSet(delay, [=] { ide_bg_scheduler.Broadcast(); });
}
