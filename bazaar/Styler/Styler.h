#ifndef _Styler_Styler_h
#define _Styler_Styler_h

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#define IMAGECLASS StylerCtrlsImg
#define IMAGEFILE <Styler/StylerCtrls.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <Styler/Styler.lay>
#include <CtrlCore/lay.h>

Color Invert(const Color& c);

void ColorizeIml(Iml& target, Iml& source, const Color& c, int alpha = 100);
void RefreshGlobalStyles();
void RefreshLayoutDeepAll();

void ChStylerSkin(const Color& c, int blnd, const Color& fg, const Color& bg);

void XmlizeCol(XmlIO& xml, const String& tag, Color& c);

//dummy for saving the global status
struct PerChStyle : ChStyle<PerChStyle> {};

NAMESPACE_UPP
template<> Stream& operator%(Stream& s, PerChStyle& o);
template<> void Xmlize(XmlIO& xml, PerChStyle& o);
END_UPP_NAMESPACE

bool SaveGlobalStyle(const String& filename);
bool LoadGlobalStyle(const String& filename);

//for manipulating the SColors
class ChamStyleCtrl : public WithChamStyleLay<ParentCtrl> {
public:
	typedef ChamStyleCtrl CLASSNAME;
	ChamStyleCtrl();

	virtual void Updated();
	
	void DoLoad();
	void DoSave();
	
	void OnColorAction();
	
	Callback WhenSColorsChange;
};

class StylerCtrl : public WithStylerLay<ParentCtrl> {
public:
	typedef StylerCtrl CLASSNAME;
	StylerCtrl();

	virtual void Updated();

	void OnSkinAction();
	void OnInvertAction();

	Callback WhenSkinChange;

protected:
	void SetColorizedSkin();
};


//overriding CH_STYLE definitions for a class:: StyleFunction by own definitions, which are not bound to the namespace
#define CH_LOCSTYLE(klass, type, style) \
struct COMBINE5(klass, __, type, __, style) : klass::type { \
	void Init(); \
	static void InitIt(); \
}; \
\
const klass::type& style(); \
void COMBINE5(klass, __, type, __, style)::InitIt() { \
	style(); \
} \
\
const klass::type& style() \
{ \
	static COMBINE5(klass, __, type, __, style) b, standard; \
	if(b.status == 0) { \
		ChRegisterStyle__(b.status, b.registered, COMBINE5(klass, __, type, __, style)::InitIt); \
		b.Init(); \
		b.status = 1; \
		standard = b; \
		standard.standard = b.standard = &standard; \
	} \
	return b; \
} \
\
void COMBINE5(klass, __, type, __, style)::Init()

#endif

