#include "GLDraw.h"

namespace Upp {

void GLTriangles::Draw(const GLContext2D& dd)
{
	static GLCode program(R"(
		#version 330 core
	    in vec3 p;
	    in vec3 c;
		uniform vec2 offset;
		uniform vec2 scale;
		out vec4 v_color;
	    void main()
	    {
			gl_Position = vec4(scale * p.xy + offset, 0, 1);
			v_color = vec4(1/255.0 * c.rgb, p.z);
	    }
	)", R"(
		#version 330 core
		in vec4 v_color;
		void main()
		{
			gl_FragColor = v_color;
		}
	)");

	static int ioffset = program["offset"];
	static int iscale = program["scale"];

	GLVertexData va;
	va.Add(pos, 3);
	va.Add(color, 3);
	va.Index(elements);
	
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	
	program(ioffset, Sizef(-1, 1))
	       (iscale, dd.vs)
	;
	
	va.Draw(program);
}

};