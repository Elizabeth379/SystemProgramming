#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void SendCommand(int clientSocket, const char* command) {
    if (send(clientSocket, command, strlen(command), 0) < 0) {
        perror("Error sending the command");
        exit(EXIT_FAILURE);
    }
}

void SendFileName(int clientSocket, const char* fileName) {
    if (send(clientSocket, fileName, strlen(fileName), 0) < 0) {
        perror("Error sending the file name");
        exit(EXIT_FAILURE);
    }
}

void SendFile(int clientSocket, const char* filePath) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    FILE* file = fopen(filePath, "rb");
    if (!file) {
        perror("Error opening file for reading");
        return;
    }

    // Отправляем команду "UPLOAD" серверу
    

    // Отправляем имя файла серверу
    const char* fileName = strrchr(filePath, '/') + 1; // Извлекаем имя файла из полного пути
    SendFileName(clientSocket, fileName);

    // Отправляем содержимое файла
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (send(clientSocket, buffer, bytesRead, 0) < 0) {
            perror("Error sending file data");
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("Sent file to the server: %s\n", filePath);
}


void ReceiveFile(int clientSocket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    // Отправляем команду "DOWNLOAD" серверу
    char downloadCommand[] = "DOWNLOAD";
    if (send(clientSocket, downloadCommand, sizeof(downloadCommand), 0) < 0) {
        fprintf(stderr, "Error sending the download command.\n");
        return;
    }

    printf("Enter the file name you want to download: ");
    char fileNameToDownload[BUFFER_SIZE];
    scanf("%s", fileNameToDownload);

    // Отправляем имя файла, который хотим скачать
    if (send(clientSocket, fileNameToDownload, strlen(fileNameToDownload), 0) < 0) {
        fprintf(stderr, "Error sending the file name to download.\n");
        return;
    }

    FILE *receivedFile = fopen(fileNameToDownload, "wb");
    if (!receivedFile) {
        fprintf(stderr, "Error opening file for writing: %s\n", fileNameToDownload);
        return;
    } else {
        while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
            fwrite(buffer, 1, bytesRead, receivedFile);
        }
        fclose(receivedFile);
        printf("Received and saved file: %s\n", fileNameToDownload);
    }
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(12355);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error connecting to the server");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    char name[] = "Client1"; // Уникальное имя клиента
    send(clientSocket, name, strlen(name), 0);

    printf("Choose an option:\n1. Upload a file to the server\n2. Download a file from the server\n");
    int option;
    scanf("%d", &option);

    if (option == 1) {
    	SendCommand(clientSocket, "UPLOAD");
        char filePath[BUFFER_SIZE];
        printf("Enter the full path to the file you want to send: ");
        scanf("%s", filePath);
        getchar(); // Очищаем буфер ввода
        SendFile(clientSocket, filePath);
    } else if (option == 2) {
        ReceiveFile(clientSocket);
    } else {
        fprintf(stderr, "Invalid option.\n");
    }

    close(clientSocket);

    return 0;
}

