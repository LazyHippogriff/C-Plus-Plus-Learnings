#include <stdio.h> //for printf()
#include <stdlib.h> //for exit()
#include <arpa/inet.h> //for inet_pton()
#include <netdb.h> // for NI_MAXHOST, getnameinfo() and gai_strerror()
#include <errno.h> // for errno
#include <string.h> // for strerror()
 
int main(int argc, char** argv) {
  if(argc<2) {
    printf("\n%s [IP]\n",argv[0]);
    printf("For e.g. %s 10.32.129.77\n",argv[0]);
    exit(-1);
  }
  struct sockaddr_in sa;
  int res = inet_pton(AF_INET, argv[1] , &sa.sin_addr);
  switch(res) {
    case 0: printf("\nInput address is not a valid IPv4 address.\n");
    case -1: if(res == -1)
               printf("\nError(%s)\n",strerror(errno));
             int n_res = inet_pton(AF_INET6, argv[1] , &sa.sin_addr);
             switch(n_res) {
               case 0: printf("\nInput address is not a valid IPv6 address.\n");
               case -1: if(n_res == -1)
                          printf("\nError(%s)\n",strerror(errno));
                        exit(-1);
               case 1: sa.sin_family = AF_INET;
             }
    case 1: sa.sin_family = AF_INET;
  }

  printf("\nsa.sin_addr.s_addr[%d]\n",sa.sin_addr.s_addr);

  char node[NI_MAXHOST];
  res = getnameinfo((struct sockaddr*)&sa, sizeof(sa), node, sizeof(node), NULL, 0, 0);
  if (res)
  {
    printf("&#37;s\n", gai_strerror(res));
    exit(1);
  }
  printf("\nIP[%s]\n",argv[1]);
  printf("HOSTNAME[%s]\n", node);
  return 0;
}
