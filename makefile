NAME = cub3D
CC = cc
CFLAGS = #-g -Wall -Wextra -Werror
RM = rm -rf
MLX = $(MLX_DIR)/libmlx_Linux.a
MLX_DIR = ./minilibx-linux
MLX_FLAGS = -L$(MLX_DIR) -lmlx_Linux -L/usr/X11R6/lib -lXext -lX11 -lm -lbsd
BOLD      = \033[1m
CGREEN    = \033[32m
CCYAN     = \033[36m

LIBFTDIR = libft
LIBFT = libft/libft.a
SRC = main.c parsing/parsing.c parsing/texture_color.c parsing/tools.c parsing/parse_map.c \
		gnl/get_next_line.c gnl/get_next_line_utils.c \
		ray-cast/execute.c ray-cast/render.c ray-cast/events.c

OBJ = $(SRC:.c=.o)

all: print_art ${LIBFT} ${MLX} ${NAME}

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

$(MLX):
	@$(MAKE) -C $(MLX_DIR)

${NAME}: ${OBJ}
	@echo "$(BOLD)$(CGREEN)building the project...\033[0m"
	${CC} ${CFLAGS} ${OBJ} ${LIBFT} ${MLX_FLAGS} -o ${NAME}

clean:
	@echo "$(BOLD)$(CGREEN)cleaning ...\033[0m"
	@$(MAKE) -C $(LIBFTDIR) clean
	@$(MAKE) -C $(MLX_DIR) clean
	${RM} ${OBJ}

fclean: clean
	@${RM} ${NAME}
	@$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: