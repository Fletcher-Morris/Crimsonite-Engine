#version 330 core

layout(location = 0) out vec4 color;

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

uniform vec4 Color;
uniform sampler2D MainTex;

void main()
{
color = texture(MainTex, TexCoord);
}