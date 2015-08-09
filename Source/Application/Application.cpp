
#include <Caffeine/Application/Input.hpp>
#include <unistd.h>
#include <Application/Application.hpp>
#include <Application/Common/AppConstants.hpp>
#include <vector>
#include <fstream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <array>

#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>
#include <Caffeine/Systems/StateSystem.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Application/Common/ApplicationEvents.hpp>
#include <Application/States/IntroState.hpp>

// Components
#include <Application/Components/LevelViewComponent.hpp>
#include <Application/Components/PlayerModelComponent.hpp>
#include <Application/Components/PlayerViewComponent.hpp>
#include <Application/Components/PlayerControllerComponent.hpp>
#include <Application/Components/ChaseCameraComponent.hpp>
#include <Application/Components/BulletModelComponent.hpp>
#include <Application/Components/BulletViewComponent.hpp>
#include <Application/Components/BulletControllerComponent.hpp>
#include <Application/Components/CarrierModelComponent.hpp>
#include <Application/Components/CarrierViewComponent.hpp>
#include <Application/Components/CarrierControllerComponent.hpp>
#include <Application/Components/CameraComponent.hpp>
#include <Application/Components/InputControllerComponent.hpp>
#include <Application/Components/EnemyFighterControllerComponent.hpp>
#include <Application/Components/EnemyFighterModelComponent.hpp>
#include <Application/Components/EnemyFighterViewComponent.hpp>

// Managers Components
#include <Application/Managers/BulletManagerComponent.hpp>
#include <Application/Managers/EnemyFighterManagerComponent.hpp>


#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Systems/Camera.hpp>
#include <Caffeine/Systems/PhysicsManager.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/DebugRenderer.hpp>
#include <Caffeine/Systems/PhysicsManager.hpp>
#include <Caffeine/Math/Math.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Entity/Entity.hpp>

using namespace std;

namespace {
	
	void OnStartHook(std::vector<CaffEnt::EntityUniquePtr> &entities)
	{
		auto it = entities.begin();
		
		while(it != entities.end())
		{
			if((*it).get())
			{
				(*it)->onStart();
				it++;
			}
			else
			{
				(*it)->onEnd();
				it = entities.erase(it);
			}
		}
	}
	
	template<typename S, typename T>
	void OnThinkHook(S &container, void (T::*hook)(const float), const float dt)
	{
		auto it  = container.begin();
		
		while(it != container.end())
		{
			if((*it).get() && (*it).get()->isActive())
			{
				((*it).get()->*hook)(dt);
				it++;
			}
			else
			{
				(*it)->onEnd();
				it = container.erase(it);
			}
		}
	}
	
	void OnRenderHook(const std::vector<CaffEnt::EntityUniquePtr> &entities)
	{
		auto it = entities.begin();
		auto end = entities.end();
		
		for(; it != end; ++it)
		{
			CaffComp::RenderableComponent *comp = (*it)->getComponent<CaffComp::RenderableComponent>();
			
			if(comp)
			{
				comp->render();
			}
		}
	}

}

