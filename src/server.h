typedef enum exit {
    DEFAULT,
    EXIT,
    CLOSE
} exit_t;


int create_listener(char *);
int send_all(char *, int);
int add_connection(struct pollfd *, int); // takes in the array of connections and the new capacity
int delete_connection(struct pollfd *, int);
void * resize_array(void *);
