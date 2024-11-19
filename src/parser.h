#ifndef PARSER_H
#define PARSER_H

typedef enum auth {
    ADMIN,
    USER,
    INVALID = -1
} auth_t;

typedef struct message {
    char *msg;
    int auth;
} msg_t;


extern msg_t parseMessage(char *);
extern char * authorizeHeader(char *, msg_t *);
extern void destroyMessage(msg_t *);
extern const msg_t ERROR_MSG;
#endif
