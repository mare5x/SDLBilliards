#version 330 core
layout (location = 0) in vec2 in_v_pos;
layout (location = 1) in vec2 in_offset;

out vec2 vert_pos;

void main()
{
	vert_pos = vec2(in_v_pos + in_offset);
	gl_Position = vec4(vert_pos, 0.0, 1.0);
}