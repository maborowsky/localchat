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

#include "localchat.h"


int sock_desc;

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

void chat_setup(char send_ip[15]) {
	printf(send_ip);
    int                  client_s;        // Client socket descriptor
    struct sockaddr_in   server_addr;     // Server Internet address
    char                 out_buf[4096];   // Output buffer for data
    char                 in_buf[4096];    // Input buffer for data
    int                  retcode;         // Return code
    int                  finished = 0;


    // >>> Step #1 <<<
    // Create a client socket
    //   - AF_INET is Address Family Internet and SOCK_STREAM is streams
    client_s = socket(AF_INET, SOCK_STREAM, 0);
    if (client_s < 0)
    {
        printf("*** ERROR - socket() failed \n");
        exit(-1);
    }

    // >>> Step #2 <<<
    // Fill-in the server's address information and do a connect with the
    // listening server using the client socket - the connect() will block.
    //
    server_addr.sin_family = AF_INET;                 // Address family to use
    server_addr.sin_port = htons(PORT_NUM_TCP);       // Port num to use
    server_addr.sin_addr.s_addr = inet_addr(send_ip); // IP address to use

    retcode = connect(client_s, (struct sockaddr *)&server_addr, 
            sizeof(server_addr));
    if (retcode < 0)
    {
        printf("*** ERROR - connect() failed \n");
    }

    sock_desc = client_s;

    // >>> Step #3 <<<
    // Receive from the server using the client socket
    //MIGHT BE ABLE TO MAKE NEW RECEIVE THREAD USING A DRIVER METHOD AND RECV
    //    retcode = recv(client_s, in_buf, sizeof(in_buf), 0);
    //    if (retcode < 0)
    //    {
    //        printf("*** ERROR - recv() failed \n");
    //        exit(-1);
    //    }

}

void chat_send(char msg[140], char user[32]) {
    if (sock_desc == -1) { 
        printf("No current chat"); 
        return;
    }

    send(sock_desc, msg, strlen(msg), 0);

    //char data[140];
    //printf("Send Chat Message: ");
    //fgets(data, 140, stdin);
    //strtok(data, "\n");
    //char message[50] = "MSG:";
    //strcat(message, data);
}

void chat_end() {
    // >>> Step #5 <<<man
    // Close the client socket
    int retcode = close(sock_desc);
    if (retcode < 0)
    {
        printf("*** ERROR - close() failed \n");
        exit(-1);
    }

    sock_desc = -1;
}


void *receive_TCP(void *arg) {
    int                  server_s;             // Server socket descriptor
    struct sockaddr_in   server_addr;          // Server Internet address
    int                  client_s;             // Client socket descriptor
    struct sockaddr_in   client_addr;          // Client Internet address
    struct in_addr       client_ip_addr;       // Client IP address
    socklen_t            addr_len;             // Internet address length
    char           in_buf[150];     // Input buffer
    int                  retcode;              // Return code

    // Create a socket, fill-in address information, and then bind it
    server_s = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUM);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to an address
    retcode = bind(server_s, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (retcode < 0) {
        printf("*** ERROR - bind() failed \n");
        exit(-1);
    }

    // Set-up the listen
    listen(server_s, 100);

    printf(">>> capServer is running on port %d <<< \n", PORT_NUM);

    // Main loop to accept connections and then create
    // a thread to handle the client.

    addr_len = sizeof(client_addr);
    // Accept a connection from a client
    client_s = accept(server_s, (struct sockaddr *)&client_addr, &addr_len);
    
    
    //Loop header
    char msg_type[12]; // The type of message that was recieved (token1)
    char token2[140]; // The token after msg_type
    char* token; // Used to tokenize strings
    while(1) {
		// Receive a cap request from a client
        memset(in_buf,0, 150);
        retcode = recv(client_s, in_buf, 150, 0);
        printf("Received from client: %s\n", in_buf);

        // If the recv() return code is bad then bail-out
        if (retcode <= 0)
        {
            close(client_s);
            pthread_exit(NULL);
        }

       

        // Get the message type from packet
        token = strtok(in_buf, ":");
        strcpy(msg_type, token);

        token = strtok(NULL, ":");
        if (token != NULL) {
            strcpy(token2, token);
        } else {
            // Packets without a second token
        }
        printf(msg_type);
        if ( !strcmp(msg_type, "CHATREQ") ) {
            if (acceptChat() == 1) {
                //------------------------------------------------------------------------------------------------------
                printf("aaaaaaaaaaaaaaaaaaa");
                //setup chat
                //send(client_s, out_buf, strlen(out_buf), 0);
                // end chat(not here, but eventually)
            }
        } else if ( !strcmp(msg_type, "CHATY") ) {
            printf("Peer accepted chat");
            //chat();
        } else if ( !strcmp(msg_type, "CHATN") ) {
            printf("Peer is not accepting a chat");
        } else if ( !strcmp(msg_type, "MSG") ) {
            printf("MESSAGE: %s\n", token2);
        }
    } // End loop

    //NEVER GETS HERE
    // >>> Step #5 <<<man
    // Close the client socket
    retcode = close(client_s);
    if (retcode < 0)
    {
        printf("*** ERROR - close() failed \n");
        exit(-1);
    }
}



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
    char                 peer_ip[16];

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
    char token2[32]; // The token after msg_type
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

        // Get the peer's IP
        // If getting weird IP errors, strcpy may not be working right
        strncpy(peer_ip, inet_ntoa(client_ip_addr), sizeof(peer_ip));
        peer_ip[sizeof(peer_ip) - 1] = '\0';
        //printf(peer_ip);

        // Get the message type from packet
        token = strtok(in_buf, ":");
        strcpy(msg_type, token);

        token = strtok(NULL, ":");
        if (token != NULL) {
            strcpy(token2, token);
        } else {
            // Packets without a second token
        }

        if ( !strcmp(msg_type, "HELLO") ) {
            // TODO check if user is already in table. Should implement checkUser()
            // Do not add self to table
            if ( strcmp(token2, username) ) {
                addPeer(token2, peer_ip); // token2 will be the username

                // Send an ok packet back
                char ok_msg[50] = "OK:";
                strcat(ok_msg, username);
                strcat(ok_msg, ":Y");
                //send_to_IP(client_addr.sin_addr.s_addr);
                send_to_IP( ok_msg, inet_ntoa(client_addr.sin_addr) ); 
            }
        } else if ( !strcmp(msg_type, "OK") ) {
            // TODO check if user is already in table. Should implement checkUser()
            addPeer(token2, peer_ip); // token2 will be the username
        } else if ( !strcmp(msg_type, "BYE") ) {
            printf("removing peer %s\n", token2);
            removePeer(token2);
            fflush(stdout);
        }

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


int acceptChat()
{
    char yes[6] = "CHATY:";
    char no[6] = "CHATN:";
    printf("Do you want to accept the chat? Answer 'Yes' or 'No'\n");
    char accept[32];
    fgets(accept, 32, stdin);
    if ( !strcmp(accept, "Yes\n") || !strcmp(accept, "yes\n") || !strcmp(accept, "y\n") || !strcmp(accept, "Y\n")  ) {
        broadcast(yes);
        return 1;
    }
    else {
        broadcast(no);
        return 0;
    }
}

