#include "CueRenderer.h"
#include "Cue.h"
#include "glm\gtc\matrix_transform.hpp"

CueRenderer::~CueRenderer()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &quad_vbo);
}

void CueRenderer::init(const glm::mat4& projection)
{
	float quad_vertices[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f, 

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	cue_shader = Shader("Shaders/cue_vert.vs", "Shaders/cue_frag.fs");
	cue_shader.use();

	cue_shader.setMat4("projection", projection);
}

void CueRenderer::render_cue(float x, float y, float rotation_radians)
{
	cue_shader.use();

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(x, y, 0.0f));
	model = glm::rotate(model, rotation_radians, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(Cue::width, Cue::height, 1.0f));
	cue_shader.setMat4("model", model);

	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}
