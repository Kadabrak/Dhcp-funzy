#include "Socket_sender.h"
#include "Dhcp_discover_struct.h"
#include "Dhcp_discover_content.h"
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

using namespace std;

int main(){
	srand(time(0));
	Socket_Sender dhcp_socket;
	int i = 0;
	while(1==1){
		//usleep(100);
		Dhcp_Discover_content discover_packet(dhcp_socket);
		i += 1;
	}
}
