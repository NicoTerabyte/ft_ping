#ifndef UTILS_H
#define UTILS_H
// -------- standard --------
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
// ------- socket programming --------
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
// FONDAMENTALE CAVACCA
// ------- icmp package -------
#include <netinet/ip_icmp.h>



// ----------------------------

// -------- mandatory? --------
#include <linux/if_ether.h>
// ----------------------------

// -------- signals --------
#include <signal.h>
// ----------------------------

// defines for type of reply for ICMP protocol
#define ECHO_REPLY        0
#define ECHO_REQUEST      8
#define REDIRECT_MESSAGE  5
#define TIME_EXCEEDED     11
#define PARAMETER_PROBLEM 12

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


#define PACKET_ERROR "⡐⢂⠒⢤⡲⣴⣀⠀⠘⢬⢻⡥⠐⢠⠒⠐⠂⢂⣒⣠⣒⡄⣃⠂⠆⡐⡀⢂⠰⠐⢢⠘⢤⢃⠆⠐⡄⢂⡐⢀⢂⡐⠠⢂⠐⡀⢂⠐⡀⢂⠐⡀⢂⠐⡀⠆⡐⢂⠒⡐⢢⢁⠒⡄⢂\n\
⢌⠢⡉⢖⡹⢺⣽⣂⠀⠨⠓⠀⣈⣤⣴⣶⣿⣿⠿⣭⠣⡍⡐⢈⠐⠀⠄⠁⢂⠉⠤⢉⠂⢎⠜⠀⠠⣁⠐⠂⠄⠠⡁⢂⠐⠠⢁⠂⡐⠄⢂⠐⠀⢂⠐⠰⠀⠅⠊⢀⠣⢌⠢⢘⠠\n\
⠢⡑⢌⠢⣍⢱⡻⡜⢀⣠⣶⣿⣯⣿⣿⣿⠿⣭⠻⡔⢣⠐⡁⠄⢈⠀⠂⢈⠀⠠⠀⠀⠈⠈⠘⠀⠃⠀⠊⠌⡐⠡⠐⠠⢈⠐⡀⢂⠐⡈⠄⠠⢁⠠⠈⠄⢁⡠⠎⢆⠱⢊⡔⢡⠂\n\
⠡⡍⢆⠣⢌⠠⢉⣴⢿⣯⣿⣿⣿⣿⣟⢾⡹⢆⡛⠬⣁⠂⠄⠐⠀⠠⠈⠀⡀⠂⢀⠁⠠⠀⠀⣠⢀⣀⣀⠤⡀⠁⠈⠡⠆⠠⠰⠄⠒⠀⡜⠀⢢⠀⠡⠂⡘⠤⡙⢄⠣⢌⠰⢃⡌\n\
⡑⠸⢌⠣⠌⢠⡾⣫⣿⣿⣿⣿⣿⣻⠽⣎⢳⠩⡌⠱⢀⠀⠀⠀⠀⠀⠀⠄⠀⠐⠀⡀⠂⢀⠀⡇⢸⣿⢣⠘⡱⢆⠄⠀⠀⡀⠀⠈⠀⢀⠀⠀⠀⠀⠀⠀⠅⠀⢐⠈⠒⡈⢆⠣⣒\n\
⣈⠱⡈⠅⣰⡿⣰⣿⣿⣿⣿⣿⡗⣯⢻⣌⢇⠳⡈⠅⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠂⠀⢀⠀⠀⢃⠸⢯⠁⠀⠀⠊⠆⠀⠀⠄⡁⠂⢀⠂⢀⠠⠀⠠⠀⠀⠀⠀⠀⠀⠔⠰⠆⠄⠀\n\
⠠⢁⠐⢰⡿⣰⣿⣿⣿⣿⣿⣿⣏⣟⢧⡚⣌⢣⠘⠠⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣧⣈⠀⠀⠐⠈⠀⢀⠈⠄⠠⢁⠠⠀⠂⠀⠄⠁⡀⠁⢈⠀⠄⠀⠀⠀⡀⠐⠂\n\
⠐⠂⢀⣿⢳⣿⣿⣿⣿⣿⣿⣿⡷⣺⢧⡛⣤⠃⢎⠁⠄⠀⠂⠀⠄⠂⠀⡀⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠠⠐⢈⠠⠈⠄⢁⠠⠀⠂⢁⠀⠂⠠⠀⢈⠀⠠⠀⠌⠀⠂⠀⠄⠀\n\
⠘⡀⢸⠇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⡟⣧⣘⠛⡄⠘⠀⠀⢀⠀⠀⠀⠄⠀⠀⠠⠀⠀⠀⠀⠀⠀⠀⠀⡘⠀⡀⠄⡀⠄⠃⡘⠀⡀⢃⠘⠀⡀⠘⠀⠠⠀⠀⠄⠠⢀⠘⠀⠃⡘⠀\n\
⠂⠀⣼⡄⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡽⣲⢅⡋⠄⠡⠀⣀⠀⠀⠀⠈⠀⠠⠈⢀⠠⠀⠐⠀⠀⠠⠀⢁⠠⠐⠠⠐⠀⠄⠡⠐⠠⠐⡀⠌⠐⡀⠌⠠⠁⠄⠁⠂⡐⠀⠂⢁⠠⢀⠀\n\
⢆⠁⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢵⡓⢎⠘⣠⡴⠛⡀⠀⠀⠀⠀⠁⠠⠐⠀⠀⡐⠀⠈⠀⡁⢀⠂⠐⡈⠐⢈⠐⡈⠄⡁⢂⠡⠐⡈⠐⠠⢀⠁⢂⠈⠄⡁⠠⢈⠐⠀⡀⠄⠀\n\
⢏⡆⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠏⢉⣤⠾⣫⠴⢋⣤⡶⠆⠀⠀⠀⠄⠀⠂⠁⠀⠠⠁⠠⢀⠀⠄⢁⠠⢈⠠⠐⢀⠂⡐⢀⠂⡁⠄⡁⠂⠄⡈⠄⢂⠐⡀⠡⠀⠂⠐⠀⡀⠀\n\
⠺⡄⢸⣿⣿⣿⣿⣿⣿⣿⣿⠟⣋⣴⡾⢛⣡⡾⢋⣴⣿⠟⠁⠀⠀⠀⠀⠀⠂⠀⠂⠁⠠⠀⢁⠠⠀⠌⢀⠐⠠⠐⡈⠄⢂⠐⡀⢂⠐⠠⢀⠁⢂⠐⡈⠄⡐⠠⢀⠡⠀⠁⡀⠀⠀\n\
⠑⠄⠈⣿⣿⣿⣿⣿⣿⣯⣴⣾⣿⣯⣶⡿⢋⣴⡿⡿⠃⠀⠀⠀⠀⠀⠀⠀⠂⠁⠀⠌⢀⠈⠠⠀⠌⠀⢂⠈⡐⠠⠐⡈⠄⢂⠐⠠⢈⠐⠠⠈⠄⢂⠐⠠⢀⠡⠀⠠⠈⠀⠀⠀⠀\n\
⡑⣚⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢛⣡⣾⣿⣿⠟⠀⠀⠀⠀⡀⠐⡈⠄⠡⠐⠈⠀⠄⠂⠠⠁⡈⠄⡈⠄⡐⠀⠡⠐⡀⠌⠠⢈⠐⠠⠈⠄⠡⢈⠀⢂⠁⠄⡐⠀⠁⠠⠀⠀⠀⠀\n\
⡘⠔⠂⣿⣿⣿⣿⣿⣿⣿⣭⣡⣶⣿⣿⣿⡿⠋⠀⠀⠀⣠⠂⡅⢃⡐⡈⠐⠠⠁⡈⢀⠂⢁⠐⡀⠄⠐⠠⢀⠁⢂⠡⠀⠌⡐⠠⠈⠄⠡⢈⠐⠠⢈⠀⠌⠀⠄⢈⠠⠀⠀⠀⠀⠀\n\
⠠⠈⠀⣿⣿⣿⣿⣿⣿⣿⣿⡘⣿⣿⣿⠏⠀⠀⠀⢠⠜⡤⠓⠌⠂⠐⠀⠀⠀⠀⠀⠂⡈⠄⠂⠄⢂⠁⠂⠄⡈⠄⠂⡁⠂⠄⡁⠂⠌⡐⠠⠈⠄⠂⡈⠄⠡⢈⠀⠀⠀⠀⠀⠀⠀\n\
⢂⠡⠀⣿⣿⣿⣿⣿⣿⣿⣿⡇⠻⠿⠏⠀⠀⡠⢘⠢⣉⠂⠁⢀⠒⡄⠀⠀⠀⠀⢀⢠⡑⣊⠱⡈⠤⢈⠐⠠⠐⡈⠐⠠⢁⠂⠄⡁⠂⠄⡁⠂⠌⠐⠠⢈⠐⠠⠈⠀⠀⠀⠀⠀⠀\n\
⢢⡑⠠⠉⠛⠿⣿⣿⣿⣿⡿⠟⡀⠠⡔⣎⠧⡑⡈⠐⠀⢀⠀⠀⠃⠐⠊⢁⠠⡰⢌⡒⡜⠤⠓⡌⢒⠠⠌⡠⠁⠄⡁⠂⠄⡈⠐⠠⢁⠂⠄⡁⠂⡁⠂⠄⡈⠄⠐⠀⠀⠀⠀⠀⠀\n\
⠣⠌⢁⣤⣄⡀⠀⠀⠁⠀⠀⠀⠀⠀⠙⣬⠳⡱⡀⠀⠐⠀⠄⠀⠐⠠⠘⣀⠣⠔⢣⠜⣌⢣⡙⡔⢣⠘⠤⢡⠘⠠⢀⠅⠂⢄⡁⠂⠄⡈⠐⡀⠡⢀⠁⠂⠄⡐⠀⠀⠀⠀⠀⠀⠀\n\
⠀⣠⣾⠇⣻⡁⢀⠐⠀⣤⣤⣀⠀⠀⠀⠀⡳⢡⠅⣂⠀⢈⠠⠀⢈⠀⡁⠄⠂⠍⢰⢊⠴⢣⠜⡰⢃⡍⢎⠤⢃⠅⢢⢈⠂⢄⠠⢁⠂⠄⠡⢀⠁⠂⠌⠐⡀⠠⠀⠀⠀⠀⠀⠀⠀\n\
⣠⣿⣷⣿⣿⣰⠸⣿⣅⡀⠋⠁⠄⣀⠐⢀⣴⣶⣦⣤⣤⣤⣄⠀⠀⠀⢀⠀⠀⠀⠀⠉⠚⠵⣊⠵⣃⠞⣌⢎⡱⢊⡔⢢⠉⡄⠂⠄⠌⠠⢁⠂⠌⡐⠈⡀⠀⠄⠀⠀⠀⠀⠀⠀⠀\n\
⣿⣿⣿⣿⣿⣿⣄⢻⣿⣿⣿⣶⣶⣦⣶⠟⣿⣿⣿⣿⣿⣿⠃⠀⠀⠈⠀⠄⠡⠀⠀⠀⡐⠤⡀⠤⢈⠌⡠⠌⠠⠃⠌⡄⢣⠐⡡⠈⠄⠃⠄⡈⠐⠠⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⣿⣿⣿⣿⣿⣿⣿⣆⢻⣿⣿⣿⣿⣿⡗⣾⣿⣿⣿⣿⠿⠃⠀⢀⠀⠠⠁⠂⠀⠀⡄⣒⠡⢂⠱⡈⠆⠘⠀⠡⠃⡜⠐⡨⠐⡂⠅⡘⠠⢁⠂⠄⡁⠂⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⣿⣿⣿⣿⣿⣿⣿⢠⣧⠙⣯⢻⣿⣿⠀⢻⣿⣿⣯⣵⠂⢠⡍⣦⠑⠀⠀⠀⣦⢱⢰⡌⢢⡅⢢⠁⡄⠀⠀⠀⠂⡌⠂⢡⠑⠈⡆⢡⡌⠐⠈⠐⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⣿⣿⣿⣿⣿⣿⣿⣼⣞⡠⠙⢿⣿⡿⠀⡈⠿⣿⠻⣥⡘⢦⡱⢂⢇⢡⡰⣍⢦⠓⡦⡑⠦⡘⠄⠂⠀⠀⠠⢁⠂⡐⢈⠰⢀⢃⠐⡂⠄⠃⠌⠀⠐⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⣿⣿⣿⣿⣿⣿⣿⣿⣳⠆⢀⠀⠈⠑⠲⢐⠲⠀⣤⣤⣉⣄⣉⠁⡎⢧⠳⣜⢮⡙⡖⣩⠂⠀⠀⠀⠀⠐⡀⢂⠐⡠⢁⠢⠈⡄⢊⠐⡈⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⡄⠀⠀⠀⠀⠀⠀⠺⡛⢿⣿⣿⣿⣆⠐⣶⣶⣤⣅⡙⢈⠑⠈⠀⠀⢀⠠⢁⠰⠠⣁⠒⡨⢄⠃⡔⠨⢀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⣿⣿⣿⣿⣿⣿⣿⡻⣞⢠⡅⠀⠀⠀⠀⠀⠀⠀⠈⠢⢍⠻⢿⣿⣦⣬⣿⠿⠛⠁⣀⣤⡴⠀⠀⢆⡘⢄⠣⡑⢄⠣⠔⡌⠒⡌⢂⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⣿⣿⣿⣿⣿⡟⣿⣝⡿⢸⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣷⣶⣂⠤⠤⣤⣴⣶⣿⡿⠋⠄⢠⡉⢖⠨⢆⡱⡘⡌⢆⢣⡘⢡⠘⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⣿⣿⣿⣿⣿⣽⣿⣮⣝⡣⢎⡐⠈⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⡿⣿⣿⠿⠛⠁⠀⡠⢎⡲⣩⢎⡕⢮⡔⢳⠸⡘⢦⠑⠢⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⣿⣿⣿⣿⣿⣿⣿⣷⢎⠷⡨⢐⠀⠐⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠁⠀⠤⣐⡪⢟⣵⣫⣗⣳⠾⣜⡳⢎⣧⠫⣕⠊⠑⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⢹⣿⣿⣿⣿⣿⣿⣻⣎⡳⢄⡁⢂⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠢⠤⠽⢯⡟⠿⠹⠙⣩⡴⠦⣄⠀⠀⠀⠀⢀⠠⠀⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"


