
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Math/Math.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <Caffeine/Utilities/XMLHelpers.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>
#include <map>
#include <array>

namespace {

	// Default vectors
	// TODO: Find a better home for them.
	const glm::vec3 xAxis(1.f, 0.f, 0.f);
	const glm::vec3 yAxis(0.f, 1.f, 0.f);
	const glm::vec3 zAxis(0.f, 0.f, 1.f);
	
	// Param processors
	void ParamPosition(glm::vec3 &pos, const std::vector<std::string> &tokens)
	{
		switch(tokens.size())
		{
			case 3:
				pos = CaffUtil::MakeVec3(tokens.at(0), tokens.at(1), tokens.at(2));
			break;
			
			case 1:
				pos = CaffUtil::MakeVec3(tokens.at(0), tokens.at(0), tokens.at(0));
			break;
			
			case 2:
				pos = CaffUtil::MakeVec3(tokens.at(0), tokens.at(1), "0");
			break;
		}
	}
	
	void ParamRotation(glm::vec3 &rot, const std::vector<std::string> &tokens)
	{
		// We can use this as its the same functionality
		ParamPosition(rot, tokens);
	}
	
	void ParamScale(glm::vec3 &scale, const std::vector<std::string> & tokens)
	{
		// We can use this as its the same functionality
		ParamPosition(scale, tokens);
	}
	
}


