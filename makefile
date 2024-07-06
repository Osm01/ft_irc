
NAME = server
COMPI = c++ -std=c++98
ERRORS = -Wall -Wextra -Werror

CPP_FILES = server.cpp ft_irc.cpp client.cpp chanel.cpp

all : $(NAME)

$(NAME) : $(CPP_FILES)
	$(COMPI) $(ERRORS) $(CPP_FILES) -o $(NAME)

clean :
	rm -rf $(NAME)

fclean : clean

re : clean all

.PHONY : all clean fclean re