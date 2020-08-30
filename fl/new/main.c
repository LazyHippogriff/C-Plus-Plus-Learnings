#include "header.h"

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
