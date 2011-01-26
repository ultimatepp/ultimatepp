#ifndef _Styler_Styler_h
#define _Styler_Styler_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS StylerCtrlsImg
#define IMAGEFILE <Styler/StylerCtrls.iml>
#include <Draw/iml_header.h>

void ColorizeIml(Iml& target, Iml& source, const Color& c, int alpha = 100);
void RefreshGlobalStyles();
void RefreshLayoutDeepAll();

#define LAYOUTFILE <Styler/Styler.lay>
#include <CtrlCore/lay.h>

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
	void ColorAction();
};

#endif

