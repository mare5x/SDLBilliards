#include "BilliardsGame.h"
#include "vector_tools.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include <vector>

const int FPS = 120;
const float FPS_ms = 1 / static_cast<float>(FPS) * 1000;

const float dt = 0.01;  // fixed time step (seconds)

const float TABLE_WIDTH = 2.0f;  // meters
const float TABLE_HEIGHT = 4.0f;  // meters

const float friction_coefficient = 0.0075f; 
const float friction_force = Ball::mass * 9.81f * friction_coefficient;
const float friction_acceleration_scalar = friction_force / Ball::mass;


BilliardsGame::BilliardsGame(int w, int h) :
	balls{}, ball_positions{}, ball_collisions_table{}, cue(), table(TABLE_WIDTH, TABLE_HEIGHT), balls_moving(false),
		dt_accumulator(0), current_time(),
	width(w), height(h), quit_requested(false),
	sdl_event(), window(nullptr), gl_context(),
	projection(glm::ortho(0.0f, TABLE_WIDTH, TABLE_HEIGHT, 0.0f, -1.0f, 1.0f)),
	ball_renderer()
{
	if (!init())
		quit();
}

void BilliardsGame::run()
{
	current_time = SDL_GetTicks();
	while (!quit_requested) {
		unsigned int start_time = SDL_GetTicks();

		input();
	
		update();

		render();

		unsigned int frame_time = SDL_GetTicks() - start_time;
		if (frame_time < FPS_ms)
			SDL_Delay(FPS_ms - frame_time);
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
	if (is_game_over()) {
		SDL_Log("GAME OVER!");
		return;
	}

	if (is_player_turn()) {
		current_time = SDL_GetTicks();
		cue.update(current_time);
	}
	else {
		unsigned int time = SDL_GetTicks();
		float delta = (time - current_time) / 1000.0f;
		current_time = time;

		dt_accumulator += delta;

		while (dt_accumulator >= dt) {
			update_balls();

			dt_accumulator -= dt;
		}

		balls_moving = are_balls_moving();

		if (!balls_moving) {
			if (balls[0].in_pocket) {
				balls[0].in_pocket = false;
				balls[0].set_pos(TABLE_WIDTH / 2.0f, TABLE_HEIGHT * 0.75f);
			}

			cue.set_pos(balls[0].get_x(), balls[0].get_y());
			int mouse_x, mouse_y;
			SDL_GetMouseState(&mouse_x, &mouse_y);
			cue.mouse_move(mouse_x / static_cast<float>(width) * TABLE_WIDTH, 
						   mouse_y / static_cast<float>(height) * TABLE_HEIGHT);
		}

		update_ball_positions_vbo();
	}
}

void BilliardsGame::render()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	table.render();

	render_balls();

	if (is_player_turn())
		cue.render();

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

		SDL_GL_SetSwapInterval(0);  // uncap FPS

		return init_gl();
	}
	else {
		SDL_Log("INIT ERROR: %s", SDL_GetError());
		return false;
	}
}

bool BilliardsGame::init_gl()
{
	table.init(projection);

	ball_renderer.init(projection);

	cue.init(projection);

	// Init ball ids
	for (int i = 0; i < balls.size(); i++) {
		balls[i].set_id(i);
	}

	reset_balls();
	update_ball_positions_vbo();

	return true;
}

void BilliardsGame::quit()
{
	SDL_GL_DeleteContext(gl_context);
	gl_context = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_Quit();
}

void BilliardsGame::render_balls()
{
	ball_renderer.render_all();
}

