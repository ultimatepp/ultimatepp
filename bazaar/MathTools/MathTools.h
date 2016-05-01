#ifndef _ScatterCtrl_Demo_ScatterCtrl_Demo_h
#define _ScatterCtrl_Demo_ScatterCtrl_Demo_h

#include <CodeEditor/CodeEditor.h>
#include <ScatterCtrl/ScatterCtrl.h>
#include <ScatterCtrl/PieCtrl.h>

using namespace Upp;

class Calculator : public CodeEditor {
typedef Calculator CLASSNAME;

public:
	Callback WhenEnter;
	String GetLine();
	
private:
	virtual bool Key(dword key, int count) {
		if (key == K_ENTER) 
			WhenEnter();
		return CodeEditor::Key(key, count);
	}
};

#define LAYOUTFILE <MathTools/MathTools.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MyImages
#define IMAGEFILE <MathTools/MathTools.iml>
#include <Draw/iml_header.h>


struct Tool : StaticRect {
	virtual void Init() {};
	virtual void End() {};
	//virtual ScatterCtrl &Scatter() = 0;
};
 
void RegisterExample(const char *name, Tool* (*ctrl)(), String fileName);

class MathTools : public WithMathTools<TopWindow> {
typedef MathTools CLASSNAME;
	
public:
	MathTools();
	void End();
	
private:
	void OnSel();

	Vector<StaticRect *> examplesRects;
};

class TabRegression : public Tool {
public:
	typedef TabRegression CLASSNAME;

	TabRegression() : notFirstTime(false) {};	
	//virtual ScatterCtrl &Scatter()	{return scatter;};
	void Jsonize(JsonIO& json) {
		json
			("scatter", up.scatter)
			("gridTrend", down.gridTrend)
			//("gridDef", gridDef)
			//("grid", grid)
			("minR2", down.minR2)
			("switchColsRows", down.switchColsRows)
			("yBesideX", down.yBesideX)
			("firstCellIsName", down.firstCellIsName)
			("coefficients", down.coefficients)
			("notFirstTime", notFirstTime)
		;
	}
	UserEquation *userEquation;
	
private:
	void Init();
	void End();
	void Fit();
	void OnButtons();
	void OnClear();
	void OnAutoset();
	void OnUpdate();
	void ArrayCopy();
	void ArraySelect();
	void OnArrayBar(Bar &menu);
	
	Splitter splitter;
	WithTabRegression_Up<StaticRect> up;
	WithTabRegression_Down<StaticRect> down;
	
	Array<ExplicitEquation> equationTypes;

	Array<EditString> editGrid;
	EditInt xCell, xFrom, xTo, yCell, yFrom, yTo;
	GridCtrlSource ds;
	bool notFirstTime;
};

class TabCalculator : public WithTabCalculator<Tool> {
public:
	typedef TabCalculator CLASSNAME;
	
	void OnEnter();
	void OnFunction();
	void OnConstant();
	void UpdateVars();
		
	void Jsonize(JsonIO& json) {
		json
		;
	}
	
private:
	void Init();
	void End() {};
	
	EvalExpr eval;
};


#endif

