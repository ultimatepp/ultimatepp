#include <CtrlLib/CtrlLib.h>
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

void Shader::Set(const char* var, float v0, float v1, float v2, float v3)
{
	int id = GetUniformId(var);
	if(id >= 0)
		glUniform4f(id, v0, v1, v2, v3);
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


const String fragAlphaMag = 
"uniform sampler2D Texture;                                                                  \n"
"uniform vec4 GlyphColor;                                                                    \n"
"uniform vec4 GlowColor;                                                                     \n"
"uniform vec4 OutlineColor;                                                                  \n"
"                                                                                            \n"
"uniform bool Outline;                                                                       \n"
"uniform bool Glow;                                                                          \n"
"uniform bool Shadow;                                                                        \n"
"                                                                                            \n"
"const float AlphaCenter = 0.5;                                                              \n"
"const float DistanceScale = 15.0;                                                           \n"
"                                                                                            \n"
"uniform float OutlineCenter;                                                                \n"
"uniform float GlowCenter;                                                                   \n"
"                                                                                            \n"
"void main()                                                                                 \n"
"{                                                                                           \n"
"   vec4 color = texture2D(Texture, gl_TexCoord[0].xy);                                      \n"
"   float alpha = color.a;                                                                   \n"
"   float width = fwidth(gl_TexCoord[0].x) * DistanceScale;                                  \n"
"                                                                                            \n"
"   vec4 finalColor = GlyphColor;                                                            \n"
"                                                                                            \n"
"   float ma = smoothstep(AlphaCenter - width, AlphaCenter + width, alpha);                  \n"
"   finalColor.a = ma;                                                                       \n"
"                                                                                            \n"
"   if(Outline)                                                                              \n"
"   {                                                                                        \n"
"       float mo = smoothstep(OutlineCenter - width, OutlineCenter + width, alpha);          \n"
"       finalColor = mix(OutlineColor, finalColor, ma);                                      \n"
"       finalColor.a = mo;                                                                   \n"
"   }                                                                                        \n"
"                                                                                            \n"
"   if(Glow && alpha >= finalColor.a - 0.5)                                                  \n"
"   {                                                                                        \n"
"       float mg = smoothstep(AlphaCenter, GlowCenter, sqrt(alpha));                         \n"
"       finalColor = mix(GlowColor, finalColor, ma);                                         \n"
"       finalColor.a = mg;                                                                   \n"
"   }                                                                                        \n"
"                                                                                            \n"
"   gl_FragColor = finalColor * GlyphColor.a;                                                \n"
"}                                                                                           \n"
;

const String vertAlphaMag =
"void main()                                                                                 \n"
"{                                                                                           \n"
"	gl_TexCoord[0] = gl_MultiTexCoord0;                                                      \n"
"	gl_Position = ftransform();                                                              \n"
"}                                                                                           \n"
;
END_UPP_NAMESPACE