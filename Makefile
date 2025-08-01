FLAGS = -Wall -Wextra -Werror -ggdb
STD98 = -std=c++98
CXX = c++
INCLUDES = -I includes
SRC_DIR = src
OBJ_DIR = obj
SRC =  main.cpp Channel.cpp Client.cpp Input.cpp Reply.cpp Errors.cpp Server.cpp
SRCS = $(addprefix $(SRC_DIR)/, $(SRC))
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
NAME = ircserv
DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(FLAGS) $(STD98) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(FLAGS) $(STD98) $(INCLUDES) -MMD -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all