#include <arpa/inet.h>
#include <ncurses.h>
#include <netinet/ip.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_IP_COUNT 50
#define INET_ADDRSTRLEN 16

// Structure to store IP statistics.
typedef struct {
  char ip[INET_ADDRSTRLEN];
  unsigned long packets;
  unsigned long bytes;
} IPStats;

IPStats ipStats[MAX_IP_COUNT]; // Array to store IP statistics.
int ipCount = 0; // Number of IP addresses.

// Function to add or update IP statistics.
void updateIPStats(const char *ip, unsigned int bytes) {
  int i;
  for (i = 0; i < ipCount; i++) {
    if (strcmp(ipStats[i].ip, ip) == 0) {
      ipStats[i].packets++;
      ipStats[i].bytes += bytes;
      return;
    }
  }
  if (ipCount < MAX_IP_COUNT) {
    strcpy(ipStats[ipCount].ip, ip);
    ipStats[ipCount].packets = 1;
    ipStats[ipCount].bytes = bytes;
    ipCount++;
  }
}

// Packet capture callback function.
void packetHandler(u_char *userData __attribute__((unused)),
                   const struct pcap_pkthdr *pkthdr, const u_char *packet) {
  const struct ip *ipHeader =
      (struct ip *)(packet + 14);  // Skip Ethernet header
  char srcIP[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(ipHeader->ip_src), srcIP, INET_ADDRSTRLEN);
  updateIPStats(srcIP, pkthdr->len);
}

// Function to display IP statistics using ncurses.
void displayIPStats() {
  clear();
  printw(
      "-netblame-\n\nIP Address       Packets    "
      "Bytes\n----------------------------------\n");
  for (int i = 0; i < ipCount; i++) {
    printw("%-16s %-10lu %-10lu\n", ipStats[i].ip, ipStats[i].packets,
           ipStats[i].bytes);
  }
  refresh();
}

int main() {
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_if_t *devices, *device;
  pcap_t *pcapHandle;
  char filterExp[] = "ip";

  // Initialize pcap
  if (pcap_findalldevs(&devices, errbuf) == -1) {
    fprintf(stderr, "Error finding devices: %s\n", errbuf);
    return EXIT_FAILURE;
  }
  if ((device = devices) == NULL) {
    fprintf(stderr, "No devices found.\n");
    return EXIT_FAILURE;
  }

  pcapHandle = pcap_open_live(device->name, BUFSIZ, 1, 1000, errbuf);
  if (pcapHandle == NULL) {
    fprintf(stderr, "Unable to open the device %s: %s\n", device->name, errbuf);
    return EXIT_FAILURE;
  }

  // Apply the packet filter
  struct bpf_program fp;
  if (pcap_compile(pcapHandle, &fp, filterExp, 0, PCAP_NETMASK_UNKNOWN) == -1 ||
      pcap_setfilter(pcapHandle, &fp) == -1) {
    fprintf(stderr, "Error setting filter: %s\n", pcap_geterr(pcapHandle));
    return EXIT_FAILURE;
  }

  // Initialize ncurses
  initscr();
  noecho();
  cbreak();

  // Main loop
  int ch;
  nodelay(stdscr, TRUE);
  while ((ch = getch()) != 'q') {
    pcap_dispatch(pcapHandle, 10, packetHandler, NULL);
    displayIPStats();
    napms(500);
  }

  // Cleanup
  endwin();
  pcap_freecode(&fp);
  pcap_close(pcapHandle);
  pcap_freealldevs(devices);

  return EXIT_SUCCESS;
}
