
/*
	CameraComponent.hpp
	-------------------
	
	Project: Reaper Squad

*/

#ifndef APPLICATION_CHASE_CAMERA_COMPONENT_INCLUDED
#define APPLICATION_CHASE_CAMERA_COMPONENT_INCLUDED

#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <Caffeine/Systems/Camera.hpp>
#include <glm/glm.hpp>

namespace App {

// TODO: Offset x component does nothing

class ChaseCameraComponent : public Caffeine::Entity::Component
{
public:

	COMPONENT_RTTI(Camera)

	explicit			ChaseCameraComponent();
						~ChaseCameraComponent();
	
	void				onAttach() override;
	void				onDisattach() override;
	void				onLateThink(const float dt) override;
	
	// The position of what the camera is following, and the direction the camera is facing.
	void				setTargetPosition(const float *pos);
	void				setDirection(const float *up, const float *forward);
	
	// Sets the camera settings
	void				setCameraSettings(const float *offset, const float elasticity, const float maxLength, const float maxFOV, const float minFOV);
	
	// Sets the camera as active on the camera manager.
	void				makeActive();
	
private:

	CaffSys::Camera		m_camera;
	
	glm::vec3			m_targetPos;
	glm::vec3			m_currentOffset;
	glm::vec3			m_offset;
	float				m_maxLength;
	
	glm::vec3			m_up;
	glm::vec3			m_forward;
	
	float				m_elasticity;
	float				m_minFOV, m_maxFOV;

}; // class


COMPONENT_FACTORY_INTERFACE(ChaseCameraComponent)
} // namespace

#endif // include guard
