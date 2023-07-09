#version 330 core

// Input is a series of 3 vertices representing a triangle and output is a line strip representing the normal visualization with a max of 6 vertices representing the start and end normal visualization line
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

// computed normal for each vertex in projection space
in VS_OUT {
    vec3 normal;
} gs_in[];

// magnitude length of normal visualization line
const float MAGNITUDE = 0.2;

// projection matrix input (because vertex only applies view and model)
uniform mat4 projection;

void GenerateLine(int index)
{
    // Start of normal line (at vertex position)
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    // End of the normal line (at magnitude away from start in direction of normal)
    gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    // Connect the points to form the normal line
    EndPrimitive();
}

void main()
{
    // Generate a normal line for each of the 3 triangle vertices
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}