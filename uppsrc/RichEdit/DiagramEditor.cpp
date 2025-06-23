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
	ink << [=] { SetAttrs(); };

	paper.ColorImage(DiagramImg::Paper())
	   .NullImage(DiagramImg::PaperNull())
	   .StaticImage(DiagramImg::PaperA());
	paper.Tip(t_("Background color"));
	paper << [=] { SetAttrs(); };
	
	int cy = GetStdFontCy();
	
	Size shape_sz = Size(DPI(24), cy);
	
	auto MakeImage = [=](DiagramItem& m) -> Image {
		ImagePainter iw(shape_sz);
		iw.Scale(DPI(1));
		iw.Clear();
		m.Paint(iw);
		return iw;
	};
	
	cy /= DPI(1);
	
	auto PopPaint = [=](Draw& w, const Image& m, bool sel) {
		int x = DPI(2);
		int y = DPI(1);
		if(sel)
			w.DrawImage(x, y, m, SColorHighlightText());
		else
			w.DrawImage(x, y, m);
	};

	shape_popup.count = DiagramItem::SHAPE_COUNT - 1;
	shape_popup.columns = 3;
	shape_popup.isz = shape_sz + Size(DPI(4), DPI(2));
	shape_popup.WhenPaintItem = [=](Draw& w, Size isz, int ii, bool sel) {
		PopPaint(w, ShapeIcon(ii + 1), sel);
	};
	shape_popup.WhenSelect = [=](int i) {
		mode = shape_i = i + 1;
		SetBar();
		SetAttrs();
	};

	start_cap.count = DiagramItem::CAP_COUNT;
	start_cap.columns = 3;
	start_cap.isz = shape_sz + Size(DPI(4), DPI(2));
	start_cap.WhenPaintItem = [=](Draw& w, Size isz, int ii, bool sel) {
		PopPaint(w, CapIcon(ii, 0), sel);
	};
	start_cap.WhenSelect = [=](int i) {
		line_start = i;
		SetBar();
		SetAttrs();
	};

	end_cap.count = DiagramItem::CAP_COUNT;
	end_cap.columns = 3;
	end_cap.isz = shape_sz + Size(DPI(4), DPI(2));
	end_cap.WhenPaintItem = [=](Draw& w, Size isz, int ii, bool sel) {
		PopPaint(w, CapIcon(0, ii), sel);
	};
	end_cap.WhenSelect = [=](int i) {
		line_end = i;
		SetBar();
		SetAttrs();
	};
	
	line_dash << [=] { SetAttrs(); };
	
	for(int i = 0; i < 10; i++)
		line_width.Add(i);
	line_width << [=] { SetAttrs(); };

	ResetUndo();
	Sync();
	
	sb.AutoHide();
	sb.WithSizeGrip();
	AddFrame(sb);
	sb.WhenScroll << [=] { Sync(); };
	
	editor = true;
}

Image DiagramEditor::MakeIcon(DiagramItem& m, Size isz)
{
	struct IconMaker : ImageMaker {
		Size         isz;
		DiagramItem& m;
		String Key() const override {
			return StoreAsString(m) + String((byte *)&isz, sizeof(isz));
		}
		Image Make() const override {
			ImagePainter iw(isz);
			iw.Clear();
			m.Paint(iw);
			return iw;
		}
		
		IconMaker(DiagramItem& m) : m(m) {}
	};
	
	IconMaker mk(m);
	mk.isz = isz;
	return MakeImage(mk);
}

Image DiagramEditor::ShapeIcon(int i)
{
	Size isz = IconSz();
	DiagramItem m;
	m.p1 = Point(2, 2);
	m.p2 = Point(isz.cx - 2, isz.cy - 2);
	m.width = DPI(1);
	m.shape = i;
	m.paper = Null;
	return MakeIcon(m, isz);
}

Image DiagramEditor::CapIcon(int start, int end)
{
	Size isz = IconSz();
	DiagramItem m;
	m.p1 = Point(DPI(4), isz.cy / 2);
	m.p2 = Point(isz.cx - DPI(4), isz.cy / 2);
	m.shape = 0;
	m.width = DPI(1);
	m.line_start = start;
	m.line_end = end;
	return MakeIcon(m, isz);
}

void DiagramEditor::Paint(Draw& w)
{
	RTIMING("Paint");
	DrawPainter iw(w, GetSize());
	iw.Co();
	iw.Clear(SWhite());

	iw.Scale(GetZoom());
	iw.Offset(-(Point)sb);
	Size dsz = data.GetSize();
	iw.Move(dsz.cx, 0).Line(dsz.cx, dsz.cy).Line(0, dsz.cy).Stroke(0.2, SColorHighlight());

	if(data.item.GetCount() == 0)
		iw.DrawText(DPI(30), DPI(30), "Right-click to insert item(s)", ArialZ(30).Italic(), SLtGray());

	data.Paint(iw, *this);
	
	if(HasCapture() && doselection) {
		Rect r(dragstart, dragcurrent);
		r.Normalize();
		iw.Rectangle(r).Fill(30 * SColorHighlight()).Stroke(1, LtRed()).Dash("4").Stroke(1, White());
	}
}

void DiagramEditor::Sync()
{
	Refresh();
	SetBar();
	sb.SetTotal(data.GetSize());
	sb.SetPage(GetSize() / GetZoom());
	sb.SetLine(8, 8);
	SyncEditor();
}

void DiagramEditor::Layout()
{
	Sync();
}

void DiagramEditor::ResetUndo()
{
	undoredo.Reset(GetCurrent());
}

void DiagramEditor::Commit()
{
	if(IsCursor()) {
		DiagramItem& m = CursorItem();
		if(!m.IsLine())
			m.Normalize();
	}
	if(undoredo.Commit(GetCurrent())) {
		SetBar();
		Sync();
	}
}

String DiagramEditor::GetCurrent()
{
	return StoreAsString(data);
}

bool DiagramEditor::SetCurrent(const String& s)
{
	bool b = LoadFromString(data, s);
	Sync();
	return b;
}

void DiagramEditor::CancelSelection()
{
	FinishText();
	sel.Clear();
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

void DiagramEditor::MouseWheel(Point, int zdelta, dword keyflags) {
	if(keyflags & K_CTRL) {
		zoom_percent = clamp((zoom_percent / 25 + sgn(zdelta)) * 25, 25, 400);
		Sync();
		return;
	}
	if(keyflags & K_SHIFT)
		sb.WheelX(zdelta);
	else
		sb.WheelY(zdelta);
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

}