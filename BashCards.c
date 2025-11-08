#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


// ------------------------------------------ General Utility Functions ------------------------------------------

// Returns number of lines in a .txt file
int countFileLines(char *fileName){
    // Counts numbers of lines in txt file by counting number of '\n's used.

    FILE *readInFile;
    readInFile = fopen(fileName,"r");

    int count = 0, lines = 0, character;

    while (character != EOF){
        character = fgetc(readInFile);
        if (character == '\n')
            lines++;
        count++;
    }

    fclose(readInFile);
    return lines;
}


// Intended to work the same way as fgets, except you can pass in the specific line number
// of a .txt file you want it to return.
void fgetsAtLineNum(char *stringLocation, int bufferSize, char *fileName, int lineNum){
	// NOTE: Must take stringLocation input arg as &arrayname[0]
	// This function will cause problems if the bufferSize you're inputting into this funcion
	// and the actual buffersize of the string you're writing to are different.

	char lineString[bufferSize] = {};
	FILE *activeDeckFile;

	activeDeckFile = fopen(fileName,"r");
	
	for (int i = 1; i <= lineNum ; i ++)
		fgets(lineString,bufferSize, activeDeckFile);
	
	// sets the variable at the location stringLocation to linesString[0]
	// and then again for lineString [1] 2 3 etc...
	for (int i = 0; i < bufferSize ; i ++)
		*(stringLocation + i) = lineString[i];
	
        fclose(activeDeckFile);

	// might want to add EOF functionality
}


// Just draws a line of hyphons: made into function for ease of use.
void drawLine(int lineLength){
	for (int i = 0 ; i < lineLength ; i ++)
		printf("-");
	printf("\n");
}



//--------------------------- App Functions ---------------------------

// Incomplete
void help(){
    printf("help()\n");
}

//adds questions and answers of user input to decks 
void add(){

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
	//using test example deck for time being.
	FILE *activeDeckFile;
	activeDeckFile = fopen("decks/PredicateLogicIntro.txt","r");
	
	char activeLine[300];
	int qNum = 2, hNum = -1, headerAndQsLocations[10][60] = {};

	// Constructs array with the linenumber of each header and question, as well as the number of questions
	// e.g. first header in a file :[0][line of header, there are 3 questions in this header, line of question 1, line of question 2, line of question 3]
	// headerAndQsLocations[headernumber][]=[line of header, number of questions, line number of question 1, line number of q2, etc...]
	for (int lineNum = 1; lineNum<= countFileLines("decks/PredicateLogicIntro.txt"); lineNum++){
		int n = 300;
		fgets(activeLine,n, activeDeckFile);

		if (activeLine[0] == 'h' && activeLine[1] == ':'){
			//line found is a header
			printf("header found on line %d\n",lineNum);
			hNum++;
			headerAndQsLocations[hNum][0]=lineNum;
			qNum = 2;
			printf(" - header linenumber (%d) written to array[%d][%d]\n",headerAndQsLocations[hNum][0],hNum,0);
		}else if (activeLine[0] == 'q' && activeLine[1] == ':'){
			//then it's a question	
			printf("question found on line %d\n",lineNum);
			headerAndQsLocations[hNum][qNum]=lineNum;
			printf(" - question linenumber (%d) written to array[%d][%d]\n",headerAndQsLocations[hNum][qNum],hNum,qNum);
			qNum++;
			printf("number of questions in previous header: %d\n",qNum-2);

			//writes qNum-2 to the array each time,
			//so when the loop leaves and goes to the next header,
			//headerAndQsLocations[hNum][1] will be left with the number of questions in that header
			headerAndQsLocations[hNum][1]=qNum-2;
		}
	}
	
	srand(time(NULL));

	hNum++;
	//hnum is now equal to the amount of headers
	
	printf("hNum (amount of headers) is %d\n",hNum);
	
	// Shuffles all questions within each each header in the array
	// Goes one header at a time
	for (int header = 0; header < hNum ;header++){
		printf("looking at header %d\n",header);
		
		//shuffle array (but only from [i][1] onwards)
		//goes one question at a time
		//swaps with a different question chosen at random
		for (int question = 2; question < headerAndQsLocations[header][1] + 2 ; question++){
			int temp = 0, transferTo = 0;
			temp = headerAndQsLocations[header][question];
			transferTo = rand() % (headerAndQsLocations[header][1]) + 2;
			headerAndQsLocations[header][question] = headerAndQsLocations[header][transferTo];
			headerAndQsLocations[header][transferTo] = temp; 
			for (int o = 0 ; o < 20; o++){
				printf("%d,",headerAndQsLocations[header][o]);
			}
			printf("\n");
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
	
	// ASKING QUESTIONS
	// Will be reusing char activeLine[] since no longer in use
	char input[300];
	for (int header = 0 ; header < hNum ; header ++){
		fgetsAtLineNum(&activeLine[0],300,"decks/PredicateLogicIntro.txt",headerAndQsLocations[header][0]);
		
		// Shift activeLine left 2
		for (int i = 0; i < 298 ; i ++)
			activeLine[i] = activeLine[i+2];

		drawLine(20);
		printf("Section %d: %s", headerAndQsLocations[header][0], activeLine);
		drawLine(20);
	

		for (int question = 0 ; question < headerAndQsLocations[header][1]; question ++){
			//print question
			fgetsAtLineNum(&activeLine[0],300,"decks/PredicateLogicIntro.txt",headerAndQsLocations[header][question+2]);
			printf("%s\n",activeLine);

			//works out correct answer
			fgetsAtLineNum(&activeLine[0],300,"decks/PredicateLogicIntro.txt",headerAndQsLocations[header][question+2]+1);
				// Shift activeLine left 2
			for (int i = 0; i < 298 ; i ++)
				activeLine[i] = activeLine[i+2];
			
			//wait for user answer
			fgets(input, sizeof(input), stdin);

			if (!strcmp(input,activeLine))
				printf("Correct!\n");
			else{
				printf("Incorrect :(\n");
				printf("The correct answer was: %s", activeLine);
			}
			// print answer explanation
			fgetsAtLineNum(&activeLine[0],300,"decks/PredicateLogicIntro.txt",headerAndQsLocations[header][question+2]+2);
			printf(activeLine);
			drawLine(20);
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
