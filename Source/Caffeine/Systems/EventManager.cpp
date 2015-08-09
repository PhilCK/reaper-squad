
/*
	EventManager.cpp
	----------------
	
	Project: Caffeine
*/

#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <Caffeine/Utilities/Logging.hpp>

#define QUEUED_EVENT_STD_VEC_HINT 128
#define NUM_CONTROLLERS_STD_VEC_HINT 8

namespace Caffeine {
namespace Systems {


EventManager::EventManager()
: m_controllers()
, m_queuedEvents()
{
	m_queuedEvents.reserve(QUEUED_EVENT_STD_VEC_HINT);
}



EventManager::~EventManager()
{	
	m_controllers.clear();
	m_queuedEvents.clear();
}



bool EventManager::addController(Caffeine::Systems::Eventable *controller, const unsigned int eventID)
{
	assert(controller);

	ControllerList & controllerList = m_controllers[eventID];

	// If new list reserve.
	if(!controllerList.capacity() && controllerList.empty())
	{
		controllerList.reserve(NUM_CONTROLLERS_STD_VEC_HINT);
	}

	// Check to see if the controller has already subscribed to this event.
	for(auto it = controllerList.begin(); it != controllerList.end(); ++it)
	{
		if((*it) == controller)
		{
			// Already subscribed
			return false;
		}
	}
	
	controllerList.push_back(controller);
	
	return true;
}



bool EventManager::removeController(const Caffeine::Systems::Eventable *controller, const unsigned int eventID)
{
	assert(controller);

	ControllerList & controllerList = m_controllers[eventID];
	
	// Check to see if the controller has subscribed to this event.
	for(auto it = controllerList.begin(); it != controllerList.end(); ++it)
	{
		if((*it) == controller)
		{
			controllerList.erase(it);
			
			return true;
		}
	}

	// None found to remove.
	return false;
}



bool EventManager::removeController(const Caffeine::Systems::Eventable *controller)
{
	assert(controller);
	
	// Loop through all registered events to look for controller.
	for(auto it = m_controllers.begin(); it != m_controllers.end(); ++it)
	{
		ControllerList & controllerList = it->second;
		
		for(auto cit = controllerList.begin(); cit != controllerList.end(); ++cit)
		{
			if((*cit) == controller)
			{
				controllerList.erase(cit);
				
				return true;
			}
		}
	}

	// None found to remove.
	return false;
}


void EventManager::sendQueuedEvent(const Event *data)
{
	assert(data);

	m_queuedEvents.push_back(data);
}



void EventManager::deliverQueuedEvents()
{
	// Loop through all stored events and send them.
	for(auto it = m_queuedEvents.begin(); it != m_queuedEvents.end(); ++it)
	{
		sendInstantEvent((*it));
	}
	
	m_queuedEvents.clear();
}



void EventManager::sendInstantEvent(const Caffeine::Systems::Event * data)
{
	assert(data);

	ControllerList & controllerList = m_controllers[data->id];
	
	// Find the controller and send the msg data.
	for(auto it = controllerList.begin(); it != controllerList.end(); ++it)
	{
		(*it)->eventController(data);
	}
}


} // namespace
} // namespace
