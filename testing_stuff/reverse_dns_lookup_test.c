#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

// #define _DEFAULT_SOURCE

void	dns_lookup(struct sockaddr_in *packet)
{
	//ok mi so perso qualcosa
	struct	sockaddr_in	addr;
	struct	addrinfo	hints, res;

	//eh ok serve sempre sapere quale pacchetto dobbiamo toccare
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;


}

int main(int argc, char *argv[])
{
	struct in_addr addr;

	if (argc != 2)
	{
		fprintf(stderr, "%s <dotted-address>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (inet_aton(argv[1], &addr) == 0)
	{
		fprintf(stderr, "Invalid address\n");

		exit(EXIT_FAILURE);
	}

	printf("%s\n", inet_ntoa(addr));
	exit(EXIT_SUCCESS);
}
