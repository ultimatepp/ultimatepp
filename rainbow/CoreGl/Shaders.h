#ifndef _CoreGl_Shaders_h_
#define _CoreGl_Shaders_h_

class Shader
{
private:
	int program;
	String error;
	String compileError;
	String fragSrc;
	String vertSrc;
	VectorMap<String, int> uniforms;
	
	void PrintShaderInfoLog(GLuint obj, const char* fileName);
	void PrintProgramInfoLog(GLuint obj, const char* fileName);
	void PrintProgramValidationLog(GLuint obj, const char* fileName);
	bool CheckGLError();
	bool IsProgramCompiled(int program);
	bool IsProgramLinked(int program);
	int GetUniformId(const char* var);
	
public:

	int  CompileProgram(const char* vs, const char* fs);
	int  CompileProgram(const byte* vs, const byte* fs);
	int  GetProgram();
	void Set(const char* var, float v);
	void Set(const char* var, bool v);
	void Set(const char* var, int v);
	void Set(const char* var, float v0, float v1);
	void Set(const char* var, float v0, float v1, float v2);
	void Set(const char* var, float v0, float v1, float v2, float v3);
	void Set(const char* var, float* v, int size, int count);
	void Start();
	void Stop();
	String GetError();
	
	Shader();
};

#endif
