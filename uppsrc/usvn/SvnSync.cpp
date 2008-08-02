#include "usvn.h"

SvnSync::SvnSync()
{
	CtrlLayoutOKCancel(*this, "SvnSynchronize SVN repositories");
	list.AddIndex();
	list.AddIndex();
	list.AddColumn("Action");
	list.AddColumn("Path");
	list.ColumnWidths("153 619");
	list.NoCursor().EvenRowColor();
	Sizeable().Zoomable();
}

void SvnSync::Add(const char *working)
{
	String path = GetFullPath(working);
	list.Add(REPOSITORY, path,
	         AttrText("Working directory").SetFont(StdFont().Bold()).Ink(White).Paper(Blue),
	         AttrText(path).SetFont(Arial(20).Bold()).Paper(Blue).Ink(White));
	list.SetLineCy(list.GetCount() - 1, 26);
	Vector<String> ln = Split(Sys("svn status " + path), '\n');
	bool actions = false;
	for(int i = 0; i < ln.GetCount(); i++) {
		String h = ln[i];
		if(h.GetCount() > 7) {
			String file = h.Mid(7);
			if(IsFullPath(file)) {
				h.Trim(7);
				int action = h.Mid(1, 6) == "      " ? String("MC?!").Find(h[0]) : -1;
				String an;
				Color  color;
				if(action < 0)
					color = Gray;
				else {
					static const char *as[] = { "Modify", "Conflict resolved", "Add", "Remove" };
					static Color c[] = { LtBlue, Magenta, Green, LtRed };
					an = as[action];
					color = c[action];
					actions = true;
				}
				int ii = list.GetCount();
				list.Add(action, file,
				         action < 0 ? Value(AttrText("Unknown: " + h.Mid(0, 7)).Ink(color)) : Value(true),
				         AttrText("  " + file.Mid(path.GetCount() + 1)).Ink(color));
				if(action >= 0)
					list.SetCtrl(ii, 0, confirm.Add().SetLabel(an).NoWantFocus());
			}
		}
	}
	if(actions) {
		list.Add(MESSAGE, Null, AttrText("Commit message:").SetFont(StdFont().Bold()));
		list.SetLineCy(list.GetCount() - 1, EditField::GetStdHeight() + 4);
		list.SetCtrl(list.GetCount() - 1, 1, message.Add());
	}
}
