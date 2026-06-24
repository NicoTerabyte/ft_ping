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

0xFFFF = 1111 1111 1111 1111 -> con la maschera mi tengo sempre i primi 4 bytes

nella parte finale dove ho >> 16
qualsiasi valore carry viene shiftato a destra
e sommato al valore originale mascherando vecchi carry
se avessi 0x0001ABC5 io vorrei sommare 0x0001 e 0xABC5
quindi shifto per per ottenere 0x0001 e poi
lo sommo a 0x0001ABC5 che con & 0xFFFF diventa 0xABC5

l'algoritmo prende spunto dal RFC 1071


*/

unsigned short	sender_checksum(void *package, int pckg_len)
{
	unsigned int sum=0; // 32 bit per gestire sequenze dispari
	unsigned short result; // 16 bit
	unsigned short *buf = package; // 16 bit

	for (sum = 0; pckg_len > 1; pckg_len -= 2)
		sum += *buf++;

	if (pckg_len == 1)
		sum+= *(unsigned char*)buf;


	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);

	result = ~sum;
	return result;
}

/*
il receiver per verificare il checksum deve prendere quello del mittente
e sommarlo al messaggio che si aspetta, in questo modo può verificare che il messaggio sia integro
il ragionamento per capire se il checksum è valido è abbastanza analogo a quello di calcolarlo
l'unica differenza sostanziale è che alla fine dopo aver ricalcolato il checksum lo sommo con quello
che ho trovato e se rigirando tutti i bit il risultato è 1 allora ci siamo
*/

unsigned short	checksum_receiver(void *package, int pckg_len, unsigned int s_checksum)
{
	unsigned int sum=0;
	unsigned short res;
	unsigned short *buf = package;
	unsigned int final_checksum;

	for (sum = 0; pckg_len > 1; pckg_len -= 2)
		sum += *buf++;

	if (pckg_len == 1)
		sum += *(unsigned char*)buf;

	// questa parte è l'extra che non sta nel checksum normale
	sum += s_checksum;

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);

	//fa la differenza quando usa '~'? fa un -1 extra
	//ci sono più numeri negativi che positivi
	return ~sum;
}
/*
Nota importante, alle checksum non interessa del segno e l'algoritmo
lavora con 2 bytes non 4 quindi le variabili relative al checksum devono essere degli
unsigned short. la cosa importante è il segno che se non viene rimosso rischia di rovinare il calcolo
se stai facendo l'inversione dei valori, visto che un unsigned si comporterebbe diversamente rispetto ad un valore con segno.
*/

int	main(void)
{
	char	msg[22];
	unsigned short		s_checksum;
	unsigned short		rec_checksum;
	int		test = 2;
	memset(msg, 0, sizeof(msg));
	strcpy(msg, "ciao mondo pesce cane");
	printf("%s\n", msg);
	s_checksum = sender_checksum(&msg,  sizeof(msg));

	rec_checksum = checksum_receiver(&msg, sizeof(msg), s_checksum);

	if (rec_checksum == 0)
		printf("ci siamo sono uguali\n");
	else
		printf("ho sbagliato qualcosa nella conversione: sender: %x receiver: %x\n", s_checksum, rec_checksum);

	printf("testing %d\n", ~test);
}
