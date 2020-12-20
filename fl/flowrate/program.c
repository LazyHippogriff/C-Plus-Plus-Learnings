#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


FILE * outputFile = NULL;
long width, depth, rise, run, area, wettedPerimeter;
float hydraulicRadius, bedSlope;
char materialName[5][100];
float manningRoughnessCoefficients[5];

void printAllConstants() {
  fprintf(stdout,"\nwidth-->%ld, depth-->%ld, rise--%ld, run-->%ld, area-->%ld, wettedPerimeter-->%ld\n",width, depth, rise, run, area, wettedPerimeter);
  fprintf(stdout,"hydraulicRadius-->%f, bedSlope-->%f\n",hydraulicRadius, bedSlope);
}

//Declaration of functions used in main()
int getDataFromInputFile();
void getDataFromUser();
void calculateChannelConstants();
void calculateArea();
void calculateWettedPerimeter();
void calculateBedSlope();
void calculateHydraulicRadius();
float calculateVelocity(const float slope, const float manningRoughnessCoefficient);
float calculateFlowRate(const float velocity);
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

  calculateChannelConstants();

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

}

void calculateChannelConstants() {
  calculateArea();
  calculateWettedPerimeter();
  calculateBedSlope();
  calculateHydraulicRadius();
}

void calculateArea() {
  area = width * depth;
}

void calculateWettedPerimeter() {
  wettedPerimeter = width + (2 * depth);
}

void calculateBedSlope() {
  bedSlope = rise/(float)run;
}

void calculateHydraulicRadius() {
  hydraulicRadius = area/(float)wettedPerimeter;
}

float calculateVelocity(const float slope, const float manningRoughnessCoefficient) {
  const float velocity = ((pow(hydraulicRadius,0.67) * pow(slope,0.5) ) / manningRoughnessCoefficient);
  return velocity;
}


float calculateFlowRate(const float velocity) {
        const float flowRate = velocity * area;
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

  float slopeRange[5];
  for(int i =0;i<5;++i) {
    slopeRange[i] = bedSlope + (0.05*i);
  }

  fprintf(stdout,"\n%-25s*\t\t\tSlope\n\n%-15s%-10s"," ", "Material"," ");
  fprintf(outputFile,"\n%-25s*\t\t\tSlope\n\n%-15s%-10s"," ", "Material"," ");

  for(int i =0 ;i<5;++i) {
    fprintf(stdout, "*   %-8.2f  ",slopeRange[i]);
    fprintf(outputFile, "*   %-8.2f  ",slopeRange[i]);
  }

  fprintf(stdout,"*  \n");
  fprintf(outputFile,"*  \n");

  for(int i=0;i<100;++i){
    fprintf(stdout,"-");
    fprintf(outputFile,"-");
  }

  fprintf(stdout,"\n");
  fprintf(outputFile,"\n");

  for(int rowNumber=0;rowNumber<5;rowNumber++) {
    fprintf(stdout,"%-25s*",materialName[rowNumber]);
    fprintf(outputFile,"%-25s*",materialName[rowNumber]);
    for(int colNumber = 0;colNumber<5;colNumber++) {
      const float velocity = calculateVelocity(slopeRange[colNumber],manningRoughnessCoefficients[rowNumber]);
      const float flowRate = calculateFlowRate(velocity);
      fprintf(stdout,"   %-8.2f  |",flowRate);
      fprintf(outputFile,"   %-8.2f  |",flowRate);
    }
    fprintf(stdout,"\n");
    fprintf(outputFile,"\n");
  }

  fclose(outputFile);
  return 0;
}


