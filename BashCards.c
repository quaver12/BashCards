#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
//#include <time.h>


//--------------------------- Functions ---------------------------

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





//--------------------------- Function Selector ---------------------------


int main(int argc, char *argv[]){


    if (argc == 1){
        //help
	help();

    }else if (strcmp(argv[1],"add")==0){
        //add
	add();

    }else{
        printf("Unknown BashCards command. Please try again.\n");
    }

    return 0;
}
