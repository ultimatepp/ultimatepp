SHADER(400 core,
	in vec4 fs_color;\n
	in vec3 fs_normal;\n
	in vec3 fs_fragPos;\n
	in vec2 fs_texCoord;\n
	
    out vec4 FragColor;\n
    
    uniform sampler2D tex;
    uniform int useTexture;

    void main()
    {
        if(useTexture != 0){
            FragColor = texture(tex, fs_texCoord);
        }else{
			FragColor = fs_color;
        }
		
    }
)