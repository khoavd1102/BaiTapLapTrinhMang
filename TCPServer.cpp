#include <stdio.h>
#define	_WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    listen(listener, 5);
    
    SOCKET client = accept(listener, NULL, NULL);
    char msg[256];

    int ret = recv(client, msg, sizeof(msg), 0);
    if (ret <= 0)
    {
        printf("Loi ket noi!");
        system("pause");
        return 1;
    }

    if (ret < 256) msg[ret] = 0;
    printf("Received: %s\n", msg);

    while (1) {
        printf("Nhap xau ki tu: ");
        fgets(msg, sizeof(msg), stdin);
        if (strcmp(msg, "exit") == 0) break;
        send(client, msg, strlen(msg), 0);
    }

    closesocket(client);
    closesocket(listener);
    WSACleanup();

    return 0;
}