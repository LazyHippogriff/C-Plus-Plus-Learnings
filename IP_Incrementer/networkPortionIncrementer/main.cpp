//This program increments only the network portion of the IP and keeps the host portion unchanged.

#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <string>
#include <math.h>

using namespace std;

const char *bit_rep[16] = {
    [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
    [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
    [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
    [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

unsigned char hostBits[8] = {
    [0] = 0b0,
    [1] = 0b1,
    [2] = 0b11,
    [3] = 0b111,
    [4] = 0b1111,
    [5] = 0b11111,
    [6] = 0b111111,
    [7] = 0b1111111,
};

void print_byte(uint8_t byte)
{
    printf("%s%s", bit_rep[byte >> 4], bit_rep[byte & 0x0F]);
}

/*The below function will take the byte of the IP address and add 1 in the network portion. Range of netBitStartPos is [0,7]. A value of 3 for example means that there are 5 network bits i.e. bit 3,4,5,6 and 7. The return value indicates whether overlow has happened or not.*/
bool addOneInNetworkPortionOfOctet(unsigned char& inputByte, short netBitStartPos = 0) {
  printf("\nInput Byte -->");
  print_byte(inputByte);
  printf(", startPos--> %d ",netBitStartPos);
  unsigned char networkPortion = inputByte >> netBitStartPos;
  unsigned char hostPortion = inputByte & hostBits[netBitStartPos];
  printf(" hostPortion--> ");
  print_byte(hostPortion);
  printf(" networkPortion --> ");
  print_byte(networkPortion);

  networkPortion += 1;
  printf(" networkPortionNew --> ");
  print_byte(networkPortion);

  inputByte = (networkPortion << netBitStartPos) + hostPortion;
  printf("\nNew Value --> ");
  print_byte(inputByte);
  unsigned char is_overflow = networkPortion<<netBitStartPos;

  return !(is_overflow);
}

/* Given below is the algorithm
 1. Add one in the network portion of the octet.
 2. If there is no overflow then break.
 3. Otherwise, add one to the previous octet and continue doing so until there is no overflow.
 */
string getNextNetworkAddr(const char* ipAddr, int netMask, sa_family_t ipType) {
  if(ipType == AF_INET6) {
    struct in6_addr sn6;
    inet_pton(AF_INET6,ipAddr,&sn6);
    short currentOctet = netMask/8 + (netMask%8?1:0) - 1;
    printf("\ncurrentOctet[%d] -->",currentOctet);
    print_byte(((unsigned char*)(&sn6.s6_addr))[currentOctet]);
    bool is_overflow = addOneInNetworkPortionOfOctet(((unsigned char*)(&sn6.s6_addr))[currentOctet--], (netMask%8 == 0)?0:(8-(netMask%8))); //Step 1 of algorithm.
    if(is_overflow) {
      while(currentOctet>=0) {
    printf("\ncurrentOctet[%d] -->",currentOctet);
    print_byte(((unsigned char*)(&sn6.s6_addr))[currentOctet]);
        if(!addOneInNetworkPortionOfOctet(((unsigned char*)(&sn6.s6_addr))[currentOctet--]))//Step 2 of algorithm.
          break;
      }
      //Step 3 of algorithm i.e. we continue in the while loop.
    }
    char buff[INET6_ADDRSTRLEN]={0};
    string newIp(inet_ntop(AF_INET6,&sn6,buff,INET6_ADDRSTRLEN));
    printf("\nOld IP-->%s\n",ipAddr);
    printf("New IP-->%s\n",newIp.c_str());
    return newIp;
  }
  else {
    struct in_addr sn;
    inet_pton(AF_INET,ipAddr,&sn);
    short currentOctet = netMask/8 + (netMask%8?1:0) - 1;
    printf("\ncurrentOctet[%d] -->",currentOctet);
    print_byte(((unsigned char*)(&sn.s_addr))[currentOctet]);
    bool is_overflow = addOneInNetworkPortionOfOctet(((unsigned char*)(&sn.s_addr))[currentOctet--],(netMask%8 == 0)?0:(8-(netMask%8))); //Step 1 of algorithm.
    if(is_overflow) {
      while(currentOctet>=0) {
    printf("\ncurrentOctet[%d] -->",currentOctet);
    print_byte(((unsigned char*)(&sn.s_addr))[currentOctet]);
        if(!addOneInNetworkPortionOfOctet(((unsigned char*)(&sn.s_addr))[currentOctet--]))//Step 2 of algorithm.
          break;
      }
      //Step 3 of algorithm i.e. we continue in the while loop.
    }
    char buff[INET_ADDRSTRLEN]={0};
    string newIp(inet_ntop(AF_INET,&sn,buff,INET_ADDRSTRLEN));
    printf("\nOld IP-->%s\n",ipAddr);
    printf("New IP-->%s\n",newIp.c_str());
    return newIp;
  }
}
int main(int argc, char** argv) {

  if(argc!=5) {
    printf("\n%s [IP] [nIterations] [netMask] [IP Type]\ne.g. %s 2405:201:fffb:125::51:a03 3 64 ipv6\n",argv[0],argv[0]);
    printf("e.g. %s 172.16.129.68 3 31 ipv4\n",argv[0]);
    exit(-1);
  }
  int nIterations = std::stoi(argv[2]);

  string nextIp(argv[1]);
  int netMask = stoi(argv[3]);
  sa_family_t ipType;
  if(strcmp(argv[4],"ipv6") == 0)
    ipType = AF_INET6;
  else
    ipType = AF_INET;

  while(nIterations >0){
    --nIterations;
    nextIp = getNextNetworkAddr(nextIp.c_str(),netMask,ipType);
  }
}
