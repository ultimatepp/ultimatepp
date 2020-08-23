SHADER(400 core,
	layout (triangles) in;
	layout (line_strip, max_vertices = 4) out;
	
	in vec4 fs_color[3];\n
	in vec3 fs_normal[3];\n
	in vec3 fs_fragPos[3];\n
	in vec2 fs_texCoord[3];\n
	
	out vec4 color;\n
	out vec3 normal;\n
	out vec3 fragPos;\n
	out vec2 texCoord;\n;

    void main()
    {
		int i;
		vec3 v1 = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;
		vec3 v2 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
		vec3 v3 = normalize(cross(v1,v2));
		for(i = 0; i < gl_in.length(); i++){
			gl_Position =  gl_in[i].gl_Position;
			color = fs_color[i];
			texCoord = fs_texCoord[i];
			normal = v3;
			fragPos = gl_in[i].gl_Position.xyz;
			EmitVertex();
		}
		
		
		gl_Position = gl_in[0].gl_Position;
		color = fs_color[0];
		texCoord = fs_texCoord[i];
		normal = v3;
		fragPos = gl_in[0].gl_Position.xyz;
		EmitVertex();
		EndPrimitive();
    }
)