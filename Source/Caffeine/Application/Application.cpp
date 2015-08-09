
/*
	Application.cpp
	---------------
	
	Project: Caffeine
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

#include <Caffeine/Application/Application.hpp>
#include <Caffeine/Application/Window.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Application/Input.hpp>
#include <Caffeine/Application/Settings.hpp>

#include <Caffeine/Utilities/Timer.hpp>
#include <Caffeine/Platform.hpp>

#include <Caffeine/Systems/StateSystem.hpp>
#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/PhysicsManager.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Systems/TextureManager.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

#include <Caffeine/Services/Services.hpp>

// TODO: Make Application less dependent on Engine stuff, Application layer should be bare bones standalone framework.

namespace Caffeine {
namespace Application {


#pragma mark - pImpl

struct Application::Impl
{
	explicit Impl()
	: window(nullptr)
	, renderer(nullptr)
	, input(nullptr)
	, stateMgr(nullptr)
	, eventMgr(nullptr)
	, cameraMgr(nullptr)
	, modelMgr(nullptr)
	{
	}
	
	~Impl() {}
	
	std::unique_ptr<Window>						window;
	std::unique_ptr<Renderer>					renderer;
	std::unique_ptr<Input>						input;
	std::unique_ptr<CaffSys::StateManager>		stateMgr;
	std::unique_ptr<CaffSys::EventManager>		eventMgr;
	std::unique_ptr<CaffSys::CameraManager>		cameraMgr;
	std::unique_ptr<CaffSys::ModelManager>		modelMgr;
	std::unique_ptr<CaffSys::PhysicsManager>	physMgr;
	std::unique_ptr<CaffSys::TextureManager>	texMgr;
	std::unique_ptr<CaffSys::EntityFactory>		entFact;

	
}; // struct


#pragma mark - Application

Application::Application(const std::string & title, const uint16_t width, const uint16_t height, const bool isFullscreen)
	: m_pImpl(new Impl)
{
	// Look for Application Settings
	AppXML settings;
	
    // Init SDL.
	Uint32 flags = SDL_INIT_EVERYTHING;
    if(SDL_Init(flags) != 0) { std::runtime_error(SDL_GetError()); }
	
	// SDL Attribute Hints.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, settings.rendererDetails.multisamples);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
	
	// Create Window and Renderer.
	m_pImpl->window.reset(new Window(settings.windowDetails.title,
							  settings.windowDetails.width,
							  settings.windowDetails.height,
							  settings.windowDetails.isFullscreen));
				
	m_pImpl->renderer.reset(new Renderer(m_pImpl->window->getSDLWindow()));
	m_pImpl->input.reset(new Input());
	
	// Create the Application Services.
	m_pImpl->stateMgr.reset(new Systems::StateManager());
	m_pImpl->eventMgr.reset(new Systems::EventManager());
	m_pImpl->cameraMgr.reset(new Systems::CameraManager());
	
	m_pImpl->modelMgr.reset(new Systems::ModelManager(settings.modelDetails.modelsDir, settings.modelDetails.modelsExt, settings.modelDetails.hotLoading, getRenderer()));
	
	m_pImpl->physMgr.reset(new Systems::PhysicsManager());
	
	m_pImpl->texMgr.reset(new Systems::TextureManager(settings.textureDetails.texturesDir, settings.textureDetails.texturesExt, settings.textureDetails.hotLoading, getRenderer()));
	
	m_pImpl->entFact.reset(new Systems::EntityFactory());
	
	// Set services
	Services::SetApplication(this);
}


Application::Application(const AppParams &params)
: Application(params.appTitle, params.width, params.height, params.isFullscreen)
{
}


Application::~Application() {
	SDL_Quit();
}



void Application::start()
{
	bool isRunning = true;
	Utilities::Timer deltaTimer;
	
	deltaTimer.start();
		
	// Game Loop
	while(isRunning)
	{
		// Process SDL events.
		SDL_Event event;
		
		// Reset delta's to zero incase msg isn't called.
		m_pImpl->input->updateMouseDelta(0, 0);
		m_pImpl->input->flushFrameData();

		while(SDL_PollEvent(&event))
		{
			// QUIT?
			if(event.type == SDL_QUIT) { isRunning = false; }
			
			// TODO: Make msg system modular and move stuff like this into input class.
			if(event.type == SDL_KEYDOWN)
			{
				SDL_Keycode key = event.key.keysym.sym;
				// TODO: Make input class eventable.
				getInput().updateKeyState((Caffeine::Application::KeyID)key, KeyState::DOWN);
				
				Systems::Event evt = Systems::SetupInputEvent(key, (unsigned int)KeyState::DOWN);
				getEventManager().sendInstantEvent(&evt);
			}
			
			if(event.type == SDL_KEYUP)
			{
				SDL_Keycode key = event.key.keysym.sym;
				// TODO: Make input class eventable.
				getInput().updateKeyState((Caffeine::Application::KeyID)key, KeyState::UP);
				
				Systems::Event evt = Systems::SetupInputEvent(key, (unsigned int)KeyState::UP);
				getEventManager().sendInstantEvent(&evt);
			}
			
			if(event.type == SDL_MOUSEMOTION) {
				getInput().updateMouseDelta(event.motion.xrel, event.motion.yrel);
			}
			
			if(event.type == SDL_WINDOWEVENT)
			{
				switch(event.window.event)
				{
					case SDL_WINDOWEVENT_FOCUS_GAINED:
						getModelManager().checkForUpdates();
						getTextureManager().checkForUpdates();
						break;
				}
			}
		}
		
		const float dt = (deltaTimer.split() / 1000.0f);
		
		
		getStateManager().processPending();
		// FIXME: Don't like checking this consistantly, can we throw if no state was given for system_start_up_message?
		if(getStateManager().hasStates()) {
			getStateManager().topState().think(dt);
		}
		
		// Fire Queued events.
		getEventManager().deliverQueuedEvents();
		
	
		think(dt); // FOR THE CHOP
		
		render();
	
		SDL_GL_SwapWindow(m_pImpl->window->getSDLWindow());
	}
}



#pragma mark - Application Modules

Renderer & Application::getRenderer() const {
	return *m_pImpl->renderer;
}

Window & Application::getWindow() const {
	return *m_pImpl->window;
}

Input & Application::getInput() const {
	return *m_pImpl->input;
}


#pragma mark - Application Services

Systems::StateManager & Application::getStateManager() const {
	return *m_pImpl->stateMgr;
}


Systems::EventManager & Application::getEventManager() const {
	return *m_pImpl->eventMgr;
}


Systems::CameraManager & Application::getCameraManager() const {
	return *m_pImpl->cameraMgr;
}


Systems::ModelManager & Application::getModelManager() const {
	return *m_pImpl->modelMgr;
}

Systems::PhysicsManager & Application::getPhysicsManager() const {
	return *m_pImpl->physMgr;
}

Systems::TextureManager & Application::getTextureManager() const {
	return *m_pImpl->texMgr;
}

Systems::EntityFactory & Application::getEntityFactory() const {
	return *m_pImpl->entFact;
}

} // namespace
} // namespace

