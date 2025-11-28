#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <getopt.h>

#define BUFFSIZE 300

// BROADER IMPROVEMENTS I COULD MAKE TO PROGRAM
//  - I think im just allocating the array of pointers that holds q&a&etc, not actual space for the whole question strings
//  - put all deck functions into headers - 'bcdeck.h'
//  - just pass around pointers when shuffling instead of using strcpy
//  - rename buffsize buffmax or maxbuff
//  - use pointer notation when handling subdeck?

// NEXT TIMES
//
//  - create header files and separate code
//  - make different functions for windows specific implementation.
//  - add windows preprocessors and ability.
//  - add windows installation option
//  - listAvailable decks only to dispay .txt files
//  - remove '.txt' from listAvailableDecks output
//
//  - only see .txt files in directory
//  - add a couple of example decks for publishing with files
//
//  - also allow you to run any deck file in active directory but putting '.txt' on end.
//  - let user pass in deck name as argument from command line
//  - allow for running decks in decks directory without having to put '.txt' on end

// A 'subdeck' has one header (h:) and then all the questions/answers/explanations until the next header. Questions are randomised within a subdeck.
struct subdeckFormat{
    char header[BUFFSIZE];
    int questionAmount;
    char question[BUFFSIZE][BUFFSIZE]; // array of strings
    char answer[BUFFSIZE][BUFFSIZE];
    char explanation[BUFFSIZE][BUFFSIZE];
};

// ------------------------------------------ General Utility Functions ------------------------------------------
int countFileLines(char *fileName); // counts number of lines in .txt file. Returns number of lines as int
void drawLine(int lineLength); // Draws a line of hyphons ending with \n.
char *shiftArray(char *array, int bufferSize, int shiftAmt); // shifts each element of array by shiftAmt. Right if positive left if negative. Returns array
char *newLineToNull(char *string); // replaces first '\n' found with '\0'. Returns same string passed in.

// ------------------------------------------ Program Specific Functions -----------------------------------------
char *findDecks(char *deckFilesDirectory); // locates the file location of the downloaded decks. Dictated by .config file.
char **listAvailableDecks(); // Prints the the downloaded decks in the default deck location to the shell. MEMORY ALLOCATED DURING FUNCTION - BE SURE TO FREE THIS
int countHeaders(FILE *inFile); // counts the number of headers/subdecks in file. resets fgets count of file at end.
struct subdeckFormat *buildSubdecks(struct subdeckFormat *subdeck, FILE *inFile); // returns an array of each 'subdeck' in a file (containing 1 header, the number of questions, and all questions in that header)
struct subdeckFormat *shuffleSubdecks(struct subdeckFormat *subdeck, int headersAmount); // Shuffles each of the questions within each subdeck
void listHeaders (struct subdeckFormat *subdeck, int headersAmount); // Lists the headers of the active subdeck.
int askQuestions(struct subdeckFormat *subdeck, int subdeckToAsk); // asks questions of one specific subdeck (passed into function). Returns 0 if worked normally. Returns different values if user has used a 'command';
int setSaveToWorkingDir(); // does what it says on the tin. -1 for fail 0 for success.
// ---------------------------------------------- Primary App Modes ----------------------------------------------
void testme();         // tests the user on a deck of flashcards. -- Working
void help();           // Gives a quick help option for users. -- Not Started

//---------------------------------------------- App Mode Selection ----------------------------------------------

int main(int argc, char *argv[]){

    int opterr = 0;
    int c;
    int optionIndex = 0;

    struct option longOptions[] = {
        {"file-path",no_argument,0,'f'},
        {"set-save",no_argument,0,'s'}
    };

    while (-1 != (c = getopt_long (argc,argv,"sf",longOptions, &optionIndex))){
        switch(c){
            case 's':
                setSaveToWorkingDir();
                break;
            case 'f':
                char s[BUFFSIZE];
                printf("%s\n",findDecks(s));
                break;
        }
    }

    if (optind == 1) // run bcards normally if no flags found
        testme();

    return EXIT_SUCCESS;
}

//--------------------------- Primary App Modes ---------------------------

