#pragma once
#include "glm\glm.hpp"

class Cue {
public:
	Cue() : x{ 0 }, y{ 0 }, active{ false }, force { 0.0f } { }

	bool is_active() const { return active; }
	void set_active(bool val) { active = val; }

	float get_x() const { return x; }
	float get_y() const { return y; }

	void set_x(float v) { x = v; }
	void set_y(float v) { y = v; }

	void set_pos(float x, float y);

	const glm::vec2& get_force() const { return force; }
	void set_force(glm::vec2 new_force) { force = new_force; }
private:
	float x, y;  // position of the tip of the stick (white ball position)

	bool active;

	glm::vec2 force;  // force vector
};