/*
** EPITECH PROJECT, 2018
** MYARPSPOOF
** File description:
** print_func.c
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/ether.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include "spoof.h"

static void fill_target_dest(arp_header_t *arp
    , struct ethhdr *send_req
    , char *value)
{
    int ind = 0;
    char save[3];

    for (int i = 0; i < 6; ++i) {
        sprintf(save, "%s", strncpy(save, value + ind, 2));
        send_req->h_dest[i] = hexadecimalToDecimal(save);
        arp->target_mac[i] = hexadecimalToDecimal(save);
        ind += 3;
    }
}

static void printbroadcast(struct ifreq *if_mac, char **av, char *mac_addr)
{
    char sendbuf[BUF_SIZE];
    struct ethhdr *send_req = (struct ethhdr *)sendbuf;
    arp_header_t arp;
    struct sockaddr_ll sock_addr;

    memset(sendbuf, 0, BUF_SIZE);
    fill_arp(&arp, ARP_REQUEST, mac_addr);
    fill_arp_eth(send_req, &arp, &sock_addr, if_mac);
    fill_sock_addr(&sock_addr, 0);
    fill_arp_send_target(&arp, av[0], av[1]);
    print_arp_packet(&arp, send_req);
}

static void printspoof(struct ifreq *if_mac, char **av, char *mac_addr)
{
    char sendbuf[BUF_SIZE];
    struct ethhdr *send_req = (struct ethhdr *)sendbuf;
    arp_header_t arp;
    struct sockaddr_ll sock_addr;

    memset(sendbuf, 0, BUF_SIZE);
    fill_arp(&arp, ARP_REPLY, mac_addr);
    fill_arp_eth(send_req, &arp, &sock_addr, if_mac);
    fill_sock_addr(&sock_addr, 0);
    fill_arp_send_target(&arp, av[0], av[1]);
    fill_target_dest(&arp, send_req, av[4]);
    print_arp_packet(&arp, send_req);
}

int print_function(char **av)
{
    struct ifreq if_mac;
    char *mac_addr = calloc(18, sizeof(char));

    if (mac_addr == NULL || !get_addr(av[2], &mac_addr))
        return (FAILURE);
    printf("%s\n", mac_addr);
    if (strcmp(av[3], "--printSpoof") == 0)
        printspoof(&if_mac, av, mac_addr);
    else
        printbroadcast(&if_mac, av, mac_addr);
    free(mac_addr);
    return (SUCCESS);
}