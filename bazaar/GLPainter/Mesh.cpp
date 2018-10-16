#include "GLPainter.h"

namespace Upp {

GLMesh::GLMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
}

GLMesh::~GLMesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    for(auto h : VBO)
        glDeleteBuffers(1, &h);
}

GLMesh& GLMesh::Add(const void *data, int type, int ntuple, int count)
{
    glBindVertexArray(VAO);
    int ii = VBO.GetCount();
	GLuint& vbo = VBO.Add();
	glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    int sz = decode(type, GL_FLOAT, sizeof(float),
	                      GL_BYTE, sizeof(byte),
	                      GL_UNSIGNED_BYTE, sizeof(byte),
	                      GL_SHORT, sizeof(int16),
	                      GL_UNSIGNED_SHORT, sizeof(uint16),
	                      GL_INT, sizeof(int32),
	                      GL_UNSIGNED_INT, sizeof(uint32),
	                      (int)sizeof(double));
	glBufferData(GL_ARRAY_BUFFER, sz * ntuple * count, data, GL_STATIC_DRAW);
	glVertexAttribPointer(ii, ntuple, type, GL_FALSE, ntuple * sz, (void*)0);
    glEnableVertexAttribArray(ii);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return *this;
}

GLMesh& GLMesh::Index(const int *indices, int count)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * count, indices, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	elements = count;
	return *this;
}

void GLMesh::Draw(int mode) const
{
	glBindVertexArray(VAO);
	glDrawElements(mode, elements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GLMesh::Draw(GLCode& shaders, int mode) const
{
	shaders.Use();
	Draw(mode);
}

GLMesh& GLMesh::Add(const Vector<Pointf>& pt)
{
	Buffer<float> f(2 * pt.GetCount());
	float *t = f;
	for(const Pointf& p : pt) {
		*t++ = (float)p.x;
		*t++ = (float)p.y;
	}
	return Add(f, GL_FLOAT, 2, pt.GetCount());
}

};