
/*
	Application.hpp
	---------------
	
	Project:
	Caffeine
	
	Purpose:
	
	
*/

#ifndef CAFFEINE_APPLICATION_APPLICATION_INCLUDED
#define CAFFEINE_APPLICATION_APPLICATION_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <string>
#include <memory>

namespace Caffeine {
namespace Application {


/*
	Application Paramaters
	----------------------
	
	Settings that can be passed to the application constructor.
*/
struct AppParams
{
	uint16_t		width;
	uint16_t		height;
	bool			isFullscreen;
	std::string		appTitle;
				
}; // struct

/*
	Application
	-----------
	Main application class.
	
	TODO: This would be better off as a p_impl class.
 */


class Application : private CaffUtil::NonCopyable // final?
{
public:
    
    explicit					Application(const std::string & title, const uint16_t width, const uint16_t height, const bool isFullscreen); // TODO: Deprecated
	explicit					Application(const AppParams &params);
								~Application();
    
    virtual void				think(const float dt) = 0;  // TODO: Virtual Deprecated
    virtual void				render() = 0;				// TODO: Virtual Deprecated
	void						start();
	
	
	#pragma mark - Application Modules
	
	// TODO: Get rid of these in favour of services.
	Renderer &					getRenderer() const;
	Window &					getWindow() const;
	Input &						getInput() const;
	
	
	#pragma mark - Application Services
	
	// TODO: Get rid of these in favour of services.
	CaffSys::StateManager &		getStateManager() const;
	CaffSys::EventManager &		getEventManager() const;
	CaffSys::CameraManager &	getCameraManager() const;
	CaffSys::ModelManager &		getModelManager() const;
	CaffSys::PhysicsManager &	getPhysicsManager() const;
	CaffSys::TextureManager &	getTextureManager() const;
	CaffSys::EntityFactory &	getEntityFactory() const;
		
private:
		
	struct Impl;
	std::unique_ptr<Impl> m_pImpl;
    
}; // class



} // namespace
} // namespace

#endif // include guard