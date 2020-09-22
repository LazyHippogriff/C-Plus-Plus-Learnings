#include "tokenBucket.hpp"
#include <thread>
#include <vector>
#include <atomic>
#include <unistd.h>
#include <iostream>

std::atomic<uint64_t> global_counter;

int main(int argc, char** argv) {
  if(argc!=3) {
    std::cout<<argv[0]<<" [tokenFillRatePerSecond] [numberOfThreads]\nPlease give the tokenFillRatePerSecond and the number of threads in the input argument to the program.\nE.g. "<<argv[0]<<" 5 5\n";
    exit(1);
  }

  int l_tokenFillRate = std::stoi(argv[1]);
  int l_nThreads = std::stoi(argv[2]);

  tokenBucket aBucket(l_tokenFillRate );

  std::vector<std::thread> threadList;
  for(int i =0;i<10;++i) {

    threadList.push_back(std::thread([&aBucket](){
        while(true) {
        if(aBucket.areTokensAvailable()) {
        ++global_counter;
        }
        }
        }));
  }

  while(true) {
    static uint64_t oldVal = 0;
    std::cout<<"\nTPS-->"<<global_counter-oldVal<<"\t\toldVal("<<oldVal<<")\t\tglobal_counter("<<global_counter.load()<<")";
    oldVal = global_counter.load();
    sleep(1);
  }
}
