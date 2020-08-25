SHADER(400 core,

in vec3 fs_texCoord;\n

out vec4 FragColor;\n

uniform samplerCube skybox;\n

void main(){
    FragColor = texture(skybox, fs_texCoord);
}

)