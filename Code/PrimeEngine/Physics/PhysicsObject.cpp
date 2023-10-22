#include "PhysicsObject.h"
#include "PrimeEngine/Physics/PhysicsObject.h"
#include "PrimeEngine/Geometry/PositionBufferCPU/PositionBufferCPU.h"
#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "PrimeEngine/Utils/PEMap.h"
#include "PrimeEngine/Scene/MeshManager.h"
#include "PrimeEngine/Scene/Skeleton.h"
#include "PrimeEngine/Scene/Mesh.h"

namespace PE {

	namespace Components {

		PE_IMPLEMENT_CLASS1(PhysicsObject, Component);
		
		PhysicsObject::~PhysicsObject()
		{
		}

		PhysicsObject::PhysicsObject(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself, const char* assetName, const char* assetPackage)
			: Component(context, arena, hMyself)
		{
		
			char key[StrTPair<Handle>::StrSize];
			sprintf(key, "%s/%s", assetPackage, assetName);

			int index = m_pContext->getMeshManager()->m_assets.findIndex(key);
			Handle h;
			if (index != -1)
			{
				h = m_pContext->getMeshManager()->m_assets.m_pairs[index].m_value;
			}

			if (StringOps::endswith(assetName, "skela"))
			{
				Skeleton* skeleton = h.getObject<Skeleton>();
				// Code to be added for skeleton if required. I don't think it is required for this assignment.
			}
			else if (StringOps::endswith(assetName, "mesha"))
			{
				Mesh* mesh = h.getObject<Mesh>();
				PositionBufferCPU* position_buffer_cpu = mesh->m_hPositionBufferCPU.getObject<PositionBufferCPU>();

				Min_Max_AABB::Pos_Coordinates max_aabb = position_buffer_cpu->min_max_aabb.max_coordinates;
				Min_Max_AABB::Pos_Coordinates min_aabb = position_buffer_cpu->min_max_aabb.min_coordinates;

				aabb_minmax.push_back(max_aabb.m_x_pos);
				aabb_minmax.push_back(max_aabb.m_y_pos);
				aabb_minmax.push_back(max_aabb.m_z_pos);
				aabb_minmax.push_back(min_aabb.m_x_pos);
				aabb_minmax.push_back(min_aabb.m_y_pos);
				aabb_minmax.push_back(min_aabb.m_z_pos);

			}
		}

		void PhysicsObject::set_Mbase(Matrix4x4 base)
		{
			m_base = base;
		}

		std::vector<Vector3> PhysicsObject::CalculateBoundingCoordinates()
		{
			std::vector<Vector3> aabb_box;

			aabb_box.push_back(m_base * Vector3(aabb_minmax[0], aabb_minmax[1], aabb_minmax[2]));
			aabb_box.push_back(m_base * Vector3(aabb_minmax[0], aabb_minmax[1], aabb_minmax[5]));
			aabb_box.push_back(m_base * Vector3(aabb_minmax[0], aabb_minmax[4], aabb_minmax[2]));
			aabb_box.push_back(m_base * Vector3(aabb_minmax[0], aabb_minmax[4], aabb_minmax[5]));
			aabb_box.push_back(m_base * Vector3(aabb_minmax[3], aabb_minmax[1], aabb_minmax[2]));
			aabb_box.push_back(m_base * Vector3(aabb_minmax[3], aabb_minmax[1], aabb_minmax[5]));
			aabb_box.push_back(m_base * Vector3(aabb_minmax[3], aabb_minmax[4], aabb_minmax[2]));
			aabb_box.push_back(m_base * Vector3(aabb_minmax[3], aabb_minmax[4], aabb_minmax[5]));

			return aabb_box;
		}

		PE_IMPLEMENT_CLASS1(SoldierPhysicsObject, PhysicsObject);

		SoldierPhysicsObject::SoldierPhysicsObject(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself, const char* assetName, const char* assetPackage)
			: PhysicsObject(context, arena, hMyself, assetName, assetPackage) 
		{
			radius = 0;
		}

		void SoldierPhysicsObject::CalculateSphere(Vector3 dist)
		{
			centre = Vector3(m_base.getPos().m_x, m_base.getPos().m_y + 0.5, m_base.getPos().m_z);
			radius = 0.5;

			dist.normalize();
			ptToCheck = centre + dist*radius;

			aabb_minmax[0] = centre.m_x + radius;
			aabb_minmax[1] = centre.m_y + radius;
			aabb_minmax[2] = centre.m_z + radius;

			aabb_minmax[3] = centre.m_x - radius;
			aabb_minmax[4] = centre.m_y - radius;

			aabb_minmax[5] = centre.m_z - radius;

		}

		std::vector<Vector3> SoldierPhysicsObject::CalculateBoundingCoordinates()
		{
			std::vector<Vector3> aabb_box;

			aabb_box.push_back(Vector3(aabb_minmax[0], aabb_minmax[1], aabb_minmax[2]));
			aabb_box.push_back(Vector3(aabb_minmax[0], aabb_minmax[1], aabb_minmax[5]));
			aabb_box.push_back(Vector3(aabb_minmax[0], aabb_minmax[4], aabb_minmax[2]));
			aabb_box.push_back(Vector3(aabb_minmax[0], aabb_minmax[4], aabb_minmax[5]));
			aabb_box.push_back(Vector3(aabb_minmax[3], aabb_minmax[1], aabb_minmax[2]));
			aabb_box.push_back(Vector3(aabb_minmax[3], aabb_minmax[1], aabb_minmax[5]));
			aabb_box.push_back(Vector3(aabb_minmax[3], aabb_minmax[4], aabb_minmax[2]));
			aabb_box.push_back(Vector3(aabb_minmax[3], aabb_minmax[4], aabb_minmax[5]));

			return aabb_box;
		}

		SoldierPhysicsObject::~SoldierPhysicsObject()
		{
		}

	}
}