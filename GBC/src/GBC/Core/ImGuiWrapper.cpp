#include "gbcpch.h"
#if GBC_ENABLE_IMGUI
#include "ImGuiWrapper.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glfw/glfw3.h"
#include "Application.h"

namespace gbc
{
	ImGuiWrapper::ImGuiWrapper()
	{
		GBC_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		Init();
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), true);
		ImGui_ImplOpenGL3_Init("#version 460"); // TODO: this version should be gotten from the window
	}

	ImGuiWrapper::~ImGuiWrapper()
	{
		GBC_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiWrapper::Begin()
	{
		GBC_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiWrapper::End()
	{
		GBC_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Window& window = Application::Get().GetWindow();
		io.DisplaySize = ImVec2((float)window.GetWidth(), (float)window.GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(context);
		}
	}

	void ImGuiWrapper::Init()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard
					   | ImGuiConfigFlags_DockingEnable
					   | ImGuiConfigFlags_ViewportsEnable;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;

		// Colors
		ImGui::StyleColorsDark();

		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = {0.1f, 0.105f, 0.11f, 1.0f};

		// Headers
		colors[ImGuiCol_Header] = {0.2f, 0.205f, 0.21f, 1.0f};
		colors[ImGuiCol_HeaderHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
		colors[ImGuiCol_HeaderActive] = {0.15f, 0.155f, 0.16f, 1.0f};

		// Buttons
		colors[ImGuiCol_Button] = {0.2f, 0.205f, 0.21f, 1.0f};
		colors[ImGuiCol_ButtonHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
		colors[ImGuiCol_ButtonActive] = {0.15f, 0.155f, 0.16f, 1.0f};

		// Frame Background
		colors[ImGuiCol_FrameBg] = {0.2f, 0.205f, 0.21f, 1.0f};
		colors[ImGuiCol_FrameBgHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
		colors[ImGuiCol_FrameBgActive] = {0.15f, 0.155f, 0.16f, 1.0f};

		// Tabs
		colors[ImGuiCol_Tab] = {0.15f, 0.155f, 0.16f, 1.0f};
		colors[ImGuiCol_TabHovered] = {0.38f, 0.385f, 0.39f, 1.0f};
		colors[ImGuiCol_TabActive] = {0.28f, 0.285f, 0.29f, 1.0f};
		colors[ImGuiCol_TabUnfocused] = {0.15f, 0.155f, 0.16f, 1.0f};
		colors[ImGuiCol_TabUnfocusedActive] = {0.2f, 0.205f, 0.21f, 1.0f};

		// Title Background
		colors[ImGuiCol_TitleBg] = {0.15f, 0.155f, 0.16f, 1.0f};
		colors[ImGuiCol_TitleBgActive] = {0.15f, 0.155f, 0.16f, 1.0f};
		colors[ImGuiCol_TitleBgCollapsed] = {0.15f, 0.155f, 0.16f, 1.0f};
	}

	bool ImGuiWrapper::IsUsingKeyEvents() const
	{
		return blockEvents && ImGui::GetIO().WantCaptureKeyboard;
	}

	bool ImGuiWrapper::IsUsingMouseEvents() const
	{
		return blockEvents && ImGui::GetIO().WantCaptureMouse;
	}
}
#endif
