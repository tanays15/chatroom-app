#ifndef PARSER_H
#define PARSER_H

typedef enum cmd_type {
    JOIN,
    SEND,
    LEAVE,
    ERR
} cmd_type;

typedef struct msg {
    cmd_type type;
    int room;
    char *data;
} msg_t;

extern const msg_t ERR_MSG;

msg_t parse_message(char *);
cmd_type get_command(char);
int validate_join(char *);
int validate_send(char *);
int validate_leave(char *);

#endif
