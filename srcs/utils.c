#include "./includes/utils.h"

// for now it's just inlcudes

int	raw_socket_setup(t_icmp_packet *packet)
{

	packet->sock_r = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

	if (packet->sock_r < 0)
	{
		perror("Error during creation of the socket");
		return (-1);
	}
	return (0);
}

int		receive_raw_data(t_icmp_packet *packet)
{
	// unsigned char	*buffer;
	int				saddr_len;
	ssize_t			buflen;

	packet->buffer = (unsigned char *) malloc(65536); //! remember to free
	memset(packet->buffer, 0, 65536);
	saddr_len = sizeof(packet->saddr);
	buflen = recvfrom(packet->sock_r, packet->buffer, 65536, 0, &packet->saddr, (socklen_t *)&saddr_len);

	if (buflen < 0)
	{
		perror("Error during receiving of raw data");
		return -1;
	}
	//do i need it in the struct?
	return buflen;
}

void sighandler(int signum)
{
	printf("Got signal %d ping statistics coming soon....", signum);
	// function to cleanup malloc'd data
	exit(0);
}
