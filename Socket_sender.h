#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <ctime>
#include "Dhcp_discover_struct.h"

using namespace std;

class Socket_Sender {
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    
private:
    struct ethhdr eth_header;
    struct sockaddr_ll socket_types;
    struct iphdr ip_header;
    struct udphdr udp_header;
    struct dhcp_discover_content message;
   
public:
    Socket_Sender() {
        socket_types_construction();
    	ip_header_construction();
    	udp_header_construction();
    }
    
    void eth_header_construction(unsigned char mac_address[]){
        unsigned char destination[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        unsigned char source[6] = {0xd4,0x6d,0x6d,0x25,0xc4,0xb1};
        memcpy(eth_header.h_dest,destination,6);
        memcpy(eth_header.h_source,mac_address,6);
        eth_header.h_proto = htons(ETH_P_IP);
    }
    void socket_types_construction(){
        memset(&socket_types, 0, sizeof(struct sockaddr_ll));
        socket_types.sll_family = AF_PACKET;
        socket_types.sll_protocol = htons(ETH_P_ALL);
        socket_types.sll_ifindex = 3; // à faire
        socket_types.sll_halen = ETH_ALEN;
        unsigned char broadcast_mac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	memcpy(socket_types.sll_addr, broadcast_mac, ETH_ALEN);
    
    }
    void udp_header_construction(){
    	memset(&udp_header, 0, sizeof(struct udphdr));
    	udp_header.source = htons(68);
    	udp_header.dest = htons(67);
    	udp_header.len = htons(308); // Calculer
    	udp_header.check = 0;
    }
    void ip_header_construction(){
    	memset(&ip_header, 0, sizeof(struct iphdr));
        ip_header.version = 4;
    	ip_header.ihl = 5;
    	ip_header.tos = 0;
    	ip_header.tot_len = htons(328);
    	ip_header.id = htons(54321);
    	ip_header.frag_off = 0;
    	ip_header.ttl = 64;
    	ip_header.protocol = IPPROTO_UDP;
    	ip_header.saddr = inet_addr("0.0.0.0");
    	ip_header.daddr = inet_addr("255.255.255.255");
    	ip_header.check = 0; // calculer
    }
    
    void send_packet(dhcp_discover_content message, unsigned char mac_address[], uint8_t transaction_id[]){
    	int packet_size = sizeof(eth_header)+sizeof(ip_header)+sizeof(udp_header)+sizeof(message);
    	char packet[packet_size];
    	
    	int offset = 0;
	memcpy(packet+offset,&eth_header,sizeof(eth_header));
	offset = offset+sizeof(eth_header);
	memcpy(packet+offset,&ip_header,sizeof(ip_header));
	offset = offset+sizeof(ip_header);
	memcpy(packet+offset,&udp_header,sizeof(udp_header));
	offset = offset+sizeof(udp_header);
	memcpy(packet+offset,&message,sizeof(message));
    	
    	size_t sent_bytes = sendto(sock, packet, packet_size, 0, (struct sockaddr*)&socket_types, sizeof(socket_types));
    	if (sent_bytes == -1) {
		perror("sendto");
	}
	else{
		cout << "discover send with mac address: ";
		for(int i=0;i<6;i++){
    			printf("%02X",mac_address[i]);
    			if(i<5){
    				cout << ":";
    			}
    		}
    		cout << " with the transaction id: 0x";
    		for(int i=0;i<4;i++){
    			printf("%02X",transaction_id[i]);
    		}
    		cout << "\n";
	}
    }
    void killsocket(){
    	close(sock);
    }
};
