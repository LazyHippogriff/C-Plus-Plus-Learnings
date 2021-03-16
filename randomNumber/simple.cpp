#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>


using namespace std::chrono_literals;
using namespace std;

int main() {
  srand((unsigned) time(0));
  int randomNumber;
  
while(true) {
    randomNumber = (rand() % 10) + 1;
    cout << randomNumber << endl;
    std::this_thread::sleep_for(1s);
  }
}
