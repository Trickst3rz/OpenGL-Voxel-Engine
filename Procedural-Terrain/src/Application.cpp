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
#include <memory>
#include <ctime>

#include "Renderer.h"

#include "GLErrorManager.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Chunk.h"
#include "ChunkManager.h"
#include "Mesh.h"
#include "Frustum.h"
#include "Global.h"

#include "imgui/imgui.h"
#include <imgui/imgui_impl_opengl3.h>
#include <imgui\imgui_impl_glfw.h>

#include "tests/DebugGUI.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;
static float nearD = 1.0f;

int main(void)
{
	srand((unsigned int)time(NULL));

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
	window = glfwCreateWindow((int)Global::Width, (int)Global::Height, "Voxel World", NULL, NULL);
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
	
		Global::SetSeed();
		Camera::SetCameraPosition(glm::vec3(0.0f, 32.0f, 0.0f));
		
		ChunkManager::GetInstance().Start();
		ChunkManager::GetInstance().AsyncLoadChunks();

		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model = glm::mat4(1.0f);

		Shader shader("resources/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform3f("u_lightPos", 50.0f, 50.0f, 2.0f);
		shader.SetUniform3f("u_lightColour", 1.0f, 1.0f, 1.0f);
		shader.SetUniformMat4f("u_Projection", proj);
		shader.Unbind();

		Shader DebugShader("resources/shaders/Lines.shader");
		DebugShader.Bind();
		DebugShader.SetUniform3f("u_Colour", 1.0f, 1.0f, 1.0f);
		DebugShader.Unbind();

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		ImGui::StyleColorsDark();

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColour>("Clear Colour");

		Camera::SetCameraPosition(glm::vec3(16.0f, 32.0f, 16.0f));

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			//Delta Time of game loop
			float currentFrame = (float)glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			Camera::processInput(window, deltaTime);

			/* Render here */
			Renderer::Clear();
			
			test::DebugGUI::GetInstance().OnUpdate(deltaTime);
			test::DebugGUI::GetInstance().OnRender();

			test::DebugGUI::GetInstance().NewFrame();

			shader.Bind();

			proj = glm::perspective(glm::radians(Global::FOV), Global::Width / Global::Height, nearD, 5000.0f);
			Frustum::GetInstance().SetFrustum(glm::radians(Global::FOV), Global::Width / Global::Height, nearD, Global::farDistance);
			view = glm::lookAt(Camera::GetCameraPosition(), Camera::GetCameraPosition() + Camera::GetCameraFront(), Camera::GetCameraUp());

			shader.SetUniformMat4f("u_Model", model);
			shader.SetUniformMat4f("u_View", view);
			shader.SetUniformMat4f("u_Projection", proj);
			ChunkManager::GetInstance().Update(shader);
			DebugShader.Bind();
			DebugShader.SetUniformMat4f("u_MVP", proj * view * model);
			Frustum::GetInstance().DrawLines(DebugShader);

			test::DebugGUI::GetInstance().OnImGuiRender(shader);

			Renderer::SetDrawCalls(0);
			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());

		}
		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	}
	
	// Cleanup
	test::DebugGUI::GetInstance().NewFrame();
	ChunkManager::GetInstance().Terminate();

	glfwTerminate();
	return 0;
}