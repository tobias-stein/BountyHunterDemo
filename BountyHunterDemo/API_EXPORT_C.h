#pragma once

#ifndef __API_EXPORT_C_H__
#define __API_EXPORT_C_H__

#ifdef BOUNTY_AI_API_EXPORT
	#define BOUNTY_AI_API __declspec(dllexport)   
#else  
	#define BOUNTY_AI_API __declspec(dllimport)   
#endif  


#ifdef __cplusplus
extern "C" {
#endif

	///-------------------------------------------------------------------------------------------------
	/// Fn:	Game* CreateNewGameInstance()
	///
	/// Summary:	Creates a new game instance.
	///
	/// Author:	Tobias Stein
	///
	/// Date:	10/12/2018
	///
	/// Returns:	Null if it fails, else the new new game instance.
	///-------------------------------------------------------------------------------------------------

	BOUNTY_AI_API void* CreateNewGameInstance();

	///-------------------------------------------------------------------------------------------------
	/// Fn:	void InitializeGame(Game* gameInstance)
	///
	/// Summary:	Initializes the game.
	///
	/// Author:	Tobias Stein
	///
	/// Date:	10/12/2018
	///
	/// Parameters:
	/// gameInstance - 	[in,out] If non-null, the game instance.
	///-------------------------------------------------------------------------------------------------

	BOUNTY_AI_API void InitializeGame(void* gameInstance);

	///-------------------------------------------------------------------------------------------------
	/// Fn:	void RestartGame(Game* gameInstance)
	///
	/// Summary:	Restart game.
	///
	/// Author:	Tobias Stein
	///
	/// Date:	10/12/2018
	///
	/// Parameters:
	/// gameInstance - 	[in,out] If non-null, the game instance.
	///-------------------------------------------------------------------------------------------------

	BOUNTY_AI_API void RestartGame(void* gameInstance);

	///-------------------------------------------------------------------------------------------------
	/// Fn:	BOUNTY_AI_API long AddPlayer();
	///
	/// Summary:	Adds player.
	///
	/// Author:	Tobias Stein
	///
	/// Date:	11/12/2018
	///
	/// Returns:	A long.
	///-------------------------------------------------------------------------------------------------

	BOUNTY_AI_API size_t AddPlayer(void* gameInstance);

	///-------------------------------------------------------------------------------------------------
	/// Fn:
	/// BOUNTY_AI_API bool StepGame(void* gameInstance, struct PlayerAction** const in_actions,
	/// struct PlayerState* out_states, void* out_framebuffer);
	///
	/// Summary:	Step game.
	///
	/// Author:	Tobias Stein.
	///
	/// Date:	13/12/2018.
	///
	/// Parameters:
	/// gameInstance -    	[in,out] If non-null, the game instance.
	/// in_actions - 	  	[in,out] If non-null, the in actions.
	/// out_states - 	  	[in,out] If non-null, list of states of the outs.
	/// out_framebuffer - 	[in,out] If non-null, the out framebuffer.
	///
	/// Returns:	Returns non-negative value, if game is over. Value indicates winner id.
	///-------------------------------------------------------------------------------------------------

	BOUNTY_AI_API int StepGame(void* gameInstance, struct PlayerAction** const in_actions, struct PlayerState* out_states, void* out_framebuffer);

	///-------------------------------------------------------------------------------------------------
	/// Fn:	void TerminateGame(Game* gameInstance)
	///
	/// Summary:	Terminate game.
	///
	/// Author:	Tobias Stein
	///
	/// Date:	10/12/2018
	///
	/// Parameters:
	/// gameInstance - 	[in,out] If non-null, the game instance.
	///-------------------------------------------------------------------------------------------------

	BOUNTY_AI_API void TerminateGame(void* gameInstance);

#ifdef __cplusplus
}
#endif

#endif // __API_EXPORT_C_H__