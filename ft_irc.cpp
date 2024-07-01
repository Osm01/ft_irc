#include "server.h"
# include "chanel.h"

int main()
{
	Server srv(8080, "PASS");
	if (srv.Setup_Sever())
		srv.Multiplexing();
	return (0);
}