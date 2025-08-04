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
	Diagram clip;
	StringBuffer cb;
	for(int i : sel) {
		const DiagramItem& m = data.item[i];
		clip.item << m;
		clip.AddBlob(data.GetBlob(m.blob_id));
	}
	StringBuffer r;
	clip.Save(r);
	WriteClipboardText(String(r));
}

void DiagramEditor::Cut()
{
	Copy();
	Delete();
}

void DiagramEditor::Paste()
{
	if(IsClipboardAvailableImage()) {
		Image img = ReadClipboardImage();
		if(IsNull(img))
			return;
		Sizef sz = img.GetSize();
		int ii = data.item.GetCount();
		DiagramItem& m = data.item.Add();
		m.shape = DiagramItem::SHAPE_IMAGE;
		m.blob_id = data.AddBlob(PNGEncoder().SaveString(img));
		m.pt[0] = Rectf(Sizef(data.GetSize())).CenterPos(sz);
		m.pt[1] = m.pt[0] + sz;
		SetCursor(ii);
	}
	else {
		String txt = ReadClipboardText();
		Diagram clip;
		CParser p(txt);
		try {
			clip.Load(p);
		}
		catch(CParser::Error) {
		}
		for(DiagramItem& m : clip.item) {
			int ii = data.item.GetCount();
			data.item << m;
			data.AddBlob(clip.GetBlob(m.blob_id));
			SetCursor(ii);
		}
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
