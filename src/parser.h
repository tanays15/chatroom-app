

typedef enum auth {
    ADMIN,
    USER,
    INVALID = -1
} auth_t;

typedef struct message {
    char *msg;
    int auth;
} msg_t;


msg_t parseMessage(char *);
int authorizeHeader(char *);
