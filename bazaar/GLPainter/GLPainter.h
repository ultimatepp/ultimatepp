#ifndef _GLDrawDemo_Ugl_h_
#define _GLDrawDemo_Ugl_h_

#include <GLDraw/GLDraw.h>
#include <plugin/tess2/tess2.h>

namespace Upp {

struct GLCode : GLProgram {
	GLCode(const char *vertex_shader, const char *pixel_shader);

	int operator[](const char *id)                                             { return GetUniform(id); }
	
	GLCode& Uniform(int i, double a);
	GLCode& Uniform(int i, double a, double b);
	GLCode& Uniform(int i, double a, double b, double c);
	GLCode& Uniform(int i, double a, double b, double c, double d);

	GLCode& Uniform(const char *id, double a);
	GLCode& Uniform(const char *id, double a, double b);
	GLCode& Uniform(const char *id, double a, double b, double c);
	GLCode& Uniform(const char *id, double a, double b, double c, double d);

	GLCode& operator()(const char *id, double a)                               { return Uniform(id, a); }
	GLCode& operator()(const char *id, double a, double b)                     { return Uniform(id, a, b); }
	GLCode& operator()(const char *id, double a, double b, double c)           { return Uniform(id, a, b, c); }
	GLCode& operator()(const char *id, double a, double b, double c, double d) { return Uniform(id, a, b, c, d); }

	GLCode& operator()(int i, double a)                                        { return Uniform(i, a); }
	GLCode& operator()(int i, double a, double b)                              { return Uniform(i, a, b); }
	GLCode& operator()(int i, double a, double b, double c)                    { return Uniform(i, a, b, c); }
	GLCode& operator()(int i, double a, double b, double c, double d)          { return Uniform(i, a, b, c, d); }

	GLCode& operator()(const char *id, Pointf p)                               { return Uniform(id, p.x, p.y); }
	GLCode& operator()(int i, Pointf p)                                        { return Uniform(i, p.x, p.y); }
	GLCode& operator()(const char *id, Sizef sz)                               { return Uniform(id, sz.cx, sz.cy); }
	GLCode& operator()(int i, Sizef sz)                                        { return Uniform(i, sz.cx, sz.cy); }
};

/*
class GLTexture {
	int textureid = 0;
	
public:
	void Clear() { if(textureid) 
	void Set(const Image& img, dword flags);
	
	operator GLuint() const     { return textureid; }
	GLuint operator~() const    { return textureid; }
	void     Bind() const       { 

	GLTexture();
	GLTexture(const Image& img) { Set(img); }
	~GLTexture()                { Clear(); }
}
*/

Sizef GLMakeViewScale(Size view_size);

void GLBind(const Image& img, dword style = TEXTURE_LINEAR|TEXTURE_MIPMAP);

void GLDrawImage(Sizef vs, const Rect& rect, const Image& img, double alpha);

class GLMesh {
	GLuint   VAO = 0;
	GLuint   EBO = 0;
	int      elements;
    
	Vector<GLuint> VBO;
	
	void     Make();

public:
	GLMesh& Add(const void *data, int type, int ntuple, int count);
	GLMesh& Add(const float *data, int ntuple, int count)           { return Add(data, GL_FLOAT, ntuple, count); }
	GLMesh& Add(const Vector<Pointf>& pt);
	GLMesh& Index(const int *indices, int count);
	GLMesh& Index(const Vector<int>& indices)                       { return Index(indices, indices.GetCount()); }
	
	void    Draw(int mode = GL_TRIANGLES) const;

	void    Draw(GLCode& shaders, int mode = GL_TRIANGLES) const;
	
	void    Clear();
	
	bool    IsEmpty() const                                         { return VAO == 0; }

	GLMesh();
	~GLMesh();
};

const GLMesh& GLRectMesh();

void GLMakePolygon(GLMesh& mesh, const Vector<Vector<Pointf>>& polygon);
void GLDrawPolygon(Sizef vs, Point at, const GLMesh& mesh, Sizef scale, Color color, double alpha);

template <typename Src>
void GLStencilPolygon(GLMesh& mesh, const Src& polygon)
{
	Vector<Pointf> vertex; // todo: Optimize!
	Vector<int> ndx;
	
	for(const auto& p: polygon) {
		int i0 = vertex.GetCount();
		for(int i = 0; i < p.GetCount(); i++) {
			if(i > 1)
				ndx << i0 << i0 + i - 1 << i0 + i;
			vertex.Add(p[i]);
		}
	}
	
	mesh.Add(vertex).Index(ndx);
}

void GLDrawStencilPolygon(Sizef vs, Pointf at, const GLMesh& mesh, Sizef scale, Color color, double alpha);

};

#endif
