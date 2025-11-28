#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <getopt.h>
// ------------------------------------------ General Utility Functions ------------------------------------------

//Shifts each element of array by amount
   //Right if positive left if negative
    // - leaves elements in place if no number is being shifted to it.
    // - making shiftAmt more than buffer should on paper not do anything
    //   - I have not fully tested this so I do not recommend.
    // - does not write elements beyond buffer
char *shiftArray(char *arrayLocation, int bufferSize, int shiftAmt){
    // The value at location arrayLocation should be set to the value of arraylocation + shiftAmt
    if(shiftAmt < 0)
        for (int i = -shiftAmt; i < bufferSize ; i++)
            *(arrayLocation + i + shiftAmt) = *(arrayLocation + i);
    else
        for (int i = (bufferSize-shiftAmt-1); i >= 0 ; i--)
            *(arrayLocation + i + shiftAmt) = *(arrayLocation + i);
    return arrayLocation;
}

// Just draws a line of hyphons finishing with \n
void drawLine(int lineLength){
    while (lineLength--)
        printf("-");
    printf("\n");
}

//replaces the first \n found in string with \0
char *newLineToNull(char *string){
    while (*string++)
        if (*string == '\n')
            *string = '\0';
    return string;
}


