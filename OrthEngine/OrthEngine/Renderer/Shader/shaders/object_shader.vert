#version 330 core
layout (location = 0) in vec3 aPos; // vertex position
layout (location = 1) in vec3 aNormal; // vertex normal
layout (location = 2) in vec2 aTexCoords; // vertex texture coordinates

// Output variables to fragment
out vec3 FragPos; // fragment position at vertex
out vec3 Normal; // normal of vertex
out vec2 TexCoords; // texture coordinates of vertex

// Uniforms for camera translation vector
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); // fragment position is in world coordinates for easier lighting computations
    Normal = mat3(transpose(inverse(model))) * aNormal; // compute normal even when non-uniform scaling occurs
    TexCoords = aTexCoords; // texture coordinates of vertex
    
    gl_Position = projection * view * vec4(FragPos, 1.0); // compute the clip space position of the vertex
}