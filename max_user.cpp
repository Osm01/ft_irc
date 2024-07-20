# include "chanel.h"
# include "client.hpp"

void	Chanel::set_max_user(int fd_user, double max, std::map<int, Client> &server_users)
{
	std::string msg;

	if (fd_user != admin_fd)
	{
		if (users.find(fd_user) != users.end())
		{
			std::map<std::string , int>::iterator check = permision_info.op_privileges.find(convert_fd_to_name(fd_user, server_users));
			if (check == permision_info.op_privileges.end() || \
			(check != permision_info.op_privileges.end() && check->second != 1))
			{
				msg = RED "PERMISSION DENIED\n" RESET;
				return ((void)(send(fd_user, msg.c_str(), msg.length(), 0)));
			}
		}
		else
		{
			msg = RED "You are Not on Chanel\n" RESET;
			return ((void)(send(fd_user, msg.c_str(), msg.length(), 0)));
		}
	}
	if (max == -1)
		msg = GREEN "Number of Client on Chanel is set to unlimited\n" RESET;
	else
	{
		std::stringstream ss;
		ss << max;
		msg = GREEN "Number of Client on Chanel is set to " + ss.str() + "\n" RESET;
	}
	Broadcast_message(msg, server_users, fd_user);
	this->permision_info.max_user = max;
}

void	Max_user_manager(int fd_user, double max, std::string &chanel_name , \
						std::map<std::string , Chanel> &chanels , std::map<int, Client> &server_users)
{
	std::string msg;

	if (Check_Existng_Chanel(chanel_name, chanels))
		chanels.find(chanel_name)->second.set_max_user(fd_user, max, server_users);
	else
	{
		msg = RED "Chanel not Found\n" RESET;
		return ((void)(send(fd_user, msg.c_str(), msg.length(), 0)));
	}
}