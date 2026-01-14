#include <stdio.h>

void printHelp(){

    char helpText[] = "Usage:\n  bcards [options]\n\nTest yourself on flashcards from the command line!\n\nOptions:\n  -f,--file-path\n    Prints current deck save location\n  -s,--set-save\n    Set deck save location to current working directory\n  -S,--force-set-save\n    Same as --set-save but doesn't prompt for user comfirmation\n  -h,--help\n    Displays useful help text\n\nCommands (use when running a flashcard deck):\n  :{i}\n    Skip to header number {i}\n  :h\n    List headers and display help\n  :q\n    Ruit program\n  :n  \n    Skip to next header\n  :p\n    Move to previous header\n  :r\n    Restart current header\n";

    printf(helpText);
}

void printDeckFormatGuide(){

    char deckFormatGuide[] = "h:This is a header - headers are asked in the order they appear in the file\nq:This is a question - all questions within each header are shuffled.\na:A question must be followed by an answer - like this! Answers are specific and case sensitive.\n - The line after an answer is the explanation for the answer, no specific formatting needed - everything on this line will be printed\n\n        Also as long as a line isn't following an answer and doesn't have a line type prefix, it will be ignored.\n        This means you can edit a deck file with comments like this.\n        Note: always be sure to leave a line after an answer line for the explanation. Even if you don't want to write anything in it, just leave it empty.\n        This is because anything on the line below an answer will get interpreted as the explanation, and will be printed.\n\nh:Example questions\nq:True or False: The capital of Turkey is Constantinople.\na:False\n  -> It's Istanbul not Constantinople!\nq:What was the previous name of the city now called New York.\na:New Amsterdam\n- New York was once a Dutch settlement before being taken by the English military in 1664.\n\n\nq::   <<-- be sure to never write a question like this with another colon after the q: prefix.\n            (a user will never be able to answer it because bcards will assume the user is writing a command)\n";

    printf(deckFormatGuide);

}
