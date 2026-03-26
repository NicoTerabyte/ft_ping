#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
// -------- mandatory? --------
#include <linux/if_ether.h>
// ----------------------------
#include <string.h>
#include <signal.h>
// defines for type of reply for ICMP protocol
#define ECHO_REPLY   0
#define ECHO_REQUEST 8


#define SHREK " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀           ⢀⣀⣠⣤⣤⣤⣤⣤⣄⣀\n\
⠀⠀⢀⣴⣶⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡴⠶⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠉⠓⠶⢄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⠀⣴⠏⠀⢸⣿⠻⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡴⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠳⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⣸⠁⠀⠀⢸⣿⠀⢱⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡾⠋⠀⠀⠀⢀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⣇⢀⠀⠀⢸⠟⠃⠀⢧⡀⠀⠀⠀⠀⠀⠀⢀⣼⠏⠀⠀⣠⣤⣶⣿⣟⢳⣶⣶⣤⣄⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⠘⠿⣶⣴⣤⣀⡀⠀⠀⠱⣄⠀⠀⠀⠀⢠⡿⢋⡇⢀⣾⡿⠿⠛⠋⠉⠉⠉⠛⠻⠿⢿⡿⠗⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠉⠙⠻⢦⣄⠀⠈⠳⢦⣀⣰⠟⢁⣨⡵⠏⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⣶⣶⠶⣦⣤⣀⣨⡷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠐⠲\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠙⢳⣄⠀⠀⢤⣥⡼⠛⠛⠇⠀⠀⠀⠀⠀⣀⣀⣉⣛⡳⢤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠛⠷⠿⢿⣇⣽⣦⠀⠀⠀⠀⠀⠀⠀⠀⣠⡞⠁⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⠹⣦⣀⢚⠁⠂⠲⢦⣤⣀⣀⣤⣶⠿⢻⣭⣭⡛⠻⣦⡽⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⠀⠀⠀⠉⠙⢿⣧⠀⠀⠀⠀⠀⣠⡼⠋⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⡏⠈⠀⠀⠀⠀⠀⠉⠛⠿⣽⣶⣿⠿⣿⣿⣀⣈⣷⡸⣆⡤⠤⠂⠀⠀⠠⠀⡀⠀⣺⣭⣥⡴⣦⣤⣅⠀⠀⠀⢸⡀⠀⣠⡴⠚⠉⠀⠀⠲⣄⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠛⠛⠋⠊⠉⠁⠈⢁⡀⠀⠀⠀⠀⠀⠀⠳⣼⣏⣿⣿⣿⣦⠈⠙⢷⡀⠀⠀⠉⠉⠀⠀⢀⣀⣤⠴⠶⠿⠿⠦\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⡿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠻⠿⢿⣛⠒⠒⢛⠃⠀⠀⠀⢀⡤⠞⠋⠀⠀⠀⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⢰⡿⢁⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⠀⠀⠀⠀⣴⣏⠀⠀⣀⡖⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠁⠀⠀⠀⢰⣏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⣾⡇⢸⠀⠀⠀⠀⠀⠀⣰⣾⡿⠿⠿⠛⠓⠀⠀⢿⣿⣿⣿⣿⣿⣶⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠋⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⢠⣿⢡⠘⠀⠀⠀⠀⠀⢚⡏⠁⠀⠀⠀⠀⠀⠀⠀⠀⠈⠁⠙⠻⢿⣿⣯⡛⠻⢶⣶⣶⣶⣶⣶⣿⣷⣶⣶⣁⠀⠀⠀⠀⠀⠀⠀⠀⠙⡆⠀⠀⠀⠀⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⢸⣿⣸⡁⠀⠀⠀⠀⠀⣾⠁⢙⢶⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠛⠓⠒⠒⠛⠉⠉⠉⠉⠉⠁⠈⠙⠶⣾⣷⣆⠀⠀⠀⠀⢹⠀⠀⠀⠀⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⣾⣧⣿⠃⠀⠀⠀⠀⢸⡇⠀⠈⠁⠈⠙⠳⢦⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣷⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⢠⣿⡿⠸⠀⠀⠀⠀⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠲⠦⢤⣤⣄⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡤⠈⢿⡇⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀\n\
⠀⠀⠀⣀⣴⡾⢻⣿⠁⠀⠀⠀⠀⠀⠀⠘⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠙⠛⠒⠒⠲⠶⠶⠶⠶⠦⢤⣤⠤⠴⠒⠋⠉⠀⠀⠈⠃⠀⠀⡞⠀⠀⠀⠀⠀⠀⠀⠀\n\
⣴⣾⠿⣋⣭⣴⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⢻⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⡇⠀⠀⠀⠀⠀⠀⠀⠀\n\
⣫⠾⣋⣽⣿⣷⣿⠀⠀⠀⠙⢧⡀⠀⠀⠀⠀⠷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣆⠀⠀⠀⠀⠀⠀⠀\n\
⠋⣼⣿⢿⣶⣿⡇⠀⠀⠀⢀⠈⢿⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡮⡛⣦⣄⠀⠀⠀⠀\n\
⣤⣿⣿⣾⣿⣿⡆⠀⠀⠀⠙⠄⠈⠻⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⢻⣷⢹⡎⣿⣷⣤⣀⣀\n\
⣿⣿⡿⣿⣿⠤⣫⡀⠀⠀⠀⠀⠀⠀⠈⢷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⢹⣬⣿⡞⢻⡿⣿⣧⡻⢿\n\
⣿⣿⣾⣿⡯⣠⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠙⣷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⠃⢸⣿⣿⠦⡄⢹⣾⣿⣷⠙\n\
⣿⣿⣿⣿⡿⠋⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡴⢀⡼⠃⠀⢸⣿⡇⢠⣄⣘⣿⣷⡿⣷\n\
⣿⣿⣿⣿⡷⠘⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡴⣋⣴⠟⠁⠀⣠⡿⣿⣇⣀⠉⣿⣿⡋⠱⣏\n\
⣿⣿⣿⣿⣧⠸⢻⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡴⣾⣾⠟⠁⠀⠀⣼⡏⣷⣿⣛⢻⣶⣿⠿⠿⣆⣟\n\
⣿⣿⣿⣿⣿⡂⠀⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣽⡾⠟⠁⠀⠀⠀⣴⡟⢷⣿⣏⠛⢿⣿⣦⣄⡀⠛⠛\n\
⣿⣿⣿⣿⣿⣧⠠⠈⢿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⠶⠶⣶⣿⡿⠛⠉⠀⠀⠀⠀⢀⣾⣿⢻⣿⠿⣿⡷⣾⣿⣍⣿⡇⢀⣴\n\
⣿⣿⣿⣿⣿⣿⡆⠀⠸⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠋⠓⠀⠠⠄⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠴⠟⠉⠀⠀⠀⠀⠀⠀⣀⣼⣿⠟⠁⣾⣿⣧⣼⣏⢿⣯⡀⣽⠃⣾⠁\n\
⣿⣿⣿⣿⣿⣿⣿⡄⠀⢹⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠈⠁⠐⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣤⣼⡿⠋⢀⣾⣇⠈⣿⣏⣻⡟⠉⣿⢋⣿⠛⣷"

