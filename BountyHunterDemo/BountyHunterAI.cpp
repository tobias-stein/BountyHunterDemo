///-------------------------------------------------------------------------------------------------
/// File:	BountyHunterAI.cpp.
///
/// Summary:	The bounty hunter app entry point.
///-------------------------------------------------------------------------------------------------

#include "Game.h"

Game* g_GameInstance = new Game(GAME_TITLE);

int main(int argc, const char* args[])
{
	// initialize game
	g_GameInstance->Initialize(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, GAME_WINDOW_FULLSCREEN);

	// run 100 frames
	for(int i = 0; i < 100; ++i)
		g_GameInstance->Step();

	delete g_GameInstance;
	g_GameInstance = nullptr;

    return 0;
}

