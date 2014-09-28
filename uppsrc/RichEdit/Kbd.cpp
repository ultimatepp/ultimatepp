#include "RichEdit.h"

NAMESPACE_UPP

bool RichEdit::Key(dword key, int count)
{
	useraction = true;
	NextUndo();
	if(CursorKey(key, count))
		return true;
	if(IsReadOnly())
		return false;
	switch(key) {
	case K_CTRL_BACKSPACE:
		if(RemoveSelection(true)) return true;
		if(cursor > 0 && IsW(text[cursor - 1])) {
			int c = cursor;
			ReadFormat();
			MoveWordLeft(false);
			if(InvalidRange(cursor, c))
				return true;
			Remove(cursor, c - cursor);
			objectpos = -1;
			FinishNF();
			return true;
		}
	case K_BACKSPACE:
		if(RemoveSelection(true)) return true;
		if(cursor <= 0 || RemoveSpecial(cursor, cursor - 1, true))
			return true;
		anchor = --cursor;
		begtabsel = false;
		Remove(cursor, 1);
		break;
	case K_DELETE:
		if(RemoveSelection()) return true;
		if(cursor < text.GetLength() && !RemoveSpecial(cursor, cursor + 1, false))
			Remove(cursor, 1, true);
		break;
	case K_INSERT:
		overwrite = !overwrite;
		PlaceCaret();
		break;
	case K_CTRL_DELETE:
		if(RemoveSelection()) return true;
		if(cursor < text.GetLength()) {
			int c = cursor;
			if(IsW(text[c]))
				MoveWordRight(false);
			else
				cursor++;
			if(InvalidRange(cursor, c))
				return true;
			Remove(c, cursor - c);
			cursor = anchor = c;
			begtabsel = false;
			break;
		}
		break;
	case K_CTRL_Z:
		Undo();
		return true;
	case K_SHIFT_CTRL_Z:
		Redo();
		return true;
	case K_ENTER: {
			if(singleline)
				return false;
			if(!RemoveSelection() && InsertLineSpecial())
				return true;
			
			RichText::FormatInfo f = formatinfo;
			InsertLine();
			formatinfo = f;
			ShowFormat();
			FinishNF();
		}
		return true;
	case K_CTRL_ENTER:
		{
			int c = GetCursor(), l = GetLength();
			RichObject object;
			while(c < l) {
				RichPos p = text.GetRichPos(c);
				object = p.object;
				if(object || p.chr > ' ')
					break;
				c++;
			}
			if(object) {
				NextUndo();
				objectpos = c;
				RichObject o = object;
				o.DefaultAction(context);
				if(o.GetSerialId() != object.GetSerialId())
					ReplaceObject(o);
				return true;
			}
		}
		return false;
	case K_F9:
		EvaluateFields();
		break;
	case K_F3:
		Find();
		break;
	case K_CTRL_H:
		Hyperlink();
		break;
	case K_CTRL_Q:
		IndexEntry();
		break;
	case K_ESCAPE:
		CloseFindReplace();
		return false;
	case K_CTRL_C:
	case K_CTRL_INSERT:
		Copy();
		return true;
	case K_CTRL_X:
	case K_SHIFT_DELETE:
		Cut();
		return true;
	case K_CTRL_V:
	case K_SHIFT_INSERT:
		Paste();
		return true;
	case K_SHIFT_CTRL_SPACE:
	case K_CTRL_SPACE:
		key = 160;
	case K_TAB:
		if(cursorp.table && cursorp.posintab == cursorp.tablen) {
			TableInsertRow();
			return true;
		}
		if(cursorp.table && cursorp.posincell == cursorp.celllen) {
			cursor = anchor = cursor + 1;
			begtabsel = false;
			break;
		}
	default:
		if(key >= K_ALT_0 && key <= K_ALT_9) {
			ApplyStyleKey(key - K_ALT_0);
			return true;
		}
		if(key >= (K_SHIFT|K_ALT_0) && key <= (K_SHIFT|K_ALT_9)) {
			ApplyStyleKey(key - (K_SHIFT|K_ALT_0) + 10);
			return true;
		}
		if(key == K_SHIFT_SPACE)
			key = ' ';
		if(key == 9 || key >= 32 && key < 65536) {
			RichPara::Format f;
			if(IsSelection()) {
				f = text.GetRichPos(min(cursor, anchor)).format;
				RemoveSelection();
			}
			else
				f = formatinfo;
			RichPara p;
			p.format = f;
			p.Cat(WString(key, count), f);
			RichText txt;
			txt.SetStyles(text.GetStyles());
			txt.Cat(p);
			if(overwrite) {
				RichPos p = text.GetRichPos(cursor);
				if(p.posinpara < p.paralen)
					Remove(cursor, 1);
			}
			Filter(txt);
			Insert(cursor, txt, true);
			Move(cursor + count, false);
			break;
		}
		return false;
	}
	objectpos = -1;
	Finish();
	return true;
}

END_UPP_NAMESPACE