#define REVERSE_DNS_ERROR "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠛⠛⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n\
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⣿⣿⣿⡿⠋⣠⣶⣿⣿⣿⣷⣄⠙⢿⣿⣿⣿⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n\
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠈⠿⡿⠁⣀⣄⡉⠻⣿⣿⠟⠉⢀⣉⠻⠟⠁⠀⣨⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n\
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣶⡄⠀⠀⢀⠀⣀⠀⢀⡃⠀⠀⢀⠀⠱⢠⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n\
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⢠⣶⣤⡤⠤⢶⣿⣿⣦⣤⣄⣤⣮⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n\
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠘⠛⠋⣀⣀⠈⠛⢛⣋⣩⣌⠻⠿⠆⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n\
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠿⠠⣄⡙⠿⢿⣿⣿⣿⣿⠿⢷⣄⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n\
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⢀⣶⢸⣿⣿⣤⣤⣤⣴⣶⣶⣿⣿⣿⣆⡘⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n\
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⢠⣿⡙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣼⣿⣷⣮⣕⣘⣛⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n\
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠁⠀⠀⠀⠙⠃⣤⡙⠛⠻⢿⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣮⣝⢿⣿⣿⣿⣿⣿\n\
⣿⣿⣿⣿⣿⣿⣿⡿⠟⣋⣍⣩⣤⣄⠀⢀⣤⣶⣶⣄⣈⠙⠂⣀⣀⣭⣥⣴⣶⣾⢡⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⡙⣿⣿⣿⣿\n\
⣿⣿⣿⣿⣿⡿⠁⣤⣾⣿⣿⣿⣿⣿⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣛⣭⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡜⣿⣿⣿\n\
⣿⣿⣿⣿⠏⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠮⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⢸⣿⣿\n\
⣿⣿⣿⠏⠀⠾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠁⠘⠻⣿⣿⣿⣿⣿⣿⡌⢿⣿\n\
⣿⣿⣿⠀⢁⡀⠿⡿⠿⠙⠁⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠁⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢏⠔⠀⢀⣠⣅⣉⣿⣿⣿⣿⣿⡌⣿\n\
⣿⣿⡿⠀⠘⢁⠀⠀⠀⠀⠀⢈⠉⠛⢿⣿⣿⣿⣿⣿⣿⣿⡿⢋⡀⠀⠘⣿⢿⣿⡿⠿⠿⡭⠛⠉⠀⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⢸\n\
⣿⣿⢁⣾⣦⠀⢷⣶⣦⠀⠀⠈⠓⢤⡀⠉⠉⠋⠉⠉⠉⠛⠛⣛⣁⠀⠀⠀⢀⣀⠀⣀⡀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢸\n\
⣿⠇⣼⣿⣿⡇⣼⣿⣿⡇⠀⠀⠀⠀⠀⠀⠈⠁⠀⠀⣠⢶⣿⣿⣿⣷⣦⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢸\n\
⣿⠀⣿⣿⡿⢰⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⣀⣀⣤⣤⣶⣾⣿⣿⣿⣿⣿⣶⣟⣀⣠⣄⣠⣴⣶⡀⠀⠙⢿⡏⢿⣿⣿⣿⣿⣿⣿⣾\n\
⡿⠀⢹⡿⢁⣿⣿⣿⣿⠁⣰⡀⠀⠀⠀⠐⠦⡈⢻⣿⣿⣿⣿⣿⡇⢻⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⠀⠄⡀⠈⠿⠈⠻⣿⣿⣿⣿⡇⣿\n\
⡇⣼⣟⠃⣼⣿⣿⠟⠁⢠⣿⣷⡀⠀⢀⣀⠀⠀⢀⣀⣀⣀⣈⠻⣷⣀⣤⣄⣉⡉⠻⠿⠿⢟⣷⡤⢀⣼⣷⡀⠀⠀⢀⣿⣿⣿⣿⣿⢿\n\
⣠⣿⣿⡄⠉⠉⠁⠀⢠⣾⣿⣿⡇⠀⡺⣿⣷⠀⢸⣿⣿⣿⣿⣿⠈⣽⣿⣿⣿⣿⣯⣴⣶⣶⣿⡇⣾⣿⣿⣿⡄⠀⣤⣿⣿⣿⣿⡿⣸\n\
⣿⣿⣿⣿⣆⠀⠀⣴⣿⣿⣿⣿⡇⠘⣿⣏⢿⡇⠈⠛⠛⢻⣿⣿⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢹⣿⣿⣿⣿⢁⣿⣿⣿⣿⣿⣿⡃⢸\n\
⣿⣿⣿⣿⣿⣦⡄⣻⣿⣿⣿⣿⠀⣶⣿⣿⣷⣬⡐⢶⣶⣶⡎⠉⣰⣦⣤⣥⣾⣿⣿⣿⣿⣿⠃⣸⣿⣿⣿⡿⢸⣿⣿⣿⣿⣿⣿⣿⢸\n\
⣿⣿⣿⣿⣿⣿⣄⣻⣿⣿⣿⡿⠀⣿⡟⢻⣿⣿⣿⠾⣾⢿⣿⡟⣿⣿⣿⣿⣿⣿⣿⣿⡿⠯⢠⣿⣿⣿⣿⡇⢸⣿⣿⣿⣿⣿⣿⡟⢸\n"



