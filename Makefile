NAME	:= cub3D
CC		:= cc
FLAGS	:= -Wall -Wextra -Werror -g
LIBS	:= -lXext -lX11 -lm

# Library Paths - No Spaces Around '='
LIBFT_DIR	:= libft
MLX_DIR		:= minilibx-linux

# Source Files

SRCS := main.c \
		launcher.c \
		draw.c \
		draw_utils.c \
		draw_texture.c \
		player.c \
		player_utils.c \
		input.c \
		arg_check.c \
		free_functions.c \
		init.c \
		parse_id.c \
		parse_line.c \
		parse_map.c \
		parse_utils.c \
		map_check.c \
		map_check_utils.c \
		free_utils.c

OBJS := $(SRCS:.c=.o)

# Explicit Library Paths
LIBFT_A		:= $(LIBFT_DIR)/libft.a
MLX_A		:= $(MLX_DIR)/libmlx.a

# Main Target
all: $(NAME)

# Executable Creation
$(NAME): $(OBJS) $(LIBFT_A) $(MLX_A)
	@echo "Compiling $(NAME)..."
	$(CC) $(FLAGS) -o $@ $(OBJS) $(LIBFT_A) $(MLX_A) $(LIBS)

# Library Compilation
$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

$(MLX_A):
	$(MAKE) -C $(MLX_DIR)

# Compilation Rule
%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

# Cleaning Rules
clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(MLX_DIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re