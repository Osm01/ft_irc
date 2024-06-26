#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>
#include <vector>

#define EXIT_FAILURE 1

int main()
{
	int                 srv_socket, cli_socket, opt = 1, epoll_fd;
	struct sockaddr_in  srv_addr, cli_addr, data;
    socklen_t           srv_addrlen, cli_addrlen, data_len;
    

    if ((srv_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return  (std::cerr << "ERROR : create a socket" << std::endl , EXIT_FAILURE);

	if (setsockopt(srv_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		return (std::cerr << "ERROR : on reusing the port" << std::endl, EXIT_FAILURE);
    std::memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(1234);
	srv_addr.sin_addr.s_addr = INADDR_ANY;
	srv_addrlen = sizeof (srv_addr);
    if (bind(srv_socket, (const sockaddr *) &srv_addr , srv_addrlen) == -1)
		return (std::cerr << "ERROR : bind the endpoint of socket" << std::endl, EXIT_FAILURE);
	if (getsockname(srv_socket, (sockaddr *) &data, &data_len) == -1)
		return (std::cerr << "ERROR : cannot get socket name", 0);
	if (listen(srv_socket, 2) == -1)
		return (std::cerr << "ERROR : listing for socket" << std::endl , EXIT_FAILURE);


	epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
		return (std::cerr << "ERROR : CREATE EPOLL" << std::endl, EXIT_FAILURE);
	struct epoll_event	event;
	event.events = EPOLLIN;
	event.data.fd = srv_socket;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, srv_socket, &event) == -1)
		return (std::cerr << "ERROR : EPOOL CTL" << std::endl, EXIT_FAILURE);
	std::vector<struct epoll_event> events(10);
	while (1)
	{
		int num_event = 0;
		if ((num_event = epoll_wait(epoll_fd, events.data(), events.size(), -1)) == -1)
			return (std::cerr << "ERROR : EPOOL WAIT" << std::endl, 0);
		for (int i = 0; i < num_event; i ++)
		{
			if (events[i].data.fd == srv_socket)
			{
				if ((cli_socket = accept(srv_socket, (struct sockaddr *)&cli_addr, &cli_addrlen)) == -1)
					return (std::cerr << "ERROR : accept connection" << std::endl, EXIT_FAILURE);
				struct epoll_event	cli_event;
				cli_event.data.fd = cli_socket;
				cli_event.events = EPOLLIN | EPOLLOUT;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cli_socket, &cli_event) == -1)
					return (std::cerr << "ERROR : EPOOL CTL FOR CLIENT" << std::endl, EXIT_FAILURE);
			}
		}
	}
    return 0;
}