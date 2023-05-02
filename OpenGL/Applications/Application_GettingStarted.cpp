#include "Application_GettingStarted.hpp"

//-------------------------------------------------------------------

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "wtypes.h"
using namespace std;

//-------------------------------------------------------------------

namespace Applications
{
	Application_GettingStarted::Application_GettingStarted()
		: boxTexture(0), faceTexture(0)
	{
		RECT desktop;
		// Get a handle to the desktop window
		const auto hDesktop = GetDesktopWindow();
		// Get the size of screen to the variable desktop
		GetWindowRect(hDesktop, &desktop);
		// The top left corner will have coordinates (0,0)
		// and the bottom right corner will have coordinates
		// (horizontal, vertical)
		auto horizontal = desktop.right;
		auto vertical = desktop.bottom;

		window = std::make_unique<Utils::Window>("LearnOpenGL", horizontal, vertical);
	}

	//-------------------------------------------------------------------

	void Application_GettingStarted::Run()
	{
		Initialize();
		LoadContent();

		auto lastFrame = 0.0f; // Time of last frame

		// Render loop, each iteration is a frame
		while (!window->GetShouldClose())
		{
			const auto currentFrame = window->GetElapsedTime();
			const auto deltaTime = currentFrame - lastFrame; // Time between frames
			lastFrame = currentFrame;

			Update(deltaTime);
			Render();

			window->SwapBuffers();
			window->PollEvents();
		}

		UnloadContent();
	}

	//-------------------------------------------------------------------

	void Application_GettingStarted::Initialize()
	{
		window->ActivateInputFor(this);
		camera = std::make_unique<Utils::Camera3D>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);

		stbi_set_flip_vertically_on_load(true);

		glEnable(GL_DEPTH_TEST);
	}

	//-------------------------------------------------------------------

	void Application_GettingStarted::LoadContent()
	{
		//
		// Textures
		//

		glGenTextures(1, &boxTexture);
		glBindTexture(GL_TEXTURE_2D, boxTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, channels;
		std::string texturePath = "Content/Textures/container.jpg";

		auto data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			std::string errorMessage = "Failed to load texture: ";
			errorMessage.append(texturePath);

			throw std::exception(errorMessage.c_str());
		}

		stbi_image_free(data);

		glGenTextures(1, &faceTexture);
		glBindTexture(GL_TEXTURE_2D, faceTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		texturePath = "Content/Textures/awesomeface.png";

		auto facedata = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);

		if (facedata)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, facedata);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			std::string errorMessage = "Failed to load texture: ";
			errorMessage.append(texturePath);

			throw std::exception(errorMessage.c_str());
		}

		stbi_image_free(facedata);

		//
		// Buffers
		//

		//Square
		//float vertices[] =
		//{
		//	// positions		//colors			// texture coords
		//	0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // top right
		//	0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // bottom right
		//	-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// bottom left
		//	-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f
		//};

		//Cube
		constexpr float vertices[] = 
		{
			//positions			  // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		//unsigned int indices[] = {  // note that we start from 0!
		//	0, 1, 3,   // first triangle
		//	1, 2, 3    // second triangle
		//};

		va = std::make_unique<Graphics::VertexArray>();
		auto vb = std::make_unique<Graphics::VertexBuffer>(vertices, sizeof(vertices));

		vb->SetAttributes({
			{"aPos", Graphics::VertexAttributeType::VEC3F},
			{"aTexCoord", Graphics::VertexAttributeType::VEC2F},
		});

		//auto eb = std::make_unique<Graphics::ElementBuffer>(indices, 6);

		va->SetVertexBuffer(std::move(vb));
		//va->SetElementBuffer(std::move(eb));

		//
		// Shaders
		//

		shader = std::make_unique<Graphics::ShaderProgram>("Content/Shaders/getting_started.vert", "Content/Shaders/getting_started.frag");

		shader->Use();

		shader->SetInt("boxTexture", 0);
		shader->SetInt("faceTexture", 1);

		shader->Unuse();
	}

	//-------------------------------------------------------------------

	void Application_GettingStarted::UnloadContent()
	{
		glDeleteTextures(1, &boxTexture);
		glDeleteTextures(1, &faceTexture);

		va = nullptr;
		shader = nullptr;
		window = nullptr;
	}

	//-------------------------------------------------------------------

	void Application_GettingStarted::Update(float deltaTime)
	{
		if (inputManager.IsKeyDown(Input::Keys::ESCAPE))
			window->SetShouldClose(true);

		//const auto timeValue = window->GetElapsedTime();
		//const auto greenValue = sin(timeValue) / 2.0f + 0.5f;
		//const auto redValue = sin(timeValue) / 2.0f + 2.0f;
		//const auto vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//if (vertexColorLocation != -1)
		//{
		//	glUseProgram(shaderProgram);
		//	glUniform4f(vertexColorLocation, /*redValue*/0.0f, greenValue, 0.0f, 1.0f);
		//	glUseProgram(0);
		//}

		camera->Update(deltaTime, inputManager);

		inputManager.ResetState();
	}

	//-------------------------------------------------------------------

	void Application_GettingStarted::Render() const
	{
		glClearColor(0.393f, 0.585f, 0.930f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Begin draw
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boxTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, faceTexture);

		va->Bind();
		shader->Use();

		constexpr glm::vec3 cubePositions[] =
		{
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(2.0f, 5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f, 3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f, 2.0f, -2.5f),
			glm::vec3(1.5f, 0.2f, -1.5f),
			glm::vec3(-1.3f, 1.0f, -1.5f)
		};

		auto view = camera->GetViewMatrix();

		const auto windowSize = window->GetSize();

		const auto projection = glm::perspective(
			glm::radians(camera->GetZoom()), windowSize.x / windowSize.y, 0.1f, 100.0f);

		shader->SetMat4f("view", view);
		shader->SetMat4f("projection", projection);

		for (auto i = 0; i < 10; i++)
		{
			auto model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			
			model = glm::rotate(model, window->GetElapsedTime() * glm::radians(20.0f * cubePositions[i].z), glm::vec3(1.0f, 0.3f, 0.5f));

			shader->SetMat4f("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		//End Draw
		va->Unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
		shader->Unuse();
	}
}

