#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

unsigned short sendMsg(char* payload, size_t payloadLength, const char* ip6Addr, unsigned short port)
{
        int sock;
        size_t size;
        struct sockaddr_in6 server_addr, client_addr;

        sock = socket(PF_INET6, SOCK_DGRAM, 0);

        if (sock < 0) {
                perror("creating socket");
                exit(1);
        }

        sockaddr_in6 addr = {0};
        memset(&addr, 0, sizeof(addr));
        addr.sin6_family = AF_INET6;
        inet_pton(AF_INET6, ip6Addr, &addr.sin6_addr);
        addr.sin6_port = htons(0);

        bind(sock, (sockaddr*)&addr, sizeof(addr));
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin6_family = AF_INET6;
        inet_pton(AF_INET6, ip6Addr, &server_addr.sin6_addr);
        server_addr.sin6_port = htons(port);

        size = sendto(sock, payload, payloadLength, 0 , (const struct sockaddr *) &server_addr,sizeof(server_addr));
        if(size < 0) {
          printf("Error in sending message\n");
          return -1;
        }
        sockaddr_in6 addrLocal = {0};
        socklen_t addrSize = sizeof(addrLocal);
        getsockname(sock, (sockaddr*)&addrLocal, &addrSize);

        port = addrLocal.sin6_port;
        printf("[%d] bytes written to network at port-->%d\n",size, port);
        return port;
}

unsigned short receiveMsg(char* payload, size_t payloadLength, const char* ip6Addr, unsigned short port) {
  int sock;
  size_t size;

  sock = socket(PF_INET6, SOCK_DGRAM, 0);

  if (sock < 0) {
    perror("creating socket");
    exit(1);
  }

  sockaddr_in6 addr = {0};
  memset(&addr, 0, sizeof(addr));
  addr.sin6_family = AF_INET6;
  inet_pton(AF_INET6, ip6Addr, &addr.sin6_addr);
  addr.sin6_port = htons(port);

  if (bind(sock, (sockaddr*)&addr, sizeof(addr))< 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

    socklen_t addrlen;
    addrlen = sizeof(addr);

  size = recvfrom(sock, payload, payloadLength, MSG_TRUNC , (struct sockaddr *) &addr,&addrlen);
  printf("\nNumber of bytes received-->%d. From port-->%d\n",size,addr.sin6_port);
  return addr.sin6_port;
}

short port = 8805;

int main() {
  char payload[1000];
  memset(payload,0,1000);
  int i =1;
  unsigned short port = 8805;
  while(true){
    printf("\nIteration %d\n",i++);
    port = sendMsg(payload,1000,"2405:201:fffb:87:1260:4bff:fea4:560",port);
    port = receiveMsg(payload,1000,"2405:201:fffb:87:1260:4bff:fea4:560",port);
    sleep(2);
    printf("\n\n");
  }
}
