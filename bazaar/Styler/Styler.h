#ifndef _Styler_Styler_h
#define _Styler_Styler_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct PerChStyle : ChStyle<PerChStyle>
{
	//dummy for saving the current status
};

NAMESPACE_UPP
template<> Stream& operator%(Stream& s, PerChStyle& o);
template<> void Xmlize(XmlIO xml, PerChStyle& o);
END_UPP_NAMESPACE

#define IMAGECLASS StylerCtrlsImg
#define IMAGEFILE <Styler/StylerCtrls.iml>
#include <Draw/iml_header.h>

void ColorizeIml(Iml& target, Iml& source, const Color& c, int alpha = 100);
void RefreshGlobalStyles();
void RefreshLayoutDeepAll();

#define LAYOUTFILE <Styler/Styler.lay>
#include <CtrlCore/lay.h>

#define CH_LOCSTYLE(klass, type, style) \
struct COMBINE5(klass, __, type, __, style) : klass::type { \
	void Init(); \
	static void InitIt(); \
}; \
\
const klass::Style& style(); \
void COMBINE5(klass, __, type, __, style)::InitIt() { \
	style(); \
} \
\
const klass::Style& style() \
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

void XmlizeCol(XmlIO xml, const String& tag, Color& c);

void RefreshGlobalStyles();
void RefreshLayoutDeepAll();

void ChStylerSkin(const Color& c, int blnd, const Color& fg, const Color& bg);

class StylerCtrl : public WithStylerLay<ParentCtrl> {
public:
	typedef StylerCtrl CLASSNAME;
	StylerCtrl();

	virtual void Updated();

	void SkinAction();
	void InvertAction();

protected:
	void ColorizedSkin();
};

class ChamStyleCtrl : public WithChamStyleLay<ParentCtrl> {
public:
	typedef ChamStyleCtrl CLASSNAME;
	ChamStyleCtrl();

	virtual void Updated();
	
	void LoadCB();
	void SaveCB();
	
	void ColorAction();
};

#endif

