#ifndef _CoreGl_Shaders_h_
#define _CoreGl_Shaders_h_

#include <Core/Core.h>
#include <Draw/Draw.h>
#include "Gl.h"

NAMESPACE_UPP

class Shader
{
private:
	int program;
	String error;
	String compileError;
	String fragSrc;
	String vertSrc;
	String geomSrc;
	VectorMap<String, int> uniforms;
	VectorMap<String, int> attributes;
	
	void PrintShaderInfoLog(GLuint obj, const char* fileName);
	void PrintProgramInfoLog(GLuint obj, const char* fileName);
	void PrintProgramValidationLog(GLuint obj, const char* fileName);
	bool CheckGLError();
	bool IsProgramCompiled(int program);
	bool IsProgramLinked(int program);
	
public:
	int  GetUniformId(const char* var);
	int  GetAttributeId(const char* var);

	int  CompileProgram(const char* vs, const char* fs, const char* gs = NULL);
	int  CompileProgram(const byte* vs, const byte* fs, const byte* gs = NULL);
	int  GetProgram();

	void SetUniform(const char* var, float v);
	void SetUniform(const char* var, bool v);
	void SetUniform(const char* var, int v);
	void SetUniform(const char* var, float v0, float v1);
	void SetUniform(const char* var, float v0, float v1, float v2);
	void SetUniform(const char* var, float v0, float v1, float v2, float v3);
	void SetUniform(const char* var, float* v, int size, int count = 1);
	void SetMatrixUniform(const char* var, float* v, int size, int count = 1);
	
	void EnableAttribute(const char* var, bool b = true);
	void DisableAttribute(const char* var);
	void DisableAllAtrributes();

	void SetAttribute(const char* var, float v);
	void SetAttribute(const char* var, float v0, float v1);
	void SetAttribute(const char* var, float v0, float v1, float v2);
	void SetAttribute(const char* var, float v0, float v1, float v2, float v3);
	void SetAttribute(const char* var, float* v, int size);

	void Start();
	void Stop();
	String GetError();
	void Release();
	
	Shader();
};

END_UPP_NAMESPACE
#endif
