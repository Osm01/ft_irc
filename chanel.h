
#ifndef CHANEL_HPP
#define CHANEL_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <cerrno>


# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define PURPLE "\033[35m"
# define CYAN "\033[36m"
# define RESET "\033[0m"

class Client;

typedef struct s_password
{
	bool		active;
	std::string	password;
}				t_password;

typedef struct s_permission
{
	bool				invite_only;
	bool				restrictive_topic;
	double				max_user;
	// op_privileges , int for socket client as (ID) and int for status 
	std::map<std::string , int>	op_privileges;
}						t_permission;

class Chanel
{
    private :
		std::string				chanel_name;
        std::string				topic;
		int						admin_fd;
        std::map<int , Client >	users;
		t_password				password_info;
		t_permission			permision_info;

		Chanel(const Chanel &copy);
		Chanel	&operator=(const Chanel &copy);
	public :
		Chanel(int admin, std::string &chanel_name , std::string &topic);
		~Chanel();
		void	invite_user(std::map<int, Client> &server_users, int client_fd_ask ,int user_to_invite);
		void	join_user(int fd_new_user, std::map<int, Client> &server_users);
		int		Check_UserOnChanel(int fd_user);
		int		Check_UserOnServer(std::map<int, Client> &server_users , int fd_user);
		// void	kick_user(std::string &username);
		// void	set_new_topic(std::string &topic);
		// void	set_password(bool status,  const std::string &pass);
		// void	set_invite_only(bool status);
		// void	set_restrictive_topic(bool status);
		// void	set_max_user(double max);
		// void	set_op_privileges(std::string &username, int status);
};

#endif