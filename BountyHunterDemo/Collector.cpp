///-------------------------------------------------------------------------------------------------
/// File:	Collector.cpp.
///
/// Summary:	Implements the collector class.
///-------------------------------------------------------------------------------------------------

#include "Collector.h"

#include "ShapeComponent.h"
#include "RespawnComponent.h"
#include "CollisionComponent2D.h"

#include "ShapeGenerator.h"
#include "MaterialGenerator.h"

Collector::Collector(GameObjectId spawnId) :
	m_PlayerId(INVALID_PLAYER_ID),
	m_CollectedBounty(0.0f),
	m_MaxMoveSpeed(FLOAT_SETTING(COLLECTOR_MAX_MOVE_SPEED)),
	m_MaxTurnSpeed(FLOAT_SETTING(COLLECTOR_MAX_TURN_SPEED))
{
	Shape shape = ShapeGenerator::CreateShape<TriangleShape>();

	AddComponent<ShapeComponent>(shape);
	this->m_ThisMaterial = AddComponent<MaterialComponent>(MaterialGenerator::CreateMaterial<DefaultMaterial>());
	AddComponent<RespawnComponent>(FLOAT_SETTING(COLLECTOR_RESPAWNTIME), spawnId, true);
	this->m_ThisTransform = GetComponent<TransformComponent>();
	this->m_ThisRigidbody = AddComponent<RigidbodyComponent>();
	AddComponent<CollisionComponent2D>(shape, this->m_ThisTransform->AsTransform()->GetScale(), CollisionCategory::Player_Category, CollisionMask::Player_Collision);

}

Collector::~Collector()
{
}



void Collector::OnEnable()
{
	this->m_ThisRigidbody->SetTransform(*this->m_ThisTransform->AsTransform());
	this->m_ThisRigidbody->m_Box2DBody->SetLinearVelocity(b2Vec2_zero);
	this->m_ThisRigidbody->m_Box2DBody->SetAngularVelocity(0.0f);
	this->m_ThisRigidbody->m_Box2DBody->SetActive(true);
}

void Collector::OnDisable()
{
	this->m_ThisRigidbody->m_Box2DBody->SetActive(false);
}


void Collector::Move(float move)
{
	glm::vec2 vel = this->m_ThisTransform->AsTransform()->GetUp() * glm::clamp(move, 0.0f, this->m_MaxMoveSpeed);
	this->m_ThisRigidbody->m_Box2DBody->SetLinearVelocity(b2Vec2(vel.x, vel.y));
}

void Collector::Turn(float degrees_sec)
{
	this->m_ThisRigidbody->m_Box2DBody->SetAngularVelocity(glm::clamp(degrees_sec, -this->m_MaxMoveSpeed, this->m_MaxMoveSpeed));
}

void Collector::ResetCollectedBounty()
{
	this->m_CollectedBounty = 0.0f;
}

void Collector::CollectBounty(float bounty)
{
	this->m_CollectedBounty = glm::min<float>(this->m_CollectedBounty + bounty, FLOAT_SETTING(PLAYER_POCKET_SIZE));
}