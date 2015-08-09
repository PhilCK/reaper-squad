
#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Utilities/Logging.hpp>

namespace Caffeine {
namespace Systems {

State::State(const std::string stateName)
: m_name(stateName)
{
}


void State::onFocus()
{
	std::string msg = "State: " + m_name + " pushed.";
	DEBUG_LOG(msg);
		
	onPushed();
}

void State::onDeFocus()
{
	std::string msg = "State: " + m_name + " popped.";
	DEBUG_LOG(msg);
	
	onPopped();
}


} // namespace
} // namespace