/*
 * localchat.c
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

#include <net/if.h>		  // Needed for local ip
#include <ifaddrs.h>	  // Needed for local ip
#include <errno.h>		  // Needed for local ip

#include "localchat.h"


char username[32];
pthread_t listener;

void main(void)
{
	logon();
    
    // Main loop (command line)
    char input[32];
    
    while( strcmp(input,"exit\n") ) {
        printf("> ");
        fgets(input, 32, stdin);

        if ( !strcmp(input, "who\n") ) {
            who();
        } else if ( !strcmp(input, "test\n") ) {
            broadcast("HELLO:username");
        } else if ( !strcmp(input, "chat\n") ) {
			requestChat();
		}
		
		
    } // End loop
	
    logout();
}

/*
 * Gets a username, opens up a listening thread, and broadcasts a hello message
 */
void logon()
{
    // Get a username
    printf("Please enter a username: "); 
    fgets(username, 32, stdin);
    strtok(username, "\n"); // Gets rid of trailing "\n"

    // Listening thread
    if ( pthread_create(&listener, NULL, receive, NULL) ) {
        printf("error creating thread\n");
        abort();
    }
    

    // Broadcast hello message
    char hello[50] = "HELLO:";
    strcat(hello, username);
    broadcast(hello);

} // End logon

void logout()
{
    // Broadcast bye message
    char bye[50] = "BYE:";
    strcat(bye, username);
    broadcast(bye);

    // Close listening thread
    //if ( pthread_join(&listener, NULL ) ) {
    //    printf("error joining thread.");
    //    abort();
    //}
    pthread_cancel(&listener);
}

void requestChat()
{
	char request[8] = "CHATREQ:";
	broadcast(request);
}

void acceptChat()
{
	char yes[6] = "CHATY:";
	char no[6] = "CHATN:";
	printf("Do you want to accept the chat? Answer 'Yes' or 'No'\n");
	char accept[32];
	fgets(accept, 32, stdin);
	if ( !strcmp(accept, "Yes\n") ) {
		broadcast(yes);
	}
	else if ( !strcmp(accept, "No\n") ) {
		broadcast(no);
	}
}

void chat()
{
	char data[140];
	printf("Send Chat Message: ");
	fgets(data, 140, stdin);
	strtok(data, "\n");
	char message[50] = "MSG:";
	strcat(message, data);
	broadcast(message);
}
