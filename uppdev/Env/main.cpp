#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class A
{
 public:
   A() { DUMP(Environment().Get("PATH")); }
};

A a;  // <<= STATIC OBJECT CAUSES CRASH

struct B : TopWindow
{
 
};

GUI_APP_MAIN
{
 B().Run();
}
