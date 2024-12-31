NAME = cub3D
CC = cc
CFLAGS = -g -Wall -Wextra -Werror
RM = rm -rf

BOLD      = \033[1m
CGREEN    = \033[32m
CCYAN     = \033[36m

LIBFTDIR = libft
LIBFT = libft/libft.a
SRC = main.c parsing/parsing.c parsing/parse_texture.c parsing/parse_color.c parsing/parse_map.c \
		gnl/get_next_line.c gnl/get_next_line_utils.c \

OBJ = ${SRC:.c=.o}

all: print_art ${LIBFT} ${NAME}

print_art:
	@echo "                                                                            "
	@echo "                                                                            "
	@echo "\033[35m██       \033[35m█████  \033[35m██   ██ \033[35m █████  \033[35m██████  \033[34m██████  \033[34m██████  \033[0m"
	@echo "\033[35m██      \033[35m██   ██ \033[35m██  ██  \033[35m██   ██ \033[35m██   ██ \033[34m     ██ \033[34m██   ██ \033[0m"
	@echo "\033[35m██      \033[35m███████ \033[35m█████   \033[35m███████ \033[35m██████  \033[34m █████  \033[34m██   ██ \033[0m"
	@echo "\033[35m██      \033[35m██   ██ \033[35m██  ██  \033[35m██   ██ \033[35m██   ██ \033[34m     ██ \033[34m██   ██ \033[0m"
	@echo "\033[35m███████ \033[35m██   ██ \033[35m██   ██ \033[35m██   ██ \033[35m██████  \033[34m██████  \033[34m██████  \033[0m"
	@echo "                                                                            "
	@echo "\033[1m                     ✨ LAKAB3D ✨                         \033[0m"
	@echo "                                                                            "
	@echo "                                                                            "


$(LIBFT):
	@$(MAKE) -C $(LIBFTDIR)

${NAME}: ${OBJ}
	@echo "$(BOLD)$(CGREEN)building the project...\033[0m"
	${CC} ${CFLAGS} ${OBJ} ${LIBFT} -o ${NAME}

clean:
	@echo "$(BOLD)$(CGREEN)cleaning ...\033[0m"
	@$(MAKE) -C $(LIBFTDIR) clean
	${RM} ${OBJ}

fclean: clean
	@${RM} ${NAME}
	@$(MAKE) -C $(LIBFTDIR) fclean
re: fclean all

.PHONY: all clean fclean re
.SECONDARY: