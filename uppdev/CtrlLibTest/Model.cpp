#include "Model.h"

bool ModelMesh::Load(String file, VBOLibrary &lib, Point3f scale)
{
	FileIn fin(file);
	if (!fin.IsOpen())
		return false;
	Vector<Triangle> t;
	Vector<Vertex> v;
	bool smoothnormals;
	s % alphasort % smoothnormals;
	v.Serialize(fin);
	t.Serialize(t);
	Scale(v, t, scale, smoothnormals);
	vbo = lib.GetVertexBuffer(v);
	if (!vbo) return false;
	ibo = lib.GetIndexBuffer(t);
	return ibo > 0;
}

void ModelMesh::Draw()
{
	#error Do model drawing
}

void ModelMesh::Scale(Vector<Vertex> &v, Vector<Vertex> &tri, Point3f scale, bool smooth_normals)
{
	for (int i = 0; i < v.GetCount(); i++) {
		v[i].pos.x *= scale.x;
		v[i].pos.y *= scale.y;
		v[i].pos.z *= scale.z;	
		v[i].normal = Point3f(0.0f, 0.0f, 0.0f);
	}
	if (smooth_normals) {
		for (int i = 0; i < tri.GetCount(); i++) {
			Triangle &t = tri[i];
			Point3f n = t.GetNormal(v[t.a].pos, v[t.b].pos, v[t.v].pos);
			v[t.a].normal += n;
			v[t.b].normal += n;
			v[t.c].normal += n;
		}	
		for (int i = 0; i < v.GetCount(); i++)
			v[i].normal.Unit();
	}
	else
		for (int i = 0; i < tri.GetCount(); i++) {
			Triangle &t = tri[i];
			Point3f n = t.GetNormal(v[t.a].pos, v[t.b].pos, v[t.v].pos);
			v[t.a].normal = n;
			v[t.b].normal = n;
			v[t.c].normal = n;
		}
}

void CreateUpStair(Vector<Vertex> &v, Vector<Quad> &t, float floorcy)
{
	float landingcx = 0.2;
	float stairstart = 1.0f - landingcx;
	// 0 - 3 Landing upper face
	v.Add(Vertex(0.0f, 0.0f, 0.5f));		
	v.Add(Vertex(landingcx, 0.0f, 0.5f));
	v.Add(Vertex(landingcx, 1.0f, 0.5f));		
	v.Add(Vertex(0.0f, 1.0f, 0.5f));			
	// 4 - 7 Landing lower face
	v.Add(Vertex(0.0f, 0.0f, 0.5f - floorcy));		
	v.Add(Vertex(landingcx, 0.0f, 0.5f - floorcy));
	v.Add(Vertex(landingcx, 1.0f, 0.5f - floorcy));		
	v.Add(Vertex(0.0f, 1.0f, 0.5f - floorcy));			
	// 8 - 10 Ramp upper face
	v.Add(Vertex(stairstart, stairstart, 0.0f));		
	v.Add(Vertex(stairstart, 1.0f, 0.0f));
	v.Add(Vertex(landingcx, 1.0f, 0.5f));
	v.Add(Vertex(landingcx, stairstart, 0.5f));
	// 11 - 13 Ramp lower face
	v.Add(Vertex(stairstart, stairstart, 0.5f - floorcy));		
	v.Add(Vertex(stairstart, 1.0f, -floorcy));
	v.Add(Vertex(landingcx, 1.0f, 0.5f - floorcy));
	v.Add(Vertex(landingcx, stairstart, 0.5f - floorcy));
	
	float a = floorcy
	float c = 1.0f - landingcx*2;
	float d = 0.5f - floorcy;
	
	AddCube(t, 0, true, true, true);
	AddCube(t, 8, true, false, true);	
}

void CreateDownStair(Vector<Vertex> &v, Vector<Quad> &t, float floorcy)
{
	float landingcx = 0.2;
	float stairend = 1.0f - landingcx;
	floorcy = -floorcy;
	// 0 - 3 Floor pt1 upper face
	v.Add(Vertex(0.0f, landingcx, 0.0));		
	v.Add(Vertex(1.0f, landingcx, 0.0));
	v.Add(Vertex(1.0f, 1.0f, 0.0));		
	v.Add(Vertex(0.0f, 1.0f, 0.0));
	// 4 - 7 Floor pt1 lower face
	v.Add(Vertex(0.0f, landingcx, floorcy));		
	v.Add(Vertex(1.0f, landingcx, floorcy));
	v.Add(Vertex(1.0f, 1.0f, floorcy));		
	v.Add(Vertex(0.0f, 1.0f, floorcy));		
	// 8 - 11 Floor pt2 upper face
	v.Add(Vertex(stairend, 0.0f, 0.0f));		
	v.Add(Vertex(1.0f, 0.0f, 0.0f));
	v.Add(Vertex(1.0f, landingcx, 0.0f));		
	v.Add(Vertex(stairend, landingcx, 0.0f));
	// 12 - 15 Floor pt2 lower face
	v.Add(Vertex(stairend, 0.0f, floorcy));		
	v.Add(Vertex(1.0f, 0.0f, floorcy));
	v.Add(Vertex(1.0f, landingcx, floorcy));		
	v.Add(Vertex(stairend, landingcx, floorcy));
	// 16 - 19 Ramp upper face
	v.Add(Vertex(landingcx, 0.0f, 0.5f));		
	v.Add(Vertex(stairend, 0.0f, 0.5f));
	v.Add(Vertex(stairend, landingcx, 0.0f));
	v.Add(Vertex(landingcx, landingcx, 0.0f));
	// 20 - 23 Ramp lower face
	v.Add(Vertex(landingcx, 0.0f, 0.5f+floorcy));		
	v.Add(Vertex(stairend, 0.0f, 0.5f+floorcy));
	v.Add(Vertex(stairend, landingcx, floorcy));
	v.Add(Vertex(landingcx, landingcx, floorcy));
	
	// Upper/lower faces
	AddCube(t, 0, true, true, true);
	AddCube(t, 8, true, true, true);
	t.Remove(t.GetCount()-2); // Remove top face	
	AddCube(t, 16, true, true, true);
	t.Remove(t.GetCount()-3); // Remove right face
}

