#pragma once

//-------------------------------------------------------------------

#include <memory>

//-------------------------------------------------------------------

#include "IApplication.hpp"
#include "../Utils/Window.hpp"
#include "../Graphics/ShaderProgram.hpp"
#include "../Graphics/VertexArray.hpp"

//-------------------------------------------------------------------

namespace Applications
{
	class Application_GettingStarted : public IApplication
	{
		unsigned int boxTexture;
		unsigned int faceTexture;

	protected:
		Input::InputManager inputManager;

		std::unique_ptr<Utils::Window> window;
		std::unique_ptr<Graphics::ShaderProgram> shader;
		std::unique_ptr<Graphics::VertexArray> va;

		void Initialize();
		void LoadContent();
		void UnloadContent();
		void Update();
		void Render() const;

	public:
		Application_GettingStarted();

		void Run();

		Input::InputManager& GetInputManager() override
		{
			return inputManager;
		}
	};

}

