///-------------------------------------------------------------------------------------------------
/// File:	Collector.h.
///
/// Summary:	Declares the collector entity class.
///-------------------------------------------------------------------------------------------------


#ifndef __COLLECTOR_ENTITY_H__
#define __COLLECTOR_ENTITY_H__

#include "GameObject.h"

#include "MaterialComponent.h"
#include "RigidbodyComponent.h"

class Collector : public GameObject<Collector>, public ECS::Event::IEventListener
{
private:

	TransformComponent*	m_ThisTransform;
	RigidbodyComponent* m_ThisRigidbody;
	MaterialComponent*	m_ThisMaterial;

	PlayerId			m_PlayerId;

	float				m_CollectedBounty;

	const float			m_MaxMoveSpeed;
	const float			m_MaxTurnSpeed;

public:

	Collector(GameObjectId spawnId);
	virtual ~Collector();

	virtual void OnEnable() override;
	virtual void OnDisable() override;

	///-------------------------------------------------------------------------------------------------
	/// Fn:	inline void Collector::SetPlayer(const PlayerId& playerId)
	///
	/// Summary:	Sets the current player controlling this collector.
	///
	/// Author:	Tobias Stein
	///
	/// Date:	22/10/2017
	///
	/// Parameters:
	/// playerId - 	Identifier for the player.
	///-------------------------------------------------------------------------------------------------

	inline void SetPlayer(const PlayerId playerId) 
	{ 
		this->m_ThisMaterial->SetColor(PLAYER_COLOR[playerId][0], PLAYER_COLOR[playerId][1], PLAYER_COLOR[playerId][2], PLAYER_COLOR[playerId][3]);
		this->m_PlayerId = playerId; 
	}

	inline const PlayerId GetPlayer() const { return this->m_PlayerId; }

	inline float GetCollectedBounty() const { return this->m_CollectedBounty; }

	///-------------------------------------------------------------------------------------------------
	/// Fn:	void Collector::Move(float speed);
	///
	/// Summary:	Moves the collector forward.
	///
	/// Author:	Tobias Stein
	///
	/// Date:	25/11/2018
	///
	/// Parameters:
	/// speed - 	The speed.
	///-------------------------------------------------------------------------------------------------

	void Move(float move);

	///-------------------------------------------------------------------------------------------------
	/// Fn:	void Collector::Turn(float degrees);
	///
	/// Summary:	Turns the collector. Positive = turn left, Negative = turn right.
	///
	/// Author:	Tobias Stein
	///
	/// Date:	25/11/2018
	///
	/// Parameters:
	/// degrees - 	The degrees.
	///-------------------------------------------------------------------------------------------------

	void Turn(float degrees);

	void ResetCollectedBounty();
	void CollectBounty(float bounty);

}; // class Collector

#endif // __COLLECTOR_ENTITY_H__
