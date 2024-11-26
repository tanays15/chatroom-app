#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BASE_10 10

const msg_t ERR_MSG = {ERR, -1, "", ""};

msg_t parse_message(char *buffer) {
    if (strlen(buffer) < 1) {
        return ERR_MSG;
    }
    char *p = buffer;
    msg_t new_msg;
    char cmd = *p++;
    cmd_type type;
    int validate;
    if ((type = get_command(cmd)) == ERR) {
        return ERR_MSG;
    }
    new_msg.type = type;
    while (*p == ' ') {
        p++;
    }
    if (type == JOIN) {
        validate = validate_join(p);
    } else if (type == SEND) {
        validate = validate_send(p);
    } else {
        validate = validate_leave(p);
    }
    if (validate == -1) {
        return ERR_MSG;
    }
    return new_msg;
}

cmd_type get_command(char cmd) {
    if (cmd == 'j') {
        return JOIN;
    } else if (cmd == 'm') {
        return SEND;
    } else if (cmd == 'l') {
        return LEAVE;
    } else {
        return ERR;
    }
}

int validate_join(char *data) {
    char *end_ptr;
    long chatroom = strtol(data, &end_ptr, BASE_10);
    if (*end_ptr == '\0' || end_ptr == data) {
        return -1;
    }
    return chatroom;
}
int validate_send(char *data) {
    return *data == '\0' ? -1 : 0;
}
int validate_leave(char *data) {
    return *data != '\0' ? -1 : 0;
}
