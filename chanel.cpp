#include "chanel.h"
#include "client.h"

Chanel::Chanel(int admin, std::string &chanel_name , std::string &topic)
{
    this->admin_fd = admin;
	this->chanel_name = chanel_name;
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

int	Chanel::Check_UserOnChanel(int fd_user)
{
	std::map<int, Client>::iterator it = users.find(fd_user);
	if (it != users.end() || fd_user == admin_fd)
		return (1);
	return (0);
}

int	Chanel::Check_UserOnServer(std::map<int, Client> &server_users , int fd_user)
{
	std::map<int, Client>::iterator it = server_users.find(fd_user);
	if (it != server_users.end())
		return (1);
	return (0);
}

void	Chanel::invite_user(std::map<int, Client> &server_users, int client_fd_ask ,int user_to_invite)
{
	if (Check_UserOnChanel(client_fd_ask))
	{
		if (Check_UserOnServer(server_users, user_to_invite))
		{
			// Send invitation to client
			std::string message = "Invitation from " + users.find(client_fd_ask)->second.username + " to join chanel " + chanel_name;
			send(user_to_invite, message.c_str(), message.length(), 0);
			std::cout << GREEN << "Invitation successfully Send to " << users.find(client_fd_ask)->second.username << RESET << std::endl;
		}
		else
			std::cout << RED << "Client not found on Server " << RESET << std::endl;
	}
	else
		std::cout << RED << "User not found on Chanel" << RESET << std::endl;
}

void	Chanel::join_user(int fd_new_user, std::map<int, Client> &server_users)
{
	if (Check_UserOnServer(server_users, fd_new_user))
	{
		if (!Check_UserOnChanel(fd_new_user))
			users[users.end()->first] = server_users.find(fd_new_user)->second;
		else
			std::cout << RED << "User already on Chanel" << RESET << std::endl;
	}
	else
		std::cout << RED << "Client not exist" << std::endl;
}


// void	kick_user(std::string &username, std::string &username_to_kick)
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
// 	if (users.find(username_to_kick) != users.end())
// 		users.erase(username_to_kick);
// 	else
// 		std::cout << RED << "ERROR : User " << username_to_kick << " Not found on chanel" << RESET << std::endl;
// }

// void	set_new_topic(std::string &username, std::string &topic)
// {
// 	if (username != admin.username)
// 	{
// 		if (users.find(username) != users.end())
// 		{
// 			std::map<std::string , int>::iterator check = password_info.op_privileges.find(username);
// 			if (check == password_info.op_privileges.end() || \
// 			(check != password_info.op_privileges.end() && check->second != 1) || \
// 			!password_info.restrictive_topic)
// 				return ((void)std::cout << RED << "PERMISSION DENIED" << RESET << std::endl);
// 		}
// 		else
// 			return ((void)std::cout << RED << "USER NOT ON CHANEL" << RESET << std::endl);
// 	}
// 	this->topic = topic;
// }

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