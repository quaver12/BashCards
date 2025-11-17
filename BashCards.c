#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>

#define BUFFSIZE 300

// IMPROVEMENTS I COULD MAKE TO PROGRAM
//  - just pass around pointers when shuffling instead of using strcpy
//  - rename buffsize buffmax or maxbuff

// FOR NEXT TIME
//  - test shuffle and see if actually working
//  - once complete, begin replacing array with struct version

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
char *fgetsAtLineNum(char *string, int bufferSize, char *fileName, int lineNum); // reads the line at lineNum from a txt file into string. Returns string or NULL if error
void drawLine(int lineLength); // Draws a line of hyphons ending with \n.
char *shiftArray(char *array, int bufferSize, int shiftAmt); // shifts each element of array by shiftAmt. Right if positive left if negative. Returns array
char *newLineToNull(char *string); // replaces first '\n' found with '\0'. Returns same string passed in.
int isCorrect(const char userAns[], const char correctAns[], int uaLen, int caLen); // Propietry Fuzzy matcher. Returns true or false. Still work in progress.

// ------------------------------------------ Program Specific Functions -----------------------------------------
char *findDecks(char *deckFilesDirectory); // locates the file location of the downloaded decks. Dictated by .config file.
void listAvailableDecks(); // Prints the the downloaded decks in the default deck location to the shell.
int countHeaders(FILE *inFile); // counts the number of headers/subdecks in file. resets fgets count of file at end.
struct subdeckFormat *buildSubdecks(struct subdeckFormat *subdeck, FILE *inFile); // returns an array of each 'subdeck' in a file (containing 1 header, the number of questions, and all questions in that header)
struct subdeckFormat *shuffleSubdecks(struct subdeckFormat *subdeck, int headersAmount); // Shuffles each of the questions within each subdeck

// ---------------------------------------------- Primary App Modes ----------------------------------------------
void testme();         // tests the user on a deck of flashcards.
void help();           // Gives a quick help option for users.
void add();            // User can make a deck by adding flashcards.
void decks();          // Lists available decks

//---------------------------------------------- App Mode Selection ----------------------------------------------

int main(int argc, char *argv[]){

    if (argc == 1){
        testme(); // run testme by default

    }else if (strcmp(argv[1],"add")==0){
        //add to existing deck of flashcards or make a new one!
        add();

    }else if (strcmp(argv[1],"testme")==0){
        //asks practice questions from chosen deck of flashcards
        testme();

    }else{
        printf("Unknown BashCards command. Please try again.\n");
    }

    return EXIT_SUCCESS;
}

//--------------------------- Primary App Modes ---------------------------

