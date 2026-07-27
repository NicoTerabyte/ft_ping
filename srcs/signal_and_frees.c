#include "includes/utils.h"

void sighandler(int signum)
{
	//printf("Got signal %d ping statistics coming soon....\n", signum);
	//function  to handle ctrl + C signal
	(void)signum;
	loop_var = 1;
}

/*
for now when dns_status is one i don't free the packet->result
this is a temporary fix since it would give me a segfault if i free it when the dns
is not resolved
non serve liberare packet->ip_name perché è nello stack
*/
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
	// if (packet && packet->ip)
	// 	free(packet->ip);

	if (packet->sock_r >= 0)
		packet->sock_r = -1;

	// printf("%s", OK_CHECKOUT);
}


void	print_ping_statistics(t_stats *stats, char *target_name)
{
	int loss = 0;
	double total_time_ms = 0;

	gettimeofday(&stats->end_time, NULL);
	total_time_ms = (stats->end_time.tv_sec - stats->start_time.tv_sec) * 1000.0 +
					(stats->end_time.tv_usec - stats->start_time.tv_usec) / 1000.0;

	// Calcolo % di perdita
	if (stats->packets_transmitted > 0)
		loss = ((stats->packets_transmitted - stats->packets_received) * 100) / stats->packets_transmitted;

	printf("--- %s ping statistics ---\n", target_name);
	printf("%ld packets transmitted, %ld received, %d%% packet loss, time %.0fms\n",
			stats->packets_transmitted, stats->packets_received, loss, total_time_ms);

	// Evita la divisione per zero e stampa i valori rtt
	if (stats->packets_received > 0)
	{
		double avg = stats->rtt_sum / stats->packets_received;
		double variance = (stats->rtt_sum_sq / stats->packets_received) - (avg * avg);
		double mdev = sqrt(variance);

		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
			   stats->rtt_min, avg, stats->rtt_max, mdev);
	}
}
