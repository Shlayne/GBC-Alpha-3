#pragma once

#include "Panels/Panel.h"
#include "GBC/IO/DirectoryChange.h"
#include "GBC/Rendering/Texture.h"
#include <filesystem>
#include <list>
#include <vector>

namespace gbc
{
	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel(const std::string& name, EditorLayer* editorLayer);

		virtual void OnImGuiRender() override;
	private:
		struct File
		{
			std::filesystem::path path;
			bool isDirectory = false;
		};

		struct Directory
		{
			std::filesystem::path path;
			std::vector<Directory> subdirectories;
		};
	private:
		void DrawHierarchy(const Directory& isDirectory);
		void DrawSearchBar();
		void DrawExplorer();

		void ForwardDirectory();
		void BackwardDirectory();
		void PushDirectory(const std::filesystem::path& path);
		void RefreshDirectories();
		void RefreshDirectory(Directory& subdirectory);
	private:
		void ShowDeleteConfirmationMessage();
		void MakeCurrentItemDragDropSource(const std::filesystem::path& sourcePath);
		void MakeCurrentItemDragDropTarget(const std::filesystem::path& targetPath);
	private:
		struct FileButton
		{
			const std::filesystem::path& filepath;
			glm::ivec2 position;
		};

		void BeginDragSelect();
		void UpdateDragSelect();
		void EndDragSelect(const std::vector<FileButton>& fileButtons, const glm::ivec2& buttonSize);
	public:
		inline bool IsDragSelectActive() const noexcept { return dragSelectActive; }
		void GetDragSelectBounds(glm::ivec2& topLeft, glm::ivec2& bottomRight) const;
		inline const std::list<std::filesystem::path>& GetSelectedFiles() const noexcept { return selectedFiles; }

		inline bool HasExplorerSizeChanged() const noexcept { return explorerSizeChanged; }
		inline const glm::ivec2& GetExplorerSize() const noexcept { return explorerSize; }
		inline const glm::ivec2& GetExplorerPosition() const noexcept { return explorerPosition; }
	private:
		Directory assetDirectory;

		// Directory change notifications
		DirectoryChange::Notifier notifier;
		bool OnDirectoryNotification(bool error);

		// Update flags
		bool forwardDirectory = false;
		bool backwardDirectory = false;
		bool pushDirectory = false;
		bool refreshDirectories = true;

		bool creatingDirectory = false;
		bool renamingFile = false;
		bool deletingFile = false;
		bool hierarchyOptionsOpen = false;
		bool showDeleteOption = true;

		// Temp data
		File tempFile;

		// Cached directories
		static constexpr size_t maxCachedDirectories = 16;
		std::list<std::filesystem::path> cachedDirectories;
		std::list<std::filesystem::path>::iterator currentCachedDirectory;
		std::string currentCachedDirectoryText;

		// File explorer
		std::vector<File> files;
		size_t clickedFileIndex = 0;

		// Renaming
		const char* GetNewFolderName(size_t& outSize) const;
		void CalculateFileNameBufferSize(bool isDirectory);

		size_t fileNameSize = 0;
		char fileNameBuffer[__std_fs_max_path];
		size_t fileNameBufferSize = 0;

		// Searching
		size_t searchSize = 0;
		static constexpr size_t searchBufferSize = __std_fs_max_path;
		char searchBuffer[searchBufferSize]{ '\0' };

		// Drag select
		std::list<std::filesystem::path> selectedFiles;

		bool dragSelectActive = false;
		glm::ivec2 dragSelectStart{ 0 };
		glm::ivec2 dragSelectCurrent{ 0 };

		bool explorerSizeChanged = false;
		glm::ivec2 explorerSize{ 1 };
		glm::ivec2 explorerPosition{ 0 };

		// Resources
		Ref<Texture2D> directoryTexture;
		Ref<Texture2D> fileTexture;
	};
}