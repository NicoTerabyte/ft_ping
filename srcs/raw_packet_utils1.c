#include "./includes/utils.h"
#include <stdio.h>

//just for self knowledge
int		setup_raw_socket(t_raw_socket_sniffer_packet *packet)
{
	packet->sock_r = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (packet->sock_r < 0)
	{
		perror("FAILED ON CREATING SOCKET \n");
		perror(SHREK);
		return (-1);
	}
	return (0);
}

//this is to receive data network packets
//raw socket impl
int		receive_raw_data(t_raw_socket_sniffer_packet *packet)
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
		free_anything_raw_socket(packet);
		return (-1);
	}

		packet->eth = (struct ethhdr *)(packet->buffer);
	return (0);
}

//raw socket impl
void	extract_ip_header_from_raw_packet(t_raw_socket_sniffer_packet *packet)
{
	//TODO not mandatory though
	(void)packet;
}


void	free_anything_raw_socket(t_raw_socket_sniffer_packet *packet)
{
	(void)packet;
	printf("To implement....");
}

// extraction of ethernet header
void	print_eth(t_raw_socket_sniffer_packet *packet) //?not useful anymore
{
	printf("\nEthernet Header\n");
	printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", packet->eth->h_source[0],packet->eth->h_source[1],packet->eth->h_source[2],packet->eth->h_source[3],packet->eth->h_source[4],packet->eth->h_source[5]);
	printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", packet->eth->h_dest[0],packet->eth->h_dest[1],packet->eth->h_dest[2],packet->eth->h_dest[3],packet->eth->h_dest[4],packet->eth->h_dest[5]);
	printf("\t|-Protocol : %d\n", packet->eth->h_proto);
}



