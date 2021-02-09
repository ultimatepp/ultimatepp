class Delaunay
{
public:
	Delaunay() : tihull(-1) {}
	Delaunay(const Array<Pointf>& points, double epsilon = 1e-6) { Build(points, epsilon); }

	void                          Build(const Array<Pointf>& points, double epsilon = 1e-6);

public:
	struct Triangle // anticlockwise
	{
		bool IsProper() const                       { return vertex[0] >= 0; }
		int  operator [] (int i) const              { return vertex[i]; }
		int& operator [] (int i)                    { return vertex[i]; }

		int  Next(int i) const                      { return nextindex[i] >> 2; }
		int  NextIndex(int i) const                 { return nextindex[i] & 3; }

		void Set(int a, int b, int c)               { vertex[0] = a; vertex[1] = b; vertex[2] = c; }
		void SetNext(int i, int next, int index)    { ASSERT(index >= 0 && index <= 2); nextindex[i] = next * 4 + index; }

		int  vertex[3];    // [0] = -1 => infinity
		int  nextindex[3]; // neighbour[i] is opposite to vertex[i]; bit 0 & 1 = my index in neighbour's neighbour list
	};

	int                           GetCount() const                             { return triangles.GetCount(); }
	const Triangle&               operator [] (int i) const                    { return triangles[i]; }
	Pointf                        At(int i) const                              { return points[i]; }
	Pointf                        At(const Triangle& t, int i) const           { return points[t[i]]; }
	int                           GetHullIndex() const                         { return tihull; }

private:
	bool                          IsNear(const Pointf& a, const Pointf& b) const { return fabs(a.x - b.x) <= epsilon && fabs(a.y - b.y) <= epsilon; }
	void                          CreatePair(int i, int j);
	void                          AddHull(int i);
	void                          Link(int ta, int ia, int tb, int ib);

private:
	Array<Pointf>                 points;
	Vector<int>                   order;
	Array<Triangle>               triangles;
	double                        epsilon;
	double                        epsilon2;
	int                           tihull;
};
