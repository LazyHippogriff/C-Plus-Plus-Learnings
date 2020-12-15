#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


long width, depth, rise, run;
float velocity, hydraulicRadius, bedSlope;
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

int readInputFile(char materialName[5][100],float manningRoughnessCoefficient[5])
{
  FILE* inputFile = fopen("inputFile.txt","r");
  if(!inputFile) {
    fprintf(stdout,"\nError in opening file(inputFile.txt). Please make sure that this file is available\n");
    return -1;
  }

  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  int materialCount = 0;
  while ((read = getline(&line, &len, inputFile)) != -1) {
    int countNameLen = 0;
    for(int i=0;i<read;++i) {
      if(line[i] != ',') {
        ++countNameLen;
      }
      else {
        break;
      }
    }
    strncpy(materialName[materialCount],line,countNameLen);
    char* pend;
    manningRoughnessCoefficient[materialCount] = strtof(line+countNameLen+1, &pend);
    fprintf(stdout,"\nMaterial-->%s,countNameLen-->%d,ManningRoughnessCoefficient-->%f\n",materialName[materialCount],countNameLen,manningRoughnessCoefficient[materialCount]);
    if (line) {
      free(line);
      line = NULL;
      len = 0;
    }
  }

    if (line) {
      free(line);
    }

  fclose(inputFile);
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


FILE * outputFile = NULL;

int manipulateWidth(int width, int depth);
int manipulateDepth(int width, int depth);
int displayAndSaveOutput(int width, int depth);

int main() {
  char materialName[5][100];
  memset(materialName,0,500);
  float manningRoughnessCoefficient[5]={0};
  readInputFile(materialName,manningRoughnessCoefficient);
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
