#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Tcc_Demo_Gui/Tcc_Demo_Gui.lay>
#include <CtrlCore/lay.h>

class Tcc_Demo : public WithMainLayout<TopWindow> {
public:
	void RunProgram();
	void OpGetExe_Action();

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
char my_program_in_memory[] =
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
		
char my_program_in_file[] =
		"#include <stdio.h>\n"
		"\n"
		"int fib(n)\n"
		"{\n"
		"	if (n <= 2)\n"
		"		return 1;\n"
		"	else\n"
		"		return fib(n-1) + fib(n-2);\n"
		"}\n"
		"\n"
		"int main(int argc, char **argv)\n"
		"{\n"
		"	int n = 30;\n"
		"\n"
		"	printf(\"\\nCompute nth Fibonacci number fib(%d) = \", n);\n"
		"	printf(\"%d\", fib(n));\n"
		"\n"
		"	printf(\"\\nPress a key to end\");\n"
		"	getchar();\n"
		"	return 0;\n"
		"}";

void Tcc_Demo::RunProgram()
{
	try {
		Tcc tcc(EditTccLibPath.GetData().ToString());
#if !defined(PLATFORM_WIN32)		
		if (OpNoStdlib)
			tcc.NoStdlib();
#endif		
		if (OpGetExe)
			tcc.SetOutputExe();
		else
			tcc.SetOutputMemory();
		
		// Set the right directories here
		tcc.AddIncludePath(EditIncludePath.GetData().ToString());
		tcc.AddLibraryPath(EditLibsPath.GetData().ToString());
	
	    tcc.Compile(SourceCode.GetData().ToString());
	
		if (!OpGetExe) {
		    tcc.AddSymbol("plus", (void *)&plus);
			tcc.Link();
			double (*mytest1)(double, char *) = (double (*)(double, char *))tcc.GetSymbol("test"); 

			char str[1024];
			double res = mytest1(90, str);
			PromptOK("The result for Test is " + FormatDouble(res) + ". " + str);
		} else {
			tcc.Link(EditExeFile.GetData().ToString());
			PromptOK("Program compiled in " + DeQtf(EditExeFile.GetData().ToString()));
		}
	} catch(Exc err){
		Exclamation(DeQtfLf(err));
	}
}

void Tcc_Demo::OpGetExe_Action()
{
	switch (OpGetExe) {
		case 0: 
			EditExeFile.Enable(false);
			SourceCode.SetData(my_program_in_memory);
			break;
		default:
			EditExeFile.Enable(true);
			SourceCode.SetData(my_program_in_file);
	}
}

Tcc_Demo::Tcc_Demo()
{
	CtrlLayout(*this, "Tcc Demo"); 
	ButRun.WhenPush = THISBACK(RunProgram);
	OpGetExe.WhenAction = THISBACK(OpGetExe_Action);
	OpGetExe = 0;
	OpGetExe_Action();
#if defined(PLATFORM_WIN32)
	EditExeFile.SetData(AppendFileName(GetHomeDirectory(), "tccdemo.exe")); 
	OpNoStdlib.Disable();
	EditTccLibPath <<= "libtcc";
#else
	EditExeFile.SetData(AppendFileName(GetHomeDirectory(), "tccdemo")); 
	OpNoStdlib = true;
#endif
	EditIncludePath <<= "...put here right path...bazaar/plugin/tcc/include";		
	EditLibsPath <<= "...put here right path...bazaar/plugin/tcc/lib";
	SourceCode.SetFont(Courier(12));
	Sizeable();
}

GUI_APP_MAIN
{
	Tcc_Demo().Run();
}
