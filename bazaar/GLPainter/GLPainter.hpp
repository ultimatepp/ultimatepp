template <typename Src>
void GLPolygon(GLVertexData& mesh, const Src& polygon)
{
	Vector<Pointf> vertex; // todo: Optimize!
	Vector<int> ndx;
	
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
void GLPolyline(GLVertexData& data, const Src& polygon)
{
}

