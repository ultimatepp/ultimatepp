#include "GLPainter.h"

#if 0
namespace Upp {

void GLMakePolygon(GLVertexData& mesh, const Vector<Vector<Pointf>>& polygon)
{
	Vector<Pointf> vertex;
	Vector<int> ndx;
	Tesselate(polygon, vertex, ndx);
	
	mesh.Add(vertex).Index(ndx);
}

void GLDrawPolygon(Sizef vs, Point at, const GLVertexData& mesh, Sizef scale, Color color, double alpha)
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

			RTIMING("Poly");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mesh.Draw(
		program("offset", vs * at + Sizef(-1, 1))
		       ("scale", vs * scale)
		       ("color", color.GetR() / 255.0f, color.GetG() / 255.0f, color.GetB() / 255.0f, alpha)
	);
}

}
#endif