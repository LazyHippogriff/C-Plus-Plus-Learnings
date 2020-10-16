#include <stdio.h>
#include <stdlib.h>

FILE* outputFile = NULL;
int Width = 0;
int Depth = 0;

int manipulateWidth(int width, int depth);
int manipulateDepth(int width, int depth);
int displayAndSaveOutput(int width, int depth);

int main() {
	printf("\nThis program will ask you for two values, manipulate them and then produce a graphic of a hollow rectangle to both the screen and a file.\n");
	printf("The first number will be the width of the rectangle.\n");
	printf("The second number will be the depth of the rectangle.\n");
	printf("The hollow rectangle will consist of single stars. *\n");

	printf("\nPlease enter the rectangle dimensions now. Please enter Width -> ");
	scanf("%d",&Width);
	printf("\nPlease enter Depth(make sure that Depth >= 5) -> ");
	scanf("%d",&Depth);

	if(Depth < 5) {
		while(Depth < 5) {
      printf("\nDepth value entered was < 5. Please enter a value >= 5. Please enter Depth again(make sure that Depth >= 5) -> ");
			scanf("%d",&Depth);
		}
	}

  int oldWidth = Width;
  int oldDepth = Depth;

	Width = manipulateWidth(Width,Depth);
	Depth = manipulateDepth(Width,Depth);

  printf("\nValue of Width given in the input-->%d. Manipulated new value of width -->%d\n",oldWidth,Width);
  printf("\nValue of Depth given in the input-->%d. Manipulated new value of depth -->%d\n",oldDepth,Depth);
  printf("\nNow the program will print a hollow rectangle of *s having Width(%d) and Depth(%d)\n",Width,Depth);

	outputFile = fopen("OutputFile.txt", "w");
	if(outputFile == NULL) {
		printf("\nUnable to open the OutputFile.txt for writing the result. Program will Terminate\n");
		return -1;
	}

	displayAndSaveOutput(Width,Depth);
	fclose(outputFile);
	printf("\nProgram has done the job successfully. Going to exit.\n");
	return 0;
}

int displayAndSaveOutput(int width, int depth) {

	//Printing the first line.
	for(int i = 0;i<width;++i) {
		fprintf(stdout,"*");
		fprintf(outputFile,"*");
	}

	fprintf(stdout,"\n");
	fprintf(outputFile,"\n");

  for(int i = 0;i<depth-2;++i) {
    for(int j = 0;j<width;++j) {

      if(j==0 || j == width - 1) {
        fprintf(stdout,"*");
        fprintf(outputFile,"*");
      }
      else {
        fprintf(stdout," ");
        fprintf(outputFile," ");
      }
    }

  fprintf(stdout,"\n");
  fprintf(outputFile,"\n");
  }

	//Printing the last line.
	for(int i = 0;i<width;++i) {
		fprintf(stdout,"*");
		fprintf(outputFile,"*");
	}

	fprintf(stdout,"\n");
	fprintf(outputFile,"\n");

	return 0;
}


int manipulateWidth(int width, int depth) {
  int newWidth = width;
  if(width > depth) {
    newWidth += 2;
  }
  else {
    newWidth -= 1;
  }

  return newWidth;
}

int manipulateDepth(int width, int depth) {
  int newDepth = depth;
  if((width + depth) % 2 != 0) {
    newDepth += 3;
  }
  else {
    newDepth += 2;
  }

  return newDepth;
}
