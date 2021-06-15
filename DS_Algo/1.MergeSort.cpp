#include <iostream>

void printArr(int* arr, const int arrSize, const int recursionDepth = 0) {
  std::cout<<"\n";
#ifdef _debug
  for(int i = 0;i<recursionDepth;++i) {
    std::cout<<"\t";
  }
#endif

  std::cout<<"Going to print the Array\n";

#ifdef _debug
  for(int i = 0;i<recursionDepth;++i) {
    std::cout<<"\t";
  }
#endif

  for(int i =0;i<arrSize;++i) {
    std::cout<<arr[i]<<"  ";
  }
  std::cout<<"\n";
}

void merge(int* arr, const int leftIndex, const int midPoint, const int rightIndex, const int recursionDepth = 0) {
  std::cout<<"\n";
#ifdef _debug
  for(int i = 0;i<recursionDepth;++i) {
    std::cout<<"\t";
  }
  std::cout<<"In merge(). leftIndex["<<leftIndex<<"], midPoint["<<midPoint<<"], rightIndex["<<rightIndex<<"]\n";
#endif

  const int leftSize = (midPoint - leftIndex) + 1;
  const int rightSize = rightIndex - midPoint;

  int* leftArr = new int[leftSize];
  int* rightArr = new int[rightSize];

  //copying the left Array.
  for(int i =0;i<leftSize; ++i) {
    leftArr[i] = arr[leftIndex + i];
  }

  //copying the right Array.
  for(int i = 0;i<rightSize; ++i) {
    rightArr[i] = arr[midPoint + i + 1];
  }

  int i=0,j=0;

  while(i<leftSize && j<rightSize) {
    if(leftArr[i] < rightArr[j]) {
      arr[leftIndex + i + j] = leftArr[i];
      ++i;
    }
    else {
      arr[leftIndex + i + j] = rightArr[j];
      ++j;
    }
  }

  //transferring remaining elements.
  while(i<leftSize) {
      arr[leftIndex + i + j] = leftArr[i];
      ++i;
  }

  while(j<rightSize) {
      arr[leftIndex + i + j] = rightArr[j];
      ++j;
  }

#ifdef _debug
  printArr(arr+leftIndex,rightIndex-leftIndex +1,recursionDepth);
#endif

  delete[] leftArr;
  delete[] rightArr;
}


void recursiveMergeSort(int* arr, const int leftIndex, const int rightIndex, const int recursionDepth = 0) {
#ifdef _debug
  for(int i = 0;i<recursionDepth;++i) {
    std::cout<<"\t";
  }
#endif

  if(leftIndex >= rightIndex) {
    return;
  }

  const int midPoint = (leftIndex + rightIndex) / 2;

#ifdef _debug
  std::cout<<"In recursiveMergeSort(). leftIndex["<<leftIndex<<"], midPoint["<<midPoint<<"], rightIndex["<<rightIndex<<"]\n";

  std::cout<<"\n";
  for(int i = 0;i<recursionDepth;++i) {
    std::cout<<"\t";
  }
  std::cout<<"Going to recurse on left Arr. leftIndex["<<leftIndex<<"], rightIndex["<<midPoint<<"]\n";
#endif

  recursiveMergeSort(arr, leftIndex, midPoint, recursionDepth+1);


#ifdef _debug
  std::cout<<"\n";
  for(int i = 0;i<recursionDepth;++i) {
    std::cout<<"\t";
  }
  std::cout<<"Going to recurse on right Arr. leftIndex["<<midPoint+1<<"], rightIndex["<<rightIndex<<"]\n";
#endif

  recursiveMergeSort(arr, midPoint + 1, rightIndex, recursionDepth+1);

#ifdef _debug
  std::cout<<"\n";
  for(int i = 0;i<recursionDepth;++i) {
    std::cout<<"\t";
  }

  std::cout<<"Now going to merge leftArr and rightArr. leftIndex["<<leftIndex<<"], midPoint["<<midPoint<<"], rightIndex["<<rightIndex<<"]\n";
#endif

  merge(arr, leftIndex, midPoint, rightIndex, recursionDepth);
}

int main() {
  std::cout<<"Enter the size of Array: ";
  int arrSize;
  std::cin>>arrSize;

  std::cout<<"Now enter "<<arrSize<<" array elements\n";

  int* arr = new int[arrSize];

  for(auto i=0;i<arrSize;++i) {
    std::cin>>arr[i];
  }

  recursiveMergeSort(arr,0,arrSize-1);

  printArr(arr,arrSize);
}
@vishal1.sharma:
