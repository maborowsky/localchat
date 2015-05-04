/*
 * peers.c
 * TODO when removing, have to free memory
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


peer *header;

/*
 * Print everyone in peerTable
 */
void who() {
    peer *temp = header;

    printf("-- PEERS ----------\n");
    fflush(stdout);
    while (temp != NULL) {
        printf(temp->username);
        printf("    ");
        printf(temp->ip);
        printf("\n");
        temp = temp->next;
    }
    printf("--------------------\n\n");
}

void addPeer(char newUsername[32], char newIP[15]) {
    // TODO check if user is already in table
    peer *temp;

    temp = (peer *) malloc( sizeof(peer) );
    strcpy(temp->username, newUsername);
    strcpy(temp->username, newIP);
    temp->next = header;
    temp->prev = NULL;

    if (header != NULL) {
        header->prev = temp;
    }

    header = temp;
}

void removePeer(char user[32]) {
    peer *temp = header;

    while ( strcmp(temp->next->username, user) ) {
        temp = temp->next;

    }

    if (temp == NULL) {
        printf ("No user found with username %s", user);
        return;
    } else {
        if (DEBUG) { printf("Peer to remove: %s", temp->username); }
        removePeerNode(temp);
    }
}

/*
 * Remove a peer node from the list of peers
 * with a given pointer to a node
 *
 * del  Pointer to the node to be deleted
 */
void removePeerNode(peer *del)
{
    // Base case
    if(header == NULL || del == NULL)
        return;

    // If the node to be deleted is the head node
    if(header == del)
        header = del->next;

    // Change next only if node to be deleted is NOT the last node
    if(del->next != NULL)
        del->next->prev = del->prev;

    // Change prev only if node to be deleted is NOT the first node
    if(del->prev != NULL)
        del->prev->next = del->next;     

    /* Finally, free the memory occupied by del*/
    free(del);
    return;
}
