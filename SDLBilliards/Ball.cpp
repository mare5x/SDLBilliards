#include "Ball.h"

const float Ball::radius = 0.05f;
const float Ball::width = 2 * Ball::radius;


void Ball::set_pos(float x, float y)
{
	set_x(x);
	set_y(y);
}
