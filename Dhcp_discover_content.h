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

class Dhcp_Discover_content{
	private:
		struct dhcp_discover_content message;
		Socket_Sender& socket_sender;
    		uint8_t* generate_random_mac_address(){
    			static uint8_t mac_address[6];
    			
    			for(int i=0;i<6;i++){
    				mac_address[i] = rand()%256;
    			}

    			return mac_address;
    		};
    		
    		uint8_t* generate_random_transaction_id(){
			static uint8_t id[4];
			
    			for(int i=0;i<4;i++){
    				id[i] = rand()%256;
    			}

    			return id;
    		};
    		
	public:
		Dhcp_Discover_content(Socket_Sender& sender) : socket_sender(sender){
			uint8_t* mac_address = generate_random_mac_address();
			
    			memset(&message, 0, sizeof(struct dhcp_discover_content));
    			message.message_type = 0x01;
    			message.hardware_type = 0x01;
    			message.hardware_length = 0x06;
			message.hops = 0x00;
			
			uint8_t flags[2] = {0x80,0x00};
			memcpy(message.flags, flags, 2);
			
			uint8_t* transaction_id = generate_random_transaction_id();
			memcpy(message.transaction_id, transaction_id, 4);

			memcpy(message.client_mac_address, mac_address, 6);
			
			uint8_t magic_cookie[4] = {0x63, 0x82, 0x53, 0x63};
			memcpy(message.magic_cookie, magic_cookie, 4);
			
			uint8_t option_53[3] = {0x35, 0x1, 0x1};
    			memcpy(message.option_53, option_53, 3);
    	
    			uint8_t option_55[16] = {0x37, 0xe, 0x1, 0x79, 0x3, 0x6, 0xc, 0xf, 0x1a, 0x1c, 0x21, 0x33, 0x36, 0x3a, 0x3b, 0x77};
    			memcpy(message.option_55, option_55, 16);
    	
    			uint8_t option_57[4] = {0x39, 0x2, 0x5, 0xd0};
    			memcpy(message.option_57, option_57, 4);
    	
			uint8_t option_61[21] = {0x3d, 0x13, 0xff, 0x6d, 0x25, 0xc4, 0xb1, 0x0, 0x1, 0x0, 0x1, 0x30, 0xba, 0x1f, 0xef};
			for(int i = 0;i<6;i++){
	 			int id = 15+i;
	 			option_61[id] = mac_address[i];
	 		}
			memcpy(message.option_61, option_61, 21);
    			message.end=0xff;
    			
    			sender.eth_header_construction(mac_address);
    			sender.send_packet(message,mac_address,transaction_id);
		}

};
