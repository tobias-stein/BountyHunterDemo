///-------------------------------------------------------------------------------------------------
/// File:	Bounty.cpp.
///
/// Summary:	Implements the bounty class.
///-------------------------------------------------------------------------------------------------

#include "Bounty.h"

#include "ShapeComponent.h"
#include "RespawnComponent.h"

#include "ShapeGenerator.h"
#include "MaterialGenerator.h"




Bounty::Bounty(GameObjectId spawnId)
{
	Shape shape = ShapeGenerator::CreateShape<QuadShape>();

	AddComponent<ShapeComponent>(shape);
	this->m_ThisMaterial = AddComponent<MaterialComponent>(MaterialGenerator::CreateMaterial<DefaultMaterial>());
	AddComponent<RespawnComponent>(FLOAT_SETTING(BOUNTY_RESPAWNTIME), spawnId, true);
	this->m_ThisRigidbody = AddComponent<RigidbodyComponent>(0.0f, 0.0f, 0.0f, 0.0f, 0.0001f);
	this->m_ThisTransform = GetComponent<TransformComponent>();
	this->m_ThisCollision = AddComponent<CollisionComponent2D>(shape, this->m_ThisTransform->AsTransform()->GetScale(), CollisionCategory::Bounty_Category, CollisionMask::Bounty_Collision);
	this->m_ThisLifetime = AddComponent<LifetimeComponent>(FLOAT_SETTING(BOUNTY_MIN_LIFETIME), FLOAT_SETTING(BOUNTY_MAX_LIFETIME));
}

Bounty::~Bounty()
{
}

void Bounty::OnEnable()
{
	ShuffleBounty();

	this->m_ThisLifetime->ResetLifetime();

	this->m_ThisRigidbody->SetTransform(*this->m_ThisTransform->AsTransform());
	this->m_ThisRigidbody->m_Box2DBody->SetLinearVelocity(b2Vec2_zero);
	this->m_ThisRigidbody->m_Box2DBody->SetAngularVelocity(0.0f);
	this->m_ThisRigidbody->m_Box2DBody->SetActive(true);
}

void Bounty::OnDisable()
{
	this->m_ThisRigidbody->m_Box2DBody->SetActive(false);
}

void Bounty::ShuffleBounty()
{
	float alpha = glm::linearRand(0.0f, 1.0f);

	this->m_Value = glm::lerp(FLOAT_SETTING(BOUNTY_MIN_VALUE), FLOAT_SETTING(BOUNTY_MAX_VALUE), alpha);

	float scale = glm::lerp(FLOAT_SETTING(BOUNTY_MIN_SCALE), FLOAT_SETTING(BOUNTY_MAX_SCALE), alpha) * FLOAT_SETTING(GLOBAL_SCALE);

	this->m_ThisTransform->AsTransform()->SetScale(glm::vec3(scale));

	this->m_ThisMaterial->SetColor(FLOAT_SETTING(BOUNTY_COLOR_R), FLOAT_SETTING(BOUNTY_COLOR_G), FLOAT_SETTING(BOUNTY_COLOR_B), FLOAT_SETTING(BOUNTY_COLOR_A));
	this->m_ThisRigidbody->SetScale(glm::vec2(scale));
}
