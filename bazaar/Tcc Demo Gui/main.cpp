#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Tcc Demo Gui/Tcc Demo Gui.lay>
#include <CtrlCore/lay.h>

class Tcc_Demo : public WithDaysLayout<TopWindow> {
public:
	void RunProgram();

	typedef Tcc_Demo CLASSNAME;
	Tcc_Demo();
};

#include "Tcc/Tcc.h"

// Function to be called from the tcc program
double plus(double a, double b)
{
    return a + b;
} 

// Source code
char my_program[] =
		"#include <stdio.h>\n"
		"#include <math.h>\n"
		"\n"
		"double plus(double a, double b);	// External function has to be declared\n"
		"\n"
		"double pow2(double d)\n"
		"{\n"
		"	return (d*d);\n"
		"}\n"
		"\n"
		"double test(double a, char *str)\n"
		"{\n"	
		"	if (a < 0.)\n"
		"		throw(\"Argument is negative!\");\n"	
		"	double ret = 0;\n"
		"	int i;\n"
		"	for (i = 0; i < 10000000; ++i)\n"	
		"		ret += 0.000000001*pow2(sqrt(plus(a, 10)));\n"
		"	snprintf(str, 1024, \"The result is %f\", ret);\n"
		"	return ret;\n"
		"}";

void Tcc_Demo::RunProgram()
{
    Tcc tcc;
    
    // Set the right directories here
    tcc.AddIncludePath(EditIncludePath.GetData().ToString());
	tcc.AddLibraryPath(EditLibsPath.GetData().ToString());
	
    try {
	    tcc.Compile(SourceCode.GetData().ToString());
	
	    tcc.AddSymbol("plus", (void *)&plus);
		tcc.Link();
		double (*mytest1)(double, char *) = (double (*)(double, char *))tcc.GetSymbol("test");
		
		char str[1024];
		double res = mytest1(90, str);
		PromptOK("\nThe result for Test is " + FormatDouble(res) + ". " + str);
	} catch(Exc err){
		Exclamation(DeQtfLf(err));
	}
}

Tcc_Demo::Tcc_Demo()
{
	CtrlLayout(*this, "Tcc Demo"); 
	ButRun.WhenPush = THISBACK(RunProgram);
	SourceCode.SetData(my_program);
	EditIncludePath <<= "...put here right path.../Tcc/lib/include";		//c:/desarrollo/aplicaciones/zlibs
	EditLibsPath <<= "...put here right path.../Tcc/lib/lib";
	SourceCode.SetFont(Courier(12));
	Sizeable();
}

GUI_APP_MAIN
{
	Tcc_Demo().Run();
}
