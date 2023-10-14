#pragma once

#ifndef __pe_physicsobj_h__
#define __pe_physicsobj_h__

#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/Math/Vector3.h"
#include "PrimeEngine/Math/Matrix4x4.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"

#include <vector>

namespace PE {
	namespace Components {

		class PhysicsObject : public Component {
		public:

			PE_DECLARE_CLASS(PhysicsObject);
			
			std::vector<PrimitiveTypes::Float32> aabb_minmax;
			Matrix4x4 m_base;

			PhysicsObject(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself, const char* assetName, const char* assetPackage);

			void set_Mbase(Matrix4x4 base);

			virtual std::vector<Vector3> CalculateBoundingCoordinates();

			virtual ~PhysicsObject();
		};

		class SoldierPhysicsObject : public PhysicsObject {
		public:

			PE_DECLARE_CLASS(SoldierPhysicsObject);

			SoldierPhysicsObject(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself, const char* assetName, const char* assetPackage);

			void CalculateSphere();

			virtual std::vector<Vector3> CalculateBoundingCoordinates() override;

			virtual ~SoldierPhysicsObject();

			float radius;
			Vector3 centre;
		};
	}
}

#endif