#pragma once

#include "core.h"

class PrincipleAxesModel {

private:

	GLuint					paVertexArrayObj;

	GLuint					paVertexBuffer;
	GLuint					paColourBuffer;

	GLuint					paIndexBuffer;

	GLuint					paShader;

public:

	PrincipleAxesModel();

	~PrincipleAxesModel();

	void render(const glm::mat4& T);
};


