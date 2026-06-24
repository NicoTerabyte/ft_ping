#include "./includes/utils.h"
#include <stdio.h>


//print to emulate
// 64 bytes from arg: icmp_seq=1 ttl=53 time=193 ms
// full FQDN = full domain not dns significa non dare pezzi separati
//PING www.google.com (192.178.204.105) 56(84) bytes of data.
// 64 bytes from wz-in-f105.1e100.net (192.178.204.105): icmp_seq=1 ttl=107 time=57.9 ms
//il FQDN è www.google.com

// è tutto da costruire?
// checksum da calcolare?

int	loop_var = 0;

/*
Note relative al codice:

TODO:
t_raw_socket_sniffer_packet	sniffer;
receive_raw_data(&packet); ONLY FOR SNIFFING AT LEVEL 2

line 59
	se non torna 1 vuol dire che è un indirizzo di dominio "www.google.com" quindi in ordine
	per fare in seguito il reverse dns devo PER FORZA reperire l'ip
	if (is_dns_needed(&packet) == 0)
	da solo controlla anche l'ip stesso zio pera

line 59:
	questo va fatto per il nome per intero
	il www.google.it non ci interessa, è solo usato per trovare l'ip
	se invece abbiamo già l'ip getaddrinfo non farà il dns_lookup da solo
	ma la funziona fa comodo per il setup del destinatario packet->dest

devo gestire meglio gli errori per il reperimento del dns e del reverse
da fare sarebbe controllare le apposite librerie per questo
*/

int main(int argc, char **argv)
{
	t_dest_packet				packet;
	int 						res_of_dns;
	int							seq;

	signal(SIGINT, sighandler);
	if (argc <= 1)
	{
		printf("Not enough arguments\n");
		printf("%s", SHREK);
		exit(0);
	}
	setup_packet_to_zero(&packet);
	if (icmp_dest_socket_setup(&packet, argv[1]))
	{
		printf("%s", SHREK);
		free_anything(&packet, 1);
		exit(1);
	}

	printf("tests passed all initialized\n");
	res_of_dns = dns_lookup(&packet);

	res_of_dns = reverse_dns_lookup(&packet, res_of_dns);

	printf("resolution of dns went: %d\n", res_of_dns);

	if (res_of_dns == 0)
		printf("%s", OK_CHECKOUT);
	else
	{
		free_anything(&packet, 0);
		printf("GOOFY AHH ERROR WHILE RESOLVING THE ADDRESS\n");
		printf(PACKET_ERROR);
		exit(1);
	}

	seq = 1;
	while (loop_var == 0)
	{
		printf("64 bytes from %s: icmp_seq=%d\n", packet.dns_ip, seq);
		sleep(1);
		seq++;
	}
	free_anything(&packet, 0);
	printf("finito\n %s", ending);
	return (0);
}
