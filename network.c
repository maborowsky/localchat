/*
 * network.c
 *
 * Handles all netowkring functions for localchat
 *
 */

#include <stdio.h>        // Needed for printf()
#include <stdlib.h>       // Needed for exit()
#include <string.h>       // Needed for memcpy() and strcpy()
#include <sys/types.h>    // Needed for sockets stuff
#include <netinet/in.h>   // Needed for sockets stuff
#include <sys/socket.h>   // Needed for sockets stuff
#include <arpa/inet.h>    // Needed for sockets stuff
#include <fcntl.h>        // Needed for sockets stuff
#include <netdb.h>        // Needed for sockets stuff

#include <sys/ioctl.h>	  // Needed for local ip
#include <unistd.h>		  // Needed for local ip
#include <net/if.h>		  // Needed for local ip

#include "localchat.h"


char* ipadd;
char* peer_ip;

void broadcast(char message[50]) {
    send_to_IP(message, BCAST_IP);
}

/*
 * Sends a message to a given IP address
 */
void send_to_IP(char message[50], char ip_addr[15]) {
    int                  client_s;        // Client socket descriptor
    struct sockaddr_in   server_addr;     // Server Internet address
    struct in_addr       server_ip_addr;  // Server IP Address
    int                  addr_len;        // Internet address length
    char                 out_buf[4096];   // Output buffer for data
    char                 in_buf[4096];    // Input buffer for data
    int                  retcode;         // Return code
    int                  iOptVal;         // Socket option value
    int                  iOptLen;         // Socket option length
    int                  i;               // Loop control variable

    // Create a socket
    // TODO remove
    client_s = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_s < 0) {
        printf("*** ERROR - socket() failed \n");
        exit(-1);
    }

    // Fill-in server socket's address information
    server_addr.sin_family = AF_INET;                  // Address family to use
    server_addr.sin_port = htons(PORT_NUM);            // Port num to use
    server_addr.sin_addr.s_addr = inet_addr(ip_addr); // Given IP

    // Allow socket to Broadcast - by default sockets disallow broadcast

    iOptVal = 1;
    iOptLen = sizeof(int);
    setsockopt(client_s, SOL_SOCKET, SO_BROADCAST, (void*)&iOptVal, iOptLen);
    // Assign a message to buffer out_buf
    strcpy(out_buf, message);

    // Now send the message
    retcode = sendto(client_s, out_buf, (strlen(out_buf) + 1), 0,
            (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (retcode < 0) {
        printf("*** ERROR - sendto() failed \n");
        exit(-1);
    }

    // Close all open sockets
    retcode = close(client_s);
    if (retcode < 0)
    {
        printf("*** ERROR - close() failed \n");
        exit(-1);
    }

} // End send_to_IP

void print_ip(int ip)
{
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;   
    printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);        
}

