#include <Core/Core.h>

using namespace Upp;

Index<String> md5_index;

void ScanFolder(const String& path)
{
	FindFile ff(path + "/*.*");
	int nfiles = 0;
	Md5Stream smd5;
	Time tm = Time::High();
	int64 size = 0;
	while(ff) {
		if(ff.IsFolder())
			ScanFolder(ff.GetPath());
		else
		if(ff.IsFile()) {
			FileIn in(ff.GetPath());
			ASSERT(in);
			smd5.Put(in);
			tm = min(tm, Time(ff.GetLastWriteTime()));
			size += ff.GetLength();
			nfiles++;
		}
		ff.Next();
	}
	String md5 = smd5.FinishString();
	LOG(path << ", files: " << nfiles
	    << ", time: " << tm
	    << ", size: " << (size >> 20) << "MB"
	    << ", " << (md5_index.Find(md5) < 0 ? "NEW" : "COPY"));
	md5_index.Add(md5);
}

CONSOLE_APP_MAIN
{
//	ScanFolder(GetHomeDirFile("/mnt/nas1/DigiFoto"));
	ScanFolder("/mnt/nas1/DigiFoto");
	LOG("FINISHED");
}
