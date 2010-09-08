#ifndef _AnimatedClip_AnimatedClip_h_
#define _AnimatedClip_AnimatedClip_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class RasterPlayer : public Ctrl {
private:
	virtual void Paint(Draw& w);
	
	Array<Image> images;
	Array<int> delays;
	int ind;
	Color background;
	double speed;
	
public:
	RasterPlayer();
	~RasterPlayer();

	bool Load(const String &fileName);
	bool LoadBuffer(const String &buffer);

	void Play();
	void Stop();
	void NextFrame();
	inline void NextPage() {NextFrame();};
	RasterPlayer& SetBackground(Color c)	{background = c; Refresh(); return *this;}
	RasterPlayer& SetSpeed(double s = 1)	{speed = s; Refresh(); return *this;}
	
	Callback WhenShown;
	
	int GetPageCount() 	{return images.GetCount();};
	int GetFrameCount() {return images.GetCount();};	
	int GetPage() 		{return ind;};
	void SetPage(int i) {ind = minmax(i, 0, images.GetCount());};

	friend void RasterPlayerThread(RasterPlayer *animatedClip);

protected:
	volatile Atomic running, kill;
};


#endif
