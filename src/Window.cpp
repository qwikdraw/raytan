#include "Window.hpp"
#include <stdexcept>

Window::Window(int width, int height, std::string name) :
	_screenCornerX(0),
	_screenCornerY(0),
	_width(1),
	_height(1),
	_windowWidth(width),
	_windowHeight(height)
{
	GLuint vertex_array_id;

	if (glfwInit() == GLFW_FALSE)
		throw std::runtime_error("Failed to initialize GLFW");
	WindowHints();
	glfwSetErrorCallback(ErrorCallback);
	_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (!_window)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to initialize window");
	}
	glfwSetWindowUserPointer(_window, this);
	glfwSetWindowSizeCallback(_window, WindowResizeCallback);
	glfwSetWindowPosCallback(_window, WindowMoveCallback);
	glfwSetKeyCallback(_window, KeyCallback);
	glfwSetMouseButtonCallback(_window, MouseButtonCallback);
	glfwSetCursorPosCallback(_window, MousePositionCallback);
	glfwMakeContextCurrent(_window);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);
}

void	Window::WindowHints(void)
{
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
	glfwWindowHint(GLFW_SAMPLES, 4);
}


void	Window::GetWindowSize(int &width, int &height)
{
	width = _windowWidth;
	height = _windowHeight;
}

void	Window::GetMaxRenderSize(float &width, float &height)
{
	int iwidth, iheight;

	glfwGetFramebufferSize(_window, &iwidth, &iheight);
	
	width = static_cast<float>(iwidth);
	height = static_cast<float>(iheight);
}

float Window::GetAspect(void)
{
	float width, height;
	GetSize(width, height);
	return width / height;
}

bool	Window::ShouldClose(void)
{
	return glfwWindowShouldClose(_window);
}

void	Window::Close(void)
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void	Window::GetSize(float &width, float &height)
{
	float actualWidth, actualHeight;

	GetMaxRenderSize(actualWidth, actualHeight);

	width = _width * actualWidth;
	height = _height * actualHeight;
}

void	Window::SetRenderMask(float x, float y, float width, float height)
{
	float windowWidth, windowHeight;

	_width = width;
	_height = height;
	_screenCornerX = x;
	_screenCornerY = y;
	
	GetMaxRenderSize(windowWidth, windowHeight);
	glEnable(GL_SCISSOR_TEST);
	glViewport(windowWidth * x,
		   windowHeight * y,
		   windowWidth * width,		
		   windowHeight * height);
	glScissor(windowWidth * x,
		  windowHeight * y,
		  windowWidth * width,
		  windowHeight * height);
}

void	Window::RemoveRenderMask(void)
{
	_screenCornerX = 0;
	_screenCornerY = 0;
	_width = 1;
	_height = 1;

	int width, height;
	glfwGetFramebufferSize(_window, &width, &height);
	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
	glDisable(GL_SCISSOR_TEST);
}

void	Window::Render(void)
{

	for (bool &status : _mouseButtonClicked)
		status = false;
	for (bool &status : _keyJustPressed)
		status = false;

	glfwSwapBuffers(_window);
	glfwPollEvents();
}

void	Window::Clear(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void	Window::RefreshRenderMask(void)
{
	SetRenderMask(_screenCornerX, _screenCornerY, _width, _height);
}

void	WindowResizeCallback(GLFWwindow *glfwWindow, int width, int height)
{
	Window *window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	window->_windowWidth = width;
	window->_windowHeight = height;
	window->RefreshRenderMask();
}

void	Window::WindowMoveCallback(GLFWwindow *glfwWindow, int, int)
{
	Window *window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

	window->RefreshRenderMask();
}

void	Window::ErrorCallback(int, const char *description)
{
	std::cerr << description << std::endl;
}

GLFWwindow* Window::GetGLWindow(void)
{
	return _window;
}

void	KeyCallback(GLFWwindow *glfwWindow, int key, int, int action, int)
{
	Window *window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	
	if (action == GLFW_PRESS)
	{
		window->_keys[key] = true;
		window->_keyJustPressed[key] = true;
	}
	else if (action == GLFW_RELEASE)
		window->_keys[key] = false;
}

bool Window::Key(int key)
{
	if (key >= 0 && key < 512)
		return _keys[key];
	else
		return false;
}

bool	Window::KeyPress(int key)
{
	if (key >= 0 && key < 512)
		return _keyJustPressed[key];
	else
		return false;
}

void	MouseButtonCallback(GLFWwindow *glfwWindow, int button, int action, int)
{
	Window *window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	
	if (action == GLFW_PRESS)
	{
		window->_mouseButtons[button] = true;
		window->_mouseButtonClicked[button] = true;
	}
	else if (action == GLFW_RELEASE)
		window->_mouseButtons[button] = false;
}

bool	Window::MouseButton(int button)
{
	if (button >= 0 && button < 8)
		return _mouseButtons[button];
	else
		return false;
}

bool	Window::MouseClick(int button)
{
	if (button >= 0 && button < 8)
		return _mouseButtonClicked[button];
	else
		return false;
}

void	MousePositionCallback(GLFWwindow *glfwWindow, double x, double y)
{
	Window *window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	int width, height;
	window->GetWindowSize(width, height);
	window->_mousePosition.x = (x * 2 / width) - 1;
	window->_mousePosition.y = (y * -2 / height) + 1;
}

const glm::vec2& Window::MousePos(void)
{
	return const_cast<const glm::vec2&>(_mousePosition);
}
