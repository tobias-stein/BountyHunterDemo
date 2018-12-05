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

	PlayerAction** DUMMY_ACTIONS = new PlayerAction*[INT_SETTING(MAX_PLAYER)];

	// add max. players
	for (int i = 0; i < INT_SETTING(MAX_PLAYER); ++i)
	{
		game.AddPlayer();
		DUMMY_ACTIONS[i] = i > 0 ? new PlayerAction{ FLOAT_SETTING(COLLECTOR_MAX_MOVE_SPEED) / i, 0.0f } : nullptr;
	}

	// run 100 frames 
	for (int i = 0; i < 5000; ++i)
	{
		const PlayerState* playerStates = game.Step(DUMMY_ACTIONS);

		//for (int y = 0; y < GAME_WINDOW_HEIGHT; y += 2)
		//{
		//	for (int x = 0; x < GAME_WINDOW_WIDTH; x += 2)
		//	{
		//		int i = ((y * GAME_WINDOW_WIDTH) + x) * 3;
		//		printf("PIXEL[%d,%d] = r: %d, g: %d, b: %d\n", x, y, playerStates[0].image[i], playerStates[0].image[i + 1], playerStates[0].image[i + 2]);
		//	}
		//}
		


		printf("=== FRAME [%d] ===\n", i);
		for (int p = 0; p < INT_SETTING(MAX_PLAYER); ++p)
		{
			// access player state+reward
			const PlayerState& state = playerStates[p];
			printf("Player[%d]: {Dead: %s, X: %.2f, Y: %.2f, A: %.2f, P: %.2f, S: %.2f, R: %.2f }\n",
				p,
				state.playerDead ? "true" : "false",
				state.playerPositionX,
				state.playerPositionY,
				state.playerRotation,
				state.playerPocketLoad,
				state.playerStashLoad,
				state.playerReward);
		}
	}

    return 0;
}

