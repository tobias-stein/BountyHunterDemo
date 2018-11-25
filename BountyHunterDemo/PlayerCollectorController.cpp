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


void PlayerCollectorController::Update(const ActionState* action)
{
	if (this->m_Pawn->IsActive() == false || action == nullptr)
		return;

	// forward action to pawn
	this->m_Pawn->Move(action->move);
	this->m_Pawn->Turn(action->turn);
}