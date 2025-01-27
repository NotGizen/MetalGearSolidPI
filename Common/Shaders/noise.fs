#version 310 es
precision mediump float; 
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    vec4 imageColor = texture(texture1, TexCoord);
    float noiseValue = texture(texture2, TexCoord).r;
   vec4 result = imageColor * noiseValue;
   FragColor = result;
}