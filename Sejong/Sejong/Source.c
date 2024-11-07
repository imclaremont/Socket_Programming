#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock...\n");
        return 1;
    }

    char host[256] = { 0 }; // 호스트명 최대 길이
    int port;

    printf("Enter the host (e.g., example.com): ");
    scanf("%s", host);

    printf("Enter the port (e.g., 80 for HTTP or 443 for HTTPS): ");
    scanf("%d", &port);

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Failed to create socket...\n");
        WSACleanup();
        return 1;
    }

    struct hostent* hostent = gethostbyname(host);
    if (!hostent) {
        printf("Failed to resolve host...\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    struct in_addr** addr_list = (struct in_addr**)hostent->h_addr_list;
    char* ip = inet_ntoa(*addr_list[0]);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        printf("Failed to connect to the server...\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    SSL* ssl = NULL;

    if (port == 443) {
        SSL_library_init();
        SSL_CTX* ssl_ctx = SSL_CTX_new(TLS_client_method());
        ssl = SSL_new(ssl_ctx);
        SSL_set_fd(ssl, sockfd);

        if (SSL_connect(ssl) != 1) {
            printf("Failed to establish SSL/TLS connection...\n");
            SSL_free(ssl);
            SSL_CTX_free(ssl_ctx);
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }
    }

    char request[1024];
    const char* path = "/";
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, host);

    if (port == 443) {
        SSL_write(ssl, request, strlen(request));
    }
    else {
        send(sockfd, request, strlen(request), 0);
    }

    char response[10000000];
    int bytesRead;

    FILE* file = fopen("index.html", "wb");
    if (file == NULL) {
        printf("Failed to open the file for writing.\n");
        return 1;
    }

    if (port == 443) {
        while ((bytesRead = SSL_read(ssl, response, sizeof(response) - 1)) > 0) {
            response[bytesRead] = '\0';
            fwrite(response, 1, bytesRead, file);
        }
    }
    else {
        while ((bytesRead = recv(sockfd, response, sizeof(response) - 1, 0)) > 0) {
            response[bytesRead] = '\0';
            fwrite(response, 1, bytesRead, file);
        }
    }

    fclose(file);
    printf("HTML content has been successfully saved to 'index.html'.\n");

    if (port == 443) {
        SSL_free(ssl);
    }

    closesocket(sockfd);
    WSACleanup();

    return 0;
}