// tests user on selected deck
void testme(){
    //list available decks
    //if (isCorrect("predicates and relations","Properties and Relations",24,24))
    //printf("isCorrect returned true!!\n");

    listAvailableDecks();

    char decksLocation[BUFFSIZE];
    findDecks(decksLocation);

    char activeDeckName[BUFFSIZE];
    printf("Which deck file would you like to be tested on? Please enter full file name: ");

    char input[BUFFSIZE];
    fgets(input, sizeof(input), stdin);
    newLineToNull(input);

    sprintf (activeDeckName,"%s/%s",decksLocation,input);

    printf("Opening %s...\n", activeDeckName);

    FILE *activeDeckFile;
    if(!(activeDeckFile = fopen(activeDeckName,"r"))){
        printf("Unable to locate that file\n");
        return;
    }else
    printf("Succesffully opened!\n");

    char activeLine[BUFFSIZE];
    int qNum = 2, hNum = -1, headerAndQsLocations[10][60] = {};


    // instead of contructing array, construct struct using constructSubdecks()
    // struct subdeck *constructsSubdecks()
    // will return the amount of subdecks (equal to number of headers)
    // subdeck[2].question[2]

    int headersAmount = countHeaders(activeDeckFile);

    struct subdeckFormat *subdeck = (struct subdeckFormat*) malloc (headersAmount*sizeof(struct subdeckFormat));

    if (subdeck == NULL){
        printf("Memory Allocation Failed.");
        return;
    }

    buildSubdecks(subdeck, activeDeckFile); // fill/build that array
    

    //shuffle questions of subdecks
    shuffleSubdecks(subdeck, headersAmount);

    printf("the second header is: %s\n",subdeck[1].header);
    printf("the number of questions in subdeck 3 is: %d\n",subdeck[2].questionAmount);
    printf("question 3 of the sixth header is: %s\n",subdeck[5].question[2]);
    printf("the answer to h2 q4 is: %s\n",subdeck[1].answer[3]);
    printf("the explanation to of h6 q3 is: %s\n",subdeck[5].explanation[2]);


    // Constructs array with the linenumber of each header and question, as well as the number of questions
    // e.g. first header in a file :[0][line of header, there are 3 questions in this header, line of question 1, line of question 2, line of question 3]
    // headerAndQsLocations[headernumber][]=[line of header, number of questions, line number of question 1, line number of q2, etc...]
    for (int lineNum = 1; lineNum<= countFileLines(activeDeckName); lineNum++){
        int n = BUFFSIZE;
        fgets(activeLine,n, activeDeckFile);

        if (activeLine[0] == 'h' && activeLine[1] == ':'){
            //line found is a header
            hNum++;
            headerAndQsLocations[hNum][0]=lineNum;
            qNum = 2;
        }else if (activeLine[0] == 'q' && activeLine[1] == ':'){
            //then it's a question	
            headerAndQsLocations[hNum][qNum]=lineNum;
            qNum++;

            //writes qNum-2 to the array each time,
            //so when the loop leaves and goes to the next header,
            //headerAndQsLocations[hNum][1] will be left with the number of questions in that header
            headerAndQsLocations[hNum][1]=qNum-2;
        }
    }


    hNum++;
    //hnum is now equal to the amount of headers

    srand(time(NULL));
    // Shuffles all questions within each each header in the array
    // Goes one header at a time
    for (int header = 0; header < hNum ;header++){

        //shuffle array (but only from [i][1] onwards)
        //goes one question at a time
        //swaps with a different question chosen at random
        for (int question = 2; question < headerAndQsLocations[header][1] + 2 ; question++){
            int temp = 0, transferTo = 0;
            temp = headerAndQsLocations[header][question];
            transferTo = rand() % (headerAndQsLocations[header][1]) + 2;
            headerAndQsLocations[header][question] = headerAndQsLocations[header][transferTo];
            headerAndQsLocations[header][transferTo] = temp; 
        }
    }

    //prints headers
    printf("\n%s",input);
    char headerForPrinting[BUFFSIZE];
    for (int i = 0 ; i < hNum ; i++){
        fgetsAtLineNum(headerForPrinting,BUFFSIZE,activeDeckName,headerAndQsLocations[i][0]);
        printf(" - Section %d: %s", i+1, headerForPrinting);
    }
    printf("\nSkip to desired header by typing :h + header number when prompted to answer a question.\nE.g :h2\n");

    // ASKING QUESTIONS
    // Will be reusing char activeLine[]  since no longer in use
    for (int header = 0 ; header < hNum ; header ++){
        fgetsAtLineNum(activeLine,BUFFSIZE,activeDeckName,headerAndQsLocations[header][0]);

        // Shift activeLine left 2
        shiftArray(activeLine,BUFFSIZE,-2);

        drawLine(100);
        printf("Section %d: %s", header+1, activeLine);
        drawLine(100);


        for (int question = 0 ; question < headerAndQsLocations[header][1]; question ++){
            //print question
            fgetsAtLineNum(activeLine,BUFFSIZE,activeDeckName,headerAndQsLocations[header][question+2]);
            // Shift activeLine left 2 - gets rid of 'q:'
            shiftArray(activeLine,BUFFSIZE,-2);
            printf("%s",activeLine);

            //works out correct answer
            fgetsAtLineNum(activeLine,BUFFSIZE,activeDeckName,headerAndQsLocations[header][question+2]+1);
            shiftArray(activeLine,BUFFSIZE,-2);

            //wait for user answer
            fgets(input, sizeof(input), stdin);

            if (input[0]==':'&&input[1]=='h'){
                printf("Skipping to header %c. Give me 4 seconds.\n",input[2]);
                question = headerAndQsLocations[header][1];
                header = input[2]-2-'0';
            }else if (!strcmp(input,activeLine))
                printf("Correct!\n");
            else{
                printf("Incorrect :(\n");
                printf("The correct answer was: %s", activeLine);
            }
            // print answer explanation
            fgetsAtLineNum(activeLine,BUFFSIZE,activeDeckName,headerAndQsLocations[header][question+2]+2);
            printf("%s",activeLine);

            sleep(4);
            drawLine(100);

        }
    }
        free(subdeck);
        fclose(activeDeckFile);
}


