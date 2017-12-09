#pragma once
#include "glad\glad.h"
#include "Shader.h"

class CueRenderer {
public:
	CueRenderer() : vao(-1), quad_vbo(-1), cue_shader() { }
	~CueRenderer();

	void init(const glm::mat4& projection);

	// x and y of top left cue quad in world coordinates
	void render_cue(float x, float y, float rotation_radians);
private:
	GLuint vao, quad_vbo;

	Shader cue_shader;
};