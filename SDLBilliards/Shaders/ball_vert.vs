#version 330 core
layout (location = 0) in vec2 quad;
layout (location = 1) in vec2 ball_offset;

out vec2 vert_pos;
flat out int is_white;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	vert_pos = quad.xy;

	is_white = gl_InstanceID == 0 ? 1 : 0;

	vec4 pos = projection * (model * vec4(quad.xy, 0.0f, 1.0f) + vec4(ball_offset.xy, 0.0f, 0.0f));
	gl_Position = pos;
}