#include "GLPainter.h"

namespace Upp {

GLCode::GLCode(const char *vertex_shader, const char *pixel_shader)
{
	Create(vertex_shader, pixel_shader);
	Vector<Tuple2<int, const char *>> ins;
	CParser p(vertex_shader);
	int ii = 0;
	int ti = 0;
	auto readID = [&] {
		String id;
		while(!p.IsEof() && !p.Char(';'))
			if(p.IsId())
				id = p.ReadId();
			else
				p.SkipTerm();
		return id;
	};
	while(!p.IsEof() && !p.Char('{'))
		if(p.Id("attribute") || p.Id("in")) {
			String id = readID();
			if(id.GetCount())
				glBindAttribLocation(program, ii++, id);
		}
		else
			p.SkipTerm();
	
	p.Set(pixel_shader);
	while(!p.IsEof() && !p.Char('{'))
		if(p.Id("sampler2D") || p.Id("sampler3D")) {
			String id = readID();
			if(id.GetCount())
				glUniform1i(GetUniform(id), ti++);
		}
		else
			p.SkipTerm();
}

GLCode& GLCode::Uniform(int i, double a)
{
	Use();
	glUniform1f(i, (float)a);
	return *this;
}

GLCode& GLCode::Uniform(int i, double a, double b)
{
	Use();
	glUniform2f(i, (float)a, (float)b);
	return *this;
}

GLCode& GLCode::Uniform(int i, double a, double b, double c)
{
	Use();
	glUniform3f(i, (float)a, (float)b, (float)c);
	return *this;
}

GLCode& GLCode::Uniform(int i, double a, double b, double c, double d)
{
	Use();
	glUniform4f(i, (float)a, (float)b, (float)c, (float)d);
	return *this;
}

GLCode& GLCode::Uniform(const char *id, double a)
{
	Use();
	glUniform1f(GetUniform(id), (float)a);
	return *this;
}

GLCode& GLCode::Uniform(const char *id, double a, double b)
{
	Use();
	glUniform2f(GetUniform(id), (float)a, (float)b);
	return *this;
}

GLCode& GLCode::Uniform(const char *id, double a, double b, double c)
{
	Use();
	glUniform3f(GetUniform(id), (float)a, (float)b, (float)c);
	return *this;
}

GLCode& GLCode::Uniform(const char *id, double a, double b, double c, double d)
{
	Use();
	glUniform4f(GetUniform(id), (float)a, (float)b, (float)c, (float)d);
	return *this;
}

};
