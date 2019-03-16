#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

vec3 colour;

void main()
{
	color = v_TexCoord;
	color.r += 1;
	color.r /= 2;
	color.g += 1;
	color.g /= 2;
	color.b += 1;
	color.b /= 2;
}