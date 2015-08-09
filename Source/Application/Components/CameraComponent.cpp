
#include <Application/Components/CameraComponent.hpp>
#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Systems/Camera.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Application/Window.hpp>
#include <Application/Common/ApplicationEvents.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <cstring> // memcpy
#include <glm/gtc/type_ptr.hpp>
#include <Caffeine/Application/Input.hpp>

namespace App {


CameraComponent::CameraComponent()
: m_camera(Caffeine::Services::WindowManager().getWidth(),
           Caffeine::Services::WindowManager().getHeight(),
		   60)
, m_targetPosition(0.0f, 0.0f, 1.0f)
, m_targetDirection(0.0f, 0.0f, -1.0f)
, m_isActive(false)
{
}


CameraComponent::~CameraComponent()
{
}


void CameraComponent::onAttach()
{
	CaffServ::EventManager().addController(this, (unsigned int)App::EventID::CAMERA_SELECT_MSG);
	
	const glm::vec3 pos(0, 1, 0);
	const glm::vec3 rot(0, 0, 0);
	
	m_camera.updateAbsolute(&pos[0], &rot[0]);
	
	CaffServ::CameraManager().setCurrentCamera(&m_camera);
}


void CameraComponent::onDisattach()
{
	CaffServ::EventManager().removeController(this, (unsigned int)App::EventID::CAMERA_SELECT_MSG);
}


void CameraComponent::onEarlyThink(const float dt)
{
	if(m_isActive)
	{
		const CaffApp::Input &input = CaffServ::InputManager();

		glm::vec3 rot(0,0,0);
		glm::vec3 pos(0,0,0);
		
		rot.x = -(input.getMouseDeltaY() * dt) * 0.25f;
		rot.y = +(input.getMouseDeltaX() * dt) * 0.25f;
		
		
		if(input.isDown(CaffApp::KeyID::KB_W)) {
			pos.z += +(10.0f * dt);
		}
		
		if(input.isDown(CaffApp::KeyID::KB_S)) {
			pos.z += -(10.0f * dt);
		}
		
		if(input.isDown(CaffApp::KeyID::KB_D)) {
			pos.x += -(10.0f * dt);
		}
		
		if(input.isDown(CaffApp::KeyID::KB_A)) {
			pos.x += +(10.0f * dt);
		}
		
		m_camera.updateDelta(&pos[0], &rot[0]);
	}
}


void CameraComponent::eventController(const Caffeine::Systems::Event *eventData)
{
	if(eventData->id == (unsigned int)App::EventID::CAMERA_SELECT_MSG)
	{
		if(!strcmp(eventData->left.sData, "FreeCam"))
		{
			CaffServ::CameraManager().setCurrentCamera(&m_camera);
			m_isActive = true;
		}
		else
		{
			m_isActive = false;
		}
	}
}

COMPONENT_FACTORY_SOURCE(CameraComponent)

} // namespace