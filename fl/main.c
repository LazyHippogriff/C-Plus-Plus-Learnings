#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#define SUCCESS 0
#define FAILURE -1
#define NUMBER_OF_INPUT_IN_FILE 5
#define STEEL_SECTION_NAME_MAX_LENGTH 100
#define MAX_RESULT_SIZE 10000
#define PI 3.14159

char output[MAX_RESULT_SIZE];
char steelSectionName[STEEL_SECTION_NAME_MAX_LENGTH];
int steelSectionNameSize = 0;
uintmax_t Zx;
uintmax_t Iy;
uintmax_t J;
uintmax_t Iw;


void test();
void printValues();
int initialiseValuesFromFile(FILE* inputFile);
int createOutputString(double ratios[10][11]);
int createOutputFile(const char* fileName);
void calculateArray();


int main (int argc, char** argv) {
  if(argc != 2) {
    printf("\nIncorrect number of input arguments\n%s [Input File Name]\ne.g. %s inputParameters.txt\n",argv[0],argv[0]);
    return FAILURE;
  }

  FILE *inputFile = NULL;

  inputFile = fopen (argv[1], "r");

  if(inputFile == NULL)  {
    printf("\nUnable to open the file given in the input.(%s)\n",argv[1]);
    fclose(inputFile);
    return FAILURE;
  }

  if(initialiseValuesFromFile(inputFile) != SUCCESS) {
    fclose(inputFile);
    return FAILURE;
  }

  calculateArray();

  printf("%s",output);

  char outputFileName[STEEL_SECTION_NAME_MAX_LENGTH+5];
  memset(outputFileName,0,STEEL_SECTION_NAME_MAX_LENGTH+5);

  sprintf(outputFileName,"%s.dat",steelSectionName);

  if(createOutputFile(outputFileName) != SUCCESS) {
    printf("\nError while creating output file(%s)\n",outputFileName);
    fclose(inputFile);
    return FAILURE;
  }


  fclose(inputFile);
  return SUCCESS;
}

double getMax(double input1, double input2) {
  return input1>input2?input1:input2;
}

void calculateArray() {
  uintmax_t Ms = 300 * Zx;
  uintmax_t EIy = 200000 * Iy;
  uintmax_t GJ = 80000 * J;
  uintmax_t EIw = 200000 * Iw;

  double values[10][11];
  for(int i = 0;i<10;++i) {
    for(int j=0;j<11;++j) {
      values[i][j] = -1.0;
    }
  }

  for(int udl = 1; udl<=10; ++udl) {
    double Mb = 0;
    double Mo = 0;
    double MoArg1 = pow(PI,2) * EIy;
    double MoArg2 = pow(PI,2) * EIw;
    double alpha = 0;
    double Moment = 0;
    double MomentByMs = 0;
    double MomentByMb = 0;
    for(int le = 0;le<=10; ++le) {
      if(le == 0) {
        int l_le = 5;
        Moment = (udl * pow(l_le,2))/8;
        MomentByMs = (Moment * pow(10,6))/(double)Ms;
        values[udl-1][le] = MomentByMs;
      }
      else {
        Moment = (udl * pow(le,2))/8;
        Mo = pow(((MoArg1)/(pow(le * 1000,2)))*(GJ + ((MoArg2)/(pow(le * 1000,2)))),0.5);
        alpha = 0.6 * ((pow((pow((double)Ms/Mo,2) + 3), 0.5)) - ((double)Ms/Mo));
        Mb = alpha * Ms;
        MomentByMs = (Moment * pow(10,6))/(double)Ms;
        MomentByMb = (Moment * pow(10,6))/Mb;
        values[udl-1][le] = getMax(MomentByMs,MomentByMb);
      }
    }
  }

  createOutputString(values);
}


int createOutputFile(const char* fileName) {
  FILE *outputFile = NULL;

  outputFile = fopen(fileName, "w");
  if(outputFile == NULL) {
    printf("\nUnable to open the output file(%s) for writing the result\n", fileName);
    return FAILURE;
  }

  fputs(output, outputFile);

  /*Closing the Output file after successful writing*/
  fclose(outputFile);

  return SUCCESS;
}

