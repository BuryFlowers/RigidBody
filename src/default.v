#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 TexCoords;

out vec3 pos;
out vec3 normal;
out vec3 albedo;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    pos = vec3(model * vec4(aPos, 1.0f));
    normal =  vec3(vec4(aNormal, 1.0f) * inverse(model));
    albedo = aNormal;
    
}