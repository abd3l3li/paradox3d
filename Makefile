
CC = cc
CFLAGS = #-Wall -Wextra -Werror 
NAME = cube3d
SRCS = main.c render.c events.c
MLX_DIR = ./minilibx-linux
MLX = $(MLX_DIR)/libmlx_Linux.a
MLX_FLAGS = -L$(MLX_DIR) -lmlx_Linux -L/usr/X11R6/lib -lXext -lX11 -lm -lbsd

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(MLX_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(MLX) $(MLX_FLAGS) -o $(NAME)

bonus: $(BONUS_OBJS)
	$(MAKE) -C $(MLX_DIR)
	$(CC) $(CFLAGS) $(BONUS_OBJS) $(MLX) $(MLX_FLAGS) -o $(NAME)_bonus

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME) $(NAME)_bonus
	$(MAKE) clean -C $(MLX_DIR)

re: fclean all
