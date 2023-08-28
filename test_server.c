#include "atcli.h"

//-----------------------------------------------------------------------------

#define BUFFER_SIZE  CMD_LENGTH_MAX

//-----------------------------------------------------------------------------

int main(void) {
    printf("Server is running!\n");
    fflush(stdout);

    // Delete old socket (if exists).
    unlink(SOCKET_PATH);

    // Create local socket.
    int sfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (sfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Create and clear socket structure.
    struct sockaddr_un name;
    memset(&name, 0, sizeof(name));

    // Bind socket to socket name.
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_PATH, sizeof(name.sun_path) - 1);
    int res = bind(sfd, (const struct sockaddr *) &name, sizeof(name));
    if (res == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for connections.
    res = listen(sfd, 20);
    if (res == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int asfd = 0;  // Accepted socket file descriptor.
    char buffer[BUFFER_SIZE];
    bool flag_end = false;

    // Process incoming connections.
    while (true) {
        
        // Wait for incoming connections.
        asfd = accept(sfd, NULL, NULL);
        if (asfd == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        while (true) {
            
            // Wait for the next data packet.
            res = read(asfd, buffer, BUFFER_SIZE);
            if (res == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            // Check end of the buffer.
            buffer[BUFFER_SIZE - 1] = 0;
            
            // Process enf command.
            if (!strncmp(buffer, "0", res)) {
                flag_end = true;
                break;
            }

            // Print read result.
            printf("%s\n", buffer);
            fflush(stdout);
        }

        // Close socket.
        close(asfd);

        if (flag_end)
            break;
    }
    close(sfd);

    // Delete socket.
    unlink(SOCKET_PATH);

    printf("Goodbye! %d\n", UNIVERSE_ANSWER);
    exit(EXIT_SUCCESS);
}