void printValues() {
  printf("\nSteel Section Name --> %s\n",steelSectionName);
  printf("Zx --> %lu\n",Zx);
  printf("Iy --> %lu\n",Iy);
  printf("J --> %lu\n",J);
  printf("Iw --> %lu\n",Iw);
}

void test() {
double random[10][11];
for(int row = 0; row < 10; ++row) {
for(int column = 0; column <= 10; ++column) {
random[row][column] = (double)(row+1)/(double)(column+1);
}
}

createOutputString(random);
}


int createOutputString(double ratios[10][11]) {
  for(int row = 1; row <= 10; ++row) {
    if(row!=10) {
      //sprintf(output+strlen(output),"\t%d *\t  ",row);
      sprintf(output+strlen(output),"    %d *      ",row);
    }
    else {
      //sprintf(output+strlen(output)," %d *\t  ",row);
      sprintf(output+strlen(output),"   %d *      ",row);
    }

    //sprintf(output+strlen(output),"%0.4f\t",ratios[row-1][0]);
    sprintf(output+strlen(output),"%0.4f    ",ratios[row-1][0]);
    for(int column = 1; column <= 10; ++column) {
      //sprintf(output+strlen(output),"%0.4f\t",ratios[row-1][column]);
      sprintf(output+strlen(output),"%0.4f    ",ratios[row-1][column]);
    }
    sprintf(output+strlen(output),"\n");
  }

  return SUCCESS;
}

void addStars(int count) {
  char* temp = (char*)calloc(count+1, sizeof(char));
  for(int i=0;i<count;++i) {
    strcat(temp,"*");
  }
  strcat(output,temp);
  free (temp);
}

int initialiseValuesFromFile(FILE* inputFile) {
  memset(steelSectionName,0,STEEL_SECTION_NAME_MAX_LENGTH);
  for (int wordNumber = 0; wordNumber < NUMBER_OF_INPUT_IN_FILE; ++wordNumber)
  {
    char tempBuffer[STEEL_SECTION_NAME_MAX_LENGTH];
    memset(tempBuffer,0,STEEL_SECTION_NAME_MAX_LENGTH);
    // Read a word from the file
    if (fscanf(inputFile, "%s", tempBuffer) != 1) {
      printf("\nInput file doesn't have all the required parameter values\n");
      return FAILURE;
    }

    // note: "!=1" checks for end-of-file; using feof for that is usually a bug

    char *end;
    switch(wordNumber) {
      case 0: { //Steel Section Name.
                strncpy(steelSectionName,tempBuffer,STEEL_SECTION_NAME_MAX_LENGTH);
                steelSectionNameSize = strlen(steelSectionName);
                break;
              }
      case 1: {
                Zx = strtoumax(tempBuffer,&end,10);
                break;
              }
      case 2: {
                Iy = strtoumax(tempBuffer,&end,10);
                break;
              }
      case 3: {
                J = strtoumax(tempBuffer,&end,10);
                break;
              }
      case 4: {
                Iw = strtoumax(tempBuffer,&end,10);
                break;
              }
      default:;
    }

  }

  strcat(output," ** Beam Capacity Ratio Calculator **\n\nFor a ");
  strcat(output,steelSectionName);
  strcat(output," the ratio of applied UDL to\nmoment capacity is as follows:\n\n UDL");
  for(int i=0;i<=13;++i) {
    //strcat(output,"\t");
    strcat(output,"    ");
  }
  strcat(output,"Effective Length (m)\n");
  //strcat(output,"(kN/m)*\t\t ");
  strcat(output,"(kN/m)*         ");
  for(int i=0;i<=10;++i) {
    if(i!=10) {
    //sprintf(output+strlen(output),"%d\t\t\t ",i);
    sprintf(output+strlen(output),"%d         ",i);
    }
    else {
    sprintf(output+strlen(output),"%d",i);
    }
  }
  strcat(output,"\n");
  addStars(125);
  strcat(output,"\n\n");

  return SUCCESS;
}
