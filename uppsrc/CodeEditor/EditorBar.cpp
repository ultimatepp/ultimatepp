#include "CodeEditor.h"

namespace Upp {

void Renumber(LineInfo& lf)
{
	LineInfo tf;
	int l = 0;
	if(lf.GetCount()) {
		LineInfoRecord& t = tf.Add();
		t.breakpoint = lf[0].breakpoint;
		t.lineno = 0;
		t.count = lf[0].count;
		t.firstedited = lf[0].firstedited;
		t.edited = lf[0].edited;
		l += t.count;
	}
	for(int i = 1; i < lf.GetCount(); i++) {
		LineInfoRecord& r = lf[i];
		if(r.breakpoint.IsEmpty() && r.edited == 0 &&
			tf.Top().breakpoint.IsEmpty() && tf.Top().edited == 0)
			tf.Top().count += r.count;
		else {
			LineInfoRecord& t = tf.Add();
			t.breakpoint = r.breakpoint;
			t.firstedited = r.firstedited;
			t.edited = r.edited;
			t.count = r.count;
			t.lineno = l;
		}
		l += r.count;
	}
	lf = pick(tf);
}

void ClearBreakpoints(LineInfo& lf)
{
	for(int i = 0; i < lf.GetCount(); i++)
		lf[i].breakpoint.Clear();
}

void ValidateBreakpoints(LineInfo& lf)
{
	for(int i = 0; i < lf.GetCount(); i++)
		if(lf[i].breakpoint[0] == 0xe)
			lf[i].breakpoint = "1";
}

void EditorBar::sPaintImage(Draw& w, int y, int fy, const Image& img)
{
	w.DrawImage(0, y + (fy - img.GetSize().cy) / 2, img);
}

void EditorBar::Paint(Draw& w)
{
	Color bg = Nvl(bg_color, GrayColor(IsDarkTheme() ? 70 : 247));
	Size sz = GetSize();
	w.DrawRect(0, 0, sz.cx, sz.cy, bg);
	for(int i = 0; i < animate.GetCount(); i++)
		w.DrawRect(i, 0, 1, sz.cy, animate[i]);
	w.DrawText(0, sz.cy - Zy(10), 900, text, Arial(DPI(20)).Italic(), text_color);
	if(!editor) return;
	for(int pass = 0; pass < 2; pass++) {
		int fy = editor->GetFontSize().cy;
		int hy = fy >> 1;
		int y = 0;
		int i = editor->GetScrollPos().y;
		int cy = GetSize().cy;
		String hl = editor->GetHighlight();
		bool hi_if = (hilite_if_endif && findarg(hl, "cpp", "cs", "java") >= 0);
		Vector<IfState> previf;
		if(hi_if)
			previf <<= editor->GetIfStack(i);
		int ptri[2];
		for(int q = 0; q < 2; q++)
			ptri[q] = ptrline[q] >= 0 ? GetLineNo(ptrline[q]) : -1;
		while(y < cy) {
			String b;
			int err = 0;
			int edit = 0;
			String ann;
			Image  icon;
			if(i < li.GetCount()) {
				const LnInfo& l = li[i];
				b = l.breakpoint;
				edit = l.edited;
				icon = l.icon;
				ann = l.annotation;
			}
			if(pass == 0) {
				if(editor->GetCaret().top == y && editor->barline)
					w.DrawRect(0, y, sz.cx, fy, Blend(SColorHighlight(), bg, 200));
				if(line_numbers && i < editor->GetLineCount()) {
					String n = AsString((i + 1) % 1000000);
					Font fnt = editor->GetFont();
					Size tsz = GetTextSize(n, fnt);
					w.DrawText(sz.cx - Zx(4 + 12) - tsz.cx, y + (fy - tsz.cy) / 2, n, fnt, SBrown());
				}
				if(hi_if) {
					Vector<IfState> nextif;
					if(i < li.GetCount())
						nextif <<= editor->GetIfStack(i + 1);
					int pifl = previf.GetCount(), nifl = nextif.GetCount();
					int dif = max(pifl, nifl);
					if(--dif >= 0) {
						char p = (dif < pifl ? previf[dif].state : 0);
						char n = (dif < nifl ? nextif[dif].state : 0);
						int wd = min(2 * (dif + 1), sz.cx);
						int x = sz.cx - wd;
						Color cn = EditorSyntax::IfColor(n);
						if(p == n)
							w.DrawRect(x, y, 1, fy, cn);
						else {
							Color cp = EditorSyntax::IfColor(p);
							w.DrawRect(x, y, 1, hy, cp);
							w.DrawRect(x, y + hy, wd, 1, Nvl(cn, cp));
							w.DrawRect(x, y + hy, 1, fy - hy, cn);
							if(--dif >= 0) {
								x = sz.cx - min(2 * (dif + 1), sz.cx);
								if(!p)
									w.DrawRect(x, y, 1, hy, EditorSyntax::IfColor(dif < pifl ? previf[dif].state : 0));
								if(!n)
									w.DrawRect(x, y + hy, 1, fy - hy, EditorSyntax::IfColor(dif < nifl ? nextif[dif].state : 0));
							}
						}
					}
					previf = pick(nextif);
				}
				if(editor->GetMarkLines()) {
					int width = CodeEditorImg::Breakpoint().GetWidth() >> 1;
					if(edit)
					{
						int age = (int)(log((double)(editor->GetUndoCount() + 1 - edit)) * 30);
						w.DrawRect(0, y, width, fy, Blend(SLtBlue(), bg, min(220, age)));
					}
					if(err)
						w.DrawRect(width, y, width, fy, err == 1 ? LtRed() : (err == 2 ? Color(255, 175, 0) : SGreen()));
				}
			}
			
			if(pass == 1) {
				if(!b.IsEmpty())
					sPaintImage(w, y, fy, b == "1"   ? CodeEditorImg::Breakpoint() :
					                      b == "\xe" ? CodeEditorImg::InvalidBreakpoint() :
					                                   CodeEditorImg::CondBreakpoint());
				for(int q = 0; q < 2; q++)
					if(ptri[q] == i)
						sPaintImage(w, y, fy, ptrimg[q]);
		
				if(annotations && !IsNull(icon))
					w.DrawImage(sz.cx - annotations, y + (fy - icon.GetSize().cy) / 2, icon);
			}
	
			y += fy;
			i++;
		}
		if(pass == 0)
			w.DrawImage(0, 0, status_image);
	}
}

void EditorBar::MouseMove(Point p, dword flags)
{
	int pa = active_annotation;
	if(p.x > GetSize().cx - annotations)
		active_annotation = p.y / editor->GetFont().Info().GetHeight() + editor->GetScrollPos().y;
	else
		active_annotation = -1;
	if(active_annotation >= editor->GetLineCount())
		active_annotation = -1;
	if(pa != active_annotation)
		WhenAnnotationMove();
	if(editor)
		editor->MouseMove(Point(0, p.y), flags);
}

void EditorBar::MouseLeave()
{
	int pa = active_annotation;
	active_annotation = -1;
	if(pa != active_annotation)
		WhenAnnotationMove();
}

void EditorBar::LeftDown(Point p, dword flags)
{
	if(p.x > GetSize().cx - annotations)
		WhenAnnotationClick();
	else
	if(editor) {
		editor->LeftDown(Point(0, p.y), flags);
		ReleaseCtrlCapture();
	}
}

String& EditorBar::PointBreak(int& y)
{
	y = minmax(y / editor->GetFont().Info().GetHeight()
		+ editor->GetScrollPos().y, 0, editor->GetLineCount());
	static String e;
	return y < 100000 ? li.At(y).breakpoint : e;
}

void EditorBar::LeftDouble(Point p, dword flags)
{
	if(!editor || !bingenabled) return;
	String& b = PointBreak(p.y);
	if(b.IsEmpty())
		b = "1";
	else
		b.Clear();
	WhenBreakpoint(p.y);
	Refresh();
}

void EditorBar::RightDown(Point p, dword flags)
{
	if(p.x > GetSize().cx - annotations)
		WhenAnnotationRightClick();
}

void EditorBar::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(editor) {
		int i = editor->GetScrollPos().y;
		editor->MouseWheel(p, zdelta, keyflags);
		if(i != editor->GetScrollPos().y)
			MouseMove(p, keyflags);
	}
}

