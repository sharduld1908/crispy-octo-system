#pragma once

#ifndef __pe_physicsmanager_h__
#define __pe_physicsmanager_h__

#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/Utils/Array/Array.h"
#include "PrimeEngine/Math/Vector3.h"

#include "PhysicsObject.h"
#include <iostream>

#include <vector>

namespace PE {

	namespace Components {

		class PhysicsManager : public Component {
		public:

			PE_DECLARE_CLASS(PhysicsManager);

			PhysicsManager(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself);

			Array<PhysicsObject*,1> g_physicsobjs;

			void addPhysicsObject(PhysicsObject* phyObj);
		};
	}
}

#endif
