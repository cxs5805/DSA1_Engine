#include "Texture.h"

Texture::Texture()
{
	//load();
}


Texture::~Texture()
{
	//unload();
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::unload()
{
	glDeleteTextures(1, &texID);
}

void Texture::load()
{
	// load texture
	const char* texFileCstr = texFile.c_str();

	// DEBUG
	std::cout << "Start of file\n";
	std::cout << texFileCstr;
	std::cout << "\nEnd of file\n-\n";
	
	FREE_IMAGE_FORMAT type = FreeImage_GetFileType(texFileCstr, 0);
	FIBITMAP* image = FreeImage_Load(type, texFileCstr);
	
	// DEBUG
	//void* test = image->data;
	
	// check if loaded
	if (image == nullptr)
		std::cout << "Image didn't load :/";

	// convert to 32bit bitmap
	FIBITMAP* image32Bit = FreeImage_ConvertTo32Bits(image);

	// check if loaded
	if (image32Bit == nullptr)
		std::cout << "Something wrong with bitmap :/";

	// unload image from memory
	FreeImage_Unload(image);

	// generate and bind new texture
	glGenTextures(1, &texID);
	bind();

	// upload texture to graphics card
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_SRGB_ALPHA,
		FreeImage_GetWidth(image32Bit), // width
		FreeImage_GetHeight(image32Bit), // height
		0,
		GL_BGRA,
		GL_UNSIGNED_BYTE,
		(void*)FreeImage_GetBits(image32Bit)); // address

	// set min filter to linear instead of mipmap linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// unload texture from RAM
	FreeImage_Unload(image32Bit);

	// unbind texture as uploading is done
	glBindTexture(GL_TEXTURE_2D, 0);
	//unload();
}
