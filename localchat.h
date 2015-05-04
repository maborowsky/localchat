#include <netinet/in.h>

#define PORT_NUM        6021 // UDP Port TODO: add _UDP to name
#define PORT_NUM_TCP    6022 // TCP Port
#define BCAST_IP        "192.168.130.255"
#define DEBUG           1

extern char username[32]; // The client's username

// A node in the linked list of peers
struct peer_node
{
    char username[32];
    char ip[15];
    struct peer_node *next;
    struct peer_node *prev;
};

typedef struct peer_node peer;

extern peer *header; // The head of the linked list of peers


// networks.c
void broadcast(char message[50]);
void send_to_IP();
void *receive(void *arg);

// localchat.c
void logon();
void logout();
void chat();
void requestChat();
void acceptChat();

// peers.c
void who();
void addPeer(char newUsername[32], char newIP[15]);
void removePeer(char user[32]);
void removePeerNode(peer *del);

