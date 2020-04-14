#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

unsigned short sendMsg(char* payload, size_t payloadLength, const char* ipAddr, unsigned short port, sa_family_t ipType)
{
  printf("\nInside sendMsg IP[%s], port[%d]\n",ipAddr,port);
  int sock;
  size_t size;
  struct sockaddr_in6 serverAddr6;
  struct sockaddr_in serverAddr4;

  if(ipType == AF_INET6) {
    sock = socket(PF_INET6, SOCK_DGRAM, 0);

    if (sock < 0) {
      perror("creating socket");
      exit(1);
    }

    sockaddr_in6 addr = {0};
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, ipAddr, &addr.sin6_addr);
    addr.sin6_port = htons(0);

    bind(sock, (sockaddr*)&addr, sizeof(addr));
    memset(&serverAddr6, 0, sizeof(serverAddr6));
    serverAddr6.sin6_family = AF_INET6;
    inet_pton(AF_INET6, ipAddr, &serverAddr6.sin6_addr);
    serverAddr6.sin6_port = htons(port);

    size = sendto(sock, payload, payloadLength, 0 , (const struct sockaddr *) &serverAddr6,sizeof(serverAddr6));
    if(size < 0) {
      printf("Error in sending message\n");
      return -1;
    }
    sockaddr_in6 addrLocal = {0};
    socklen_t addrSize = sizeof(addrLocal);
    getsockname(sock, (sockaddr*)&addrLocal, &addrSize);
    close(sock);

    unsigned short sendPort = addrLocal.sin6_port;
    printf("[%d] bytes written to %s:%d from port-->%d\n",size, ipAddr, port, sendPort);
    return sendPort;
  }
  else {
    sock = socket(PF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
      perror("creating socket");
      exit(1);
    }

    sockaddr_in addr = {0};
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ipAddr, &addr.sin_addr);
    addr.sin_port = htons(0);

    bind(sock, (sockaddr*)&addr, sizeof(addr));
    memset(&serverAddr4, 0, sizeof(serverAddr4));
    serverAddr4.sin_family = AF_INET;
    inet_pton(AF_INET, ipAddr, &serverAddr4.sin_addr);
    serverAddr4.sin_port = htons(port);

    size = sendto(sock, payload, payloadLength, 0 , (const struct sockaddr *) &serverAddr4,sizeof(serverAddr4));
    if(size < 0) {
      printf("Error in sending message\n");
      return -1;
    }
    sockaddr_in addrLocal = {0};
    socklen_t addrSize = sizeof(addrLocal);
    getsockname(sock, (sockaddr*)&addrLocal, &addrSize);
    close(sock);

    unsigned short sendPort = addrLocal.sin_port;
    printf("[%d] bytes written to %s:%d from port-->%d\n",size, ipAddr, port, sendPort);
    return sendPort;

  }
}


unsigned short receiveMsg(char* payload, size_t payloadLength, const char* ipAddr, unsigned short port, sa_family_t ipType) {
  printf("\nInside receiveMsg IP[%s], port[%d]\n",ipAddr,port);
  int sock;
  size_t size;

  if(ipType == AF_INET6) {
    sock = socket(PF_INET6, SOCK_DGRAM, 0);
    if (sock < 0) {
      perror("creating socket");
      exit(1);
    }

    sockaddr_in6 addr = {0};
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, ipAddr, &addr.sin6_addr);
    addr.sin6_port = htons(port);

    if (bind(sock, (sockaddr*)&addr, sizeof(addr))< 0) {
      perror("bind failed");
      exit(EXIT_FAILURE);
    }

    socklen_t addrlen;
    addrlen = sizeof(addr);

    size = recvfrom(sock, payload, payloadLength, MSG_TRUNC , (struct sockaddr *) &addr,&addrlen);
    close(sock);
    char buff[1000];
    memset(buff,0,1000);
    printf("\n[%d] bytes received from %s:%d at port-->%d\n",size,inet_ntop(AF_INET6,&(addr.sin6_addr),buff,1000),addr.sin6_port,port);
    return addr.sin6_port;
  }
  else {
    sock = socket(PF_INET, SOCK_DGRAM, 0);
      if (sock < 0) {
      perror("creating socket");
      exit(1);
    }

    sockaddr_in addr = {0};
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ipAddr, &addr.sin_addr);
    addr.sin_port = htons(port);

    if (bind(sock, (sockaddr*)&addr, sizeof(addr))< 0) {
      perror("bind failed");
      exit(EXIT_FAILURE);
    }

    socklen_t addrlen;
    addrlen = sizeof(addr);

    size = recvfrom(sock, payload, payloadLength, MSG_TRUNC , (struct sockaddr *) &addr,&addrlen);
    close(sock);
    char buff[1000];
    memset(buff,0,1000);
    printf("\n[%d] bytes received from %s:%d at port-->%d\n",size,inet_ntop(AF_INET,&(addr.sin_addr),buff,1000),addr.sin_port,port);
    return addr.sin_port;
  }
}

short port = 8805;

int main(int argc, char** argv) {
  if(argc!=3) {
    printf("\n%s [IP] [IPType]\n",argv[0]);
    printf("e.g. %s 2405:201:fffb:87:1260:4bff:fea4:560 AF_INET6\n%s 172.16.128.130 AF_INET\n",argv[0],argv[0]);
    exit(-1);
  }

  const char* IP = argv[1];
  bool check=false;
  sa_family_t ipType;
  if(strcmp(argv[2],"AF_INET6")==0) {
    check=true;
    ipType = AF_INET6;
  }
  else if(strcmp(argv[2],"AF_INET")==0) {
    check=true;
    ipType = AF_INET;
  }

  if(!check) {
    printf("\nIncorrect IpType[%s]\n",argv[2]);
    exit(-1);
  }

  char payload[1000];
  memset(payload,0,1000);
  int i =1;
  unsigned short port = 8805;
  while(true){
    printf("\nIteration %d\n",i++);
    port = sendMsg(payload,1000,IP,port,ipType);
    port = receiveMsg(payload,1000,IP,port,ipType);
    sleep(2);
    printf("\n\n");
  }
}
          
