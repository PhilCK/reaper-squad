
#include <Application/Components/ChaseCameraComponent.hpp>
#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Systems/Camera.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Application/Window.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <cstring> // memcpy
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

namespace App {

ChaseCameraComponent::ChaseCameraComponent()
: m_camera(CaffServ::WindowManager().getWidth(),
           CaffServ::WindowManager().getHeight(),
		   60)
, m_targetPos(0.f, 0.f, 0.f)
, m_currentOffset(0.f,0.f,0.f)
, m_offset(0.f,0.f,0.f)
, m_maxLength(10)
, m_up(0.f,1.f,0.f)
, m_forward(0.f,0.f,1.f)
, m_elasticity(0.f)
, m_minFOV(45.f)
, m_maxFOV(90.f)
{
}


ChaseCameraComponent::~ChaseCameraComponent()
{
}


void ChaseCameraComponent::onAttach()
{
}


void ChaseCameraComponent::onDisattach()
{
}


void ChaseCameraComponent::onLateThink(const float dt)
{
	// Get Offset
	{
		glm::vec3 targetOffset = m_targetPos;
		targetOffset -= m_forward * m_offset.z;
		targetOffset -= m_up * m_offset.y;
		
		m_currentOffset  = glm::lerp(m_currentOffset, targetOffset, 0.2f);
		//m_currentOffset = targetOffset;
	}
	
	// Get FOV
	{
		
	}
	
	// Set the camera
	m_camera.lookAt(m_currentOffset, m_targetPos, m_up);
}


void ChaseCameraComponent::setTargetPosition(const float *pos)
{
	memcpy(&m_targetPos[0], pos, sizeof(float) * 3);
}


void ChaseCameraComponent::setDirection(const float *up, const float *forward)
{
	memcpy(&m_up[0],	  up,	   sizeof(float) * 3);
	memcpy(&m_forward[0], forward, sizeof(float) * 3);
}


void ChaseCameraComponent::setCameraSettings(const float *offset, const float elasticity, const float maxLength, const float maxFOV, const float minFOV)
{
	memcpy(&m_offset[0], offset, sizeof(float) * 3);
	m_elasticity = elasticity;
	m_maxLength = maxLength;
	m_minFOV = minFOV;
	m_maxFOV = maxFOV;
}


void ChaseCameraComponent::makeActive()
{
	CaffServ::CameraManager().setCurrentCamera(&m_camera);
}


COMPONENT_FACTORY_SOURCE(ChaseCameraComponent)


} // namespace
