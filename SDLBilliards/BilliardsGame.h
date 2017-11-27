#pragma once
#include "SDL.h"
#include "glad\glad.h"
#include "Shader.h"

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

	void handle_input(SDL_Event &e);

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
};