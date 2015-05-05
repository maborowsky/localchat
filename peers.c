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

#include <net/if.h>		  // Needed for local ip
#include <ifaddrs.h>	  // Needed for local ip
#include <errno.h>		  // Needed for local ip

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
        printf("\n");
        temp = temp->next;
    }
    printf("--------------------\n\n");
}

void addPeer(char newUsername[32]) {
    // TODO check if user is in table
    peer *temp;

    temp = (peer *) malloc( sizeof(peer) );
    strcpy(temp->username, newUsername);
    temp->next = header;
    temp->prev = NULL;

    if (header != NULL) {
        header->prev = temp;
    }

    header = temp;
}

void removePeer(char user[32]) {
    peer *temp = header;
    
    if(strcmp(temp->username, user) == 0)
    {
		header = temp->next;
		free(temp);
	}
	
    while (temp->next != NULL)
	{
		if (strcmp(user,temp->next->username) == 0)
		{
			removePeerNode(temp->next);
			return;
		}
		temp = temp->next;
		fflush(stdout);
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
    peer *temp = del;
    if (temp->next != NULL)
    {
		temp->prev->next = temp->next;
	}
	else
		temp->prev->next = NULL;
    
    free(del);
}

int checkTable (char user[32])
{
	if (header == NULL)
		return 0;
	
	peer *temp = header;

	if(strcmp(temp->username, user) == 0)
    {
		return 1;
	}
	
	while (temp->next != NULL)
	{
		if (strcmp(user,temp->next->username) == 0)
		{
			return 1;
		}
		temp = temp->next;
		fflush(stdout);
	}
	return 0;
}

