#include "GLPainter.h"

namespace Upp {

void GLDrawEllipse(const GLContext2D& dd, Pointf center, Sizef radius, Color color, double alpha)
{
	static GLCode program(R"(
		#version 330 core
	    in vec2 aPos;
	    void main()
	    {
			gl_Position = vec4(2 * aPos - vec2(1, 1), 0, 1);
	    }
	)", R"(
		#version 330 core
		uniform vec2 center;
		uniform vec2 radius1;
		uniform vec4 color;
		void main()
		{
			gl_FragColor = color;
			vec2 h = radius1 * (gl_FragCoord.xy - center);
			h = h * h;
			if(h.x + h.y > 1)
				gl_FragColor.a = 0;
		}
	)");

	static int icenter = program["center"];
	static int iradius1 = program["radius1"];
	static int icolor = program["color"];

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	
	DDUMP(Sizef(center) * dd.vs + Sizef(-1, 1));

	GLRectMesh().Draw(
		program(icenter, center)
		       (iradius1, Sizef(1 / radius.cx, 1 / radius.cy))
		       (icolor, color.GetR() / 255.0f, color.GetG() / 255.0f, color.GetB() / 255.0f, dd.alpha * alpha)
	);
}

};