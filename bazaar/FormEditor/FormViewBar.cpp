#include "FormView.hpp"

void FormView::CreateObjectMenu(Bar& bar, int id)
{
	int selCount = GetSelected().GetCount();

	if (selCount > 1)
		bar.Add(t_("Align selected"), THISBACK(AlignObjectMenu));

	if (selCount != GetObjectCount())
		bar.Add(t_("Invert selection"), THISBACK(InvertSelection));
	
	if ((selCount != GetObjectCount()) || selCount > 1)
		bar.Separator();

	if (selCount == 1)
	{
		bar.Add(t_("Outline"), THISBACK1(ToggleOutlineDraw, id)).Check(IsOutlineDraw(id));
		bar.Separator();
	}

	if (selCount <= 1)
	{
		bar.Add(t_("Top"), THISBACK1(MoveToTopObject, id));
		bar.Add(t_("Up"), THISBACK1(MoveUpObject, id));
		bar.Add(t_("Down"), THISBACK1(MoveDownObject, id));
		bar.Add(t_("Bottom"), THISBACK1(MoveToBottomObject, id));
		bar.Separator();
	}

	bar.Add(t_("Delete"), THISBACK(RemoveSelection));

	if (selCount >= 1)
	{
		bar.Separator();
		bar.Add(t_("Properties"), THISBACK1(OpenObjectProperties, GetSelected()));
	}
}

void FormView::AddObjectMenu(Bar& bar, Point p)
{
	bar.Add(t_("Button"), THISBACK2(CreateObject, p, "Button"));
	bar.Add(t_("EditField"), THISBACK2(CreateObject, p, "EditField"));
	bar.Add(t_("EditInt"), THISBACK2(CreateObject, p, "EditInt"));
	bar.Add(t_("DropDate"), THISBACK2(CreateObject, p, "DropDate"));
	bar.Separator();
	bar.Add(t_("ProgressBar"), THISBACK2(CreateObject, p, "ProgressBar"));
	bar.Add(t_("Label"), THISBACK2(CreateObject, p, "Label"));
	bar.Separator();
	bar.Add(t_("TabCtrl"), THISBACK2(CreateObject, p, "TabCtrl"));
	bar.Add(t_("GridCtrl"), THISBACK2(CreateObject, p, "GridCtrl"));
	bar.Separator();
	bar.Add(t_("Form"), THISBACK2(CreateObject, p, "Form"));
}

void FormView::AlignObjectMenu(Bar& bar)
{
	bar.Add(t_("Top "), THISBACK(AlignTopSelection));
	bar.Add(t_("Left"), THISBACK(AlignLeftSelection));
	bar.Add(t_("Right"), THISBACK(AlignRightSelection));
	bar.Add(t_("Bottom "), THISBACK(AlignBottomSelection));
}
