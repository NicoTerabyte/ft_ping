#include "./includes/utils.h"

// for now it's just inlcudes
//STEP 1
void	setup_packet_to_zero(t_icmp_packet	*packet)
{
	printf("Initiliazing struct\n");
	printf("is the packet real? %lu\n", sizeof(packet));
	//packet = malloc(sizeof(t_icmp_packet));
	packet->buffer = 0;
	packet->sock_r = 0;
	packet->buflen = 0;
}

int	icmp_socket_setup(t_icmp_packet *packet)
{
	packet->sock_r = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (packet->sock_r < 0)
	{
		perror("Error during creation of the socket");
		return (-1);
	}
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


