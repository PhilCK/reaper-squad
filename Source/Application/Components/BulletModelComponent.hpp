
#ifndef APPLICATION_BULLET_MODEL_COMPONENT_INCLUDED
#define APPLICATION_BULLET_MODEL_COMPONENT_INCLUDED

#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <BulletDynamics/btBulletCollisionCommon.h>
#include <glm/glm.hpp>

namespace App {

class BulletModelComponent : public CaffEnt::Component
{
public:

	COMPONENT_RTTI(BulletModel)

	explicit			BulletModelComponent();
						~BulletModelComponent();
				
	void				onAttach() override;
	void				onDisattach() override;
	void				onThink(const float dt) override;

	inline glm::vec3 	getStartPos() const			{ return m_startPos;	}
	inline glm::vec3 	getDirection() const		{ return m_direction;	}
	inline glm::vec3 	getCurrentPos() const		{ return m_currentPos;  }
	inline glm::vec3 	getCurrentColor() const		{ return m_color;		}
	inline float		getSpeed() const			{ return m_speed;		}
	inline float		getScatter() const			{ return m_scatter;		}

//private:

	// Sets the position
	void				setStart(const glm::vec3 &startPoint, const glm::vec3 &direction);
	
	// Optional stuff to fine tune behaviour
	void				setOptionalLength(const glm::vec3 &length);
	void				setOptionalColor(const glm::vec3 &color);
	void				setOptionalScatter(const float scatter);
	
	glm::vec3			m_startPos;
	glm::vec3			m_endPos;
	glm::vec3			m_currentPos;
	glm::vec3			m_direction;
	glm::vec3			m_color;
	float				m_speed;
	float				m_scatter;
	
	glm::vec3			m_lastPosition;
	
	bool				m_fired;
	float				counter;
	
	btCollisionWorld::ClosestRayResultCallback *ray;
	
}; // class

COMPONENT_FACTORY_INTERFACE(BulletModelComponent)

} // namespace

#endif // include guard
