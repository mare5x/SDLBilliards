#include "BallSpriteRenderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Ball.h"


BallSpriteRenderer::BallSpriteRenderer()
	: vao(-1), quad_vbo(-1), ball_positions_vbo(-1),
	shader()
{
}

BallSpriteRenderer::~BallSpriteRenderer()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &quad_vbo);
	glDeleteBuffers(1, &ball_positions_vbo);
}

void BallSpriteRenderer::render_all()
{
	shader.use();

	glBindVertexArray(vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 16);
	glBindVertexArray(0);
}

void BallSpriteRenderer::update_ball_positions(const std::array<float, 16 * 2>& positions)
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, ball_positions_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16 * 2, positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void BallSpriteRenderer::init(const glm::mat4 &projection)
{
	float base_ball_position_quad[] = {
		0, 0,
		0, 1.0f,
		1.0f, 0,

		0, 1.0f,
		1.0f, 0,
		1.0f, 1.0f
	};

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(base_ball_position_quad), base_ball_position_quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));

	glGenBuffers(1, &ball_positions_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ball_positions_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16 * 2, NULL, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);

	glBindVertexArray(0);

	shader = Shader("Shaders/ball_vert.vs", "Shaders/ball_frag.fs");
	shader.use();

	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(Ball::width, Ball::width, 1.0f));

	shader.setMat4("projection", projection);
	shader.setMat4("model", model);
}
