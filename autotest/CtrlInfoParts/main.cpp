#include <CtrlLib/CtrlLib.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	auto Print = [&](Ctrl& h) {
		DLOG("========================");
		DDUMP(h.GetLayoutId());
		DDUMP(h.GetTip());
		DDUMP(h.GetDescription());
		DDUMP(h.GetHelpLine());
	};

	{
		Ctrl h;
		h.LayoutIdLiteral("just some literal");
		Print(h);
	}

	{
		Ctrl h;
		h.LayoutId(String("just some text"));
		Print(h);
	}

	{
		Ctrl h;
		h.LayoutIdLiteral("just some literal");
		h.HelpLine("some helpline");
		Print(h);
	}

	{
		Ctrl h;
		h.LayoutId(String("just some text"));
		h.HelpLine("some helpline");
		Print(h);
	}

	{
		Ctrl h;
		h.LayoutIdLiteral("just some literal");
		h.Tip("this is tip");
		h.HelpLine("some helpline");
		h.Description("this is description");
		Print(h);
	}
	
	CheckLogEtalon();
}
