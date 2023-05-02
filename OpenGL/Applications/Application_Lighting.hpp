#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "IApplication.hpp"
#include "../Graphics/ShaderProgram.hpp"
#include "../Graphics/VertexArray.hpp"
#include "../Utils/Camera3D.hpp"
#include "../Utils/Window.hpp"

namespace Applications
{
	class Application_Lighting : public IApplication
	{
		glm::vec3 lightPos;
		unsigned boxDiffuseMap;
		unsigned boxSpecularMap;
		unsigned boxEmissionMap;

		protected:
			Input::InputManager inputManager;

			std::unique_ptr<Utils::Window> window;
			std::unique_ptr<Utils::Camera3D> camera;
			std::unique_ptr<Graphics::ShaderProgram> objectShader;
			std::unique_ptr<Graphics::ShaderProgram> lightShader;
			std::unique_ptr<Graphics::VertexArray> objectVa;
			std::unique_ptr<Graphics::VertexArray> lightVa;

			void Initialize();
			void LoadContent();
			void UnloadContent();
			void Update(float deltaTime);
			void Render() const;
		public:
			Application_Lighting();

			void Run();

		Input::InputManager& GetInputManager() override
		{
			return inputManager;
		}
	};
}
