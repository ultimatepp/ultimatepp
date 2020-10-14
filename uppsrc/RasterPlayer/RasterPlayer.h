#ifndef _RasterPlayer_RasterPlayer_h_
#define _RasterPlayer_RasterPlayer_h_

#include <CtrlLib/CtrlLib.h>

namespace Upp {

class RasterPlayer : public Ctrl {
private:
	virtual void Paint(Draw& w);
	bool IsKilled();
	
	Upp::Array<Image> images;
	Upp::Array<int> delays;
	int ind;
	Color background;
	double speed;
	bool mt;
		
	TimeStop tTime;
	double tFrame_ms;

public:
	RasterPlayer();
	virtual ~RasterPlayer() noexcept;

	bool Load(const String &fileName);
	bool LoadBuffer(const String &buffer);

	void Play();
	void Stop();
	bool IsRunning()	{return running;}
	void NextFrame();
	inline void NextPage() {NextFrame();};
	RasterPlayer& SetBackground(Color c)	{background = c; Refresh(); return *this;}
	RasterPlayer& SetSpeed(double s = 1)	{speed = s; Refresh(); return *this;}
	RasterPlayer& SetMT(bool _mt = false);
	
	Event<> WhenShown;
	
	int GetPageCount() 	{return images.GetCount();};
	int GetFrameCount() {return images.GetCount();};	
	int GetPage() 		{return ind;};
	void SetPage(int i) {ind = minmax(i, 0, images.GetCount());};
	
#ifdef _MULTITHREADED	
	friend void RasterPlayerThread(RasterPlayer *animatedClip);
#endif	
	void TimerFun();
	
protected:
	volatile Atomic running, kill;
};

}

#endif
