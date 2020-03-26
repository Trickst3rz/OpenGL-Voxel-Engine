#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <noise/noise.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

#include "Renderer.h"

#include "GLErrorManager.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Chunk.h"
#include "Mesh.h"

#include "imgui/imgui.h"
#include <imgui/imgui_impl_opengl3.h>
#include <imgui\imgui_impl_glfw.h>

#include "tests/DebugGUI.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool BatchToggle = true;
bool instanceToggle = false;

typedef glm::tvec3<GLbyte> Byte3;

namespace testChunk {
	const std::size_t Width = 32, Height = 32, Depth = 32;
	const std::size_t TotalSize = Width * Height * Depth;
}

int main(void)
{
	GLFWwindow* window;
	
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            
#endif

	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 720, "Voxel World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(0); // vsync = 0(off) 1(on)

	//Mouse input callback
	glfwSetCursorPosCallback(window, Camera::mouse_callback);

	//Tells GLFW where to capture the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
		std::cout << "glewInit failed something is wrong";
	
	std::cout << glGetString(GL_VERSION) << std::endl;

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS));
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_BACK));

	{

		if (flags && GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			std::cout << "Registered OpenGL Debug Callback" << std::endl;
			GLCall(glEnable(GL_DEBUG_OUTPUT));
			GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
			GLCall(glDebugMessageCallback(glDebugOutput, nullptr));
			GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE));
		}

		GLbyte vertices[] = {
			//positions	//TexCoord
			//Front 
			0,  0,  0,  0, 0, //0
			0,  1,  0,  1, 0, //1
			1,  0,  0,  1, 1, //2
			0,  1,  0,  1, 1, //2
			1,  1,  0,  0, 1, //3
			1,  0,  0,  0, 0, //0

			//Back
			1,  0,  1,  0, 0, //4
			1,  1,  1,  1, 0, //5
			0,  0,  1,  1, 1, //6
			1,  1,  1,  1, 1, //6
			0,  1,  1,  0, 1, //7
			0,  0,  1,  0, 0, //4

			//Right
			1,  0,  0,  1, 0, //8
			1,  1,  0,  1, 1, //9
			1,  0,  1,  0, 1, //10 
			1,  1,  0,  0, 1, //10 
			1,  1,  1,  0, 0, //4
			1,  0,  1,  1, 0, //8

			//Left
			0,  0,  0,  1, 0, //11
			0,  0,  1,  1, 1, //2
			0,  1,  0,  0, 1, //12
			0,  1,  0,  0, 1, //12
			0,  0,  1,  0, 0, //13
			0,  1,  1,  1, 0, //11

			//Bottom
			0,  0,  0,  0, 1, //10
			1,  0,  0,  1, 1, //14
			1,  0,  1,  1, 0, //5
			1,  0,  1,  1, 0, //5
			0,  0,  1,  0, 0, //4
			0,  0,  0,  0, 1, //10 

			//Top
			1,  1,  0,  0, 1, //3
			0,  1,  0,  1, 1, //2
			0,  1,  1,  1, 0, //11
			0,  1,  1,  1, 0, //11
			1,  1,  1,  0, 0, //15
			1,  1,  0,  0, 1, //3
		};
		
		Byte3* offsetTranslation = new Byte3[testChunk::TotalSize];
		int offsetIndex = 0;
		for (int x = 0; x < testChunk::Width; x++)
		{
			for (int y = 0; y < testChunk::Height; y++)
			{
				for (int z = 0; z < testChunk::Depth; z++)
				{
					offsetTranslation[offsetIndex++] = Byte3(x, y, z);
				}
			}
		}
		
		Chunk* chunk = new Chunk;
		//chunk.SetupSphere(); //SetupLandscape here for procedural generation
		chunk->SetupLandscape();
		//chunk->SetupAll();
		chunk->CreateMesh();
		
		VertexArray LightingVAO;
		VertexArray InstanceVertexArray;
		VertexArray BatchVertexArray;

		VertexBuffer LightingVBO(&vertices, sizeof(vertices));
		VertexBuffer InstanceVertexBuffer(&vertices, sizeof(vertices));
		VertexBuffer BatchVertexBuffer(chunk->GetVertex(), chunk->GetElementCount() * sizeof * chunk->GetVertex());
		VertexBuffer instanceVBO(&offsetTranslation[0], offsetIndex * sizeof * offsetTranslation);

		VertexBufferLayout BatchLayout;
		BatchLayout.Push<GLbyte>(3);
		BatchLayout.Push<GLbyte>(3);
		//BatchLayout.Push<GLbyte>(1);
		BatchVertexArray.Bind();
		BatchVertexArray.AddBuffer(BatchVertexBuffer, BatchLayout);

		VertexBufferLayout layout;
		layout.Push<GLbyte>(3);
		layout.Push<GLbyte>(2);
		InstanceVertexArray.Bind();
		InstanceVertexArray.AddBuffer(InstanceVertexBuffer, layout);

		VertexBufferLayout InstanceLayout;
		InstanceLayout.Push<GLbyte>(3);
		InstanceVertexArray.AddInstanceBuffer(instanceVBO, InstanceLayout, 3);

		LightingVAO.Bind();
		LightingVAO.AddBuffer(LightingVBO, layout);

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 1.0f, 150.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -40.0f));

		Shader shader("resources/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform3f("u_Colour", 1.0f, 0.5f, 0.5f);
		shader.SetUniform3f("u_lightPos", 50.0f, 50.0f, 2.0f);
		shader.SetUniform3f("u_lightColour", 1.0f, 1.0f, 1.0f);
		shader.SetUniformMat4f("u_Projection", proj);

		Texture texture("resources/textures/Crate.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		LightingVAO.Unbind();
		InstanceVertexArray.Unbind();
		BatchVertexArray.Unbind();
		shader.Unbind();

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		ImGui::StyleColorsDark();

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColour>("Clear Colour");

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			//Delta Time of game loop
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			Camera::processInput(window, deltaTime);

			/* Render here */
			Renderer::Clear();
			
			test::DebugGUI::GetInstance().OnUpdate(deltaTime);
			test::DebugGUI::GetInstance().OnRender();

			test::DebugGUI::GetInstance().NewFrame();

			//For changing menu with button
			/*if (currentTest)
			{
				currentTest->OnUpdate(deltaTime);
				currentTest->OnRender();
				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("<-"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}*/

			shader.Bind();

			view = glm::lookAt(Camera::GetCameraPosition(), Camera::GetCameraPosition() + Camera::GetCameraFront(), Camera::GetCameraUp());
			int k = sizeof(vertices);
			if (BatchToggle)
			{
				glm::mat4 model = glm::mat4(1.0f);
				float angle = 0.0;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
				shader.SetUniformMat4f("u_Model", model);
				shader.SetUniformMat4f("u_View", view);
				Renderer::Draw(BatchVertexArray, shader, chunk->GetElementCount());
				//shader.SetUniform4f("u_Colour", 1.0f, 0.5f, 0.31f, 1.0f);
				//Renderer::Draw(LightingVAO, shader, sizeof(vertices));
			}
			if(instanceToggle)
			{
				glm::mat4 model = glm::mat4(1.0f);
				float angle = 0.0;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
				shader.SetUniformMat4f("u_Model", model);
				shader.SetUniformMat4f("u_View", view);
				Renderer::DrawInstanced(InstanceVertexArray, shader, sizeof(vertices), testChunk::TotalSize);
			}

			test::DebugGUI::GetInstance().OnImGuiRender(BatchToggle, instanceToggle);

			Renderer::SetDrawCalls(0);
			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());

		}
		delete[] offsetTranslation;
		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	}
	
	// Cleanup
	test::DebugGUI::GetInstance().NewFrame();

	glfwTerminate();
	return 0;
}