namespace Caffeine {
namespace Components {


TransformComponent::TransformComponent()
: Entity::Component()
, m_dirty(false)
, m_forward(zAxis)
, m_up(yAxis)
, m_right(xAxis)
, m_rotation(0,0,0)
, m_position(0,0,0)
, m_scale(1,1,1)
, m_worldMat()
{
	// Build the matrix and direction vectors.
	getWorldMatrix();
}


TransformComponent::~TransformComponent()
{
}


void TransformComponent::loadParams(const std::string &params)
{
	tinyxml2::XMLDocument doc;
	doc.Parse(params.c_str());
	
	tinyxml2::XMLElement *root = doc.FirstChildElement("params");
	
	// Position
	{
		std::string tokenStr;
		CaffUtil::XMLGetTokens(tokenStr, root, "param", "position");
		
		if(tokenStr != "")
		{
			std::vector<std::string> tokens = CaffUtil::SplitStringBySpaces(tokenStr);
			ParamPosition(m_position, tokens);
		}
	}
	
	// Rotation
	{
		std::string tokenStr;
		CaffUtil::XMLGetTokens(tokenStr, root, "param", "rotation");
		
		if(tokenStr != "")
		{
			std::vector<std::string> tokens = CaffUtil::SplitStringBySpaces(tokenStr);
			ParamRotation(m_rotation, tokens);
		}
	}
	
	// Scale
	{
		std::string tokenStr;
		CaffUtil::XMLGetTokens(tokenStr, root, "param", "scale");
		
		if(tokenStr != "")
		{
			std::vector<std::string> tokens = CaffUtil::SplitStringBySpaces(tokenStr);
			ParamScale(m_scale, tokens);
		}
	}
}


#pragma mark - Directional Vectors

const glm::vec3 & TransformComponent::getForwardVec()
{
	if(m_dirty) {
		getWorldMatrix();
	}
	
	return m_forward;
}


const glm::vec3 & TransformComponent::getUpVec()
{
	if(m_dirty) {
		getWorldMatrix();
	}
	
	return m_up;
}


const glm::vec3 & TransformComponent::getRightVec()
{
	if(m_dirty) {
		getWorldMatrix();
	}
	
	return m_right;
}


#pragma mark - Rotation

void TransformComponent::addDeltaRotation(const float pitch, const float yaw, const float roll)
{
	m_dirty = true;
	
	m_rotation.x += pitch;
	m_rotation.y += yaw;
	m_rotation.z += roll;
}


void TransformComponent::setRotation(const glm::vec3 & rot)
{
	m_dirty = true;

	m_rotation.x = rot.x;
	m_rotation.y = rot.y;
	m_rotation.z = rot.z;
}


#pragma mark - Position

void TransformComponent::addDeltaPosition(const float x, const float y, const float z)
{
	m_dirty = true;
	
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
}


void TransformComponent::setPosition(const glm::vec3 & pos)
{
	m_dirty = true;
	
	m_position.x = pos.x;
	m_position.y = pos.y;
	m_position.z = pos.z;
}


#pragma mark - Scale

void TransformComponent::addDeltaScale(const float x, const float y, const float z)
{
	m_dirty = true;
	
	m_scale.x += x;
	m_scale.y += y;
	m_scale.z += z;
}

void TransformComponent::setScale(const glm::vec3 & scale)
{
	m_dirty = true;
	
	m_scale.x = scale.x;
	m_scale.y = scale.y;
	m_scale.z = scale.z;
}


#pragma mark - World Matrix

const glm::mat4  TransformComponent::getWorldMatrix()
{
	if(!m_dirty)
	{
		return m_worldMat;
	}
	
	else
	{
		// Apply scale
		glm::mat4 scale;
		scale = glm::scale(scale, m_scale);
	
		// Apply rotations.
		glm::mat4 rotMat;
		/*
		rotMat = glm::rotate(rotMat, m_rotation.x, xAxis);
		rotMat = glm::rotate(rotMat, m_rotation.y, yAxis);
		rotMat = glm::rotate(rotMat, m_rotation.z, zAxis);
		*/
		rotMat = glm::yawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);
		
		
		// Update direction vecs.
		glm::vec4 dirX = glm::vec4(xAxis, 1.0f) * rotMat;
		glm::vec4 dirY = glm::vec4(yAxis, 1.0f) * rotMat;
		glm::vec4 dirZ = glm::vec4(zAxis, 1.0f) * rotMat;
		
		m_forward  = glm::normalize(glm::vec3(dirZ));
		m_up	   = glm::normalize(glm::vec3(dirY));
		m_right    = glm::normalize(glm::vec3(dirX));
		
		// Apply position.
		glm::mat4 transMat;
		transMat = glm::translate(transMat, m_position);
		
		// Recalc world mat.
		m_worldMat = transMat * rotMat * scale;
		//m_worldMat = rotMat * transMat * scale;
	
		m_dirty = false;
		
		return m_worldMat;
	}
}

void TransformComponent::setWorldMatrix(const glm::mat4 & setWorld)
{
	m_worldMat = setWorld;

	// Get the updated position.
	m_position.x = m_worldMat[3][0];
	m_position.y = m_worldMat[3][1];
	m_position.z = m_worldMat[3][2];

	// Get the updated scale.
	glm::vec4 dirX = glm::vec4(xAxis, 1.0f) * m_worldMat;
	glm::vec4 dirY = glm::vec4(yAxis, 1.0f) * m_worldMat;
	glm::vec4 dirZ = glm::vec4(zAxis, 1.0f) * m_worldMat;

	m_scale.x = glm::length(glm::vec3(dirX));
	m_scale.y = glm::length(glm::vec3(dirY));
	m_scale.z = glm::length(glm::vec3(dirZ));

	// Update the direction vectors.
	m_forward  = glm::normalize(glm::vec3(dirZ));
	m_up	   = glm::normalize(glm::vec3(dirY));
	m_right    = glm::normalize(glm::vec3(dirX));

	// https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2012/07/euler-angles.pdf
	// http://nghiaho.com/?page_id=846
	// http://stackoverflow.com/questions/6504671/problem-with-euler-angles-from-yzx-rotation-matrix
	// 

	glm::mat4 & m = m_worldMat;

	m_rotation.x = Math::ATan2(-m[2][1], m[2][2]);
	float c = Math::SquareRoot((m[2][1] * m[2][1]) + (m[2][2] * m[2][2]));
	//m_rotation.y = Math::ATan2(m[2][0], c);
	m_rotation.y = -Math::ASin(m[2][0]);
	m_rotation.z = Math::ATan2(m[1][0], m[0][0]);

	/*
	m_rotation.y = Math::ATan2(m[1][0], m[0][0]);

	float c = Math::SquareRoot((m[2][1] * m[2][1]) + (m[2][2] * m[2][2]));

	m_rotation.x = Math::ATan2(-m[2][0], c);
	m_rotation.z = Math::ATan2(m[2][1], m[2][2]);

	*/

	/*
	m_rotation.x = Math::RadToDeg(m_rotation.x);
	m_rotation.y = Math::RadToDeg(m_rotation.y);
	m_rotation.z = Math::RadToDeg(m_rotation.z);
	*/

	m_dirty = false;
}


} // namespace
} // namespace
