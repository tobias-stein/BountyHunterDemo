///-------------------------------------------------------------------------------------------------
/// File:	Game.h.
///
/// Summary:	The game application class. Manages core initilization stuff, clean-up and main 
/// game loop.
///-------------------------------------------------------------------------------------------------
/// 

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <SDL.h> 

// to prevent:
// Error	LNK2019	unresolved external symbol SDL_main referenced in function main_utf8	BountyHunterDemo	..\EntityComponentSystem\BountyHunterDemo\SDL2main.lib(SDL_windows_main.obj)	1
#undef main 


#include "GameConfiguration.h"
#include "GameEvents.h"
#include "GameTypes.h"

// game systems
#include "InputSystem.h"
#include "RenderSystem.h"
#include "WorldSystem.h"
#include "PlayerSystem.h"
#include "LifetimeSystem.h"
#include "RespawnSystem.h"
#include "ControllerSystem.h"
#include "PhysicsSystem.h"
#include "CheatSystem.h"

// game entities
#include "TabletopCamera.h"
#include "Collector.h"
#include "Bounty.h"
#include "Stash.h"
#include "PlayerSpawn.h"
#include "BountySpawn.h"
#include "Wall.h"

// player controller
#include "AICollectorController.h"
#include "PlayerCollectorController.h"



class Game : protected ECS::Event::IEventListener {

public:

	///-------------------------------------------------------------------------------------------------
	/// EVENT HANDLER
	///-------------------------------------------------------------------------------------------------

	void OnRestartGame(const RestartGameEvent* event);
	void OnQuitGame(const QuitGameEvent* event);
	void OnToggleFullscreen(const ToggleFullscreenEvent* event);

	void OnCollisionBegin(const CollisionBeginEvent* event);

	void OnStashFull(const StashFull* event);

private:

	SDL_Window*					m_Window;

	int							m_WindowPosX;
	int							m_WindowPosY;

	int							m_WindowWidth;
	int							m_WindowHeight;

	const char*					m_GameTitle;

	GameContext					m_GameContext;

	GameState					m_GameState;

	ECS::SystemWorkStateMask	m_Ingame_SystemWSM;
	ECS::SystemWorkStateMask	m_NotIngame_SystemWSM;

private:

	void InitializeECS();

	void InitializeSDL();

	///-------------------------------------------------------------------------------------------------
	/// Fn:	void Game::ProcessWindowEvent();
	///
	/// Summary:	Process all window event.
	///
	/// Author:	Tobias Stein
	///
	/// Date:	4/10/2017
	///-------------------------------------------------------------------------------------------------

	void ProcessWindowEvent();

	void Terminate();

public:

	/** C'tor
	*/
	Game(const char* name = "Game Name");

	/** D'tor
	*/
	~Game();

	/** Init
		The init method will conatin all code that will initialize the new game application
		instance.
	*/
	void Initialize(int width, int height, bool fullscreen = false);

	///-------------------------------------------------------------------------------------------------
	/// Fn:	void Game::Step();
	///
	/// Summary:	Advances the game state by one frame.
	///
	/// Author:	Tobias Stein
	///
	/// Date:	19/11/2018
	///-------------------------------------------------------------------------------------------------

	void Step();

	///-------------------------------------------------------------------------------------------------
	/// Fn:	void Game::Restart();
	///
	/// Summary:	Restarts the game. Resets all game states.
	///
	/// Author:	Tobias Stein
	///
	/// Date:	19/11/2018
	///-------------------------------------------------------------------------------------------------

	void Restart();

	///-------------------------------------------------------------------------------------------------
	/// Fn:	void Game::GameOver();
	///
	/// Summary:	Game over.
	///
	/// Author:	Tobias Stein
	///
	/// Date:	19/11/2018
	///-------------------------------------------------------------------------------------------------

	void GameOver();

	inline SDL_Window*	GetWindow()				const { return this->m_Window; }

	inline void			ChangeState(GameState state)  { this->m_GameState = state; }
	inline GameState	GetActiveGameState()	const { return this->m_GameState; }
	inline bool			IsRestarted()			const { return (this->m_GameState == GameState::RESTARTED); }
	inline bool			IsRunning()				const { return (this->m_GameState == GameState::RUNNING); }
	inline bool			IsGameOver()			const { return (this->m_GameState == GameState::GAMEOVER); }
	inline bool			IsTerminated()			const { return (this->m_GameState == GameState::TERMINATED); }

	

}; // class GameApp

// global game instance, created in 'BountyHunterDemo.cpp'
extern Game* g_GameInstance;

#endif // __APPLICATION_H__