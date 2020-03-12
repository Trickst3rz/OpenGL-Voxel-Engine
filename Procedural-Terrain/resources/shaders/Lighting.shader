#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

uniform mat4 u_MVP;
uniform vec3 u_Position;

out vec3 v_Normal;

void main()
{
	gl_Position = u_MVP * (vec4(position.xyz + offset.xyz, 1.0f));
	v_Normal = normal;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec4 v_Normal;

uniform vec4 u_Colour;
uniform vec3 u_lightPos;

void main()
{
	colour = u_Colour;
};