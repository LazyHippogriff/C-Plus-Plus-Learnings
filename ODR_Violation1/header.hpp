#ifndef __HEADER_HPP__
#define __HEADER_HPP__
#include <string>
using namespace std;

namespace test {
   string var = "Hello World"; //This results in ODR violation.
  //const string var = "Hello World"; // This compiles fine.
  // constexpr string var = "Hello World"; // This gives compilation errors.
};


#endif 