#define OK_CHECKOUT "            88\n\
            88\n\
            88\n\
 ,adPPYba,  88   ,d8\n\
a8\"     \"8a 88 ,a8\"\n\
8b       d8 8888[\n\
\"8a,   ,a8\" 88`\"Yba,\n\
 `\"YbbdP\"'  88   `Y8a\n"




#define ending "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMohkhhaoMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMowUvvuvczzvvvCdMMMMMMMMMMMMhdqqwqdbdqqpqwwqpqwqdhoMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMoLruFtllltxxncuxxvCMMMMMoadmYvx!IiI;;;iiiIIljxTtTTfjncCUCLoMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMhrti:,,:tvT::inrnxjjnaMMMbnt!!TFjruzUCzUCJmLJQCQCUvjjfTttruhMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMhf!;,:ImMMMMMML!iFFFFFjLMMhu!IiIlIIIIlI!lllll!llIIIiiIT!ilTjhMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMnT!::iUMMMMMMMMMbuIjjffFcaMJti;;;iIIII!uXTT!l!tTttlIllIi;ilUoMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMjll;ifdMMMMMMMMMMMpTljTtTFLwTi;::::;;i;ii;;;::;:,,,,,,,::iuhMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMxIi:ITCMMMMMMMMMMMMWf;fjfFFtiIiiiiiiiIlIfxiiIlllli;;;!l;iTubMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMIIIl!caMMMMMMMMMMMMMY;ixft!llIIllIIlTfTt!!!l!!lllTIiIIllvuJoMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMplt!TtUoMMMMMMMMMMMMMCi!TTTtTTTT!tffvzXvFttTT!!llllll!!tfxxxXqMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMJlF!!tnoMMMMMMMMMMMMdxjxvJzxjFTTfFFFjjjjjjFjjjjrnrxurfFjrxvvrxuMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMXtjtt!rqMMMMMMMMMaUqoadYvxjjrxnxrrrrrrxxxnxnnnxnuvnnxxnrnvzzzuuzbMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMJlxF!TfuQoMMMMbLCdmXxjrjrxxrrxxxxxrxnnvuuuuxnxxxuunxxxnvuvvccvvcUaMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMd!rrttTfFumCLUzvrxjjFjrrunrrnuzXzcXXXcXcuxrxruvxnXqLvuvcuvnnuunuzwMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMbFtrffjTvLXYxuxfTtIltTfrnnczzXzzzXzXcvvcvzzXczzunrrjjffjxnxxxnnuvpMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMk!lfffrYXXrjtI;;:::::;;IjYCzunuvzzzvzYXzzvvjfFtIi;;;:::iIlItTvuuuQMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMJiIITzcrTI;;l!!!t!l!i::::lvJcvnxxxnuuucvjti::,,,,;;;:;::,,,;IFvuuQMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMlIIncj!iltFnnvnrrrrFFfT;::;!fFFjjxrrrtI:,,,:ltfFrnYuxrFtIi,,;txuuQMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMmtjnTITffFxcccuvxxxxrrjFjti:;frFFjti,,,::ITfFxnxuvccXcvnxrti::IUuzkMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMhTFT!!tffjl;;:::iFjTnxxxjFTlI!vxT!ltill!TFjrvnrxrl;;;l!tfjvxtllTwnmMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMCf!!l!iitxvxfTjccxf!lfnrxF!ltFrFtlIl!tjjrTli!TrIiIltfi:,:IfjfTTFvnaMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMozt!il!i;::i!tTfttlIIIT!IiIlltTjrT!lltfT::iTtfTttFunFlIi::,,:lllTfrwMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMarfltFrrjFTlii;;;;;iTjjfftIIlTfrxftlI;:;tfjfi::,:;i;:::;lfrFtllIl!frMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMhFftfjrxrjrrrfTttTFnuxffrfl!fFFxxFTtI;ITrrxuJnfTtIIltjzvvvuxFfTtttTjMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMhFfFFjjjruuunxjjxnxrFjFFjT!FFfjrxjfxtITrrrxxvuuxvxnuvzzzvxrFfTTfFTTFaMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMhjfTFFFFrxxxjrxrnjrxrrjFttFFrrrrxjTtujTrrrxxnuvcnvzzczunjffTT!!tTTTfhMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMhfTtttfTTFjvvzcuxrjnnxTTTFFFFjrjxrfTxjTfjrjFjrrrjjjrjxjFjTTtt!!lttTFoMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMavTtttTTlTrYXxFFfffuFTfjFtfrjrrrnFttfrfffFTTfFFFjfTt!ttTttTtt!!l!tTFMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMQft!tttl!TTuF!Ii;inuuxFt!frrrxrrft!tttfFFFIi!tTTT!!tttlll!ttllll!TTMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMmTt!!!llIIIIIi;:;txxrxFtTfrnxFrjjT!!!!TTfFti:;;IIIllIIIIll!!llll!TQMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMav!lIIIIIIi;;:::iltFFFTt!TjncxjjTt!l!!!t!!tI,::::;;:;iiiIIillIII!fhMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMY!i;;;;;;:;;::::;iI!tt!llTuYnxfT!!l!lIIi;:;:::;;;i;:::;;;;iiiiItJoMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMf;;:::;;i;:;;;llI;il!!!!!jzjjftTlIIi:::,i:;;;;;;;;;::ii;;i;;:!FwMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMoIi;;;::;llIlIl!l;i;iIIl!fxjf!lIIi;:,;::;I!!lIllll!!Ii,:::::;TYMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMLIi;:::::;Il!!t!!!T!;iIIllIIIi;:::iIllllllllllll!lIiI::::;:inoMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMoYi;::;:,:ltT!Tt!!TXfI;iiii;;:::IlllllltlIll!Ttl;::::::::::tbMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMvI;;;::;Iittt!!!!!llliIIIIll!l!lllllt!lIiiI;;Ii:::::::::!bMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMnii;:::;IIIlIlllIl!!i;Illtl!!l!lll!ttllI;;:;;::::;::::iCMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMzIi;:;;;I;iIIl!t!!!!l!!TzFfTffTTt!!!lI;;::::,:::::::iuMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMoYIi;iii;i;;itT!lii;iIlTuli;;;;;iiilI;:::,,,:;;::::ljMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMXIiiIliii;;IlfqftlliiiiiiiIitftli;;:,::::::;;:,:IUMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMMvlIIIiiiiiiiIl!tttTtTTTTTTtlIi;;;:::::::;;;:::lLMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMMM!lIiiiiiii;iiIl!!l!!l!lliiiii;::::::::;;;::i!cMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMMMll!IiiiIIIii;iiiiiIii;;;::::::::;;;;:;;:::ll!cMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMMMdI!tlIiiIIIIIi;:::;;;:::::;;;;;;ii;;;:::i!!I!XMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMMMaxII!!liIIiIiIIIiIIii;;;;iii;;;;;;::::;IliilfMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMkr!II!t!ii;iIIIlIIiii;iI;;i;;;:;::;;ii;iiiFaMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMFIliii!tI;;;;;;;;;;;::::::::;ii;::::i;;LMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMac!iiiiiI!!lIii;;;;;;iiiii;;;;:;;::tvMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMwXliiii;;;;;;;;i;;::::::;;;;;nQaMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMhdXlii;;;:::::::::::;ixqkMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMohkkkhMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\
"

