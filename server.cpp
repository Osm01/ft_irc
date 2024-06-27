//
// Created by ouidriss on 6/27/24.
//

#include "server.h"

Server::Server(int port, std::string pass) : Events(1024)
{
    Server_Socket = 0;
	this->port = port;
	this->password = pass;
    std::memset(&Server_addr, 0, sizeof(Server_addr));
	// opt = 1 to enable to resue the socket on implimentation of Set_ReuseSocket()
	opt = 1;
}

int	Server::Create_Socket()
{
	if ((Server_Socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return (std::cerr <<  RED << "ERROR : Socket CREATING" << RESET << std::endl, -1);
    return (1);
}

int	Server::Set_ReuseSocket()
{
	if (setsockopt(Server_Socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		return (std::cerr <<  RED << "ERROR : On reusing the port" << RESET << std::endl, -1);
	return (1);
}

int	Server::Bind_Socket()
{
	Server_addr.sin_family = AF_INET;
	Server_addr.sin_port = htons(this->port);
	Server_addr.sin_addr.s_addr = INADDR_ANY;
	Server_addrlen = sizeof (Server_addr);
	if (bind(Server_Socket, (const sockaddr *) &Server_addr, Server_addrlen) == -1)
		return (std::cerr << RED << "ERROR : Bind Socket" << RESET << std::endl, -1);
	return (1);
}

int	Server::Listen()
{
	if (listen(Server_Socket, 100) == -1)
		return (std::cerr << RED << "ERROR : LISTING FAILED" << RESET << std::endl, -1);
	return (1);
}

int	Server::Setup_Sever()
{
	if (Create_Socket() && Set_ReuseSocket() && Bind_Socket() && Listen())
		return (1);
	std::cout << GREEN << "SUCCESFULLY SETUP SERVER" << RESET << std::endl;
	return (-1);
}

int	Server::Multiplexing()
{
	epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
		return (std::cerr << RED <<"ERROR : Create EPOLL" << RESET <<std::endl, -1);
	Event.events = EPOLLIN;
	Event.data.fd = Server_Socket;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, Server_Socket, &Event) == -1)
		return (std::cerr << RED << "ERROR : EPOLL CTL" << RESET << std::endl, -1);
	while (1)
	{
		int num_event = 0;
		if ((num_event = epoll_wait(epoll_fd, Events.data(), Events.size(), -1)) == -1)
			return (std::cerr << RED << "ERROR : EPOOL WAIT" <<  RESET << std::endl, 0);
		if (num_event == static_cast<int>(Events.size()))
			Events.resize(Events.size() * 2);
		for (int i = 0; i < num_event; i ++)
		{
			if (Events[i].data.fd == Server_Socket)
			{
				if ((Client_Socket = accept(Server_Socket, (struct sockaddr *)&Client_addr, &Client_addrlen)) == -1)
					return (std::cerr << RED <<"ERROR : Accept Connection" << RESET <<std::endl, -1);
				Client_event.data.fd = Client_Socket;
				Client_event.events = EPOLLIN | EPOLLOUT;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, Client_Socket, &Client_event) == -1)
					return (std::cerr << RED << "ERROR : EPOOL CTL CLIENT" << RESET << std::endl, -1);
			}
			else
			{
				char buffer[1024];
				if (Events[i].events & EPOLLIN)
				{
					int nb_byte = read(Events[i].data.fd, buffer, 1024);
					if (nb_byte == -1)
						return (std::cerr << RED << "ERROR : Read data on User Socket" << RESET << std::endl, -1);
					if (nb_byte == 0)
					{
						if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, Events[i].data.fd, NULL) == -1)
							return (std::cerr << RED << "ERROR : EPOOL CTL CLIENT" << RESET << std::endl, -1);
						close(Events[i].data.fd);
						std::cerr << BLUE << "ERROR : Connection of Client closed" <<  RESET << std::endl;
					}
					buffer[nb_byte] = 0;
					
				}
				else if (Events[i].events & EPOLLOUT)
				{
					std::cout << "RESPONDE BY : " << buffer;
					 // responde
				}
			}
		}
	}
	return (1);
}