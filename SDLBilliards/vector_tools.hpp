#pragma once
#include "glm\glm.hpp"

inline bool is_zero(const glm::vec2 v)
{
	return v.x == 0 && v.y == 0;
}

inline bool is_zero(const glm::vec2 v, float epsilon)
{
	return abs(v.x) <= epsilon && abs(v.y) <= epsilon;
}