// Incomplete
void help(){
    printf("help()\n");
}

//adds questions and answers of user input to decks 
void add(){
	
	// 'Deck files' will be formatted the following way for BashCards:
	//
	// h:This is a header - headers are asked in the order they appear in the file
	// q:This is a question - all questions within each header are shuffled when asked with testme().
	// a:This is the answer to the question above! Answers are specific and case sensitive at the minute.
	//  - The line after an answer is the explanation for the answer, no specific formatting needed - everything on this line will be printed.
	//        Also as long as a line isn't following an answer and doesn't have a line type prefix, it will be ignored.
	//        This means you can edit a file with comments like this.
	// h: Example questions
	// q:True or False: The capital of Turkey is Constantinople.
	// a:False
	//  -> It's Istanbul not Constantinople!
	// q:What was the previous name of the city now called New York.
	// a:New Amsterdam
	// 	- New York was once a Dutch settlement before being taken by the English military in 1664.

	char decksLocation[BUFFSIZE];
	findDecks(decksLocation);
	
	char activeDeckName[BUFFSIZE];
	printf("Which deck file from /decks would you like to add to?\nInputting unregonised file name will make a new deck with that name.\n");

	char input[BUFFSIZE];
	scanf("%s",&input);

	strcat(activeDeckName,decksLocation);
	strcat(activeDeckName,"/");
	strcat(activeDeckName,input);

	printf("Opening %s...\n", activeDeckName);

	FILE *activeDeckFile;
	activeDeckFile = fopen(activeDeckName,"a");

	printf("Enter the question you'd like to ask! (start line with :h for header)\n");
	
	fgets(input, sizeof(input), stdin);
	
	//if header tag seen only add first line
	if (input[0]=='h' && input[1]==':'){
		fprintf(activeDeckFile, "\n%s\n",input);
			
	//if h: not seen, prompt user for q and a
	}else{

		fprintf(activeDeckFile, "q:%s",input);

		printf("Enter the specific answer you're looking for:\n");
		fgets(input, sizeof(input), stdin);
		fprintf(activeDeckFile, "a:%s",input);

		printf("Enter any explanation you'd like about the answer.\n");
		fgets(input, sizeof(input), stdin);
		fprintf(activeDeckFile, " - %s",input);


	}

	printf("Flashcard saved and editable in %s\n",activeDeckName);
		
        fclose(activeDeckFile);
}

// lists all downloaded decks (.txt files in decks/)
void decks(){
	printf("deck()");
}





// ------------------------------------------ General Utility Functions ------------------------------------------
// Returns number of lines in a .txt file
int countFileLines(char *fileName){
    // Counts numbers of lines in txt file by counting number of '\n's used.

    FILE *readInFile;
    if(!(readInFile = fopen(fileName,"r"))){
        printf("Failed to open %s during countFileLines.",fileName);
        return 0;
    }

    int lines = 0, character;

    while ((character = fgetc(readInFile)) != EOF)
        if (character == '\n')
            lines++;

    fclose(readInFile);
    return lines;
}

// Intended to work the same way as fgets, except you can pass in the specific line number
// of a .txt file you want it to return.
char *fgetsAtLineNum(char *string, int bufferSize, char *fileName, int lineNum){
    // NOTE: This function will cause problems if the bufferSize you're inputting into this funcion
    // and the actual buffersize of the string you're writing to are different.

    char lineString[bufferSize] = {};
    FILE *activeDeckFile;

    if (!(activeDeckFile = fopen(fileName,"r"))){
        printf("Unable to open file during fgetsAtLineNum()");
        return NULL;
    }

    for (int i = 1; i <= lineNum ; i ++)
        fgets(lineString,bufferSize, activeDeckFile);

    // sets the variable at the location stringLocation to linesString[0]
    // and then again for lineString [1] 2 3 etc...
    for (int i = 0; i < bufferSize ; i ++)
        *(string + i) = lineString[i];

    fclose(activeDeckFile);
    return string;
}

