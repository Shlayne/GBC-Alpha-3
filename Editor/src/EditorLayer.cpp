#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "imguizmo/ImGuizmo.h"
#include "Panels/StasticsPanel.h"
#include "Panels/ProfilingPanel.h"
#include "GBC/Scene/SceneSerializer.h"

namespace gbc
{
	void EditorLayer::OnAttach()
	{
		GBC_PROFILE_FUNCTION();

		Window& window = Application::Get().GetWindow();

		editorCamera = EditorCamera(90.0f, 0.001f, 1000.0f);
		editorCamera.OnViewportResize(window.GetWidth(), window.GetHeight());

		FramebufferSpecification framebufferSpecification;
		framebufferSpecification.width = window.GetWidth();
		framebufferSpecification.height = window.GetHeight();
		framebufferSpecification.attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RedInteger, FramebufferTextureFormat::Depth24Stencil8};
		framebuffer = Framebuffer::CreateRef(framebufferSpecification);

		scene = CreateRef<Scene>();
		scene->OnCreate();

		// TODO: Figure out a different way to have Panels change values in EditorLayer
		sceneViewportPanel = AddPanel<SceneViewportPanel>("Scene Viewport", viewportSizeChanged, viewportFocused, viewportHovered, viewportSize, viewportPos, absoluteMousePos, framebuffer, scene, selectedEntity, gizmoType, editorCamera);
		sceneHierarchyPanel = AddPanel<SceneHierarchyPanel>("Scene Hierarchy", scene, selectedEntity);
		scenePropertiesPanel = AddPanel<ScenePropertiesPanel>("Scene Properties", selectedEntity);
#if GBC_ENABLE_STATS
		AddPanel<StatisticsPanel>("Statistics", BasicRenderer::GetStatistics());
#endif
#if GBC_ENABLE_PROFILE_RUNTIME
		AddPanel<ProfilingPanel>("Profiling");
#endif
	}

	void EditorLayer::OnDetach()
	{
		GBC_PROFILE_FUNCTION();

		scene->OnDestroy();

		for (auto& [name, panel] : panels)
			delete panel;
	}

	void EditorLayer::OnUpdate(Timestep timestep)
	{
		GBC_PROFILE_FUNCTION();

		Application::Get().GetImGuiLayer().SetBlockEvents(!viewportFocused && !viewportHovered);

		if (viewportSizeChanged)
		{
			viewportSizeChanged = false;

			editorCamera.OnViewportResize(viewportSize.x, viewportSize.y);
			framebuffer->OnViewportResize(viewportSize.x, viewportSize.y);
			scene->OnViewportResize(viewportSize.x, viewportSize.y);
		}

		editorCamera.OnUpdate(timestep);
		scene->OnUpdateEditor(timestep);
	}

	void EditorLayer::OnRender()
	{
		GBC_PROFILE_FUNCTION();

	#if GBC_ENABLE_STATS
		BasicRenderer::ResetStatistics();
	#endif

		framebuffer->Bind();

		Renderer::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
		Renderer::Clear();
		framebuffer->ClearColorAttachment(-1, 1);

		scene->OnRenderEditor(editorCamera);

		framebuffer->Unbind();
	}

#if GBC_ENABLE_IMGUI
	void EditorLayer::OnImGuiRender()
	{
		GBC_PROFILE_FUNCTION();

		ImGuiWindowFlags dockspaceFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
		ImGui::Begin("Dockspace", nullptr, dockspaceFlags);
		ImGui::PopStyleVar(2);

		ImGuiStyle& style = ImGui::GetStyle();
		float prevWindowMinWidth = style.WindowMinSize.x;
		style.WindowMinSize.x = 200.0f;
		ImGui::DockSpace(ImGui::GetID("Dockspace"));
		style.WindowMinSize.x = prevWindowMinWidth;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();
				if (ImGui::MenuItem("Save...", "Ctrl+S"))
					SaveScene();
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveAsScene();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Alt+F4"))
					Application::Get().Terminate();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				Window& window = Application::Get().GetWindow();
				if (ImGui::MenuItem(window.IsFullscreen() ? "Windowed" : "Fullscreen"))
					window.ToggleFullscreen();

				ImGui::Separator();

				for (auto& [name, panel] : panels)
					if (ImGui::MenuItem(name.c_str()))
						panel->ToggleEnabled();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		ImGui::End();

		for (auto& [name, panel] : panels)
			panel->OnImGuiRender();
	}
