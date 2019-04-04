#version 330 core

layout(location = 0) out vec4 color;

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

uniform vec4 Color;
uniform sampler2D MainTex;

void main()
{
vec4 texColor = texture(MainTex, TexCoord);

vec3 norm = normalize(Normal);
vec3 dir;
dir.x = 1;
dir.y = -1;
dir.z = 1;
dir = normalize(dir);
float min = 0.2;
float light = max(dot(norm, dir), 0.0);
light = max(min, light);
color = texColor * Color * light;
}