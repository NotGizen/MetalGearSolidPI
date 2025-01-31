#version 310 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 VertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    VertexColor = vec4(aColor, 1.0f);
}