void EditorBar::InsertLines(int i, int count)
{
	li.InsertN(minmax(i + 1, 0, li.GetCount()), max(count, 0));
	if(editor->GetCheckEdited()) {
		if(editor->IsUndoOp() && li_removed.GetCount() >= count) {
			for(int t = 0; t < count; t++) {
				li.At(i + t).firstedited = li_removed[li_removed.GetCount() - count + t].firstedited;
				li[i + t].edited = li_removed[li_removed.GetCount() - count + t].edited;
			}
			li_removed.Drop(count);
			SetEdited(i + count, 1);
			ignored_next_edit = true;
		}
		else {
			if (li[i].firstedited == 0) {
				bool fe = li[i].firstedited;
				li.At(i + count).firstedited = fe;
			}
			SetEdited(i + 1, count);
		}
	}
	Refresh();
}

void EditorBar::RemoveLines(int i, int count)
{
	if(editor->GetCheckEdited() && !editor->IsUndoOp()) {
		for(int t = i - 1; t < i + count - 1; t++) {
			LineInfoRemRecord& rm = li_removed.Add();
			rm.firstedited = li[t].firstedited;
			rm.edited = li[t].edited;
		}
		if(li.At(i + count - 1).firstedited)
			next_age = li[i + count - 1].firstedited;
		else
			next_age = editor->GetUndoCount();
	}
	i = minmax(i, 0, li.GetCount());
	li.Remove(i, minmax(count, 0, li.GetCount() - i));
	Refresh();
}

