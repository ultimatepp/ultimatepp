#include <OpenMP_demo/OpenMP_demo.h>

int GetInfo() {
	int threadId, nThreads;
	
	#pragma omp parallel private(threadId) shared(nThreads)
  	{
		threadId = omp_get_thread_num();
		
		if (threadId == 0) {	// Only master thread does this 
			Cout() << Format("Number of processors = %d\n", omp_get_num_procs());
			Cout() << Format("Number of threads = %d\n", omp_get_num_threads());
			int numProcessors = omp_get_max_threads();
			Cout() << Format("Max threads = %d\n", numProcessors);
			Cout() << Format("In parallel? = %d\n", omp_in_parallel());
			Cout() << Format("Dynamic threads enabled? = %d\n", omp_get_dynamic());
			Cout() << Format("Nested parallelism supported? = %d\n", omp_get_nested());
			
			nThreads = numProcessors > 1 ? int(numProcessors/2.) : 1;	// Just for demo
			Cout() << Format("\nOpenMP demos will run with %d processors\n", nThreads); 
		}
 	} 
 	return nThreads;
}
 
void MatMult_Bench(int nThreads);
void Pi_Bench(int nThreads);

CONSOLE_APP_MAIN
{
    try {
        Cout() << "\OpenMP demo\n\n";
        
        int nThreads = GetInfo();
        
		Pi_Bench(nThreads);
		MatMult_Bench(nThreads);
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