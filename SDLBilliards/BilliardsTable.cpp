#include "BilliardsTable.h"
#include <algorithm>

BilliardsTable::TableEdge BilliardsTable::collides(const Ball& ball) const
{
	if (ball.get_x() - ball.radius <= 0.0f) return TableEdge::LEFT;
	if (ball.get_x() + ball.radius >= width) return TableEdge::RIGHT;
	if (ball.get_y() - ball.radius <= 0.0f) return TableEdge::TOP;
	if (ball.get_y() + ball.radius >= height) return TableEdge::BOTTOM;
	return TableEdge::NAUGHT;
}

void BilliardsTable::clamp_ball(Ball & ball) const
{
	ball.set_x(std::min(width, std::max(ball.get_x(), 0.0f)));
	ball.set_y(std::min(height, std::max(ball.get_y(), 0.0f)));
}
