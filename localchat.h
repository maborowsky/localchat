#include <netinet/in.h>

#define PORT_NUM        6021 // UDP Port TODO: add _UDP to name
#define PORT_NUM_TCP    6022 // TCP Port
#define BCAST_IP        "192.168.130.255"
#define DEBUG           1

extern char username[32]; // The client's username
extern int sock_desc; // Socket descriptor of the conversation. Set to -1 if no conversation

// A node in the linked list of peers
struct peer_node
{
    char username[32];
    char ip[16];
    struct peer_node *next;
    struct peer_node *prev;
};

typedef struct peer_node peer;

extern peer *header; // The head of the linked list of peers


// networks.c
void broadcast(char message[50]);
void send_to_IP(char message[50], char ip_addr[15]);
void *receive_TCP(void *arg);
void *receive(void *arg);
void chat_setup(char send_ip[15]);
void chat_send(char msg[150]);
void chat_end();
int acceptChat();

// localchat.c
void logon();
void logout();
void requestChat();
void message();

// peers.c
void who();
peer *getPeer(char user[32]);
void addPeer(char newUsername[32], char newIP[16]);
void removePeer(char user[32]);
void removePeerNode(peer *del);

