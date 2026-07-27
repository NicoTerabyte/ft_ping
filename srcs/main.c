#include "./includes/utils.h"
#include <bits/types/struct_timeval.h>
#include <sys/time.h>




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
da fare sarebbe controllare le apposite librerie per questo.


particolare il fatto che sendto ti dice solo se l'invio è andato bene (parlo dell'invio del pacchetto)
serve la funzione gemella che gestisca la risposta su quel socket.

answerer_to_ping mi serve per capire se colui a cui ho mandato il pacchetto è lo stesso che mi ha risposto
the asnwerer doens't work when i send the data with "www.google.com" but if i use the og ping and i do it with it works
but it still gives ame a lot of problems ip is wrong and it works only when og ping is working..... strange
*/

int main(int argc, char **argv)
{
	t_icmp_packet_to_send		packet_to_send;
	t_dest_data					destinatary_data;
	t_communication_manager		betweener; // handler of te communication between me and the dest
	t_stats						stats;
	int 						res_of_dns; //status
	int							seq = 1; // counter di pacchetti
	struct timeval				send_time; //TODO calculate time
	int							is_verbose = 0;

	memset(&stats, 0, sizeof(t_stats));
	signal(SIGINT, sighandler);
	if (argc <= 1)
	{
		printf("ft_ping: usage error: Destination address required\n");
		// printf("%s", SHREK);
		exit(1);
	}
	else if (argc == 3 && strcmp(argv[1],"-v") == 0)
	{
		is_verbose = 1;
		if (argc < 3)
		{
			printf("ft_ping: usage error: Destination address required\n");
			// printf("%s", SHREK);
			exit(1);
		}
	}
	else if (strcmp(argv[1], "-?") == 0)
	{
		printf("\nUsage\n  ping [options] <destination>\n\nOptions:\n  <destination>      dns name or ip address\n  -v                 verbose output\n  -?                 give this help list\n\n");
		exit(0);
	}
	setup_dest_data_to_zero(&destinatary_data);
	if (is_verbose != 1)
	{
		if (icmp_dest_socket_setup(&destinatary_data, argv[1]))
		{
			printf("ft_ping: ERROR you must be root to run\n");
			// printf("%s", SHREK);
			free_anything(&destinatary_data, 1);
			exit(1);
		}
	}
	else
	{
		if (icmp_dest_socket_setup(&destinatary_data, argv[2]))
		{
			printf("ft_ping: ERROR you must be root to run\n");
			// printf("%s", SHREK);
			free_anything(&destinatary_data, 1);
			exit(1);
		}
	}

	res_of_dns = dns_lookup(&destinatary_data);
	res_of_dns = reverse_dns_lookup(&destinatary_data, res_of_dns);

	// if (res_of_dns == 0)
	// 	printf("%s", OK_CHECKOUT);
	if (res_of_dns != 0)
	{
		printf("ping: %s: Name or service not known\n", destinatary_data.dns_name);
		free_anything(&destinatary_data, 0);
		// printf(PACKET_ERROR);
		exit(1);
	}

	icmp_packet_to_send_setup(&packet_to_send);

	if (is_verbose == 1)
	{
		printf("ping: sock4.fd %d (socktype: SOCK_RAW), hints.ai_family: AF_UNSPEC\n", destinatary_data.sock_r);
		printf("ping: ai->ai_family: AF_INET ai->canonname: %s\n", destinatary_data.dns_name);
	}
	printf("PING %s (%s) %zu(84) bytes of data\n", destinatary_data.dns_name, destinatary_data.dns_ip, sizeof(packet_to_send.packet_content));

	gettimeofday(&stats.start_time, NULL);
	communication_manager_setup(&betweener, destinatary_data.sock_r);
	while (loop_var == 0 && betweener.res_of_message != -1)
	{
		gettimeofday(&send_time, NULL);
		betweener.res_of_message = sendto(destinatary_data.sock_r, &packet_to_send, sizeof(packet_to_send), 0, (struct sockaddr*)destinatary_data.dest, sizeof(*destinatary_data.dest));

		if (betweener.res_of_message == -1)
		{
			printf("The message went wrong, corrupted???\n");
			printf("or no internet connection\n");
			break ;
		}
		else
			stats.packets_transmitted++;
		//Loop for message receiving
		package_message_loop(&betweener, &destinatary_data, seq, send_time, is_verbose, &stats);
		seq++;
		sleep(1);
	}

	// printf("--- %s ping statistics ---\n", destinatary_data.dns_name);
	print_ping_statistics(&stats, destinatary_data.dns_name);
	free_anything(&destinatary_data, 0);
	close(destinatary_data.sock_r);
	// printf("all close\n %s", ending);
	return (0);
}
