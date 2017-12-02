#pragma once
#include "glm/glm.hpp"


class Ball {
public:
	Ball() : x(0), y(0), id(-1) {}

	void set_id(int v) { id = v; }
	int get_id() const { return id; }

	// Returns the position of the top left quad point in world space.
	glm::vec4 get_model_pos() const { return glm::vec4(x - radius, y - radius, 0, 1); }

	void set_x(float val) { x = val; }
	void set_y(float val) { y = val; }

	void set_pos(float x, float y);

	void translate_pos(const glm::vec2& dx);
	void translate_pos(float x, float y);

	void translate_velocity(const glm::vec2& dv);

	float get_x() const { return x; }
	float get_y() const { return y; }

	glm::vec2 get_pos() const { return glm::vec2(x, y); }

	glm::vec2 get_velocity() const { return velocity; }
	glm::vec2 get_acceleration() const { return acceleration; }

	// Returns the normalized direction vector of the velocity.
	glm::vec2 get_velocity_direction() const { return glm::normalize(velocity); }

	void set_velocity(glm::vec2 v) { velocity = v; }
	void set_acceleration(glm::vec2 a) { acceleration = a; }

	static const float radius;  // meters
	static const float width;  // meters
	static const float mass;  // kg
private:
	float x, y;  // position of center
	int id;  // 0 == white ball

	glm::vec2 velocity, acceleration;
};