void EditorBar::ClearLines()
{
	li.Clear();
	li.Shrink();
	li_removed.Clear();
	li_removed.Shrink();
	Refresh();
}

LineInfo EditorBar::GetLineInfo() const
{
	LineInfo lf;
	int l = -2;
	for(int i = 0; i < li.GetCount(); i++) {
		const LnInfo& ln = li[i];
		if(!ln.breakpoint.IsEmpty() || ln.edited) {
			LineInfoRecord& r = lf.Add();
			r.lineno = ln.lineno;
			r.count = 1;
			r.breakpoint = ln.breakpoint;
			r.firstedited = ln.firstedited;
			r.edited = ln.edited;
			l = -2;
		}
		else
		if(ln.lineno != l) {
			LineInfoRecord& r = lf.Add();
			r.lineno = l = ln.lineno;
			r.count = 1;
		}
		else
			lf.Top().count++;
		if(l >= 0) l++;
	}
	return lf;
}

void EditorBar::SetLineInfo(const LineInfo& lf, int total)
{
	li.Clear();
	if(lf.GetCount() == 0) {
		for(int i = 0; i < total; i++)
			li.Add().lineno = i;
	}
	else {
		for(int i = 0; i < lf.GetCount() && (total < 0 || li.GetCount() < total); i++) {
			const LineInfoRecord& r = lf[i];
			int l = r.lineno;
			for(int j = r.count; j-- && li.GetCount() < total;) {
				LnInfo& ln = li.Add();
				ln.lineno = l;
				ln.breakpoint = r.breakpoint;
				ln.firstedited = r.firstedited;
				ln.edited = r.edited;
				if(l >= 0) l++;
			}
		}
		while(li.GetCount() < total)
			li.Add().lineno = -1;
	}
}

void EditorBar::Renumber(int linecount)
{
	li.SetCount(linecount);
	for(int i = 0; i < linecount; i++)
		li[i].lineno = i;
}

void EditorBar::ClearBreakpoints()
{
	for(int i = 0; i < li.GetCount(); i++)
		li[i].breakpoint.Clear();
	Refresh();
}

