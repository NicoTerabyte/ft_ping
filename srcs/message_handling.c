#include "includes/utils.h"
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>


/*
This function is made in order to get the real answer we are focusing on
quoting an article regarding the ping behavior
"When you receive data from a raw socket,
you get the entire IP packet, not just the ICMP payload:
"
so i have to strip some of the data to get the real results

if i wouldn't take the icmp only the packet would be too bigthe guy added

i have to get the buffer to do what i have to do (the answer itself contains the header and all

remember
ipv4 header = 20 bytes
icmp header = 8 bytes
payload content = 64 bytes

sendto has sent 72 (64 + 8)

the function checks pid if the message we receive has the ECHO_REPLY code
*/

static int	check_packet_code_type(t_communication_manager *betweener)
{
	//this has to be done to get the icmp_packet
	struct ip *ip_header = (struct ip *)&betweener->answer;
	int ip_len = ip_header->ip_hl * 4;
	struct icmphdr *icmp_packet = 0;

	betweener->retrieved_ttl = ip_header->ip_ttl;
	icmp_packet = (struct icmphdr *)(betweener->answer + ip_len);

	if (icmp_packet->type == ICMP_ECHOREPLY)
	{
		if ((uint16_t)getpid() == ntohs(icmp_packet->un.echo.id))
			return ICMP_ECHOREPLY;
	}
	else if (icmp_packet->type == ICMP_TIME_EXCEEDED || icmp_packet->type == ICMP_DEST_UNREACH)
	{
		//we have to surpass the error packet, that comes with the packet we sent
		//in order to know if the error is related to our packet by checking the pid
		struct ip *orig_ip = (struct ip*)((char*)icmp_packet + sizeof(struct icmphdr));
		int orig_ip_len = orig_ip->ip_hl * 4;

		//we get to our packet in order to check if it really is our request that went in error
		struct icmphdr *orig_icmp = (struct icmphdr *)((char *)orig_ip + orig_ip_len);

		if (ntohs(orig_icmp->un.echo.id) == (uint16_t)getpid())
			return icmp_packet->type;
	}
	//the packet doens't belong to us
	return -1;
}

/*

this function calculates the ttl (time to live of the packet)

*/

static int remove_headers_and_get_packet_size(t_communication_manager *betweener)
{

	struct ip *ip_header = (struct ip *)&betweener->answer;
	int ip_len = ip_header->ip_hl * 4;
	// struct icmphdr *real_packet = 0;

	// real_packet = (struct icmphdr *)(betweener->answer + ip_len);

	return ((((int)betweener->res_of_receiving - ip_len)));
}

static void print_hexdump(const void *addr) {
	const struct ip *ip_hdr = (struct ip *)addr;
	const unsigned char *bytes = (const unsigned char *)ip_hdr;
	int ip_len = ip_hdr->ip_hl * 4;
	char	src_ip[INET_ADDRSTRLEN];
	char	dst_ip[INET_ADDRSTRLEN];

	printf("IP Hdr Dump:\n ");
	for (int i = 0; i < ip_len; i++) {
		printf("%02x", bytes[i]);
		if (i % 2 == 1) printf(" ");
	}

	printf("\n");
	printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src	Dst	Data\n");
	printf(" %1x", ip_hdr->ip_v);
	printf("  %1x", ip_hdr->ip_hl);
	printf("  %02x", ip_hdr->ip_tos);
	printf(" %04x", ntohs(ip_hdr->ip_len));
	printf(" %04x", ntohs(ip_hdr->ip_id));
	// find flags 3 bits
	printf("   %01x ", (ntohs(ip_hdr->ip_off) >> 13) & 0x7);
	printf("%04x ", ntohs(ip_hdr->ip_off) & 0x1FFF);
	printf(" %02x", ip_hdr->ip_ttl);
	printf("  %02x", ip_hdr->ip_p);
	printf(" %04x", ntohs(ip_hdr->ip_sum));

	inet_ntop(AF_INET, &(ip_hdr->ip_src), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip_hdr->ip_dst), dst_ip, INET_ADDRSTRLEN);
	printf(" %s", src_ip);
	printf("  %s", dst_ip);
	printf("\n");
}

