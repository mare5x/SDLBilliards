#version 330 core

in vec2 vert_pos;
in vec2 ball_center;

out vec4 frag_color;

void main()
{
	float f = length(vert_pos - ball_center);
	frag_color = vec4(1, 0, 0, 1);
}