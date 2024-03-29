#include "gbcpch.h"
#include "FileTypes.h"

namespace gbc
{
	Scope<FilterGenerator> FilterGenerator::generator = nullptr;

	FilterGenerator& FilterGenerator::Get()
	{
		if (!generator)
			generator = CreateScope<FilterGenerator>();
		return *generator;
	}

	FilterGenerator::FilterGenerator()
	{
		// Generic filters
		CreateFilter(L"Texture", FileType::textures);
		CreateFilter(L"3D Model", FileType::model3Ds);

		// GBC filters
		CreateFilter(L"GBC Project", FileType::gProject);
		CreateFilter(L"GBC Scene", FileType::gScene);
		CreateFilter(L"GBC 3D Model", FileType::gModel3D);
		CreateFilter(L"GBC 4D Model", FileType::gModel4D);
		CreateFilter(L"GBC Texture", FileType::gTexture);
	}

	FilterGenerator::~FilterGenerator()
	{
		for (auto [extension0, filter] : filters)
			delete[] filter;
	}

	void FilterGenerator::CreateFilter(std::wstring_view fileTypeName, std::wstring_view extension) noexcept
	{
		wchar_t* filter = new wchar_t[fileTypeName.size() + 7 + extension.size() * 2 + 1];

		std::memcpy(filter, fileTypeName.data(), fileTypeName.size() * sizeof(wchar_t));
		size_t offset = fileTypeName.size();
		filter[offset] = L' ';
		filter[++offset] = L'(';
		filter[++offset] = L'*';
		std::memcpy(filter + ++offset, extension.data(), extension.size() * sizeof(wchar_t));
		filter[offset += extension.size()] = L')';
		filter[++offset] = L'\0';

		filter[++offset] = L'*';
		std::memcpy(filter + ++offset, extension.data(), extension.size() * sizeof(wchar_t));
		filter[offset += extension.size()] = L'\0';
		filter[++offset] = L'\0';

		filters[extension] = filter;
	}

	const wchar_t* FilterGenerator::GetFilter(std::wstring_view extension) noexcept
	{
		if (auto it = filters.find(extension); it != filters.end())
			return it->second;
		return nullptr;
	}

	const wchar_t* GetFilter(std::wstring_view extension) noexcept
	{
		return FilterGenerator::Get().GetFilter(extension);
	}

	std::filesystem::path AppendType(const std::filesystem::path& filepath, std::wstring_view extension)
	{
		return filepath.native() + extension.data();
	}

	std::filesystem::path& AppendTypeTo(std::filesystem::path& filepath, std::wstring_view extension)
	{
		return filepath = filepath.native() + extension.data();
	}

	// Simply those alias' definitions
#define FILE_EXTENSION_DEFINITION(name, exts) \
	std::filesystem::path Append##name##Type(const std::filesystem::path& filepath) { return AppendType(filepath, exts); } \
	std::filesystem::path& Append##name##TypeTo(std::filesystem::path& filepath) { return AppendTypeTo(filepath, exts); } \
	const wchar_t* Get##name##Filter() noexcept { return GetFilter(exts); }

	// Generic extensions
	FILE_EXTENSION_DEFINITION(Texture, FileType::textures);
	FILE_EXTENSION_DEFINITION(3DModel, FileType::model3Ds);

	// GBC extensions
	FILE_EXTENSION_DEFINITION(GProject, FileType::gProject);
	FILE_EXTENSION_DEFINITION(GScene, FileType::gScene);
	FILE_EXTENSION_DEFINITION(G3DModel, FileType::gModel3D);
	FILE_EXTENSION_DEFINITION(G4DModel, FileType::gModel4D);
	FILE_EXTENSION_DEFINITION(GTexture, FileType::gTexture);
}
