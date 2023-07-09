#version 330 core

out vec4 FragColor;

void main()
{
    // All lights should be rendered as all bright and not impacted by other lights
    FragColor = vec4(1.0);
}