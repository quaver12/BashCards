#ifndef BCDECK_H_
#define BCDECK_H_

// A 'subdeck' has one header (h:) and then all the questions/answers/explanations until the next header. Questions are randomised within a subdeck.
struct subdeckFormat{
    char header[BUFFSIZE];
    int questionAmount;
    char question[BUFFSIZE][BUFFSIZE]; // array of strings
    char answer[BUFFSIZE][BUFFSIZE];
    char explanation[BUFFSIZE][BUFFSIZE];
};


// ------------------------------------------ Program Specific Functions -----------------------------------------
char *findDecks(char *deckFilesDirectory); // locates the file location of the downloaded decks. Dictated by .config file.
char **listAvailableDecks(); // Prints the the downloaded decks in the default deck location to the shell. MEMORY ALLOCATED DURING FUNCTION - BE SURE TO FREE THIS
int countHeaders(FILE *inFile); // counts the number of headers/subdecks in file. resets fgets count of file at end.
struct subdeckFormat *buildSubdecks(struct subdeckFormat *subdeck, FILE *inFile); // returns an array of each 'subdeck' in a file (containing 1 header, the number of questions, and all questions in that header)
struct subdeckFormat *shuffleSubdecks(struct subdeckFormat *subdeck, int headersAmount); // Shuffles each of the questions within each subdeck
void listHeaders (struct subdeckFormat *subdeck, int headersAmount); // Lists the headers of the active subdeck.
int askQuestions(struct subdeckFormat *subdeck, int subdeckToAsk); // asks questions of one specific subdeck (passed into function). Returns 0 if worked normally. Returns different values if user has used a 'command';
int setSaveToWorkingDir(); // does what it says on the tin. -1 for fail 0 for success.
/

#endif
