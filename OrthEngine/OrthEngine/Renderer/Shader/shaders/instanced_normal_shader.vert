#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords; // vertex texture coordinates
layout (location = 3) in mat4 aModelMatrix; // instanced matrix with model matrix

out vec3 normal;

// output the normal vector for use by the geometry shader
out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 view;

void main()
{
    // compute and output the normal vector for use by geometry shader
    mat3 normalMatrix = mat3(transpose(inverse(view * aModelMatrix)));
    vs_out.normal = vec3(vec4(normalMatrix * aNormal, 0.0));

    // output the position of the vertex in projection space
    gl_Position = view * aModelMatrix * vec4(aPos, 1.0); 
}
