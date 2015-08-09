
#ifndef CAFFEINE_SYSTEMS_DEBUG_RENDERER_INCLUDED
#define CAFFEINE_SYSTEMS_DEBUG_RENDERER_INCLUDED

#include <Caffeine/Platform.hpp>
#include <glm/glm.hpp>

namespace Caffeine {
namespace Systems {

/*
	DebugRenderer functions.
	------------------------
	
	Helper functions for drawing in debug mode.
	These are not optimised and highly inefficiant.
	In release mode they will not render anything.
*/
namespace DebugRenderer {

// Draws a line from start to finish in a given color.
void DrawLine(const glm::vec3 & start, const glm::vec3 & end, const glm::vec3 & color);

// Draws a cube made up of lines.
void DrawCube(const glm::vec3 & center, const glm::vec3 & size, const glm::vec3 & color);

} // namespace
} // namespace
} // namespace


#endif // include guard
