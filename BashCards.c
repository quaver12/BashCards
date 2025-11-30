#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "bcdeck.h"
#include "bcutil.h"

// BROADER IMPROVEMENTS I COULD MAKE TO PROGRAM
//  - I think im just allocating the array of pointers that holds q&a&etc, not actual space for the whole question strings
//  - put all deck functions into headers - 'bcdeck.h'
//  - just pass around pointers when shuffling instead of using strcpy
//  - rename buffsize buffmax or maxbuff
//  - use pointer notation when handling subdeck?

// NEXT TIMES
//  - let bcards -s set deck save location
//
//  - make different functions for windows specific implementation.
//  - add windows preprocessors and ability.
//  - add windows installation option
//
//  - add a couple of example decks for publishing with files
//
//  - also allow you to run any deck file in active directory but putting '.txt' on end.
//  - let user pass in deck name as argument from command line
//  - allow for running decks in decks directory without having to put '.txt' on end


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
    sprintf(activeDeckName,"%s/%s.txt",decksLocation,arrayOfDecks[inD-1]);

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
    for (int i = 0 ; i < headersAmount ;){
        //askQuestions returns -1 if running normally, or different if user wants to run a command.
        command = askQuestions(subdeck,i);
        drawLine(100);
        if (command>0)
            i = command-1;
        else{
            switch (command){
                case (0):
                    i++;
                    break;
                case (-1): // by default askQuestions return this
                    break; // this will restart current header
                case (-2):
                    listHeaders(subdeck,headersAmount);
                    break;
                default:
                    printf("Not a valid header to jump to or command. Restarting current header...\n");
                    drawLine(100);
            }
        }
    }

    free(subdeck);
}


// Incomplete
void help(){
    printf("help()\n");
}






