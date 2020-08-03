SHADER(400 core,
	in vec3 color;\n
	in vec3 normal;\n
	in vec3 fragPos;\n
	
    out vec4 FragColor;\n

	uniform vec3 viewPos;

	//material
	struct Material{
		vec3 Diffuse;
		vec3 Specular;
		float Shininess;
	};
	uniform Material mat;
	
	//light
    vec3 LightPosition =vec3( 0.0, 50.0, 0.0);
    vec3 LightAmbient = vec3( 1.0, 1.0, 1.0);
    vec3 LightDiffuse = vec3( 1.0, 1.0, 1.0);
    vec3 LightSpecular =vec3( 0.64, 0.64, 0.64);

    void main()
    {
		// ambient
		vec3 ambient = LightAmbient * color;
		
		// diffuse
		vec3 norm = normalize(normal);
		vec3 lightDir = normalize(LightPosition - fragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = LightDiffuse * (diff * mat.Diffuse);
		
		// specular
		vec3 viewDir = normalize(viewPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.Shininess);
		vec3 specular = LightSpecular * (spec * mat.Specular);
		
		vec3 result = ambient + diffuse + specular;

		FragColor = vec4(result, 1.0);

    }
)