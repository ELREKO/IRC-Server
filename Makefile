NAME = ircserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -g -std=c++98

SRCS = main.cpp SRC/Authenti.cpp SRC/Channel.cpp SRC/Client.cpp SRC/Server.cpp \
	CMD/INVITE.cpp CMD/JOIN.cpp CMD/KICK.cpp CMD/MODE.cpp CMD/PART.cpp CMD/PRIVMSG.cpp CMD/QUIT.cpp CMD/TOPIC.cpp CMD/HELP.cpp

SRCS_BONUS = BONUS/main.cpp BONUS/player.cpp BONUS/bot.cpp

OBJ_DIR = obj
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
OBJS_BONUS = $(SRCS_BONUS:%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

bonus: $(OBJS_BONUS)
	@$(CC) $(CFLAGS) -o $(NAME_BONUS) $(OBJS_BONUS)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS) a.out

re: fclean all

.PHONY: all clean fclean re bonus
