NAME = webserver

CC = c++
CFLAGS = -Wall -Wextra -Werror

CPP98_FLAG = -std=c++98

FILES = main \
		composite/composite \
		config/AConfigBlock \
		config/Directive \
		config/ServerBlock \
		config/HttpBlock \
		config/LocationBlock \

SRCS_DIR = ./src/
OBJS_DIR = ./src/

SRCS = $(addprefix $(SRCS_DIR), $(addsuffix .cpp, $(FILES)))
OBJS = $(addprefix $(OBJS_DIR), $(addsuffix .o, $(FILES)))

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@$(CC) $(FLAGS) $(CPP98_FLAG) -c $< -o $@

all: $(NAME)

$(NAME) : $(OBJS)
	@$(CC) $(FLAGS) $(CPP98_FLAG) $(OBJS) -o $(NAME)
	@echo
	@echo "$(GREEN)WebServer compiled!"
	@echo

clean:
	@$(RM) $(OBJS)
	@echo
	@echo "$(RED)Cleaning objects. $(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED) Cleaning executables. $(RESET)"
	@echo

re: fclean all

PHONY: clean fclean all re

# Colors
MAGENTA = \033[35;1m
YELLOW  = \033[33;1m
GREEN   = \033[32;1m
WHITE   = \033[37;1m
RESET   = \033[0m
GRAY    = \033[0;90m
BLUE    = \033[34;1m
CYAN    = \033[37;1m
BOLD    = \033[1m
RED     = \033[31;1m