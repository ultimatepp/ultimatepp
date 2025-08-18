#include "RichEdit.h"

namespace Upp {

bool HasNumbering(const RichPara::Format& f)
{
	if(f.after_number.GetCount() || f.before_number.GetCount())
		return true;
	for(int i = 0; i < 8; i++)
		if(f.number[i] != RichPara::NUMBER_NONE)
			return true;
	return false;
}

bool RichEdit::RemoveBullet(bool backspace)
{
	RichPos p = text.GetRichPos(cursor);
	if((backspace ? p.posinpara : p.paralen) == 0 &&
	   (p.format.bullet != RichPara::BULLET_NONE || HasNumbering(p.format))) {
	    Style();
		RichText::FormatInfo nobullet;
		nobullet.paravalid = RichText::NUMBERING|RichText::BULLET;
		nobullet.charvalid = 0;
		ApplyFormatInfo(nobullet);
		return true;
	}
	return false;
}

bool RichEdit::Key(dword key, int count)
{
	useraction = true;
	NextUndo();
	if(CursorKey(key, count))
		return true;
	if(IsReadOnly())
		return false;
	if(key == (K_CTRL_KEY|K_KEYUP) && show_zoom) {
		show_zoom = false;
		Refresh();
	}
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
	case K_SHIFT_BACKSPACE:
		if(RemoveSelection(true)) return true;
		if(RemoveBullet(true)) break;
		if(cursor <= 0 || RemoveSpecial(cursor, cursor - 1, true))
			return true;
		Move(cursor - 1);
		anchor = cursor;
		begtabsel = false;
		if(cursor > 0) {
			RichPos p = text.GetRichPos(cursor - 1);
			if(p.format.bullet != RichPara::BULLET_NONE || HasNumbering(p.format)) {
				Remove(cursor, 1, true);
				break;
			}
		}
		SetLastCharFormat();
		Remove(cursor, 1);
		break;
	case K_DELETE:
		if(RemoveSelection()) return true;
		if(cursor < text.GetLength() && !RemoveSpecial(cursor, cursor + 1, false)) {
			SetLastCharFormat();
			Remove(cursor, 1, true);
		}
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
	case K_ALT_BACKSPACE:
		Undo();
		return true;
	case K_SHIFT_CTRL_Z:
	case K_SHIFT|K_ALT_BACKSPACE:
	case K_CTRL_Y:
		Redo();
		return true;
	case K_ENTER: {
			if(findreplace.IsOpen()) {
				findreplace.ok.PseudoPush();
				return true;
			}
			if(singleline)
				return false;
			if(!RemoveSelection() && InsertLineSpecial())
				return true;
			if(RemoveBullet(false))
				break;
			RichText::FormatInfo f = formatinfo;
			InsertLine();
			formatinfo = f;
			formatinfo.link.Clear();
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
		Find(false);
		break;
	case K_SHIFT_F3:
		Find(true);
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
		static int altkeys[10] = { K_ALT_0, K_ALT_1, K_ALT_2, K_ALT_3, K_ALT_4,
		                           K_ALT_5, K_ALT_6, K_ALT_7, K_ALT_8, K_ALT_9 };
		for(int i = 0; i < 10; i++) {
			if(key == altkeys[i]) {
				ApplyStyleKey(i);
				break;
			}
			if(key == (K_SHIFT|altkeys[i])) {
				ApplyStyleKey(i);
				break;
			}
		}
		if(key == K_SHIFT_SPACE)
			key = ' ';
		if(key == 9 || key >= 32 && key < K_CHAR_LIM) {
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
			SetLastCharFormat(f);
			break;
		}
		return false;
	}
	objectpos = -1;
	Finish();
	return true;
}

}
