#pragma once
#include "SDL.h"
#include "glad\glad.h"
#include "Shader.h"

#include <array>

#include "Ball.h"
#include "BallSpriteRenderer.h"
#include "Cue.h"
#include "BilliardsTable.h"

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

	void update_ball_positions_vbo();
	void reset_balls();

	void update_balls();
	void update_ball_position(Ball& ball);

	// Call this when the cue is released to start moving ball 0. (Assumes all balls are stationary.)
	void apply_cue_force();

	bool are_balls_moving() const;
	bool is_player_turn() const;

	bool handle_collision(Ball& ball);

	void handle_input(SDL_Event &e);

	// Game variables
	std::array<Ball, 16> balls;
	std::array<float, 16 * 2> ball_positions;  // (x, y) of balls in world space

	std::array<std::array<bool, 16>, 16> ball_collisions_table;  // true if collides

	Cue cue;
	BilliardsTable table;

	bool balls_moving;

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

bool is_zero(const glm::vec2 v);
bool is_zero(const glm::vec2 v, float epsilon);