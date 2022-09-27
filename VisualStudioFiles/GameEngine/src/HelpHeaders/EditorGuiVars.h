#pragma once


#include "TextureHandeling/Texture_H.h"
#include "RandomProps.h"

class EditorGuiVars {

public:

	EditorGuiVars() {

		instance = this;
	}

	inline static EditorGuiVars* Inst() {
		return instance;
	}

	void SetTextures() {

		contentBrowserFolderIcon = Texture::TextureFromFile("Assets/EditorResources/Icons/ContentBrowser/Folder.png", GL_TEXTURE_2D, TEXTURE_NO_VERTICAL_FLIP);
		contentBrowserFileIcon = Texture::TextureFromFile("Assets/EditorResources/Icons/ContentBrowser/File.png", GL_TEXTURE_2D, TEXTURE_NO_VERTICAL_FLIP);

	}

	void SetAsInstance() {
		instance = this;
	}

	std::filesystem::path contentBrowserCurrentPath = RandomProps::assetsDirectoryPath;


	float contentBrowserpadding = 32.0f;
	float contentBrowserthumbnailSize = 50.0f;

	Texture* contentBrowserFolderIcon = nullptr;
	Texture* contentBrowserFileIcon = nullptr;

private:
	inline static EditorGuiVars* instance;
};