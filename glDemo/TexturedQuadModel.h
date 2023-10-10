#pragma once

#include "core.h"
#include "TextureProperties.h"

// Model a simple textured quad oriented to face along the +z axis (so the textured quad faces the viewer in (right-handed) eye coordinate space.  The quad is modelled using VBOs and VAOs and rendered using the basic texture shader in Resources\Shaders\basic_texture.vs and Resources\Shaders\basic_texture.fs

class TexturedQuadModel {

private:

	GLuint					quadVertexArrayObj;

	GLuint					quadVertexBuffer;
	GLuint					quadTextureCoordBuffer;

	GLuint					quadShader;

	GLuint					texture;

	//
	// Private API
	//

	void loadShader();
	void setupVAO();


	//
	// Public API
	//

public:

	TexturedQuadModel(std::string filename, FREE_IMAGE_FORMAT fileType, const TextureProperties& properties);
	TexturedQuadModel(GLuint texture);

	~TexturedQuadModel();

	GLuint getTexture();

	void render(const glm::mat4& T);
};
