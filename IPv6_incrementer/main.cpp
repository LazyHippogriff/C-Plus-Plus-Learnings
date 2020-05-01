#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <string>

using namespace std;

char zeroByte(unsigned char& inputByte, int maxBitPos) {
  printf("\ninputByte-->%x, To zero upto-->%d\n",inputByte,maxBitPos);
  inputByte &= (255<< maxBitPos);
  printf("\ninputByte After zeroing-->%x\n",inputByte);
  return inputByte;
}

int getZeroPosition(unsigned char inputByte) {
    //printf("\ninputByte-->%x\n",inputByte);
      for(int i=0;i<8;++i) {
            if(!(inputByte>>i & 1))
                    return i;
              }
        return 8;
}

char addOneInByte(unsigned char& inputByte, int posOfFirstZeroBit) {
  printf("\nTo Add 1 in byte-->%x\n",inputByte);
  if(posOfFirstZeroBit == 8)  {
    inputByte = 0;
    return inputByte;
  }
  //printf("\ninputByte-->%x, To complement upto-->%d\n",inputByte,posOfFirstZeroBit);
  inputByte ^= 1<<posOfFirstZeroBit; //To complement the bit at posOfFirstZeroBit
  //printf("\ninputByte after xor -->%x\n",inputByte);
  inputByte &= 255<<posOfFirstZeroBit; //To zero out the bits from 0 to posOfFirstZeroBit - 1
  //printf("\ninputByte after and -->%x\n",inputByte);
  return inputByte;
}


string getNextUeIp(const char* ipv6, int netMask) {
  //printf("\nInside getNextUeIp\n");
  struct in6_addr sn6;
  inet_pton(AF_INET6,ipv6,&sn6);
  int hostMask = 128-netMask;
  printf("\nhostMask-->%d\n",hostMask);
  int minOctet = 16 - (hostMask%8?((hostMask/8) + 1) : (hostMask/8));

  int bitPos = 0;
  for(int octet =15;octet>=minOctet;--octet) {
    int zeroPos = getZeroPosition(sn6.s6_addr[octet]);

    //Case 1: Last Host Octet
    if(octet == minOctet) {
      //Case 1a: first zero bit is found *before* hostMask ends.
      if(zeroPos < hostMask%8){
        printf("\nCase 1a\n");
        addOneInByte(sn6.s6_addr[octet],zeroPos);
      }

      //Case 1b: No zero bit is found or first zero bit is found *after* hostMask ends.
      else{
        printf("\nCase 1b\n");
        zeroByte(sn6.s6_addr[octet],hostMask%8);
      }
    }
    //Case 2
    else {
      //Case 2a: All the bits in the octet are 1.
      if(zeroPos == 8){
        printf("\nCase 2a\n");
        sn6.s6_addr[octet] = 0;
      }
      else {
        //Case 2b: A zero bit is found.
        printf("\nCase 2b\n");
        addOneInByte(sn6.s6_addr[octet],zeroPos);
        break;
      }
    }
  }
  char buff[INET6_ADDRSTRLEN]={0};
  string newIp(inet_ntop(AF_INET6,&sn6,buff,INET6_ADDRSTRLEN));
  printf("\nOld IP-->%s\n",ipv6);
  printf("New IP-->%s\n",newIp.c_str());
  return newIp;
}

int main(int argc, char** argv) {

  if(argc!=4) {
    printf("\n%s [IP] [nIterations] [netMask]\ne.g. %s 2405:201:fffb:125::51:a03 3 64\n",argv[0],argv[0]);
    exit(-1);
  }
  printf("\nIP given: %s\n",argv[1]);
  int nIterations = std::stoi(argv[2]);

  string nextIp(argv[1]);
  int netMask = stoi(argv[3]);

  while(nIterations >0){
    --nIterations;
    nextIp = getNextUeIp(nextIp.c_str(),netMask);
  }
}
