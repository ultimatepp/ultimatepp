#include <stdapp/EnhParentCtrl.hpp>

void EnhancedParentCtrl::RightDown(Point, dword)
{
        MenuBar::Execute(THISBACK(LocalMenu));
}

void EnhancedParentCtrl::LocalMenu(Bar& bar)
{
    bar.Add(t_("Local menu 1"), THISBACK1(OnLocal, t_("Local menu 1")));
    bar.Add(t_("Local menu 2"), THISBACK1(OnLocal, t_("Local menu 2")));
    bar.Add(t_("Local menu 3"), THISBACK1(OnLocal, t_("Local menu 3")));
}

void EnhancedParentCtrl::OnLocal(String menu_name)
{
    Exclamation(Format("You still have to write your own \"%s\" code!", menu_name));
}
