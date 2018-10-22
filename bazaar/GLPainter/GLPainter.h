#ifndef _GLDrawDemo_Ugl_h_
#define _GLDrawDemo_Ugl_h_

#include <GLDraw/GLDraw.h>
#include <plugin/tess2/tess2.h>

namespace Upp {
	
struct GLContext2D { // TODO: This should be changed to regular matrix (later)
	Sizef  vs;
	double alpha = 1;
	
	GLContext2D(Size sz) { vs = Sizef(2.0 / sz.cx, -2.0 / sz.cy); }
	GLContext2D()        {}
};

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
	GLCode& operator()(const char *id, Color c, double alpha = 1);
	GLCode& operator()(int i, Color c, double alpha = 1);
};

class GLTexture {
	struct Data {
		int     refcount = 1;
		GLuint  textureid = 0;
		Size    sz = Size(0, 0);
	};
	
	Data    *data = NULL;
	
	void Set(GLuint texture, Size sz);
	
	friend class GLTextureDraw;
	
public:
	void     Clear();
	void     Set(const Image& img, dword flags = TEXTURE_LINEAR|TEXTURE_MIPMAP);
	
	void     Bind() const       { if(data) glBindTexture(GL_TEXTURE_2D, data->textureid); }
	int      GetID() const      { return data ? data->textureid : 0; }
	operator GLuint() const     { return GetID(); }
	Size     GetSize() const    { return data ? data->sz : Size(0, 0); }

	GLTexture()                 {}
	GLTexture(const Image& img, dword flags = TEXTURE_LINEAR|TEXTURE_MIPMAP) { Set(img, flags); }
	~GLTexture()                { Clear(); }

	GLTexture(const GLTexture& src);
	GLTexture& operator=(const GLTexture& src);
};

void GLBind(const Image& img, dword style = TEXTURE_LINEAR|TEXTURE_MIPMAP);

void GLDrawTexture(const GLContext2D& dd, const Rect& rect, int textureid, double alpha = 1);
void GLDrawTexture(const GLContext2D& dd, const Rect& rect, const Image& img, double alpha = 1);
void GLDrawImage(const GLContext2D& dd, const Rect& rect, const Image& img, double alpha = 1);

class GLTextureDraw {
	GLuint framebuffer = 0;
    GLuint texture = 0;
	GLuint rbo = 0;
	Size   sz;
	int    msaa = 0;

public:
	void      Clear();
	void      Create(Size sz, int msaa = 0);
	GLTexture GetResult();
	operator  GLTexture()                { return GetResult(); }

	GLTextureDraw()                      {}
	GLTextureDraw(Size sz, int msaa = 0) { Create(sz, msaa); }
	~GLTextureDraw()                     { Clear(); }
};

class GLVertexData {
	struct Data {
		int            refcount = 1;
		GLuint         VAO = 0;
		GLuint         EBO = 0;
		int            elements = 0;
	    
		Vector<GLuint> VBO;
	};
	
	Data    *data = NULL;
	
	void     Do();

public:
	void    Clear();

	GLVertexData& Add(const void *data, int type, int ntuple, int count);
	GLVertexData& Add(const float *data, int ntuple, int count) { return Add(data, GL_FLOAT, ntuple, count); }
	GLVertexData& Add(const Vector<float>& data, int ntuple)    { return Add(data, ntuple, data.GetCount() / ntuple); }
	GLVertexData& Add(const Vector<Pointf>& pt);
	GLVertexData& Index(const int *indices, int count);
	GLVertexData& Index(const Vector<int>& indices)             { return Index(indices, indices.GetCount()); }
	
	void    Draw(int mode = GL_TRIANGLES) const;

	void    Draw(GLCode& shaders, int mode = GL_TRIANGLES) const;
	
	operator bool() const                                       { return data; }
	bool    IsEmpty() const                                     { return !data; }

	GLVertexData()                                              {}
	~GLVertexData()                                             { Clear(); }

	GLVertexData(const GLVertexData& src);
	GLVertexData& operator=(const GLVertexData& src);
};

const GLVertexData& GLRectMesh();

#if 0 // this is polygon draw by tesselation - for now abandoned
void GLMakePolygon(GLVertexData& mesh, const Vector<Vector<Pointf>>& polygon);
void GLDrawPolygon(Sizef vs, Point at, const GLVertexData& mesh, Sizef scale, Color color, double alpha);
#endif

template <typename Src>
void GLPolygon(GLVertexData& mesh, const Src& polygon);

void GLDrawPolygon(const GLContext2D& dd, Pointf at, const GLVertexData& mesh, Sizef scale, Color color, double alpha);
void GLDrawConvexPolygon(const GLContext2D& dd, Pointf at, const GLVertexData& mesh, Sizef scale, Color color, double alpha);

template <typename Src>
void GLPolyline(GLVertexData& data, const Src& polygon);

void GLDrawPolyline(const GLContext2D& dd, Pointf at, const GLVertexData& mesh, double scale, double width, Color color, double alpha);

void GLDrawStencil(Color color, double alpha);

void GLDrawEllipse(const GLContext2D& dd, Pointf center, Sizef radius, Color color, double alpha);

#include "GLPainter.hpp"

};

#endif
