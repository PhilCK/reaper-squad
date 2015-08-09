
#ifndef APPLICATION_GAME_STATE_INCLUDED
#define APPLICATION_GAME_STATE_INCLUDED

#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Entity/EntityFwd.hpp>

namespace Application {

class GameState : public Caffeine::Systems::State
{
public:

	explicit				GameState();
							~GameState();
				
	void					onPushed();
	void					onPopped();
	
	void					think(const float dt);
	
private:

	// Game Entities
	Caffeine::Entity::SimpleEntity	*m_player;
	Caffeine::Entity::SimpleEntity	*m_cam;
	Caffeine::Entity::SimpleEntity	*m_level;

};

} // namespace

#endif // include guard
