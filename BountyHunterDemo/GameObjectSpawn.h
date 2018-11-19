///-------------------------------------------------------------------------------------------------
/// File:	GameObjectSpawn.h.
///
/// Summary:	Declares the game object spawn class.
///-------------------------------------------------------------------------------------------------


#ifndef __GAME_OBJECT_SPAWN_H__
#define __GAME_OBJECT_SPAWN_H__

#include "GameObject.h"
#include "SpawnComponent.h"

class GameObjectSpawn : public GameObject<GameObjectSpawn>
{
public:

	virtual SpawnInfo GetSpawnInfo() = 0;

}; // class GameObjectSpawn

#endif // __GAME_OBJECT_SPAWN_H__
