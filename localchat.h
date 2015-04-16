#define PORT_NUM    6021
#define BCAST_IP    "192.168.130.255"
#define DEBUG       1

extern char username[32]; // The client's username

// A node in the linked list of peers
struct peer_node
{
    char username[32];
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

// peers.c
void who();
void addPeer(char newUsername[32]);
void removePeer(char user[32]);
void removePeerNode(peer *del);
