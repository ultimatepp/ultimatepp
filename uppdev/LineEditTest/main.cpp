#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class Test : public TopWindow
{
public:
  LineEdit a, b;
  Splitter s;
  void aChanged() { b <<= ~a; }
  Test()
  {
    a <<= THISBACK(aChanged);
    Add(s.Vert(a, b).SizePos());
    Zoomable().Sizeable();
  }
  typedef Test CLASSNAME;
};

GUI_APP_MAIN
{
   Test().Run();
}
