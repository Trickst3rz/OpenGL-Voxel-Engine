#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVP;
uniform vec3 u_Position;

void main()
{
	gl_Position = u_MVP * position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

uniform vec3 u_Colour;

void main()
{
	colour = vec4(u_Colour, 1.0f);
};