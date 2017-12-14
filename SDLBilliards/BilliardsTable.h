#pragma once
#include "Ball.h"
#include "Shader.h"

#include "glad\glad.h"
#include "glm\glm.hpp"


class BilliardsTable {
public:
	enum class TableEdge {
		TOP, LEFT, BOTTOM, RIGHT, NAUGHT
	};

	BilliardsTable(float w, float h) : width(w), height(h) { }
	~BilliardsTable();

	void init(const glm::mat4& projection);

	void render();

	// Returns with which edge the ball collides or NAUGHT if it doesn't.
	TableEdge collides(const Ball& ball) const;

	bool in_pocket(const Ball& ball) const;

	// Clamp the ball's position so that it's inside the table.
	void clamp_ball(Ball& ball) const;
private:
	float width, height;

	GLuint vao, vbo;
	Shader shader;
};