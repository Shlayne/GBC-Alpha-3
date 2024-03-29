#pragma once

#include <cstdint>
#include <filesystem>

namespace gbc
{
	class LocalTexture2D
	{
	public:
		constexpr LocalTexture2D() = default;
		LocalTexture2D(const std::filesystem::path& filepath, int32_t requiredChannels = 0);
		LocalTexture2D(const std::filesystem::path& filepath, bool flipVertically, int32_t requiredChannels = 0);
		LocalTexture2D(int32_t width, int32_t height, int32_t channels);
		LocalTexture2D(const LocalTexture2D& texture);
		LocalTexture2D(LocalTexture2D&& texture) noexcept;
		LocalTexture2D& operator=(const LocalTexture2D& texture);
		LocalTexture2D& operator=(LocalTexture2D&& texture) noexcept;
		~LocalTexture2D();

		bool ReadFile(const std::filesystem::path& filepath, int32_t requiredChannels = 0);
		bool ReadFile(const std::filesystem::path& filepath, bool flipVertically, int32_t requiredChannels = 0);
		bool WriteFile(const std::filesystem::path& filepath);
		bool WriteFile(const std::filesystem::path& filepath, bool flipVertically);

		bool SetSubregion(const LocalTexture2D& texture, int32_t positionX, int32_t positionY);

		constexpr const std::filesystem::path& GetFilepath() const noexcept { return filepath; }
		constexpr int32_t GetWidth() const noexcept { return width; }
		constexpr int32_t GetHeight() const noexcept { return height; }
		constexpr int32_t GetChannels() const noexcept { return channels; }
		constexpr uint8_t* GetData() const noexcept { return data; }

		constexpr operator bool() const noexcept { return data != nullptr; }
		constexpr bool operator==(const LocalTexture2D& texture) const noexcept { return data == texture.data; }

		static Ref<LocalTexture2D> Create();
		static Ref<LocalTexture2D> Create(const std::filesystem::path& filepath, int32_t requiredChannels = 0);
		static Ref<LocalTexture2D> Create(const std::filesystem::path& filepath, bool flipVertically, int32_t requiredChannels = 0);
		static Ref<LocalTexture2D> Create(int32_t width, int32_t height, int32_t channels);
	private:
		std::filesystem::path filepath;

		int32_t width = 0;
		int32_t height = 0;
		int32_t channels = 0;
		uint8_t* data = nullptr;
	};
}
