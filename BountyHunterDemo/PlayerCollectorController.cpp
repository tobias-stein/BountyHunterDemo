///-------------------------------------------------------------------------------------------------
/// File:	PlayerCollectorController.cpp.
///
/// Summary:	Implements the player collector controller class.
///-------------------------------------------------------------------------------------------------

#include "PlayerCollectorController.h"


PlayerCollectorController::PlayerCollectorController(const GameObjectId collectorId, const PlayerId playerId) : PlayerController(collectorId)
{
	this->m_Pawn->SetPlayer(playerId);
}

PlayerCollectorController::~PlayerCollectorController()
{
}


void PlayerCollectorController::Update(const PlayerAction* action)
{
	if (this->m_Pawn->IsActive() == false || action == nullptr)
		return;

	// forward action to pawn
	this->m_Pawn->Move(glm::clamp(action->move,  0.0f, 1.0f));
	this->m_Pawn->Turn(glm::clamp(action->turn, -1.0f, 1.0f));
}