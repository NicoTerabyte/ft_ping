COMPILING_RULES := gcc
PROGRAM_NAME := ft_ping
C_FILES := $(shell find srcs -name '*.c')
# I'm saying: the OBJS variable is made of the files that you
# find in the C_FILES variable in the srcs directory BUT they are now
# in the objs directory and called .o
OBJS := $(C_FILES:srcs/%.c=objs/%.o)
FLAGS := -Wall -Wextra -Werror -g 

GREEN := \e[0;32m
RESET := \e[0m
RED   := \e[1;31m



# objs folder rule creation without the -c the c files that don't have a main
# would go in an error the % is an expander it matches every file that has a .o or .c
# YOU HAVE to specify the folder or it would search in the same folder of the makefile
objs/%.o: srcs/%.c
	@printf "$(GREEN)file object compilation$(RESET)\n"
	@mkdir -p $(dir $@)
	$(COMPILING_RULES) $(FLAGS) -c $< -o $@ 


all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJS)
	@printf "$(GREEN)program name rule$(RESET)\n"
	$(COMPILING_RULES) $(FLAGS) $(OBJS) -o $(PROGRAM_NAME) -lm

clean:
	@printf "$(GREEN)cleaning directory of objs$(RESET)\n"
	rm -rf objs

fclean: clean
	@printf "$(GREEN)removing every file created by Makefile$(RESET)\n"
	rm -f $(PROGRAM_NAME)

re: fclean all

.PHONY: all clean fclean re


