#include <stdapp/stdapp.hpp>

void stdapp::Edit(Bar& bar)
{
    bar.Add(t_("Cut"), THISBACK(OnCut)).Key(K_CTRL_X).Help(t_("Cut out the current object"));
    bar.Add(t_("Copy"), THISBACK(OnCopy)).Key(K_CTRL_C).Help(t_("Copy the current object to the clipboard"));
    bar.Add(t_("Paste"), THISBACK(OnPaste)).Key(K_CTRL_V).Help(t_("Paste an object from the clipboard"));
    bar.Add(t_("Delete"), THISBACK(OnDelete)).Help(t_("Delete the current object"));
    bar.MenuSeparator();
    bar.Add(t_("Undo"), THISBACK(OnUndo)).Key(K_CTRL_Z).Help(t_("Undo the last operation"));
    bar.Add(t_("Redo"), THISBACK(OnRedo)).Key(K_SHIFT_CTRL_Z).Help(t_("Redo the last undone operation"));
}

void stdapp::OnCut()
{
    Exclamation("You still have to write your own \"Cut\" code!");
}

void stdapp::OnCopy()
{
    Exclamation("You still have to write your own \"Copy\" code!");
}

void stdapp::OnPaste()
{
    Exclamation("You still have to write your own \"Paste\" code!");
}

void stdapp::OnDelete()
{
    Exclamation("You still have to write your own \"Delete\" code!");
}

void stdapp::OnUndo()
{
    Exclamation("You still have to write your own \"Undo\" code!");
}

void stdapp::OnRedo()
{
    Exclamation("You still have to write your own \"Redo\" code!");
}
