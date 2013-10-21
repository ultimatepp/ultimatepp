#include "GLDraw.h"

#ifdef GL_USE_SHADERS

namespace Upp {

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
		String error = "shader failed to compile ";
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if(infoLen > 1) {
			Buffer<char> infoLog(infoLen);
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			error << ~infoLog;
		}
		Panic(error);
	}

	return shader;
}

void GLProgram::Create(const char *vertex_shader_, const char *fragment_shader_,
                       Tuple2<int, const char *> *bind_attr, int bind_count)
{
	Clear();

	program = glCreateProgram();

	vertex_shader = LoadShader(vertex_shader_, GL_VERTEX_SHADER);
	fragment_shader = LoadShader(fragment_shader_, GL_FRAGMENT_SHADER);
	
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	
	for(int i = 0; i < bind_count; i++)
		glBindAttribLocation(program, bind_attr[i].a, bind_attr[i].b);

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
		Panic("Failed to link");
		Clear();
	}
	
	Use();
}

void GLProgram::Create(const char *vertex_shader, const char *fragment_shader,
                       int attr1, const char *attr1name)
{
	Tuple2<int, const char *> bind[] = {
		{ attr1, attr1name },
	};
	Create(vertex_shader, fragment_shader, bind, __countof(bind));
}

void GLProgram::Create(const char *vertex_shader, const char *fragment_shader,
                       int attr1, const char *attr1name,
                       int attr2, const char *attr2name)
{
	Tuple2<int, const char *> bind[] = {
		{ attr1, attr1name },
		{ attr2, attr2name },
	};
	Create(vertex_shader, fragment_shader, bind, __countof(bind));
}

void GLProgram::Create(const char *vertex_shader, const char *fragment_shader,
                       int attr1, const char *attr1name,
                       int attr2, const char *attr2name,
                       int attr3, const char *attr3name)
{
	Tuple2<int, const char *> bind[] = {
		{ attr1, attr1name },
		{ attr2, attr2name },
		{ attr3, attr3name },
	};
	Create(vertex_shader, fragment_shader, bind, __countof(bind));
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

};

#endif
