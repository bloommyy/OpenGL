#include "Application_Lighting.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>

namespace Applications
{
	Application_Lighting::Application_Lighting()
		: lightPos(1.2f, 1.0f, 1.0f), boxDiffuseMap(0), boxSpecularMap(0), boxEmissionMap(0)
	{
		window = std::make_unique<Utils::Window>("TU.CG.Lab", 800, 600);
	}

	void Application_Lighting::Run()
	{
		auto lastFrame = 0.0f;

		Initialize();
		LoadContent();

		while (!window->GetShouldClose())
		{
			const auto currentFrame = window->GetElapsedTime();
			const auto deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			Update(deltaTime);
			Render();

			window->SwapBuffers();
			window->PollEvents();
		}

		UnloadContent();
	}

	void Application_Lighting::Initialize()
	{
		window->ActivateInputFor(this);

		camera = std::make_unique<Utils::Camera3D>(
			glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);

		stbi_set_flip_vertically_on_load(true);

		glEnable(GL_DEPTH_TEST);
	}

	void Application_Lighting::LoadContent()
	{
		//
		// -- Textures
		//

		glGenTextures(1, &boxDiffuseMap);
		glBindTexture(GL_TEXTURE_2D, boxDiffuseMap);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, channels;
		std::string texturePath = "Content/Textures/container2.png";
		auto data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, data);

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

		glGenTextures(1, &boxEmissionMap);
		glBindTexture(GL_TEXTURE_2D, boxEmissionMap);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		texturePath = "Content/Textures/matrix.jpg";
		data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, data);

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

		glGenTextures(1, &boxSpecularMap);
		glBindTexture(GL_TEXTURE_2D, boxSpecularMap);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		texturePath = "Content/Textures/container2_specular.png";
		data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, data);

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

		//
		// -- Buffers
		//

		constexpr float lightVertices[] =
		{
			// positions
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		constexpr float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
		};

		lightVa = std::make_unique<Graphics::VertexArray>();

		auto lightVb = std::make_unique<Graphics::VertexBuffer>(
			lightVertices, sizeof lightVertices
			);

		lightVb->SetAttributes({
			{"aPos", Graphics::VertexAttributeType::VEC3F},
			});

		lightVa->SetVertexBuffer(std::move(lightVb));

		objectVa = std::make_unique<Graphics::VertexArray>();

		auto objectVb = std::make_unique<Graphics::VertexBuffer>(
			vertices, sizeof vertices
		);

		objectVb->SetAttributes({
			{"aPos", Graphics::VertexAttributeType::VEC3F},
			{"aNormal", Graphics::VertexAttributeType::VEC3F},
			{"aTexCoords", Graphics::VertexAttributeType::VEC2F} });

		objectVa->SetVertexBuffer(std::move(objectVb));

		// Shaders

		lightShader = std::make_unique<Graphics::ShaderProgram>(
			"Content/Shaders/light_box.vert",
			"Content/Shaders/light_box.frag"
			);

		objectShader = std::make_unique<Graphics::ShaderProgram>(
			"Content/Shaders/lighting.vert",
			"Content/Shaders/lighting.frag"
			);

		objectShader->Use();

		objectShader->SetInt("material.diffuse", 0);
		objectShader->SetInt("material.specular", 1);
		objectShader->SetInt("material.emission", 2);
		objectShader->SetFloat("material.shininess", 32.0f);
		objectShader->SetVec3f("light.position", lightPos);
		objectShader->SetVec3f("light.ambient", glm::vec3(0.2f));
		objectShader->SetVec3f("light.diffuse", glm::vec3(0.5f));
		objectShader->SetVec3f("light.specular", glm::vec3(1.0f));

		objectShader->Unuse();
	}

	void Application_Lighting::UnloadContent()
	{
		glDeleteTextures(1, &boxDiffuseMap);
		glDeleteTextures(1, &boxSpecularMap);
		glDeleteTextures(1, &boxEmissionMap);

		objectVa = nullptr;
		objectShader = nullptr;
		lightVa = nullptr;
		lightShader = nullptr;
	}

	void Application_Lighting::Update(const float deltaTime)
	{
		if (inputManager.IsKeyDown(Input::Keys::ESCAPE))
			window->SetShouldClose(true);

		camera->Update(deltaTime, inputManager);

		inputManager.ResetState();
	}

	void Application_Lighting::Render() const
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto model = glm::mat4(1.0f);

		const auto view = camera->GetViewMatrix();

		const auto windowSize = window->GetSize();

		const auto projection = glm::perspective(
			glm::radians(camera->GetZoom()), windowSize.x / windowSize.y, 0.1f, 100.0f);

		const auto normalMatrix = glm::inverseTranspose(glm::mat3(model));

		objectShader->Use();
		objectVa->Bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boxDiffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, boxSpecularMap);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, boxEmissionMap);

		objectShader->SetMat4f("model", model);
		objectShader->SetMat4f("view", view);
		objectShader->SetMat4f("projection", projection);
		objectShader->SetMat3f("normal", normalMatrix);
		objectShader->SetVec3f("viewPos", camera->GetPosition());

		glDrawArrays(GL_TRIANGLES, 0, 36);

		objectVa->Unbind();
		objectShader->Unuse();

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lightShader->Use();
		lightVa->Bind();

		lightShader->SetMat4f("model", model);
		lightShader->SetMat4f("view", view);
		lightShader->SetMat4f("projection", projection);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightVa->Unbind();
		lightShader->Unuse();

	}
}
