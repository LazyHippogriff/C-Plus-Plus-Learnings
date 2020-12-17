#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

FILE * outputFile = NULL;
long width, depth, rise, run;
float velocity, hydraulicRadius, bedSlope;

//Declaration of functions used in main()
int getDataFromInputFile(char materialName[5][100],float manningRoughnessCoefficients[5]);
float calculateHydraulicRadius(const long area, const long wettedPerimeter);
float calculateVelocity(const float hydraulicRadius, const float bedSlope, const float manningRoughnessCoefficient);
float calculateFlowRate(const float velocity, const long width, const long depth);
void printProgramIntroduction();
void printChannel(const long width, const long depth);
int manipulateWidth(int width, int depth);
int manipulateDepth(int width, int depth);
int displayAndSaveOutput(const float slope, FILE* outputFile);


int main() {
  char materialName[5][100];
  memset(materialName,0,500);
  float manningRoughnessCoefficients[5]={0};
  getDataFromInputFile(materialName,manningRoughnessCoefficients);
  printProgramIntroduction();
  fprintf(stdout,"\nPlease enter the following channel properties:\n\tWidth (m) -> ");
  scanf("%ld", &width);
  fprintf(stdout,"\tDepth (m) -> ");
  scanf("%ld", &depth);
  fprintf(stdout,"Please enter the slope parameters:\n\tRise (m) -> ");
        scanf("%ld",&rise);
        fprintf(stdout,"\tRun (m) -> ");
        scanf("%ld",&run);
        printChannel(width,depth);
        return 0;
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

int getDataFromInputFile(char materialName[5][100],float manningRoughnessCoefficients[5]) {
        FILE* inputFile = fopen("inputFile.txt","r");
        if(!inputFile) {
                fprintf(stdout,"\nError in opening file(inputFile.txt). Please make sure that this file is available\n");
                return -1;
        }
    else {
                        int index = 0;
                        while (fscanf(inputFile, "%20[^,],%f\n", materialName[index],&manningRoughnessCoefficients[index]) == 2) {
                                fprintf(stdout,"\nMaterial-->%s,ManningRoughnessCoefficient-->%f\n",materialName[index],manningRoughnessCoefficients[index]);
                                ++index;
                                if(index == 5) {
                                        break;
                                }
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

void printChannel(const long width, const long depth) {
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



int displayAndSaveOutput(const float slope, FILE* outputFile) {
        fprintf(stdout,"\n%-25s*\t\t\tSlope\n"," ");
        fprintf(stdout,"\n%-15s%-10s*  %-7f  *  %-7f  *  %-7f  *  %-7f  *  %-7f  *  \n","Material"," ",slope,slope+0.05,slope+0.1,slope+0.15,slope+0.2);
        for(int i=0;i<100;++i){
                fprintf(stdout,"-");
        }
        fprintf(stdout,"\n");
        for(int index=0;index<5;index++) {
                fprintf(stdout,"%-25s*  %-7f  |  %-7f  |  %-7f  |  %-7f  |  %-7f  |\n",materialName[index],mValue[index],mValue[index],mValue[index],mValue[index],mValue[index]);
        }

        fprintf(out_file,"\n%-25s*\t\t\tSlope\n"," ");
        fprintf(out_file,"\n%-15s%-10s*  %-7f  *  %-7f  *  %-7f  *  %-7f  *  %-7f  *  \n","Material"," ",0.01,0.06,0.11,0.16,0.21);
        for(int i=0;i<100;++i){
                fprintf(out_file,"-");
        }
        fprintf(out_file,"\n");
        for(int index=0;index<5;index++) {
                fprintf(out_file,"%-25s*  %-7f  |  %-7f  |  %-7f  |  %-7f  |  %-7f  |\n",materialName[index],mValue[index],mValue[index],mValue[index],mValue[index],mValue[index]);
        }

        fclose(out_file);
}
