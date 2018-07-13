NAME = raetan

SRC = src/*.cpp

FRAMEWORKS = -framework OpenGl

GLFW_INC = -I ~/.brew/include

GLFW_LINK = -L ~/.brew/lib -lglfw

MY_INC = -I src/.

FLAGS = -std=c++14 -O3

make:
	g++ $(FLAGS) $(SRC) -o $(NAME) $(FRAMEWORKS) $(MY_INC) $(GLFW_INC) $(GLFW_LINK)

fclean:
	rm $(NAME)
