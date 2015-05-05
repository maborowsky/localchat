#define PORT_NUM    	6021
#define PORT_NUM_TCP    6022
#define BCAST_IP    	"192.168.130.255"
#define DEBUG       	1

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
void send_to_IP(char message[50], char ip_addr[15]);
void *receive_TCP(void *arg);
int chat_setup(char send_ip[15]);
void chat_send(int sock_desc, char msg[140]);
void chat_end(int sock_desc);
void chat();
void requestChat();
int acceptChat();

// localchat.c
void logon();
void logout();

// peers.c
void who();
void addPeer(char newUsername[32]);
void getIP(char *ip[16], char user[32]);
void removePeer(char user[32]);
void removePeerNode(peer *del);

