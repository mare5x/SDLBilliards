#version 330 core

in vec2 vert_pos;
flat in int is_white;

out vec4 frag_color;

void main()
{
	if (length(vert_pos - vec2(0.5, 0.5)) > 0.5)
		discard;
	if (is_white == 1)
		frag_color = vec4(1, 1, 1, 1);
	else
		frag_color = vec4(1, 0, 0, 1);
}