#include <Core/Core.h>

using namespace Upp;

#include "Tcc.h"

#if __GNUC__
#define T_tcc_new				tcc_new
#define T_tcc_delete			tcc_delete
#define T_tcc_set_output_type	tcc_set_output_type
#define T_tcc_set_error_func	tcc_set_error_func
#define T_tcc_get_symbol		tcc_get_symbol
#define T_tcc_add_symbol		tcc_add_symbol
#define T_tcc_relocate			tcc_relocate
#define T_tcc_compile_string	tcc_compile_string
#define T_tcc_add_include_path	tcc_add_include_path
#define T_tcc_add_library_path	tcc_add_library_path
#define T_tcc_output_file		tcc_output_file
#endif

int Tcc::numInstances = 0;

#if defined(PLATFORM_WIN32)
Tcc::Tcc(const char *dllName) {
	Init(dllName);
}
#else
Tcc::Tcc(const char *libPath) {
	Init(libPath);
}
#endif

#if defined(COMPILER_MSC)
void Tcc::Init(const char *dllName)
#else
void Tcc::Init(const char *libPath)
#endif
{
	numInstances++;
#if defined(COMPILER_MSC)
	if (dllName == NULL || *dllName == '\0')
		dllName = "libtcc.dll";
	hinstLib = LoadLibrary(TEXT(dllName));
	if (hinstLib == NULL) 
		throw Exc(t_("Tcc library not found"));

	T_tcc_new = (TCCState *(*)(void))GetProcAddress(hinstLib, "tcc_new");
	if (T_tcc_new == NULL) {
		FreeLibrary(hinstLib);
		throw Exc(Format(t_("Tcc function %s not found"), "tcc_new"));
	}
	T_tcc_delete = (void (*)(TCCState *))GetProcAddress(hinstLib, "tcc_delete");
	if (T_tcc_delete == NULL) {
		FreeLibrary(hinstLib);
		throw Exc(Format(t_("Tcc function %s not found"), "tcc_delete"));
	}
	T_tcc_set_output_type = (int (*)(TCCState *s, int output_type))GetProcAddress(hinstLib, 
																		 "tcc_set_output_type");
	if (T_tcc_set_output_type == NULL) {
		FreeLibrary(hinstLib);
		throw Exc(Format(t_("Tcc function %s not found"), "tcc_set_output_type"));
	}
	T_tcc_set_error_func = (void (*)(TCCState *s, void *error_opaque, void (*error_func)
				(void *opaque, const char *msg)))GetProcAddress(hinstLib, "tcc_set_error_func");
	if (T_tcc_set_error_func == NULL) {
		FreeLibrary(hinstLib);
		throw Exc(Format(t_("Tcc function %s not found"), "tcc_set_error_func"));
	}		
	T_tcc_get_symbol = (int (*)(TCCState *s, unsigned long *pval, const char *name))
													GetProcAddress(hinstLib, "tcc_get_symbol");
	if (T_tcc_get_symbol == NULL) {
		FreeLibrary(hinstLib);
		throw Exc(Format(t_("Tcc function %s not found"), "tcc_get_symbol"));
	}
	T_tcc_add_symbol = (int (*)(TCCState *s, const char *name, unsigned long val))
													GetProcAddress(hinstLib, "tcc_add_symbol");
	if (T_tcc_add_symbol == NULL) {
		FreeLibrary(hinstLib);
		throw Exc(Format(t_("Tcc function %s not found"), "tcc_add_symbol"));
	}
	T_tcc_relocate = (int (*)(TCCState *s))GetProcAddress(hinstLib, "tcc_relocate");
	if (T_tcc_relocate == NULL) {
		FreeLibrary(hinstLib);
		throw Exc(Format(t_("Tcc function %s not found"), "tcc_relocate"));
	}
	T_tcc_compile_string = (int (*)(TCCState *s, const char *buf))GetProcAddress(hinstLib, 
																		"tcc_compile_string");
	if (T_tcc_compile_string == NULL) {
		FreeLibrary(hinstLib);
		throw Exc(Format(t_("Tcc function %s not found"), "tcc_compile_string"));
	}	
	T_tcc_add_include_path = (int (*)(TCCState *s, const char *buf))GetProcAddress(hinstLib, 
																		"tcc_add_include_path");
	if (T_tcc_add_include_path == NULL) {
		FreeLibrary(hinstLib);
		throw Exc(Format(t_("Tcc function %s not found"), "tcc_add_include_path"));
	}	
	T_tcc_add_library_path = (int (*)(TCCState *s, const char *buf))GetProcAddress(hinstLib, 
																		"tcc_add_library_path");
	if (T_tcc_add_library_path == NULL) {
		FreeLibrary(hinstLib);
		throw Exc(Format(t_("Tcc function %s not found"), "tcc_add_library_path"));
	}
	T_tcc_output_file = (int (*)(TCCState *s, const char *buf))GetProcAddress(hinstLib, 
																		"tcc_output_file");
	if (T_tcc_output_file == NULL) {
		FreeLibrary(hinstLib);
		throw Exc(Format(t_("Tcc function %s not found"), "tcc_output_file"));
	}	
#endif
    stateTcc = T_tcc_new();
    if (!stateTcc) 
        throw Exc("Tcc can not initialize");
#if __GNUC__
	if (libPath != NULL)
		if (*libPath != '\0' && GetFileExt(libPath) != ".dll")
			SetLibPath(libPath);
#endif

	T_tcc_set_error_func(stateTcc, this, Tcc::DefaultErrorHandler);
    errorMsg = "";
    
    SetOutputMemory();
}

