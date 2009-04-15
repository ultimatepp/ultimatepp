#include "Crush.h"

Crush::Crush()
{
	arcCrush.AddColumn(t_("Artista"));
	arcCrush.AddColumn(t_("Disco"));
	arcCrush.SetSortColumn(0);
	
	arcCrush.Set(0, 0, "Morbid Angel");
	arcCrush.Set(0, 1, "Domination");
	arcCrush.Set(1, 0, "Cradle of Filth");
	arcCrush.Set(1, 1, "The Principle of Evil Made Flesh");

	btnCrush <<= THISBACK(CrushThis);
	
	Zoomable().Sizeable();
	CtrlLayout(*this, "Crush Example");
}

void Crush::CrushThis()
{
	arcCrush.DoColumnSort();
}

GUI_APP_MAIN
{
	Crush().Run();
}
