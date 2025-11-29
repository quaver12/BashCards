#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include "bcutil.h"
#include "bcdeck.h"

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
        if (individualFiles->d_name[0] != '.' && isFileType(individualFiles->d_name,".txt")){
            deckArr[i] = malloc (300 * sizeof(char));
            if (deckArr[i] == NULL){
                printf("Memory Allocation Failed.\n");
                return NULL;
            }
            strcpy(deckArr[i],individualFiles->d_name);
            remFileSuffix(deckArr[i]);
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