// tests user on selected deck
void testme(){


    
    //count available decks - dynamically allocate memory - build list of decks
    char **arrayOfDecks;
    if (NULL == (arrayOfDecks = listAvailableDecks())){
        printf("Unable to list available decks\n");
        return;
    }

    char decksLocation[BUFFSIZE],activeDeckName[BUFFSIZE];

    findDecks(decksLocation);

    printf("Please enter the number of the deck you'd like to be tested on: ");
    char inC[BUFFSIZE];
    int inD;
    fgets(inC, sizeof(inC), stdin);
    if (0==(inD = atoi(inC))){
        printf("Invalid entry\n");
        return;
    }
    sprintf(activeDeckName,"%s/%s",decksLocation,arrayOfDecks[inD-1]);

    //free memory allocated by listAvailableDecks
    for (int i=0; arrayOfDecks[i] ; i++)
        free(arrayOfDecks[i]);
    free(arrayOfDecks);

    printf("Opening %s...\n", activeDeckName);

    FILE *activeDeckFile;
    if(!(activeDeckFile = fopen(activeDeckName,"r"))){
        printf("Unable to locate that deck file\n");
        return;
    }else
    printf("Succesffully opened!\n");

    int headersAmount = countHeaders(activeDeckFile);

    struct subdeckFormat *
    subdeck = (struct subdeckFormat*) malloc (headersAmount*sizeof(struct subdeckFormat));
    if (subdeck == NULL){
        printf("Memory Allocation Failed.\n");
        return;
    }

    if (activeDeckFile == NULL){
        printf("File == NULL. Terminating.\n");
        return;
    }
    buildSubdecks(subdeck, activeDeckFile);
    fclose(activeDeckFile);
    
    shuffleSubdecks(subdeck, headersAmount);

    listHeaders(subdeck, headersAmount);

    int command;
    //ask questions but checks for 'commands'
    for (int i = 0 ; i < headersAmount ; i++){
        //askQuestions returns -1 if running normally, or different if user wants to run a command.
        command = askQuestions(subdeck,i);
        if (command){
            drawLine(100);
            if (command == -1){
                listHeaders(subdeck,headersAmount);
                i--;
            }else if (command>headersAmount || command<1){
                printf("That's not a valid header to skip to. Restarting current header...\n");
                i--;
            }else
                i = command-2;// going to be incremented anyway
        }
    }

    free(subdeck);
}


// Incomplete
void help(){
    printf("help()\n");
}

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


// ------------------------------------------ Program Specific Functions -----------------------------------------

char *findDecks(char *output){

#ifdef _WIN32
    //----------------Windows implementation-------------------
    

    // add code here



#else
    //----------------Linux implementation-------------------
    char deckFilesLocation[BUFFSIZE] = {}, configFileLocation[BUFFSIZE] = {};
    FILE *configFile;
    char *homeDirectory = getenv("HOME");
    if (homeDirectory == NULL){
        printf("Failed to identify home directory\n");
        return NULL;
    }

    strcat(configFileLocation,homeDirectory);
    strcat(configFileLocation,"/.config/bashcards/decksavelocation");

    //open the config file with the deck save locations
    if (!(configFile = fopen(configFileLocation,"r"))){
        printf("Unable to open ~/.config/bashcards/decksavelocation\n - Fix or reinstallation required.\n");
        return NULL;
    }
    fgets(deckFilesLocation,BUFFSIZE,configFile);
    fclose(configFile);

    //remove \n from deckslocation
    newLineToNull(deckFilesLocation);

    //printf("deck files location %s",deckFilesLocation);
    strcpy(output,deckFilesLocation);
    return output;
#endif 
}

char **listAvailableDecks(){
    //rewrite to also take input of an array, and return a pointer to that array.
    // - make array miss files that start with '.'
    // - make array only print .txt files
    // - make array remove '.txt' from those files
    
#ifdef _WIN32
    //---------------Windows Implementation-------------------

    // add code here

#else
    //---------------Linux Implementation-------------------
    char decksLocation[BUFFSIZE];
    if (!findDecks(decksLocation)){
        printf("Unable to locate deck save location. Please set deck save location in ~/.config/bashcards/decksavelocation\n");
        return NULL;
    }

    DIR *decksDirectory;
    if (!(decksDirectory = opendir(decksLocation))){
        printf("Unable to open %s\nIf this is the incorrect save location for your decks, please change this at ~/.config/bashcards/decksavelocation\n",decksLocation);
        return NULL;
    }
    struct dirent* individualFiles;

    //work out amount of memory needed for allocation
    int count = 0;
    while(individualFiles = readdir(decksDirectory))
        if (individualFiles->d_name[0] != '.')
            count++;
        count++; // one extra for null character terminating array
    rewinddir(decksDirectory);

    char **deckArr = (char **)malloc(count * sizeof(char *));
    if (deckArr == NULL){
        printf("Memory Allocation Failed.\n");
        return NULL;
    }

    printf("Downloaded decks available at %s are as follows:\n\n",decksLocation);
    //build the array
    int i = 0;
    while (individualFiles = readdir(decksDirectory)){
        //if it doesn't start with '.'     and as long as it ends  with '.txt'
        if (individualFiles->d_name[0] != '.'){
            deckArr[i] = malloc (300 * sizeof(char));
            if (deckArr[i] == NULL){
                printf("Memory Allocation Failed.\n");
                return NULL;
            }
            strcpy(deckArr[i],individualFiles->d_name);
            //printf("%s\n",deckArr[i]);
            i++;
        }
    }
    deckArr[i]=NULL; // make the array null terminated
    closedir(decksDirectory);
    
    for (int j; j< i; j++)
        printf("%d - %s\n",j+1,deckArr[j]);

    printf("\nIf you wish to change your deck save location please go to ~/.config/bashcards/decksavelocation\n");
    drawLine (100);
    return deckArr; // return pointer to the allocate memory
#endif 
}

