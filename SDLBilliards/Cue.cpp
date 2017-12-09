#include "Cue.h"
#include "Ball.h"
#include "vector_tools.hpp"

#include <algorithm>

const float MIN_FORCE = 1;  // Newtons
const float MAX_FORCE = 250;  // Newtons

const float SECONDS_FOR_MAX_FORCE = 1.5f;
const unsigned int MS_FOR_MAX_FORCE = SECONDS_FOR_MAX_FORCE * 1000.0f;
const unsigned int MS_BEFORE_MAX_FORCE_RESET = 0.5f * 1000.0f;

const float Cue::width = 0.05f;
const float Cue::height = 1.5f;

// pretend the cue is a spring
const float CUE_BALL_DISTANCE = Ball::width;  // m
const float MAX_EXTENSION = 0.25f;  // m
const float CUE_COEFFICIENT = MAX_FORCE / MAX_EXTENSION;  // N/m


void Cue::set_pos(float x, float y)
{
	set_x(x);
	set_y(y);
}

void Cue::press_start(unsigned int time)
{
	press_start_time = time;
	press_started = true;
}

void Cue::press_release(float mouse_x, float mouse_y, unsigned int time)
{
	if (!press_started)
		return;

	unsigned int time_held = time - press_start_time;  // ms
	update_force(mouse_x, mouse_y, time_held);

	press_started = false;

	press_start_time = -1;
}

void Cue::mouse_move(float mouse_x, float mouse_y)
{
	cue_direction = glm::normalize(glm::vec2(mouse_x - get_x(), mouse_y - get_y()));
	
	cue_render_angle = std::atan2f(cue_direction.x, cue_direction.y);

	cue_render_offset = cue_direction * CUE_BALL_DISTANCE;
	cue_render_offset += glm::vec2(
		Cue::width / 2.0f * -1 * std::cos(cue_render_angle), 
		Cue::width / 2.0f * std::sin(cue_render_angle)
	);
}

void Cue::update(unsigned int time)
{
	if (press_started) {
		float force_scalar = calc_force(time - press_start_time);
		float extension = force_scalar / CUE_COEFFICIENT;

		cue_render_offset = cue_direction * (CUE_BALL_DISTANCE + extension);
		cue_render_offset += glm::vec2(
			Cue::width / 2.0f * -1 * std::cos(cue_render_angle),
			Cue::width / 2.0f * std::sin(cue_render_angle)
		);
	}
}

void Cue::render()
{
	cue_renderer.render_cue(get_x() + cue_render_offset.x, get_y() + cue_render_offset.y, cue_render_angle);
}

float Cue::calc_force(unsigned int time_held) const
{
	float lerp_force = std::min(time_held % (MS_FOR_MAX_FORCE + MS_BEFORE_MAX_FORCE_RESET), MS_FOR_MAX_FORCE) / static_cast<float>(MS_FOR_MAX_FORCE);
	return MIN_FORCE + lerp_force * (MAX_FORCE - MIN_FORCE);
}

void Cue::update_force(float mouse_x, float mouse_y, unsigned int time_held)
{
	float force_scalar = calc_force(time_held);
	glm::vec2 force_dir = glm::normalize(glm::vec2(get_x() - mouse_x, get_y() - mouse_y));

	set_force(force_dir * force_scalar);
}
