#include "./includes/utils.h"
#include <string.h>


// for now it's just inlcudes
//STEP 1 inizializzare le variabili "semplici"
//buffer->packet seems to not be used
void	setup_dest_data_to_zero(t_dest_data	*packet)
{
	printf("Initiliazing struct\n");
	//packet = malloc(sizeof(t_dest_data));
	packet->sock_r = 0;
	packet->dns_name = 0;
	packet->ip = 0;
}

/*
HERE WE JUST DO A SETUP FOR ANY TYPE OF DATA ELABORATION
Like the one to resolve the dns refer to other files
qui tutto ok

i valori relativi al destinatario sono:
dns_name
hints
ip -> no purpose for now
result
dest
dns_ip
fqdn
*/
int	icmp_dest_socket_setup(t_dest_data *packet, char *dns_name)
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

	// non l'ho ancora usato
	packet->ip = calloc(NI_MAXHOST, sizeof(char));
	return (0);
}



void	icmp_packet_to_send_setup(t_icmp_packet_to_send *packet_to_send)
{
	memset(packet_to_send->packet_content, 0, sizeof(packet_to_send->packet_content));
	packet_to_send->icmp_packet.type = ECHO_REQUEST;
	packet_to_send->icmp_packet.code = 0;
	packet_to_send->icmp_packet.un.echo.id = htons(getpid());
	packet_to_send->icmp_packet.un.echo.sequence = 1;
	packet_to_send->icmp_packet.checksum = checksum_interpretation_creation(&packet_to_send->icmp_packet, sizeof(packet_to_send->icmp_packet), 0, 0);

	printf("message size %zu icmp packet size %zu\n", sizeof(packet_to_send->packet_content), sizeof(packet_to_send->icmp_packet));
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


