#include "SDLGLDraw.h"

static GLuint LoadShader(const char *src, GLenum type) {
	GLuint shader;
	GLint compiled;

	shader = glCreateShader(type);
	if(!shader)
		return 0;

	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if(!compiled) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if(infoLen > 1) {
			Buffer<char> infoLog(infoLen);
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			Panic(infoLog);
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void GLProgram::Create(const char *vertex_shader_src, const char *fragment_shader_src)
{
	Clear();

	program = glCreateProgram();

	vertex_shader = LoadShader(vertex_shader_src, GL_VERTEX_SHADER);
	fragment_shader = LoadShader(fragment_shader_src, GL_FRAGMENT_SHADER);
	
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);

	GLint linked;

	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if(!linked){
		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
		if(infoLen > 1) {
			Buffer<char> infoLog(infoLen);
			glGetProgramInfoLog(program, infoLen, NULL, infoLog);
			Panic(infoLog);
		}
		Clear();
	}
}

void GLProgram::Clear()
{
	if(program)
		glDeleteProgram(program);
	if(vertex_shader)
		glDeleteShader(vertex_shader);
	if(fragment_shader)
		glDeleteShader(fragment_shader);
}

GLProgram::GLProgram()
{
	vertex_shader = fragment_shader = program = 0;
}
