#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


FILE * outputFile = NULL;
long width, depth, rise, run;
float velocity, hydraulicRadius, bedSlope;
char materialName[5][100];
float manningRoughnessCoefficients[5];


//Declaration of functions used in main()
int getDataFromInputFile();
void getDataFromUser();
float calculateHydraulicRadius(const long area, const long wettedPerimeter);
float calculateVelocity(const float hydraulicRadius, const float bedSlope, const float manningRoughnessCoefficient);
float calculateFlowRate(const float velocity, const long width, const long depth);
void printProgramIntroduction();
void printChannel();
int manipulateWidth(int width, int depth);
int manipulateDepth(int width, int depth);
int displayAndSaveOutput();


int main() {

  printProgramIntroduction();

  if(getDataFromInputFile() != 0) {
    fprintf(stderr,"\nError while getting data from the input file. Terminating the application\n");
    return -1;
  }

  getDataFromUser();

  printChannel();

  if(displayAndSaveOutput() != 0) {
    fprintf(stderr,"\nError while displaying/saving the output. Terminating the application\n");
    return -1;
  }

  return 0;
}

void getDataFromUser() {
  fprintf(stdout,"\nPlease enter the following channel properties:\n\tWidth (m) -> ");
  scanf("%ld", &width);
  while(width < 0) {
    fprintf(stdout,"\nPlease enter a positive value of width\n");
    fprintf(stdout,"\nPlease enter width of the channel(m) -> ");
    scanf("%ld", &width);
  }

  fprintf(stdout,"\tDepth (m) -> ");
  scanf("%ld", &depth);
  while(depth < 0) {
    fprintf(stdout,"\nPlease enter a positive value of depth\n");
    fprintf(stdout,"\nPlease enter depth of the channel(m) -> ");
    scanf("%ld", &depth);
  }

  fprintf(stdout,"Please enter the slope parameters:\n\tRise (m) -> ");
        scanf("%ld",&rise);
  while(rise < 0) {
    fprintf(stdout,"\nPlease enter a positive value of rise\n");
    fprintf(stdout,"\nPlease enter rise of the channel(m) -> ");
    scanf("%ld", &rise);
  }

        fprintf(stdout,"\tRun (m) -> ");
  scanf("%ld",&run);
  while(run < 0) {
    fprintf(stdout,"\nPlease enter a positive value of run\n");
    fprintf(stdout,"\nPlease enter run of the channel(m) -> ");
    scanf("%ld", &run);
  }

  bedSlope = rise/run;
}


float calculateHydraulicRadius(const long area, const long wettedPerimeter) {
  return area/wettedPerimeter;
}

float calculateVelocity(const float hydraulicRadius, const float bedSlope, const float manningRoughnessCoefficient) {
  const float velocity = ((pow(hydraulicRadius,0.67) * pow(bedSlope,0.5) ) / manningRoughnessCoefficient);
  return velocity;
}


float calculateFlowRate(const float velocity, const long width, const long depth) {
        const long crossSectionalArea = width * depth;
        const float flowRate = velocity * crossSectionalArea;
        return flowRate;
}

int getDataFromInputFile() {
        FILE* inputFile = fopen("inputFile.txt","r");
        if(!inputFile) {
                fprintf(stdout,"\nError in opening file(inputFile.txt). Please make sure that this file is available\n");
                return -1;
        }
        else {
                int index = 0;
                while (fscanf(inputFile, "%20[^,],%f\n", materialName[index],&manningRoughnessCoefficients[index]) == 2) {
                        //fprintf(stdout,"\nMaterial-->%s,ManningRoughnessCoefficient-->%f\n",materialName[index],manningRoughnessCoefficients[index]);
                        ++index;
                        if(index == 5) {
                                break;
                        }
                }
                if(index != 5) {
                        fprintf(stdout,"\ninputFile.txt doesn't contain 5 lines. There should be five entries, one per line, in this file. Each line will consist of a name for the material and the associated roughness.\n");
                        fclose(inputFile);
                        return -1;
                }
                fclose(inputFile);
        }
        return 0;
}

void printProgramIntroduction() {
  for(int i=0;i<21;++i) {
    fprintf(stdout,"*");
  }
  fprintf(stdout,"\n*");
  for(int i=0;i<19;++i) {
    fprintf(stdout," ");
  }
  fprintf(stdout,"*\n*");
  fprintf(stdout," Open Channel Flow *\n");
  fprintf(stdout,"*");
  for(int i=0;i<19;++i) {
    fprintf(stdout," ");
  }
  fprintf(stdout,"*\n");
  for(int i=0;i<21;++i) {
    fprintf(stdout,"*");
  }
  fprintf(stdout,"\n");
  fprintf(stdout,"- Using Manning's equation calculate the flow rate of an open channel for a range of materials & bed slopes\n");
}

void printChannel() {
  fprintf(stdout,"\nThe channel\n***********\n");
  for(int row = 0;row < depth + 3; ++row) {
    fprintf(stdout,"\n\t*");

    char ch;
    if(row == 1) {
      ch = '-';
    } else if(row == depth + 2) {
      ch = '*';
    } else {
      ch = ' ';
    }

    for(int column = 0; column < width; ++column) {
      fprintf(stdout,"%c",ch);
    }

    fprintf(stdout,"*");
    if(row == depth/2) {
      fprintf(stdout,"  %ld",depth);
    }

  }

  fprintf(stdout,"\n\t");
  for(int column = 0;column<width/2;++column) {
    fprintf(stdout," ");
  }
  fprintf(stdout,"%ld\n",width);
}



int displayAndSaveOutput() {
        outputFile = fopen("theAdventuresOfZorro.txt","w");
        if(!outputFile) {
                fprintf(stdout,"\nError in opening output file(theAdventuresOfZorro.txt).\n");
                return -1;
        }

        fprintf(stdout,"\n%-25s*\t\t\tSlope\n"," ");
        fprintf(stdout,"\n%-15s%-10s*  %-7f  *  %-7f  *  %-7f  *  %-7f  *  %-7f  *  \n","Material"," ",bedSlope,bedSlope+0.05,bedSlope+0.1,bedSlope+0.15,bedSlope+0.2);
        for(int i=0;i<100;++i){
                fprintf(stdout,"-");
        }
        fprintf(stdout,"\n");
        for(int index=0;index<5;index++) {
                fprintf(stdout,"%-25s*  %-7f  |  %-7f  |  %-7f  |  %-7f  |  %-7f  |\n",materialName[index],manningRoughnessCoefficients[index],manningRoughnessCoefficients[index],manningRoughnessCoefficients[index],manningRoughnessCoefficients[index],manningRoughnessCoefficients[index]);
        }

        fprintf(outputFile,"\n%-25s*\t\t\tSlope\n"," ");
        fprintf(outputFile,"\n%-15s%-10s*  %-7f  *  %-7f  *  %-7f  *  %-7f  *  %-7f  *  \n","Material"," ",0.01,0.06,0.11,0.16,0.21);
        for(int i=0;i<100;++i){
                fprintf(outputFile,"-");
        }
        fprintf(outputFile,"\n");
        for(int index=0;index<5;index++) {
                fprintf(outputFile,"%-25s*  %-7f  |  %-7f  |  %-7f  |  %-7f  |  %-7f  |\n",materialName[index],manningRoughnessCoefficients[index],manningRoughnessCoefficients[index],manningRoughnessCoefficients[index],manningRoughnessCoefficients[index],manningRoughnessCoefficients[index]);
        }

        fclose(outputFile);
        return 0;
}

