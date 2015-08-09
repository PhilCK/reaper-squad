
#ifndef CAFFEINE_SYSTEMS_STATE_SYSTEM_INCLUDED
#define CAFFEINE_SYSTEMS_STATE_SYSTEM_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <stack>

namespace Caffeine {
namespace Systems {


/*
	StateSystem
	-----------
	A very simple state system based off a stack system.
	
	TODO: Move away from stack based system to an array based one.
	TODO: Rename file to StateManager as it more accuretly represents its nature.
	TODO: Make push / pop private again by integrating events system.
 */

class StateManager : private CaffUtil::NonCopyable
{
public:

	explicit					StateManager();
								~StateManager();

	// Returns the active state.
	State &						topState() const;
	
	// Process pending pops / pushes, pops will be processed first.
	void						processPending();
	
	// Returns true if states exist.
	bool						hasStates() const;
				
//private:
	
	#pragma mark - Manage States
	
	// These are to be used through the event system.
	void						pushState(State *pushState);
	void						popState();
		
private:

	std::stack<State*>			m_stateStack;
	State*						m_pendingPush; // next state to be pushed on.
	bool						m_pendingPop;  // pop this state when ready.

}; // class


} // namespace
} // namespace

#endif // include guard