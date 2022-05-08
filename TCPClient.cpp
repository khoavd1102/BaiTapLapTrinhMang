#include <stdio.h>
#define	_WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main(int argc, char* argv[]) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        printf("Loi ket noi!");
        system("pause");
        return 1;
    }

    char msg[256] = "Hello server! I am a new client.";
    send(client, msg, strlen(msg), 0);

    while (1) {
        ret = recv(client, msg, sizeof(msg), 0);
        if (ret <= 0) {
            printf("Loi ket noi!");
            break;
        }

        if (ret < 256) msg[ret] = 0;
        printf("Received: %s\n", msg);
    }

    closesocket(client);
    WSACleanup();
    return 0;
}