// Just draws a line of hyphons finishing with \n
void drawLine(int lineLength){
    while (lineLength--)
        printf("-");
    printf("\n");
}

//Shifts each element of array by amount
    //Right if positive left if negative
    // - leaves elements in place if no number is being shifted to it.
    // - making shiftAmt more than buffer should on paper not do anything
    //   - I have not fully tested this so I do not recommend.
    // - does not write elements beyond buffer
char *shiftArray(char *arrayLocation, int bufferSize, int shiftAmt){
    // The value at location arrayLocation should be set to the value of arraylocation + shiftAmt
    if(shiftAmt < 0)
        for (int i = shiftAmt; i < bufferSize ; i++)
            *(arrayLocation + i + shiftAmt) = *(arrayLocation + i);
    else
        for (int i = (bufferSize-shiftAmt-1); i >= 0 ; i--)
            *(arrayLocation + i + shiftAmt) = *(arrayLocation + i);
    return arrayLocation;
}

//replaces the first \n found in string with \0
char *newLineToNull(char *string){
    while (*string++)
        if (*string == '\n')
            *string = '\0';
    return string;
}

//proprietry fuzzy matcher - checks if userAns is close enough to correctAns
//returns true or false
int isCorrect(const char userAns[], const char correctAns[], int uaLen, int caLen){
	//if character in userAns is present in correctAns, increase score by 1
	//if at the end score is at least 75% of the correctanswer's length, return true
	int score = 0;

	for (int i = 0 ; i < uaLen ; i ++){
		for (int j = 0 ; j < caLen ; j++){
			if (userAns[i] == correctAns[j]){
				score++;
				j = caLen;
			}
		}
	}
	printf("score is: %d\n", score);


	if (score>=((caLen/4)*3))
		return true;
	else
		return false;

	
}


// ------------------------------------------ Program Specific Functions -----------------------------------------

char *findDecks(char *output){

    //----------------Linux implementation-------------------
    char deckFilesLocation[BUFFSIZE] = {}, configFileLocation[BUFFSIZE] = {};
    char *homeDirectory = getenv("HOME");
    FILE *configFile;


    strcat(configFileLocation,homeDirectory);
    strcat(configFileLocation,"/.config/bashcards/decksavelocation");

    //open the config file with the deck save locations
    if (!(configFile = fopen(configFileLocation,"r"))){
        printf("Failed to Locate config file 'decksavelocation'.");
        return NULL;
    }
    fgets(deckFilesLocation,BUFFSIZE,configFile);
    fclose(configFile);

    //remove \n from deckslocation
    newLineToNull(deckFilesLocation);

    //printf("deck files location %s",deckFilesLocation);
    strcpy(output,deckFilesLocation);
    return output;
}

void listAvailableDecks(){

    //---------------Linux Implementation-------------------
    char decksLocation[BUFFSIZE];
    findDecks(decksLocation);
    printf("Downloaded decks available at %s are as follows:\n\n",decksLocation);

    DIR *decksDirectory;
    decksDirectory = opendir(decksLocation);
    struct dirent* individualFiles;

    while (individualFiles = readdir(decksDirectory))
        printf(" - %s\n",individualFiles->d_name);

    closedir(decksDirectory);
    printf("\nIf you wish to change your deck save location please go to ~/.config/bashcards/decksavelocation\n");
    drawLine (100);
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
            strcpy(subdeck[hNum].header,activeLine);

        }else if (activeLine[0] == 'q' && activeLine[1] == ':'){
            //if question found
            qNum ++;
            newLineToNull(activeLine);
            strcpy(subdeck[hNum].question[qNum],activeLine);

        }else if (activeLine[0] == 'a' && activeLine[1] == ':'){
            //if answer found
            newLineToNull(activeLine);
            strcpy(subdeck[hNum].answer[qNum],activeLine);
            //then also get explanation on next line
            fgets(activeLine, BUFFSIZE, inFile);
            newLineToNull(activeLine);
            strcpy(subdeck[hNum].explanation[qNum],activeLine);
        }
    }
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