void BilliardsGame::update_ball_positions_vbo()
{
	for (int i = 0; i < balls.size(); i++) {
		Ball &ball = balls[i];
		ball_positions[i * 2] = ball.get_x() - ball.radius;
		ball_positions[i * 2 + 1] = ball.get_y() - ball.radius;
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

	for (Ball& ball : balls)
		ball.in_pocket = false;
}

void BilliardsGame::update_balls()
{
	for (Ball& ball : balls) {
		if (!ball.in_pocket)
			update_ball_position(ball);
	}
	ball_collisions_table = {};
	for (Ball& ball : balls) {
		if (!ball.in_pocket)
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

bool BilliardsGame::is_game_over() const
{
	int in_pocket = 0;
	for (int i = 1; i < balls.size(); i++) {
		if (balls[i].in_pocket) in_pocket++;
	}
	return in_pocket == balls.size() - 1;
}

bool BilliardsGame::handle_collision(Ball & ball)
{
	bool collision = false;

	if (table.in_pocket(ball)) {
		ball.set_velocity({ 0.0f, 0.0f });
		ball.in_pocket = true;
		return true;
	}

	BilliardsTable::TableEdge table_collision = table.collides(ball);
	if (table_collision != BilliardsTable::TableEdge::NAUGHT) {
		if (table_collision == BilliardsTable::TableEdge::TOP) {
			ball.set_velocity(glm::reflect(ball.get_velocity(), glm::vec2(0.0f, -1.0f)));
		} else if (table_collision == BilliardsTable::TableEdge::LEFT) {
			ball.set_velocity(glm::reflect(ball.get_velocity(), glm::vec2(1.0f, 0.0f)));
		} else if (table_collision == BilliardsTable::TableEdge::BOTTOM) {
			ball.set_velocity(glm::reflect(ball.get_velocity(), glm::vec2(0.0f, 1.0f)));
		} else if (table_collision == BilliardsTable::TableEdge::RIGHT) {
			ball.set_velocity(glm::reflect(ball.get_velocity(), glm::vec2(-1.0f, 0.0f)));
		}
		table.clamp_ball(ball);

		collision = true;
	}

	// if ball1 collides with ball2, calculate the collision only once instead of twice
	std::vector<Ball*> ball_collisions;
	for (Ball& ball2 : balls) {
		if (ball2.get_id() == ball.get_id() || ball2.in_pocket) continue;  // ignore balls in pockets
		if (!ball_collisions_table[ball.get_id()][ball2.get_id()] && ball.collides(ball2)) {
			ball_collisions_table[ball2.get_id()][ball.get_id()] = true;

			ball_collisions.push_back(&ball2);
		}
	}

	for (Ball* ball2 : ball_collisions) {
		glm::vec2 collision_dir = glm::normalize(ball2->get_pos() - ball.get_pos());

		// if two balls collide, they can get stuck together so unstick them
		float delta_collision_distance = (Ball::width - glm::length(ball2->get_pos() - ball.get_pos())) / 2.0f;
		ball2->translate_pos(collision_dir * delta_collision_distance);
		ball.translate_pos(-collision_dir * delta_collision_distance);

		// https://en.wikipedia.org/wiki/Elastic_collision
		// https://stackoverflow.com/questions/345838/ball-to-ball-collision-detection-and-handling
		float u1 = glm::dot(ball.get_velocity(), collision_dir);
		float u2 = glm::dot(ball2->get_velocity(), collision_dir);

		float v1 = u2;
		float v2 = u1;

		ball.translate_velocity((v1 - u1) * collision_dir);
		ball2->translate_velocity((v2 - u2) * collision_dir);
	}

	return collision;
}

void BilliardsGame::handle_input(SDL_Event & e)
{
	if (!is_player_turn())
		return;

	if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
		cue.press_start(e.button.timestamp);
	} else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
		if (cue.was_pressed()) {
			cue.press_release(e.button.x / static_cast<float>(width) * TABLE_WIDTH, 
							  e.button.y / static_cast<float>(height) * TABLE_HEIGHT, 
							  e.button.timestamp);
			apply_cue_force();
			balls_moving = true;
		}
	} else if (e.type == SDL_MOUSEMOTION) {
		cue.mouse_move(e.motion.x / static_cast<float>(width) * TABLE_WIDTH,
					   e.button.y / static_cast<float>(height) * TABLE_HEIGHT);
	}
}