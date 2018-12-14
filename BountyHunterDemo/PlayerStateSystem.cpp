#include "PlayerStateSystem.h"
#include "PlayerSystem.h"
#include "RenderSystem.h"

#include "Collector.h"

PlayerStateSystem::PlayerStateSystem()
{
	RegisterEventCallbacks();
}

PlayerStateSystem::~PlayerStateSystem()
{
	UnregisterEventCallbacks();
}

void PlayerStateSystem::RegisterEventCallbacks()
{
	RegisterEventCallback(&PlayerStateSystem::OnPlayerJoined);
	RegisterEventCallback(&PlayerStateSystem::OnPlayerLeft);
	RegisterEventCallback(&PlayerStateSystem::OnPlayerDied);
	RegisterEventCallback(&PlayerStateSystem::OnPlayerSpawned);
	RegisterEventCallback(&PlayerStateSystem::OnPlayerPocketFillStateChange);
	RegisterEventCallback(&PlayerStateSystem::OnPlayerStashFillStateChange);
}

void PlayerStateSystem::UnregisterEventCallbacks()
{
	UnregisterEventCallback(&PlayerStateSystem::OnPlayerJoined);
	UnregisterEventCallback(&PlayerStateSystem::OnPlayerLeft);
	UnregisterEventCallback(&PlayerStateSystem::OnPlayerDied);
	UnregisterEventCallback(&PlayerStateSystem::OnPlayerSpawned);
	UnregisterEventCallback(&PlayerStateSystem::OnPlayerPocketFillStateChange);
	UnregisterEventCallback(&PlayerStateSystem::OnPlayerStashFillStateChange);
}

void PlayerStateSystem::PreUpdate(float dt)
{
	PlayerSystem* PS = ECS::ECS_Engine->GetSystemManager()->GetSystem<PlayerSystem>();

	for (PlayerId pID = 0; pID < INT_SETTING(MAX_PLAYER); ++pID)
	{
		Player* player = PS->GetPlayer(pID);
		if (player != nullptr)
		{
			// reset last frame accumulated reward
			this->m_PlayerStates[pID].playerReward = 0.0f;
		}
	}
}

void PlayerStateSystem::PostUpdate(float dt)
{
	PlayerSystem* PS = ECS::ECS_Engine->GetSystemManager()->GetSystem<PlayerSystem>();

	for (PlayerId pID = 0; pID < INT_SETTING(MAX_PLAYER); ++pID)
	{
		Player* player = PS->GetPlayer(pID);
		if (player != nullptr)
		{
			GameObjectId collectorID = player->GetController().GetPossessed();

			//Collector* collector = (Collector*)ECS::ECS_Engine->GetEntityManager()->GetEntity(collectorID);
			RigidbodyComponent* collectorRBcomp = ECS::ECS_Engine->GetComponentManager()->GetComponent<RigidbodyComponent>(collectorID);
			
			// update player transform state
			auto pos = collectorRBcomp->m_Box2DBody->GetPosition();
			this->m_PlayerStates[pID].playerPositionX = pos.x;
			this->m_PlayerStates[pID].playerPositionY = pos.y;
			this->m_PlayerStates[pID].playerRotation = glm::degrees(collectorRBcomp->m_Box2DBody->GetAngle());
		}
	}
}

void PlayerStateSystem::Reset()
{
}

#pragma region EVENT HANDLER

void PlayerStateSystem::OnPlayerJoined(const PlayerJoined* event)
{
	this->m_PlayerStates[event->playerID].playerDead = false;
	this->m_PlayerStates[event->playerID].playerPositionX = 0.0f;
	this->m_PlayerStates[event->playerID].playerPositionY = 0.0f;
	this->m_PlayerStates[event->playerID].playerRotation = 0.0f;
	this->m_PlayerStates[event->playerID].playerPocketLoad = 0.0f;
	this->m_PlayerStates[event->playerID].playerStashLoad = 0.0f;
	this->m_PlayerStates[event->playerID].playerReward = 0.0f;
}

void PlayerStateSystem::OnPlayerLeft(const PlayerJoined* event)
{
	// do nothing.
}

void PlayerStateSystem::OnPlayerDied(const PlayerDied* event)
{
	this->m_PlayerStates[event->playerID].playerDead = true;

	// GIVE REWARD
	this->m_PlayerStates[event->playerID].playerReward += FLOAT_SETTING(REWARD_SCALE_PLAYER_DIED);
}

void PlayerStateSystem::OnPlayerSpawned(const PlayerSpawned* event)
{
	this->m_PlayerStates[event->playerID].playerDead = false;
}

void PlayerStateSystem::OnPlayerPocketFillStateChange(const PlayerPocketFillStateChange* event)
{
	// GIVE REWARD, IFF NEW BOUNTY WAS COLLECTED
	if(event->fillState > this->m_PlayerStates[event->playerID].playerPocketLoad)
		this->m_PlayerStates[event->playerID].playerReward += (event->fillState - this->m_PlayerStates[event->playerID].playerPocketLoad) * FLOAT_SETTING(REWARD_SCALE_BOUNTY_COLLECTED);

	this->m_PlayerStates[event->playerID].playerPocketLoad = event->fillState;
}

void PlayerStateSystem::OnPlayerStashFillStateChange(const PlayerStashFillStateChange* event)
{
	// GIVE REWARD
	this->m_PlayerStates[event->playerID].playerReward += (event->fillState - this->m_PlayerStates[event->playerID].playerStashLoad) * FLOAT_SETTING(REWARD_SCALE_BOUNTY_STASHED);

	this->m_PlayerStates[event->playerID].playerStashLoad = event->fillState;	
}

#pragma endregion