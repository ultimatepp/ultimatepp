#include <CtrlLib/CtrlLib.h>

#ifdef GUI_LINUXGL

#include "Shaders.h"

NAMESPACE_UPP

void Shader::PrintShaderInfoLog(GLuint obj, const char* fileName)
{
	int infoLength = 0;
	int charsWritten = 0;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infoLength);

	if(infoLength > 1)
	{
		char* info = new char[infoLength];
		glGetShaderInfoLog(obj, infoLength, &charsWritten, info);
		if(charsWritten > 0)
		{
			compileError = info;
			FileOut f(fileName);
			f.Put(info);
		}
		delete[] info;
	}
}

void Shader::PrintProgramInfoLog(GLuint obj, const char* fileName)
{
	int infoLength = 0;
	int charsWritten = 0;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infoLength);

	if(infoLength > 1)
	{
		char* info = new char[infoLength];
		glGetShaderInfoLog(obj, infoLength, &charsWritten, info);
		if(charsWritten > 0)
		{
			FileOut f(fileName);
			f.Put(info);
		}
		delete[] info;
	}
}

void Shader::PrintProgramValidationLog(GLuint obj, const char* fileName)
{
	char log[1024];
	int len = 0;
	glValidateProgram(obj);
	glGetProgramInfoLog(obj, 1024, &len, log);
	if(len > 0)
	{
		FileOut f(fileName);
		f.Put(log);
	}
}

bool Shader::CheckGLError()
{
	unsigned int lastError = glGetError();
	if(GL_NO_ERROR != lastError)
	{
		error = (const char*) gluErrorString(lastError);
		return false;
	}
	return true;
}

bool Shader::IsProgramCompiled(int program)
{
	GLint result = GL_FALSE;
	glGetShaderiv(program, GL_COMPILE_STATUS, &result);
	return result == GL_TRUE;
}

bool Shader::IsProgramLinked(int program)
{
	GLint result = GL_FALSE;
	glGetShaderiv(program, GL_LINK_STATUS, &result);
	return result == GL_TRUE;
}

int Shader::CompileProgram(const char* vs, const char* fs)
{
	vertSrc = vs;
	fragSrc = fs;
	
	const GLchar* vertexShaderSrc = (const GLchar*) vertSrc;
	const GLchar* fragmentShaderSrc = (const GLchar*) fragSrc;
	
	DeleteFile("shader_vertex.log");
	DeleteFile("shader_fragment.log");
	DeleteFile("shader_link.log");
	DeleteFile("shader_validation.log");

	GLenum vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);
	
	if(!IsProgramCompiled(vertexShader))
	{
		PrintShaderInfoLog(vertexShader, "shader_vertex.log");
		error = "Vertex shader compilation error:\n\n" + compileError;
		return -1;
	}
	
	GLenum fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);
	
	if(!IsProgramCompiled(fragmentShader))
	{
		PrintShaderInfoLog(fragmentShader, "shader_fragment.log");
		error = "Fragment shader compilation error:\n\n" + compileError;
		return -1;
	}

	program = glCreateProgram();
	
	if(program > 0)
	{
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		if(!IsProgramLinked(program))
		{
			PrintProgramInfoLog(program, "shader_link.log");
			PrintProgramValidationLog(program, "shader_validation.log");
			return -1;
		}
	}
	else
		program = -1;
	
	return program;
}

int Shader::CompileProgram(const byte* vs, const byte* fs)
{
	return CompileProgram((const char*) vs, (const char*) fs);
}

int Shader::GetProgram()
{
	return program;
}

String Shader::GetError()
{
	return error;
}

int Shader::GetUniformId(const char* var)
{
	int n = uniforms.Find(var);
	if(n < 0)
	{
		int id = glGetUniformLocation(program, var);
		CheckGLError();
		if(id >= 0)
			uniforms.Add(var, id);
		return id;
	}
	else
		return uniforms[n];
}

void Shader::Set(const char* var, float v)
{
	int id = GetUniformId(var);
	if(id >= 0)
		glUniform1f(id, v);
}

void Shader::Set(const char* var, bool v)
{
	int id = GetUniformId(var);
	if(id >= 0)
		glUniform1i(id, int(v));
}

void Shader::Set(const char* var, int v)
{
	int id = GetUniformId(var);
	if(id >= 0)
		glUniform1i(id, v);
}

void Shader::Set(const char* var, float v0, float v1)
{
	int id = GetUniformId(var);
	if(id >= 0)
		glUniform2f(id, v0, v1);
}

void Shader::Set(const char* var, float v0, float v1, float v2)
{
	int id = GetUniformId(var);
	if(id >= 0)
		glUniform3f(id, v0, v1, v2);
}

void Shader::Set(const char* var, float v0, float v1, float v2, float v3)
{
	int id = GetUniformId(var);
	if(id >= 0)
		glUniform4f(id, v0, v1, v2, v3);
}

void Shader::Set(const char* var, float* v, int size, int count)
{
	int id = GetUniformId(var);
	if(id >= 0)
	{
		if(size == 2)
			glUniform2fv(id, count, v);
		else if(size == 3)
			glUniform3fv(id, count, v);
		else if(size == 4)
			glUniform4fv(id, count, v);			
	}
}

void Shader::Start()
{
	glUseProgram(program);
	CheckGLError();
}

void Shader::Stop()
{
	glUseProgram(0);
}

Shader::Shader() : program(-1)
{}

END_UPP_NAMESPACE

#endif