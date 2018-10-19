#include "GLPainter.h"

namespace Upp {

void GLDrawPolygon(const GLContext2D& dd, bool generic, Pointf at, const GLVertexData& mesh, Sizef scale, Color color, double alpha)
{
	static GLCode program(R"(
		#version 330 core
	    in vec2 aPos;
		uniform vec2 offset;
		uniform vec2 scale;
	    void main()
	    {
			gl_Position = vec4(scale * aPos + offset, 0, 1);
	    }
	)", R"(
		#version 330 core
		uniform vec4 color;
		void main()
		{
			gl_FragColor = color;
		}
	)");
	
	static int ioffset = program["offset"];
	static int iscale = program["iscale"];
	static int icolor = program["color"];

	program("offset", Pointf(dd.vs) * at + Sizef(-1, 1))
	       ("scale", dd.vs * scale)
	       ("color", color.GetR() / 255.0f, color.GetG() / 255.0f, color.GetB() / 255.0f, dd.alpha * alpha);

	if(generic) {
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	
		glEnable(GL_STENCIL_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_NEVER, 0, 1);
		glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
		
		mesh.Draw(program);
	
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_EQUAL, 1, 1);
		glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	program("offset", Pointf(dd.vs) * at + Sizef(-1, 1))
	       ("scale", dd.vs * scale)
	       ("color", color.GetR() / 255.0f, color.GetG() / 255.0f, color.GetB() / 255.0f, dd.alpha * alpha);

	GLRectMesh().Draw(program("offset", -1, -1)("scale", 2, 2));

	if(generic)
		glDisable(GL_STENCIL_TEST);
}

void GLDrawPolygon(const GLContext2D& dd, Pointf at, const GLVertexData& mesh, Sizef scale, Color color, double alpha)
{
	GLDrawPolygon(dd, true, at, mesh, scale, color, alpha);
}

void GLDrawConvexPolygon(const GLContext2D& dd, Pointf at, const GLVertexData& mesh, Sizef scale, Color color, double alpha)
{
	GLDrawPolygon(dd, false, at, mesh, scale, color, alpha);
}

};