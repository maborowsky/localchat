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
