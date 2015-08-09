
#ifndef SPITFIRE_APPLICATION_INCLUDED_HPP
#define SPITFIRE_APPLICATION_INCLUDED_HPP

#include <Caffeine/Application/Application.hpp>
#include <BulletDynamics/btBulletCollisionCommon.h>
#include <Caffeine/Entity/SimpleEntity.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace App {

class SpitfireApp : public CaffApp::Application, public CaffSys::Eventable
{
public:
    
    explicit			SpitfireApp();
						~SpitfireApp();
    
    void				think(const float dt);
    void				render();
		
	void				eventController(const CaffSys::Event *eventData);
	
	
private:

	std::vector<CaffEnt::EntityUniquePtr> m_entities;
	std::vector<CaffEnt::EntityUniquePtr> m_newEntities;
	

}; // class
    
} // include

#endif // include guard