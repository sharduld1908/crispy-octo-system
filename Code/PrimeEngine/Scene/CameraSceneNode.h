#ifndef __PYENGINE_2_0_CAMERA_SCENE_NODE_H__
#define __PYENGINE_2_0_CAMERA_SCENE_NODE_H__

// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes
#include <assert.h>
#include <cmath>
#include <vector>
#include <iostream>

// Inter-Engine includes
#include "PrimeEngine/Render/IRenderer.h"
#include "PrimeEngine/MemoryManagement/Handle.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"
#include "../Events/Component.h"
#include "../Utils/Array/Array.h"
#include "PrimeEngine/Math/CameraOps.h"

#include "SceneNode.h"


// Sibling/Children includes

namespace PE {

	namespace Components {

		struct CameraSceneNode : public SceneNode
		{

			enum FRUSTUM_PLANES {
				TOP_PLANE = 0,
				BOTTOM_PLANE = 1,
				RIGHT_PLANE = 2,
				LEFT_PLANE = 3,
				NEAR_PLANE = 4,
				FAR_PLANE = 5,
				NUM_PLANES = 6
			};

			PE_DECLARE_CLASS(CameraSceneNode);

			// Constructor -------------------------------------------------------------
			CameraSceneNode(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself);

			virtual ~CameraSceneNode() {}

			// Component ------------------------------------------------------------
			virtual void addDefaultComponents();

			PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_CALCULATE_TRANSFORMATIONS);
			virtual void do_CALCULATE_TRANSFORMATIONS(Events::Event* pEvt);

			Vector4* getFrustumPlane(FRUSTUM_PLANES plane_id) {
				return frustum_planes[plane_id];
			}

			void MoveFrustumWithCamera(Vector3 pos_, Vector3 target_, Vector3 up_, float near_, float far_, float aspect);

			// Individual events -------------------------------------------------------

			Matrix4x4 m_worldToViewTransform; // objects in world space are multiplied by this to get them into camera's coordinate system (view space)
			Matrix4x4 m_worldToViewTransform2;
			Matrix4x4 m_worldTransform2;
			Matrix4x4 m_viewToProjectedTransform; // objects in local (view) space are multiplied by this to get them to screen space
			float m_near, m_far;

			std::vector<Vector4*> frustum_planes;

		};

	}; // namespace Components

}; // namespace PE
#endif
