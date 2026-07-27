#include "includes/utils.h"


/*
è interessante che il getaddrinfo sa se fare il dns_lookup in base a cosa gli passi
se gli passi un dominio valido "www.google.com" lo fa, se gli passi un ip valido "1.1.1.1" sa
di NON doverlo fare ma comunque procede con il setup corretto

line 29:
	essendo che result è un sockaddr generico, noi stiamo lavorando con ipv4 quindi
	sockaddr_in va fatto il cast guarda il file reverse_dns_lookup_test è stato spiegato

grazie a getaddrinfo siamo in grado di reperire il destinatario
semplicemente ntop -> network to presentation (leggibile) trasforma l'address da un ip non leggibile ad una stringa
IMPORTANTE
getnameinfo può fallire anche con un ip valido, questo perché non tutti i domini hanno un nome definito occhio alla casistica
*/
int	dns_lookup(t_dest_data *packet)
{
	int	client_ip = 0;

	// printf("trying to retrieve ip from %s\n", packet->dns_name);
	client_ip = getaddrinfo(packet->dns_name, NULL, &packet->hints, &
		packet->result);

	if (client_ip != 0)
	{
		fprintf(stderr, "Failure from getaddrinfo in dns_lookup func \n");

		// printf("%s\n", SHREK);
		return (EXIT_FAILURE);
	}
	packet->dest = (struct sockaddr_in *)packet->result->ai_addr;
	inet_ntop(AF_INET, &(packet->dest->sin_addr), packet->dns_ip, INET_ADDRSTRLEN);
	printf("ip di riferimento %s\n", packet->dns_ip);
	return (0);
}

int	reverse_dns_lookup(t_dest_data *packet, int other_dns_status)
{
	int status;
	if (other_dns_status != 0)
	{
		// printf("SUPER GOOFY DETECTED\n");
		// printf("%s", REVERSE_DNS_ERROR);
		return (-1);
	}

	status = getnameinfo((struct sockaddr *)packet->dest, sizeof(*packet->dest), packet->fqdn, sizeof(packet->fqdn), NULL, 0, NI_NAMEREQD);

	if (status != 0)
	{
		printf("REAL REVERSE LOOKUP FAILED or full name not available\n");
		inet_ntop(AF_INET, packet->dest, packet->fqdn, sizeof(packet->fqdn));
	}
	printf("reverse we got %s\n", packet->fqdn);
	return 0;
}

/*
for more information check the code in the "testing_stuff" dir.
the mode var it's used to check if whoever called the function is the sender (mode = 0) of the packet
so it does need to create it or if the receiver (mode = 1), so it needs to add it to it's calculated checksum
to check if the packet it's valid, before the shifting
*/
unsigned short	checksum_interpretation_creation(void *package, int pckg_len, int mode, unsigned int s_checksum)
{
	unsigned int sum=0; // 32 bit per gestire sequenze dispari
	unsigned short result; // 16 bit
	unsigned short *buf = package; // 16 bit

	for (sum = 0; pckg_len > 1; pckg_len -= 2)
		sum += *buf++;

	if (pckg_len == 1)
		sum+= *(unsigned char*)buf;

	if (mode == 1)
		sum+=s_checksum;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);

	result = ~sum;
	return result;
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
int		is_dns_needed(t_dest_data *packet)
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



