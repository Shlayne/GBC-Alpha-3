#pragma once

// For use by client applications

#include "GBC/Core/Application.h"
#include "GBC/Core/Assert.h"
#include "GBC/Core/AssetManager.h"
#include "GBC/Core/Core.h"
#include "GBC/Core/Debugbreak.h"
#include "GBC/Core/FileTypes.h"
#include "GBC/Core/Input.h"
#include "GBC/Core/Joysticks.h"
#include "GBC/Core/Keycodes.h"
#include "GBC/Core/Layer.h"
#include "GBC/Core/Logger.h"
#include "GBC/Core/MouseButtons.h"
#include "GBC/Core/Timestep.h"
#include "GBC/Core/UUID.h"
#include "GBC/Core/Window.h"

#include "GBC/Debug/Profiler.h"

#include "GBC/Events/DeviceEvents.h"
#include "GBC/Events/Event.h"
#include "GBC/Events/KeyEvents.h"
#include "GBC/Events/Mods.h"
#include "GBC/Events/MouseEvents.h"
#include "GBC/Events/WindowEvents.h"

#include "GBC/ImGui/ImGuiHelper.h"

#include "GBC/IO/CommonYAML.h"
#include "GBC/IO/DirectoryChange.h"
#include "GBC/IO/FileDialog.h"
#include "GBC/IO/FileIO.h"

#include "GBC/Math/Math.h"

#include "GBC/Model/Mesh3D.h"
#include "GBC/Model/MeshFactory3D.h"

#include "GBC/Rendering/Buffer.h"
#include "GBC/Rendering/Camera.h"
#include "GBC/Rendering/Context.h"
#include "GBC/Rendering/Framebuffer.h"
#include "GBC/Rendering/LocalTexture2D.h"
#include "GBC/Rendering/Renderer.h"
#include "GBC/Rendering/RendererAPI.h"
#include "GBC/Rendering/RendererCapabilities.h"
#include "GBC/Rendering/Shader.h"
#include "GBC/Rendering/Texture.h"
#include "GBC/Rendering/VertexArray.h"
#include "GBC/Rendering/Renderers/Renderer2D.h"
#include "GBC/Rendering/Renderers/Renderer3D.h"

#include "GBC/Scene/Entity.h"
#include "GBC/Scene/Scene.h"
#include "GBC/Scene/SceneCamera.h"
#include "GBC/Scene/SceneSerializer.h"
#include "GBC/Scene/ScriptableEntity.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/IDComponent.h"
#include "GBC/Scene/Components/Model3DComponent.h"
#include "GBC/Scene/Components/NativeScriptComponent.h"
#include "GBC/Scene/Components/SpriteRendererComponent.h"
#include "GBC/Scene/Components/TagComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"
#include "GBC/Scene/Components/Physics/BoxCollider2DComponent.h"
#include "GBC/Scene/Components/Physics/Rigidbody2DComponent.h"

#include "GBC/Util/TextureAtlas.h"
#include "GBC/Util/Timer.h"