//global variable admited to handle signals
extern int loop_var;





/*

me lo segno per spiegarmi meglio diciamo,
il reverse dns lookup pare un casino ma in realtà è un inganno velato
da funzione che prende buona parte delle sue variabili a NULL (vaffanculo creatore della funzione)
essenzialmente lavora con il destinatario, che abbiamo reperito con il dns lookup, e l'host, questo viene riempito perché... viene riempito

in realtà fino ad ora la struct è stata utilizzata con lo scopo di fare da definitore per il destinatario, tutti gli utilizzi di questa struttura
non vengono mai usati per creare il pacchetto che verrà poi inviato al destinatario stesso
*/

typedef struct s_dest_dest_packet
{
//--------- Special types ---------
	struct sockaddr			saddr;
	struct sockaddr_in		*dest; // dest sarebbe la conversione del argv all'effettivo come destinatario del pacchetto, questo grazie a dns lookup
	struct iphdr			*ip; //this is used to get the ip header
	struct addrinfo			hints, *result;
	//hints fa da filtro. result penso sia autoesplicativo,
//---------------------------------

	int						sock_r; // praticamente il fd del socket
	unsigned char			buffer_packet[64]; // literally the packet content
	ssize_t					buflen; //ancora un mistero aahahah

	//--------- for reverse dns lookup ---------
	char					fqdn[NI_MAXHOST]; // reverse dns lookup
	//--------- for dns lookup ---------
	char					*dns_name; //argv[1]
	char					dns_ip[INET_ADDRSTRLEN]; //reperito in caso debba fare dns_lookup normalizzato a stringa per comodità
} t_dest_packet;


