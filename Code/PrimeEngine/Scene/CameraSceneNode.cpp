#include "CameraSceneNode.h"
#include "../Lua/LuaEnvironment.h"
#include "PrimeEngine/Events/StandardEvents.h"

#define Z_ONLY_CAM_BIAS 0.0f

namespace PE {

	namespace Components {

		PE_IMPLEMENT_CLASS1(CameraSceneNode, SceneNode);

		CameraSceneNode::CameraSceneNode(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself) : SceneNode(context, arena, hMyself)
		{
			m_near = 0.05f;
			m_far = 2000.0f;

			frustum_planes.resize(NUM_PLANES);
		}
		void CameraSceneNode::addDefaultComponents()
		{
			Component::addDefaultComponents();
			PE_REGISTER_EVENT_HANDLER(Events::Event_CALCULATE_TRANSFORMATIONS, CameraSceneNode::do_CALCULATE_TRANSFORMATIONS);
		}

		void CameraSceneNode::do_CALCULATE_TRANSFORMATIONS(Events::Event* pEvt)
		{
			Handle hParentSN = getFirstParentByType<SceneNode>();
			if (hParentSN.isValid())
			{
				Matrix4x4 parentTransform = hParentSN.getObject<PE::Components::SceneNode>()->m_worldTransform;
				m_worldTransform = parentTransform * m_base;
			}

			Matrix4x4& mref_worldTransform = m_worldTransform;

			Vector3 pos = Vector3(mref_worldTransform.m[0][3], mref_worldTransform.m[1][3], mref_worldTransform.m[2][3]);
			Vector3 n = Vector3(mref_worldTransform.m[0][2], mref_worldTransform.m[1][2], mref_worldTransform.m[2][2]);
			Vector3 target = pos + n;
			Vector3 up = Vector3(mref_worldTransform.m[0][1], mref_worldTransform.m[1][1], mref_worldTransform.m[2][1]);

			m_worldToViewTransform = CameraOps::CreateViewMatrix(pos, target, up);

			m_worldTransform2 = mref_worldTransform;

			m_worldTransform2.moveForward(Z_ONLY_CAM_BIAS);

			Vector3 pos2 = Vector3(m_worldTransform2.m[0][3], m_worldTransform2.m[1][3], m_worldTransform2.m[2][3]);
			Vector3 n2 = Vector3(m_worldTransform2.m[0][2], m_worldTransform2.m[1][2], m_worldTransform2.m[2][2]);
			Vector3 target2 = pos2 + n2;
			Vector3 up2 = Vector3(m_worldTransform2.m[0][1], m_worldTransform2.m[1][1], m_worldTransform2.m[2][1]);

			m_worldToViewTransform2 = CameraOps::CreateViewMatrix(pos2, target2, up2);

			PrimitiveTypes::Float32 aspect = (PrimitiveTypes::Float32)(m_pContext->getGPUScreen()->getWidth()) / (PrimitiveTypes::Float32)(m_pContext->getGPUScreen()->getHeight());

			PrimitiveTypes::Float32 verticalFov = 0.33f * PrimitiveTypes::Constants::c_Pi_F32;
			if (aspect < 1.0f)
			{
				//ios portrait view
				static PrimitiveTypes::Float32 factor = 0.5f;
				verticalFov *= factor;
			}

			m_viewToProjectedTransform = CameraOps::CreateProjectionMatrix(verticalFov,
				aspect,
				m_near, m_far);

			SceneNode::do_CALCULATE_TRANSFORMATIONS(pEvt);
			
			MoveFrustumWithCamera(pos, target, up, m_near, m_far, verticalFov, aspect);

		}

		void CameraSceneNode::MoveFrustumWithCamera(Vector3 pos_, Vector3 target_, Vector3 up_, float near_, float far_, float fov, float aspect)
		{
			// Calculations for Frustum
			// Forward Vector - Normalized vector pointing from the camera position to the target position.
			Vector3 forward_vector = (target_ - pos_);
			forward_vector.normalize();
			// Right Vector - Normalized cross product of forward_vector and up
			Vector3 right_vector = forward_vector.crossProduct(up_);
			right_vector.normalize();
			//Normalized Up. Create copy and then normalize.
			up_ = right_vector.crossProduct(forward_vector);
			up_.normalize();

			PrimitiveTypes::Float32 verticalFov = 0.33f * PrimitiveTypes::Constants::c_Pi_F32;
			// Calculate half FOV (in radians)
			float halfFov = verticalFov / 2.0f;
			// Calculate the height of the near and far planes
			float nearHeight = 2.0f * std::tan(halfFov) * near_;
			float farHeight = 2.0f * std::tan(halfFov) * far_;
			// Calculate the width of the near and far planes based on aspect ratio (assuming a square viewport)
			float nearWidth = nearHeight * aspect;
			float farWidth = farHeight * aspect;

			// Calculate frustum points
			Vector3 near_center = pos_ + forward_vector * near_;
			Vector3 far_center = pos_ + forward_vector * far_;

			// Use near plane center, move it up or down using the near plane height/2 value and move it right or left using the near plane width/2
			Vector3 near_top_left	  = near_center + up_ * (nearHeight / 2) - right_vector * (nearWidth / 2);
			Vector3 near_top_right	  = near_center + up_ * (nearHeight / 2) + right_vector * (nearWidth / 2);
			Vector3 near_bottom_left  = near_center - up_ * (nearHeight / 2) - right_vector * (nearWidth / 2);
			Vector3 near_bottom_right = near_center - up_ * (nearHeight / 2) + right_vector * (nearWidth / 2);

			// Use far plane center, move it up or down using the far plane height/2 value and move it right or left using the far plane width/2
			Vector3 far_top_left	  = far_center + up_ * (farHeight / 2) - right_vector * (farWidth / 2);
			Vector3 far_top_right	  = far_center + up_ * (farHeight / 2) + right_vector * (farWidth / 2);
			Vector3 far_bottom_left   = far_center - up_ * (farHeight / 2) - right_vector * (farWidth / 2);
			Vector3 far_bottom_right  = far_center - up_ * (farHeight / 2) + right_vector * (farWidth / 2);

			auto calculatePlane = [](Vector3 point1, Vector3 point2, Vector3 point3) {
				Vector3 vec1 = point2 - point1;
				Vector3 vec2 = point3 - point1;

				Vector3 normal = vec1.crossProduct(vec2);
				normal.normalize();
				float d = -normal.dotProduct(point1);
				return (new Vector4(normal.m_x, normal.m_y, normal.m_z, d));
			};

			// Calculate Vector4 for each plane
			frustum_planes[LEFT_PLANE]	 = calculatePlane(near_bottom_left, far_bottom_left, far_top_left);
			frustum_planes[RIGHT_PLANE]  = calculatePlane(near_top_right, far_top_right, near_bottom_right);
			frustum_planes[TOP_PLANE]	 = calculatePlane(near_top_left, far_top_left, far_top_right);
			frustum_planes[BOTTOM_PLANE] = calculatePlane(near_bottom_right, far_bottom_right, far_bottom_left);
			frustum_planes[NEAR_PLANE]	 = calculatePlane(near_bottom_right, near_bottom_right, near_bottom_left);
			frustum_planes[FAR_PLANE]	 = calculatePlane(far_bottom_right, far_top_right, far_top_left);
			
		}

	}; // namespace Components

}; // namespace PE
