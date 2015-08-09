
/*
	EventManager.hpp
	----------------
	
	Project: Caffeine
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

#ifndef CAFFEINE_SYSTEMS_EVENT_MANAGER_INCLUDED
#define CAFFEINE_SYSTEMS_EVENT_MANAGER_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <vector>
#include <map>

namespace Caffeine {
namespace Systems {

/*
	EventManager
	------------
	Controls the events in the game.
 */
 
class EventManager : private CaffUtil::NonCopyable
{
public:
	
	explicit				EventManager();
							~EventManager();

	// Add/Remove controllers for events.
	bool					addController(Eventable * controller, const unsigned int eventID);
	bool					removeController(const Eventable * controller, const unsigned int eventID);
	bool					removeController(const Eventable * controller);
	
	// Deleivers all the events that have been queued, this should be a per frame thing.
	void					deliverQueuedEvents();
	
	// Send events to the controllers, queued will be stored until the next frame.
	void					sendQueuedEvent(const Event * data);
	void					sendInstantEvent(const Event * data);

	inline std::size_t		numberOfQueuedEvents() const { return m_queuedEvents.size(); }

private:

	typedef std::vector<Eventable *> ControllerList;
	
	// List of controllers, and queued events.
	std::map<unsigned int, ControllerList>  m_controllers;
	std::vector<const Event* >				m_queuedEvents;

};


} // namespace
} // namespace

#endif
