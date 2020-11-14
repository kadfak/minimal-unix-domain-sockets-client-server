#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "server.socket"

int main() {
    struct sockaddr_un address;
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        printf("failed to create socket: %d\n", errno);
        return 1;
    }

    if (connect(socket_fd, (struct sockaddr *)&address, sizeof(address)) != 0) {
        printf("failed to connect: %d\n", errno);
        return 1;
    }

    uint8_t buffer[3] = {49, 50, 51}; // 123
    int n = send(socket_fd, buffer, sizeof(buffer), 0);
    if (n < 0) {
        printf("failed to send: %d\n", errno);
        return 1;
    }
    printf("sent %d bytes: %s\n", n, buffer);

    n = recv(socket_fd, buffer, sizeof(buffer), 0);
    if (n < 0) {
        printf("failed to recv: %d\n", errno);
        return 1;
    }
    printf("received %d bytes: %s\n", n, buffer);

    return 0;
}
