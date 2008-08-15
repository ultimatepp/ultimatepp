#ifndef ENH_PARENT_CTRL_HPP
#define ENH_PARENT_CTRL_HPP

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class EnhancedParentCtrl : public ParentCtrl
{
private:
    virtual void RightDown(Point, dword);
    void LocalMenu(Bar& bar);
    void OnLocal(String menu_name);

    typedef EnhancedParentCtrl CLASSNAME;

public:
};

#endif
