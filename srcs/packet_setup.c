#include "./includes/utils.h"

// for now it's just inlcudes

void	setup_packet_to_zero(t_icmp_packet	*packet)
{
	printf("Initiliazing struct\n");
	printf("is the packet real? %lu\n", sizeof(packet));
	//packet = malloc(sizeof(t_icmp_packet));
	packet->buffer = 0;
	packet->sock_r = 0;
	packet->buflen = 0;
}

int	raw_socket_setup(t_icmp_packet *packet)
{
	packet->sock_r = socket(AF_PACKET, SOCK_RAW, htons(IPPROTO_ICMP));

	if (packet->sock_r < 0)
	{
		perror("Error during creation of the socket");
		free_anything(packet);
		return (-1);
	}
	return (0);
}


