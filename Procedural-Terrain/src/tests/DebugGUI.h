#pragma once

#include "TestClearColour.h"
#include "Test.h"

namespace test {
	class DebugGUI : Test
	{
	public:
		static DebugGUI& GetInstance()
		{
			static DebugGUI instance;

			return instance;
		};


		void OnUpdate(float deltaTime);

		void OnRender();

		void OnImGuiRender(bool &instanceToggle);

		void WireFrame();

		void NewFrame();

		void ImGuiCleanUp();

		private:
			DebugGUI() {};
			TestClearColour test;
			int chunks = 8;
			bool isWireFrame = false;
	};
}