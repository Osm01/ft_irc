#include "chanel.h"
#include "client.hpp"
#include "string.h"

Chanel::Chanel(int admin, std::string chanel_name , std::string topic)
{
    this->admin_fd = admin;
	this->chanel_global_name = chanel_name;
    this->topic = topic;
	// set default value of struct password
	this->password_info.active = false;
	this->password_info.password = "";
	// set default value of struct permission
	this->permision_info.invite_only = false;
	this->permision_info.restrictive_topic = false;
	this->permision_info.max_user = 200;
}

Chanel::~Chanel(){}

std::string	convert_fd_to_name(int fd, std::map<int, Client> &server_users)
{
	std::map<int, Client>::iterator it;
	for (it = server_users.begin() ; it != server_users.end() ; it ++)
	{
		if (it->first == fd)
			return (it->second.username);
	}
	return (std::string(""));
}

int	convert_name_to_fd(std::string name, std::map<int, Client> &server_users)
{
	std::map<int, Client>::iterator it;
	for (it = server_users.begin() ; it != server_users.end() ; it ++)
	{
		if (it->second.username == name)
			return (it->first);
	}
	return (0);
}

int	Check_Existng_Chanel(std::string &name , std::map<std::string, Chanel> &chanels)
{
	if (chanels.find(name) != chanels.end())
		return (1);
	return (0);
}

int	Chanel::Check_UserOnChanel(int fd_user)
{
	std::map<int, Client>::iterator it = users.find(fd_user);
	if (it != users.end() || fd_user == admin_fd)
		return (1);
	return (0);
}

int	Check_UserOnServer(std::map<int, Client> &server_users , int fd_user)
{
	std::map<int, Client>::iterator it = server_users.find(fd_user);
	if (it != server_users.end())
		return (1);
	return (0);
}

void	join_user(std::string &chanel_name, std::map<std::string , Chanel> &chanels, int fd_new_user, \
		std::map<int, Client> &server_users)
{
	if (Check_UserOnServer(server_users, fd_new_user))
	{
		if (Check_Existng_Chanel(chanel_name, chanels))
		{
			std::cout << GREEN << "\nEXIST CHANEL" << RESET << std::endl;
			if (!chanels.find(chanel_name)->second.Check_UserOnChanel(fd_new_user))
			{
				std::string msg = std::string(GREEN"You have been join to Chanel (") + chanel_name.c_str() + ")"RESET + "\n";
				send(fd_new_user, msg.c_str(), msg.length(), 0);
				chanels.find(chanel_name)->second.Add_User(fd_new_user, server_users.find(fd_new_user)->second);
			}
			else
			{
				std::string msg(RED"\nYOU already on Chanel\n"RESET);
				send(fd_new_user, msg.c_str(), msg.length(), 0);
			}
		}
		else
		{
			std::cout << GREEN << "\nCREATE A CHANEL" << RESET << std::endl;
			std::string msg = std::string(GREEN"You just Create a new Chanel (") + chanel_name.c_str() + ") and now you're the admin"RESET + "\n";
			send(fd_new_user, msg.c_str(), msg.length(), 0);
			Chanel new_chanel(fd_new_user, chanel_name, std::string("tpic name"));
			chanels.insert(std::pair<std::string, Chanel>(chanel_name, new_chanel));
		}
	}
	else
		std::cout << RED << "Client not exist" << RESET << std::endl;
}

void	Chanel::Add_User(int fd_new_user, Client &client)
{
	users.insert(std::pair<int, Client>(fd_new_user, client));
}

void	Chanel::kick_user(std::string username, std::string &username_to_kick, std::map<int, Client> &server_users)
{
	if (username == "")
		return ((void)(std::cout << RED << "USER NOT EXIST" << RESET << std::endl));
	if (username != convert_fd_to_name(admin_fd, server_users))
	{
		if (this->users.find(convert_name_to_fd(username, server_users)) != this->users.end())
		{
			std::map<std::string , int>::iterator check = permision_info.op_privileges.find(username);
			if (check == permision_info.op_privileges.end() || \
			(check != permision_info.op_privileges.end() && check->second != 1))
				return ((void)(std::cout << RED << "PERMISSION DENIED" << RESET << std::endl));
		}
		else
			return ((void)(std::cout << RED << "USER NOT ON CHANEL" << RESET << std::endl));
	}
	if (users.find(convert_name_to_fd(username_to_kick, server_users)) != users.end())
		users.erase(convert_name_to_fd(username_to_kick, server_users));
	else
		std::cout << RED << "ERROR : User " << username_to_kick << " Not found on chanel" << RESET << std::endl;
}

void	Kick_manager(int fd_user_cmd, std::string &user_to_kick, std::string &chanel_name, \
		std::map<std::string, Chanel> &chanels, std::map<int, Client> &server_users)
{
	if (Check_UserOnServer(server_users, fd_user_cmd) && Check_Existng_Chanel(chanel_name, chanels))
	{
		std::string user_cmd = convert_fd_to_name(fd_user_cmd, server_users);
		chanels.find(chanel_name)->second.kick_user(user_cmd, user_to_kick, server_users);
	}
	else
		std::cout << RED << "User want to kick not found or you are not exist XD" << RESET << std::endl;
}

void	Chanel::set_new_topic(int fd_user, std::string &topic)
{
	if (fd_user != admin_fd)
	{
		if (!Check_UserOnChanel(fd_user))
			return ((void)(std::cout << RED << "User not on Channel" << RESET << std::endl));
	}
	this->topic = topic;
}

