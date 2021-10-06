#pragma once

#include "GBC/Model/Mesh3D.h"
#include "GBC/Rendering/Camera.h"
#include "GBC/Rendering/EditorCamera.h"
#include "GBC/Rendering/Texture.h"

namespace gbc
{
	class Renderer3D
	{
	public:
		static void BeginScene(const Camera& camera, const glm::mat4& view);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
	public:
		static void Submit(const Ref<Mesh3D>& mesh, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f), const Ref<Texture2D>& texture = nullptr);
	private:
		static void Reset();

		static uint32_t GetTexIndex(const Ref<Texture2D>& texture);
		static void EnsureBatch(uint32_t vertexCount, uint32_t indexCount, uint32_t texIndex);
	private:
		friend class Renderer;
		static void Init();
		static void Shutdown();
	public:
#if GBC_ENABLE_STATS
		struct Statistics
		{
		public:
			inline uint32_t GetDrawCallCount() const noexcept { return drawCallCount; }
			inline uint32_t GetVertexCount() const noexcept { return vertexCount; }
			inline uint32_t GetIndexCount() const noexcept { return indexCount; }
			inline uint32_t GetMeshCount() const noexcept { return meshCount; }
			inline uint32_t GetTextureCount() const noexcept { return textureCount; }
		private:
			friend class Renderer3D;
			uint32_t drawCallCount = 0;
			uint32_t vertexCount = 0;
			uint32_t indexCount = 0;
			uint32_t meshCount = 0;
			uint32_t textureCount = 0;
		};

		static const Statistics& GetStatistics();
		static void ResetStatistics();
#endif
	};
}
