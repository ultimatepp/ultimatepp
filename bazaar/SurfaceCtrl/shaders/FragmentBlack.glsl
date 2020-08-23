SHADER(400 core,
	in vec4 fs_color;\n
	in vec3 fs_normal;\n
	in vec3 fs_fragPos;\n
	in vec2 fs_texCoord;\n
	
    out vec4 FragColor;\n

	uniform vec4 CustomColor = vec4(0.0,0.0,0.0,1.0);
    void main()\n
    {\n
		FragColor = CustomColor;\n
    }\n
)