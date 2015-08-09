
/*
	CameraManager.hpp
	-----------------
	
	Project: Caffeine
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

#ifndef CAFFEINE_SYSTEMS_CAMERA_MANAGER_INCLUDED
#define CAFFEINE_SYSTEMS_CAMERA_MANAGER_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>

namespace Caffeine {
namespace Systems {


class CameraManager : private CaffUtil::NonCopyable
{
public:

	explicit				CameraManager();

	inline Camera &			getCurrentCamera() const { return *m_currentCamera; }
	void					setCurrentCamera(Camera * camera);
	
	
private:

	Camera *				m_currentCamera;

}; // class


} // namespace
} // namespace

#endif
