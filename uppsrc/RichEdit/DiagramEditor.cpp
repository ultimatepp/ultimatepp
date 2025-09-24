#include "RichEdit.h"

namespace Upp {

#define IMAGECLASS DiagramImg
#define IMAGEFILE <RichEdit/Diagram.iml>
#include <Draw/iml_source.h>

DiagramEditor::DiagramEditor()
{
	AddFrame(toolbar);

	Add(text_editor);
	text_editor.NoRuler().ViewBorder(0);
	text_editor.WhenRefreshBar = [=] { SetBar(); };
	text_editor.WhenEnter << [=] { FinishText(); };
	text_editor.WhenEsc << [=] { edit_text = false; Sync(); };
	text_editor.WhenAction << [=] {
		SyncEditorRect();
	};

	ink.ColorImage(DiagramImg::Ink())
	   .NullImage(DiagramImg::InkNull())
	   .StaticImage(DiagramImg::InkA());
	ink.Tip(t_("Line color"));
	ink << [=] { SetAttrs(ATTR_INK); };

	paper.ColorImage(DiagramImg::Paper())
	   .NullImage(DiagramImg::PaperNull())
	   .StaticImage(DiagramImg::PaperA());
	paper.Tip(t_("Background color"));

	paper << [=] { SetAttrs(ATTR_PAPER); };
	shape << [=] { SetAttrs(ATTR_SHAPE); SetFocus();};
	Shapes(shape.popup);
	shape.popup.count = DiagramItem::SHAPE_SVGPATH;
	
	Caps(line_start.popup, true);
	Caps(line_end.popup, false);
	line_start << [=] { SetAttrs(ATTR_CAP0); SetFocus(); };
	line_end << [=] { SetAttrs(ATTR_CAP1); SetFocus(); };


	Widths(line_width.popup);
	line_width << [=] { SetAttrs(ATTR_WIDTH); SetFocus(); };

	Dashes(line_dash.popup);
	line_dash << [=] { SetAttrs(ATTR_DASH); SetFocus(); };

	tl[0].shape = DiagramItem::SHAPE_LINE;
	tl[1].shape = DiagramItem::SHAPE_ROUNDRECT;

	ResetUndo();
	Sync();

	sb.AutoHide();
	sb.WithSizeGrip();
	AddFrame(sb);
	sb.WhenScroll << [=] { Sync(); };

	editor = true;

	GetAttrs(DiagramItem());
}

void DiagramEditor::SerializeSettings(Stream& s)
{
	int version = 1;
	
	s % version;
	
	s % grid % display_grid;

	for(int i = 0; i < 2; i++)
		s % tl[i];
}

void DiagramEditor::SetupDark(ColorPusher& c) const
{
	c.AllowDarkContent(allow_dark_content);
	c.DarkContent(dark_content);
}

bool DiagramEditor::IsDarkContent() const
{
	return dark_content || allow_dark_content && IsDarkTheme();
}

DiagramEditor& DiagramEditor::DarkContent(bool b)
{
	dark_content = b;
	Sync();
	return *this;
}

DiagramEditor& DiagramEditor::AllowDarkContent(bool b)
{
	allow_dark_content = b;
	Sync();
	return *this;
}

void DiagramEditor::Skin()
{
	SetBar();
	editor_bar.Skin();
}

void DiagramEditor::Paint(Draw& w)
{
	fast = HasCapture() && paint_ms > 50 && moved;
	int t0 = msecs();
	{
		DrawPainter iw(w, GetSize());
		iw.Co();
		iw.Clear(SWhite());

		iw.Scale(GetZoom());
		iw.Offset(-(Point)sb);
		Size dsz = data.GetSize();
		iw.Move(dsz.cx, 0).Line(dsz.cx, dsz.cy).Line(0, dsz.cy);
		if(IsNull(data.size))
		   iw.Dash("2").Stroke(1, Gray());
		else
		   iw.Stroke(0.2, SColorHighlight());

		if(data.item.GetCount() == 0) {
			iw.DrawText(DPI(30), DPI(30), "Right-click to insert item(s)", ArialZ(10).Italic(), SLtGray());
			iw.DrawText(DPI(30), DPI(50), "Double-click to edit text", ArialZ(10).Italic(), SLtGray());
		}

		if(display_grid)
			for(int x = 0; x < dsz.cx; x += 8)
				for(int y = 0; y < dsz.cy; y += 8)
					iw.DrawRect(x, y, 1, 1, Blend(SWhite(), SGreen()));

		dark = IsDarkContent();
		data.Paint(iw, *this);

		if(HasCapture() && doselection && tool < 0) {
			Rect r(dragstart, dragcurrent);
			r.Normalize();
			iw.Rectangle(r).Fill(30 * SColorHighlight()).Stroke(1, LtRed()).Dash("4").Stroke(1, White());
		}

	}
	if(!fast)
		paint_ms = msecs() - t0;
}

void DiagramEditor::Sync()
{
	Refresh();
	SetBar();
	sb.SetTotal(data.GetEditSize() + Point(10, 10));
	sb.SetPage(sb.GetReducedViewSize() / GetZoom());
	sb.SetLine(8, 8);
	SyncEditor();
	
	if(!IsCursor() && findarg((int)~shape, DiagramItem::SHAPE_SVGPATH, DiagramItem::SHAPE_IMAGE) >= 0)
		shape <<= DiagramItem::SHAPE_ROUNDRECT;
}

void DiagramEditor::Layout()
{
	Sync();
}

void DiagramEditor::ResetUndo()
{
	Index<Value> blob_ids;
	for(const DiagramItem& m : data.item)
		if(m.blob_id.GetCount())
			blob_ids.FindAdd(m.blob_id);
	undoredo.Reset(GetCurrent(), ValueArray(blob_ids.PickKeys()));
}

void DiagramEditor::Commit()
{
	if(IsCursor()) {
		DiagramItem& m = CursorItem();
		if(!m.IsLine())
			m.Normalize();
	}
	Index<Value> blob_ids;
	size_t blobsz = 0;
	for(const DiagramItem& m : data.item) {
		if(m.blob_id.GetCount()) {
			if(blob_ids.Find(m.blob_id) < 0) {
				blob_ids.Add(m.blob_id);
				blobsz += data.GetBlob(m.blob_id).GetCount();
			}
		}
	}
	if(undoredo.Commit(GetCurrent(), ValueArray(clone(blob_ids.GetKeys())))) {
		size_t limit = max((size_t)20000000, 2 * blobsz);
		for(;;) { // make sure that blobs are not excessive
			Index<String> ublob_ids;
			size_t ublobsz = 0;
			auto AddIds = [&](const ValueArray& va) {
				for(Value v : va) {
					String id = ~v;
					if(ublob_ids.Find(id) < 0) {
						ublob_ids.Add(id);
						ublobsz += data.GetBlob(id).GetCount();
					}
				}
			};
			AddIds(undoredo.GetCommitInfo());
			for(int i = 0; i < undoredo.GetUndoCount(); i++)
				AddIds(undoredo.GetUndoInfo(i));
			for(int i = 0; i < undoredo.GetRedoCount(); i++)
				AddIds(undoredo.GetRedoInfo(i));
			if(ublobsz <= limit) {
				data.SweepBlobs(ublob_ids); // remove blobs that are not used anymore
				break;
			}
			if(undoredo.GetUndoCount())
				undoredo.DropUndo();
			else
			if(undoredo.GetRedoCount())
				undoredo.DropRedo();
			else
				break;
		}
		
		SetBar();
		Sync();
	}
	conns.Clear();
}

String DiagramEditor::GetCurrent()
{
	return StoreAsString(data);
}

bool DiagramEditor::SetCurrent(const String& s)
{
	KillCursor();
	bool b = LoadFromString(data, s);
	Sync();
	return b;
}

void DiagramEditor::CancelSelection()
{
	sel.Clear();
	FinishText();
	KillCursor();
	Sync();
}

void DiagramEditor::Zoom()
{
	zoom_percent = (zoom_percent / 25 + 1) * 25;
	if(zoom_percent > 400)
		zoom_percent = 25;
	Sync();
}

void DiagramEditor::HorzMouseWheel(Point, int zdelta, dword)
{
	sb.WheelX(zdelta);
}

String DiagramEditor::Save() const
{
	StringBuffer r;
	data.Save(r);
	return String(r);
}

bool DiagramEditor::Load(const String& s)
{
	CParser p(s);
	try {
		data.Load(p);
	}
	catch(CParser::Error) {
		Reset();
		return false;
	}
	Reset();
	return true;
}

void DiagramEditor::Reset()
{
	doselection = false;
	grid = true;
	edit_text = false;
	ResetUndo();
	Sync();
}

bool DiagramEditor::Key(dword key, int count)
{
	switch(key) {
	case K_ESCAPE:
		if(tool >= 0) {
			tool = -1;
			SetBar();
			return true;
		}
		if(IsCursor()) {
			KillCursor();
			return true;
		}
		break;
	}
	return Ctrl::Key(key, count);
}

}