void EditorBar::ValidateBreakpoints()
{
	for(int i = 0; i < li.GetCount(); i++)
		if(li[i].breakpoint[0] == 0xe)
			li[i].breakpoint = "1";
	Refresh();
}

String EditorBar::GetBreakpoint(int ln)
{
	return ln < li.GetCount() ? li[ln].breakpoint : Null;
}

void EditorBar::ClearAnnotations()
{
	for(int i = 0; i < li.GetCount(); i++) {
		li[i].icon.Clear();
		li[i].annotation.Clear();
	}
}

void EditorBar::SetAnnotation(int line, const Image& img, const String& ann)
{
	if(line >= 0 && line < li.GetCount()) {
		li[line].icon = img;
		li[line].annotation = ann;
	}
	Refresh();
}

String EditorBar::GetAnnotation(int line) const
{
	return line >= 0 && line < li.GetCount() ? li[line].annotation : String();
}

void EditorBar::SetBreakpoint(int ln, const String& s)
{
	li.At(ln).breakpoint = s;
	WhenBreakpoint(ln);
}

void EditorBar::SetEdited(int ln, int count)
{
	if(ignored_next_edit) {
		ignored_next_edit = false;
		return;
	}
	int age = editor->GetUndoCount() + 1;
	bool undo = editor->IsUndoOp();
	for(int i = 0; i < count; i++) {
		if(undo) {
			if (li.At(ln + i).firstedited >= age - 1) {
				li[ln + i].firstedited = 0;
				li[ln + i].edited = 0;
			}
		}
		else {
			if(next_age) {
				li[ln + i].firstedited = next_age;
				li[ln + i].edited = age;
				next_age = 0;
			}
			else {
				if(li.At(ln + i).firstedited == 0)
					li[ln + i].firstedited = age;
				li[ln + i].edited = age;
			}
		}
	}
	Refresh();
}

void EditorBar::ClearEdited()
{
	for(int i = 0; i < li.GetCount(); i++) {
		li.At(i).firstedited = 0;
		li[i].edited = 0;
	}
	li_removed.Clear();
	li_removed.Shrink();
	Refresh();
}

int  EditorBar::GetLineNo(int lineno) const {
	for(int i = 0; i < li.GetCount(); i++) {
		if(lineno <= li[i].lineno)
			return i;
	}
	return lineno;
}

int  EditorBar::GetNoLine(int line) const {
	int n = 0;
	for(int i = 0; i < li.GetCount(); i++) {
		if(li[i].lineno >= 0)
			n = li[i].lineno;
		if(i == line) return n;
	}
	return n;
}

void EditorBar::SetPtr(int line, const Image& img, int i)
{
	ASSERT(i >= 0 && i < 2);
	ptrline[i] = line;
	ptrimg[i] = img;
	Refresh();
}

void EditorBar::HidePtr()
{
	ptrline[0] = ptrline[1] = -1;
	Refresh();
}

void EditorBar::SyncSize()
{
	int n = editor ? editor->GetLineCount() : 0;
	int i = 0;
	while(n) {
		i++;
		n /= 10;
	}
	int w = max(DPI(32), (line_numbers && editor ? editor->GetFont()['0'] * i : 0) + Zx(12 + 4) + annotations);
	if(w != GetWidth())
		Width(w);
	Refresh();
}

void EditorBar::LineNumbers(bool b)
{
	line_numbers = b;
	SyncSize();
}

void EditorBar::Annotations(int width)
{
	annotations = width;
	SyncSize();
}

EditorBar::EditorBar()
{
	editor = NULL;
	line_numbers = false;
	bingenabled = true;
	hilite_if_endif = true;
	line_numbers = false;
	annotations = 0;
	ignored_next_edit = false;
	next_age = 0;
	active_annotation = -1;
	SyncSize();
}

void EditorBar::StatusImage(const Image& m)
{
	if(status_image.IsSame(m))
		return;
	status_image = m;
	Refresh();
}

EditorBar::~EditorBar()
{
}

}
