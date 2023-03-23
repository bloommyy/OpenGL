#pragma once

//-------------------------------------------------------------------

#include <unordered_map>
#include <utility>

//-------------------------------------------------------------------

#include "Keys.hpp"
#include "MouseButtons.hpp"

//-------------------------------------------------------------------

namespace Input
{
	class InputManager
	{
		std::unordered_map<Keys, bool> keyMap;
		std::unordered_map<MouseButtons, bool> buttonMap;

		std::pair<float, float> cursorPosition = std::make_pair(0.0f, 0.0f);
		float scrollValue = 0;

	public:
		void PressKey(Keys key);
		void ReleaseKey(Keys key);
		void PressButton(MouseButtons button);
		void ReleaseButton(MouseButtons button);
		void Scroll(float value);

		bool IsKeyDown(Keys key);
		bool IsButtonDown(MouseButtons button);
		// Possible optims : IsKeyPressed, IsButtonPressed

		void ResetState();

		void SetCursorPosition(const float x, const float y)
		{
			cursorPosition.first = x;
			cursorPosition.second = y;
		}

		[[nodiscard]] std::pair<float, float> GetCursorPosition() const
		{
			return cursorPosition;
		}

		[[nodiscard]] float GetScrollValue() const
		{
			return scrollValue;
		}
	};
}

