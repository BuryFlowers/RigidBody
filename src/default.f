#version 330 core
out vec4 FragColor;

in vec3 pos;
in vec3 normal;
in vec3 albedo;
uniform vec3 cameraPos;
uniform vec3 lightDir;
uniform vec3 lightRadiance;

void main()
{

    vec3 ambient = vec3(0.2f, 0.2f, 0.2f);

    vec3 cameraDir = cameraPos - pos;
    cameraDir = normalize(cameraDir);
    vec3 halfDir = cameraDir - lightDir;
    halfDir = normalize(halfDir);

    vec3 diffuse = albedo * lightRadiance * max(0.0f, dot(-lightDir, normal));

    vec3 specular = lightRadiance * max(0.0f, pow(dot(halfDir, normal), 2.0f));


    FragColor = vec4(ambient + diffuse + specular, 1.0f);
    //FragColor.x = pow(FragColor.x, 1.0f / 2.2f);
    //FragColor.y = pow(FragColor.y, 1.0f / 2.2f);
    //FragColor.z = pow(FragColor.z, 1.0f / 2.2f);

}