#include "RichEdit.h"

namespace Upp {

void DiagramEditor::TheBar(Bar& bar)
{
	bool b = IsCursor();

	bar.Add(undoredo.IsUndo(), CtrlImg::undo(), [=] { SetCurrent(undoredo.Undo(GetCurrent())); })
	   .Key(K_ALT_BACKSPACE)
	   .Key(K_CTRL_Z);
	bar.Add(undoredo.IsRedo(), CtrlImg::redo(), [=] { SetCurrent(undoredo.Redo(GetCurrent())); })
	   .Key(K_SHIFT|K_ALT_BACKSPACE)
	   .Key(K_CTRL_Y)
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
	bar.Add(b, "Horizontal center", DiagramImg::HorzCenter(), [=] { Align(true, ALIGN_NULL); });
	bar.Add(b, "Vertical center", DiagramImg::VertCenter(), [=] { Align(false, ALIGN_NULL); });
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
	bar.Add("Display grid and connections", DiagramImg::DisplayGrid(), [=] { display_grid = !display_grid; SetBar(); Refresh(); }).Check(display_grid);
	bar.Separator();
	bar.Add("Diagram size", DiagramImg::Size(), [=] { ChangeSize(); });
	bar.Separator();
	int icx = IconSz().cx + DPI(4) + DPI(18);
	bar.Add(shape, icx);
	shape.Enable(!(IsCursor() && findarg(CursorItem().shape, DiagramItem::SHAPE_SVGPATH, DiagramItem::SHAPE_IMAGE) >= 0));
	bar.Add(line_start, icx);
	bar.Add(line_end, icx);
	bar.Add(line_width, icx);
	bar.Add(line_dash, icx);
	ink.DarkContent(IsDarkContent());
	bar.Add(ink);
	paper.DarkContent(IsDarkContent());
	bar.Add(paper);
	bool fh = b;
	bool fv = b;
	bool ar = b;
	ForEachConst([&](const DiagramItem& m) {
		fh = fh && m.flip_horz;
		fv = fv && m.flip_vert;
		ar = ar && m.aspect_ratio;
	});
	bar.Add(b, "Flip Horizontal", DiagramImg::FlipHorz(), [=] {
		ForEach([&](DiagramItem& m) { m.flip_horz = !m.flip_horz; });
	})
	.Check(fh);
	bar.Add(b, "Flip Vertical", DiagramImg::FlipVert(), [=] {
		ForEach([&](DiagramItem& m) { m.flip_vert = !m.flip_vert; });
	})
	.Check(fv);
	bar.Add(b, "Aspect Ratio", DiagramImg::Aspect(), [=] {
		ForEach([&](DiagramItem& m) {
			m.aspect_ratio = !m.aspect_ratio;
			if(m.aspect_ratio && !m.IsLine()) {
				Sizef sz1, sz2;
				ComputeAspectSize(m, sz1, sz2);
				m.size = (sz1.cx < sz2.cx ? sz1 : sz2) / 2;
			}
		});
	})
	.Check(ar);
	bar.Separator();
	Size isz = IconSz();
	for(int i = 0; i < tool_count; i++) {
		DiagramItem m = tl[i];
		if(m.IsLine()) {
			m.pos = Point(2, 2);
			m.size = Size(isz.cx - 4, isz.cy - 4);
		}
		else {
			m.pos = Pointf(Point(isz)) / 2;
			m.size = m.pos - 2;
		}
		m.width = log(m.width + 1);
		bar.Add(MakeIcon(m, isz), [=] {
			CancelSelection();
			if(tool == i)
				tool = -1;
			else {
				tool = i;
				GetAttrs(tl[i]);
			}
			SetBar();
		})
		.Key(get_i(i, K_CTRL_1, K_CTRL_2, K_CTRL_3, K_CTRL_4))
		.Check(tool == i);
	}
	bar.Break();
	RichEdit& be = edit_text ? text_editor : editor_bar;
	if(!edit_text) {
		if(sel.GetCount()) {
			editor_bar.formatinfo = GetSelectionFormatInfo();
			editor_bar.SetEditable();
			editor_bar.ShowFormat();
			editor_bar.diagram_bar_hack = true;
			editor_bar.WhenSel = [=] {
				for(int i : sel) {
					String& qtf = data.item[i].qtf;
					RichText txt = ParseQTF(qtf);
					txt.ApplyFormatInfo(0, editor_bar.formatinfo, txt.GetLength());
					qtf = AsQTF(txt, CHARSET_UTF8, QTF_BODY|QTF_NOCHARSET|QTF_NOLANG|QTF_NOSTYLES);
				}
				Sync();
			};
		}
		else
			editor_bar.SetReadOnly();
	}
	be.FontTools(bar);
	be.InkTool(bar);
	be.PaperTool(bar);
	bar.Separator();
	be.ParaTools(bar);
}

void DiagramEditor::SetBar()
{
	toolbar.Set([=](Bar& bar) { TheBar(bar); });
}

}