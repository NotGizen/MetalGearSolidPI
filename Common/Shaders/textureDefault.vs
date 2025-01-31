#version 310 es
layout(location = 0) in vec3 aPosition; // Quad vertices
layout(location = 1) in vec2 aTexCoord; // Texture coordinates

out vec2 TexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    TexCoord = aTexCoord;
}