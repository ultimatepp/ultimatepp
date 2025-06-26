#include "RichEdit.h"

namespace Upp {

void DiagramEditor::Delete()
{
	FinishText();
	if(!IsCursor())
		return;
	Vector<int> h = clone(sel.GetKeys());
	Sort(h);
	data.item.Remove(h);
	CancelSelection();
	Commit();
}

void DiagramEditor::Copy()
{
	StringBuffer cb;
	for(int i : sel)
		data.item[i].Save(cb);
	WriteClipboardText(cb);
}

void DiagramEditor::Cut()
{
	Copy();
	Delete();
}

void DiagramEditor::Paste()
{
	if(IsClipboardAvailable("dib")) {
		data.img = ReadClipboardImage();
		data.img_hd = IsUHDMode();
		Commit();
	}
	else {
		String txt = ReadClipboardText();
		try {
			CParser p(txt);
			while(!p.IsEof()) {
				DiagramItem m;
				m.Load(p);
				int ii = data.item.GetCount();
				data.item << m;
				SetCursor(ii);
			}
		}
		catch(CParser::Error) {}
	}
	Commit();
	SetBar();
	Sync();
}

void DiagramEditor::Duplicate()
{
	if(!IsCursor())
		return;

	Rect r = CursorItem().GetRect();
	for(int i : sel)
		r.Union(data.item[i].GetRect());

	Size sz = r.GetSize();
	Point offset = sz.cx > sz.cy ? Point(0, sz.cy + 16) : Point(sz.cx + 16, 0);
	
	int p = data.item.GetCount();
	int q = p;
	data.item.SetCount(data.item.GetCount() + sel.GetCount());
	for(int i : sel) {
		DiagramItem& m = data.item[q++];
		m = data.item[i];
		m.pt[0] += offset;
		m.pt[1] += offset;
	}
	sel.Clear();
	for(int i = p; i < data.item.GetCount(); i++) {
		sel << i;
		cursor = i;
	}
	
	Commit();
	Sync();
}

void DiagramEditor::SelectAll()
{
	sel.Clear();
	for(int i = 0; i < data.item.GetCount(); i++) {
		sel.Add(i);
		cursor = i;
	}
	Sync();
}

}
