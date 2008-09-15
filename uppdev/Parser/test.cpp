struct X {
	int x, y;
}
point[] = { {1, 2}, {3, 4} };

String test[] = { "ahoj", "kamarade" };

struct String {
	void Test();
};

StatusBar::StatusBar()
{
	String x;
	Height(5 + max(16, Draw::GetStdFontCy()));
	frame.style = frame.style = &StyleDefault();
	SetFrame(frame);
	AddFrame(grip);
}

