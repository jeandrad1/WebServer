NAME = webserver

CC = c++
CFLAGS = -Wall -Wextra -Werror

CPP98_FLAG = -std=c++98

FILES = main \
		composite/createBlocksComposite \
		composite/buildConfigLine \
		composite/AConfigBlock \
		composite/Directive \
		composite/ServerBlock \
		composite/HttpBlock \
		composite/LocationBlock \
		strategy/AutoindexStrategy \
		strategy/ClientMaxBodySizeStrategy \
		strategy/ErrorPageStrategy \
		strategy/LimitExceptStrategy \
		factory/registerAllStrategies \
		factory/validateConfigTreeFactory \
		factory/createStrategies \
		factory/StrategyFactory \
		strategy/IndexStrategy \
		strategy/RootStrategy \
		strategy/ServerNameStrategy \
		strategy/ListenStrategy \
		strategy/ReturnStrategy \
		strategy/HttpBlockStrategy \
		strategy/LocationBlockStrategy \
		strategy/ServerBlockStrategy \
		manager/WebservManager \
		builder/createConfigClasses \
		builder/HttpBuilder \
		builder/LocationBuilder \
		builder/ServerBuilder \
		builder/HttpConfig \
		builder/ServerConfig \
		builder/LocationConfig \
		utils/utils \
		execute/LaunchSockets


$(shell mkdir -p ./build)
$(shell mkdir -p ./build/composite)
$(shell mkdir -p ./build/strategy)
$(shell mkdir -p ./build/factory)
$(shell mkdir -p ./build/builder)
$(shell mkdir -p ./build/utils)
$(shell mkdir -p ./build/manager)
$(shell mkdir -p ./build/execute)
$(shell mkdir -p ./build/charge_flag_makefile)

SRCS_DIR = ./src/
OBJS_DIR = ./build/

SRCS = $(addprefix $(SRCS_DIR), $(addsuffix .cpp, $(FILES)))
OBJS = $(addprefix $(OBJS_DIR), $(addsuffix .o, $(FILES)))

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@$(CC) $(FLAGS) $(CPP98_FLAG) -c $< -o $@

all: $(NAME)

$(NAME) : $(OBJS) $(OBJS_DIR)charge_flag_makefile/charge.flag
	@$(CC) $(FLAGS) $(CPP98_FLAG) $(OBJS) -o $(NAME)
	@echo
	@echo "$(GREEN)WebServer compiled!"
	@echo

clean:
	@$(RM) $(OBJS)
	@$(RM) $(OBJS_DIR)charge_flag_makefile/charge.flag:
	$(shell rm -rf ./build)
	@echo
	@echo "$(RED) Cleaning objects. $(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED) Cleaning executables. $(RESET)"
	@echo

re: fclean setup all

setup:
	$(shell mkdir -p ./build)
	$(shell mkdir -p ./build/composite)
	$(shell mkdir -p ./build/strategy)
	$(shell mkdir -p ./build/factory)
	$(shell mkdir -p ./build/builder)
	$(shell mkdir -p ./build/utils)
	$(shell mkdir -p ./build/manager)
	$(shell mkdir -p ./build/execute)
	$(shell mkdir -p ./build/charge_flag_makefile)

PHONY: clean fclean all re setup

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

# Rule for the charging bar
$(OBJS_DIR)charge_flag_makefile/charge.flag:
	@echo
	@echo -n "$(GREEN)Compiling: $(RESET)["
	@for i in $$(seq 1 2); do \
		echo -n "##"; \
		sleep 0.20; \
	done
	@for i in $$(seq 1 6); do \
		echo -n "####"; \
		sleep 0.10; \
	done
	@echo "] $(GREEN)Done!$(RESET)"
	@touch $(OBJS_DIR)charge_flag_makefile/charge.flag