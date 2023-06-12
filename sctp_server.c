#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <signal.h>

#define MAX_BUFFER 256

void sigpipe_handler(int unused)
{
    //printf("Client got disconnected, trying to write on closed socket!\n");
}

int main(int argc, char *argv[])
{
    int listen_sock, client_sock, ret;
    struct sockaddr_in servaddr = {0};
    struct sockaddr_in cliaddr = {0};
    struct sctp_initmsg initmsg = {0};
    char buffer[MAX_BUFFER + 1];
    int flags;
    socklen_t len;
    struct sctp_sndrcvinfo sndrcvinfo;

    if (argc != 2) {
        printf("Usage: %s [PORT]\n", argv[0]);
        exit(1);
    }

    sigaction(SIGPIPE, &(struct sigaction){sigpipe_handler}, NULL);

    listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (listen_sock == -1) {
        perror("socket()");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[1]));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(listen_sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret == -1) {
        perror("bind()");
        close(listen_sock);
        exit(1);
    }

    initmsg.sinit_num_ostreams = 5;
    initmsg.sinit_max_instreams = 5;
    initmsg.sinit_max_attempts = 4;
    ret = setsockopt(listen_sock, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg));
    if (ret == -1) {
        perror("setsockopt()");
        close(listen_sock);
        exit(1);
    }

    ret = listen(listen_sock, 5);
    if (ret == -1) {
        perror("listen()");
        close(listen_sock);
        exit(1);
    }

    while (1) {
        len = sizeof(struct sockaddr_in);
        client_sock = accept(listen_sock, (struct sockaddr *)&cliaddr, &len);
        if (client_sock == -1) {
            perror("accept()");
            continue;
        }

        //printf("Accepted connection from %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

        while (1) {
            memset(buffer, 0, sizeof(buffer));
            //ret = sctp_recvmsg(client_sock, buffer, sizeof(buffer), (struct sockaddr *)NULL, 0, &sndrcvinfo, &flags);
            ret = recv(client_sock, buffer, sizeof(buffer), 0);
            if (ret == -1) {
                perror("sctp_recvmsg()");
                break;
            }
            if (ret > 0)
                printf("Received data : %s\n", buffer);
            else if (ret == 0)
                break;

            // ret = sctp_sendmsg(client_sock, buffer, strlen(buffer), NULL, 0, sndrcvinfo.sinfo_ppid,
            //                    sndrcvinfo.sinfo_flags,
            //                    sndrcvinfo.sinfo_stream,
            //                    0,
            //                    0);
            // if (ret == -1) {
            //     perror("sctp_sendmsg()");
            //     break;
            // }
        }
        close(client_sock);
    }

    close(listen_sock);

    return 0;
}
