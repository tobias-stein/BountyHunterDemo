
#include "API_EXPORT_C.h"
#include "Game.h"

void* CreateNewGameInstance() { return new Game("Bounty Hunter AI"); }

void InitializeGame(void* gameInstance) { ((Game*)gameInstance)->Initialize(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT); }

void RestartGame(void* gameInstance) { ((Game*)gameInstance)->Restart(); }

size_t AddPlayer(void* gameInstance) { return ((Game*)gameInstance)->AddPlayer(); }

int StepGame(void* gameInstance, struct PlayerAction** const in_actions, struct PlayerState* out_states, void* out_framebuffer) { return ((Game*)gameInstance)->Step(in_actions, out_states, out_framebuffer); }

void TerminateGame(void* gameInstance) { delete gameInstance; gameInstance = nullptr; }
