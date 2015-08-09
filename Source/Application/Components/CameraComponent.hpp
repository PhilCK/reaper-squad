
/*
	CameraComponent.hpp
	-------------------
	
	Project: Reaper Squad
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

#ifndef APPLICATION_CAMERA_COMPONENT_INCLUDED
#define APPLICATION_CAMERA_COMPONENT_INCLUDED

#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <Caffeine/Systems/Camera.hpp>
#include <glm/glm.hpp>

namespace App {

class CameraComponent : public Caffeine::Entity::Component, public Caffeine::Systems::Eventable
{
public:

	COMPONENT_RTTI(Camera)

	explicit			CameraComponent();
						~CameraComponent();
	
	
	// private:

	void				onAttach() override;
	void				onDisattach() override;
	
	void				onEarlyThink(const float dt) override;
	
	void				eventController(const Caffeine::Systems::Event *data);
	
	
	
private:

	Caffeine::Systems::Camera m_camera;
	
	glm::vec3 m_targetPosition, m_targetDirection;
	
	bool m_isActive;
	

}; // class

COMPONENT_FACTORY_INTERFACE(CameraComponent)

} // namespace

#endif // include guard
