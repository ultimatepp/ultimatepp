#include "ScatterCtrl_Demo.h"


void TabPie::Init()
{
	CtrlLayout(*this);	
	
	chart.ShowPercent();
	chart.AddCategory("Sunny",  8, ::Color(90, 150, 255));
	chart.AddCategory("Cloudy", 4, ::Color(90, 255, 150));	
	chart.AddCategory("Rainy",  2, ::Color(250, 150, 90));	
}
