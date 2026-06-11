#include "includes/utils.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>

int	dns_lookup(t_icmp_packet *packet)
{
	int	client_ip = 0;

	printf("trying to retrieve ip from %s\n", packet->dns_name);
	client_ip = getaddrinfo(packet->dns_name, NULL, &packet->hints, &
		packet->result);

	//? human readable host->ip?
	if (client_ip != 0)
	{
		fprintf(stderr, "Failure from getaddrinog in dns_lookup func \n");
		printf("%s\n", SHREK);
		return (EXIT_FAILURE);
	}

	//Mettiamo il destinatario nel valore giusto della struct:
	packet->dest = (struct sockaddr_in *)packet->result;
	printf("%s is in reality %s\n", packet->dns_name, inet_ntoa(packet->dest->sin_addr));

	return (0);
}

void	check_if_packet_is_address_or_digits(t_icmp_packet	*packet)
{
	(void)packet;
}

void	print_dns(t_icmp_packet packet)
{
	// int sock_len = packet.result->ai_addrlen;
	//! not working
	while(packet.result != NULL)
	{
		printf("what we have %s",packet.result->ai_canonname);
	}
}

int	reverse_dns_lookup(t_icmp_packet *packet)
{
	int status;

	(void)packet;
	//fermati bro non stai usando il cervello......
	(void)status;
	return 0;
}


int	check_which_lookup(t_icmp_packet *packet)
{
	
}
