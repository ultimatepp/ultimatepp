SHADER(400 core,
	layout (vertices = 3) out;
	
	in vec3 fs_normal[];
	in vec3 fs_fragPos[];
	in vec3 fs_color[];
	
	out vec3 color[];\n
	out vec3 normal[];\n
	out vec3 fragPos[];\n
	
	void main(void){
		if(gl_InvocationID == 0){
			gl_TessLevelInner[0] = 5.0;
			gl_TessLevelOuter[0] = 16.0;
			gl_TessLevelOuter[1] = 16.0;
			gl_TessLevelOuter[2] = 16.0;
		}
		normal[gl_InvocationID] = fs_normal[gl_InvocationID];
		fragPos[gl_InvocationID] = fs_fragPos[gl_InvocationID];
		color[gl_InvocationID] = fs_color[gl_InvocationID];
		gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	}
)