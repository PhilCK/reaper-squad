
#ifndef APPLICATION_INTRO_STATE_INCLUDED
#define APPLICATION_INTRO_STATE_INCLUDED

#include <Caffeine/Systems/State.hpp>

namespace Application {

class IntroState : public Caffeine::Systems::State
{
public:

	explicit				IntroState();
							~IntroState();
				
	void					onPushed();
	void					onPopped();
	
	void					think(const float dt);

};

} // namespace

#endif // include guard
