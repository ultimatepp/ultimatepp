SHADER(400 core,
	layout (triangles) in;\n
	in vec3 color[];\n
	in vec3 normal[];\n
	in vec3 fragPos[];\n
	out vec3 fs_normal;
	out vec3 fs_fragPos;\n
	out vec3 fs_color;\n
	void main(void){\n
		
		fs_normal = normal[0];\n
		fs_fragPos = fragPos[0];\n
		fs_color = color[0];\n
		gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) + (gl_TessCoord.y * gl_in[1].gl_Position) + (gl_TessCoord.z * gl_in[2].gl_Position);
	}\n
)