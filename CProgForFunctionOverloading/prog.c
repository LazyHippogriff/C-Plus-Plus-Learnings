#include <stdio.h>

typedef enum {
  struct1e,
  struct2e
}typeOfStruct;

typedef struct {
  void (*printMembers)(typeOfStruct type);
} struct1;

typedef struct {
  void (*printMembers)(typeOfStruct type);
} struct2;


void printMembers(typeOfStruct type) {
  if(type==struct1e)
    printf("\nInside printMembers of struct1\n");
  if(type==struct2e)
    printf("\nInside printMembers of struct2\n");
}

int main() {

  struct1 obj1;
  struct2 obj2;
  obj1.printMembers = printMembers;
  obj2.printMembers = printMembers;
  obj1.printMembers(struct1e);
  obj2.printMembers(struct2e);
}
