#include "define.h"

static volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig)
{
    (void)sig;
    keep_running = 0;
}

/**
 * @brief Print the source and destination ports of TCP and UDP packets.
 *
 * @param buffer Pointer to the packet buffer.
 */
void print_ports_info(const unsigned char *buffer) {
    struct ethhdr *eth = (struct ethhdr *)buffer;
    struct iphdr *ip;

    if (ntohs(eth->h_proto) != ETH_P_IP)
        return;

    ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));

    if (ip->protocol == IPPROTO_TCP) {

        struct tcphdr *tcp =
            (struct tcphdr *)(buffer +
                sizeof(struct ethhdr) +
                ip->ihl * 4);

        printf("TCP %u -> %u\n",
            ntohs(tcp->source),
            ntohs(tcp->dest));
    } else if (ip->protocol == IPPROTO_UDP) {
        struct udphdr *udp =
            (struct udphdr *)(buffer +
                sizeof(struct ethhdr) +
                ip->ihl * 4);

        printf("UDP %u -> %u\n",
            ntohs(udp->source),
            ntohs(udp->dest));
    }
}

/**
 * @brief Print the source and destination IP addresses of IPv4 packets.
 *
 * @param packet Pointer to the packet buffer.
 */
void print_ip_info(const unsigned char *packet)
{
    struct ethhdr *eth = (struct ethhdr *)packet;

    if (ntohs(eth->h_proto) != ETH_P_IP)
        return;

    struct iphdr *ip =
        (struct iphdr *)(packet + sizeof(struct ethhdr));

    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &ip->saddr, src_ip, sizeof(src_ip));
    inet_ntop(AF_INET, &ip->daddr, dst_ip, sizeof(dst_ip));

    printf("IPv4 %s -> %s\n", src_ip, dst_ip);
}

/**
 * @brief Print the source and destination MAC addresses of Ethernet frames.
 *
 * @param mac Pointer to the Ethernet frame buffer.
 * @param len Length of the Ethernet frame.
 */
void print_mac_address(const unsigned char *mac, ssize_t len) {
    struct ethhdr *eth = (struct ethhdr *)mac;

    printf("\nPacket %zd bytes\n", len);

    printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
        eth->h_source[0], eth->h_source[1], eth->h_source[2],
        eth->h_source[3], eth->h_source[4], eth->h_source[5]);

    printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
        eth->h_dest[0], eth->h_dest[1], eth->h_dest[2],
        eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);

    printf("EtherType: 0x%04x\n", ntohs(eth->h_proto));
}


int main(void)
{
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        perror("signal");
        return 1;
    }

    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    unsigned char buffer[65536];

    printf("Capturing packets... Press Ctrl+C to stop.\n");
    while (keep_running) {
        ssize_t len = recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL);
        if (len < 0) {
            if (errno == EINTR)
                continue;
            perror("recvfrom");
            close(sock);
            return 1;
        }

        printf("Received packet: %zd bytes\n", len);
        print_mac_address(buffer, len);
        print_ip_info(buffer);
        print_ports_info(buffer);
    }

    printf("Stopping capture...\n");
    close(sock);
    return 0;
}