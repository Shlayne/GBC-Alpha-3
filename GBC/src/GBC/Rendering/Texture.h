#pragma once

#include "LocalTexture2D.h"
#include "Framebuffer.h"

namespace gbc
{
	enum class TextureFormat
	{
		None = 0,

		RGB8,
		RGBA8
	};

	enum class TextureFilterMode
	{
		Linear,
		Nearest
	};

	enum class TextureWrapMode
	{
		ClampToEdge,
		Repeat
	};

	struct TextureSpecification
	{
		TextureSpecification() = default;
		TextureSpecification(const Ref<LocalTexture2D>& texture)
			: texture(texture) {}
		TextureSpecification(const Ref<LocalTexture2D>& texture, TextureFilterMode minFilter, TextureFilterMode magFilter, TextureWrapMode wrapS, TextureWrapMode wrapT)
			: texture(texture), minFilter(minFilter), magFilter(magFilter), wrapS(wrapS), wrapT(wrapT) {}
		TextureSpecification(TextureFilterMode minFilter, TextureFilterMode magFilter, TextureWrapMode wrapS, TextureWrapMode wrapT)
			: minFilter(minFilter), magFilter(magFilter), wrapS(wrapS), wrapT(wrapT) {}
		TextureSpecification(const TextureSpecification& specs)
			: texture(specs.texture), minFilter(specs.minFilter), magFilter(specs.magFilter), wrapS(specs.wrapS), wrapT(specs.wrapT) {}
		TextureSpecification& operator=(const TextureSpecification& specs);

		Ref<LocalTexture2D> texture = nullptr;
		TextureFilterMode minFilter = TextureFilterMode::Linear;
		TextureFilterMode magFilter = TextureFilterMode::Linear;
		TextureWrapMode wrapS = TextureWrapMode::ClampToEdge;
		TextureWrapMode wrapT = TextureWrapMode::ClampToEdge;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		virtual RendererID GetRendererID() const = 0;
		virtual const Ref<LocalTexture2D>& GetTexture() const = 0;
		virtual const TextureSpecification& GetSpecification() const = 0;

		// Call this when you want to update the internal
		// texture after the the local texture has updated.
		// Only call if this was not constructed with a Framebuffer.
		virtual void Update() = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> CreateRef(const TextureSpecification& specification);
		static Scope<Texture2D> CreateScope(const TextureSpecification& specification);

		static Ref<Texture2D> CreateRef(const TextureSpecification& specification, const Ref<Framebuffer>& framebuffer, int attachmentIndex);
		static Scope<Texture2D> CreateScope(const TextureSpecification& specification, const Ref<Framebuffer>& framebuffer, int attachmentIndex);
	};
}
