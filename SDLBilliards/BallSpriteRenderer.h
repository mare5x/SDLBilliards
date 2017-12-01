#pragma once
#include "glad\glad.h"
#include "Shader.h"
#include <array>

class BallSpriteRenderer {
public:
	BallSpriteRenderer();
	~BallSpriteRenderer();

	// Use the current state to batch render all balls.
	void render_all();

	void update_ball_positions(const std::array<float, 16 * 2> &positions);

	void init(const glm::mat4 &projection);
private:
	GLuint vao, quad_vbo, ball_positions_vbo;

	Shader shader;
};