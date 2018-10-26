#include "GLDraw.h"

namespace Upp {

void GLTriangles::Draw(const GLContext2D& dd)
{
	static GLCode program(R"(
		#version 330 core
	    in vec4 p;
	    in vec4 c;
		uniform vec2 offset;
		uniform vec2 scale;
		out vec4 v_color;
	    void main()
	    {
			gl_Position = vec4(scale * p.xy + offset, 0, 1);
			v_color = vec4(c.r / 255.0, c.g / 255.0, c.b / 255.0, p.z);
	    }
	)", R"(
		#version 330 core
		in vec4 v_color;
		void main()
		{
			gl_FragColor = v_color;
			gl_FragColor = vec4(1, 0.5, 0.5, 1);
		}
	)");

	static int ioffset = program["offset"];
	static int iscale = program["scale"];

#if 1
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), pos.begin());
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), color.begin());

	program(ioffset, Sizef(-1, 1))
	       (iscale, dd.vs)
	;

	program.Use();

	glDrawElements(GL_TRIANGLES, elements.GetCount(), GL_INT, elements);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

#else
	GLVertexData va;
	va.Add(pos, 3);
	va.Add(color, GL_BYTE, 3, elements.GetCount());
	va.Index(elements);

	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	
	program(ioffset, Sizef(-1, 1))
	       (iscale, dd.vs)
	;
	
	va.Draw(program);
#endif
}

};