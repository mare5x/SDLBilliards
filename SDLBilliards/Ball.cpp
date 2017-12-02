#include "Ball.h"

const float Ball::radius = 0.05f;
const float Ball::width = 2 * Ball::radius;
const float Ball::mass = 0.5f;


void Ball::set_pos(float x, float y)
{
	set_x(x);
	set_y(y);
}

void Ball::translate_pos(const glm::vec2 & dx)
{
	translate_pos(dx.x, dx.y);
}

void Ball::translate_pos(float x, float y)
{
	set_x(this->x + x);
	set_y(this->y + y);
}

void Ball::translate_velocity(const glm::vec2 & dv)
{
	velocity += dv;
}
