#include "GLDraw.h"

namespace Upp {

GLCode::GLCode(const char *vertex_shader, const char *pixel_shader)
{
	Compile(vertex_shader, pixel_shader);
	Vector<Tuple2<int, const char *>> ins;
	CParser p(vertex_shader);
	auto readID = [&](int& n) {
		String id;
		n = 0;
		while(!p.IsEof() && !p.IsChar(';'))
			if(p.IsId()) {
				id = p.ReadId();
				if(p.Char('[') && p.IsInt()) {
					n = p.ReadInt();
					p.Char(']');
				}
			}
			else {
				p.SkipTerm();
				n = 0;
			}
		return id;
	};
	int ii = 0;
	while(!p.IsEof() && !p.Char('{'))
		if(p.Id("attribute") || p.Id("in")) {
			int n;
			String id = readID(n);
			if(id.GetCount())
				glBindAttribLocation(program, ii++, id);
		}
		else
			p.SkipTerm();
	
	Link();
	Use();

	p.Set(pixel_shader);
	int ti = 0;
	while(!p.IsEof() && !p.Char('{'))
		if(p.Id("sampler2D") || p.Id("sampler3D")) {
			int n;
			String id = readID(n);
			if(id.GetCount()) {
				if(n) {
					for(int i = 0; i < n; i++) {
						ASSERT(GetUniform(id + '[' + AsString(i) + ']') >= 0);
						glUniform1i(GetUniform(id + '[' + AsString(i) + ']'), ti++);
					}
				}
				else
					glUniform1i(GetUniform(id), ti++);
			}
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

GLCode& GLCode::operator()(const char *id, Color c, double alpha)
{
	return Uniform(GetUniform(id), c, alpha);
}

GLCode& GLCode::operator()(int i, Color c, double alpha)
{
	return Uniform(i, c.GetR() / 255.0f, c.GetG() / 255.0f, c.GetB() / 255.0f, alpha);
}

GLCode& GLCode::Mat4(int i, float *mat4)
{
	glUniformMatrix4fv(i, 1, GL_FALSE, mat4);
	return *this;
}

GLCode& GLCode::Mat4(const char *id, float *mat4)
{
	return Mat4(GetUniform(id), mat4);
}

};
