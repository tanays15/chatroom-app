#include "parser.h"
#include <string.h>
#include <stdlib.h>

#define HEADER_SIZE 2
// arbitrary numbers? better way to do this?
#define AUTH_HEADER 0x44
#define USER_HEADER 0x45
// inputs should follow this format:
// HEADER COMMAND -> HEADER will be an x byte value, command will be a string
// headers: regular_user: SOME_HEX_CODE, admin_user: SOME_HEX_CODE
// commands: echo (server will repat what client sends), exit (client will disconnect from server), close (server will stop itself)
//
//
// Parser will have few basic methods?
// parseMessage(char *buffer) -> this will return a struct which has the permission and the command 
// authorizeHeader(char *header) -> this will take the parsed header and return the level of permission the client has

const msg_t ERROR_MSG = {"", -1};

msg_t parseMessage(char *req) {
    // first 2 bytes is header
    msg_t curr = {0};
    char *p = authorizeHeader(req, &curr);
    if (curr.auth == -1 || p == NULL) {
        curr.msg = "Invalid Header";
        return curr;
    }
    curr.msg = strdup(p);
    return curr;
}

char *authorizeHeader(char *header_str, msg_t *resp) {
    char *end_ptr;
    long header = strtol(header_str, &end_ptr, 16);
    if (end_ptr == header_str) {
        return NULL;
    }
    if (header == AUTH_HEADER) {
        resp->auth = ADMIN;
    } else if (header == USER_HEADER) {
        resp->auth = USER;
    } else {
        resp->auth = INVALID;
    }
    return end_ptr;
}

void destroyMessage(msg_t *resp) {
    free(resp->msg);
    resp->msg = NULL;
}
