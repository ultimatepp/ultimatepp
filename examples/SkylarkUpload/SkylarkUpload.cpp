#include "SkylarkUpload.h"


SkylarkUpload &Upload()
{
	return (SkylarkUpload &)SkylarkApp::TheApp();
}

SkylarkUpload::SkylarkUpload(String const &rPath)
{
	root = "server";
	rootPath = rPath;
	RealizeDirectory(rootPath);

#ifdef _DEBUG
	prefork = 0;
	use_caching = false;
#endif

}

CONSOLE_APP_MAIN
{
#ifdef _DEBUG
	StdLogSetup(LOG_FILE|LOG_COUT);
	Ini::skylark_log = true;
#endif

	SkylarkUpload upload(AppendFileName(GetHomeDirectory(), "UPLOAD/"));
	
	upload.Run();

}