namespace App {


SpitfireApp::SpitfireApp()
	: Caffeine::Application::Application("SpaceFire", 1280, 720, false)
	, m_entities()
	, m_newEntities()
{
	float gravity[] = {0, 0, 0};
	CaffServ::PhysicsManager().setGravity(&gravity[0]);
	
	// ** Events ** //
	CaffServ::EventManager().addController(this, (unsigned int)App::EventID::ENTITY_ADD_MSG);
	CaffServ::EventManager().addController(this, (unsigned int)App::EventID::ENTITY_ADD_EXISTING_MSG);
	
	// ** Entity Stuff ** //
	CaffSys::EntityFactory &entityFactory = CaffServ::EntityFactory();
	
	// Register components for entities.
	entityFactory.registerComponent("BulletManager",			new BulletManagerComponentFactory);
	entityFactory.registerComponent("EnemyFighterManager",		new EnemyFighterManagerComponentFactory);
	
	entityFactory.registerComponent("LevelView",				new LevelViewComponentFactory);
	
	entityFactory.registerComponent("PlayerModel",				new PlayerModelComponentFactory);
	entityFactory.registerComponent("PlayerView",				new PlayerViewComponentFactory);
	entityFactory.registerComponent("PlayerController",			new PlayerControllerComponentFactory);
	
	entityFactory.registerComponent("CarrierModel",				new CarrierModelComponentFactory);
	entityFactory.registerComponent("CarrierView",				new CarrierViewComponentFactory);
	entityFactory.registerComponent("CarrierController",		new CarrierControllerComponentFactory);
	
	entityFactory.registerComponent("BulletModel",				new BulletModelComponentFactory);
	entityFactory.registerComponent("BulletView",				new BulletViewComponentFactory);
	entityFactory.registerComponent("BulletController",			new BulletControllerComponentFactory);
		
	entityFactory.registerComponent("EnemyFighterModel",		new EnemyFighterModelComponentFactory);
	entityFactory.registerComponent("EnemyFighterView",			new EnemyFighterViewComponentFactory);
	entityFactory.registerComponent("EnemyFighterController",	new EnemyFighterControllerComponentFactory);
	
	entityFactory.registerComponent("ChaseCam",					new ChaseCameraComponentFactory);
	entityFactory.registerComponent("BasicFPSCam",				new CameraComponentFactory);
	entityFactory.registerComponent("InputController",			new InputControllerComponentFactory);
	
	// Create entities.
	m_entities.push_back(entityFactory.createInstance(AppConstant::entityManagers));
	m_entities.push_back(entityFactory.createInstance(AppConstant::entityAstroids));
	//m_entities.push_back(entityFactory.createInstance(AppConstant::entityPlayer));
	m_entities.push_back(entityFactory.createInstance(AppConstant::entityController));
	m_entities.push_back(entityFactory.createInstance(AppConstant::entityFreeCam));
	m_entities.push_back(entityFactory.createInstance(AppConstant::entityBritCarrier));
	
	glFinish();
	
	getInput().holdMouse(true);
	
	OnStartHook(m_entities);
}



SpitfireApp::~SpitfireApp()
{
}



void SpitfireApp::think(const float dt)
{
	// Add new Entities.
	if(!m_newEntities.empty())
	{
		OnStartHook(m_newEntities);
			
		auto it = m_newEntities.begin();
		for(; it != m_newEntities.end(); ++it)
		{
			m_entities.push_back(std::move(*it));
		}
		m_newEntities.clear();
	}
	
	// Update the world
	CaffServ::PhysicsManager().updateWorld(dt);
	
	OnThinkHook(m_entities, &CaffEnt::Entity::onEarlyThink, dt);
	OnThinkHook(m_entities, &CaffEnt::Entity::onThink,		dt);
	OnThinkHook(m_entities, &CaffEnt::Entity::onLateThink,	dt);
	
	// Error Check //
	GLenum err = glGetError();
	if(err)
	{
		printf("OpenGL Error: %d", err);
	}
	
	// For testing
	//usleep(8000);
}


void SpitfireApp::render()
{
	CaffServ::RendererManager().reset();
	CaffServ::RendererManager().setViewPort(1280, 720);
	CaffServ::RendererManager().setClearColor(0.1f, 0.15f, 0.15f);
	CaffServ::RendererManager().clearDefault(true, true);
	
	OnRenderHook(m_entities);
	
	//CaffServ::PhysicsManager().renderDebug();
	
	glFlush();
}


void SpitfireApp::eventController(const CaffSys::Event *eventData)
{
	if(IsAppEvent(eventData, EventID::ENTITY_ADD_MSG))
	{
		CaffEnt::EntityUniquePtr entity = CaffServ::EntityFactory().createInstance(eventData->left.sData);
		
		if(entity)
		{
			m_newEntities.push_back(std::move(entity));
		}
		else
		{
			const std::string type(eventData->left.sData);
			DEBUG_LOG("Unkown Entity - " + type)
		}
	}

	if(IsAppEvent(eventData, EventID::ENTITY_ADD_EXISTING_MSG))
	{
		AddEntityEventData *data = const_cast<AddEntityEventData*>(static_cast<const AddEntityEventData*>(eventData));
		
		m_newEntities.push_back(std::move(data->entity));
	}
}


} // namespace