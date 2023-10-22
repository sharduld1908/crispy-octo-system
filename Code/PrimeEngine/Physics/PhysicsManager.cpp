#include "PhysicsManager.h"
#include "PrimeEngine/Lua/LuaEnvironment.h"

#include "PrimeEngine/Scene/DebugRenderer.h"

namespace PE {

	namespace Components {

		PE_IMPLEMENT_CLASS1(PhysicsManager, Component);

		PhysicsManager::PhysicsManager(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself)
			: Component(context, arena, hMyself), g_physicsobjs(context, arena, 1) {}

		void PhysicsManager::addPhysicsObject(PhysicsObject* phyObj)
		{
			g_physicsobjs.add(phyObj);
		}

		PhysicsManager::CollisionData PhysicsManager::checkCollision(int checkingFor, std::vector<int> checkingWith) {
			CollisionData res;

			for (int checkWith : checkingWith) {

				if (checkingFor < g_physicsobjs.m_size
					&& checkWith < g_physicsobjs.m_size) {

					PhysicsObject* checkingFor_obj = g_physicsobjs[checkingFor];
					PhysicsObject* checkingWith_obj = g_physicsobjs[checkWith];

					checkCollisionWithObject(checkingFor_obj, checkingWith_obj, res);

					if (res.collided) {
						break;
					}

				}
			}

			return res;
		}

		void PhysicsManager::checkCollisionWithObject(PhysicsObject* checkingFor, PhysicsObject* checkingWith, PhysicsManager::CollisionData& collisionData) {
			Matrix4x4 m1 = checkingFor->m_base;
			Matrix4x4 m2 = checkingWith->m_base;

			auto p1 = (SoldierPhysicsObject*)checkingFor;
			auto p2 = checkingWith;

			if (p1) {

				Vector3 centre = p1->centre;
				//std::cout << "Original: " << centre.to_string();

				float radius = p1->radius;
				Matrix4x4 p2_base = p2->m_base.inverse();
				Vector3 centreInObjectSpace = p2_base * centre;
				//std::cout << "Transformed: " << centreInObjectSpace.to_string();

				PrimitiveTypes::Float32 max_x = p2->aabb_minmax[0];
				PrimitiveTypes::Float32 max_y = p2->aabb_minmax[1];
				PrimitiveTypes::Float32 max_z = p2->aabb_minmax[2];
				PrimitiveTypes::Float32 min_x = p2->aabb_minmax[3];
				PrimitiveTypes::Float32 min_y = p2->aabb_minmax[4];
				PrimitiveTypes::Float32 min_z = p2->aabb_minmax[5];

				PrimitiveTypes::Float32 closest_x = max(min_x, min(centreInObjectSpace.m_x, max_x));
				PrimitiveTypes::Float32 closest_y = max(min_y, min(centreInObjectSpace.m_y, max_y));
				PrimitiveTypes::Float32 closest_z = max(min_z, min(centreInObjectSpace.m_z, max_z));

				Vector3 closest_pt(closest_x, closest_y, closest_z);
				PrimitiveTypes::Float32 dist = (centreInObjectSpace - closest_pt).length();
				closest_pt = p2->m_base * closest_pt;

				collisionData.dir = centre - closest_pt;
				collisionData.dir.normalize();
				collisionData.intersection_dist = abs(dist - radius);

				Vector3 ptToCheck = p1->ptToCheck;
				Vector3 ptToCheckInObjectSpace = p2_base * ptToCheck;
				collisionData.collided = (isPointInsideCuboid(p2, ptToCheckInObjectSpace) || dist < radius);

				/*const int numPts = 2;
				int iPt = 0;

				Vector3 color(1.0f, 1.0f, 1.0f);
				Vector3 linepts[numPts * 2];

				linepts[iPt++] = centre;
				linepts[iPt++] = color;
				linepts[iPt++] = closest_pt;
				linepts[iPt++] = color;

				DebugRenderer::Instance()->createLineMesh(true, p2->m_base, &linepts[0].m_x, numPts, 0);*/

			}

		}

		bool PhysicsManager::isPointInsideCuboid(PhysicsObject* p2, Vector3& point)
		{
			// Initialize the min and max bounds with the first vertex
			int max_x = p2->aabb_minmax[0];
			int max_y = p2->aabb_minmax[1];
			int max_z = p2->aabb_minmax[2];
			int min_x = p2->aabb_minmax[3];
			int min_y = p2->aabb_minmax[4];
			int min_z = p2->aabb_minmax[5];

			return (point.getX() >= min_x && point.getX() <= max_x &&
					point.getY() >= min_x && point.getY() <= max_y &&
					point.getZ() >= min_x && point.getZ() <= max_z);

			
		}

	}
}