typedef struct s_icmp_packet
{
//--------- Special types ---------
	struct sockaddr			saddr;
	struct sockaddr_in		source, dest; // they are meant to be used to get the ip
	struct iphdr			*ip; //this is used to get the ip header
//---------------------------------

	int						sock_r;
	unsigned char			*buffer; // for ethernet header
	ssize_t					buflen;


} t_icmp_packet;

//! do not use for project, it is just for testing
typedef struct s_raw_socket_sniffer_packet
{
	//--------- Special types ---------
	struct sockaddr			saddr;
	struct sockaddr_in		source, dest; // they are meant to be used to get the ip
	struct ethhdr			*eth; //this IS the ethernet header NOT USEFUL FOR THIS PROJECT
	struct iphdr			*ip; //this is used to get the ip header
	//---------------------------------

	int						sock_r;
	unsigned char			*buffer; // for ethernet header
	ssize_t					buflen;
} t_raw_socket_sniffer_packet;

//--------- ICMP RELATED ---------
int		icmp_socket_setup(t_icmp_packet *packet);
void	print_eth(t_icmp_packet *packet);
void	get_ip_header(t_icmp_packet *packet);

//--------------------------------
void	sighandler(int signum);
void	free_anything(t_icmp_packet *packet);

void	setup_packet_to_zero(t_icmp_packet	*packet);

//------------- raw socket related --------------
int		raw_socket_setup(t_raw_socket_sniffer_packet *packet);
int		receive_raw_data(t_raw_socket_sniffer_packet *packet);
void	free_anything_raw_socket(t_raw_socket_sniffer_packet *packet);
void	extract_ip_header_from_raw_packet(t_raw_socket_sniffer_packet *packet);



#endif
