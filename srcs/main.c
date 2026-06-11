#include "./includes/utils.h"
#include <stdio.h>


//print to emulate
// 64 bytes from arg: icmp_seq=1 ttl=53 time=193 ms
// full FQDN = full domain not dns significa non dare pezzi separati
//PING www.google.com (192.178.204.105) 56(84) bytes of data.
// 64 bytes from wz-in-f105.1e100.net (192.178.204.105): icmp_seq=1 ttl=107 time=57.9 ms
//il FQDN è www.google.com

// è tutto da costruire?
// dns è da intepretare
// checksum da calcolare?

int	loop_var = 0;


int main(int argc, char **argv)
{
	t_icmp_packet				packet;
	int 						res_of_dns;
	//TODO:
	// t_raw_socket_sniffer_packet	sniffer;

	signal(SIGINT, sighandler);
	if (argc <= 1)
	{
		printf("Not enough arguments\n");
		printf("%s", SHREK);
		exit(0);
	}
	setup_packet_to_zero(&packet);
	if (icmp_socket_setup(&packet, argv[1]))
	{
		printf("%s", SHREK);
		free_anything(&packet, 1);
		exit(1);
	}
	//receive_raw_data(&packet); ONLY FOR SNIFFING AT LEVEL 2
	printf("tests passed all initialized\n");

	if (check_which_lookup(&packet) == 0)
		res_of_dns= dns_lookup(&packet);
	else
		res_of_dns = reverse_dns_lookup(&packet);

	printf("resolution of dns went: %d\n", res_of_dns);
	if (res_of_dns == 0)
	{
		printf("%s", OK_CHECKOUT);
	}
	else
	{
		free_anything(&packet, 0);
		printf("GOOFY AHH ERROR WHILE RESOLVING THE ADDRESS\n");
		printf(PACKET_ERROR);
		exit(1);
	}


	while (loop_var == 0)
	{
		printf("sending packet....\n");
		sleep(1);
	}
	free_anything(&packet, 0);
	return (0);
}
