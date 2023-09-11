#ifndef _CHARACTER_CONTROL_WAYPOINT_RND_NPC_
#define _CHARACTER_CONTROL_WAYPOINT_RND_NPC_

#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/Math/Matrix4x4.h"

namespace CharacterControl {
	
	namespace Events {

		struct Event_CREATE_RND_WAYPOINT : public PE::Events::Event
		{
			PE_DECLARE_CLASS(Event_CREATE_RND_WAYPOINT);

			// override SetLuaFunctions() since we are adding custom Lua interface
			static void SetLuaFunctions(PE::Components::LuaEnvironment* pLuaEnv, lua_State* luaVM);

			// Lua interface prefixed with l_
			static int l_Construct(lua_State* luaVM);

			Matrix4x4 m_base;
			char m_name[32]{};
			char m_nextWaypointName[32]{};

			PEUUID m_peuuid; // unique object id

		};

	}; // Events

	namespace Components {

		struct RndWayPoint : public PE::Components::Component
		{
			PE_DECLARE_CLASS(RndWayPoint);

			RndWayPoint( PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself, const Events::Event_CREATE_RND_WAYPOINT *pEvt);

			virtual void addDefaultComponents() ;

			char m_name[32];
			char m_nextWayPointName[32];
			Matrix4x4 m_base;

		};

	}; // namespace Components

}; // namespace CharacterControl

#endif