void CreateTrunk(Vector<Vertex> &v, Vector<Quad> &t)
{
	float thick = 0.1;
	float height = 0.3;
	Point3f c(0.5, 0.5, 0.0f)
	v.Add(Vertex(c.x - thick, c.y - thick, c.z));
	v.Add(Vertex(c.x + thick, c.y - thick, c.z));
	v.Add(Vertex(c.x + thick, c.y + thick, c.z));
	v.Add(Vertex(c.x - thick, c.y + thick, c.z));
	c.z += height;
	v.Add(Vertex(c.x - thick, c.y - thick, c.z));
	v.Add(Vertex(c.x + thick, c.y - thick, c.z));
	v.Add(Vertex(c.x + thick, c.y + thick, c.z));
	v.Add(Vertex(c.x - thick, c.y + thick, c.z));
	AddCube(t, 0, false, true, true);
}

void CreateDownStair(Vector<Vertex> &v, Vector<Quad> &t, float floorcy)
{
	float landingcx = 0.2;
	float stairend = 1.0f - landingcx;

	CreateDownStair(v, t, floorcy);
	v[2].y = stairend;
	v[3].y = stairend;
	v[6].y = stairend;
	v[7].y = stairend;
	CreateUpStair(v, t, floorcy);
	int pt3ix = v.GetCount();
	// Floor pt3 upper face
	v.Add(Vertex(stairend, 0.0f, 0.0f));		
	v.Add(Vertex(1.0f, 0.0f, 0.0f));
	v.Add(Vertex(1.0f, 1.0f, 0.0f));		
	v.Add(Vertex(stairend, 1.0f, 0.0f));
	// Floor pt3 lower face
	v.Add(Vertex(stairend, 0.0f, floorcy));		
	v.Add(Vertex(1.0f, 0.0f, floorcy));
	v.Add(Vertex(1.0f, 1.0f, floorcy));		
	v.Add(Vertex(stairend, 1.0f, floorcy));	
	
	// Upper/lower faces
	AddCube(t, pt3ix, true, true, true);
	t.Remove(t.GetCount() - 4); // Remove left face	
	t.Remove(t.GetCount() - 2); // Remove bottom face	
}

void AddCube(Vector<Quad> &t, int ix, bool vertical, bool sides, bool ends)
{
	// Up/Down
	if (vertical) {
		AddQuad(t, ix+0, ix+1, ix+2, ix+3); // Up
		AddQuad(t, ix+7, ix+6, ix+5, ix+4);	// Down
	}
	if (sides) {
		// Sides
		AddQuad(t, ix+4, ix+5, ix+1, ix+0); // Left - 4
		AddQuad(t, ix+3, ix+2, ix+6, ix+7);	// Right - 3
	}
	if (ends) {
		// Ends
		AddQuad(t, ix+0, ix+3, ix+7, ix+4); // Bottom - 2
		AddQuad(t, ix+1, ix+2, ix+6, ix+5);	// Top - 1
	}
}

void AddQuad(Vector<Quad> &t, int a, int b, int c, int d)
{
	t.Add(Triangle(a, b, c, d));	
}

void SplitTriVertex(Vector<Vertex> &v, Vector<Quad> &t, Vector<Vertex> &vout, Vector<Triangle> &tout) 
{
	out.Reserve(t.GetCount()*4);
	for (int i = 0; i < t.GetCount(); i++) {
		Quad &q = t[i];
		int ix = vout.GetCount()
		Triangle t1(ix, ix+1, ix+2);
		Triangle t2(ix+2, ix+3, ix);
		SetTextureCoords(v[t.a], v[t.b], v[t.c], v[t.d]);
		vout.Add(v[t.a]);
		vout.Add(v[t.b]);
		vout.Add(v[t.c]);
		vout.Add(v[t.d]);
		tout.Add(t1);
		tout.Add(t2);
	}
}

void SetTextureCoords(Vertex &a, Vertex &b, Vertex &c, Vertex &d)
{
	Point2f t;
	t.x = (a.pos - b.pos).Length();
	t.y = (d.pos - a.pos).Length();
	a.tex.x = 0.0f;
	a.tex.y = 0.0f;
	b.tex.x = t.x;
	b.tex.y = 0.0f;
	c.tex.x = t.xf;
	c.tex.y = t.yf;
	d.tex.x = 0.0;
	d.tex.y = t.yf;
}

