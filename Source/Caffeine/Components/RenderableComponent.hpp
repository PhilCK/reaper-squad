
/*
	Renderable Component
	--------------------
	
	Project:
	Caffeine
	
	Purpose:
	This currently only provides a virtual render function, however in time rendering should
	become more abstracted into here.
 */

#ifndef CAFFEINE_COMPONENTS_RENDERABLE_INCLUDED
#define CAFFEINE_COMPONENTS_RENDERABLE_INCLUDED

#include <Caffeine/Entity/Component.hpp>

namespace Caffeine {
namespace Components {


class RenderableComponent : public Entity::Component
{
public:

	COMPONENT_RTTI(Renderable)

	explicit				RenderableComponent();
							~RenderableComponent();
				
	virtual void			render() {}
	
}; // class


} // namespace
} // namespace

#endif // include guard
