#include"Game.h"
#include<iostream>
#include<time.h>
int main()
{
	srand(static_cast<unsigned>(time(NULL)));
	Game game;
	while (game.running())
	{
		game.update();
		game.draw();
	}

	return 0;
}