#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_MVP;

uniform vec4 u_offsets[512];

out vec2 v_TexCoord;

void main()
{
	vec4 offset = u_offsets[gl_InstanceID];
	gl_Position = u_MVP * (position + offset);
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;

uniform vec4 u_Colour;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColour = texture(u_Texture, v_TexCoord);
	colour = texColour;
};