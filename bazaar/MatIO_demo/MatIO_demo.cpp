#include <Core/Core.h>
#include "data.brc"

using namespace Upp;

#include <plugin/matio/matio.h>

CONSOLE_APP_MAIN
{
	MatFile mat;
	
	String fileName = AppendFileName(GetDesktopFolder(), "data.mat");
	
	if (!FileExists(fileName)) {
		if (!SaveFile(fileName, String(dataMat, dataMat_length))) {
			Cout() << "\nError: Impossible to save .mat file";
			return;
		}
	}
	
	if (!mat.OpenWrite(fileName)) {
		Cout() << Format("\nError: %s", MatFile::GetLastError());
		return;
	}
	
	Cout() << "\nVersion " << mat.GetVersionName();
	
	Cout() << "\nVariables list:";
	for (int i = 0; i < mat.GetVarCount(); ++i) {
		MatVar var = mat.GetVar(mat.GetVarName(i));
		Cout() << Format("\n- %s <%s>(%d, %d)", var.GetName(), var.GetTypeString(), var.GetDimCount(0), var.GetDimCount(1));
	}
	
	MatMatrix<double> aa = mat.VarRead<double>("A");		
	
	Cout() << "\nA";
	for(int i=0; i< aa.GetRows(); i++) {
		Cout() << "\n";
		for(int j=0; j < aa.GetCols(); j++)
			Cout() << Format("%.3f ", aa(i, j));
	}
	
	if (mat.VarExists("A_b")) {
		MatMatrix<double> ab = mat.VarRead<double>("A_b");
		
		Cout() << "\nA_b";
		for(int i=0; i< ab.GetRows(); i++) {
			Cout() << "\n";
			for(int j=0; j < ab.GetCols(); j++)
				Cout() << Format("%.3f ", ab(i, j));
		}
	}
	
	aa(3, 0) = 5.8;
	
	Cout() << "\nA_b";
	for(int i=0; i< aa.GetRows(); i++) {
		Cout() << "\n";
		for(int j=0; j< aa.GetCols(); j++)
			Cout() << Format("%.3f ", aa(i, j));
	}
	if (!mat.VarWrite("A_b", aa))
		Cout() << "\nProblem writing A_b";
	
	ReadStdIn();
}
