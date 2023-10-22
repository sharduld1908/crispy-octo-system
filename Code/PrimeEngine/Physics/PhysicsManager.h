#pragma once

#ifndef __pe_physicsmanager_h__
#define __pe_physicsmanager_h__

#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/Utils/Array/Array.h"
#include "PrimeEngine/Math/Vector3.h"

#include "PhysicsObject.h"

#include <iostream>

#include <vector>
#include <unordered_map>

namespace PE {

	namespace Components {

		class PhysicsManager : public Component {
		public:

			struct CollisionData {
				bool collided;
				float intersection_dist;
				Vector3 dir;
			};

			PE_DECLARE_CLASS(PhysicsManager);

			PhysicsManager(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself);

			Array<PhysicsObject*,1> g_physicsobjs;

			void addPhysicsObject(PhysicsObject* phyObj);

			CollisionData checkCollision(int checkingFor, std::vector<int> checkingWith);
			void checkCollisionWithObject(PhysicsObject* checkingFor, PhysicsObject* checkingWith, CollisionData& collision);
			bool isPointInsideCuboid(PhysicsObject* p2, Vector3& point);

		};
	}
}

#endif
