#include "AnalyseGui.h"
#include "Analyse.h"

#define IMAGECLASS Images
#define IMAGEFILE  <CodeMetric/AnalyseGui.iml>
#include <Draw/iml.h>

String defaultCode =
 "/*"
 "  This tool measures certain source code metrics.\n"
 "  You can type, paste, or load the code you would like measure into this text area.\n"
 "  Metrics provided are:\n"
 "  - Cyclomatic complexity 1: counts the decision points per method, thus estimating the\n"
 "  number of testcases needed for each method.\n"
 "  - Cyclomatic complexity 2: CC1 extended with the implicit decisions created by the\n"
 "  && || and ?: operators. Evaluation:\n"
 "  CC2  1-10  simple, low risk method\n"
 "  CC2  10-20 moderate complexity & risk method\n"
 "  CC2  21-50 high complexity & risk method\n"
 "  CC2  >50   too complex, untestable method, should be refactored\n"
 "  - Depth: measures deepest scope embedding level per method. This estimates the human\n"
 "  memory needed to keep in mind the current context. Any methods with depth > 5 is\n"
 "  considered too complex, and candidate for refactoring\n"
 "  - Logical Lines Of Code: estimates the amount of source code per method in a way\n"
 "  which is mostly independent from code formatting style. Methods longer than 80 LLOC are\n"
 "  too long, and should be refactored.\n"
 "*/\n"
 "int main()\n"
 "{\n"
 "    return 0;\n"
 "}\n";

String defaultTitle = "CodeMetric GUI 1.2";


WarningDisplay::WarningDisplay(int limit, Color warningColor) :
  limit(limit), warningColor(warningColor)
{
}

void WarningDisplay::PaintBackground(Draw& w, const Rect& r, const Value& q,
	                                 Color ink, Color paper, dword style) const
{
	double v = atof(q.ToString());
	if(v >= limit)
		paper = warningColor;
	Display::PaintBackground(w, r, q, ink, paper, style);
}


void AnalyseGui::MainMenu(Bar &bar)
{
	bar.Add("Load file", THISBACK(Open));
}

void AnalyseGui::Open()
{
	FileSelector fsel;
	fsel.ExecuteOpen("Select file");
	String fileName = fsel.Get();
	if(fileName == "")
		return;
	source <<= LoadFile(fileName);
	UpdateMetric();
	Title(defaultTitle + "  [" + fileName + "]");
}

void AnalyseGui::UpdateMetric()
{
	String s = ~source;
	CodeMetric metric(s);
	int functionCount = metric.functions.GetCount();
	chart.Clear();

	if(functionCount)
	{
		double llocAvg  = (double)metric.lloc.sum  / functionCount,
		       cc1Avg   = (double)metric.cc1.sum   / functionCount,
		       cc2Avg   = (double)metric.cc2.sum   / functionCount,
		       depthAvg = (double)metric.depth.sum / functionCount;

		chart.Add(	"Average",
					"",
					Format("%.2f",cc1Avg),
					Format("%.2f",cc2Avg),
					Format("%.2f",llocAvg),
					Format("%.2f",depthAvg));

		chart.Add(	"Maximum",
					"",
		          metric.cc1.maximum,
		          metric.cc2.maximum,
		          metric.lloc.maximum,
		          metric.depth.maximum);
	}

	for(int i = 0; i < functionCount; i++)
	{
		CodeMetric::FunctionEntry & entry =
		  metric.functions[i];
		chart.Add(entry.name,
		          entry.pos,
		          entry.cyclomaticComplexity1,
		          entry.cyclomaticComplexity2,
		          entry.logicalLinesOfCode,
		          entry.scopeDepth);
	}

	textMetric.SetData(metric.ToString());
}

void AnalyseGui::GotoFunction()
{
	int cursor = chart.GetCursor();
	if(cursor >= 0 && cursor < chart.GetCount())
	{
		int pos = (int)chart.Get(cursor, 1);
		source.SetCursor( source.GetPos(pos) );
		source.CenterCursor();
	}
}

AnalyseGui::AnalyseGui() :
  ccDisplay(50), llocDisplay(80), depthDisplay(5)
{
	Title(defaultTitle);
	Icon(Images::GaugeIcon);
	AddFrame(menu);
	menu.Set( THISBACK(MainMenu) );
	source <<= THISBACK(UpdateMetric);
	source <<= defaultCode;
	textMetric.SetEditable(false);
	chart.AddColumn("Function", 80);
	chart.AddColumn("Pos", 15);
	chart.AddColumn("CC1", 15).SetDisplay(ccDisplay);
	chart.AddColumn("CC2", 15).SetDisplay(ccDisplay);
	chart.AddColumn("LLOC", 15).SetDisplay(llocDisplay);
	chart.AddColumn("Depth", 15).SetDisplay(depthDisplay);
	chart.WhenLeftDouble = THISBACK(GotoFunction);
	chart.OddRowColor(Color(240, 240, 190));
	UpdateMetric();
	splitter.Vert() << source << chart << textMetric;
	splitter.SetPos(9000, 1);
	splitter.SetPos(6500, 0);
	Add(splitter.SizePos());
	Sizeable().Zoomable();
}


GUI_APP_MAIN
{
	AnalyseGui().Run();
}
