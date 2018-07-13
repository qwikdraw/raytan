#pragma once

#include "Raetan.hpp"

class	ShadingProgram
{
private:

	GLuint _program;
	std::vector<GLuint> _uniformIDs;
	ShadingProgram(void);

	std::string	GetShaderCode(std::string filepath);
	void	CheckCompilation(GLuint, std::string filepath);
	void	CheckLinking(void);
	
public:
	ShadingProgram(std::string vertexPath, std::string geoPath, std::string fragPath);
	void	Use(void);
	GLuint	ID(void);
};
