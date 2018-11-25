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

	ActionState** DUMMY_ACTIONS = new ActionState*[INT_SETTING(MAX_PLAYER)];

	// add max. players
	for (int i = 0; i < INT_SETTING(MAX_PLAYER); ++i)
	{
		game.AddPlayer();
		DUMMY_ACTIONS[i] = i > 0 ? new ActionState{ FLOAT_SETTING(COLLECTOR_MAX_MOVE_SPEED) / i, 0.0f } : nullptr;
	}

	// run 100 frames
	for(int i = 0; i < 500; ++i)
		game.Step(DUMMY_ACTIONS);

    return 0;
}

