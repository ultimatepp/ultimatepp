#include "IconDes.h"

bool IsSelection(const Image& m) {
	const RGBA *s = m;
	const RGBA *e = s + m.GetLength();
	while(s < e) {
		if(s->r != 255)
			return true;
		s++;
	}
	return false;
}

IconDes::EditPos IconDes::GetEditPos()
{
	EditPos e;
	e.cursor = ilist.GetKey();
	e.sc = ilist.GetScroll();
	e.slot.Clear();
	e.slot.SetCount(slot.GetCount());
	for(int i = 0; i < slot.GetCount(); i++) {
		EditPosSlot& es = e.slot[i];
		Slot& c = slot[i];
		if(IsSelection(c.selection))
			es.selection = StoreImageAsString(c.selection);
		es.undo = c.undo;
		es.redo = c.redo;
	}
	return e;
}

void IconDes::SetEditPos(const EditPos& e)
{
	if(e.slot.GetCount() == slot.GetCount()) {
		for(int i = 0; i < slot.GetCount(); i++) {
			const EditPosSlot& es = e.slot[i];
			Slot& c = slot[i];
			Image sel = LoadImageFromString(es.selection);
			if(sel.GetSize() == c.image.GetSize())
				c.selection = sel;
			else
				c.selection.Clear();
			c.undo = es.undo;
			c.redo = es.redo;
		}
		if(e.cursor >= 0 && e.cursor < slot.GetCount()) {
			ilist.ScrollTo(e.sc);
			GoTo(e.cursor);
		}
	}
	SyncImage();
}
