#pragma once
#include "glm/glm.hpp"


class Ball {
public:
	Ball() : x(0), y(0), id(-1), in_pocket(false) {}

	void set_id(int v) { id = v; }
	int get_id() const { return id; }

	void set_x(float val) { x = val; }
	void set_y(float val) { y = val; }

	void set_pos(float x, float y);

	void translate_pos(const glm::vec2& dx);
	void translate_pos(float x, float y);

	void translate_velocity(const glm::vec2& dv);

	float get_x() const { return x; }
	float get_y() const { return y; }

	glm::vec2 get_pos() const { return glm::vec2(x, y); }

	void set_velocity(glm::vec2 v) { velocity = v; }
	glm::vec2 get_velocity() const { return velocity; }

	// Returns the normalized direction vector of the velocity.
	glm::vec2 get_velocity_direction() const { return glm::normalize(velocity); }

	bool collides(const Ball& ball) const;

	bool in_pocket;

	static const float radius;  // meters
	static const float width;  // meters
	static const float mass;  // kg
private:
	float x, y;  // position of center
	int id;  // 0 == white ball

	glm::vec2 velocity;
};