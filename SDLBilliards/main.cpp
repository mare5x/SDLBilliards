#include "BilliardsGame.h"

const int WIDTH = 400;
const int HEIGHT = 800;


int main(int argc, char* argv[])
{
	BilliardsGame game(WIDTH, HEIGHT);
	game.run();

	return 0;
}