#version 330 core
layout (location = 0) in vec3 aPos; // vertex position
layout (location = 1) in vec3 aNormal; // vertex normal
layout (location = 2) in vec2 aTexCoords; // vertex texture coordinates
layout (location = 3) in mat4 aModelMatrix; // instanced matrix with model matrix
layout (location = 7) in float textureLayerIndex;

// Output variables to fragment
out vec3 FragPos; // fragment position at vertex
out vec3 Normal; // normal of vertex
out vec2 TexCoords; // texture coordinates of vertex
out float TextureArrayIndex;

// Uniforms for camera translation vector
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(aModelMatrix * vec4(aPos, 1.0)); // fragment position is in world coordinates for easier lighting computations
    Normal = mat3(transpose(inverse(aModelMatrix))) * aNormal; // compute normal even when non-uniform scaling occurs
    TexCoords = aTexCoords; // texture coordinates of vertex
    
    gl_Position = projection * view * vec4(FragPos, 1.0); // compute the clip space position of the vertex
    TextureArrayIndex = textureLayerIndex;
}