#include "BilliardsTable.h"
#include <algorithm>
#include "glm\gtc\matrix_transform.hpp"

const float POCKET_WIDTH = Ball::width * 1.75f;


BilliardsTable::~BilliardsTable()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void BilliardsTable::init(const glm::mat4& projection)
{
	float bg_quad[] = {
		0, 0,
		0, 1.0f,
		1.0f, 0,

		0, 1.0f,
		1.0f, 0,
		1.0f, 1.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bg_quad), bg_quad, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	shader = Shader("Shaders/table_vert.vs", "Shaders/table_frag.fs");
	shader.use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(width, height, 1.0f));

	shader.setMat4("projection", projection);
	shader.setMat4("model", model);
	shader.setFloat("pocket_width", POCKET_WIDTH);
}

void BilliardsTable::render()
{
	shader.use();

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

BilliardsTable::TableEdge BilliardsTable::collides(const Ball& ball) const
{
	if (ball.get_x() - ball.radius <= 0.0f) return TableEdge::LEFT;
	if (ball.get_x() + ball.radius >= width) return TableEdge::RIGHT;
	if (ball.get_y() - ball.radius <= 0.0f) return TableEdge::TOP;
	if (ball.get_y() + ball.radius >= height) return TableEdge::BOTTOM;
	return TableEdge::NAUGHT;
}

bool BilliardsTable::in_pocket(const Ball & ball) const
{
	TableEdge collision = collides(ball);
	if (collision != TableEdge::NAUGHT) {
		if (collision == TableEdge::LEFT || collision == TableEdge::RIGHT) {
			float ball_top = ball.get_y() - ball.radius;
			float ball_bot = ball_top + ball.width;
			if ((ball_top < height / 2.0f + POCKET_WIDTH / 2.0f || ball_bot < height / 2.0f + POCKET_WIDTH / 2.0f) &&
				(ball_top > height / 2.0f - POCKET_WIDTH / 2.0f || ball_bot > height / 2.0f - POCKET_WIDTH / 2.0f))
				return true;
			if ((ball_top < POCKET_WIDTH / 2.0f || ball_bot < POCKET_WIDTH / 2.0f) &&
				(ball_top >= 0 || ball_bot >= 0))
				return true;
			if ((ball_top > height - POCKET_WIDTH / 2.0f || ball_bot > height - POCKET_WIDTH / 2.0f) &&
				(ball_top <= height || ball_bot <= height))
				return true;
		}
		else if (collision == TableEdge::TOP || collision == TableEdge::BOTTOM) {
			float ball_left = ball.get_x() - ball.radius;
			float ball_right = ball_left + ball.width;
			if (ball_left < POCKET_WIDTH / 2.0f || ball_right < POCKET_WIDTH / 2.0f)
				return true;
			if (ball_left > width - POCKET_WIDTH / 2.0f || ball_right > width - POCKET_WIDTH / 2.0f)
				return true;
		}
	}
	return false;
}

void BilliardsTable::clamp_ball(Ball & ball) const
{
	ball.set_x(std::min(width, std::max(ball.get_x(), 0.0f)));
	ball.set_y(std::min(height, std::max(ball.get_y(), 0.0f)));
}
