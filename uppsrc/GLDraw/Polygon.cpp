#include "GLDraw.h"

namespace Upp {

GLCode& GLSimpleCode()
{
	static GLCode program(R"(
	    attribute vec2 aPos;
		uniform vec2 offset;
		uniform vec2 scale;
	    void main()
	    {
			gl_Position = vec4(scale * aPos + offset, 0, 1);
	    }
	)", R"(
		uniform vec4 color;
		void main()
		{
			gl_FragColor = color;
		}
	)");
	return program;
}
	
void GLDrawStencil(Color color, double alpha)
{
	GLCode& program = GLSimpleCode();

	static int ioffset = program["offset"];
	static int iscale = program["scale"];
	static int icolor = program["color"];

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLRectMesh().Draw(program(ioffset, -1, -1)(iscale, 2, 2)(icolor, color, alpha));

	glDisable(GL_STENCIL_TEST);
}

void GLDrawPolygons(const GLContext2D& dd, bool generic, Pointf at, const GLVertexData& mesh, Sizef scale, Color color)
{
	GL_TIMING("GLDrawPolygons");

	if(IsNull(color))
		return;

	GLCode& program = GLSimpleCode();

	static int ioffset = program["offset"];
	static int iscale = program["scale"];
	static int icolor = program["color"];

	program(ioffset, Pointf(dd.vs) * at + dd.off)
	       (iscale, dd.vs * scale);

	if(generic) {
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	
		glEnable(GL_STENCIL_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_NEVER, 0, 1);
		glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
		
		mesh.Draw(program);
		
		GLDrawStencil(color, dd.alpha);
	}
	else {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mesh.Draw(program(icolor, color, dd.alpha));
	}
}

void GLDrawPolygons(const GLContext2D& dd, Pointf at, const GLVertexData& mesh, Sizef scale, Color color)
{
	GLDrawPolygons(dd, true, at, mesh, scale, color);
}

void GLDrawConvexPolygons(const GLContext2D& dd, Pointf at, const GLVertexData& mesh, Sizef scale, Color color)
{
	GLDrawPolygons(dd, false, at, mesh, scale, color);
}

};