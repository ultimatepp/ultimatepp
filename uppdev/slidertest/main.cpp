#include "slidertest.h"



slidertest::slidertest()
{
	CtrlLayout(*this, "Window title");
	
	sldRegH << THISBACK2( OnSlider, &sldRegH, &txtRegH );
	sldRegV << THISBACK2( OnSlider, &sldRegV, &txtRegV );
	sldRevH << THISBACK2( OnSlider, &sldRevH, &txtRevH );
	sldRevV << THISBACK2( OnSlider, &sldRevV, &txtRevV );
	
	sldRegH.MinMax( -100, 100 );
	sldRevH.MinMax( 100, -100 );
	sldRegV.MinMax( -100, 100 );
	sldRevV.MinMax( 100, -100 );
}

void slidertest::OnSlider( SliderCtrl* sld, StaticText* txt )
{
	txt->SetText( sld->GetData().ToString().Begin() );
}

GUI_APP_MAIN
{
	slidertest().Run();
}