/*
this function has to both prints the details of the packet that went in TIME_EXCEEDED
error and it's own hedump data that is a more raw representation of the bytes that were exchanged
from the packet. the print_hexdump will handle all of that
*/
static void handle_exceeded_time_print(t_communication_manager *betweener)
{
	struct ip *ip_header = (struct ip *)&betweener->answer;
	int ip_len = ip_header->ip_hl * 4;
	struct icmphdr *icmp_err = (struct icmphdr *)(betweener->answer + ip_len);

	struct ip *orig_ip = (struct ip *)((char *)icmp_err + sizeof(struct icmphdr));
	int orig_ip_len = orig_ip->ip_hl * 4;
	struct icmphdr *orig_icmp = (struct icmphdr *)((char *)orig_ip + orig_ip_len);

	printf("ICMP: type %d, code %d, size %d, id 0x%04x, seq 0x%04x\n",
			orig_icmp->type,
			orig_icmp->code,
			ntohs(orig_ip->ip_len) - orig_ip_len,
			ntohs(orig_icmp->un.echo.id),
			ntohs(orig_icmp->un.echo.sequence));

	print_hexdump((unsigned char *)orig_ip);
}
/*
this functions print some packet statistic, size and all, and checks if
whoever answered is the same that we are trying to comunicate wit

struct ip *orig_ip = (struct ip*)((char*)icmp + sizeof(struct icmphdr));
int orig_ip_len = orig_ip->ip_hl * 4;
struct icmphdr *orig_icmp = (struct icmphdr*)((char*)orig_ip + orig_ip_len);

added the is_verbose var for handling -v prints in case of timeout
*/
int		package_print_handling(t_communication_manager *betweener, int seq, struct timeval send_time, int is_verbose, t_stats *stats)
{
	int					packet_type;
	char				sender_ip[INET_ADDRSTRLEN];
	struct				sockaddr_in	*ipv4_caster;
	struct				timeval answer_time;
	double				total_time;

	ipv4_caster = (struct sockaddr_in *)&betweener->answerer_to_ping;
	packet_type = check_packet_code_type(betweener);
	memset(sender_ip, 0, sizeof(sender_ip));
	inet_ntop(AF_INET, &(ipv4_caster->sin_addr), sender_ip, INET_ADDRSTRLEN);
	if (packet_type == ICMP_ECHOREPLY)
	{
		gettimeofday(&answer_time, NULL);
		total_time = (answer_time.tv_sec - send_time.tv_sec) * 1000.0 + (answer_time.tv_usec - send_time.tv_usec) / 1000.0;

		//updating rtt for stats
		stats->packets_received++;
		stats->rtt_sum += total_time;
		stats->rtt_sum_sq += (total_time * total_time);

		if (stats->packets_received == 1 || total_time < stats->rtt_min)
			stats->rtt_min = total_time;
		if (total_time > stats->rtt_max)
			stats->rtt_max = total_time;

		//back to printing
		printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.1f\n", remove_headers_and_get_packet_size(betweener), sender_ip, seq, betweener->retrieved_ttl, total_time);
		return 1;
	}
	else if (packet_type == ICMP_TIME_EXCEEDED)
	{
		printf("%zd bytes from %s: Time to live exceeded\n", betweener->res_of_receiving, sender_ip);
		if (is_verbose)
			handle_exceeded_time_print(betweener);
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
	manager->retrieved_ttl			= 0;
}


int		package_message_loop(t_communication_manager *betweener, t_dest_data *dest, int seq, struct timeval send_time, int is_verbose, t_stats *stats)
{
	while (1)
	{
		betweener->poll_status = poll(&betweener->traffic_manager, 1, 1000);
		if (betweener->poll_status > 0)
		{
			// printf("packets bytes %zd\n", betweener->res_of_message);
			betweener->res_of_receiving = recvfrom(dest->sock_r, &betweener->answer, sizeof(betweener->answer), 0, &betweener->answerer_to_ping, &betweener->answer_addr_len);
			if (betweener->res_of_receiving != -1)
			{
				if (package_print_handling(betweener, seq, send_time, is_verbose, stats) == 1)
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
