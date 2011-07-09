#include "RichEdit.h"

NAMESPACE_UPP

void RichEdit::CancelMode()
{
	tabmove.table = 0;
	selclick = false;
	dropcaret.Clear();
}

void RichEdit::MouseWheel(Point p, int zdelta, dword keyflags)
{
	sb.Wheel(zdelta);
}

RichHotPos RichEdit::GetHotPos(Point p)
{
	int x;
	PageY py;
	GetPagePoint(p, py, x);
	return text.GetHotPos(x, py, 4 / GetZoom(), pagesz);
}

void RichEdit::SetObjectPercent(int p)
{
	if(objectpos >= 0) {
		RichObject obj = GetObject();
		obj.SetSize(obj.GetPhysicalSize() * p / 100);
		obj.KeepRatio(true);
		ReplaceObject(obj);
	}
}

void RichEdit::SetObjectYDelta(int pt)
{
	if(objectpos >= 0) {
		RichObject obj = GetObject();
		obj.SetYDelta(pt * 25 / 3);
		ReplaceObject(obj);
	}
}

void RichEdit::SetObjectPos(int pos)
{
	Rect r = GetObjectRect(cursor);
	Rect rr = r.Offseted(GetTextRect().left, -sb);
	objectrect = GetObjectRect(pos);
	objectpos = cursor;
	PageRect pr = text.GetCaret(cursor, pagesz);
	PlaceCaret();
	Refresh(rr);
	ReadFormat();
}

void RichEdit::LeftDown(Point p, dword flags)
{
	useraction = true;
	NextUndo();
	SetFocus();
	selclick = false;
	tabmove = GetHotPos(p);
	if(tabmove.table && tabmove.column >= -2) {
		SaveTableFormat(tabmove.table);
		SetCapture();
		Move(text.GetCellPos(tabmove.table, 0, max(tabmove.column, 0)).pos);
		return;
	}
	int c = GetHotSpot(p);
	if(c >= 0 && objectpos >= 0) {
		int pos = objectpos;
		RectTracker tracker(*this);
		RichObject obj = text.GetRichPos(pos).object;
		tracker.MinSize(Size(16, 16))
		       .MaxSize(GetZoom() * pagesz)
		       .Animation()
		       .Dashed()
		       .KeepRatio(obj.IsKeepRatio());
		int tx, ty;
		switch(c) {
		case 1:
			tracker.SetCursorImage(Image::SizeVert());
			tx = ALIGN_CENTER; ty = ALIGN_BOTTOM;
			break;
		case 2:
			tracker.SetCursorImage(Image::SizeHorz());
			tx = ALIGN_RIGHT; ty = ALIGN_CENTER;
			break;
		default:
			tracker.SetCursorImage(Image::SizeBottomRight());
			tx = ALIGN_RIGHT; ty = ALIGN_RIGHT;
			break;
		}
		double zoom = GetZoom().AsDouble();
		Size sz = obj.GetSize();
		sz.cx = int(zoom * sz.cx + 0.5);
		sz.cy = int(zoom * sz.cy + 0.5);
		sz = tracker.Track(Rect(objectrect.Offseted(GetTextRect().left, -sb).TopLeft(), sz), tx, ty).Size();
		sz.cx = int(sz.cx / zoom + 0.5);
		sz.cy = int(sz.cy / zoom + 0.5);
		obj.SetSize(sz);
		ReplaceObject(obj);
	}
	else {
		c = GetMousePos(p);
		if(c >= 0) {
			if(InSelection(c)) {
				selclick = true;
				return;
			}
			Move(c, flags & K_SHIFT);
			mpos = c;
			SetCapture();
			if(cursorp.object && GetObjectRect(cursor).Offseted(GetTextRect().left, -sb).Contains(p))
				SetObjectPos(cursor);
		}
	}
}

void RichEdit::LeftUp(Point p, dword flags)
{
	useraction = true;
	NextUndo();
	int c = GetMousePos(p);
	int d = c;
	if(!HasCapture() && InSelection(d) && selclick) {
		CancelSelection();
		Move(c);
	}
	selclick = false;
}

