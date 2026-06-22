#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

/*
funzione che dovrebbe calcolare il checksum dato uno specifico messaggio
interessante è *package che lavora come un oggetto
il pacchetto viene passato come void perché intendiamo utilizzarlo
come short quindi 2 bytes, cioè 16 bit l'esadecimale fa comodo per lo shifting

0xFFFF = 1111 1111 1111 1111 ->

*/

int	sender_checksum(void *package, int pckg_len)
{
	unsigned int sum=0; // 32 bit
	unsigned short result; // 16 bit
	unsigned short *buf = package; // 16 bit

	for (sum = 0; pckg_len > 1; pckg_len -= 2)
		sum += *buf++;

	if (pckg_len == 1)
		sum+= *(unsigned char*)buf;

	// qualsiasi valore carry viene shiftato a sinistra
	// e sommato al valore originale mascherato che toglie i vecchi carry
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);

	result = ~sum;
	return result;
}

void	checksum_receiver()
{
	return;
}

int	main(void)
{
	char	msg[16];

	memset(msg, 0, sizeof(msg));
	strcpy(msg, "ciao mondo");
	printf("%s\n", msg);


}
