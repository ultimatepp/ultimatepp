#include "RichEdit.h"

namespace Upp {

void DiagramEditor::TheBar(Bar& bar)
{
	bool b = IsCursor();
	bool line = b && CursorItem().IsLine();

	bar.Add(undoredo.IsUndo(), CtrlImg::undo(), [=] { SetCurrent(undoredo.Undo(GetCurrent())); })
	   .Key(K_CTRL_Z);
	bar.Add(undoredo.IsRedo(), CtrlImg::redo(), [=] { SetCurrent(undoredo.Redo(GetCurrent())); })
	   .Key(K_SHIFT|K_CTRL_Z);
	bar.Separator();
	bar.Add(b, "Cut", CtrlImg::cut(), [=] { Cut(); })
	   .Key(K_SHIFT_DELETE)
	   .Key(K_CTRL_X);
	bar.Add(b, "Copy", CtrlImg::copy(), [=] { Copy(); })
	   .Key(K_CTRL_INSERT)
	   .Key(K_CTRL_C);
	bar.Add(IsClipboardAvailableText() || IsClipboardAvailableImage(), "Paste", CtrlImg::paste(), [=] { Paste(); })
	   .Key(K_SHIFT_INSERT)
	   .Key(K_CTRL_V);
	bar.Add(b, "Delete", CtrlImg::remove(), [=] { Delete(); })
	   .Key(K_DELETE);
	bar.Add(b, "Duplicate", DiagramImg::Duplicate(), [=] { Duplicate(); })
	   .Key(K_CTRL_D);
	bar.Add("Select all", CtrlImg::select_all(), [=] { SelectAll(); })
	   .Key(K_CTRL_A);
	bar.Separator();
	bar.Add(b, "Move back", DiagramImg::MoveBack(), [=] { MoveFrontBack(true); });
	bar.Add(b, "Move front", DiagramImg::MoveFront(), [=] { MoveFrontBack(false); });
	bar.Separator();
	bool multi = sel.GetCount() > 1;
	bar.Add(multi, "Align left", DiagramImg::AlignLeft(), [=] { Align(true, ALIGN_LEFT); });
	bar.Add(multi, "Horizontal center", DiagramImg::AlignHCenter(), [=] { Align(true, ALIGN_CENTER); });
	bar.Add(multi, "Align right", DiagramImg::AlignRight(), [=] { Align(true, ALIGN_RIGHT); });
	bar.Separator();
	bar.Add(multi, "Align top", DiagramImg::AlignTop(), [=] { Align(false, ALIGN_TOP); });
	bar.Add(multi, "Vertical center", DiagramImg::AlignVCenter(), [=] { Align(false, ALIGN_CENTER); });
	bar.Add(multi, "Align bottom", DiagramImg::AlignBottom(), [=] { Align(false, ALIGN_BOTTOM); });
	bar.Separator();
	bar.Add(multi, "Same width", DiagramImg::SameWidth(), [=] { Align(true, ALIGN_JUSTIFY); });
	bar.Add(multi, "Same height", DiagramImg::SameHeight(), [=] { Align(false, ALIGN_JUSTIFY); });
	bar.Add(multi, "Same size", DiagramImg::SameSize(), [=] { Align(true, ALIGN_JUSTIFY);  Align(false, ALIGN_JUSTIFY); });
	bar.Separator();
	bar.Add("Zoom", MakeZoomIcon(0.01 * zoom_percent), [=]{ Zoom(); });
	bar.Add("Snap to grid", DiagramImg::Grid(), [=] { grid = !grid; SetBar(); }).Check(grid);
	bar.Separator();
	bar.Add(DiagramImg::SelectMode(), [=] {
		mode = -1;
		SetBar();
	}).Check(mode < 0);
	
	bar.Gap();
	bar.Add(ShapeIcon(shape_i), [=]{
			mode = shape_i;
			SetBar();
			SetAttrs();
		})
	   .Check(mode > 0);
	bar.Add(Upp::CtrlsImg::kDA(), [=] { shape_popup.PopUp(GetMousePos(), this); });
	bar.Gap();
	
	bar.Add(Upp::CtrlsImg::kDA(), [=] { start_cap.PopUp(GetMousePos(), this); });
	bar.Add(CapIcon(line_start, line_end), [=] {
		mode = 0;
		SetBar();
	}).Check(mode == 0);
	bar.Add(Upp::CtrlsImg::kDA(), [=] { end_cap.PopUp(GetMousePos(), this); });

	bar.Add(b, line_width, DPI(50));
	bar.Add(b, line_dash, DPI(50));
	bar.Add(b, ink);
	bar.Add(b, paper);
	bar.Break();
//	ink.DarkContent(IsDarkContent());
	text_editor.FontTools(bar);
	text_editor.InkTool(bar);
	text_editor.PaperTool(bar);
	bar.Separator();
	text_editor.ParaTools(bar);
}

void DiagramEditor::SetBar()
{
	toolbar.Set([=](Bar& bar) { TheBar(bar); });
}

}