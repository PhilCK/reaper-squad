
#ifndef CAFFEINE_SYSTEMS_PROPERTY_MANAGER_INCLUDED
#define CAFFEINE_SYSTEMS_PROPERTY_MANAGER_INCLUDED

#include <Application/Application.hpp>

namespace Caffeine {
namespace Systems {

class PropertyManager
{
public:

	explicit					PropertyManager(const std::string & defaultDirectory);
	virtual						~PropertyManager();
	
	// Application Level //
	void*						getAppParams() const;
	void						setAppParams();
	
	void*						getAudioParams() const;
	void						setAudioParams();
	
	void*						getRendererParams() const;
	void						setRendererParams();
	
	// Systems  Level //
	void*						getEventManagerParams() const;
	void						setEventManagerParams();
	
	void*						getEntityManagerParams() const;
	void						setEntityManagerParams();
	
	void*						getModelManagerParams() const;
	void						setModelManagerParams();
	
	void*						getShaderManagerParams() const;
	void						setShaderManagerParams();
	
	void*						getLoggingManagerParams() const;
	void						setLoggingManagerParams();
	
	void*						getTextureManagerParams() const;
	void						setTextureManagerParams();

}; // class

} // namespace
} // namespace

#endif // include guard

propMgr.getAppParams();