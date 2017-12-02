#include "BilliardsGame.h"

#include "glm/gtc/matrix_transform.hpp"


const float TABLE_WIDTH = 2.0f;  // meters
const float TABLE_HEIGHT = 4.0f;  // meters

const float dt = 0.01;  // fixed time step

const float friction_coefficient = 0.025f; 
const float friction_force = Ball::mass * 9.81f * friction_coefficient;
const float friction_acceleration_scalar = friction_force / Ball::mass;


BilliardsGame::BilliardsGame(int w, int h) :
	balls{}, ball_positions{}, cue(), balls_moving(false),
	width(w), height(h), quit_requested(false),
	sdl_event(), window(nullptr), gl_context(),
	vao(0), vbo(0), bg_shader(), projection(glm::ortho(0.0f, TABLE_WIDTH, TABLE_HEIGHT, 0.0f, -1.0f, 1.0f)),
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
	if (is_player_turn()) {

	}
	else {
		update_balls();
		update_ball_positions_vbo();

		balls_moving = are_balls_moving();
	}
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

	// Init ball ids
	for (int i = 0; i < balls.size(); i++) {
		balls[i].set_id(i);
	}

	reset_balls();
	update_ball_positions_vbo();

	cue.set_force({ 1.0f, -50.0f });
	apply_cue_force();
	balls_moving = true;

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

void BilliardsGame::update_ball_positions_vbo()
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
	balls[0].set_pos(TABLE_WIDTH / 2.0f, TABLE_HEIGHT * 0.75f);

	cue.set_pos(balls[0].get_x(), balls[0].get_y());

	const float radius = Ball::radius;
	const float width = Ball::width;

	const float spacing = radius / 2.0f;  // spacing between adjacent balls

	float y = 0.5f;
	int ball_idx = 1;
	for (int row = 5; row != 0; row--) {
		float left_x = TABLE_WIDTH / 2.0f - (row * width + (row - 1) * spacing) / 2.0f + radius;
		for (int col = 0; col < row; col++) {
			balls[ball_idx].set_pos(left_x + col * spacing + col * width, y);
			ball_idx++;
		}
		y += width + spacing;
	}
}

void BilliardsGame::update_balls()
{
	for (Ball& ball : balls) {
		update_ball_position(ball);

		handle_collision(ball);
	}
}

void BilliardsGame::update_ball_position(Ball & ball)
{
	if (is_zero(ball.get_velocity()))
		return;

	glm::vec2 acceleration = friction_acceleration_scalar * -1 * ball.get_velocity_direction();

	ball.translate_velocity(acceleration * dt);
	ball.translate_pos(ball.get_velocity() * dt);

	if (is_zero(ball.get_velocity(), 0.001f))
		ball.set_velocity(glm::vec2(0.0f));
}

void BilliardsGame::apply_cue_force()
{
	Ball& ball = balls[0];

	glm::vec2 acceleration = (cue.get_force() - friction_force) / ball.mass;

	ball.set_velocity(acceleration * dt);
	ball.translate_pos(ball.get_velocity() * dt);
}

bool BilliardsGame::are_balls_moving() const
{
	for (const Ball& ball : balls) {
		if (!is_zero(ball.get_velocity()))
			return true;
	}
	return false;
}

bool BilliardsGame::is_player_turn() const
{
	return !balls_moving;
}

bool BilliardsGame::handle_collision(Ball & ball)
{
	return false;
}

bool BilliardsGame::collides(const Ball & ball1, const Ball & ball2) const
{
	return glm::distance(ball1.get_pos(), ball2.get_pos()) < Ball::width;
}

bool BilliardsGame::collides_with_table(const Ball & ball) const
{
	return ball.get_x() - ball.radius < 0.0f || ball.get_x() + ball.radius > TABLE_WIDTH
		|| ball.get_y() - ball.radius < 0.0f || ball.get_y() + ball.radius > TABLE_HEIGHT;
}

void BilliardsGame::handle_input(SDL_Event & e)
{
}

bool is_zero(const glm::vec2 v)
{
	return v.x == 0 && v.y == 0;
}

bool is_zero(const glm::vec2 v, float epsilon)
{
	return abs(v.x) <= epsilon && abs(v.y) <= epsilon;
}
