#include "includes/utils.h"
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <unistd.h>




/*
This method is made in order to get the real answer we are focusing on
quoting an article regarding the ping behavior
"When you receive data from a raw socket,
you get the entire IP packet, not just the ICMP payload:
"
so i have to strip some of the data to get the real results

if i wouldn't take the icmp only the packet would be too big.
the guy added
"The IP header length is variable (20–60 bytes), so you have to parse it correctly to find your ICMP data"

i have to get the buffer to do what i have to do (the answer itself contains the header and all)

ip_len is the header of the packet, we don't needed it

remember
ipv4 header = 20 bytes
icmp header = 8 bytes
payload content = 64 bytes

sendto has sent 72 (64 + 8)
*/
static int remove_headers_and_get_packet_size(t_communication_manager *betweener)
{
	struct ip *ip_header = (struct ip *)&betweener->answer;
	int ip_len = ip_header->ip_hl * 4;
	struct icmphdr *real_packet = 0;

	//does it leak?
	real_packet = (struct icmphdr *)(betweener->answer + ip_len);
	printf("my pid %d, packet pid %d\n",(uint16_t)getpid(), ntohs(real_packet->un.echo.id));


	return (((betweener->res_of_receiving - ip_len)));
}



/*
this functions print some packet statistic, size and all, and checks if
whoever answered is the same that we are trying to comunicate with

the size of the packet is not well defined i have to change it later (i mean the use of the var res_of_receiving)

it's to rework because it's the packet itself that needs to be shifted to get the icmp sign

i need to check the pids correctly, how though, i have to separate the packet size and the
icmp pid 2 functions

*/
int		print_msg_rec_data(t_communication_manager *betweener, int seq, t_dest_data destinatary)
{
	char				sender_ip[1024];
	struct	sockaddr_in	*ipv4_caster;

	ipv4_caster = (struct sockaddr_in *)&betweener->answerer_to_ping;

	memset(sender_ip, 0, sizeof(sender_ip));
	inet_ntop(AF_INET, &(ipv4_caster->sin_addr), sender_ip, INET_ADDRSTRLEN);
	if (strcmp(sender_ip, destinatary.dns_ip) == 0)
	{
		printf("%d bytes from %s: icmp_seq=%d\n", remove_headers_and_get_packet_size(betweener), sender_ip, seq);
		return 1;
	}
	else
	{
		printf("someone %s is answering instead of the destinatary %s\n", sender_ip, destinatary.dns_ip);
		return -1;
	}

	// printf("The  address of the sender is not defined uknown answerer\n");
	return -1;
}

void	communication_manager_setup(t_communication_manager *manager, int sock_to_monitor)
{
	memset(manager->answer, 0, sizeof(manager->answer));
	manager->res_of_message = 0;
	manager->res_of_receiving = 0;
	manager->answer_addr_len = sizeof(manager->answerer_to_ping);

	manager->traffic_manager.fd = sock_to_monitor;
	manager->traffic_manager.events = POLLIN;
}

int		package_message_loop(t_communication_manager *betweener, t_dest_data *dest, int seq)
{
	while (1)
	{
		betweener->poll_status = poll(&betweener->traffic_manager, 1, 2000);
		if (betweener->poll_status > 0)
		{
			// printf("packets bytes %zd\n", betweener->res_of_message);
			betweener->res_of_receiving = recvfrom(dest->sock_r, &betweener->answer, sizeof(betweener->answer), 0, &betweener->answerer_to_ping, &betweener->answer_addr_len);
			if (betweener->res_of_receiving != -1)
			{
				if (print_msg_rec_data(betweener, seq, *dest) == 1)
					return 1;
				continue;
			}
			else
			{
				printf("the packet wasn't retrieved yet\n");
				continue;
			}
		}
		else if (betweener->poll_status == 0)
		{
			printf("request timeout\n");
			return -2;
		}
		else
		{
			printf("something went wrong while polling.....\n");
			return -1;
		}
	}
}
