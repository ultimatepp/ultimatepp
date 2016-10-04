#include "ThreadedImageLoader.h"

#define LLOG(x) TLOG(x)

String LogTime()
{
	String out;
	out << GetSysTime() << " % " << (dword)msecs() % 10000;
	return out;
}

String LogContext()
{
	String out = LogTime();
	out << ", t=" << (int)GetCurrentThreadId() << ": ";
	return out;
}

#define TLOG(x) RLOG(LogContext() << x);

ImageLoader::ImageLoader() :
ready_to_load_(false),
time_to_die_(false),
new_image_ready_(false),
status_("Thread not started") {
	LLOG("LoadingThread> "+status_);
}

void ImageLoader::LoadingThreadFunction()
{
	// Repeat until time_to_die, then exit
	ready_to_load_ = true;
	while (!time_to_die_) {
		status_ = "Waiting for job";
		LLOG("Wait");
		job_ready_.Wait();
		LLOG("//Wait");
		if (time_to_die_)
			break;

		status_ = "Loading";
		LLOG("StdLoadImage");
		loading_info_.image = StdLoadArrayFile(loading_info_.filepath).pixel;
		LLOG("//StdLoadImage");
		//------------------------------------------------------
		Size cachesize = GetFitSize(loading_info_.image.GetSize(), ScreenInfo().GetClip().Size());
		LLOG("Image ctr");
//		Image cacheimage(cachesize);
		LLOG("//Image ctr");
		LLOG("srcim");
		const PixelArray& srcim = loading_info_.image;
//		PixelArray srcim(ImageToPixelArray(loading_info_.image));
		LLOG("dstim");
		PixelArray dstim(cachesize, -3);
//		PixelArray dstim(ImageToPixelArray(cacheimage));
		//-- Either just resize or resize with anti-aliasing
		//PixelCopy(dstim,cacheimage.GetSize(),srcim,loading_info_.image.GetSize());
		TLOG("PixelCopyAntiAlias");
		PixelCopyAntiAlias(dstim,dstim.GetSize(),srcim,srcim.GetSize());
		TLOG("//PixelCopyAntiAlias");
		loading_info_.image = dstim; //PixelArrayToImage(dstim);
		TLOG("//PixelArrayToImage");
		//------------------------------------------------------
		loading_info_.valid = true;
		status_ = "Image ready";
		LLOG("LoadingThread> "+status_);
		new_image_ready_ = true;
		ready_to_load_ = true;
		PostCallback(WhenImageReady);
	}
	status_ = "Finished";
	//LLOG("LoadingThread> "+status_);
//	Thread::Exit(0);
}
void ImageLoader::LoadNextImage(const ThreadedImageLoadInfo& info)
{
	Reset();
	ready_to_load_ = false;

	ThreadedImageLoadInfo& blah = loading_info_;
	blah = info;
	loading_info_.image = PixelArray();
	loading_info_.valid = false;

	TLOG("LoadNextImage / Set");
	job_ready_.Set();
	TLOG("//LoadNextImage / Set");
}
void ImageLoader::Die()
{
	WhenImageReady = Callback::Empty();
	new_image_ready_ = false;
	time_to_die_ = true;
	ready_to_load_ = false;
	status_ = "Terminating";
	job_ready_.Set();
}

ThreadedImageLoader::ThreadedImageLoader()
{
	CtrlLayout(*this, "Threaded Image Loader");
	AddFrame(menus);
	menus.Set(THISBACK(SetupMainMenu));
	loader.WhenImageReady = THISBACK(OnImageLoaded);
	loader_thread.Run(callback(&loader,ImageLoader::LoadingThreadFunction));
	loader_thread.Priority(0);
	Zoomable().Sizeable();
	SetTimeCallback(-200, THISBACK(SyncTime));
}
ThreadedImageLoader::~ThreadedImageLoader()
{
	loader.Die();
	loader_thread.Wait();
}
void ThreadedImageLoader::SyncTime()
{
	time <<= LogTime();
}
void ThreadedImageLoader::OnImageLoaded() {
	TLOG("OnImageLoaded, display.SetImage");
	display.SetImage(PixelArrayToImage(loader.Result().image));
	TLOG("//display.SetImage");
	TLOG("Loaded Image: "+loader.Result().filepath);
	if (!remaining.IsEmpty()) {
		TLOG(" - next is: "+remaining.Top());
		loader.LoadNextImage(remaining.Pop());
	}
}
void ThreadedImageLoader::OnSelectDir() {
	FileSel filesel;
	filesel.Type("All Images","*.jpg;*.png;*.bmp;*.jpeg");
	filesel.Type("All files","*.*");
	if (!filesel.ExecuteSelectDir("Select directory")) {
		return;
	}
	remaining.Clear();
	String dirname = filesel;
	FindFile ff(dirname + "\\*.jpg");
	while (ff.Next()) {
		remaining.Insert(0,dirname+"\\"+ff.GetName());
	}
	LLOG("Adding files to load list from directory: "+dirname);
	for (int i = 0; i < remaining.GetCount(); ++i)
		LLOG(" "+remaining[i]);

	if (!remaining.IsEmpty())
		loader.LoadNextImage(remaining.Pop());
}
void ThreadedImageLoader::SetupMainMenu(Bar& bar)
{
	bar.Add("[E&xit]",THISBACK(Close)).Key(K_CTRL_E);
	bar.Add("[Select &Directory]",THISBACK(OnSelectDir)).Key(K_CTRL_D);
}

GUI_APP_MAIN
{
	JpgEncoder::Register();
	ThreadedImageLoader().Run();
}
