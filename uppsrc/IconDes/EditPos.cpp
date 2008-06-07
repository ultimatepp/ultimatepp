#include "IconDes.h"

NAMESPACE_UPP

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
	e.cursor = list.GetCursor();
	e.sc = list.GetCursorSc();
	e.slot.Clear();
	e.slot.SetCount(slot.GetCount());
	for(int i = 0; i < slot.GetCount(); i++) {
		EditPosSlot& es = e.slot[i];
		Slot& c = slot[i];
		if(IsSelection(c.selection))
			es.selection = StoreImageAsString(c.selection);
		es.undo = c.undo;
		es.redo = c.redo;
		es.supersampling = c.supersampling;
		if(c.supersampling)
			es.supersample = StoreImageAsString(c.image);
	}
	return e;
}

void IconDes::SetEditPos(const EditPos& e)
{
	if(e.slot.GetCount() == slot.GetCount()) {
		for(int i = 0; i < slot.GetCount(); i++) {
			const EditPosSlot& es = e.slot[i];
			Slot& c = slot[i];
			c.supersampling = es.supersampling;
			if(c.supersampling)
				c.image = LoadImageFromString(es.supersample);
			Image sel = LoadImageFromString(es.selection);
			if(sel.GetSize() == c.image.GetSize())
				c.selection = sel;
			else
				c.selection.Clear();
			c.undo = es.undo;
			c.redo = es.redo;
		}
		if(e.cursor >= 0 && e.cursor < list.GetCount()) {
			list.SetCursor(e.cursor);
			list.ScCursor(e.sc);
		}
	}
	list.ScrollIntoCursor();
	SyncImage();
}

END_UPP_NAMESPACE
