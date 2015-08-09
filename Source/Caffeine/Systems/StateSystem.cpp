
#include <Caffeine/Systems/StateSystem.hpp>
#include <Caffeine/Systems/State.hpp>

namespace Caffeine {
namespace Systems {


StateManager::StateManager()
: m_stateStack()
, m_pendingPush(nullptr)
, m_pendingPop(false)
{
}


StateManager::~StateManager()
{
	// Delete the states left in the stack.
	for(unsigned int i(0); i < m_stateStack.size(); ++i)
	{
		State *state = m_stateStack.top();
		m_stateStack.pop();
		
		delete state;
	}
}


State & StateManager::topState() const {
	return *(m_stateStack.top());
}


bool StateManager::hasStates() const {
	return !m_stateStack.empty();
}


void StateManager::processPending()
{
	// Pop current state.
	if(m_pendingPop)
	{
		m_stateStack.top()->onDeFocus();
		m_stateStack.pop();
		
		m_pendingPop = false;
	}
	
	// Push next state.
	if(m_pendingPush)
	{
		m_stateStack.push(m_pendingPush);
		m_stateStack.top()->onFocus();
		
		m_pendingPush = nullptr;
	}
}


#pragma mark - Manage States

void StateManager::pushState(State *pushState)  { m_pendingPush = pushState; }
void StateManager::popState()					{	m_pendingPop = true; }




} // namespace
} // namespace