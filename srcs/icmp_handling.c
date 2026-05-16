#include "includes/utils.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	resolve_address_to_ip(t_icmp_packet *packet)
{
	int	client_ip = 0;
	client_ip = getaddrinfo(packet->dns_name, NULL, &packet->hints, &packet->result);

	if (client_ip != 0)
	{
		fprintf(stderr, "Failure from getaddrinfo %s\n", gai_strerror(client_ip));
		printf("%s\n", SHREK);
		return (EXIT_FAILURE);
	}

	return (0);
}


void	print_dns(t_icmp_packet packet)
{
	// int sock_len = packet.result->ai_addrlen;
	while(packet.result != NULL)
	{
		printf("what we have %s",packet.result->ai_canonname);
	}
}
