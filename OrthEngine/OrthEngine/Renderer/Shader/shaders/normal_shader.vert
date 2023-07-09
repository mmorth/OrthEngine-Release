#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// output the normal vector for use by the geometry shader
out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 view;
uniform mat4 model;

void main()
{
    // compute and output the normal vector for use by geometry shader
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = vec3(vec4(normalMatrix * aNormal, 0.0));

    // output the position of the vertex in projection space
    gl_Position = view * model * vec4(aPos, 1.0); 
}