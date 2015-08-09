
#ifndef APPLICATION_EVENT_INCLUDED
#define APPLICATION_EVENT_INCLUDED

#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/Event.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstring> // memcpy


namespace App {


/*
	Application
	-----------
	Event ID's for the Application layer.
 */
enum class EventID : unsigned int
{
	GAME_STARTED_MSG = (unsigned int)Caffeine::EventID::SIZE,
	
	// Input
	INPUT_SET_KEY_MSG,					// Setting a key message.
	
	INPUT_PLANE_PITCH_MSG,				// General plane controls.
	INPUT_PLANE_ROLL_MSG,
	INPUT_PLANE_YAW_MSG,
	INPUT_PLANE_SHOOT_MSG,
	INPUT_PLANE_BOOST_MSG,
	INPUT_PLANE_SLOW_MSG,
	INPUT_PLANE_SUSPENDED_MSG,			// Suspend hardware input.
	INPUT_PLANE_RESUME_MSG,				// Resume hardware input.
	
	INPUT_UP_MSG,						// General menu navigation controls.
	INPUT_DOWN_MSG,
	INPUT_LEFT_MSG,
	INPUT_RIGHT_MSG,
	INPUT_ENTER_MSG,
	
	// Player Msgs
	PLAYER_SPAWNED_MSG,					// When the players aircraft has respawned.
	PLAYER_TELEPORT_MSG,				// Teleport the player entity to a position.
	PLAYER_MOVED_MSG,					// When the player has moved.
	PLAYER_DIED_MSG,					// When the player has died.
	PLAYER_SHOOT_MSG,					// When the player shoots guns.
	
	// Enemy Plane Msgs
	ENEMY_FIGHTER_DESTROYED_MSG,
	ENEMY_FIGHTER_DAMAGED_MSG,
	
	// General Plane Msgs
	PLANE_SHOOT_MSG,
	PLANE_DAMAGED_MSG,
	PLANE_DESTROYED_MSG,
		
	// Camera Msgs
	CAMERA_SELECT_MSG,					// Changing the active camera.
	
	// These prolly gonna get moved to Caffeine.
	ENTITY_ADD_MSG,						// Add an entity to the scene with a string id.
	ENTITY_ADD_EXISTING_MSG,			// Add a pre-existing entity to the scene.
	ENTITY_REMOVE_MSG,					// Request an entity to be removed with a string id.
	
