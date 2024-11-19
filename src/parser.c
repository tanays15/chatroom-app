// c file to parse incoming messages from clients
// inputs should follow this format:
// HEADER COMMAND -> HEADER will be an x byte value, command will be a string
// headers: regular_user: SOME_HEX_CODE, admin_user: SOME_HEX_CODE
// commands: echo (server will repat what client sends), exit (client will disconnect from server), close (server will stop itself)
//
//
// Parser will have few basic methods?
// parseMessage(char *buffer) -> this will return a struct which has the permission and the command 
// authorizeHeader(char *header) -> this will take the parsed header and return the level of permission the client has
