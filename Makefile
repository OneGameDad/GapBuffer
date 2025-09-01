# Color codes
X := \033[0;39m
BLACK := \033[0;30m
DARK_RED := \033[0;31m
DARK_GREEN := \033[0;32m
DARK_YELLOW := \033[0;33m
DARK_BLUE := \033[0;34m
DARK_MAGENTA := \033[0;35m
DARK_CYAN := \033[0;36m
DARK_GRAY := \033[0;90m
LIGHT_GRAY := \033[0;37m
RED := \033[0;91m
GREEN := \033[0;92m
YELLOW := \033[0;93m
BLUE := \033[0;94m
MAGENTA := \033[0;95m
CYAN := \033[0;96m
WHITE := \033[0;97m

# Project variables
CC := g++
RM := rm -rf

SRCS_FILES := main.cpp gap_buffer.cpp

# Detect Platform
UNAME_S := $(shell uname -s)

ifeq ($(OS),Windows_NT)
	NAME := hyper-bol-e.exe
	PLATFORM = WINDOWS
	CXX = g++
	CXXFLAGS = -Wall -Werror -Wextra -std=c++20 $(DEPFLAGS) #-Ilib/raylib/src
	LDFLAGS = -lraylib -lfreetype -lopengl32 -lgdi32 -lwinmm #-Llib/raylib/src
else ifeq ($(UNAME_S),Linux)
	NAME := hyper-bol-e
	PLATFORM = LINUX
	CXX = g++
	CXXFLAGS := -Wall -Wextra -Werror -std=c++20 $(DEPFLAGS) -I/usr/include/freetype2 #-Ilib/raylib/src
	LDFLAGS = -lraylib -lfreetype -lfmt -lGL -lm -ldl -lpthread -lrt -lX11 #-Llib/raylib/src
endif

SRCS_DIR := srcs/
OBJS_DIR := objs/
INCL_DIR := includes/
DEPS_DIR := deps/

SRCS := $(addprefix $(SRCS_DIR), $(SRCS_FILES))
OBJS := $(addprefix $(OBJS_DIR), $(SRCS_FILES:.cpp=.o))
DEPS := $(OBJS:.o=.d)

#RAYLIB_DIR = lib/raylib
#RAYLIB_REPO = https://github.com/raysan5/raylib.git
#RAYLIB_LIB = $(RAYLIB_DIR)/src/libraylib.a

DEPFLAGS := -MMD -MP
SANITIZERS := -fsanitize=address -fsanitize=leak -fsanitize=undefined
INCL := -I $(INCL_DIR)

.PHONY: all clean fclean re debug rebug raylib

# Main target
all: $(NAME) #raylib
	@echo "$(GREEN)Run the program with ./$(NAME) $(X)"

#raylib:
#	@if [ ! -d "$(RAYLIB_DIR)" ]; then \
#		echo "$(DARK_CYAN)Cloning raylib..."; \
#		git clone --depth=1 $(RAYLIB_REPO) $(RAYLIB_DIR); \
#		echo "$(DARK_CYAN)Raylib cloned $(X)"; \
#	fi
#	@$(MAKE) -C $(RAYLIB_DIR)/src RAYLIB_LIBTYPE=STATIC=$(PLATFORM)

# Linking
$(NAME): $(OBJS)
	@$(CXX) $(OBJS) $(CXXFLAGS) -o $@ $(LDFLAGS)
	@echo "$(DARK_YELLOW)$@ compiled $(X)"

# Object compilation
$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(DEPS_DIR)
	@echo "$(DARK_CYAN)Compiling $<... $(X)"
	@$(CXX) $(CXXFLAGS) $(INCL) -c $< -o $@
	@echo "$(BLUE)Compiled: $@$(X)"

-include $(DEPS)

# Clean object and dep files
clean:
#	$(MAKE) -C $(RAYLIB_DIR)/src clean
#	echo "$(DARK_RED)Raylib cleaned $(X)";

	@if [ -d "$(OBJS_DIR)" ]; then \
	$(RM) $(OBJS_DIR); \
	echo "$(DARK_RED)Objects cleaned $(X)"; \
	else \
	echo "No object files to clean"; \
	fi
	
	@if [ -d "$(DEPS_DIR)" ]; then \
	$(RM) $(DEPS_DIR); \
	echo "$(DARK_RED)Dependencies cleaned $(X)"; else \
	echo "No dependency files to clean"; \
	fi

# Full clean
fclean: clean
	@if [ -f "$(NAME)" ]; then \
	$(RM) $(NAME); \
	echo "$(DARK_RED)$(NAME) removed $(X)"; \
	else \
	echo "No executables to clean"; \
	fi
	
#	@if [ -d "$(RAYLIB_DIR)" ]; then \
#		$(RM) $(RAYLIB_DIR); \
#		echo "$(DARK_RED)Raylib directory removed $(X)"; \
#	else \
#		echo "Raylib directory not found"; \
#	fi

# Rebuild
re: fclean all
	@echo "$(GREEN)$(NAME) Cleaned and rebuilt $(X)"

# Debug build
debug: CXXFLAGS += -ggdb3 $(SANITIZERS)
debug: re

# Rebuild with debug
rebug:
	@$(MAKE) clean
	@$(MAKE) debug
	
