#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

mat4 MVP;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	MVP = Projection * View * Model;
	gl_Position = MVP * vec4(position.x, position.y, position.z, 1.0);

	Position = vec3(vec4(position, 1.0) * Model);
	Normal = mat3(transpose(inverse(Model))) * normal;
	TexCoord = texCoord;
}