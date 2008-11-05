class ModelMesh : public Moveable<ModelMesh>
{
public:
	ModelMesh() : vboid(0), iboid(0) { }
	
	bool Load(String file, VBOLibrary &lib, Point3f scale);
	void Draw();
private:
	bool alphasort;
	GLuint vbo;
	GLuint ibo;
	
	void Scale(Vector<Vertex> &v, Vector<Vertex> &tri, Point3f scale)		
};

class Billboard : public Moveable<Billboard>
{
public:
	Billboard() : depth(0.0f), sz(1.0f), texture(0.0f) { }
	
	void Set(Point3f position, float size, RGBA col, GLuint texture);
	void SetDepth(Point3f camera_pos);
	
	void DoState(GLuint last_texture);
	
	operator>(const Billboard &b) const { return depth > b.depth; }
	operator<(const Billboard &b) const { return depth < b.depth; }
private:
	GLuint  texture;
	RGBA   	color;
	float 	depth;
	Point3f pos;
	float 	sz;
};