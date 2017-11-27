#version 330 core
layout (location = 0) in vec3 in_v_pos;
layout (location = 1) in vec2 in_tex_pos;

out vec2 tex_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// gl_Position = projection * view * model * vec4(in_v_pos, 1.0);
	gl_Position = vec4(in_v_pos, 1.0);
	tex_pos = in_tex_pos;
}