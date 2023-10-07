#define NOMINMAX
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes

// Inter-Engine includes
#include "PrimeEngine/FileSystem/FileReader.h"
#include "PrimeEngine/APIAbstraction/GPUMaterial/GPUMaterialSet.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"
#include "PrimeEngine/APIAbstraction/Texture/Texture.h"
#include "PrimeEngine/APIAbstraction/Effect/EffectManager.h"
#include "PrimeEngine/APIAbstraction/GPUBuffers/VertexBufferGPUManager.h"
#include "PrimeEngine/Scene/Skeleton.h"
#include "DefaultAnimationSM.h"
#include "Light.h"

#include "PrimeEngine/GameObjectModel/Camera.h"

// Sibling/Children includes
#include "MeshInstance.h"
#include "MeshManager.h"
#include "SceneNode.h"
#include "CameraManager.h"
#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "PrimeEngine/Events/StandardEvents.h"

namespace PE {

	namespace Components {

		PE_IMPLEMENT_CLASS1(MeshInstance, Component);

		MeshInstance::MeshInstance(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself)
			: Component(context, arena, hMyself), AABB_Coordinates(context, arena, 8), m_culledOut(false)
		{

		}

		void MeshInstance::addDefaultComponents()
		{
			Component::addDefaultComponents();
		}

		void MeshInstance::CalculateBoundingCoordinates(const char* assetName, const char* assetPackage)
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
				
				PositionBufferCPU::Min_Max_AABB::Pos_Coordinates max_aabb = position_buffer_cpu->min_max_aabb.max_coordinates;
				PositionBufferCPU::Min_Max_AABB::Pos_Coordinates min_aabb = position_buffer_cpu->min_max_aabb.min_coordinates;

				AABB_Coordinates.add(Vector3(max_aabb.m_x_pos, max_aabb.m_y_pos, max_aabb.m_z_pos));
				AABB_Coordinates.add(Vector3(max_aabb.m_x_pos, max_aabb.m_y_pos, min_aabb.m_z_pos));
				AABB_Coordinates.add(Vector3(max_aabb.m_x_pos, min_aabb.m_y_pos, max_aabb.m_z_pos));
				AABB_Coordinates.add(Vector3(max_aabb.m_x_pos, min_aabb.m_y_pos, min_aabb.m_z_pos));
				AABB_Coordinates.add(Vector3(min_aabb.m_x_pos, max_aabb.m_y_pos, max_aabb.m_z_pos));
				AABB_Coordinates.add(Vector3(min_aabb.m_x_pos, max_aabb.m_y_pos, min_aabb.m_z_pos));
				AABB_Coordinates.add(Vector3(min_aabb.m_x_pos, min_aabb.m_y_pos, max_aabb.m_z_pos));
				AABB_Coordinates.add(Vector3(min_aabb.m_x_pos, min_aabb.m_y_pos, min_aabb.m_z_pos));
				
			}

		}

		void MeshInstance::initFromFile(const char* assetName, const char* assetPackage, int& threadOwnershipMask)
		{
			Handle h = m_pContext->getMeshManager()->getAsset(assetName, assetPackage, threadOwnershipMask);
			CalculateBoundingCoordinates(assetName, assetPackage);
			initFromRegisteredAsset(h);
		}

		bool MeshInstance::hasSkinWeights()
		{
			Mesh* pMesh = m_hAsset.getObject<Mesh>();
			return pMesh->m_hSkinWeightsCPU.isValid();
		}

		void MeshInstance::initFromRegisteredAsset(const PE::Handle& h)
		{
			m_hAsset = h;
			//add this instance as child to Mesh so that skin knows what to draw
			static int allowedEvts[] = { 0 };
			m_hAsset.getObject<Component>()->addComponent(m_hMyself, &allowedEvts[0]);
		}


	}; // namespace Components
}; // namespace PE
