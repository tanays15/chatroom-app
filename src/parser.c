#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


msg_t parse_message(char *buffer) {
    msg_t empty = {0, 0, "", ""};
    return empty;
}
int get_command(char cmd, msg_t *msg) {
    if (cmd == 'j') {
        msg->type = JOIN; 
    } else if (cmd == 'm') {
        msg->type = SEND;
    } else if (cmd == 'l') {
        msg->type = LEAVE;
    } else {
        return -1;
    }
    return 0;
}
int validate_join(char *data) {
    return 0;
}
int validate_send(char *data) {
    return 0;
}
int validate_leave(char *data) {
    return 0;
}
