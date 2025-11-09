#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// ------------------------------------------ General Utility Functions ------------------------------------------

// Returns number of lines in a .txt file
int countFileLines(char *fileName){
    // Counts numbers of lines in txt file by counting number of '\n's used.

    FILE *readInFile;
    readInFile = fopen(fileName,"r");

    int lines = 0, character;

    while ((character = fgetc(readInFile)) != EOF){
        if (character == '\n')
            lines++;
    }

    fclose(readInFile);
    return lines;
}


// Intended to work the same way as fgets, except you can pass in the specific line number
// of a .txt file you want it to return.
void fgetsAtLineNum(char *string, int bufferSize, char *fileName, int lineNum){
	// NOTE: This function will cause problems if the bufferSize you're inputting into this funcion
	// and the actual buffersize of the string you're writing to are different.

	char lineString[bufferSize] = {};
	FILE *activeDeckFile;

	activeDeckFile = fopen(fileName,"r");
	
	for (int i = 1; i <= lineNum ; i ++)
		fgets(lineString,bufferSize, activeDeckFile);
	
	// sets the variable at the location stringLocation to linesString[0]
	// and then again for lineString [1] 2 3 etc...
	for (int i = 0; i < bufferSize ; i ++)
		*(string + i) = lineString[i];
	
        fclose(activeDeckFile);

	// might want to add EOF functionality
}


// Just draws a line of hyphons: made into function for ease of use.
void drawLine(int lineLength){
	for (int i = 0 ; i < lineLength ; i ++)
		printf("-");
	printf("\n");
}


//Shifts each element of array by amount
	//Right if positive left if negative
	// - leaves elements in place if no number is being shifted to it.
	// - making shiftAmt more than buffer should on paper not do anything
	//   - I have not fully tested this so I do not recommend.
	// - does not write elements beyond buffer
void shiftArray(char *arrayLocation, int bufferSize, int shiftAmt){
	// The value at location arrayLocation should be set to the value of arraylocation + shiftAmt
	if(shiftAmt < 0){
		for (int i = shiftAmt; i < bufferSize ; i++)
			*(arrayLocation + i + shiftAmt) = *(arrayLocation + i);
	}else{
		for (int i = (bufferSize-shiftAmt-1); i >= 0 ; i--)
			*(arrayLocation + i + shiftAmt) = *(arrayLocation + i);
	}
}













//--------------------------- App Functions ---------------------------

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



	FILE *activeDeckFile;
	activeDeckFile = fopen("decks/PredicateLogicIntro.txt","a");

	printf("Enter the question you'd like to ask! (start line with :h for header)\n");

	char input[200] = {};
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

	printf("Flashcard saved and editable in decks/PredicateLogicIntro.txt\n");
		
        fclose(activeDeckFile);
}

// lists all downloaded decks (.txt files in decks/)
void decks(){
	printf("deck()");
}

// tests user on selected deck
void testme(){
	//list available decks

	//ask user which one to open
	// --------------------- very proprietry deck chooser ----------------------
	char activeDeckName[300];
	printf("Which deck file from /decks would you like to open?\n");
	
	scanf("%s",&activeDeckName);

	//add "decks/" to beginning of input
	shiftArray(activeDeckName,300,6);
	for (int i = 0 ; i < 6 ; i++){
		activeDeckName[i] = "decks/"[i];

	}

	printf("%s",activeDeckName);

	


	//using test example eck for time being.
	FILE *activeDeckFile;
	activeDeckFile = fopen(activeDeckName,"r");
	
	char activeLine[300];
	int qNum = 2, hNum = -1, headerAndQsLocations[10][60] = {};

	// Constructs array with the linenumber of each header and question, as well as the number of questions
	// e.g. first header in a file :[0][line of header, there are 3 questions in this header, line of question 1, line of question 2, line of question 3]
	// headerAndQsLocations[headernumber][]=[line of header, number of questions, line number of question 1, line number of q2, etc...]
	for (int lineNum = 1; lineNum<= countFileLines(activeDeckName); lineNum++){
		int n = 300;
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
	
	srand(time(NULL));

	hNum++;
	//hnum is now equal to the amount of headers
	
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

	printf("\nThe array for debugging:  \n");
	for (int i = 0 ; i <4 ; i++){
		for (int j = 0 ; j < 15; j ++){
		printf("%d ",headerAndQsLocations[i][j]);

		}
	printf("\n");
	}
	printf("\n");
	
	char input[300];
	// ASKING QUESTIONS
	// Will be reusing char activeLine[]  since no longer in use
	for (int header = 0 ; header < hNum ; header ++){
		fgetsAtLineNum(activeLine,300,activeDeckName,headerAndQsLocations[header][0]);
		
		// Shift activeLine left 2
		shiftArray(activeLine,300,-2);
		
		drawLine(20);
		printf("Section %d: %s", headerAndQsLocations[header][0], activeLine);
		drawLine(20);
	

		for (int question = 0 ; question < headerAndQsLocations[header][1]; question ++){
			//print question
			fgetsAtLineNum(activeLine,300,activeDeckName,headerAndQsLocations[header][question+2]);
			// Shift activeLine left 2 - gets rid of 'q:'
			shiftArray(activeLine,300,-2);
			printf("%s",activeLine);

			//works out correct answer
			fgetsAtLineNum(activeLine,300,activeDeckName,headerAndQsLocations[header][question+2]+1);
			shiftArray(activeLine,300,-2);
			
			//wait for user answer
			fgets(input, sizeof(input), stdin);
			

			if (!strcmp(input,activeLine))
				printf("Correct!\n");
			else{
				printf("Incorrect :(\n");
				printf("The correct answer was: %s", activeLine);
			}
			// print answer explanation
			fgetsAtLineNum(activeLine,300,activeDeckName,headerAndQsLocations[header][question+2]+2);
			printf("%s",activeLine);

			sleep(4);
			drawLine(40);

		}
	}
        fclose(activeDeckFile);
}

//--------------------------- Function Selector ---------------------------


int main(int argc, char *argv[]){


    if (argc == 1){
        //help
	help();

    }else if (strcmp(argv[1],"add")==0){
        //add
	add();

    }else if (strcmp(argv[1],"testme")==0){
        //asks practice questions from chosen deck of flashcards
	testme();

    }else{
        printf("Unknown BashCards command. Please try again.\n");
    }

    return 0;
}
