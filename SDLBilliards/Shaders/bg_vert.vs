#version 330 core
layout (location = 0) in vec2 in_v_pos;

void main()
{
	gl_Position = vec4(in_v_pos, 0.0, 1.0);
}