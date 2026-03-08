#include "./includes/utils.h"


//print to emulate
// 64 bytes from arg: icmp_seq=1 ttl=53 time=193 ms
// full FQDN = full domain not dns significa non dare pezzi separati
//PING www.google.com (192.178.204.105) 56(84) bytes of data.
// 64 bytes from wz-in-f105.1e100.net (192.178.204.105): icmp_seq=1 ttl=107 time=57.9 ms
//il FQDN è www.google.com





int main(int argc, char **argv)
{
	t_icmp_packet	packet;

	signal(SIGINT, sighandler);
	(void)argv;
	if (argc <= 1)
	{
		printf("Not enough arguments\n");
		printf("%s", SHREK);
		exit(0);
	}

	if (raw_socket_setup(&packet))
	{
		printf("%s", SHREK);
		free_anything(&packet);
		exit(1);
	}
	receive_raw_data(&packet);
	printf("tests passed all initialized\n");

	printf("checking ethernet header\n");
	print_eth(&packet);

	while (1)
	{
		printf("sending packet....\n");
		sleep(1);
	}
	return (0);
}
