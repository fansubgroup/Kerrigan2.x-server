#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 15636

void *thread(void *vargp);
void *threadsend(void *vargp);
void *threadrecv(void *vargp);

void*thread(void *vargp)
{

    pthread_t tid1, tid2;

    int connfd = *((int *)vargp);

    int idata;

    char temp[100];

    pthread_create(&tid1, NULL, threadsend, vargp);

    pthread_create(&tid2, NULL, threadrecv, vargp);

    return NULL;

}

void *threadsend(void *vargp)
{

    int connfd = *((int *)vargp);

    int idata;

    char temp[100];

    while(1){

        fgets(temp, 100, stdin);

        send(connfd, temp, 100, 0);

        printf("   Server Send OK   \n");

    }

    return NULL;

}

void *threadrecv(void *vargp)
{

    char temp[100];

    int connfd = *((int *)vargp);

    while(1){

        int idata = 0;

        idata = recv(connfd, temp, 100, 0);

        if(idata > 0){

            printf("<<Client>>:\n%s\n", temp);

        }
    }

    return NULL;
}

int main()
{

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if(listenfd < 0){

        perror("socket");

        exit(1);

    }

    struct hostent *hp;

    struct sockaddr_in serveraddr;

    bzero((char *)&serveraddr, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;

    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    serveraddr.sin_port = htons(PORT);

    if(bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){

        perror("connect");

        exit(1);

    }

    if(listen(listenfd, 1024) < 0){

        perror("listen error");

        exit(1);

    }

    struct sockaddr_in clientaddr;

    int clientlen, *connfdp;

    clientlen = sizeof(clientaddr);

    while(1){

        connfdp = (int *)malloc(sizeof(int));

        *connfdp = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);

        pthread_t tid;

        printf("Accepted!\n");

        pthread_create(&tid, NULL, thread, connfdp);
    }

    return EXIT_SUCCESS;
}