int countHeaders(FILE *inFile){
    int i = 0;
    char array[BUFFSIZE];
    int n = 300;
    while (fgets(array, n, inFile))
        if (array[0]=='h'&&array[1]==':')
            i++;
    rewind(inFile);
    return i;
}

struct subdeckFormat *
buildSubdecks (struct subdeckFormat *subdeck, FILE *inFile){
    // add ability to remove line type prefix when reading in 

    /* As a reminder, subdecks are formatted as follows:
 
    struct subdeckFormat{
        char header[BUFFSIZE];
        int questionAmount;
        char question[BUFFSIZE][BUFFSIZE]; // array of strings
        char answer[BUFFSIZE][BUFFSIZE];
        char explanation[BUFFSIZE][BUFFSIZE];
    }; */

    char activeLine[BUFFSIZE];
    int hNum = -1, qNum = -1;
    while (fgets(activeLine, BUFFSIZE , inFile)){
        if (activeLine[0] == 'h' && activeLine[1] == ':'){
            //if header found
            if (hNum >= 0)
                subdeck[hNum].questionAmount = qNum+1;
            qNum = -1;

            hNum++;
            newLineToNull(activeLine);
            shiftArray(activeLine,BUFFSIZE,-2);
            strcpy(subdeck[hNum].header,activeLine);

        }else if (activeLine[0] == 'q' && activeLine[1] == ':'){
            //if question found
            qNum ++;
            newLineToNull(activeLine);
            shiftArray(activeLine,BUFFSIZE,-2);
            strcpy(subdeck[hNum].question[qNum],activeLine);

        }else if (activeLine[0] == 'a' && activeLine[1] == ':'){
            //if answer found
            newLineToNull(activeLine);
            shiftArray(activeLine,BUFFSIZE,-2);
            strcpy(subdeck[hNum].answer[qNum],activeLine);
            //then also get explanation on next line
            fgets(activeLine, BUFFSIZE, inFile);
            newLineToNull(activeLine);
            strcpy(subdeck[hNum].explanation[qNum],activeLine);
        }
    }
    // had to add here to set questionAmount the final time - maybe consider making code cleaner
    subdeck[hNum].questionAmount = qNum+1;
    rewind(inFile);
    return subdeck;
}

struct subdeckFormat *shuffleSubdecks(struct subdeckFormat *subdeck, int sdNum){
    srand(time(NULL));
    // Shuffles all questions within each each header in the array
    // Goes one header at a time
    for (int sd= 0; sd< sdNum ;sd++){

        //goes one question at a time
        //swaps with a different question chosen at random
        for (int q = 0; q < subdeck[sd].questionAmount ; q++){
            if (subdeck[sd].questionAmount > 1){
                char tempQ[BUFFSIZE];
                char tempA[BUFFSIZE];
                char tempE[BUFFSIZE];
                strcpy(tempQ,subdeck[sd].question[q]);
                strcpy(tempA,subdeck[sd].answer[q]);
                strcpy(tempE,subdeck[sd].explanation[q]);

                int r = rand() % (subdeck[sd].questionAmount - 1);

                strcpy(subdeck[sd].question[q],subdeck[sd].question[r]);
                strcpy(subdeck[sd].answer[q],subdeck[sd].answer[r]);
                strcpy(subdeck[sd].explanation[q],subdeck[sd].explanation[r]);

                strcpy(subdeck[sd].question[r],tempQ);
                strcpy(subdeck[sd].answer[r],tempA);
                strcpy(subdeck[sd].explanation[r],tempE);
            }
        }
    }
}

void listHeaders (struct subdeckFormat *subdeck, int sNum){
    printf("\n");
    for (int i = 0 ; i < sNum ; i++){
        printf(" - %d: %s\n",i+1,subdeck[i].header);
    }
    printf("\nType : followed by a number to skip to that header.\n:0 skips to the next header and :-1 lists all headers again!\nUSING ANY COMMAND WILL ERASE ANY CURRENT PROGRESS\n");
    drawLine(100);
}

int askQuestions(struct subdeckFormat *subdeck, int s){
    // asks questions
    // notably returns NULL if worked successfully, or
    char input[BUFFSIZE];
    printf("Section %d: %s\n",s+1,subdeck[s].header);
    drawLine(100);
    for (int i = 0 ; i<subdeck[s].questionAmount ; i++){

        printf ("%s\n",subdeck[s].question[i]);
        fgets(input, sizeof(input), stdin);
        newLineToNull(input);

        if (input[0]==':'){
            //if input is a command
            shiftArray(input,BUFFSIZE,-1);
            return atoi(input); // returns the integer value
        }else if (!strcmp(input,subdeck[s].answer[i]))
            printf("Correct!\n");
        else
            printf("Incorrect\nThe correct answer was: %s\n",subdeck[s].answer[i]);
        sleep(1);
        printf("%s\n",subdeck[s].explanation[i]);
        drawLine(100);
    }
    printf("End of questions.\n");
    return -1;
}


int setSaveToWorkingDir(){

    printf("Setting save to working directory - function not currently completed.\n");


}




