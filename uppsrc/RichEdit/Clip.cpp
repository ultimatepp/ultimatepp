#include "RichEdit.h"

NAMESPACE_UPP

#define RTFS "Rich Text Format;text/rtf;application/rtf"

void RichEdit::InsertImage()
{
	if(!imagefs.ExecuteOpen(t_("Open image from file")))
		return;
	String fn = ~imagefs;
	if(GetFileLength(fn) > 17000000) {
		Exclamation("Image is too large!");
		return;
	}
	String data = LoadFile(fn);
	StringStream ss(data);
	if(!StreamRaster::OpenAny(ss)) {
		Exclamation(NFormat(t_("Unsupported image format in file [* \1%s\1]."), ~imagefs));
		return;
	}
	RichText clip;
	RichPara p;
	p.Cat(CreateRawImageObject(data), formatinfo);
	clip.Cat(p);
	ClipPaste(clip);
}

bool RichEdit::Accept(PasteClip& d, RichText& clip)
{
	if(IsReadOnly())
		return false;
	for(int i = 0; i < RichObject::GetTypeCount(); i++) {
		RichObjectType& rt = RichObject::GetType(i);
		if(rt.Accept(d)) {
			Value data = rt.Read(d);
			if(!IsNull(data)) {
				RichPara p;
				p.Cat(RichObject(&rt, data, pagesz), formatinfo);
				clip.Cat(p);
			}
			return true;
		}
	}
	if(AcceptFiles(d)) {
		Vector<String> s = GetFiles(d);
		if(s.GetCount()) {
			String fn = s[0];
			String ext = ToUpper(GetFileExt(fn));
			if(ext == ".PNG" || ext == ".JPG" || ext == ".JPEG" || ext == ".GIF" || ext == ".TIF" || ext == ".TIFF") {
				if(d.Accept()) {
					if(StreamRaster::LoadFileAny(fn)) {
						RichPara p;
						p.Cat(CreateRawImageObject(LoadFile(fn)), formatinfo);
						clip.Cat(p);
					}
					return true;
				}
				return false;
			}
		}
		d.Reject();
	}
	if(d.Accept("text/QTF"))
		clip = ParseQTF(~d, 0, context);
	else
	if(d.Accept(RTFS))
		clip = ParseRTF(~d);
	else
	if(AcceptText(d))
		clip = AsRichText(GetWString(d), formatinfo);
	else
		return false;
	return true;
}

void RichEdit::ClipPaste(RichText& clip)
{
	clip.ApplyZoom(clipzoom.Reciprocal());
	Filter(clip);
	NextUndo();
	if(clip.GetPartCount() == 1 && clip.IsTable(0)) {
		CancelSelection();
		if(cursorp.table) {
			NextUndo();
			SaveTable(cursorp.table);
			text.PasteTable(cursorp.table, cursorp.cell, clip.GetTable(0));
			Finish();
			return;
		}
	}
	clip.Normalize();
	PasteText(clip);
}

void RichEdit::DragAndDrop(Point p, PasteClip& d)
{
	int dropcursor = GetMousePos(p);
	if(dropcursor >= 0) {
		RichText clip;
		if(Accept(d, clip)) {
			NextUndo();
			int a = sb;
			int c = dropcursor;
			if(InSelection(c)) {
				if(!IsReadOnly())
					RemoveSelection();
				if(IsDragAndDropSource())
					d.SetAction(DND_COPY);
			}
			int sell, selh;
			if(GetSelection(sell, selh) && d.GetAction() == DND_MOVE && IsDragAndDropSource()) {
				if(c > sell)
					c -= selh - sell;
				if(!IsReadOnly())
					RemoveSelection();
				d.SetAction(DND_COPY);
			}
			Move(c);
			clip.Normalize();
			ClipPaste(clip);
			sb = a;
			Select(c, clip.GetLength());
			SetFocus();
			Action();
			return;
		}
	}
	if(!d.IsAccepted())
		dropcursor = -1;
	Rect r = Null;
	if(dropcursor >= 0 && dropcursor < text.GetLength()) {
		RichCaret pr = text.GetCaret(dropcursor, pagesz);
		Zoom zoom = GetZoom();
		Rect tr = GetTextRect();
		r = RectC(pr.left * zoom + tr.left - 1,
		          GetPosY(pr) + (pr.lineascent - pr.caretascent) * zoom,
		          2, (pr.caretascent + pr.caretdescent) * zoom);
	}
	if(r != dropcaret) {
		RefreshDropCaret();
		dropcaret = r;
		RefreshDropCaret();
	}
}

