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

#include "localchat.h"


char username[32];
pthread_t listener;
pthread_t listener_tcp;


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
        } else if ( !strcmp(input, "chat\n") ) {
            requestChat();
        } else if ( !strcmp(input, "msg\n") ) {
            message();
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

    // Listening threads
    if ( pthread_create(&listener, NULL, receive, NULL) ) { // UDP
        printf("error creating thread\n");
        abort();
    }
    if ( pthread_create(&listener_tcp, NULL, receive_TCP, NULL) ) { // TCP
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

    if (sock_desc != -1) { chat_end; }
    // TODO: Close all ongoing conversations

    // Close listening thread
    //if ( pthread_join(&listener, NULL ) ) {
    //    printf("error joining thread.");
    //    abort();
    //}
    pthread_cancel(&listener);
    pthread_cancel(&listener_tcp);
}

void requestChat()
{
    char user[32];

    //TODO: error check
    printf("With whom would you like to chat? ");
    fgets(user, 32, stdin);
    strtok(user, "\n"); // Gets rid of trailing "\n"
    char request[8] = "CHATREQ:";
    chat_setup(getPeer(user)->ip);
    chat_send(request);
}

void message() {
    if (sock_desc == -1) { printf("\nNo conversation"); return; }

    char message[150];
    char msg[140];
    strcpy(message, "MSG:");
    printf("msg: ");fflush(stdout);
    fgets(msg, 140, stdin);
    strcat(message, msg);
    chat_send(message);
    printf("\nsent: "); printf(message);
}
