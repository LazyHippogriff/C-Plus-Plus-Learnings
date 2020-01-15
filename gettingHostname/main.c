#include <arpa/inet.h> //for inet_pton()
#include <netdb.h> // for NI_MAXHOST, getnameinfo() and gai_strerror()
#include <stdio.h>
#include <stdlib.h>

union {
  struct sockaddr         sa;
  struct sockaddr_in      s4;
  struct sockaddr_in6     s6;
  struct sockaddr_storage ss;
} addr;


static int convert4(struct sockaddr_in *sa, const char *name)
{
  return inet_pton(sa->sin_family = AF_INET, name, &sa->sin_addr);
}

static int convert6(struct sockaddr_in6 *sa, const char *name)
{
  return inet_pton(sa->sin6_family = AF_INET6, name, &sa->sin6_addr);
}


int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s [IP]\nE.g. %s 10.32.129.77\n", argv[0], argv[0]);
    return 1;
  }

  if (convert4(&addr.s4, argv[1]) != 1 && convert6(&addr.s6, argv[1]) != 1) {
    fprintf(stderr, "%s: not a valid IP address.\n", argv[1]);
    return 1;
  }

  char node[NI_MAXHOST];
  int res = getnameinfo(&addr.sa, sizeof addr, node, sizeof node, NULL, 0, NI_NAMEREQD);
  if (res) {
    fprintf(stderr, "%s: %s\n", argv[1], gai_strerror(res));
    exit(1);
  }

  fprintf(stdout,"\nIP[%s],Hostname[%s]\n",argv[1],node);
}
