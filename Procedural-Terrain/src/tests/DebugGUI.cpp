#include "DebugGUI.h"
#include "imgui/imgui.h"
#include <imgui/imgui_impl_opengl3.h>
#include <imgui\imgui_impl_glfw.h>
#include "Renderer.h"
#include "Camera.h"
#include "Frustum.h"

namespace test {

	void DebugGUI::OnUpdate(float deltaTime)
	{
		test.OnUpdate(deltaTime);
	}

	void DebugGUI::OnRender()
	{
		test.OnRender();
	}

	void DebugGUI::OnImGuiRender(Shader& shader)
	{
		{
			ImGui::Begin("Debug");
			test.OnImGuiRender();
			if (ImGui::Button("Set Light Position"))
			{
				shader.Bind();
				shader.SetUniform3f("u_lightPos", Camera::GetCameraPosition().x, Camera::GetCameraPosition().y, Camera::GetCameraPosition().z);
			}
			ImGui::Checkbox("Toggle WireFrame", &isWireFrame);
			ImGui::Separator();
			ImGui::Text("Frustum Culling");
			ImGui::Checkbox("Auto Frustum Culling", &Global::ToggleFrustum);
			if (ImGui::Button("Frustum Camera"))
			{
				Global::FrustumCamera = true;
				Frustum::GetInstance().SetCamera(Camera::GetCameraPosition(), Camera::GetCameraFront(), Camera::GetCameraUp(), Camera::GetCameraRight());
				ChunkManager::GetInstance().UpdateVisibilityList();
				ChunkManager::GetInstance().UpdateRenderList();
				ChunkManager::GetInstance().UpdateCullingList();
			}
			ImGui::SliderFloat("Field of View", &Global::FOV, 10.0f, 120.0f);
			ImGui::SliderFloat("Width", &Global::Width, 200.0f, 4000.0f);
			ImGui::SliderFloat("Height", &Global::Height, 200.0f, 2000.0f);
			ImGui::SliderFloat("View Distance", &Global::farDistance, 10.0f, 3000.0f);
			ImGui::Separator();
			if (ImGui::Button("Render Distance:")) //When I do frustum culling add this feature to change render distance
			{
				if (chunks == 32)
					chunks = 8;
				else
					chunks += 8;
				ChunkManager::GetInstance().SetChunkDistance(chunks);
			}
			ImGui::SameLine();
			ImGui::Text("%d chunks", chunks);
			{
				if (ImGui::Button("Generate Terrain"))
				{
					//Delete terrain and start again
		
				}
			}
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

