# include "client.hpp"
# include "chanel.h"

void	Password_manager(int fd_user, std::string &new_pass ,bool status, std::string &chanel_name , \
		std::map<int, Client> &server_users, std::map<std::string , Chanel> &chanels)
{
	std::string msg;

	if (Check_Existng_Chanel(chanel_name ,chanels))
		chanels.find(chanel_name)->second.set_password(fd_user, new_pass, status, server_users);
	else
	{
		msg = RED "Chanel not Found\n" RESET;
		send(fd_user, msg.c_str(), msg.length(), 0);
	}
}

void	Chanel::set_password(int &fd_user, const std::string &pass, bool status, std::map<int, Client> &server_users)
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
			msg = RED "YOU are Not on Chanel\n" RESET;
			return ((void)(send(fd_user, msg.c_str(), msg.length(), 0)));
		}
	}
	if (status)
		msg = GREEN "Set ON password of Chanel\n" RESET;
	else
		msg = GREEN "Set OFF password of Chanel\n" RESET;
	Broadcast_message(msg, server_users, fd_user);
	this->password_info.active = status;
	this->password_info.password = pass;
}
