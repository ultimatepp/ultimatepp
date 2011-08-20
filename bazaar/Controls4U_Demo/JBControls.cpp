#include "JBControlsDemo.h"

JBControlsDemo::JBControlsDemo()
{
	CtrlLayout(*this);
//	PromptOK("Layout Ok");
	progress.Percent(true);
	progress.Set(~slider,100);
//	jbstar.Percent(true);
//	jbstar2.Percent(true);
	jbstar.Set(~slider,100);
	jbstar2.Set(~slider,100);
	jbstar.SetFontColor(Red);
	jbstar2.SetFontColor(Black);
	
	jbstar.ShowVotes(true);
	jbstar2.ShowVotes(true);
	
	jbstar.SetVotes(~slider2);
	jbstar2.SetVotes(~vslider);
	
//	slider.SetValue(25);
//	slider2.SetValue(55);
//	vslider.SetValue(33);
	
	slider <<= THISBACK(Slide);
	slider2 <<= THISBACK(Slide2);
	vslider <<= THISBACK(VSlide);
	novotes <<= THISBACK(NoVotes);
}

void JBControlsDemo::NoVotes()
{
	if (novotes.GetData()) jbstar2.SetVotes(-1);
	else jbstar2.SetVotes(~vslider);
}

void JBControlsDemo::Slide()
{
	meter.SetData(~slider);
	progress.Set(~slider,100);
	indic.SetData(~slider);
	jbstar.Set(~slider,100);
	jbstar2.Set(~slider,100);
}

void JBControlsDemo::Slide2()
{
	meter2.SetData(~slider2);
	jbstar.SetVotes(~slider2);
}

void JBControlsDemo::VSlide()
{
	vindic.SetData(~vslider);
	if (novotes.GetData()==false) jbstar2.SetVotes(~vslider);
}

