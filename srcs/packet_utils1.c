#include "./includes/utils.h"

int		receive_raw_data(t_icmp_packet *packet)
{
	// unsigned char	*buffer;
	int				saddr_len;
	packet->buffer = (unsigned char *) malloc(65536); //! remember to free
	memset(packet->buffer, 0, 65536);

	saddr_len = sizeof(packet->saddr);
	packet->buflen = recvfrom(packet->sock_r, packet->buffer, 65536, 0, &packet->saddr, (socklen_t *)&saddr_len);

	if (packet->buflen < 0)
	{
		perror("Error during receiving of raw data");
		free_anything(packet);
		return (-1);
	}

		packet->eth = (struct ethhdr *)(packet->buffer);
	return (0);
}

void sighandler(int signum)
{
	printf("Got signal %d ping statistics coming soon....", signum);
	// function to cleanup malloc'd data
	exit(0);
}

void	free_anything(t_icmp_packet *packet)
{
	(void)packet;
	printf("To implement....");
}

void	print_eth(t_icmp_packet *packet) //?not useful
{
	printf("\nEthernet Header\n");
	printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", packet->eth->h_source[0],packet->eth->h_source[1],packet->eth->h_source[2],packet->eth->h_source[3],packet->eth->h_source[4],packet->eth->h_source[5]);
	printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", packet->eth->h_dest[0],packet->eth->h_dest[1],packet->eth->h_dest[2],packet->eth->h_dest[3],packet->eth->h_dest[4],packet->eth->h_dest[5]);
	printf("\t|-Protocol : %d\n", packet->eth->h_proto);
}



void	get_ip_header(t_icmp_packet *packet)
{
	//* Starting from level 3 remember
	// i need to do a little map to conceptualize
	// how this part is gonna be into the initialization of the struct
	unsigned short iphdrlen;
	packet->ip = (struct iphdr*)packet->buffer;
	memset(&packet->source, 0, sizeof(packet->source));
	memset(&packet->dest, 0, sizeof(packet->dest));
	packet->dest.sin_addr.s_addr = packet->ip->daddr;
}
