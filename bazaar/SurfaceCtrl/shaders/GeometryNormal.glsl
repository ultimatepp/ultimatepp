SHADER(400 core,
    layout (triangles) in;
    layout (line_strip, max_vertices = 2) out;

	in vec4 color[3];\n
	in vec3 normal[3];\n
	in vec3 fragPos[3];\n
	in vec2 texCoord[3];\n
	
	out vec4 fs_color;\n
	out vec3 fs_normal;\n
	out vec3 fs_fragPos;\n
	out vec2 fs_texCoord;\n;
	
    uniform float normal_length = 0.8;

    void main(void)
    {
		vec3 ab = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
		vec3 ac = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
		vec3 face_normal = normalize(cross(ab, ac));
		
		vec4 tri_centroid = (gl_in[0].gl_Position +	gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;
		
		gl_Position =  tri_centroid;
		fs_fragPos = fragPos[0];
		fs_normal = normal[0];
		fs_color = color[0];
		fs_texCoord = texCoord[0];
		EmitVertex();
		
		gl_Position =  (tri_centroid + vec4(face_normal * normal_length, 0.0));
		fs_fragPos = fragPos[1];
		fs_normal = normal[1];
		fs_color = color[1];
		fs_texCoord = texCoord[1];
		EmitVertex();
		EndPrimitive();
		/*
		gl_Position =  gl_in[0].gl_Position;
		fs_fragPos = fragPos[2];
		fs_normal = normal[2];
		fs_color = color[2];
		EmitVertex();
		
		gl_Position =  (gl_in[0].gl_Position +	vec4(normal[0] * normal_length, 0.0));
		fs_fragPos = fragPos[0];
		fs_normal = normal[0];
		fs_color = color[0];
		EmitVertex();
		EndPrimitive();*/
	
    }
)