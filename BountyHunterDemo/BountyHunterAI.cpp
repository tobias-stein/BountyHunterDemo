///-------------------------------------------------------------------------------------------------
/// File:	BountyHunterAI.cpp.
///
/// Summary:	The bounty hunter app entry point.
///-------------------------------------------------------------------------------------------------

#include "Game.h"
#include "GameConfiguration.h"

int main(int argc, const char* args[])
{
	Game game = Game(GAME_TITLE);

	// initialize game
	game.Initialize(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);

	// run 100 frames
	for(int i = 0; i < 100; ++i)
		game.Step();

    return 0;
}

