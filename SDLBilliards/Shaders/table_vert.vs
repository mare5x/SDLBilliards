#version 330 core
layout (location = 0) in vec2 local_pos;

out vec2 world_pos;

uniform mat4 projection;
uniform mat4 model;

void main()
{
	vec4 world_pos4 = model * vec4(local_pos.xy, 0.0, 1.0);
	world_pos = world_pos4.xy;
	gl_Position = projection * world_pos4;
}