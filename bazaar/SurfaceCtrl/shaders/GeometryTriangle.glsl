SHADER(400 core,
	layout (triangles) in;
	layout (line_strip, max_vertices = 4) out;
	
	in vec3 color[3];\n
	in vec3 normal[3];\n
	in vec3 fragPos[3];\n
	out vec3 fs_color;\n
	out vec3 fs_normal;\n
	out vec3 fs_fragPos;\n

    void main()
    {
		int i;
		vec3 v1 = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;
		vec3 v2 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
		vec3 v3 = normalize(cross(v1,v2));
		for(i = 0; i < gl_in.length(); i++){
			gl_Position =  gl_in[i].gl_Position;
			fs_color = gl_in[i].gl_Position.xyz;//color[i];
			fs_normal = v3;
			fs_fragPos = gl_in[i].gl_Position.xyz;
			EmitVertex();
		}
		
		
		gl_Position = gl_in[0].gl_Position;
		fs_color = gl_in[0].gl_Position.xyz;//color[0];
		fs_normal = v3;
		fs_fragPos = gl_in[0].gl_Position.xyz;
		EmitVertex();
		EndPrimitive();
    }
)