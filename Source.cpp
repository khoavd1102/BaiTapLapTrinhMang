#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;
    int ret;
    char buf[256];
    char user[32], pass[32], tmp[32];
    // Xu ly dang nhap
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            return 0;
        buf[ret] = 0;
        printf("Du lieu nhan duoc: %s\n", buf);
        ret = sscanf(buf, "%s %s %s", user, pass, tmp);
        if (ret != 2)
        {
            const char* msg = "Sai cu phap. Hay nhap lai.\n";
            send(client, msg, strlen(msg), 0);
        }
        else
        {
            FILE* f = fopen("D:\\data.txt", "r");
            int found = 0;
            char diemf[32];
            while (!feof(f))
            {
                char line[256];
                fgets(line, sizeof(line), f);
                char userf[32], passf[32];
                sscanf(line, "%s %s %s", userf, passf, diemf);
                if (strcmp(user, userf) == 0 && strcmp(pass, passf) == 0)
                {
                    found = 1;
                    break;
                }
            }
            fclose(f);
            if (found == 1)
            {
                const char* msg = "Diem cua ban la: ";
                char* msg1;
                strcat(msg1, diemf);
                strcat(msg1, "\n");
                send(client, msg1, strlen(msg1), 0);
                break;
            }
            else
            {
                const char* msg = "Khong tim thay tai khoan. Hay nhap lai.\n";
                send(client, msg, strlen(msg), 0);
            }
        }
    }
    // Thuc hien lenh
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            return 0;
        buf[ret] = 0;
        printf("Du lieu nhan duoc: %s\n", buf);
        buf[ret - 1] = 0;
        char cmdbuf[256];
        sprintf(cmdbuf, "%s > c:\\test\\out.txt", buf);
        system(cmdbuf);
        FILE* f = fopen("c:\\test\\out.txt", "r");
        while (!feof(f))
        {
            char line[256];
            fgets(line, sizeof(line), f);
            send(client, line, strlen(line), 0);
        }
        fclose(f);
    }
    closesocket(client);
}
int main()
{
    // Khoi tao thu vien
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    // Tao socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Khai bao dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);
    // Gan cau truc dia chi voi socket
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    // Chuyen sang trang thai cho ket noi
    listen(listener, 5);
    while (1)
    {
        SOCKET client = accept(listener, NULL, NULL);
        printf("Client moi ket noi: %d\n", client);
        CreateThread(0, 0, ClientThread, &client, 0, 0);
    }
}