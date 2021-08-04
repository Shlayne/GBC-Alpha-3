#include "SceneViewportPanel.h"
#include "imgui/imgui.h"
#include "imguizmo/ImGuizmo.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"
#include "GBC/Math/Math.h"
#include "glm/gtx/matrix_decompose.hpp"
#include "GBC/Core/Application.h"
#include "GBC/Core/input.h"

namespace gbc
{
	SceneViewportPanel::SceneViewportPanel(const std::string& name, Ref<Framebuffer>& framebuffer, Ref<Scene>& context, Entity& selectedEntity, int& gizmoType, bool& canUseGizmos, EditorCamera& editorCamera, const OpenSceneFunc& openScene)
		: Panel(name), framebuffer(framebuffer), context(context), selectedEntity(selectedEntity), gizmoType(gizmoType), canUseGizmos(canUseGizmos), editorCamera(editorCamera), openScene(openScene) {}

	void SceneViewportPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
			ImGui::Begin(name.c_str(), &enabled);
			ImGui::PopStyleVar();
			Update();

			auto textureID = (void*)static_cast<size_t>(framebuffer->GetColorAttachment());
			auto textureSize = ImVec2(static_cast<float>(size.x), static_cast<float>(size.y));
			ImGui::Image(textureID, textureSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					std::string filepath = static_cast<const char*>(payload->Data);
					if (filepath.ends_with(".gscn"))
						openScene(filepath);
				}
				ImGui::EndDragDropTarget();
			}

			// Gizmos
			if (selectedEntity && gizmoType != -1)
			{
				// TODO: orthographic projections
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				// Get window info
				ImGuizmo::SetRect(position.x, position.y, static_cast<float>(size.x), static_cast<float>(size.y));

				// Get camera info
				//Entity cameraEntity = context->GetPrimaryCameraEntity();
				//const auto& camera = cameraEntity.GetComponent<CameraComponent>().camera;
				//const glm::mat4& projection = camera.GetProjection();
				//glm::mat4 view = glm::inverse((glm::mat4)cameraEntity.GetComponent<TransformComponent>());

				// Get editor camera info
				glm::mat4 view = editorCamera.GetView();
				glm::mat4 projection = editorCamera.GetProjection();
				
				// Get entity info
				TransformComponent& transformComponent = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = transformComponent;

				// Get snapping info
				bool snap = Input::IsKeyPressed(Keycode::LeftControl);
				float snapValue = 0.5f;
				if (gizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;
				float snapValues[3] {snapValue, snapValue, snapValue};

				// Draw gizmos
				if (ImGuizmo::Manipulate(&view[0].x, &projection[0].x, static_cast<ImGuizmo::OPERATION>(gizmoType),
					ImGuizmo::MODE::LOCAL, &transform[0].x, nullptr, snap ? snapValues : nullptr) && canUseGizmos)
				{
					glm::vec3 translation, rotation, scale;
					if (Math::Decompose(transform, translation, rotation, scale))
					{
						transformComponent.translation = translation;
						transformComponent.rotation = rotation;
						transformComponent.scale = scale;
					}
				}
			}

			ImGui::End();
		}
	}
}
