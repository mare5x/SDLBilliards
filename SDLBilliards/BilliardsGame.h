#pragma once
#include "SDL.h"
#include "glad\glad.h"
#include "Shader.h"

#include <array>

#include "Ball.h"
#include "BallSpriteRenderer.h"

class BilliardsGame {
public:
	BilliardsGame(int w, int h);
	~BilliardsGame() { quit(); }

	void run();

	void input();
	void update();
	void render();
private:
	bool init();
	bool init_gl();
	void quit();

	void render_table();
	void render_balls();

	void update_ball_positions();
	void reset_balls();

	void handle_input(SDL_Event &e);

	// Game variables
	std::array<Ball, 16> balls;
	std::array<float, 16 * 2> ball_positions;  // (x, y) of balls in world space

	// Window variables
	int width, height;
	bool quit_requested;

	// SDL stuff
	SDL_Event sdl_event;
	SDL_Window* window;
	SDL_GLContext gl_context;

	// OpenGL stuff
	GLuint vao, vbo;
	Shader bg_shader;
	glm::mat4 projection;  // orthographic projection matrix

	BallSpriteRenderer ball_renderer;
};