void Tcc::SetOutputExe()			
{
	ASSERT(stateTcc);
	T_tcc_set_output_type(stateTcc, TCC_OUTPUT_EXE);
	outputMemory = false;
};

void Tcc::SetOutputMemory()		 
{
	ASSERT(stateTcc);
	T_tcc_set_output_type(stateTcc, TCC_OUTPUT_MEMORY);
	outputMemory = true;
};
	
void Tcc::DefaultErrorHandler(void* opaque, const char* msg)
{
	Tcc &tcc = *(Tcc *)opaque;
	if (!tcc.errorMsg.IsEmpty())
		tcc.errorMsg.Cat('\n');		// When calling Relocate this handle can get more than one error
	String message = msg;
	int linePos = sizeof("Line ")-1;
	if (message.Left(linePos) == "Line ") {		// Fix the line number in the error message
		int endLinePos = message.Find(':', linePos+1);
		int line = atoi(message.Mid(linePos, endLinePos-linePos));
		message = Format(t_("Line %d"), line - tcc.initProgramLines) + ":" + message.Mid(endLinePos+1);
	}
	tcc.errorMsg.Cat(message);
}

Tcc::~Tcc()
{
	numInstances--;
	if (numInstances == 0 && stateTcc)
		T_tcc_delete(stateTcc);

#if defined(COMPILER_MSC)	
	if (hinstLib)
		FreeLibrary(hinstLib);
#endif
}

void tcc_throw(const char *str)
{
	 throw Exc(str);
}

void Tcc::Compile(const char *my_program)
{
	ASSERT(stateTcc);
	String initString = "// Basic declarations\n"	
						"typedef int bool;\n"
						"#define true  1\n"
						"#define false 0\n"	
						"#define M_PI	3.1415926535897932\n"// It does not seem to be in math.h
						"\n"
						"void throw(char *str);\n"
						"\n";
	initLen = initString.GetCount();
	initProgramLines = 0;
	int pos = 0;
	while((pos = initString.Find('\n', pos)) >= 0) {
		initProgramLines++;
		pos++;
	}
	program = initString + my_program;
		
    if (T_tcc_compile_string(stateTcc, program) != 0) 
        throw Exc(errorMsg);
    if (outputMemory)
    	AddSymbol("throw", (void *)&tcc_throw);
}

void Tcc::AddSymbol(const char *funName, void *fun)
{
	ASSERT(stateTcc);
	if (!outputMemory) 
		throw Exc(t_("Not possible to add symbols if output to file is defined"));
	T_tcc_add_symbol(stateTcc, funName, (unsigned long)fun);
	if (!errorMsg.IsEmpty())
    	throw Exc(errorMsg);
}

void Tcc::Link(const char *fileName)
{
	ASSERT(stateTcc);
	if (outputMemory) {
		if (fileName)
			throw Exc(t_("Not possible to get file if output to memory is defined"));
		if(T_tcc_relocate(stateTcc) != 0)
			throw Exc(errorMsg);
	} else if (!outputMemory) {
		if (!fileName)
			throw Exc(t_("File name is necessary if output to file is defined"));
		if (T_tcc_output_file(stateTcc, fileName) != 0)
			throw Exc(errorMsg);
	}
}

void *Tcc::GetSymbol(const char *funName)
{
	ASSERT(stateTcc);
	if (!outputMemory) 
		throw Exc(t_("Not possible to get symbols if output to file is defined"));
	unsigned long val = 0;
	T_tcc_get_symbol(stateTcc, (unsigned long *)&val, funName);
	if (!errorMsg.IsEmpty())
    	throw Exc(errorMsg);
    		
	return (void *)val;
}

bool Tcc::AddIncludePath(const char *path)
{
	ASSERT(stateTcc);
	bool ret = T_tcc_add_include_path(stateTcc, path) == 0? true: false;
	if (!errorMsg.IsEmpty())
    	throw Exc(errorMsg);
	return ret;
}

bool Tcc::AddLibraryPath(const char *path)
{
	ASSERT(stateTcc);
	bool ret = T_tcc_add_library_path(stateTcc, path) == 0? true: false;
	if (!errorMsg.IsEmpty())
    	throw Exc(errorMsg);
    return ret;	
}

String Tcc::GetProgram() {
	return program.Mid(initLen);
}
