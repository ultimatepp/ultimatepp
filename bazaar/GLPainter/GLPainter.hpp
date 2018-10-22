template <typename Src>
void GLPolygon(GLVertexData& mesh, const Src& polygon)
{
	Vector<Pointf> vertex;
	Vector<int> ndx;
	
	mesh.Clear();
	
	for(const auto& p: polygon) {
		int i0 = vertex.GetCount();
		for(int i = 0; i < p.GetCount(); i++) {
			if(i > 1)
				ndx << i0 << i0 + i - 1 << i0 + i;
			vertex.Add(p[i]);
		}
	}
	
	mesh.Add(vertex).Index(ndx);
}

template <typename Src>
void GLPolyline(GLVertexData& data, const Src& polygon, bool closed)
{
	Vector<float> vertex;
	Vector<int> ndx;
	
	int ii = 0;
	for(const auto& p: polygon) {
		int i0 = vertex.GetCount();
		for(int i = 0; i < p.GetCount() - !closed; i++) {
			Pointf p1 = p[i];
			Pointf p2 = p[i + 1 < p.GetCount() ? i + 1 : 0];
			Pointf un = p1 - p2;
			
			DDUMP(p1);
			DDUMP(p2);

			vertex << (float)p1.x << (float)p1.y << (float)un.x << (float)un.y;
			vertex << (float)p1.x << (float)p1.y << -(float)un.x << -(float)un.y;

			vertex << (float)p2.x << (float)p2.y << (float)un.x << (float)un.y;
			vertex << (float)p2.x << (float)p2.y << -(float)un.x << -(float)un.y;
			
			DDUMP(un);

			ndx << ii << ii + 1 << ii + 2
			    << ii + 3 << ii + 2 << ii + 1;
			
			if(ii)
				ndx << ii << ii + 1 << ii - 4 + 3
				    << ii - 4 + 2 << ii - 4 + 3 << ii;

			ii += 4;
		}
	}
	
	DDUMP(vertex);
	data.Add(vertex, 4).Index(ndx);
}

