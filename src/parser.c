#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define HEADER_SIZE 2
// arbitrary numbers? better way to do this?
#define AUTH_HEADER 0x44
#define USER_HEADER 0x45

const msg_t ERROR_MSG = {"", -1};

msg_t parseMessage(char *req) {
    // first 2 bytes is header
    msg_t curr = ERROR_MSG;
    char *p = authorizeHeader(req, &curr);
    if (curr.auth == INVALID || p == NULL) {
        curr.msg = "Invalid Header\n";
    } else {
        curr.msg = p;
    }
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
    return end_ptr + 1;
}
