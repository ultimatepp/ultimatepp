#include <Counter/Counter.h>

#define   LAYOUTFILE <UscCtrl/main.lay>
#include  <CtrlCore/lay.h>

GUI_APP_MAIN
{
	WithMainLayout<TopWindow> x;
	x.counter <<= 10;
	CtrlLayout(x);
	x.Run();
	PromptOK(AsString(~x.counter));
}
