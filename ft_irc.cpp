#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

int main()
{
	int                 srv_socket, cli_socket, opt = 1;
	struct sockaddr_in  srv_addr, cli_addr, data;
    socklen_t           srv_addrlen, cli_addrlen, data_len;
    

    if ((srv_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return  (std::cerr << "Cannot create a socket" << std::endl , 0);

	if (setsockopt(srv_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		return (std::cerr << "ERROR on reusing the port" << std::endl, 0);
    std::memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(1234);
	srv_addr.sin_addr.s_addr = INADDR_ANY;
	srv_addrlen = sizeof (srv_addr);
    if (bind(srv_socket, (const sockaddr *) &srv_addr , srv_addrlen) == -1)
		return (std::cerr << "Cannot bind the endpoint of socket" << std::endl, 0);

	if (getsockname(srv_socket, (sockaddr *) &data, &data_len) == -1)
		return (std::cerr << "Error cannot get socket name", 0);
	if (listen(srv_socket, 2) == -1)
		return (std::cerr << "Cannot listing for socket" << std::endl , 0);
	while (true)
	{
		if ((cli_socket = accept(srv_socket, (sockaddr *) &cli_addr, &cli_addrlen)) == -1)
			return  (std::cerr << "ERROR accept connection form client" << std::endl , 0);
		std::cout << cli_socket << std::endl;
	}
    return 0;
}