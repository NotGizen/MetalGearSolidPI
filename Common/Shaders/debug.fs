#version 310 es
precision mediump float; 
in vec4 VertexColor;
out vec4 FragColor;



void main()
{    
    FragColor = vec4(VertexColor);

}