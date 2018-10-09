#include "tess2.h"
#include "tesselator.h"

namespace Upp {

void* stdAlloc(void* userData, unsigned int size)
{
	int* allocated = ( int*)userData;
	return new byte[size];
}

void stdFree(void* userData, void* ptr)
{
	delete[] (byte *)ptr;
}

void Tesselate(const Vector<Vector<Pointf>>& shape, Vector<Pointf>& vertex,
               Vector<Tuple<int, int, int>> *triangle, Vector<int> *index,
               bool evenodd)
{
	float t = 0.0f, pt = 0.0f;
	TESSalloc ma;
	TESStesselator* tess = 0;
	unsigned char* vflags = 0;

	int allocated = 0;
	double t0 = 0, t1 = 0;

	memset(&ma, 0, sizeof(ma));
	ma.memalloc = stdAlloc;
	ma.memfree = stdFree;
	ma.userData = (void*)&allocated;
	ma.extraVertices = 256; // realloc not provided, allow 256 extra vertices.

	tess = tessNewTess(&ma);

	tessSetOption(tess, TESS_CONSTRAINED_DELAUNAY_TRIANGULATION, 1);
	
	for(const auto& contour : shape) {
		Buffer<double> data(2 * contour.GetCount());
		double *t = ~data;
		for(Pointf p : contour) {
			*t++ = p.x;
			*t++ = p.y;
		}
		tessAddContour(tess, 2, data, 2 * sizeof(double), contour.GetCount());
	}
	
	vertex.Clear();
	if(triangle)
		triangle->Clear();
	else
		index->Clear();

	tessTesselate(tess, evenodd ? TESS_WINDING_ODD : TESS_WINDING_POSITIVE, TESS_POLYGONS, 3, 2, 0);

	if (tess) {
		const double* verts = tessGetVertices(tess);
		const int nverts = tessGetVertexCount(tess);
		for(int i = 0; i < nverts; i++)
			vertex.Add(Pointf(verts[2 * i], verts[2 * i + 1]));
		
		const int* elems = tessGetElements(tess);
		const int nelems = tessGetElementCount(tess);

		for(int i = 0; i < nelems; i++) {
			const int* p = &elems[i * 3];
			if(triangle)
				triangle->Add(MakeTuple(p[0], p[1], p[2]));
			else
				*index << p[0] << p[1] << p[2];
		}
	}

	tessDeleteTess(tess);
}

void Tesselate(const Vector<Vector<Pointf>>& shape, Vector<Pointf>& vertex, Vector<Tuple<int, int, int>>& triangle,
               bool evenodd)
{
	Tesselate(shape, vertex, &triangle, NULL, evenodd);
}

void Tesselate(const Vector<Vector<Pointf>>& shape, Vector<Pointf>& vertex, Vector<int>& index,
               bool evenodd)
{
	Tesselate(shape, vertex, NULL, &index, evenodd);
}

};