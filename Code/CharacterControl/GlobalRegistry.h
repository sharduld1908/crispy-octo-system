#ifndef _CharacterControl_GLOBAL_REGISTRY_
#define _CharacterControl_GLOBAL_REGISTRY_

#include "Waypoint/WayPoint.h"
#include "Waypoint/WaypointRnd.h"
#include "Characters/SoldierNPC.h"
#include "Characters/SoldierNPCAnimationSM.h"
#include "Characters/SoldierNPCMovementSM.h"
#include "Characters/SoldierNPCBehaviorSM.h"
#include "Events/Events.h"
#include "GameObjectMangerAddon.h"
#include "ClientGameObjectManagerAddon.h"
#include "ServerGameObjectManagerAddon.h"
#include "ClientCharacterControlGame.h"
#include "ServerCharacterControlGame.h"
#include "Tank/ClientTank.h"
#include "CharacterControl/Client/ClientSpaceShip.h"
#include "CharacterControl/Client/ClientSpaceShipControls.h"

namespace PE
{
	struct GlobalRegistry;
	namespace Components
	{
		struct LuaEnvironment;
	}
}
namespace CharacterControl{
	void Register(PE::Components::LuaEnvironment *pLuaEnv, PE::GlobalRegistry *pRegistry);
};
#endif
