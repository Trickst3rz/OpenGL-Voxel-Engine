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
	{ //ImGui render for toggles, buttons etc.
		{
			ImGui::Begin("Debug");
			test.OnImGuiRender();
			if (ImGui::Button("Set Light Position"))
			{
				shader.Bind();
				shader.SetUniform3f("u_lightPos", Camera::GetInstance().GetCameraPosition().x, Camera::GetInstance().GetCameraPosition().y, Camera::GetInstance().GetCameraPosition().z);
			}
			ImGui::Checkbox("Occlusion Culling", &Global::GetInstance().OcclusionCulling);
			ImGui::Checkbox("Toggle WireFrame", &isWireFrame);
			ImGui::Separator();
			ImGui::Text("Frustum Culling");
			if (ImGui::Checkbox("Auto Frustum Culling", &Global::GetInstance().ToggleFrustum))
			{
				Frustum::GetInstance().SetCamera(Camera::GetInstance().GetCameraPosition(), Camera::GetInstance().GetCameraFront(), Camera::GetInstance().GetCameraUp());
				ChunkManager::GetInstance().UpdateVisibilityList();
				ChunkManager::GetInstance().UpdateRenderList();
			}
			if (ImGui::Button("Frustum Camera"))
			{
				Global::GetInstance().FrustumCamera = true;
				Frustum::GetInstance().SetCamera(Camera::GetInstance().GetCameraPosition(), Camera::GetInstance().GetCameraFront(), Camera::GetInstance().GetCameraUp());
				ChunkManager::GetInstance().UpdateVisibilityList();
				ChunkManager::GetInstance().UpdateRenderList();
			}
			if (ImGui::Button("Clear Frustum Camera"))
			{
				Global::GetInstance().FrustumCamera = false;
				ChunkManager::GetInstance().UpdateVisibilityList();
				ChunkManager::GetInstance().UpdateRenderList();
			}
			if (ImGui::SliderFloat("Field of View", &Global::GetInstance().FOV, 10.0f, 120.0f))
			{
				Frustum::GetInstance().SetCamera(Camera::GetInstance().GetCameraPosition(), Camera::GetInstance().GetCameraFront(), Camera::GetInstance().GetCameraUp());
				ChunkManager::GetInstance().UpdateVisibilityList();
				ChunkManager::GetInstance().UpdateRenderList();
			}
			ImGui::SliderFloat("Width", &Global::GetInstance().Width, 200.0f, 4000.0f);
			ImGui::SliderFloat("Height", &Global::GetInstance().Height, 200.0f, 2000.0f);
			if (ImGui::SliderFloat("View Distance", &Global::GetInstance().farDistance, 25.0f, 3000.0f))
			{
				Frustum::GetInstance().SetCamera(Camera::GetInstance().GetCameraPosition(), Camera::GetInstance().GetCameraFront(), Camera::GetInstance().GetCameraUp());
				ChunkManager::GetInstance().UpdateVisibilityList();
				ChunkManager::GetInstance().UpdateRenderList();
			}
			ImGui::Separator();
			ImGui::Text("Terrain Generation");
			if (ImGui::Button("Reset"))
			{
				Global::GetInstance().Frequency = 1.0;
				Global::GetInstance().OctaveCount = 6;
			}
			ImGui::InputInt("Seed", &Global::GetInstance().Seed, 0, 3000);
			ImGui::InputDouble("Frequency", &Global::GetInstance().Frequency, 0, 100);
			ImGui::SliderInt("Octave Count", &Global::GetInstance().OctaveCount, 1, 30);
			if (ImGui::Button("Render Distance:")) //When I do frustum culling add this feature to change render distance
			{
				if (Global::GetInstance().GetAmountOfChunks() == 33)
					Global::GetInstance().SetAmountOfChunks(5);
				else
					Global::GetInstance().SetAmountOfChunks((Global::GetInstance().GetAmountOfChunks() * 2) - 1);
			}
			ImGui::SameLine();
			ImGui::Text("%d chunks", (Global::GetInstance().GetAmountOfChunks() - 1) / 2);
			{
				if (ImGui::Button("Generate Terrain"))
				{
					ChunkManager::GetInstance().CreateNewTerrain();
				}
			}
			ImGui::Separator();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Draw Calls: %d", Renderer::GetDrawCalls());
			ImGui::End();
		}
		WireFrame();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void DebugGUI::WireFrame()
	{ //Toggle to fill polygons or lines for wirefame
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