/*
i have to better understand how the ping setup is made
*/
typedef struct s_icmp_packet_to_send
{
	struct	icmphdr icmp_packet;
	char	*payload;
}t_icmp_packet_to_send;

//! do not use for project, it is just for testing
typedef struct s_raw_socket_sniffer_packet
{
	//--------- Special types ---------
	struct sockaddr			saddr;

	//are they actually usdeful?
	struct sockaddr_in		source, dest; // they are meant to be used to get the ip
	struct ethhdr			*eth; //this IS the ethernet header NOT USEFUL FOR THIS PROJECT
	struct iphdr			*ip; //this is used to get the ip header
	//---------------------------------

	int						sock_r; // sto maledetto è il l'fd del pacchetto socket, senza di esso non apriamo niente
	unsigned char			*buffer; // for ethernet header
	ssize_t					buflen;
	char					*dns;
} t_raw_socket_sniffer_packet;

//--------- ICMP RELATED ---------
int		icmp_dest_socket_setup(t_dest_packet *packet, char* dns_name); //!add dns attr
void	print_eth(t_raw_socket_sniffer_packet *packet);
int		dns_lookup(t_dest_packet *packet);
int		reverse_dns_lookup(t_dest_packet *packet, int other_dns_status);
void	icmp_packet_to_send_setup(t_icmp_packet_to_send *packet_to_send);

//------------- UTILS -------------------
void	sighandler(int signum);
void	free_anything(t_dest_packet *packet, int dns_status);
void	setup_packet_to_zero(t_dest_packet *packet);

//------------- raw socket related --------------
int		raw_socket_setup(t_raw_socket_sniffer_packet *packet);
int		setup_raw_socket(t_raw_socket_sniffer_packet *packet);

int		receive_raw_data(t_raw_socket_sniffer_packet *packet);
void	free_anything_raw_socket(t_raw_socket_sniffer_packet *packet);
void	extract_ip_header_from_raw_packet(t_raw_socket_sniffer_packet *packet);

//------------- PACKAGE RELATED --------------
unsigned short	checksum_interpretation_creation(void *package, int pckg_len, int mode, unsigned int s_checksum);


#endif
