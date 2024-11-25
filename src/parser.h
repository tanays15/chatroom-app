#ifndef PARSER_H
#define PARSER_H

typedef enum cmd_type {
    JOIN,
    SEND,
    LEAVE
} cmd_type;

typedef struct msg {
    cmd_type type;
    int room;
    char *user;
    char *data;
} msg_t;

msg_t parse_message(char *);
int get_command(char, msg_t*);
int validate_send(char *);
int validate_msg(char *);
int validate_leave(char *);

#endif
