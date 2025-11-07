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
        if (character == '\n'){
            lines++;
        }
        count++;
    }

    fclose(readInFile);

    return lines;
}


// Intended to work the same way as fgets, except you can pass in the specific line number
// of a .txt file you want it to return.
void fgetsAtLineNum(char *stringLocation, int bufferSize, char *fileName, int lineNum){
	char lineString[bufferSize] = {};
	//char *variableOutsideTheFunction = stringLocation;
	FILE *activeDeckFile;
	activeDeckFile = fopen(fileName,"r");
	
	int n = 40;
	for (int i = 0; i < lineNum ; i ++){
		fgets(lineString,n, activeDeckFile);
	}
	//printf("%c\n",lineString);
	// i need to set the variable at the location stringLocation to linesString[0]
	// and then probably for lineString [1] 2 3 etc...
	for (int i = 0; i < bufferSize ; i ++){
		*(stringLocation + i) = lineString[i];
	}
	

	//return "ohno";
        fclose(activeDeckFile);
	//printf("in function %s",lineString);
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
		fprintf(activeDeckFile,"\n");
		fprintf(activeDeckFile, "%s",input);
		fprintf(activeDeckFile,"\n");
			
	//if h: not seen, prompt user for q and a
	}else{

		fprintf(activeDeckFile, "q:");
		fprintf(activeDeckFile, "%s",input);


		printf("Enter the specific answer you're looking for:\n");
		fgets(input, sizeof(input), stdin);
		fprintf(activeDeckFile, "a:");
		fprintf(activeDeckFile, "%s",input);

		printf("Enter any explanation you'd like about the answer.\n");
		fgets(input, sizeof(input), stdin);
		fprintf(activeDeckFile, "	- ");
		fprintf(activeDeckFile, "%s",input);


	}

	printf("Flashcard saved and editable in decks/PredicarteLogicIntro.txt\n");
		
        fclose(activeDeckFile);
}

// lists all downloaded decks (.txt files in decks/)
void decks(){
	printf("deck()");
}

// tests user on selected deck
void testme(){
	printf("testme()");
	//list available decks

	//ask user which one to open
	//using test example deck for time being.
	FILE *activeDeckFile;
	activeDeckFile = fopen("decks/PredicateLogicIntro.txt","r");
	
	char activeLine[100];
	int qNum = 2;
	int hNum = -1;
	int headerAndQsLocations[10][60] = {};


	// this constructs an array with the locations of each header and array as it comes across them, as well as the number of questions
	// e.g. first header in a file :[0][line of header, there are 3 questions in this header, line of question 1, line of question 2, line of question 3]
	// headerAndQsLocations[headernumber][]=[line of header, number of questions, line number of question 1, line number of q2, etc...]
	for (int lineNum = 0; lineNum< countFileLines("decks/PredicateLogicIntro.txt"); lineNum++){
		//printf("in the loop\n");	
		int n = 40;
		fgets(activeLine,n, activeDeckFile);
		//printf("%s",activeLine);
		if (activeLine[0] == 'h' && activeLine[1] == ':'){
			printf("header found on line %d\n",lineNum);
			// then it's a header
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
			//printf("%d\n",headerAndQsLocations[hNum][qNum]);
		}
	}
	

	srand(time(NULL));
	
	//go one header at a time
	for (int i = 0; i < hNum ;i++){
		printf("looking at header %d\n",hNum);
		
		//shuffle array (but only from [i][1] onwards)
		
		for (int x = 2; x < headerAndQsLocations[i][1] + 2 ; x++){
			int temp = 0;
			int transferTo = 0;
			temp = headerAndQsLocations[i][x];
			transferTo = rand() % (headerAndQsLocations[i][1]) + 2;
			//printf("looking at digit %d of the array\n",x);
			//printf(" temp is %d\n", temp);
			//printf(" randomly generated address 'transferTo' is : %d\n", transferTo);
			//printf(" value at the location of transferTo is %d\n", headerAndQsLocations[i][transferTo]);
			//printf("values before switch: %d %d\n", headerAndQsLocations[i][x],  headerAndQsLocations[i][transferTo]);
			headerAndQsLocations[i][x] = headerAndQsLocations[i][transferTo];
			headerAndQsLocations[i][transferTo] = temp; 
			//printf("values after switch: %d %d\n", headerAndQsLocations[i][x],  headerAndQsLocations[i][transferTo]);
			//printf("%d\n",headerAndQsLocations[i][x]);
			for (int o = 0 ; o < 20; o ++){
				printf("%d,",headerAndQsLocations[i][o]);
			}
			printf("\n");
		}
		//now the questions are shuffled
		//ask question for each question in the array
		for (int x = 2; x < headerAndQsLocations[i][1];i++){

		




		}


	}
	char string[60] = "eeeeeeeeee";
	fgetsAtLineNum(&string[0],50,"decks/PredicateLogicIntro.txt",4);
	printf("%s",string);

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
