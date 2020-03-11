#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
//Make sure the offset is the last layout location and the location matches InstanceBuffer
layout(location = 3) in vec4 offset;
//layout(location = 4) in bool isVisable; //This causes an error in the shader? Need to figure out how to not send vertice data you don't want to render

uniform mat4 u_MVP;

out vec2 v_TexCoord;
out vec3 v_Normal;

void main()
{
	gl_Position = u_MVP * (vec4(position.xyz + offset.xyz, 1.0f));
	v_TexCoord = texCoord;
	v_Normal = normal;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;
in vec4 v_Normal;

uniform vec4 u_Colour;
uniform vec3 u_lightPos;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColour = texture(u_Texture, v_TexCoord);
	colour = u_Colour;
};