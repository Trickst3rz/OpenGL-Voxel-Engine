#include "DebugGUI.h"
#include "imgui/imgui.h"
#include <imgui/imgui_impl_opengl3.h>
#include <imgui\imgui_impl_glfw.h>
#include "Renderer.h"

namespace test {

	void DebugGUI::OnUpdate(float deltaTime)
	{
		test.OnUpdate(deltaTime);
	}

	void DebugGUI::OnRender()
	{
		test.OnRender();
	}

	void DebugGUI::OnImGuiRender(bool &instanceToggle)
	{
		{
			ImGui::Begin("Debug");
			test.OnImGuiRender();
			ImGui::Checkbox("Instance Rendering", &instanceToggle);
			ImGui::Checkbox("Toggle WireFrame", &isWireFrame);
			if (ImGui::Button("Render Distance:")) //When I do frustum culling add this feature to change render distance
			{
				if (chunks == 32)
					chunks = 8;
				else
					chunks += 8;
			}
			ImGui::Text("%d chunks", chunks);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Draw Calls: %d", Renderer::GetDrawCalls());
			ImGui::End();
		}
		WireFrame();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void DebugGUI::WireFrame()
	{
		if (isWireFrame)
		{
			GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE))
		}
		else
		{
			GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL))
		}
	}

	void DebugGUI::NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void DebugGUI::ImGuiCleanUp()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}

