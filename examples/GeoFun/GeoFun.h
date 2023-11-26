#ifndef _GeoFun_GeoFun_h
#define _GeoFun_GeoFun_h

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>
#include <Report/Report.h>
#include <RichText/RichText.h>
#include <plugin/jpg/jpg.h>

namespace Upp {

#define IMAGECLASS GFImg
#define IMAGEFILE <GeoFun/GeoFun.iml>
#include <Draw/iml_header.h>

#define TOPICFILE <GeoFun/app.tpp/all.i>
#include <Core/topic_group.h>

#define LAYOUTFILE <GeoFun/GeoFun.lay>
#include <CtrlCore/lay.h>

bool cfgAvailable;

class DrawCanvas : public ParentCtrl {
	typedef DrawCanvas CLASSNAME;

public:
	virtual void Paint(Draw& drw);
	DrawCanvas();
	template <class T>
	void DoPaint0(T& sw, int idraw = 0);
	void DoPaint(Painter& sw);

	struct ColorPolygon : Moveable<ColorPolygon> {
		Vector<Pointf> point;
		Vector<Pointf> CenR;
		Color lcolor;
		Color fcolorM1, fcolorM2;
		int penwidth;
		bool filled;
	};

	Vector<ColorPolygon> data;

	Color fcolorB1, fcolorB2;
	bool ShowGradient, ColorFill, Zoomed, ShowAnimation;

	double Radius1, BaseR, RollR;
	int ShowTo, AnimSpeed;
};

class GeoFun : public WithGeoFunLayout<TopWindow> {
public:
	typedef GeoFun CLASSNAME;
	GeoFun();

	double ArmLen;

	DrawCanvas dc1;

	WithInputLayout<ParentCtrl> InputPane;

	Splitter sp1;
	QtfRichObject qtfim;

	TimeCallback Animate;

	int ValP, ValQ, PenW, CurvT, GradientY;
	Color PenC, M1C, M2C, B1C, B2C;

	bool First;
	bool EpiCyclo, HypoCyclo, EpiTroch, HypoTroch, AnimMn;

	// Functions
	void About();

	void Exit();

	void FirstDraw();

	Image GetImage();

	void Help();

	virtual void Layout();

	void OptionColorFill();
	void OptionGradient();
	void OptionZoom();

	void PrepareData();

	void Print();

	void SaveToImageFile(String fileName);
	void SaveToAutoCadScript(String fileName);
	void SaveToSVG(String fileName);
	void SetAnimation();
	void SetAnimationSpeed();
	void SetCurveType(int CrvType);

	void SetCurveTypeMenu(int CrvType);
	void Serialize(Stream& s);
	void ShowAnimated();

	// Menu Functions
	void MainMenu(Bar& menu);

	void CurvesMenu(Bar& bar);
	void FileMenu(Bar& bar);
	void HelpMenu(Bar& bar);
	void SettingsMenu(Bar& bar);
};

} // namespace Upp

#endif
