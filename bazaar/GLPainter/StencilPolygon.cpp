#include "GLPainter.h"

namespace Upp {

void GLDrawStencilPolygon(Sizef vs, Pointf at, const GLMesh& mesh, Sizef scale, Color color, double alpha)
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

	RTIMING("Stencil");
	
	
	program("offset", Pointf(vs) * at + Sizef(-1, 1))
	       ("scale", vs * scale)
	       ("color", color.GetR() / 255.0f, color.GetG() / 255.0f, color.GetB() / 255.0f, alpha);
	
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

//	glClearStencil(0);
	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_NEVER, 0, 1);
	glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
	
	mesh.Draw(program);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLRectMesh().Draw(
		program("offset", -1, -1)
		       ("scale", 2, 2)
	);

	glDisable(GL_STENCIL_TEST);
}

};