void	topic_manager(int fd_user, std::string &topic, std::string &chanel_name ,std::map<int, Client> &server_users, \
		std::map<std::string , Chanel> &chanels)
{
	if (Check_Existng_Chanel(chanel_name, chanels) && Check_UserOnServer(server_users, fd_user))
		chanels.find(chanel_name)->second.set_new_topic(fd_user, topic);
	else
		return ((void)(std::cout << RED << "Chanel not found or user not on server" << RESET << std::endl));
}

void	Invite_manager(int fd_user, std::string &to_invite, std::string chanel_name, std::map<int, \
		Client> &server_users, std::map<std::string , Chanel> &chanels)
{
	if (Check_UserOnServer(server_users, fd_user) && Check_UserOnServer(server_users, \
	convert_name_to_fd(to_invite, server_users)))
	{
		if (Check_Existng_Chanel(chanel_name, chanels))
			chanels.find(chanel_name)->second.invite_user(to_invite, server_users);
	}
	else
		return ((void)(std::cout << RED << "User want to invite not exist on Server" << RESET << std::endl));
}

void	Chanel::invite_user(std::string username, std::map<int, Client> &server_users)
{
	int	fd_to_invite = convert_name_to_fd(username, server_users);
	if (!Check_UserOnChanel(fd_to_invite))
	{
		std::string msg = (GREEN"You invited to chanel ") + this->chanel_global_name + " to join (JOIN " \
		+ chanel_global_name + ")" + RESET;
		send(fd_to_invite, msg.c_str(), msg.length(), 0);
	}
	else
		std::cout << RED << "The user" << username << " already on chanel" << RESET << std::endl;
}

// void	chanel::set_password(std::string &username, bool status ,const std::string &pass)
// {
// 	if (username != admin.username)
// 	{
// 		if (users.find(username) != users.end())
// 		{
// 			std::map<std::string , int>::iterator check = password_info.op_privileges.find(username);
// 			if (check == password_info.op_privileges.end() || \
// 			(check != password_info.op_privileges.end() && check->second != 1))
// 				return ((void)std::cout << RED << "PERMISSION DENIED" << RESET << std::endl);
// 		}
// 		else
// 			return ((void)std::cout << RED << "USER NOT ON CHANEL" << RESET << std::endl);
// 	}
// 	this->password_info.active = status;
// 	this->password_info.password = pass;
// }


// void	chanel::set_invite_only(std::string &username, bool status)
// {
// 	if (username != admin.username)
// 	{
// 		if (users.find(username) != users.end())
// 		{
// 			std::map<std::string , int>::iterator check = password_info.op_privileges.find(username);
// 			if (check == password_info.op_privileges.end() || \
// 			(check != password_info.op_privileges.end() && check->second != 1))
// 				return ((void)std::cout << RED << "PERMISSION DENIED" << RESET << std::endl);
// 		}
// 		else
// 			return ((void)std::cout << RED << "USER NOT ON CHANEL" << RESET << std::endl);
// 	}
// 	this->permission_info.invite_only = status;
// }

// void	chanel::set_restrictive_topic(std::string &username, bool status)
// {
// 	if (username != admin.username)
// 	{
// 		if (users.find(username) != users.end())
// 		{
// 			std::map<std::string , int>::iterator check = password_info.op_privileges.find(username);
// 			if (check == password_info.op_privileges.end() || \
// 			(check != password_info.op_privileges.end() && check->second != 1))
// 				return ((void)std::cout << RED << "PERMISSION DENIED" << RESET << std::endl);
// 		}
// 		else
// 			return ((void)std::cout << RED << "USER NOT ON CHANEL" << RESET << std::endl);
// 	}
// 	this->permission_info.restrictive_topic = status;
// }

// void	chanel::set_max_user(std::string &username, double max)
// {
// 	if (username != admin.username)
// 	{
// 		if (users.find(username) != users.end())
// 		{
// 			std::map<std::string , int>::iterator check = password_info.op_privileges.find(username);
// 			if (check == password_info.op_privileges.end() || \
// 			(check != password_info.op_privileges.end() && check->second != 1))
// 				return ((void)std::cout << RED << "PERMISSION DENIED" << RESET << std::endl);
// 		}
// 		else
// 			return ((void)std::cout << RED << "USER NOT ON CHANEL" << RESET << std::endl);
// 	}
// 	this->permission_info.max_user = max;
// }

// void	chanel::set_op_privileges(std::string &username, std::string &user_to_rank, int status)
// {
// 	if (username != admin.username)
// 	{
// 		if (users.find(username) != users.end())
// 		{
// 			std::map<std::string , int>::iterator check = password_info.op_privileges.find(username);
// 			if (check == password_info.op_privileges.end() || \
// 			(check != password_info.op_privileges.end() && check->second != 1))
// 				return ((void)std::cout << RED << "PERMISSION DENIED" << RESET << std::endl);
// 		}
// 		else
// 			return ((void)std::cout << RED << "USER NOT ON CHANEL" << RESET << std::endl);
// 	}
// 	if (users.find(user_to_rank) != users.end())
// 		this->permission_info.op_privileges.insert(pair<std::string , int>(user_to_rank, status));
// 	else
// 		std::cout << RED << "ERROR : User " << user_to_rank << " Not found on chanel to set privileges" << RESET << std::endl;
// }