
#include <Application/States/GameState.hpp>
#include <Caffeine/Entity/SimpleEntity.hpp>

#include <Application/Components/PlayerControllerComponent.hpp>
#include <Application/Components/CameraComponent.hpp>

namespace Application {

GameState::GameState()
: Caffeine::Systems::State("Game State")
{

}


GameState::~GameState()
{
	
}


void GameState::onPushed()
{
	// Camera
	m_cam = new Caffeine::Entity::SimpleEntity();

	// Build Player
	m_player = new Caffeine::Entity::SimpleEntity();
	
	// Build Level
	m_level = new Caffeine::Entity::SimpleEntity();
	
}


void GameState::onPopped()
{

}


void GameState::think(const float dt)
{

}


} // namespace