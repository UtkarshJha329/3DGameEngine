#include "../HelpHeaders/TextureHandeling/Texture_H.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../HelpHeaders/TextureHandeling/stb_image.h"

Texture::Texture(GLenum target, int width, int height, bool generateMipMaps) {

	data = nullptr;
	type = non_image;

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT,
		NULL);
	glBindImageTexture(0, ID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	if(generateMipMaps)
		glGenerateMipmap(target);
}

Texture::Texture(std::string pathToImage, GLenum target, GLint colourSpace, int flipVert) {
	//std::cout << "LODING IMAGE FROM: " << pathToImage << std::endl;
	type = missing;
	if (flipVert == 1) {
		stbi_set_flip_vertically_on_load(true);
	}
	data = stbi_load(pathToImage.c_str(), &width, &height, &nColorChannels, 0);
	if (data) {
		texturePath = pathToImage;
		// Generate and bind texture for current use.
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Fill the bound texture with data we got from the file provided.
		glTexImage2D(target, 0, GL_RGB, width, height, 0, colourSpace, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(target);
	}
	else {
		std::cout << "FAILED TO LOAD IN TEXTURE FROM: " << pathToImage << std::endl;
		ID = -1;
		texturePath = "NOT_FOUND";
	}
	stbi_image_free(data);
}

Texture* Texture::TextureFromFile(std::string pathToImage, GLenum target, int flipVert)
{
	Texture* newTex = new Texture();
	//std::cout << "LODING IMAGE FROM: " << pathToImage << std::endl;
	if (flipVert == TEXTURE_FLIP_VERTICAL) {
		stbi_set_flip_vertically_on_load(true);
	}
	newTex->data = stbi_load(pathToImage.c_str(), &newTex->width, &newTex->height, &newTex->nColorChannels, 0);
	if (newTex->data) {
		newTex->texturePath = pathToImage.c_str();
		// Generate and bind texture for current use.
		glGenTextures(1, &newTex->ID);
		glBindTexture(GL_TEXTURE_2D, newTex->ID);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		GLenum format;
		if (newTex->nColorChannels == 1)
			format = GL_RED;
		else if (newTex->nColorChannels == 3)
			format = GL_RGB;
		else if (newTex->nColorChannels == 4)
			format = GL_RGBA;

		// Fill the bound texture with data we got from the file provided.
		glTexImage2D(target, 0, format, newTex->width, newTex->height, 0, format, GL_UNSIGNED_BYTE, newTex->data);
		glGenerateMipmap(target);
	}
	else {
		std::cout << "FAILED TO LOAD IN TEXTURE FROM: " << pathToImage << std::endl;
		newTex->ID = -1;
		newTex->texturePath = "NOT_FOUND";
	}
	stbi_image_free(newTex->data);
	stbi_set_flip_vertically_on_load(false);
	return newTex;
}

Texture* Texture::TextureFromFile(std::string pathToImage, std::string directory, GLenum target, int flipVert)
{
	std::string path = directory.append("/").append(pathToImage);
	return TextureFromFile(path, target, flipVert);
}
