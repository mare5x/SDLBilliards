#version 330 core
layout (location = 0) in vec2 quad;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	vec4 pos = projection * model * vec4(quad.xy, 0.0f, 1.0f);
	gl_Position = pos;
}