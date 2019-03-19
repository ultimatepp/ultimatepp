#include "GLDraw.h"

namespace Upp {

void GLDrawPolylines(const GLContext2D& dd, Pointf at, const GLVertexData& mesh, Sizef scale,
                    double width, Color color)
{
	if(IsNull(color) || width <= 0)
		return;

	static GLCode program(R"(
	    attribute vec4 pos;
		uniform vec2 offset;
		uniform vec2 scale;
		uniform vec2 scale2;
		uniform vec2 width;
	    void main()
	    {
			vec2 p = scale * scale2 * pos.xy + offset; // avoiding precision problems
			vec2 v = scale2 * pos.zw;
			gl_Position = vec4(p + scale * width * normalize(vec2(-v.y, v.x)), 0, 1);
	    }
	)", R"(
		uniform vec4 color;
		void main()
		{
			gl_FragColor = color;
		}
	)");

	static int ioffset = program["offset"];
	static int iscale = program["scale"];
	static int iscale2 = program["scale2"];
	static int iwidth = program["width"];
	static int icolor = program["color"];

	program(ioffset, Pointf(dd.vs) * at + dd.off)
	       (iscale, dd.vs)
	       (iscale2, scale)
	       (iwidth, Sizef(width / 2, width / 2))
	       (icolor, color, dd.alpha)
	;
	
	if(dd.alpha == 1) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mesh.Draw(program);
	}
	else {
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	
		glEnable(GL_STENCIL_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_NEVER, 1, 1);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		
		mesh.Draw(program);
		
		GLDrawStencil(color, dd.alpha);
	}
}

void DashPolyline(Vector<Vector<Pointf>>& polyline, const Vector<Pointf>& line,
                  const Vector<double>& pattern, double distance)
{
	struct LineStore : LinearPathConsumer {
		Vector<Vector<Pointf>>& polyline;
	
		void Move(const Pointf& p) override { polyline.Add().Add(p); }
		void Line(const Pointf& p) override { polyline.Top().Add(p); }
		
		LineStore(Vector<Vector<Pointf>>& polyline) : polyline(polyline) {}
	};
	
	LineStore st(polyline);
	Dasher dasher;
	dasher.target = &st;
	dasher.Init(pattern, distance);
	for(int i = 0; i < line.GetCount(); i++)
		if(i)
			dasher.Line(line[i]);
		else
			dasher.Move(line[i]);
	dasher.End();
}

};