#pragma once
#include "glm\glm.hpp"

class Cue {
public:
	Cue() : x{ 0 }, y{ 0 }, visible{ true }, press_started{ false }, force { 0.0f }, press_timestamp{ 0 } { }

	float get_x() const { return x; }
	float get_y() const { return y; }

	void set_x(float v) { x = v; }
	void set_y(float v) { y = v; }

	void set_pos(float x, float y);

	const glm::vec2& get_force() const { return force; }
	void set_force(glm::vec2 new_force) { force = new_force; }

	void press_start(unsigned int time);
	// Note: mouse_x and y must be in world coordinates!
	void press_release(float mouse_x, float mouse_y, unsigned int time);

	bool was_pressed() const { return press_started; }
private:
	float x, y;  // position of the tip of the stick (white ball position)

	glm::vec2 force;  // force vector

	bool visible;
	bool press_started;
	unsigned int press_timestamp;  // timestamp of mouse press event (to calculate force)
};