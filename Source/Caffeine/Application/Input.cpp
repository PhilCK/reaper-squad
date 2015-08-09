
/*
	Application.hpp
	---------------
	
	Project: Caffeine
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

#include <Caffeine/Application/Input.hpp>

namespace Caffeine {
namespace Application {

#pragma mark - Input

Input::Input()
: m_keystates()
, m_frameKeystates()
, m_axis()
, m_xDelta(0.f)
, m_yDelta(0.f)
{
}


Input::~Input()
{
}


#pragma mark - Mouse Control

uint16_t Input::getMouseX() const
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	
	return x;
}


uint16_t Input::getMouseY() const
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	
	return y;
}


void Input::holdMouse(const bool isHeld) {
	SDL_SetRelativeMouseMode(isHeld ? SDL_TRUE : SDL_FALSE);
}


void Input::updateMouseDelta(const float deltaX, const float deltaY)
{
	m_xDelta = deltaX;
	m_yDelta = deltaY;
	
	m_axis[KeyID::MS_XAXIS] = deltaX;
	m_axis[KeyID::MS_YAXIS] = deltaY;
	
	m_keystates[KeyID::MS_XAXIS] = deltaX != 0 ? KeyState::DOWN : KeyState::UP;
	m_keystates[KeyID::MS_YAXIS] = deltaY != 0 ? KeyState::DOWN : KeyState::UP;
}


#pragma mark - Key Control

bool Input::isDown(const KeyID keyID) const
{
	auto it = m_keystates.find(keyID);
	
	if(it != m_keystates.end())
	{
		if(it->second == KeyState::DOWN) {
			return true;
		}
	}
	
	return false;
}


bool Input::isUp(const KeyID keyID) const {
	return !isDown(keyID);
}


float Input::getAxis(const KeyID keyID) const
{
	if((unsigned int)keyID >= (unsigned int)KeyID::MS_XAXIS)
	{
		return m_axis.count(keyID) ? m_axis.at(keyID) : 0.f;
	}
	
	else
	{
		return m_keystates.at(keyID) == KeyState::DOWN ? 1.f : 0.f;
	}
}


bool Input::isDownOnFrame(const KeyID keyID) const
{
	auto it = m_frameKeystates.find(keyID);
	
	if(it != m_frameKeystates.end())
	{
		if(it->second == KeyState::DOWN) {
			return true;
		}
	}
	
	return false;
}


bool Input::isUpOnFrame(const KeyID keyID) const
{
	return !isDownOnFrame(keyID);
}


void Input::updateKeyState(const KeyID keyID, const KeyState state)
{
	m_keystates[keyID] = state;
	m_frameKeystates[keyID] = state;
}


void Input::flushFrameData()
{
	m_frameKeystates.clear();
}


} // namespace
} // namespace
