
/*
	Platform.hpp
	------------
	
	Project:
	Caffeine
	
	Purpose:
	Project wide platform settings and includes.
	
*/

#ifndef CAFFEINE_UTILITIES_PLATFORM_INCLUDED
#define CAFFEINE_UTILITIES_PLATFORM_INCLUDED

// **** Types **** //

#include <cstdint>

#ifdef __APPLE__
#include <sys/stat.h>
#ifndef st_mtime
#define st_mtime st_mtimespec.tv_sec
#endif
#endif // __APPLE__


// **** SDL2 **** //

#include <SDL2/SDL.h>


// **** OpenGL **** //

// If OSX.
#ifdef __APPLE__

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>

#endif // __APPLE__


// **** Caffeine **** //
#ifdef __APPLE__

#define CAFF_ON_MAC
#define CAFF_ON_NIX

#endif


// **** Namespaces Alias's **** //

namespace Caffeine {

namespace Application	{}
namespace Services		{}
namespace Systems		{}
namespace Utilities		{}
namespace Math			{}
namespace Components	{}
namespace Entity		{}

} // namespace

namespace CaffApp  = ::Caffeine::Application;
namespace CaffServ = ::Caffeine::Services;
namespace CaffSys  = ::Caffeine::Systems;
namespace CaffUtil = ::Caffeine::Utilities;
namespace CaffMath = ::Caffeine::Math;
namespace CaffComp = ::Caffeine::Components;
namespace CaffEnt  = ::Caffeine::Entity;



#endif // include guard
