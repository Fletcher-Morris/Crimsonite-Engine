#version 330 core

layout(location = 0) out vec4 color;

in vec2 TexCoord;

uniform vec4 Color;
uniform sampler2D MainTex;

void main()
{
	vec4 texColor = texture(MainTex, TexCoord);
	color = Color;
	color = texColor;
}