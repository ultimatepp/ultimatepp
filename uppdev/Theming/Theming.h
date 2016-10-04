#ifndef _Theming_Theming_h
#define _Theming_Theming_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;


struct ThemeProperties {
	bool loading;
	VectorMap<String, String>& data;
	VectorMap<String, Image>& image;
	
	String Raw(const char *id, const String& s);
	
	int    operator()(const char *id, int p);
	Color  operator()(const char *id, Color p);
	Color  operator()(Color p);
	String operator()(const char *id, const String& s);
	String operator()(const String& s);
	Font   operator()(const char *id, Font fnt);
	Font   operator()(Font fnt);
	Image  operator()(const char *id, const Image& img);
	Image  operator()(const Image& img);

	ThemeProperties(VectorMap<String, String>& data, VectorMap<String, Image>& image, bool loading)
	: data(data), loading(loading), image(image) {}
};

class Themed {
	String id;

	void ReadTheme();
	void WriteTheme();

public:
	void Id(const char *s) { id = s; }

	virtual String GetClass() const;
	virtual void   Properties(ThemeProperties& p);
	
	void DoTheme();

	static void LoadTheme(const char *dir);
	static void SetWrite(const char *writedir);
};


#define LAYOUTFILE <Theming/Theming.lay>
#include <CtrlCore/lay.h>

class Theming : public WithThemingLayout<TopWindow> {
public:
	typedef Theming CLASSNAME;
	Theming();
};

#endif