#endif

	void EditorLayer::OnEvent(Event& event)
	{
		GBC_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(GBC_BIND_FUNC(OnWindowCloseEvent));
		dispatcher.Dispatch<KeyPressEvent>(GBC_BIND_FUNC(OnKeyPressEvent));
		dispatcher.Dispatch<MouseButtonPressEvent>(GBC_BIND_FUNC(OnMouseButtonPressEvent));

		if (!event.handled)
			editorCamera.OnEvent(event);
	}

	bool EditorLayer::OnWindowCloseEvent(WindowCloseEvent& event)
	{
		bool allowedDiscard = true;
		if (hasUnsavedChanges)
		{
			// TODO: Show "Any unsaved work will be lost!" message
			//allowedDiscard = false;
		}

		return !allowedDiscard;
	}

	bool EditorLayer::OnKeyPressEvent(KeyPressEvent& event)
	{
		bool controlPressed = Input::IsKeyPressed(Keycode::LeftControl) || Input::IsKeyPressed(Keycode::RightControl);
		bool shiftPressed = Input::IsKeyPressed(Keycode::LeftShift) || Input::IsKeyPressed(Keycode::RightShift);
		bool altPressed = Input::IsKeyPressed(Keycode::LeftAlt) || Input::IsKeyPressed(Keycode::RightAlt);

		bool control         =  controlPressed && !shiftPressed && !altPressed;
		bool shift           = !controlPressed &&  shiftPressed && !altPressed;
		bool controlShift    =  controlPressed &&  shiftPressed && !altPressed;
		bool alt             = !controlPressed && !shiftPressed &&  altPressed;
		bool controlAlt      =  controlPressed && !shiftPressed &&  altPressed;
		bool shiftAlt        = !controlPressed &&  shiftPressed &&  altPressed;
		bool controlShiftAlt =  controlPressed &&  shiftPressed &&  altPressed;

		switch (event.GetKeycode())
		{
			// Scene
			case Keycode::N:
				if (control)
					NewScene();
				break;
			case Keycode::O:
				if (control)
					OpenScene();
				break;
			case Keycode::S:
				if (control || controlShift)
				{
					if (shiftPressed) SaveAsScene();
					else SaveScene();
				}
				break;
			// Gizmos
			case Keycode::Q:
				gizmoType = -1;
				break;
			case Keycode::W:
				gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Keycode::E:
				gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Keycode::R:
				gizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressEvent(MouseButtonPressEvent& event)
	{
		if (viewportHovered && viewportFocused && event.GetButton() == Mousecode::ButtonLeft && !Input::IsKeyPressed(Keycode::LeftAlt) && !ImGuizmo::IsOver())
		{
			glm::ivec2 relativeMousePos = absoluteMousePos - viewportPos;
			relativeMousePos.y = viewportSize.y - 1 - relativeMousePos.y;

			if (relativeMousePos.x >= 0 && relativeMousePos.x < viewportSize.x &&
				relativeMousePos.y >= 0 && relativeMousePos.y < viewportSize.y)
			{
				int pixel = -1;
				framebuffer->Bind();
				framebuffer->GetColorPixel(&pixel, relativeMousePos.x, relativeMousePos.y, 1);
				framebuffer->Unbind();

				selectedEntity = pixel != -1 ? Entity(static_cast<entt::entity>(pixel), scene.get()) : Entity();
				return true;
			}
		}
		return false;
	}

	void EditorLayer::ClearScene()
	{
		glm::ivec2 viewportSize = scene->GetViewportSize();
		scene = CreateRef<Scene>();
		scene->OnViewportResize(viewportSize.x, viewportSize.y);
		selectedEntity = {};
	}

	void EditorLayer::NewScene()
	{
		bool allowedDiscard = true;
		if (hasUnsavedChanges)
		{
			// TODO: Show "Any unsaved work will be lost!" message
			//allowedDiscard = false;
		}

		if (allowedDiscard)
		{
			currentFilepath = std::string();
			ClearScene();
		}
	}

	void EditorLayer::OpenScene()
	{
		bool allowedDiscard = true;
		if (hasUnsavedChanges)
		{
			// TODO: Show "Any unsaved work will be lost!" message
			//allowedDiscard = false;
		}

		if (allowedDiscard)
		{
			auto initialDirectory = std::filesystem::absolute("./").string();
			std::string filepath = FileDialog::OpenFile(initialDirectory.c_str(), "GBC Scene (*.gscn)\0*.gscn\0");
			if (!filepath.empty())
			{
				currentFilepath = filepath;
				ClearScene();
				SceneSerializer serializer(scene);
				serializer.Deserialize(filepath);
			}
		}
	}

	void EditorLayer::SaveScene()
	{
		if (!currentFilepath.empty())
		{
			hasUnsavedChanges = false;

			SceneSerializer serializer(scene);
			serializer.Serialize(currentFilepath);
		}
		else
			SaveAsScene();
	}

	void EditorLayer::SaveAsScene()
	{
		auto initialDirectory = std::filesystem::absolute("./").string();
		std::string filepath = FileDialog::SaveFile(initialDirectory.c_str(), "GBC Scene (*.gscn)\0*.gscn\0");
		if (!filepath.empty())
		{
			// Add extension to extensionless path
			size_t index = filepath.find_last_of(".gscn");
			if (index != filepath.size() - 1)
				filepath += ".gscn";

			currentFilepath = filepath;
			SaveScene();
		}
	}
}
