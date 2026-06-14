#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>

// #define _DEFAULT_SOURCE

/*
Per riuscire a reperire il nome devo prima dare per scontato che sia ipv4(sockaddr_in)
preparo un apposito contenitore che possa contenere 16 bytes cioè INET_ADDRSTRLEN
va un attimo approfondire
*/
void	print_dns_beta(struct addrinfo *res)
{
	struct sockaddr_in *ipv4_val = (struct sockaddr_in *) res->ai_addr;

	char	ipv4_resolved[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &(ipv4_val->sin_addr), ipv4_resolved, INET_ADDRSTRLEN);
	printf("retrieved %s\n", ipv4_resolved);
}

int	dns_lookup(char	*dns_name)
{
	//sockaddr_in è perché lavoriamo con ipv4 è l'equivalente in codice
	struct	sockaddr_in	addr;
	struct	addrinfo	hints, *res;

	bzero(&hints, sizeof(hints));
	bzero(&res, sizeof(void *));
	//eh ok serve sempre sapere quale pacchetto dobbiamo toccare
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

	int status = getaddrinfo(dns_name, NULL, &hints, &res);
	if (status != 0)
	{
		printf("carcola che e' annata male\n");
		return (EXIT_FAILURE);
	}
	printf("daje\n");
	print_dns_beta(res);

	freeaddrinfo(res);
	return (0);
}
/*
Stabiliamo i ruoli
addr -> in_addr serve a....
sockaddr_in	addr; -> serve a.....
addrinfo	hints; -> questo è per identificare e risolvere in base a protocollo socket e tipo di hupacchetto
addrinfo	res -> elemento per getaddrinfo possiede il risultato della ricerca del dns FONDAMENTALE

come lo stampo?
*/
int main(int argc, char *argv[])
{

	struct in_addr addr;

	if (argc != 2)
	{
		fprintf(stderr, "%s <dotted-address>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (inet_aton(argv[1], &addr) == 0)
	{
		printf("trying with dns lookup then\n");
		dns_lookup(argv[1]);
	}

	printf("%s\n", inet_ntoa(addr));
	exit(EXIT_SUCCESS);
}
