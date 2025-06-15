#include "RichEdit.h"

namespace Upp {

void DiagramEditor::SetCursor(int i)
{
	cursor = i;
	if(i < 0) {
		ink <<= Black();
		paper <<= Black();
	}
	else
		sel.FindAdd(i);
	GetAttrs();
}

DiagramItem& DiagramEditor::CursorItem()
{
	static DiagramItem nil;
	if(cursor >= 0 && cursor < data.item.GetCount())
		return data.item[cursor];
	return nil;
}

}