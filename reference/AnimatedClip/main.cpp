#include <CtrlLib/CtrlLib.h>
#include <RasterPlayer/RasterPlayer.h>

using namespace Upp;

#include "AnimatedClip.h"
#include "AnimatedClip.brc"

AnimatedClip::AnimatedClip() {
	CtrlLayout(*this, "RasterPlayer demo");
	
	browse << [=] { Browse(); };
	play << [=] { clip.Play(); };
	stop << [=] { clip.Stop(); };
	openNext << [=] { clip.NextFrame(); };
	numThreads << [=] { clip.SetMT(numThreads == 1); };
	numThreads = 0;
	#ifndef _MULTITHREADED
		numThreads.EnableCase(1, false);
	#endif
	
	clip.WhenShown << [=] { openedPage = FormatInt(clip.GetPage()); };
	arrows.LoadBuffer(String(animatedArrow, animatedArrow_length));
	arrows.Play();
	earth.LoadBuffer(String(animatedEarth, animatedEarth_length));
	earth.Play();
		
	Sizeable().Zoomable();
}

void AnimatedClip::Browse() {
	FileSel fs;
	
	fs.PreSelect(~fileName);
	fs.Type("Animation type", "*.gif, *.tif, *.tiff");
	
	if (fs.ExecuteOpen("Choose animation file"))
		fileName <<= ~fs;

	if (!clip.Load(~fileName)) {
		Exclamation("Invalid input");
		return;
	}
	numberPages = FormatInt(clip.GetPageCount());
	openedPage = FormatInt(clip.GetPage());
}

GUI_APP_MAIN {
	AnimatedClip().Run();
}