void RichEdit::MouseMove(Point p, dword flags)
{
	useraction = true;
	if(HasCapture() && (flags & K_MOUSELEFT)) {
		if(tabmove.table && tabmove.column >= 0) {
			RichTable::Format fmt = text.GetTableFormat(tabmove.table);
			if(tabmove.column >= fmt.column.GetCount() - 1)
				return;
			int sum = Sum0(fmt.column);
			int nl = 0;
			for(int i = 0; i < tabmove.column; i++)
				nl += fmt.column[i];
			int xn = fmt.column[tabmove.column] + fmt.column[tabmove.column + 1];
			int xl = tabmove.left + tabmove.cx * nl / sum + 120;
			int xh = tabmove.left + tabmove.cx * (nl + xn) / sum - 120;
			if(xl >= xh)
				return;
			int xx = minmax(GetSnapX(p.x) - tabmove.delta, xl, xh) - tabmove.left;
			fmt.column[tabmove.column] = xx * sum / tabmove.cx - nl;
			fmt.column[tabmove.column + 1] = xn - fmt.column[tabmove.column];
			text.SetTableFormat(tabmove.table, fmt);
			Finish();
		}
		else
		if(tabmove.table && tabmove.column == -1) {
			RichTable::Format fmt = text.GetTableFormat(tabmove.table);
			fmt.lm = minmax(GetSnapX(p.x) - tabmove.textleft, 0, max(tabmove.textcx - fmt.rm - 120, 0));
			text.SetTableFormat(tabmove.table, fmt);
			Finish();
		}
		else
		if(tabmove.table && tabmove.column == -2) {
			RichTable::Format fmt = text.GetTableFormat(tabmove.table);
			fmt.rm = minmax(tabmove.textcx - GetSnapX(p.x) + tabmove.textleft, 0, tabmove.textcx - fmt.lm - 120);
			text.SetTableFormat(tabmove.table, fmt);
			Finish();
		}
		else {
			PageY py = GetPageY(p.y + sb);
			int c;
			if(py > text.GetHeight(pagesz))
				c = GetLength() + 1;
			else
				c = GetNearestPos(GetX(p.x), py);
			if(c != mpos) {
				mpos = -1;
				Move(c, true);
			}
		}
	}
}

void RichEdit::RightDown(Point p, dword flags)
{
	useraction = true;
	NextUndo();
	MenuBar menu;
	int l, h;
	Rect ocr = GetCaretRect();
	int c = GetMousePos(p);
	int fieldpos = -1;
	Id field;
	String fieldparam;
	String ofieldparam;
	RichObject object, o;
	if(GetSelection(l, h)) {
		if(c >= l && c < h) {
			CopyTool(menu);
			CutTool(menu);
		}
		PasteTool(menu);
	}
	else {
		LeftDown(p, flags);
		if(objectpos >= 0) {
			object = GetObject();
			if(!object) return;
			o = object;
			o.Menu(menu, context);
			if(!menu.IsEmpty())
				menu.Separator();
			menu.Add(t_("Object position.."), THISBACK(AdjustObjectSize));
			menu.Separator();
			menu.Add("20 %", THISBACK1(SetObjectPercent, 20));
			menu.Add("40 %", THISBACK1(SetObjectPercent, 40));
			menu.Add("60 %", THISBACK1(SetObjectPercent, 60));
			menu.Add("80 %", THISBACK1(SetObjectPercent, 80));
			menu.Add("90 %", THISBACK1(SetObjectPercent, 90));
			menu.Add("100 %", THISBACK1(SetObjectPercent, 100));
			menu.Break();
			menu.Add(t_("3 pt up"), THISBACK1(SetObjectYDelta, -3));
			menu.Add(t_("2 pt up"), THISBACK1(SetObjectYDelta, -2));
			menu.Add(t_("1 pt up"), THISBACK1(SetObjectYDelta, -1));
			menu.Add(t_("Baseline"), THISBACK1(SetObjectYDelta, 0));
			menu.Add(t_("1 pt down"), THISBACK1(SetObjectYDelta, 1));
			menu.Add(t_("2 pt down"), THISBACK1(SetObjectYDelta, 2));
			menu.Add(t_("3 pt down"), THISBACK1(SetObjectYDelta, 3));
			menu.Separator();
			CopyTool(menu);
			CutTool(menu);
		}
		else {
			RichPos p = cursorp;
			field = p.field;
			fieldparam = p.fieldparam;
			RichPara::FieldType *ft = RichPara::fieldtype().Get(field, NULL);
			if(ft) {
				ofieldparam = fieldparam;
				fieldpos = cursor;
				ft->Menu(menu, &fieldparam);
				if(!menu.IsEmpty())
					menu.Separator();
				CopyTool(menu);
				CutTool(menu);
			}
			else {
				WString w = GetWordAtCursor();
				if(!w.IsEmpty() && !SpellWord(w, w.GetLength(),
				                              fixedlang ? fixedlang : formatinfo.language)) {
					menu.Add(t_("Add to user dictionary"), THISBACK(AddUserDict));
					menu.Separator();
				}
				PasteTool(menu);
				ObjectTool(menu);
			}
		}
		LoadImageTool(menu);
	}
	Rect r = GetCaretRect();
	Refresh(r);
	Refresh(ocr);
	paintcarect = true;
	menu.Execute();
	paintcarect = false;
	Refresh(r);
	if(object && o && o.GetSerialId() != object.GetSerialId())
		ReplaceObject(o);
	if(fieldpos >= 0 && fieldparam != ofieldparam) {
		RichText::FormatInfo f = text.GetFormatInfo(fieldpos, 1);
		Remove(fieldpos, 1);
		RichPara p;
		p.Cat(field, fieldparam, f);
		RichText clip;
		clip.Cat(p);
		Insert(fieldpos, clip, false);
		Finish();
	}
}

