
/*
	CameraManager.cpp
	-----------------
	
	Project: Caffeine
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

#include <Caffeine/Systems/CameraManager.hpp>

namespace Caffeine {
namespace Systems {

CameraManager::CameraManager()
: m_currentCamera(nullptr)
{
}


void CameraManager::setCurrentCamera(Camera *camera)
{
	m_currentCamera = camera;
}

} // namespace
} // namespace