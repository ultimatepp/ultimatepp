#ifndef _Tcc_Tcc_h
#define _Tcc_Tcc_h

#include "plugin/tcc/libtcc.h" 

class Tcc
{
public:
	Tcc() {
		stateTcc = 0;
	};
	~Tcc();
#if defined(COMPILER_MSC)
	Tcc(const char *dllFile);
	void Init(const char *dllFile = "libtcc.dll");
#else
	Tcc(const char *libPath);
	void Init(const char *libPath = NULL);
#endif
	bool IsInit() {return stateTcc;}

	void NoStdlib() {::NoStdlib(stateTcc);}

	bool AddIncludePath(const char *path);
	bool AddLibraryPath(const char *path);
	
	void SetOutputExe();
	void SetOutputMemory();
	
	void Compile(const char *my_program);
	void AddSymbol(const char *funName, void *fun);
	void *GetSymbol(const char *funName); 
	void Link(const char *fileName = 0);
	
	String GetProgram();

private:
	static int numInstances;
	TCCState *stateTcc;
#if defined(COMPILER_MSC)
	HINSTANCE hinstLib;
#endif

	static void DefaultErrorHandler(void *opaque, const char *msg);
	String errorMsg;
	int initProgramLines;
	int initLen;
	String program;
	bool outputMemory;
	
#if defined(COMPILER_MSC)
	TCCState *(*T_tcc_new)(void);
	void (*T_tcc_delete)(TCCState *);
	int (*T_tcc_set_output_type)(TCCState *s, int output_type);
	void (*T_tcc_set_error_func)(TCCState *s, void *error_opaque, void (*error_func)(void *opaque, const char *msg));
	int (*T_tcc_get_symbol)(TCCState *s, unsigned long *pval, const char *name);
	int (*T_tcc_add_symbol)(TCCState *s, const char *name, unsigned long val);
	int (*T_tcc_relocate)(TCCState *s);
	int (*T_tcc_compile_string)(TCCState *s, const char *buf);
	int (*T_tcc_add_include_path)(TCCState *s, const char *buf);	
	int (*T_tcc_add_library_path)(TCCState *s, const char *buf);	
	int (*T_tcc_output_file)(TCCState *s, const char *buf);
#endif
};

#endif
