#include "GLDraw.h"

namespace Upp {

void GLTriangles::Draw(const GLContext2D& dd)
{
	if(elements.GetCount() == 0)
		return;

	static GLCode program(R"(
	#version 130
	    attribute vec3 p;
	    attribute uvec3 col;
		uniform vec2 offset;
		uniform vec2 scale;
		varying vec4 v_color;
	    void main()
	    {
			gl_Position = vec4(scale * p.xy + offset, 0, 1);
			vec3 c = col;
			v_color = vec4(1/255.0 * c.x, 1/255.0 * c.y, 1/255.0 * c.z, p.z);
	    }
	)", R"(
		varying vec4 v_color;
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
	
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);
	
	program(ioffset, dd.off)
	       (iscale, dd.vs)
	;
	
	va.Draw(program);
}

};
