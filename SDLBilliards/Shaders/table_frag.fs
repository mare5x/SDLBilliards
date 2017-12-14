#version 330 core

out vec4 frag_color;

in vec2 world_pos;

uniform float pocket_width;

void main()
{
	// black pockets
	if (world_pos.x < 0.05 || world_pos.x > 1.95) 
	{
		if ((world_pos.y < pocket_width / 2) || 
			(world_pos.y > 4 - pocket_width / 2) || 
			(world_pos.y < 2 + pocket_width / 2 && world_pos.y > 2 - pocket_width / 2))
		{
			frag_color = vec4(0, 0, 0, 1);	
			return;
		}
	}
	if (world_pos.y < 0.05 || world_pos.y > 3.95)
	{
		if ((world_pos.x < pocket_width / 2) || (world_pos.x > 2 - pocket_width / 2))
		{
			frag_color = vec4(0, 0, 0, 1);
			return;
		}
	}

	// green
	frag_color = vec4(0.58, 0.93, 0.22, 1);
}