
#include "core.h"
#include "TextureLoader.h"

using namespace std;

#pragma region MipMap processing

static CGMipmapGenMode mipmapGenMode = CG_NO_MIPMAP_GEN;
static bool mipmapModeInitialised = false;

static void initialiseMipmapMode() {

	if (glewIsSupported("GL_ARB_framebuffer_object"))
		mipmapGenMode = CG_CORE_MIPMAP_GEN;
	else if (glewIsSupported("GL_EXT_framebuffer_object"))
		mipmapGenMode = CG_EXT_MIPMAP_GEN;
	else
		mipmapGenMode = CG_NO_MIPMAP_GEN;

	mipmapModeInitialised = true;
}

#pragma endregion


#pragma region FreeImagePlus texture loader

GLuint fiLoadTexture(string filename, FREE_IMAGE_FORMAT fileType, const TextureProperties& properties) {

	GLuint				newTexture = 0;
	
	FIBITMAP* loadedBitmap = FreeImage_Load(fileType, filename.c_str(), BMP_DEFAULT);

	if (!loadedBitmap) {

		cout << "FreeImage: Cannot open image file " << filename << endl;
		return 0;
	}

	if (properties.flipImageY) {

		FreeImage_FlipVertical(loadedBitmap);
	}

	FIBITMAP* bitmap32bpp = FreeImage_ConvertTo32Bits(loadedBitmap);
	FreeImage_Unload(loadedBitmap);

	if (!bitmap32bpp) {

		cout << "FreeImage: Conversion to 32 bits successful for image " << filename << endl;
		return 0;
	}

	GLuint w = FreeImage_GetWidth(bitmap32bpp);
	GLuint h = FreeImage_GetHeight(bitmap32bpp);
	BYTE* buffer = FreeImage_GetBits(bitmap32bpp);

	if (!buffer) {

		FreeImage_Unload(bitmap32bpp);

		cout << "FreeImage: Cannot access bitmap data for image " << filename << endl;
		return 0;
	}

	glGenTextures(1, &newTexture);
	glBindTexture(GL_TEXTURE_2D, newTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, properties.internalFormat, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, buffer);

	// Setup default texture properties
	if (newTexture) {

		// Verify we don't use GL_LINEAR_MIPMAP_LINEAR which has no meaning in non-mipmapped textures.  If not set, default to GL_LINEAR (bi-linear) filtering.
		GLint minFilter = (!properties.genMipMaps && properties.minFilter == GL_LINEAR_MIPMAP_LINEAR) ? GL_LINEAR : properties.minFilter;
		GLint maxFilter = (!properties.genMipMaps && properties.maxFilter == GL_LINEAR_MIPMAP_LINEAR) ? GL_LINEAR : properties.maxFilter;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, properties.anisotropicLevel);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, properties.wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, properties.wrap_t);

		// Initialise mipmap creation method based on supported extensions
		if (!mipmapModeInitialised)
			initialiseMipmapMode();

		if (properties.genMipMaps && mipmapGenMode != CG_NO_MIPMAP_GEN) {

			if (mipmapGenMode == CG_CORE_MIPMAP_GEN)
				glGenerateMipmap(GL_TEXTURE_2D);
			else if (mipmapGenMode == CG_EXT_MIPMAP_GEN)
				glGenerateMipmapEXT(GL_TEXTURE_2D);
		}
	}

	// Cleanup resources
	FreeImage_Unload(bitmap32bpp);

	// Return texture ID
	return newTexture;
}

#pragma endregion

