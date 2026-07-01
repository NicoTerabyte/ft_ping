#include "includes/utils.h"

void sighandler(int signum)
{
	printf("Got signal %d ping statistics coming soon....\n", signum);
	// function to cleanup malloc'd data

	loop_var = 1;
}

//for now when dns_status is one i don't free the packet->result
//this is a temporary fix since it would give me a segfault if i free it when the dns
//is not resolved
//non serve liberare packet->ip_name perché è nello stack
void	free_anything(t_dest_data *packet, int dns_status)
{
	printf("freeing stuff\n");

	if (packet && packet->dns_name)
		free(packet->dns_name);

	if ((packet && packet->result) && dns_status != 1)
	{
		printf("freeaddrinfo acting");
		freeaddrinfo(packet->result);
	}
	if (packet && packet->ip)
		free(packet->ip);

	if (packet->sock_r >= 0)
		packet->sock_r = -1;

	printf("%s", OK_CHECKOUT);
}
