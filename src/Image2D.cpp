#include "Image2D.hpp"

const	std::vector<float> Image2D::_vertexArray = {1, -1, 0,
						    -1, -1, 0,
						    1, 1, 0,
						    -1, 1, 0,
						    -1, -1, 0,
						    1, 1, 0};

const	std::vector<float> Image2D::_uvArray = {1, 0,
						0, 0,
						1, 1,
						0, 1,
						0, 0,
						1, 1};

Image2D::Image2D(void)
{
	_program = new ShadingProgram(SCREENIMAGE_VERTEX_SHADER_PATH, "",
				      SCREENIMAGE_FRAGMENT_SHADER_PATH);
	_textureLocationID = glGetUniformLocation(_program->ID(), "tex");
	_textureID = 0;
	glUseProgram(_program->ID());

	glGenBuffers(1, &_vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayID);
	glBufferData(GL_ARRAY_BUFFER,
		     sizeof(GLfloat) * _vertexArray.size(),
		     &_vertexArray[0],
		     GL_STATIC_DRAW);

	glGenBuffers(1, &_uvArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, _uvArrayID);
	glBufferData(GL_ARRAY_BUFFER,
		     sizeof(GLfloat) * _uvArray.size(),
		     &_uvArray[0],
		     GL_STATIC_DRAW);
}

Image2D::~Image2D(void)
{
	glDeleteTextures(1, &_textureID);
	glDeleteBuffers(1, &_vertexArrayID);
	glDeleteBuffers(1, &_uvArrayID);
	delete _program;
}

void	Image2D::Render(const std::vector<unsigned char>& data, size_t width, size_t height)
{
	glUseProgram(_program->ID());

	glDeleteTextures(1, &_textureID);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D,
		     0,
		     GL_RGBA,
		     width,
		     height,
		     0,
		     GL_RGBA,
		     GL_UNSIGNED_BYTE,
		     &data[0]);

	Render();
}

void	Image2D::Render(void)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, _textureID);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(_textureLocationID, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _uvArrayID);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, _vertexArray.size() / 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}
