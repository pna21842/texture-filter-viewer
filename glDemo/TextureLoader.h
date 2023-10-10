#pragma once

#include "core.h"
#include "TextureProperties.h"

enum CGMipmapGenMode {
	
	CG_NO_MIPMAP_GEN = 0,
	CG_CORE_MIPMAP_GEN,
	CG_EXT_MIPMAP_GEN
};

// FreeImage texture loader
GLuint fiLoadTexture(std::string filename, FREE_IMAGE_FORMAT fileType, const TextureProperties& properties);
