#include "BallSpriteRenderer.h"

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

void BallSpriteRenderer::init()
{
	float base_ball_position_vertices[] = {
		0, 0,
		0, 0.05f,
		0.05f, 0,

		0, 0.05f,
		0.05f, 0,
		0.05f, 0.05f
	};

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(base_ball_position_vertices), base_ball_position_vertices, GL_STATIC_DRAW);

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
}
