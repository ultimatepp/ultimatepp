SHADER(400 core,
	in vec3 fs_color;\n
	in vec3 fs_normal;\n
	in vec3 fs_fragPos;\n
	
    out vec4 FragColor;\n

    void main()
    {
		FragColor = vec4(fs_color,1.0);
    }
)