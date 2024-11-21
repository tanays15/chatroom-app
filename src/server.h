typedef enum exit {
    DEFAULT,
    EXIT,
    CLOSE
} exit_t;


int create_listener(char *);
int send_all(char *, int);
