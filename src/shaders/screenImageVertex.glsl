
#version 410 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 uv;

out vec2 uvCoor;

void	main()
{
	gl_Position = vec4(vertex, 1);
	uvCoor = uv;
}