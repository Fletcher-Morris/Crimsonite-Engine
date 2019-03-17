#version 330 core

layout(location = 0) out vec4 color;

out vec4 FragmentColor;

uniform vec4 Color;

void main()
{
	FragmentColor = Color;
}