///-------------------------------------------------------------------------------------------------
/// File:	Stash.cpp.
///
/// Summary:	Implements the stash class.
///-------------------------------------------------------------------------------------------------

#include "Stash.h"

#include "ShapeComponent.h"
#include "CollisionComponent2D.h"

#include "ShapeGenerator.h"
#include "MaterialGenerator.h"


Stash::Stash(PlayerId playerId) :
	m_OwningPlayer(playerId),
	m_StashedBounty(0.0f)
{
	Shape shape = ShapeGenerator::CreateShape<QuadShape>();

	AddComponent<ShapeComponent>(shape);
	this->m_ThisMaterial = AddComponent<MaterialComponent>(MaterialGenerator::CreateMaterial<DefaultMaterial>());
	this->m_ThisRigidbody = AddComponent<RigidbodyComponent>();
	auto cc = AddComponent<CollisionComponent2D>(shape, GetComponent<TransformComponent>()->AsTransform()->GetScale(), CollisionCategory::Stash_Category, CollisionMask::Stash_Collision);
	cc->isSensor = true;

	this->m_ThisMaterial->SetColor(PLAYER_COLOR[playerId][0], PLAYER_COLOR[playerId][1], PLAYER_COLOR[playerId][2], PLAYER_COLOR[playerId][3]);
}

Stash::~Stash()
{
	this->m_OwningPlayer = INVALID_PLAYER_ID;
	this->m_StashedBounty = 0.0f;
}

void Stash::OnEnable()
{
	this->m_ThisRigidbody->m_Box2DBody->SetActive(true);
}

void Stash::OnDisable()
{
	this->m_ThisRigidbody->m_Box2DBody->SetActive(false);
}

void Stash::StashBounty(float bounty)
{
	if (this->m_StashedBounty + bounty > PLAYER_STASH_SIZE)
	{
		this->m_StashedBounty = PLAYER_STASH_SIZE;
		ECS::ECS_Engine->SendEvent<StashFull>(this->GetEntityID());
	}
	else
	{
		this->m_StashedBounty += bounty;
	}
}