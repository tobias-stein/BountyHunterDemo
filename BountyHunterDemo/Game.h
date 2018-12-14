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
#include "PlayerStateSystem.h"

// game entities
#include "TabletopCamera.h"
#include "Collector.h"
#include "Bounty.h"
#include "Stash.h"
#include "PlayerSpawn.h"
#include "BountySpawn.h"
#include "Wall.h"

// player controller
#include "PlayerCollectorController.h"

class Game : protected ECS::Event::IEventListener {

public:

	///-------------------------------------------------------------------------------------------------
	/// EVENT HANDLER
	///-------------------------------------------------------------------------------------------------

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
	void Initialize(int width, int height);

	///-------------------------------------------------------------------------------------------------
	/// Fn:
	/// bool Game::Step(PlayerAction** const in_actions, PlayerState* out_states,
	/// void* out_framebuffer);
	///
	/// Summary:
	/// Advances the game state by one frame. The step function takes in an array of actions one for
	/// each player.
	///
	/// Author:	Tobias Stein.
	///
	/// Date:	25/11/2018.
	///
	/// Parameters:
	/// in_actions - 	  	[in] Actions per collector.
	/// out_states - 	  	[in,out] If non-null, list of states of the outs.
	/// out_framebuffer - 	[in,out] If non-null, the out framebuffer.
	///
	/// Returns:	Returns non-negative value, if game is over. Value indicates winner id.
	///
	///-------------------------------------------------------------------------------------------------

	int Step(PlayerAction** const in_actions, PlayerState* out_states, void* out_framebuffer);

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
	/// Fn:	PlayerId Game::AddPlayer();
	///
	/// Summary:	Adds a new player.
	///
	/// Author:	Tobias Stein
	///
	/// Date:	25/11/2018
	///
	/// Returns:	A PlayerId.
	///-------------------------------------------------------------------------------------------------

	PlayerId AddPlayer();

	///-------------------------------------------------------------------------------------------------
	/// Fn:	void Game::GameOver(PlayerId winnerId);
	///
	/// Summary:	Game over.
	///
	/// Author:	Tobias Stein.
	///
	/// Date:	19/11/2018.
	///
	/// Parameters:
	/// winnerId - 	Identifier for the winner.
	///-------------------------------------------------------------------------------------------------

	void GameOver(PlayerId winnerId);

	inline SDL_Window*	GetWindow()				const { return this->m_Window; }

	inline void			ChangeState(GameState state)  { this->m_GameState = state; }
	inline GameState	GetActiveGameState()	const { return this->m_GameState; }
	inline bool			IsRestarted()			const { return (this->m_GameState == GameState::RESTARTED); }
	inline bool			IsRunning()				const { return (this->m_GameState == GameState::RUNNING); }
	inline bool			IsGameOver()			const { return (this->m_GameState == GameState::GAMEOVER); }
	inline bool			IsTerminated()			const { return (this->m_GameState == GameState::TERMINATED); }


}; // class GameApp

#endif // __APPLICATION_H__