#include <Esc/Esc.h>

using namespace Upp;

void SIC_Print(EscEscape& e)
{
	if(e[0].IsArray())
		Cout() << (String) e[0];
	else
	if(e[0].IsNumber())
		Cout() << e[0].GetNumber();
	else
	if(!e[0].IsVoid())
		e.ThrowError("invalid argument to 'Print'");
}

void SIC_Input(EscEscape& e)
{
	e = ReadStdIn();
}

void SIC_InputNumber(EscEscape& e)
{
	e = atof(ReadStdIn());
}

CONSOLE_APP_MAIN
{
	ArrayMap<String, EscValue> global;
	Escape(global, "Print(x)", SIC_Print);
	Escape(global, "Input()", SIC_Input);
	Escape(global, "InputNumber()", SIC_InputNumber);
	StdLib(global);
	try {
		Scan(global, LoadDataFile("script.esc"));
		Execute(global, "main", INT_MAX);
	}
	catch(CParser::Error e) {
		Cout() << "ERROR: " << e << "\n";
	}
}
