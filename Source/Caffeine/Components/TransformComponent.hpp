
#ifndef CAFFEINE_COMPONENTS_TRANSFORM_INCLUDED
#define CAFFEINE_COMPONENTS_TRANSFORM_INCLUDED

#include <Caffeine/Entity/Component.hpp>
#include <glm/glm.hpp>

namespace Caffeine {
namespace Components {

/*
 
	Transform Component
	-------------------
	Handles Rotation/Position/Scale of an entity, will also give forward/up/right vectors
	in relation to its rotation.
	
	TODO: Make suitable for nested entities, currently will not take into account a parents transform.
	TODO: Unit Test this.
	TODO: QUATS!!!
	TODO: Redo this component.
 
 */

class TransformComponent : public Entity::Component
{
public:

	COMPONENT_RTTI(Transform)

	explicit					TransformComponent();
								~TransformComponent();
	
private:
	
public:

	void						loadParams(const std::string &params) override;

	#pragma mark - Directional Vectors

	// Directional unit vectors, which way the transform is pointing.
	const glm::vec3 &			getForwardVec();
	const glm::vec3 &			getUpVec();
	const glm::vec3 &			getRightVec();
	
	
	#pragma mark - Rotation

	// Getters/Setters for the rotation of the object.
	void						addDeltaRotation(const float pitch, const float yaw, const float roll);
	void						setRotation(const glm::vec3 & rot);
	inline const glm::vec3		getRotation() const { return m_rotation; }
	
	
	#pragma mark - Position
	
	// Getters/Setters for the position of the object.
	void						addDeltaPosition(const float x, const float y, const float z);
	void						setPosition(const glm::vec3 & pos);
	inline const glm::vec3		getPosition() const { return m_position; }
	
	
	#pragma mark - Scale
	
	// Getters/Setters for the scale of the object.
	void						addDeltaScale(const float x, const float y, const float z);
	void						setScale(const glm::vec3 & scale);
	inline const glm::vec3		getScale() const { return m_scale; }
	
	
	#pragma mark - World Matrix
	
	// The world matrix for the entity.
	const glm::mat4 			getWorldMatrix();
	void						setWorldMatrix(const glm::mat4 & setWorld);
	
private:

	bool						m_dirty; // World matrix isn't built unless somebody wants it.
	
	glm::vec3					m_forward;
	glm::vec3					m_up;
	glm::vec3					m_right;
	
	glm::vec3					m_rotation;
	glm::vec3					m_position;
	glm::vec3					m_scale;
	
	glm::mat4					m_worldMat;

}; // class

} // namespace
} // namespace

#endif
