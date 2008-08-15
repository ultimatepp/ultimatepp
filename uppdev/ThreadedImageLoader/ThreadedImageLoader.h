#ifndef _ThreadedImageLoader_ThreadedImageLoader_h
#define _ThreadedImageLoader_ThreadedImageLoader_h

#include <CtrlLib/CtrlLib.h>
#include <plugin/jpg/jpg.h>
#include <ImageDisplayCtrl/ImageDisplayCtrl.h>

#define LAYOUTFILE <ThreadedImageLoader/ThreadedImageLoader.lay>
#include <CtrlCore/lay.h>

struct ThreadedImageLoadInfo : Moveable<ThreadedImageLoadInfo> {
	String filepath;

	ThreadedImageLoadInfo() {}
	ThreadedImageLoadInfo(const String& in_filepath) : filepath(in_filepath) {}
};
struct ThreadedImageLoadResult : public ThreadedImageLoadInfo , Moveable<ThreadedImageLoadResult> {
	PixelArray image;
	bool valid;
	ThreadedImageLoadResult() : valid(false) {}
};


class ImageLoader
{
	public:
		ImageLoader();

		Callback WhenImageReady;

		volatile bool  ReadyToLoad() const       { return ready_to_load_; }
		volatile bool  NewImageAvailable() const { return new_image_ready_; }
		inline void Reset()                      { new_image_ready_ = false; }

		const ThreadedImageLoadResult& Result() const        { return loading_info_; }

		void LoadNextImage( const ThreadedImageLoadInfo& info );
		void Die();

		String Status() const { return status_; }

		void LoadingThreadFunction();

	protected:
		volatile bool ready_to_load_;
		volatile bool new_image_ready_;
		volatile bool time_to_die_;

		ThreadedImageLoadResult loading_info_;

		String status_;

		Event job_ready_;
};


class ThreadedImageLoader : public WithThreadedImageLoaderLayout<TopWindow> {
public:
	typedef ThreadedImageLoader CLASSNAME;
	ThreadedImageLoader();
	~ThreadedImageLoader();

	void OnImageLoaded();
	void OnSelectDir();
	void SyncTime();

public:
	// Menu control
	MenuBar menus;
	void SetupMainMenu(Bar& bar);

protected:
	ImageLoader loader;
	Thread loader_thread;
	Vector<String> remaining;
};

#endif
