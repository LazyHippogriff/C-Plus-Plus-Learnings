/* The objective of the program is to calculate the flow capacity of a simple
 * rectangular channel using a combination of slopes and materials of
 * construction. The flow will be calculated using the Manning Equation. The
 * required inputs will be taken from an input file as well as from the user.
 * The program will first open an input file named "inputFile.csv" containing
 * 5 lines. Each line will contain comma separated values. The first value will
 * give the material name and the second value will be the corresponding manning roughness value.
 * Given below is how the contents of a sample input file will look.

 Asphalt,0.016
 Cast Iron,0.012
 Concrete,0.011
 Gravel,0.023
 Natural channels,0.060

 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


FILE * outputFile = NULL;
long width, depth, rise, run, area, wettedPerimeter;
float hydraulicRadius, bedSlope;
char materialName[5][100];
float manningRoughnessCoefficients[5];

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

/* This function will get the channel dimension values from the user. */
void getDataFromUser() {
        fprintf(stdout,"\nPlease enter the following channel properties:\n\tWidth (in metre) -> ");
        while(scanf("%ld", &width) != 1 || width < 0) {
                fprintf(stdout,"\nError!!You entered an invalid value. Please enter a positive value as width of the channel.\n");
                fprintf(stdout,"\tWidth (in metre) -> ");
                scanf("%*s");
        }

        fprintf(stdout,"\tDepth (in metre) -> ");
        while(scanf("%ld", &depth) != 1 || depth < 0) {
                fprintf(stdout,"\nError!! You entered an invalid value. Please enter a positive value as depth of the channel.\n");
                fprintf(stdout,"\tDepth (in metre) -> ");
                scanf("%*s");
        }

        fprintf(stdout,"Please enter the slope parameters:\n\tRise (in metre) -> ");
        while(scanf("%ld",&rise) != 1 || rise < 0) {
                fprintf(stdout,"\nError!! You entered an invalid value. Please enter a positive value as rise of the channel.\n");
                fprintf(stdout,"\tRise (in metre) -> ");
                scanf("%*s");
        }

        fprintf(stdout,"\tRun (in metre) -> ");
        while(scanf("%ld",&run) != 1 || run < 0) {
                fprintf(stdout,"\nError!! You entered an invalid value. Please enter a positive value as run of the channel.\n");
                fprintf(stdout,"\tRun (in metre) -> ");
                scanf("%*s");
        }
}

/* This function will calculate the Area, Wetted Perimeter, Slope and Hydraulic Radius of the channel. */
void calculateChannelConstants() {
        calculateArea();
        calculateWettedPerimeter();
        calculateBedSlope();
        calculateHydraulicRadius();
}

/* This function will calcluate the area of the channel. */
void calculateArea() {
        area = width * depth;
}

/* This function will calculate the Wetter Perimeter of the channel. */
void calculateWettedPerimeter() {
        wettedPerimeter = width + (2 * depth);
}

/* This function will calculate the slope of the channel. */
void calculateBedSlope() {
        bedSlope = rise/(float)run;
}

/* This function will calculate the Hydraulic Radius of the channel. */
void calculateHydraulicRadius() {
        hydraulicRadius = area/(float)wettedPerimeter;
}

/* This function will calculate the velocity of the flow. */
float calculateVelocity(const float slope, const float manningRoughnessCoefficient) {
        const float velocity = ((pow(hydraulicRadius,0.67) * pow(slope,0.5) ) / manningRoughnessCoefficient);
        return velocity;
}


/* This function will calculate the flow rate. */
float calculateFlowRate(const float velocity) {
        const float flowRate = velocity * area;
        return flowRate;
}


/* This function will read the input File. */
int getDataFromInputFile() {
        FILE* inputFile = fopen("inputFile.csv","r");
        if(!inputFile) {
                fprintf(stdout,"\nError in opening file(inputFile.csv). Please make sure that this file is available\n");
                return -1;
        }
        else {
                int index = 0;
                int ret = fscanf(inputFile, "%20[^,],%f\n", materialName[index],&manningRoughnessCoefficients[index]);
                while (ret == 2) {
                        //fprintf(stdout,"\nMaterial-->%s,ManningRoughnessCoefficient-->%f\n",materialName[index],manningRoughnessCoefficients[index]);
                        ++index;
                        if(index == 5) {
                                break;
                        }
                        if(ret != 2) {
                                fprintf(stdout,"\ninputFile.csv doesn't contain 5 lines. There should be five entries, one per line, in this file. Each line will consist of a name for the material and the associated roughness.\n");
                                fclose(inputFile);
                                return -1;
                        }

                        ret = fscanf(inputFile, "%20[^,],%f\n", materialName[index],&manningRoughnessCoefficients[index]);
                }
                if(index != 5) {
                        fprintf(stdout,"\ninputFile.csv is not as per expectations. In this file, there should be five entries, one per line. Each line should consist of 2 comma separated values. The first value is the name of the material and the second value is its Manning roughness factor.\n");
                        fclose(inputFile);
                        return -1;
                }
                fclose(inputFile);
        }
        return 0;
}

/* This function will print the program introduction message. */
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

/* This function will print the channel diagram indicating the width and depth of the channel using stars and dashes. */
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

/* This function will display the result matrix on screen and save that in the output file("theAdventuresOfZorro.txt") as well. */
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
