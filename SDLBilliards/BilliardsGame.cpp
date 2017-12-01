#include "BilliardsGame.h"

#include "glm/gtc/matrix_transform.hpp"


BilliardsGame::BilliardsGame(int w, int h) :
	balls{}, ball_positions{},
	width(w), height(h), quit_requested(false),
	sdl_event(), window(nullptr), gl_context(),
	vao(0), vbo(0), bg_shader(), projection(glm::ortho(0.0f, 2.0f, 4.0f, 0.0f, -1.0f, 1.0f)),
	ball_renderer()
{
	if (!init())
		quit();
}

void BilliardsGame::run()
{
	while (!quit_requested) {
		input();

		update();

		render();
	}
}

void BilliardsGame::input()
{
	while (SDL_PollEvent(&sdl_event)) {
		if (sdl_event.type == SDL_QUIT) {
			quit_requested = true;
			return;
		}
		handle_input(sdl_event);
	}
}

void BilliardsGame::update()
{
}

void BilliardsGame::render()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	render_table();

	render_balls();

	SDL_GL_SwapWindow(window);
}

bool BilliardsGame::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		window = SDL_CreateWindow("Billiards", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
		if (!window) {
			SDL_Log("CREATE WINDOW ERROR: %s", SDL_GetError());
			return false;
		}

		gl_context = SDL_GL_CreateContext(window);
		if (!gl_context) {
			SDL_Log("CREATE CONTEXT ERROR: %s", SDL_GetError());
			return false;
		}

		if (!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
			return false;

		glViewport(0, 0, width, height);

		return init_gl();
	}
	else {
		SDL_Log("INIT ERROR: %s", SDL_GetError());
		return false;
	}
}

bool BilliardsGame::init_gl()
{
	float bg_quad[] = {
		// pos (x, y)			
		-1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, -1.0f,

		-1.0f, 1.0f,
		1.0f, -1.0f,
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

	bg_shader = Shader("Shaders/bg_vert.vs", "Shaders/bg_frag.fs");

	ball_renderer.init(projection);

	reset_balls();
	update_ball_positions();

	return true;
}

void BilliardsGame::quit()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	SDL_GL_DeleteContext(gl_context);
	gl_context = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_Quit();
}

void BilliardsGame::render_table()
{
	bg_shader.use();

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void BilliardsGame::render_balls()
{
	ball_renderer.render_all();
}

void BilliardsGame::update_ball_positions()
{
	for (int i = 0; i < balls.size(); i++) {
		Ball &ball = balls[i];
		ball_positions[i * 2] = ball.get_x();
		ball_positions[i * 2 + 1] = ball.get_y();
	}

	ball_renderer.update_ball_positions(ball_positions);
}

void BilliardsGame::reset_balls()
{
	// Set the positions to a triangular arrangement.
	
	// balls[0] is the white ball
	balls[0].set_pos(1, 3.75f);

	const float radius = Ball::radius;
	const float width = Ball::width;

	const float spacing = radius / 2.0f;  // spacing between adjacent balls

	float y = 0.5f;
	int ball_idx = 1;
	for (int row = 5; row != 0; row--) {
		float left_x = 1.0f - (row * width + (row - 1) * spacing) / 2.0f + radius;
		for (int col = 0; col < row; col++) {
			balls[ball_idx].set_pos(left_x + col * spacing + col * width, y);
			ball_idx++;
		}
		y += width + spacing;
	}
}

void BilliardsGame::handle_input(SDL_Event & e)
{
}
