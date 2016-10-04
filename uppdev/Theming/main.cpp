#include "Theming.h"

Theming::Theming()
{
	CtrlLayout(*this, "Window title");
}

struct ThemedButton : Button, Themed {
	int   alpha;
	int   beta;
	Color c;
	String text;
	Font  font;
	Image image;
	
	virtual void   Properties(ThemeProperties& p) {
		alpha = p("alpha", alpha);
		beta = p("beta", beta);
		c = p(c);
		text = p("label", text);
		font = p(font);
		image = p(image);
	}
	
	ThemedButton() {
		alpha = 123456456; beta = 2;
		image = CtrlsImg::B();
	}
};

GUI_APP_MAIN
{
	ThemedButton b;
	ThemedButton c;
	c.Id("ButtonC");
	c.alpha = 100;
	c.beta = 200;
	c.HSizePos(200, 200).BottomPos(12, 13);
	b.Add(c);
	b.c = Blue;
	c.c = Yellow;
	b.text = "Just a test";
	c.text = "Id222";
	b.font = Arial(20).Bold();
	
	b.WriteTheme("e:\\theme");
	LoadTheme("e:\\theme.in");
	b.ReadTheme();
	b.WriteTheme("e:\\theme.out");
}
