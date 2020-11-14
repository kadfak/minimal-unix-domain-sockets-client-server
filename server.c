#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "server.socket"

int
main()
{
    int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        printf("failed to create socket: %d\n", errno);
        return 1;
    }

    struct sockaddr_un address;
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    if (unlink(SOCKET_PATH) != 0) {
        printf("unlink failed: %d\n", errno);
        return 1;
    }

    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) != 0) {
        printf("failed to bind: %d\n", errno);
        return 1;
    }
    
    if (listen(socket_fd, SOMAXCONN) != 0) {
        printf("failed to listen: %d\n", errno);
        return 1;
    }

    struct sockaddr_un client_address;
    socklen_t client_address_length = sizeof(client_address);
    int client_socket_fd = accept(socket_fd, (struct sockaddr *)&client_address, &client_address_length);
    if (client_socket_fd < 0) {
        printf("failed to accept: %d\n", errno);
        return 1;
    }

    uint8_t buffer[3];
    int n = recv(client_socket_fd, buffer, sizeof(buffer), 0);
    if (n < 0) {
        printf("failed to recv: %d\n", errno);
        return 1;
    }
    printf("received %d bytes: %s\n", n, buffer);
    
    buffer[0] = 52; // 4
    buffer[1] = 53; // 5
    buffer[2] = 54; // 6
    n = send(client_socket_fd, buffer, sizeof(buffer), 0);
    if (n < 0) {
        printf("failed to recv: %d\n", errno);
        return 1;
    }
    printf("sent %d bytes: %s\n", n, buffer);

    return 0;
}