void RichEdit::LeftDouble(Point p, dword flags)
{
	NextUndo();
	int c = GetMousePos(p);
	if(c >= 0) {
		if(objectpos == c) {
			RichObject object = GetObject();
			if(!object) return;
			RichObject o = object;
			o.DefaultAction(context);
			if(object.GetSerialId() != o.GetSerialId())
				ReplaceObject(o);
		}
		else {
			RichPos rp = cursorp;
			RichPara::FieldType *ft = RichPara::fieldtype().Get(rp.field, NULL);
			if(ft) {
				int fieldpos = cursor;
				ft->DefaultAction(&rp.fieldparam);
				RichText::FormatInfo f = text.GetFormatInfo(fieldpos, 1);
				Remove(fieldpos, 1);
				RichPara p;
				p.Cat(rp.field, rp.fieldparam, f);
				RichText clip;
				clip.Cat(p);
				Insert(fieldpos, clip, false);
				Finish();
			}
			else {
				int l, h;
				if(GetWordSelection(c, l, h))
					SetSelection(l, h);
			}
		}
	}
}

void RichEdit::LeftTriple(Point p, dword flags)
{
	NextUndo();
	int c = GetMousePos(p);
	if(c >= 0 && c != objectpos) {
		RichPos rp = text.GetRichPos(c);
		Select(c - rp.posinpara, rp.paralen + 1);
	}
}

Image RichEdit::CursorImage(Point p, dword flags)
{
	if(tablesel)
		return Image::Arrow();

	switch(GetHotSpot(p)) {
	case 0:
		return Image::SizeBottomRight();
	case 1:
		return Image::SizeVert();
	case 2:
		return Image::SizeHorz();
	default:
		if(text.GetRichPos(GetMousePos(p)).object) {
			return Image::Arrow();
		}
		else
		if(HasCapture() && tabmove.table && tabmove.column >= -2)
			return Image::SizeHorz();
		else {
			RichHotPos hp = GetHotPos(p);
			if(hp.table > 0)
				return Image::SizeHorz();
			else {
				int c = GetMousePos(p);
				return InSelection(c) && !HasCapture() ? Image::Arrow() : CtrlImg::ibeam0;
			}
		}
	}
	return Image::Arrow();
}

void RichEdit::LeftRepeat(Point p, dword flags)
{
	NextUndo();
	if(HasCapture() && (flags & K_MOUSELEFT)) {
		if(p.y < 0)
			MoveUpDown(-1, true);
		if(p.y > GetSize().cy)
			MoveUpDown(1, true);
	}
}

END_UPP_NAMESPACE
