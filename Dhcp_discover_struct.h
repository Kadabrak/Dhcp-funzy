#include <iostream>
#include <cstring>
#include <ctime>
#ifndef DHCP_DISCOVER_STRUCT_H
#define DHCP_DISCOVER_STRUCT_H

using namespace std;

struct dhcp_discover_content {
	uint8_t message_type;
	uint8_t hardware_type;
	uint8_t hardware_length;
	uint8_t hops;
	uint8_t transaction_id[4];
	uint8_t seconds_elpased[2];
	uint8_t flags[2];
	uint8_t client_ip[4];
	uint8_t your_client_ip[4];
	uint8_t next_server_ip[4];
	uint8_t relay_agent_ip[4];
	uint8_t client_mac_address[6];
	uint8_t client_address_padding[10];
	uint8_t server_host_name[64];
	uint8_t boot_file[128];
	uint8_t magic_cookie[4];
	uint8_t option_53[3];
	uint8_t option_55[16];
	uint8_t option_57[4];
	uint8_t option_61[21];
	uint8_t option_12[9];
	uint8_t option_116[3];
	uint8_t option_145[3];
	uint8_t end;	
};

#endif 
