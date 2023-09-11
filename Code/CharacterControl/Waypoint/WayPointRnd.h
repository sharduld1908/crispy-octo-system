#ifndef _CHARACTER_CONTROL_WAYPOINT_RND_NPC_
#define _CHARACTER_CONTROL_WAYPOINT_RND_NPC_

#include <vector>
#include <string>

#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/Math/Matrix4x4.h"
#include "WayPoint.h"

namespace CharacterControl {
	
	namespace Events {

		struct Event_CREATE_RND_WAYPOINT : public Event_CREATE_WAYPOINT
		{
			PE_DECLARE_CLASS(Event_CREATE_RND_WAYPOINT);

			// override SetLuaFunctions() since we are adding custom Lua interface
			static void SetLuaFunctions(PE::Components::LuaEnvironment* pLuaEnv, lua_State* luaVM);

			// Lua interface prefixed with l_
			static int l_Construct(lua_State* luaVM);

			int m_numRndWaypoints{};

		};

	}; // Events

	namespace Components {

		struct RndWayPoint : public WayPoint
		{
			PE_DECLARE_CLASS(RndWayPoint);

			RndWayPoint( PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself, const Events::Event_CREATE_RND_WAYPOINT *pEvt);

			virtual void addDefaultComponents();

			const char* GetNextWayPoint();

			std::vector<std::string> m_vecNextWayPoints;
			int m_numRndWaypoints{};

		};

	}; // namespace Components

}; // namespace CharacterControl

#endif

