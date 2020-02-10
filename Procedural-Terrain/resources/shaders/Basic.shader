#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{ // colour is 0-1 aka 0-255
	color = vec4(1.0, 0.0, 0.0, 1.0);
};