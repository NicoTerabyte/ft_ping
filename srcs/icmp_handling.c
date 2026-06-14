#include "includes/utils.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

/*
è interessante che il getaddrinfo sa se fare il dns_lookup in base a cosa gli passi
se gli passi un dominio valido "www.google.com" lo fa, se gli passi un ip valido "1.1.1.1" sa
di NON doverlo fare ma comunque procede con il setup corretto

line 29:
	essendo che result è un sockaddr generico, noi stiamo lavorando con ipv4 quindi
	sockaddr_in va fatto il cast guarda il file reverse_dns_lookup_test è stato spiegato

semplicemente ntop -> network to presentation (leggibile) trasforma l'address da un ip non leggibile ad una stringa
*/
int	dns_lookup(t_icmp_packet *packet)
{
	int	client_ip = 0;

	printf("trying to retrieve ip from %s\n", packet->dns_name);
	client_ip = getaddrinfo(packet->dns_name, NULL, &packet->hints, &
		packet->result);

	if (client_ip != 0)
	{
		fprintf(stderr, "Failure from getaddrinfo in dns_lookup func \n");
		printf("%s\n", SHREK);
		return (EXIT_FAILURE);
	}

	packet->dest = (struct sockaddr_in *)packet->result->ai_addr;
	//questa conversione ziopera quanto ci ho messo a capire cosa servisse.
	inet_ntop(AF_INET, &(packet->dest->sin_addr), packet->dns_ip, INET_ADDRSTRLEN);
	printf("ip di riferimento %s\n", packet->dns_ip);
	return (0);
}

int	reverse_dns_lookup(t_icmp_packet *packet, int other_dns_status)
{
	if (other_dns_status != 0)
	{
		printf("SUPER GOOFY DETECTED\n");
		printf("%s", REVERSE_DNS_ERROR);
		return (-1);
	}
	printf("todo prossima volta");
	int status;

	(void)packet;
	(void)status;
	return 0;
}

/*
La superpotenza di getaddrinfo (la funzione che fa la risoluzione DNS) è che è abbastanza intelligente da capire da sola cosa le stai passando.
Se chiami getaddrinfo("1.1.1.1", ...) lei si accorge che è già un IP, non interroga il server DNS e ti costruisce direttamente la struttura sockaddr pronta all'uso. Se le passi [www.google.com](https://www.google.com), fa la chiamata DNS.

line 65:
l'obiettivo della variabile dns_ip era prendere effettivamente
l'ip del pacchetto dopo il dns_lookup, visto che non servirebbe in questo caso
significa che il valore passato da terminale è già un ip 1.1.1.1

ma alla fine questa funzione non mi serve getaddrinfo fa l'heavy lifting
*/
int		is_dns_needed(t_icmp_packet *packet)
{
	if (inet_pton(AF_INET, packet->dns_name, &(packet->dest)) == 1)
	{
		strcpy(packet->dns_ip, packet->dns_name);
		return 1;
	}
	else
	{
		fprintf(stderr, "Failure from inet_pton in is_dns_needed func \n");
		return 0;
	}
}


