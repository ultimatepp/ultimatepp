#if defined(flagWINGL) || defined(flagLINUXGL)

#include "Shaders.h"

NAMESPACE_UPP

void Shader::PrintShaderInfoLog(GLuint obj, const char* fileName)
{
	int infoLength = 0;
	int charsWritten = 0;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infoLength);

	if(infoLength > 0)
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

	if(infoLength > 0)
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
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	return result == GL_TRUE;
}

int Shader::CompileProgram(const char* vs, const char* fs, const char* gs)
{
	vertSrc = vs;
	fragSrc = fs;
	geomSrc = gs;
	
	const GLchar* vertexShaderSrc = (const GLchar*) vertSrc;
	const GLchar* fragmentShaderSrc = (const GLchar*) fragSrc;
	const GLchar* geometryShaderSrc = (const GLchar*) geomSrc;
	
	DeleteFile("shader_vertex.log");
	DeleteFile("shader_fragment.log");
	DeleteFile("shader_geometry.log");
	DeleteFile("shader_link.log");
	DeleteFile("shader_validation.log");

	GLenum vertexShader = 0;
	GLenum fragmentShader = 0;
	GLenum geometryShader = 0;
	
	if(!vertSrc.IsEmpty())
	{
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
		glCompileShader(vertexShader);
		
		if(!IsProgramCompiled(vertexShader))
		{
			PrintShaderInfoLog(vertexShader, "shader_vertex.log");
			error = "Vertex shader compilation error:\n\n" + compileError;
			glDeleteShader(vertexShader);
			return -1;
		}
	}

	if(!fragSrc.IsEmpty())
	{
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
		glCompileShader(fragmentShader);
		
		if(!IsProgramCompiled(fragmentShader))
		{
			PrintShaderInfoLog(fragmentShader, "shader_fragment.log");
			error = "Fragment shader compilation error:\n\n" + compileError;
			glDeleteShader(fragmentShader);
			return -1;
		}
	}

	if(!geomSrc.IsEmpty())
	{
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometryShaderSrc, NULL);
		glCompileShader(geometryShader);
		
		if(!IsProgramCompiled(geometryShader))
		{
			PrintShaderInfoLog(geometryShader, "shader_geometry.log");
			error = "Geometry shader compilation error:\n\n" + compileError;
			glDeleteShader(geometryShader);
			return -1;
		}
	}

	program = glCreateProgram();
	
	if(program > 0)
	{
		if(vertexShader > 0)
			glAttachShader(program, vertexShader);
		if(fragmentShader > 0)
			glAttachShader(program, fragmentShader);
		if(geometryShader > 0)
			glAttachShader(program, geometryShader);
		
		glLinkProgram(program);
		if(!IsProgramLinked(program))
		{
			PrintProgramInfoLog(program, "shader_link.log");
			PrintProgramValidationLog(program, "shader_validation.log");
			glDeleteProgram(program);
			program = -1;
		}
	}
	else
		program = -1;
	
	return program;
}

int Shader::CompileProgram(const byte* vs, const byte* fs, const byte* gs)
{
	return CompileProgram((const char*) vs, (const char*) fs, (const char*) gs);
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

int Shader::GetAttributeId(const char* var)
{
	int n = attributes.Find(var);
	if(n < 0)
	{
		int id = glGetAttribLocation(program, var);
		CheckGLError();
		if(id >= 0)
			attributes.Add(var, id);
		return id;
	}
	else
		return attributes[n];
}

void Shader::SetUniform(const char* var, float v)
{
	int id = GetUniformId(var);
	if(id >= 0)
		glUniform1f(id, v);
}

void Shader::SetUniform(const char* var, bool v)
{
	int id = GetUniformId(var);
	if(id >= 0)
		glUniform1i(id, int(v));
}

void Shader::SetUniform(const char* var, int v)
{
	int id = GetUniformId(var);
	if(id >= 0)
		glUniform1i(id, v);
}

void Shader::SetUniform(const char* var, float v0, float v1)
{
	int id = GetUniformId(var);
	if(id >= 0)
		glUniform2f(id, v0, v1);
}

void Shader::SetUniform(const char* var, float v0, float v1, float v2)
{
	int id = GetUniformId(var);
	if(id >= 0)
		glUniform3f(id, v0, v1, v2);
}

void Shader::SetUniform(const char* var, float v0, float v1, float v2, float v3)
{
	int id = GetUniformId(var);
	if(id >= 0)
		glUniform4f(id, v0, v1, v2, v3);
}

void Shader::SetUniform(const char* var, float* v, int size, int count)
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
		else if(size == 16)
			glUniformMatrix4fv(id, count, GL_FALSE, v);
	}
}

void Shader::SetMatrixUniform(const char* var, float* v, int size, int count)
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
		else if(size == 16)
			glUniformMatrix4fv(id, count, GL_FALSE, v);
	}
}

void Shader::EnableAttribute(const char* var, bool b)
{
	int id = GetAttributeId(var);
	if(id >= 0)
	{
		if(b)
			glEnableVertexAttribArray(id);
		else
			glDisableVertexAttribArray(id);
	}
}

void Shader::DisableAttribute(const char* var)
{
	EnableAttribute(var, false);
}

void Shader::DisableAllAtrributes()
{
	for(int i = 0; i < attributes.GetCount(); i++)
	{
		int id = attributes[i];
		if(id >= 0)
			glDisableVertexAttribArray(id);
	}
}

void Shader::SetAttribute(const char* var, float v)
{
	int id = GetAttributeId(var);
	if(id >= 0)
		glVertexAttrib1f(id, v);
}

void Shader::SetAttribute(const char* var, float v0, float v1)
{
	int id = GetAttributeId(var);
	if(id >= 0)
		glVertexAttrib2f(id, v0, v1);
}

void Shader::SetAttribute(const char* var, float v0, float v1, float v2)
{
	int id = GetAttributeId(var);
	if(id >= 0)
		glVertexAttrib3f(id, v0, v1, v2);
}

void Shader::SetAttribute(const char* var, float v0, float v1, float v2, float v3)
{
	int id = GetAttributeId(var);
	if(id >= 0)
		glVertexAttrib4f(id, v0, v1, v2, v3);
}

void Shader::SetAttribute(const char* var, float* v, int size)
{
	int id = GetAttributeId(var);
	if(id >= 0)
	{
		if(size == 2)
			glVertexAttrib2fv(id, v);
		else if(size == 3)
			glVertexAttrib3fv(id, v);
		else if(size == 4)
			glVertexAttrib4fv(id, v);
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

void Shader::Release()
{
	if(program >= 0)
		glDeleteProgram(program);
}

Shader::Shader() : program(-1)
{}

END_UPP_NAMESPACE

#endif