#ifndef BCUTIL_H_
#define BCUTIL_H_

// ------------------------------------------ General Utility Functions ------------------------------------------
int countFileLines(char *fileName); // counts number of lines in .txt file. Returns number of lines as int
void drawLine(int lineLength); // Draws a line of hyphons ending with \n.
char *shiftArray(char *array, int bufferSize, int shiftAmt); // shifts each element of array by shiftAmt. Right if positive left if negative. Returns array
char *newLineToNull(char *string); // replaces first '\n' found with '\0'. Returns same string passed in.
bool isFileType (char *fileName, char* fileSuffix); // returns true if the file name ends with the file suffix

#define BUFFSIZE 300

#endif

