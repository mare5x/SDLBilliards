#include "Cue.h"
#include <algorithm>

const float MIN_FORCE = 1;  // Newtons
const float MAX_FORCE = 250;  // Newtons

const float SECONDS_FOR_MAX_FORCE = 1.5f;
const unsigned int MS_FOR_MAX_FORCE = SECONDS_FOR_MAX_FORCE * 1000.0f;


void Cue::set_pos(float x, float y)
{
	set_x(x);
	set_y(y);
}

void Cue::press_start(unsigned int time)
{
	press_timestamp = time;
	press_started = true;
}

void Cue::press_release(float mouse_x, float mouse_y, unsigned int time)
{
	if (!press_started)
		return;

	press_started = false;

	unsigned int time_held = time - press_timestamp;  // ms

	printf("%d\n", time_held);

	float lerp_force = std::min(time_held, MS_FOR_MAX_FORCE) / static_cast<float>(MS_FOR_MAX_FORCE);
	float force_scalar = MIN_FORCE + lerp_force * (MAX_FORCE - MIN_FORCE);
	
	glm::vec2 force_dir = glm::normalize(glm::vec2(get_x() - mouse_x, get_y() - mouse_y));

	set_force(force_dir * force_scalar);

	press_timestamp = -1;
}
