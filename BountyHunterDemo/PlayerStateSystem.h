///-------------------------------------------------------------------------------------------------
/// File:	PlayerStateSystem.h.
///
/// Summary:	The PlayerStateSystem takes responsibility to track all players current states.
/// It will listen to events like 'player died/respawned', 'bounty collected/stashed' etc. and will
/// update the corresponding players state according to his actions.
///-------------------------------------------------------------------------------------------------

#pragma once

#ifndef __PLAYER_STATE_SYSTEM_H__
#define __PLAYER_STATE_SYSTEM_H__

#include <ECS/ECS.h>
#include "GameTypes.h"
#include "GameEvents.h"

class PlayerStateSystem : public ECS::System<PlayerStateSystem>, protected ECS::Event::IEventListener
{

private:

	/// Summary:	An array of player states. Player slots not occupied are 'nullptr'.
	PlayerState			m_PlayerStates[MAX_PLAYER];

private:
	
	void RegisterEventCallbacks();
	void UnregisterEventCallbacks();

	void OnPlayerJoined(const PlayerJoined* event);
	void OnPlayerLeft(const PlayerJoined* event);
	void OnPlayerDied(const PlayerDied* event);
	void OnPlayerSpawned(const PlayerSpawned* event);
	void OnPlayerPocketFillStateChange(const PlayerPocketFillStateChange* event);
	void OnPlayerStashFillStateChange(const PlayerStashFillStateChange* event);

public:

	PlayerStateSystem();
	virtual ~PlayerStateSystem();

	virtual void PreUpdate(float dt) override;
	virtual void PostUpdate(float dt) override;

	void Reset();

	inline const PlayerState* GetPlayerStates() { return (const PlayerState*)&this->m_PlayerStates; }

}; // class PlayerStateSystem

#endif // __PLAYER_STATE_SYSTEM_H__
