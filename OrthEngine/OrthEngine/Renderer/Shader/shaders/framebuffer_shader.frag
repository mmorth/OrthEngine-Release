#version 330 core

// input TextCoords for fragment and output fragment color
in vec2 TexCoords;
out vec4 FragColor;

// screen texture quad
uniform sampler2D screenTexture;

// edge detection kernel
float kernel[9] = float[](
    1,  1,  1,
    1, -8,  1,
    1,  1,  1
);

void main()
{
    // normalize the kernel computation to be indepdenent of screen size
    vec2 texelSize = 1.0 / textureSize(screenTexture, 0);

    // apply convolution cenetered at current pixel
    vec3 color = vec3(0.0);
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            vec2 offset = vec2(i, j) * texelSize;
            color += texture(screenTexture, TexCoords + offset).rgb * kernel[(i + 1) * 3 + j + 1];
        }
    }

    // apply result to fragment
    FragColor = vec4(color, 1.0);
} 