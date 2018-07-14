NAME = raetan

SRC = $(wildcard src/*.cpp)

OBJ = $(patsubst src/%.cpp, src/%.o, $(SRC))

CC = g++

FRAMEWORKS = -framework OpenGl

GLFW_INC = -I ~/.brew/include

GLFW_LINK = -L ~/.brew/lib -lglfw

MY_INC = -I src/

FLAGS = -std=c++14 -O3

all: $(NAME)

$(NAME): $(OBJ)
	@printf "compiling $(NAME)..."
	@$(CC) $(FLAGS) $^ $(FRAMEWORKS) $(MY_INC) $(GLFW_INC) $(GLFW_LINK) -o $(NAME)
	@echo SUCCESS!

%.o: %.cpp %.hpp
	@echo compiling... $@
	@$(CC) $(FLAGS) $(MY_INC) $(GLFW_INC) -c -o $@ $< 

src/main.o : src/main.cpp
	@echo compiling... $@
	@$(CC) $(FLAGS) $(MY_INC) $(GLFW_INC) -c -o $@ $< 

clean:
	@echo cleaning object files...
	@/bin/rm -f $(OBJ)
	@/bin/rm -f $(patsubst src/%.cpp, src/%.hpp.gch, $(SRC))

fclean: clean
	@echo cleaning $(NAME)...
	@/bin/rm -f $(NAME)

re:
	@echo remaking $(NAME)...
	@make fclean
	@make

.PHONY: clean fclean re