void *receive(void *arg) {
    int                  server_s;        // Server socket descriptor
    struct sockaddr_in   server_addr;     // Server Internet address
    struct sockaddr_in   client_addr;     // Client Internet address
    struct in_addr       client_ip_addr;  // Client IP address
    int                  addr_len;        // Internet address length
    char                 out_buf[4096];   // Output buffer for data
    char                 in_buf[4096];    // Input buffer for data
    int                  retcode;         // Return code

    // >>> Step #1 <<<
    // Create a socket
    //   - AF_INET is Address Family Internet and SOCK_DGRAM is datagram
	
	
	
    server_s = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_s < 0)
    {
        printf("*** ERROR - socket() failed \n");
        exit(-1);
    }

    // >>> Step #2 <<<
    // Fill-in my socket's address information

    server_addr.sin_family = AF_INET;                 // Address family to use
    server_addr.sin_port = htons(PORT_NUM);           // Port number to use
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Listen on any IP address
    retcode = bind(server_s, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (retcode < 0)
    {
        printf("*** ERROR - bind() failed \n");
        exit(-1);
    }

    char msg_type[12]; // The type of message that was recieved (token1)
    char token2[140]; // The token after msg_type, changed to 140 bcause of MSG:DATA
    char token3[32];	// third token in OK Y/N
    char token4[32];	// fourth token in OK if 3rd is N
    char* token; // Used to tokenize strings

	while (1) {
        // >>> Step #3 <<<
        // Wait to receive a message from peer

        addr_len = sizeof(client_addr);
        retcode = recvfrom(server_s, in_buf, sizeof(in_buf), 0,
                (struct sockaddr *)&client_addr, &addr_len);
        if (retcode < 0)
        {
            printf("*** ERROR - recvfrom() failed \n");
            exit(-1);
        }

        // Copy the four-byte client IP address into an IP address structure
        memcpy(&client_ip_addr, &client_addr.sin_addr.s_addr, 4);

        
		

		
        // Get the message type from packet
        token = strtok(in_buf, ":");
        strcpy(msg_type, token);
        
        token = strtok(NULL, ":");
        if (token != NULL) {
            strcpy(token2, token);
        } else {
            // Packets without a second token
        }

		token = strtok(NULL, ":");
        if (token != NULL) {
            strcpy(token3, token);
        } else {
            // Packets without a third token
        }
        
        token = strtok(NULL, ":");
        if (token != NULL) {
            strcpy(token4, token);
        } else {
            // Packets without a fourth token
        }
		
		//ipadd = ipAddress();	//my ip address
		//peer_ip = inet_ntoa(client_ip_addr); // The peer's ip addr
		//printf("MY IP: %s, Client IP: %s\n", ipadd, peer_ip);
		
        if ( !strcmp(msg_type, "HELLO" ) ) {
            // TODO check if user is already in table. Should implement checkUser() - implemented checkTable()
            // Do not add self to table
			char ok_msg[50] = "OK:";	
			
			int flag;
			{
				flag = checkTable(token2);
			}
			
			if (strcmp(username, token2) == 0 && flag == 0)
			{
				addPeer(token2);
				flag = -1;
			}
			// Send an ok packet back
			strcat(ok_msg, username);
			
			if (flag == 0)
			{
				strcat(ok_msg, ":Y");
				addPeer(token2); // token2 will be the username
			}
			else if (flag == -1)
				strcat(ok_msg, ":Y");
			else
			{
				strcat(ok_msg, ":N:");
				strcat(ok_msg, username);
				printf("Bad username\n");
			}
			
			//send_to_IP(client_addr.sin_addr.s_addr);
			send_to_IP( ok_msg, inet_ntoa(client_addr.sin_addr) );
            
        } else if ( (!strcmp(msg_type, "OK" ))) {
			// TODO check if user is already in table. Should implement checkUser()
			if (strcmp(token3,"Y") == 0)
			{
				int flag = 0;
				{
					flag = checkTable(token2);
				}
				
				if (flag == 0)
				{
					addPeer(token2); // token2 will be the username
				}
				else
				{
				
				//do nothing
				}
			}
			else if (strcmp(token3, "N") == 0)
			{
				removePeer(username);
				printf("Enter new username:\n");
				fgets(username, 32, stdin);
				char hello[50] = "HELLO:";
				strcat(hello, username);
				broadcast(hello);
				addPeer(username);
			}         
			 
        } else if ( !strcmp(msg_type, "BYE") ) {
            printf("removing peer %s\n", token2);
            removePeer(token2);
            fflush(stdout);
        } else if ( !strcmp(msg_type, "CHATREQ") ) {
			acceptChat();
		} else if ( !strcmp(msg_type, "CHATY") ) {
			printf("Peer accepted chat\n");
			chat();
		} else if ( !strcmp(msg_type, "CHATN") ) {
			printf("Peer is not accepting a chat\n");
		} else if ( !strcmp(msg_type, "MSG") ) {
			printf("MESSAGE: %s\n", token2);
		}

        // >>> Step #4 <<<
        // Send to the client using the server socket
    } // End loop

    // >>> Step #5 <<<
    // Close all open sockets
    retcode = close(server_s);
    if (retcode < 0)
    {
        printf("*** ERROR - close() failed \n");
        exit(-1);
    }
}
    
char* ipAddress()
{
 int fd;
 struct ifreq ifr;

 fd = socket(AF_INET, SOCK_DGRAM, 0);

 /* I want to get an IPv4 IP address */
 ifr.ifr_addr.sa_family = AF_INET;

 /* I want IP address attached to "eth0" */
 strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

 ioctl(fd, SIOCGIFADDR, &ifr);

 close(fd);

 /* display result */
 printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
 
 ipadd = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

 return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
}
