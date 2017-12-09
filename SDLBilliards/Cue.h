#pragma once
#include "glm\glm.hpp"

#include "CueRenderer.h"


class Cue {
public:
	Cue() : x{ 0 }, y{ 0 },
		visible{ true }, press_started{ false },
		force{ 0.0f, 0.0f }, cue_direction{ 0.0f },
		cue_render_offset{ 0.0f }, cue_render_angle(0),
		press_start_time { 0 },
		cue_renderer() { }

	void init(const glm::mat4& projection) { cue_renderer.init(projection); }

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

	// Note: mouse_x and y must be in world coordinates!
	void mouse_move(float mouse_x, float mouse_y);

	void update(unsigned int time);
	void render();

	static const float width, height;  // meters
private:
	float calc_force(unsigned int time_held) const;
	void update_force(float mouse_x, float mouse_y, unsigned int time_held);

	float x, y;  // position of the tip of the stick (white ball position)

	glm::vec2 force;  // force vector
	glm::vec2 cue_direction;  // a normalized vector pointing from the mouse's position to the tip of the cue
	glm::vec2 cue_render_offset;  // rendering offset to the tip of the stick coordinates
	float cue_render_angle;

	bool visible;
	bool press_started;
	unsigned int press_start_time;  // timestamp of mouse press event (to calculate force)

	CueRenderer cue_renderer;
};