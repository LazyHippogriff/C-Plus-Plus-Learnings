#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int g_socket;

void bindSocket(const sa_family_t fa_ipType, const char* fa_ip, const int fa_port) {
  sockaddr_in6 addr6 = {0};
  sockaddr_in addr4 = {0};

  int retCode = 0;
  if(fa_ipType == AF_INET6) {
    g_socket = socket(PF_INET6, SOCK_DGRAM, 0);

    if (g_socket < 0) {
      printf("Error while creating socket");
      exit(1);
    }

    addr6.sin6_family = fa_ipType;
    inet_pton(fa_ipType, fa_ip, &addr6.sin6_addr);
    addr6.sin6_port = htons(fa_port);
    retCode = bind(g_socket, (sockaddr*)&addr6, sizeof(addr6));
    if(retCode != 0) {
      printf("Error during bind system call-->%s",strerror(retCode));
    }
    else {
      printf("\nSocket bound successfully with IP-->%s,Port-->%d\n",fa_ip,fa_port);
    }
  }
  else {
    g_socket = socket(PF_INET, SOCK_DGRAM, 0);

    if (g_socket < 0) {
      printf("Error while creating socket.");
      exit(1);
    }

    addr4.sin_family = fa_ipType;
    inet_pton(fa_ipType, fa_ip, &addr4.sin_addr);
    addr4.sin_port = htons(fa_port);
    retCode = bind(g_socket, (sockaddr*)&addr4, sizeof(addr4));
    if(retCode != 0) {
      printf("Error during bind system call-->%s",strerror(retCode));
    }
    else {
      printf("\nSocket bound successfully with IP-->%s,Port-->%d\n",fa_ip,fa_port);
    }

  }
}

int main(int argc, char** argv)
{
        if(argc != 4) {
                printf("%s [IP-Type] [IP] [Port]\ne.g.%s 6 2405:200:1413:100::5:1c 8825\n%s 4 10.32.129.78 8825\n",argv[0],argv[0],argv[0]);
                return 0;
        }

        sa_family_t l_ipType;
        int mode = std::stoi(argv[1]);
        if(mode == 4) {
                l_ipType = AF_INET;
        }
        else if(mode == 6) {
                l_ipType = AF_INET6;
        }
        else {
                printf("\nInvalid IP Type\n");
                return 0;
        }

        const char* l_ip = argv[2];
        int l_port = std::stoi(argv[3]);
        bindSocket(l_ipType,  l_ip, l_port);

        sockaddr_in my_addr = {0};
        sockaddr_in6 my_addr6 = {0};
        int myPort;
        uint32_t my_addrLen = sizeof(my_addr);
        uint32_t my_addrLen6 = sizeof(my_addr6);
        char myIP[200]={0};
        if(l_ipType==AF_INET) {
                if(getsockname(g_socket,(struct sockaddr *)&my_addr,&my_addrLen) != 0) {
                        printf("\nError in getsockname()\n");
                        return 0;
                }
                inet_ntop(AF_INET, &my_addr.sin_addr, myIP, sizeof(myIP));
                myPort = ntohs(my_addr.sin_port);
        }
        else {
                if(getsockname(g_socket,(struct sockaddr *)&my_addr6,&my_addrLen6) != 0) {
                        printf("\nError in getsockname()\n");
                        return 0;
                }
                inet_ntop(AF_INET6, &my_addr6.sin6_addr, myIP, sizeof(myIP));
                myPort = ntohs(my_addr6.sin6_port);
        }

        printf("Local ip address: %s\n", myIP);
        printf("Local port : %u\n", myPort);

        return 0;
}
