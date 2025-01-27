#version 310 es
precision mediump float; 
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;


void main() 
{
    vec4 imageColor = texture(texture1, TexCoord);
    FragColor = imageColor;
}