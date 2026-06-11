#include "./includes/utils.h"
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>

// for now it's just inlcudes
//STEP 1 inizializzare le variabili "semplici"
void	setup_packet_to_zero(t_icmp_packet	*packet)
{
	printf("Initiliazing struct\n");
	//packet = malloc(sizeof(t_icmp_packet));
	packet->buffer = 0;
	packet->sock_r = 0;
	packet->buflen = 0;
	packet->dns_name = 0;
	packet->ip = 0;
}

/*
HERE WE JUST DO A SETUP FOR ANY TYPE OF DATA ELABORATION
Like the one to resolve the dns refer to other files
qui tutto ok
*/
int	icmp_socket_setup(t_icmp_packet *packet, char *dns_name)
{
	packet->sock_r = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (packet->sock_r < 0)
	{
		perror("Error during creation of the socket");
		return (-1);
	}

	packet->dns_name = strdup(dns_name);

	// ? setup of hints for domain name resolution
	bzero(&packet->hints, sizeof(packet->hints));

	packet->result = NULL;

	packet->hints.ai_family = AF_INET;
	packet->hints.ai_socktype = SOCK_RAW;
	packet->hints.ai_protocol = IPPROTO_ICMP;

	//per reverse dns look up penso, non lo so fra ahahahah
	packet->ip = calloc(NI_MAXHOST, sizeof(char));
	return (0);
}

//this function is not used anymore, it is for raw sockets
int	raw_socket_setup(t_raw_socket_sniffer_packet *packet)
{
	packet->sock_r = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

	if (packet->sock_r < 0)
	{
		perror("Error during creation of the socket");
		return (-1);
	}
	return (0);
}


