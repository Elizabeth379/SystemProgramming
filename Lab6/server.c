#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 12355
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

struct ClientInfo {
    char name[BUFFER_SIZE];
    int socket;
};

struct ClientInfo clients[MAX_CLIENTS];
int client_count = 0;

void HandleClient(struct ClientInfo *client) {
    char buffer[BUFFER_SIZE];
    int bytesRead;

    while (1) {
        bytesRead = recv(client->socket, buffer, BUFFER_SIZE, 0);
        if (bytesRead <= 0) {
            printf("Client %s disconnected.\n", client->name);
            close(client->socket);

            // Удаление отключенного клиента из массива
            for (int i = 0; i < client_count; ++i) {
                if (clients[i].socket == client->socket) {
                    for (int j = i; j < client_count - 1; ++j) {
                        clients[j] = clients[j + 1];
                    }
                    client_count--;
                    break;
                }
            }
            return;
        }

        buffer[bytesRead] = '\0'; // Добавление нулевого символа для корректного преобразования в строку
        printf("Received data from client %s: %s\n", client->name, buffer);

        // Обработка команды UPLOAD
        if (strcmp(buffer, "UPLOAD") == 0) {
            // Получаем имя файла
            bytesRead = recv(client->socket, buffer, BUFFER_SIZE, 0);
            if (bytesRead <= 0) {
                perror("Error receiving file name");
                return;
            }
            buffer[bytesRead] = '\0';
            printf("Received file name: %s\n", buffer);

            // Открываем файл для записи
            FILE* fileStream = fopen(buffer, "wb");
            if (!fileStream) {
                perror("Error opening file for writing");
                return;
            }

            // Принимаем данные и записываем их в файл
            while ((bytesRead = recv(client->socket, buffer, BUFFER_SIZE, 0)) > 0) {
                fwrite(buffer, 1, bytesRead, fileStream);
            }

            fclose(fileStream);
            printf("Received and saved file: %s\n", buffer);
        }
    }
}


void *HandleClientThread(void *arg) {
    struct ClientInfo *pClient = (struct ClientInfo *)arg;
    HandleClient(pClient);
    return NULL;
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    pthread_t tid;

    // Создание сокета
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Заполнение структуры адреса сервера
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Привязка сокета к адресу и порту
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Прослушивание входящих соединений
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Error listening to socket");
        exit(EXIT_FAILURE);
    }

    printf("Server is running and listening on port %d.\n", PORT);

    // Основной цикл для принятия входящих соединений
    while (1) {
        // Принятие входящего соединения
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize)) == -1) {
            perror("Error accepting connection");
            exit(EXIT_FAILURE);
        }

        // Получение имени клиента
        char nameBuffer[BUFFER_SIZE];
        int nameBytes = recv(clientSocket, nameBuffer, BUFFER_SIZE, 0);
        if (nameBytes > 0) {
            nameBuffer[nameBytes] = '\0';
        }

        // Добавление нового клиента в массив
        struct ClientInfo newClient;
        strcpy(newClient.name, nameBuffer);
        newClient.socket = clientSocket;
        clients[client_count++] = newClient;

        // Создание нового потока для взаимодействия с клиентом
        if (pthread_create(&tid, NULL, HandleClientThread, (void *)&newClient) != 0) {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }

        printf("Client %s connected.\n", newClient.name);
    }

    close(serverSocket);
    return 0;
}

