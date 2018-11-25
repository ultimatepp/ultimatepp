#ifndef _MathTools_MathTools_h
#define _MathTools_MathTools_h

#include <CodeEditor/CodeEditor.h>
#include <ScatterCtrl/ScatterCtrl.h>
#include <ScatterCtrl/PieCtrl.h>

using namespace Upp;

class Calculator : public CodeEditor {
typedef Calculator CLASSNAME;

public:
	Calculator() : actualLine(-1) {};
	
	Callback WhenChange;
	String GetLine();
	
private:
	int actualLine;
	virtual bool Key(dword key, int count) {
		if (actualLine < 0)
			actualLine = GetCursorLine();
		else if (actualLine != GetCursorLine()) {
			actualLine = GetCursorLine();
			if (IsModified())
				WhenChange();
		}
		if (key == K_ENTER) {
			key = K_DOWN;
			if (GetCursorLine() == GetLineCount() - 1)
				Insert(GetLength(), "\n");
		}
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
	String name;
};
 
void RegisterExample(Tool* (*ctrl)(), String fileName);

class MathTools : public WithMathTools<TopWindow> {
typedef MathTools CLASSNAME;
	
public:
	void Init();
	void End();
	
private:
	void OnSel();

	Vector<StaticRect *> examplesRects;
};

class TabData : public Tool {
public:
	typedef TabData CLASSNAME;

	TabData() : notFirstTime(false) {};	
	void Jsonize(JsonIO& json) {
		json
			("scatter", up.scatter)
			("gridDef", downL.gridDef)
			("nIsHeader", downL.nIsHeader)
			("notFirstTime", notFirstTime)
			("editFile", down.editFile)
			("butFitPM", down.butFitPM)
			("butSetPM", down.butFitPM)
			("splitter_SetPos", splitterPos)
			("splitterDown_SetPos", splitterDownPos)
		;
	}
	
private:
	void Init();
	void End();
	void Fit();
	void OnClear();
	void OnClearDef();
	void OnAutoset();
	void OnUpdate();
	void OnOpen(bool force, bool updateButtons);
	void OnChangeFile(int delta);
	void ArrayCopy();
	void ArraySelect();
	void OnArrayBar(Bar &menu);
	void OnShowAll();
	void OnNIsheader();
		
	Splitter splitter;
	WithTabData_Up<StaticRect> up;
	WithTabData_Down<StaticRect> down;
	WithTabData_Down_L<StaticRect> downL;
	WithTabData_Down_R<StaticRect> downR;
	
	Array<EditString> editGrid;
	EditInt xCell, xFrom, xTo, yCell, yFrom, yTo;
	Option option;
	Array<GridCtrlSource> ds;
	bool notFirstTime;
	
	OpenFileButton openMenuButton;
	FrameRight<Button> butUp, butDown, openButton;
	
	int splitterPos, splitterDownPos;
};



class EvalExpr2 : public EvalExpr {
public:
	EvalExpr2();
	String Eval2(String line, int numDecimals, int tabChars);
	Unit EvalUnit(CParser& p);
	
private:
	Unit TermUnit(CParser& p);
	Unit PowUnit(CParser& p);
	Unit MulUnit(CParser& p);
	
	VectorMap<String, Unit> units;	
};

class TabCalculator : public WithTabCalculator<Tool> {
public:
	typedef TabCalculator CLASSNAME;
	
	void OnChange();
	void OnFunction();
	void OnConstant();
	void UpdateVars();
		
/*	void Jsonize(JsonIO& json) {
		json
		;
	}*/
	
private:
	void Init();
	void End() {};
	
	EvalExpr2 eval;
};


#endif

