#include "InputManager.hpp"

//-------------------------------------------------------------------

namespace Input
{
	void InputManager::PressKey(const Keys key)
	{
		keyMap[key] = true;
	}

	void InputManager::ReleaseKey(Keys key)
	{
		keyMap[key] = false;
	}

	void InputManager::PressButton(MouseButtons button)
	{
		buttonMap[button] = true;
	}

	void InputManager::ReleaseButton(MouseButtons button)
	{
		buttonMap[button] = false;
	}

	void InputManager::Scroll(float value)
	{
		scrollValue = value;
	}

	bool InputManager::IsKeyDown(Keys key)
	{
		const auto umit = keyMap.find(key);

		if (umit != keyMap.end())
			return umit->second;

		return false;
	}

	bool InputManager::IsButtonDown(MouseButtons button)
	{
		const auto umit = buttonMap.find(button);

		if (umit != buttonMap.end())
			return umit->second;

		return false;
	}

	void InputManager::ResetState()
	{
		scrollValue = 0.0f;
	}


}
