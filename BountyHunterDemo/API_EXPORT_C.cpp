
#include "API_EXPORT_C.h"
#include "Game.h"

void* CreateNewGameInstance()
{ 
	void* ptr = new Game("Bounty Hunter AI");
	return ptr;
}

void InitializeGame(void* gameInstance) 
{ 
	((Game*)gameInstance)->Initialize(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT); 
}

void RestartGame(void* gameInstance) 
{ 
	((Game*)gameInstance)->Restart(); 
}

size_t AddPlayer(void* gameInstance) 
{ 
	size_t playerId = ((Game*)gameInstance)->AddPlayer(); 
	return playerId;
}

int StepGame(void* gameInstance, struct PlayerAction** const in_actions, struct PlayerState* out_states, void* out_framebuffer) 
{ 
	int winnerId = ((Game*)gameInstance)->Step(in_actions, out_states, out_framebuffer); 
	return winnerId;
}

void TerminateGame(void* gameInstance) 
{ 
	delete gameInstance; 
	gameInstance = nullptr; 
}
