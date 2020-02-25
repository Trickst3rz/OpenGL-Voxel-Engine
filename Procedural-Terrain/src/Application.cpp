#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "GLErrorManager.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	glfwSwapInterval(1);
	
	if (glewInit() != GLEW_OK)
		std::cout << "glewInit failed something is wrong";
	
	std::cout << glGetString(GL_VERSION) << std::endl;

	{

		if (flags && GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			std::cout << "Registered OpenGL Debug Callback" << std::endl;
			GLCall(glEnable(GL_DEBUG_OUTPUT));
			GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
			GLCall(glDebugMessageCallback(glDebugOutput, nullptr));
			GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE));
		}

		float positions[] = {
			-0.5f, -0.5f,	//0
			 0.5f, -0.5f,	//1
			 0.5f,  0.5f,	//2
			-0.5f,  0.5f,	//3
		};

		unsigned short indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray vertexArray;
		VertexBuffer vertexBuffer(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		vertexArray.AddBuffer(vertexBuffer, layout);

		IndexBuffer indexBuffer(indices, 6);

		glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

		Shader shader("resources/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Colour", 0.8f, 0.3f, 0.8f, 1.0f);
		shader.SetUniformMat4f("u_MVP", proj);

		vertexArray.Unbind();
		vertexBuffer.Unbind();
		indexBuffer.Unbind();
		shader.Unbind();

		float r = 0.05f;
		float increment = 0.05f;

		double previousTime = glfwGetTime();
		int frameCount = 0;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			Renderer::Clear();

			shader.Bind();
			shader.SetUniform4f("u_Colour", r, 0.3f, 0.8f, 1.0f);

			Renderer::Draw(vertexArray, indexBuffer, shader);

			double currentTime = glfwGetTime();
			frameCount++;

			if (currentTime - previousTime >= 1.0)
			{
				std::cout << frameCount << std::endl;

				frameCount = 0;
				previousTime = currentTime;
			}
			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());
		}
	}
	
	glfwTerminate();
	return 0;
}