#version 330 core

layout(location = 0) out vec4 color;

in vec3 Normal;
in vec2 TexCoord;

uniform vec4 Color;
uniform sampler2D MainTex;

void main()
{
	color = Color;

	vec2 rg = Normal.xy;

	vec4 texColor = texture(MainTex, TexCoord);

	color = texColor * Color;
}