void RichEdit::DragRepeat(Point p)
{
	sb = (int)sb + GetDragScroll(this, p, 16).y;
}

void RichEdit::RefreshDropCaret()
{
	Refresh(dropcaret.OffsetedVert(-sb));
}

void RichEdit::Paste()
{
	if(IsReadOnly())
		return;
	RichText clip;
	PasteClip d = Clipboard();
	if(!Accept(d, clip))
		return;
	ClipPaste(clip);
}

void RichEdit::DragLeave()
{
	RefreshDropCaret();
	dropcaret.Clear();
}

static String sRTF(const Value& data)
{
	const RichText& txt = ValueTo<RichText>(data);
	return EncodeRTF(txt);
}

static String sQTF(const Value& data)
{
	const RichText& txt = ValueTo<RichText>(data);
	return AsQTF(txt);
}

void RichEdit::ZoomClip(RichText& text) const
{
	text.ApplyZoom(clipzoom);
}

void RichEdit::Copy()
{
	RichText txt;
	if(IsSelection())
		txt = GetSelection();
	else if(objectpos >= 0)
		txt = text.Copy(cursor, 1);
	else {
		BeepExclamation();
		return;
	}
	ZoomClip(txt);
	ClearClipboard();
	AppendClipboardUnicodeText(txt.GetPlainText());
	Value clip = RawPickToValue(pick(txt));
	AppendClipboard("text/QTF", clip, sQTF);
	AppendClipboard(RTFS, clip, sRTF);
	if(objectpos >= 0) {
		RichObject o = GetObject();
		Vector<String> v = Split(o.GetType().GetClipFmts(), ';');
		for(int i = 0; i < v.GetCount(); i++)
			AppendClipboard(v[i], o.GetType().GetClip(o.GetData(), v[i]));
	}
}

String RichEdit::GetSelectionData(const String& fmt) const
{
	String f = fmt;
	if(IsSelection()) {
		RichText clip = GetSelection();
		ZoomClip(clip);
		if(f == "text/QTF")
			return AsQTF(clip);
		if(InScList(f, RTFS))
			return EncodeRTF(clip);
		return GetTextClip(clip.GetPlainText(), fmt);
	}
/*	else
	if(objectpos >= 0) {
		RichObject o = GetObject();
		if(InScList(fmt, o.GetType().GetClipFmts()))
			return o.GetType().GetClip(o.GetData(), fmt);
	}*/
	return Null;
}

void RichEdit::LeftDrag(Point p, dword flags)
{
	int c = GetMousePos(p);
	Size ssz = StdSampleSize();
	if(!HasCapture() && InSelection(c)) {
		RichText sample = GetSelection(5000);
		sample.ApplyZoom(Zoom(1, 8));
		ImageDraw iw(ssz);
		iw.DrawRect(0, 0, ssz.cx, ssz.cy, White);
		sample.Paint(iw, 0, 0, 128);
		NextUndo();
		if(DoDragAndDrop(String().Cat() << "text/QTF;" RTFS ";" << ClipFmtsText(),
		                 ColorMask(iw, White)) == DND_MOVE && !IsReadOnly()) {
			RemoveSelection();
			Action();
		}
	}
/*	else
	if(objectpos >= 0 && c == objectpos) {
		ReleaseCapture();
		RichObject o = GetObject();
		Size sz = o.GetPhysicalSize();
		NextUndo();
		if(DoDragAndDrop(o.GetType().GetClipFmts(),
		                 o.ToImage(Size(ssz.cx, sz.cy * ssz.cx / sz.cx))) == DND_MOVE
		   && objectpos >= 0) {
			if(droppos > objectpos)
				droppos--;
			Remove(objectpos, 1);
		}
		Move(droppos);
		SetObjectPos(droppos);
	}*/
}

void  RichEdit::MiddleDown(Point p, dword flags)
{
	RichText clip;
	if(IsReadOnly())
		return;
	if(Accept(Selection(), clip)) {
		selclick = false;
		LeftDown(p, flags);
		ClipPaste(clip);
	}
}

END_UPP_NAMESPACE
