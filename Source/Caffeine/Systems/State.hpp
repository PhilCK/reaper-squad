
#ifndef CAFFEINE_SYSTEMS_STATE_INCLUDED
#define CAFFEINE_SYSTEMS_STATE_INCLUDED

#include <string>

namespace Caffeine {
namespace Systems {

/*
	State
	-----
	Base class for game states.
	
	Inherit and override the virutal functions.
	The internal ones are mostly for Logging and adding aditional
	functionality.
 */

class State
{

public:

	explicit			State(const std::string stateName);
	virtual				~State() {}
	
	#pragma mark - Internal State Interactions
	
	void				onFocus();
	void				onDeFocus();
	
	#pragma mark - State Interactions
	
	virtual void		onPushed() {}
	virtual void		onPopped() {}
	
	virtual void		think(const float dt) {}
	
private:

	std::string			m_name;
	
}; // class


} // namespace
} // namespace

#endif // include guard
