
/*
	Main.cpp
	--------
	
	Project: Caffeine
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

#include <Application/Application.hpp>
#include <Application/States/IntroState.hpp>
#include <Caffeine/Systems/StateSystem.hpp>
#include <Caffeine/Platform.hpp>
#include <iostream>

// TODO: Move this out to the application layer, let the app have control of the main class

int main(int argc, char * arg[])
{
	
	App::SpitfireApp *app(nullptr);

	//try {
		app = new App::SpitfireApp();
		app->getStateManager().pushState(new ::Application::IntroState());
	//}
	
	//catch(std::exception e) {
	//	std::cout << e.what() << std::endl;
	//	return 0;
	//}

	
	app->start();
	
	
    delete app;
    
    return 0;
}
