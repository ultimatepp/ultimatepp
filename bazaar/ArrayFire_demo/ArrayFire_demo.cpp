#include <Core/Core.h>

using namespace Upp;

#include <arrayfire.h>

using namespace af; 

void MatMult_Bench();
void Pi_Bench();
void Vectorize_Bench();
void Demo();

CONSOLE_APP_MAIN
{
    try {
        Cout() << "\nArrayFire library demo\n\n";
        
		int device = 0;
        setDevice(device);
        info();
        
		Demo();
		Pi_Bench();
		MatMult_Bench();
		Vectorize_Bench();
    } catch (af::exception& e) {
        Cout() << t_("Internal error:") + String(" ") + e.what() + String("\n") + t_("Program ended");
	} catch (Exc e) {
		Cout() << t_("Internal error:") + String(" ") + e + String("\n") + t_("Program ended");
	} catch(const char *cad) {
		Cout() << t_("Internal error:") + String(" ") + cad + String("\n") + t_("Program ended");
	} catch(const std::string &e) {
		Cout() << t_("Internal error:") + String(" ") + e.c_str() + String("\n") + t_("Program ended");	
	} catch (const std::exception &e) {
		Cout() << t_("Internal error:") + String(" ") + e.what() + String("\n") + t_("Program ended");
	} catch(...) {
		Cout() << t_("Internal error:") + String(" ") + t_("Unknown error") + String("\n") + t_("Program ended");
	}
	Cout() << "\nEnd";
	ReadStdIn();
}