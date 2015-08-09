
#ifndef CAFFEINE_SYSTEM_EVENT_INCLUDED
#define CAFFEINE_SYSTEM_EVENT_INCLUDED

#include <Caffeine/Platform.hpp>
#include <string>
#include <glm/glm.hpp>


namespace Caffeine {

/*
	System Wide Events
	------------------
	These events are todo with the Caffeine layer only.
	
	TODO: Currently Event Manager takes uints so these need to be cast.
 */
enum class EventID : unsigned int
{
	START_UP_MSG,
	INPUT_EVENT_MSG,
	STATE_CHANGE_MSG,
	NEW_ENTITY_MSG,
	
	SIZE,
};

} // namespace

#include <array>
namespace Caffeine {
namespace Systems {

/*
	GenericData
	-----------
	Most events will pass simple data, this should cut down
	on doing lots of static casts.
	
	However if you need to pass other data you can inherit from this.
 */
struct GenericData
{
	union
	{
		float				 fData;
		unsigned int		 uiData;
		int					 iData;
		const char *				 sData;
		void *				 pData;
		std::array<float, 4> vData;
	};
};


/*
	Event
	-----
	Base event class, holds two generic datas, left and right.
 */
struct Event
{
	unsigned int id;
	
	GenericData left;
	GenericData right;
};


// Event Helpers
inline Event SetupInputEvent(unsigned int key, unsigned int state)
{
	Systems::Event evt;
	memset(&evt, 0, sizeof(evt));
	
	evt.id = (unsigned int)EventID::INPUT_EVENT_MSG;
	evt.left.uiData = key;
	evt.right.uiData = state;
	
	return evt;
}


/*
	Eventable
	---------
	Use multiple inheritanace to make your class eventable
	
	TODO: Perhaps make this a functor, so this doesn't break inheritance is-a, and also so not mulit-inheriting everywhere.
 */
class Eventable
{
public:

	explicit	Eventable() {}
				~Eventable() {}
	
	// TODO: Rename onEvent(...);
	virtual void eventController(const Event* eventData) = 0;
};


} // namespace
} // namespace

#endif // include guard