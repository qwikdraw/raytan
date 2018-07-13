
#version 410 core

in vec2 uvCoor;

out vec4 color;

uniform sampler2D tex;

void	main()
{
	color = texture(tex, uvCoor).rgba;
}