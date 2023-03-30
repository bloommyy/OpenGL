#pragma once

//-------------------------------------------------------------------

#include <unordered_map>
#include <glm/glm.hpp>

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

		glm::vec2 cursorPosition = glm::vec2(0.0f, 0.0f);
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
			cursorPosition.x = x;
			cursorPosition.y = y;
		}

		[[nodiscard]] glm::vec2 GetCursorPosition() const
		{
			return cursorPosition;
		}

		[[nodiscard]] float GetScrollValue() const
		{
			return scrollValue;
		}
	};
}

