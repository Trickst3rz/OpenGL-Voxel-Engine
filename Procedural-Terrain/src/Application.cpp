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

#include "imgui/imgui.h"
#include <imgui/imgui_impl_opengl3.h>
#include <imgui\imgui_impl_glfw.h>

namespace Chunk {
	std::size_t Width = 16, Height = 16, Depth = 16;
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

	glfwSwapInterval(1); // vsync = 0(off) 1(on)
	
	if (glewInit() != GLEW_OK)
		std::cout << "glewInit failed something is wrong";
	
	std::cout << glGetString(GL_VERSION) << std::endl;

	GLCall(glEnable(GL_DEPTH_TEST));

	{

		if (flags && GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			std::cout << "Registered OpenGL Debug Callback" << std::endl;
			GLCall(glEnable(GL_DEBUG_OUTPUT));
			GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
			GLCall(glDebugMessageCallback(glDebugOutput, nullptr));
			GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE));
		}

		glm::vec3 cubePositions[] = {
		  glm::vec3( 0.0f,  0.0f,  0.0f),
		  glm::vec3( 2.0f,  5.0f, -15.0f),
		  glm::vec3(-1.5f, -2.2f, -2.5f),
		  glm::vec3(-3.8f, -2.0f, -12.3f),
		  glm::vec3( 2.4f, -0.4f, -3.5f),
		  glm::vec3(-1.7f,  3.0f, -7.5f),
		  glm::vec3( 1.3f, -2.0f, -2.5f),
		  glm::vec3( 1.5f,  2.0f, -2.5f),
		  glm::vec3( 1.5f,  0.2f, -1.5f),
		  glm::vec3(-8.3f,  1.0f, -1.5f)
		};

		float vertices[] = {
		//positions			 //texCoord
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

		VertexArray vertexArray;
		VertexBuffer vertexBuffer(vertices, sizeof(vertices));
		
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		vertexArray.AddBuffer(vertexBuffer, layout);

		IndexBuffer indexBuffer(indices, 36);

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

		Shader shader("resources/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Colour", 0.8f, 0.3f, 0.8f, 1.0f);

		Texture texture("resources/textures/Crate.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		vertexArray.Unbind();
		vertexBuffer.Unbind();
		indexBuffer.Unbind();
		shader.Unbind();

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		ImGui::StyleColorsDark();

		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);

		float r = 0.05f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			Renderer::Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			shader.Bind();

			for(unsigned int i = 0; i < 10; i++)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
				float angle = 20.0f * i;
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
				glm::mat4 mvp = proj * view * model;
				shader.SetUniform4f("u_Colour", r, 0.3f, 0.8f, 1.0f);
				shader.SetUniformMat4f("u_MVP", mvp);

				Renderer::Draw(vertexArray, indexBuffer, shader);
			}

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			{
				//ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 1280.0f); 
				//ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 1280.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());

		}
	}
	
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}