#pragma once
#include "glm/glm.hpp"


class Ball {
public:
	Ball() : x(0), y(0), id(0) {}

	// Returns the position of the bottom left quad point in world space.
	glm::vec4 get_model_pos() const { return glm::vec4(x - radius, y - radius, 0, 1); }

	void set_x(float val) { x = val; }
	void set_y(float val) { y = val; }

	void set_pos(float x, float y);

	float get_x() const { return x; }
	float get_y() const { return y; }

	const float radius = 0.05f;
	const float width = 2 * radius;
private:
	float x, y;  // position of center
	int id;

	glm::vec2 velocity, acceleration;
};