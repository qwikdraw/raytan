#include "ShadingProgram.hpp"

ShadingProgram::ShadingProgram(void) {}

ShadingProgram::ShadingProgram(std::string vp, std::string gp, std::string fp)
{
	std::string shader;
	GLuint shader_id;
	const GLchar *source;
	
	_program = glCreateProgram();
	
	shader = GetShaderCode(vp);
	shader_id = glCreateShader(GL_VERTEX_SHADER);	
	source = shader.c_str();	
	glShaderSource(shader_id, 1, &source, nullptr);
	glCompileShader(shader_id);
	CheckCompilation(shader_id, vp);	
	glAttachShader(_program, shader_id);

	if (!gp.empty())
	{
		shader = GetShaderCode(gp);
		shader_id = glCreateShader(GL_GEOMETRY_SHADER);		
		source = shader.c_str();		
		glShaderSource(shader_id, 1, &source, nullptr);
		glCompileShader(shader_id);
		CheckCompilation(shader_id, gp);
		glAttachShader(_program, shader_id);
	}

	shader = GetShaderCode(fp);
	shader_id = glCreateShader(GL_FRAGMENT_SHADER);	
	source = shader.c_str();	
	glShaderSource(shader_id, 1, &source, nullptr);
	glCompileShader(shader_id);
	CheckCompilation(shader_id, fp);
	glAttachShader(_program, shader_id);
	glLinkProgram(_program);
//	CheckLinking();
}

std::string	ShadingProgram::GetShaderCode(std::string filepath)
{
	std::ifstream fileStream(filepath);

	if (fileStream.fail() || !fileStream.good())
	{
		std::cerr << "Cannot read shader file" << std::endl;
	}
	std::stringstream buf;
	buf << fileStream.rdbuf();
	return buf.str();	
}

void	ShadingProgram::CheckCompilation(GLuint id, std::string path)
{
	GLint success = 0;
	GLint logsize;

	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logsize);
		
		char *log = new char[logsize];
		
		glGetShaderInfoLog(id, logsize, nullptr, log);
		std::cerr << std::endl << log << std::endl
			  << "from: " << path << std::endl << std::endl;
	}
}

void	ShadingProgram::CheckLinking(void)
{
	GLint success = 0;
	GLint logsize;

	glGetShaderiv(_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderiv(_program, GL_INFO_LOG_LENGTH, &logsize);

		char *log = new char[logsize];

		glGetShaderInfoLog(_program, logsize, nullptr, log);
		std::cerr << std::endl << log << std::endl
			  << "from: " << "path" << std::endl << std::endl;
	}	
}

void	ShadingProgram::Use(void)
{
	glUseProgram(_program);
}

GLuint	ShadingProgram::ID(void)
{
	return _program;
}
