SHADER(400 core,
	in vec3 fs_color;\n
	in vec3 fs_normal;\n
	in vec3 fs_fragPos;\n
	
    out vec4 FragColor;\n

	uniform vec3 viewPos;

	//material
    vec3 MaterialDiffuse = vec3(0.8, 0.8, 0.8);
    vec3 MaterialSpecular = vec3(0.4, 0.4, 0.4);
    float MaterialShininess = 8.0;
	
	//light
    vec3 LightPosition =vec3( 0.0, 0.0, 0.0);
    vec3 LightAmbient = vec3( 1.0, 1.0, 1.0);
    vec3 LightDiffuse = vec3( 0.7, 0.7, 0.7);
    vec3 LightSpecular =vec3( 0.5, 0.5, 0.5);

    void main()
    {
		// ambient
		vec3 ambient = LightAmbient * fs_color;
		// diffuse
		vec3 norm = normalize(fs_normal);
		vec3 lightDir = normalize(LightPosition - fs_fragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = LightDiffuse * (diff * MaterialDiffuse);
		
		// specular
		vec3 viewDir = normalize(viewPos - fs_fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), MaterialShininess);
		vec3 specular = LightSpecular * (spec * MaterialSpecular);
		
		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0);

    }
)