#include "GridCtrl.h"

namespace Upp {

bool GridCtrl::Key(dword key, int)
{
	auto NewAnchor = [&] {
		ClearSelection();
		anchor = curpos;
	};

	auto NewSelection = [&] {
		if(IsValidCursor(anchor) && IsValidCursor(curpos))
			SelectRange(Rect(anchor, curpos), true, select_row);
		Refresh();
	};

	if(!IsReadOnly())
	switch(key)
	{
		case K_ENTER:
			ClearSelection();
			WhenEnter();
			#ifdef LOG_CALLBACKS
			LGR(2, "WhenEnter()");
			#endif

			if(enter_like_tab)
				return TabKey(true);
			else if(!SwitchEdit())
				return true;
			/*
			if(th.IsSorted())
			{
				th.Multisort();
				Refresh();
			}*/
			NewAnchor();
			return true;
		case K_ESCAPE:
		{
			bool quit = true;
			if(search_string.GetCount() > 0)
			{
				ClearFound();
				quit = false;
			}
			else if(HasCtrls())
			{
				bool canceled = CancelEdit();
				quit = !canceled;
			}
			if(quit)
			{
				WhenEscape();
				return false;
			}
			else
				return true;
		}
		case K_SHIFT|K_LEFT:
			GoLeft();
			NewSelection();
			return true;
		case K_SHIFT|K_RIGHT:
			GoRight();
			NewSelection();
			return true;
		case K_SHIFT|K_UP:
			GoPrev();
			NewSelection();
			return true;
		case K_SHIFT|K_DOWN:
			GoNext();
			NewSelection();
			return true;
		case K_SHIFT|K_PAGEUP:
			GoPageUp();
			NewSelection();
			return true;
		case K_SHIFT|K_PAGEDOWN:
			GoPageDn();
			NewSelection();
			return true;
		case K_SHIFT_HOME:
			GoBegin();
			NewSelection();
			return true;
		case K_SHIFT_END:
			GoEnd();
			NewSelection();
			return true;
		case K_UP:
			GoPrev();
			NewAnchor();
			return true;
		case K_DOWN:
			GoNext();
			NewAnchor();
			return true;
		case K_LEFT:
			GoLeft();
			NewAnchor();
			return true;
		case K_RIGHT:
			GoRight();
			NewAnchor();
			return true;

		case K_HOME:
		case K_CTRL_HOME:
		case K_CTRL_PAGEUP:
			GoBegin();
			NewAnchor();
			return true;
		case K_END:
		case K_CTRL_END:
		case K_CTRL_PAGEDOWN:
			GoEnd();
			NewAnchor();
			return true;
		case K_PAGEUP:
			GoPageUp();
			NewAnchor();
			return true;
		case K_PAGEDOWN:
			GoPageDn();
			NewAnchor();
			return true;
		case K_TAB:
			return TabKey(false);
		case K_SHIFT|K_TAB:
			if(HasCtrls())
			{
				bool isprev = ShowPrevCtrl();
				return focused_ctrl ? true : isprev;
			}
			else if(tab_changes_row)
			{
				bool isprev = false;
				if(select_row)
					isprev = GoPrev();
				else
					isprev = GoLeft();
				ClearSelection();

				return isprev;
			}
			else
				return false;
		case K_CTRL|K_F:
			if(searching)
			{
				find.SetFocus();
				return true;
			}
			else
				return false;
		case K_BACKSPACE:
		{
			if(searching)
			{
				int cnt = search_string.GetCount();
				if(cnt > 0)
				{
					search_string.Remove(cnt - 1);
					find <<= search_string;
					ShowMatchedRows(search_string);
				}
				return true;
			}
			else
				return false;
		}
		case K_F3:
			if(rowfnd >= 0)
			{
				for(int i = rowfnd + 1; i < total_rows; i++)
				{
					if(vitems[i].IsFound())
					{
						rowfnd = i;
						SetCursor0(i);
						CenterCursor();
						WhenSearchCursor();
						return true;
					}
				}
				for(int i = fixed_rows; i < rowfnd; i++)
				{
					if(vitems[i].IsFound())
					{
						rowfnd = i;
						SetCursor0(i);
						CenterCursor();
						WhenSearchCursor();
						return true;
					}
				}

				return true;
			}
			return false;
		case K_CTRL_W:
			WriteClipboardText(GetColumnWidths());
			return true;
		default:
			if(searching && !isedit && Search(key))
				return true;
	}

	return MenuBar::Scan(WhenMenuBar, key);
}

};