	SIZE,								// Number of events (including Caffine's).
};


// Helpers to ID events
inline bool IsAppEvent(const CaffSys::Event *data, const EventID id)
{
	return data->id == (unsigned int)id;
}

inline bool IsCaffEvent(const CaffSys::Event *data, const Caffeine::EventID id)
{
	return data->id == (unsigned int)id;
}


/*
	Input Event
	
	Sent by the input controller when an input event corrisponding to the
	virtual controller has been fired.
	
	The presence of the event indicates which event was fired.
	optionally left.fData provides the axis information if it was mouse/gamepad movement.
 */
inline CaffSys::Event SetupInputEvent(const EventID id, float axis)
{
	CaffSys::Event evt;
	memset(&evt, 0, sizeof(evt));
	
	evt.id = (unsigned int)id;
	
	evt.left.fData = axis;
	
	return evt;
}

/*
	Player Move Event
	
	Did the player move the plane?
	
	left.vData current position.
	right.vData current direction.
 */
inline CaffSys::Event SetupPlayerMoveEvent(const glm::vec3 &pos, const glm::vec3 &dir)
{
	CaffSys::Event evt;
	memset(&evt, 0, sizeof(evt));
	
	evt.id = (unsigned int)EventID::PLAYER_MOVED_MSG;
	
	// Copy vector data
	memcpy(&evt.left.vData[0], &pos[0], sizeof(float) * 3);
	memcpy(&evt.right.vData[0], &dir[0], sizeof(float) * 3);
	
	return evt;
}

/*
	Player Teleport Event
	
	teleport the player to a new position
	
	left.vData new position.
	right.vData new rotation.
 */
inline CaffSys::Event SetupPlayerTeleport(const glm::vec3 &pos, const glm::vec3 &dir)
{
	CaffSys::Event evt;
	memset(&evt, 0, sizeof(evt));
	
	evt.id = (unsigned int)EventID::PLAYER_TELEPORT_MSG;
	
	// Copy vector data
	memcpy(&evt.left.vData[0], &pos[0], sizeof(float) * 3);
	memcpy(&evt.right.vData[0], &dir[0], sizeof(float) * 3);
	
	return evt;
}

/*
	Bullet Request
	
	Sends the information required to generate a bullet.
	
	left.vData origin of bullet.
	right.vData direction of bullet.
 */
inline CaffSys::Event SetupBullet(const glm::vec3 &start, const glm::vec3 &dir)
{
	CaffSys::Event evt;
	memset(&evt, 0, sizeof(evt));
	
	evt.id = (unsigned int)EventID::PLANE_SHOOT_MSG;
	
	memcpy(&evt.left.vData[0], &start[0], sizeof(float) * 3);
	memcpy(&evt.right.vData[0], &dir[0], sizeof(float) * 3);
	
	return evt;
}


/*
	Camera Change
		
	Requests a different camera position.
	
	left.sData is the requested cameras name.
*/
inline CaffSys::Event SetupCameraCycle(const std::string &str)
{
	CaffSys::Event evt;
	memset(&evt, 0, sizeof(evt));
	
	evt.id = (unsigned int)EventID::CAMERA_SELECT_MSG;
	
	evt.left.sData = str.c_str();
	
	return evt;
}


/*
	Plane Damage
	
	A plane got damaged by fire.
	
	left.vData position of damage.
	right.pData entity that got damaged.
 */
inline CaffSys::Event SetupPlaneDamage(const glm::vec3 &pos, const CaffEnt::Entity *entity)
{
	CaffSys::Event evt;
	memset(&evt, 0, sizeof(evt));
	
	evt.id = (unsigned int)EventID::PLANE_DAMAGED_MSG;
	
	memcpy(&evt.left.vData[0], &pos[0], sizeof(float) * 3);
	evt.right.pData = (void*)entity;
	
	return evt;
}


/*
	Plane Destroyed
	
	A plane got destroyed
	
	left.vData position
	right.pData entity;
 */
inline CaffSys::Event SetupPlaneDestroyed(const glm::vec3 &pos, const CaffEnt::Entity *entity)
{
	CaffSys::Event evt;
	memset(&evt, 0, sizeof(evt));
	
	evt.id = (unsigned int)EventID::PLANE_DESTROYED_MSG;
	
	memcpy(&evt.left.vData[0], &pos[0], sizeof(float) * 3);
	evt.right.pData = (void*)entity;
	
	return evt;
}


/*
	Enemy Fighter Destroyed
	
	Same as plane damage with different ID.
 */
inline CaffSys::Event SetupEnemyFighterDamaged(const glm::vec3 &pos, const CaffEnt::Entity *entity)
{
	CaffSys::Event evt;
	memset(&evt, 0, sizeof(evt));
	
	evt.id = (unsigned int)EventID::ENEMY_FIGHTER_DAMAGED_MSG;
	
	memcpy(&evt.left.vData[0], &pos[0], sizeof(float) * 3);
	evt.right.pData = (void*)entity;
	
	return evt;
}


/*
	Enemy Fighter Destroyed
	
	Same as plane destroyed with different ID.
 */
inline CaffSys::Event SetupEnemyFighterDestroyed(const glm::vec3 &pos, const CaffEnt::Entity *entity)
{
	CaffSys::Event evt;
	memset(&evt, 0, sizeof(evt));
	
	evt.id = (unsigned int)EventID::ENEMY_FIGHTER_DESTROYED_MSG;
	
	memcpy(&evt.left.vData[0], &pos[0], sizeof(float) * 3);
	evt.right.pData = (void*)entity;
	
	return evt;
}


/*
	Add Entity
	
	Sends a string id of the entity that is disired to be created.
	
	left.sData is the string id key.
 */
inline CaffSys::Event SetupAddNewEntity(const std::string &str)
{
	CaffSys::Event evt;
	memset(&evt, 0, sizeof(evt));
	
	evt.id = (unsigned int)EventID::ENTITY_ADD_MSG;
	
	evt.left.sData = str.c_str();
	
	return evt;
}


// Other data types.
#pragma mark - Custom Event data types

/*
	Request an entity as added to the scene.
	
	Holds unique ptr of requested entity.
	Requires controller to cast to correct data type.
 */
struct AddEntityEventData : public CaffSys::Event
{
	CaffEnt::EntityUniquePtr entity;
	
	explicit AddEntityEventData(CaffEnt::EntityUniquePtr newEntity)
	: CaffSys::Event()
	, entity(std::move(newEntity))
	{
		//entity = std::move(newEntity);
	
		memset(&left, 0, sizeof(left));
		memset(&right, 0, sizeof(right));
	
		id = (unsigned int)EventID::ENTITY_ADD_EXISTING_MSG;
	}
}; // struct


} // namespace

#endif // include guard
