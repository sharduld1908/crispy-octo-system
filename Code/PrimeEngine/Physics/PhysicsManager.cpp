#include "PhysicsManager.h"
#include "PrimeEngine/Lua/LuaEnvironment.h"


namespace PE {

	namespace Components {
		
		PE_IMPLEMENT_CLASS1(PhysicsManager, Component)

		PhysicsManager::PhysicsManager(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself)
			: Component(context, arena, hMyself), g_physicsobjs(context, arena, 1) {}

		void PhysicsManager::addPhysicsObject(PhysicsObject* phyObj)
		{
			g_physicsobjs.add(phyObj);
			std::cout << "Hi";
		}

	}
}