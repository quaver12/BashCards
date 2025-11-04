#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
//#include <time.h>


//--------------------------- Functions ---------------------------

// Incomplete
void help(){
    printf("help()\n");
}

// Incomplete
void add(){
    printf("add()\n");
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
