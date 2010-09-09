#include <CtrlLib/CtrlLib.h>
#include <RasterPlayer/RasterPlayer.h>

using namespace Upp;

#include "AnimatedClip.h"
#include "AnimatedClip.brc"

AnimatedClip::AnimatedClip() {
	CtrlLayout(*this, "RasterPlayer demo");
	
	browse   <<= THISBACK(Browse);
	play 	 <<= THISBACK(Play);
	stop 	 <<= THISBACK(Stop);
	openNext <<= THISBACK(OpenNext);
	numThreads = 0;
	numThreads <<= THISBACK(ChangeThreads);
	#ifndef _MULTITHREADED
		numThreads.EnableCase(1, false);
	#endif
	
	clip.WhenShown = THISBACK(OnShown);
	arrows.LoadBuffer(String(animatedArrow, animatedArrow_length));
	arrows.Play();
	earth.LoadBuffer(String(animatedEarth, animatedEarth_length));
	earth.Play();
		
	Sizeable().Zoomable();
}

void AnimatedClip::Browse() {
	FileSel fs;
	
	fs.PreSelect(fileName);
	fs.Type("Animation type", "*.gif, *.tif, *.tiff");
	
	if (fs.ExecuteOpen("Choose animation file"))
		fileName <<= ~fs;

	if (!clip.Load(fileName)) {
		Exclamation("Invalid input");
		return;
	}
	numberPages = FormatInt(clip.GetPageCount());
	openedPage = FormatInt(clip.GetPage());
}

void AnimatedClip::Play() {
	clip.Play();
}

void AnimatedClip::Stop() {
	clip.Stop();
}

void AnimatedClip::OpenNext() {
	clip.NextFrame();
}
	
void AnimatedClip::OnShown() {
	openedPage = FormatInt(clip.GetPage());
}

void AnimatedClip::ChangeThreads() {
	clip.SetMT(numThreads == 1);
}

GUI_APP_MAIN {
	AnimatedClip().Run();
}

