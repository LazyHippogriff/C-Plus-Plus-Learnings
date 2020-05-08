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

/* In the given octet, the below function finds the bit position of the first
 * 0. Position starts from 0. If 0 is not found, it returns -1.
 */
int getZeroPosition(unsigned char inputByte) {
    //printf("\ninputByte-->%x\n",inputByte);
      for(int i=0;i<8;++i) {
            if(!(inputByte>>i & 1))
                    return i;
              }
        return -1;
}

/* The below function will zero out the given number of Bits in the given byte.
 */
unsigned char zeroByte(unsigned char& inputByte, int numOfBits) {
  //printf("\ninputByte-->%x, To zero upto-->%d\n",inputByte,numOfBits);
  inputByte &= (255<< numOfBits);
  //printf("\ninputByte After zeroing-->%x\n",inputByte);
  return inputByte;
}

unsigned char addOneInHostPortionOfByte(unsigned char& inputByte, int posOfFirstZeroBit, int netBitStartPos = -1) {
  if(netBitStartPos !=-1) {
    if(posOfFirstZeroBit>=netBitStartPos)
      inputByte &= 255<<netBitStartPos; //To zero out the bits from 0 to netBitStartPos - 1
    else {
      //printf("\ninputByte-->%x, To complement upto-->%d\n",inputByte,posOfFirstZeroBit);
      inputByte ^= 1<<posOfFirstZeroBit; //To complement the bit at posOfFirstZeroBit
      //printf("\ninputByte after xor -->%x\n",inputByte);
      inputByte &= 255<<posOfFirstZeroBit; //To zero out the bits from 0 to posOfFirstZeroBit - 1
      //printf("\ninputByte after and -->%x\n",inputByte);
    }
    return inputByte;
  }
  else {
    //printf("\ninputByte-->%x, To complement upto-->%d\n",inputByte,posOfFirstZeroBit);
    inputByte ^= 1<<posOfFirstZeroBit; //To complement the bit at posOfFirstZeroBit
    //printf("\ninputByte after xor -->%x\n",inputByte);
    inputByte &= 255<<posOfFirstZeroBit; //To zero out the bits from 0 to posOfFirstZeroBit - 1
    //printf("\ninputByte after and -->%x\n",inputByte);
    return inputByte;
  }
}

/*The below function will take the byte of the IP address and add 1 in the host portion. -1 signifies that all bits in the given byte are of the host portion. The return value of 1 signifies that we need to continue iterating over the octets.*/
int modifyByte(unsigned char& inputByte, int zeroPos, int netBitStartPos = -1) {
  if(netBitStartPos !=-1) {
    if(zeroPos>=netBitStartPos)
      zeroByte(inputByte,netBitStartPos);
    else
      addOneInHostPortionOfByte(inputByte,zeroPos,netBitStartPos);
    return 1;
  }
  else
      addOneInHostPortionOfByte(inputByte,zeroPos);

  if (zeroPos == -1)
    return 0;
  else
    return 1;
}


string getNextUeIp(const char* ipAddr, int netMask, sa_family_t ipType) {
  //printf("\nInside getNextUeIp\n");
  if(ipType == AF_INET6) {
    struct in6_addr sn6;
    inet_pton(AF_INET6,ipAddr,&sn6);
    int numOfHostBits = 128-netMask;
    //printf("\nnumOfHostBits-->%d\n",numOfHostBits);
    int minOctet = 16 - (numOfHostBits%8?((numOfHostBits/8) + 1) : (numOfHostBits/8));
    //printf("\nminOctet->%d\n",minOctet);

    int netBitStartPos = (netMask%8==0)?(-1):(8-(netMask%8));
    for(int octet =15;octet>=minOctet;--octet) {
      int zeroPos = getZeroPosition(sn6.s6_addr[octet]);
      if(octet == minOctet)
        modifyByte(sn6.s6_addr[octet],zeroPos,netBitStartPos);
      else {
        int rc = modifyByte(sn6.s6_addr[octet],zeroPos);
        if (rc == 1)
          break;
      }
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
    int numOfHostBits = 32-netMask;
    //printf("\nnumOfHostBits-->%d\n",numOfHostBits);
    int minOctet = 4 - (numOfHostBits%8?((numOfHostBits/8) + 1) : (numOfHostBits/8));
    //printf("\nminOctet->%d\n",minOctet);

    int netBitStartPos = (netMask%8==0)?(-1):(8-(netMask%8));
    for(int octet =3;octet>=minOctet;--octet) {
      int zeroPos = getZeroPosition(((unsigned char*)(&sn.s_addr))[octet]);
      if(octet == minOctet)
        modifyByte(((unsigned char*)(&sn.s_addr))[octet],zeroPos,netBitStartPos);
      else {
        int rc = modifyByte(((unsigned char*)(&sn.s_addr))[octet],zeroPos);
        if (rc == 1)
          break;
      }
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
  //printf("\nIP given: %s\n",argv[1]);
  int nIterations = std::stoi(argv[2]);

  string nextIp(argv[1]);
  int netMask = stoi(argv[3]);
  sa_family_t ipType;
  if(strcmp(argv[4],"ipv6") == 0)
    ipType = AF_INET6;
  else
    ipType = AF_INET;

  while(nIterations >0){
//    printf("\n1 loope begins");
    --nIterations;
    nextIp = getNextUeIp(nextIp.c_str(),netMask,ipType);
 //   printf("\n1 loope ends");
  }
}

