#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

#include "imgui/imgui.h"
#include <imgui/imgui_impl_opengl3.h>
#include <imgui\imgui_impl_glfw.h>

#include "tests/DebugGUI.h"

//Mouse initial offset
float m_lastX = 640, m_lastY = 360;
//Yaw, pitch
float m_yaw = -90.0f, m_pitch = 0.0f;
bool m_firstMouse = true;

bool instanceToggle = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

namespace Chunk {
	std::size_t Width = 128, Height = 128, Depth = 128;
	int size = Width * Height* Depth;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (m_firstMouse)
	{
		m_lastX = xPos;
		m_lastY = yPos;
		m_firstMouse = false;
	}

	float xOffset = xPos - m_lastX;
	float yOffset = m_lastY - yPos; //Reset since y coordinates range from bottom to top
	m_lastX = xPos;
	m_lastY = yPos;

	const float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	m_yaw += xOffset;
	m_pitch += yOffset;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));
	Camera::SetCameraForward(glm::normalize(direction));

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

	{

		if (flags && GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			std::cout << "Registered OpenGL Debug Callback" << std::endl;
			GLCall(glEnable(GL_DEBUG_OUTPUT));
			GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
			GLCall(glDebugMessageCallback(glDebugOutput, nullptr));
			GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE));
		}

		float vertices[] = {
		//positions			 //TexCoord
	   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //0
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, //1
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //2
	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, //3

	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //4
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //5
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //6
	   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, //7

	   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //8
	   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //9
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //10 
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //11

		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //12
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //13
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f, //14
	   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, //15
		};

		unsigned short indices[] = {
			//Front //Check when free movement camera is in check which side is which
			0, 1, 2, 
			2, 3, 0,

			//Back
			4, 5, 6,
			6, 7, 4,

			//Left
			8, 9, 10,
			10, 4, 8,

			//Right
			11, 2, 12,
			12, 13, 11,

			//Bottom
			10, 14, 5,
			5, 4, 10,

			//Top
			3, 2, 11,
			11, 15, 3
		};

		glm::vec3* offsetTranslation = new glm::vec3[Chunk::size];
		int index = 0;
		for (int x = 0; x < Chunk::Width; x++)
		{
			for (int y = 0; y < Chunk::Height; y++)
			{
				for (int z = 0; z < Chunk::Depth; z++)
				{
					glm::vec3 translation = glm::vec3(x, y, z);
					offsetTranslation[index++] = translation;
				}
			}
		}

		VertexArray vertexArray;
		VertexBuffer vertexBuffer(&vertices, sizeof(vertices));
		
		VertexBuffer instanceVBO(&offsetTranslation[0], sizeof(glm::vec3)* Chunk::size);
	
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		vertexArray.Bind();
		vertexArray.AddBuffer(vertexBuffer, layout);

		VertexBufferLayout InstanceLayout;
		InstanceLayout.Push<float>(3);
		vertexArray.AddInstanceBuffer(instanceVBO, InstanceLayout, 2);

		IndexBuffer indexBuffer(indices, 36);

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 1.0f, 150.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -40.0f));

		Shader shader("resources/shaders/Basic.shader");
		shader.Bind();
		//shader.SetUniform4f("u_Colour", 0.8f, 0.3f, 0.8f, 1.0f);

		Texture texture("resources/textures/Crate.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		vertexArray.Unbind();
		indexBuffer.Unbind();
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

			if (instanceToggle)
			{
				glm::mat4 model = glm::mat4(1.0f);
				float angle = 0.0;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
				glm::mat4 mvp = proj * view * model;
				shader.SetUniformMat4f("u_MVP", mvp);
				Renderer::Draw(vertexArray, indexBuffer, shader, Chunk::size);
			}
			else
			{
				for (int i = 0; i < Chunk::size; i++)
				{
					glm::mat4 model = glm::translate(glm::mat4(1.0f), offsetTranslation[i] / 2.0f);
					float angle = 0.0;
					model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
					glm::mat4 mvp = proj * view * model;
					shader.SetUniformMat4f("u_MVP", mvp);
					Renderer::Draw(vertexArray, indexBuffer, shader);
				}	
			}

			test::DebugGUI::GetInstance().OnImGuiRender(instanceToggle);

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