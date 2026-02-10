A small CLI flashcards program to help my University studies.

## Installation

#### Linux

To install, run the following:
```
git clone https://github.com/quaver12/BashCards.git
cd BashCards
make && sudo make install
```

## How to Use

To begin, run:
```
bcards
```

All your decks of flashcards are saved in one folder.
By default this is set to the '/decks' directory in the program files.
You can change this at any point by running ```bcards --set-save``` in the directory you'd like to be you new deck save location.
You can also visit ~/.config/bashcards/decksavelocation to change this manually.

When starting bashcards for the first time, select the TUTORIAL example deck to begin with.

You can skip between different headers at any time by writing ```:``` followed by the header number whenever prompted to answer a question. (E.g. ```:5``` will jump you to the start of header 5).

This is called a command, other commands include:

 - ```:n``` - jump to the next header.
 - ```:q``` - quit
 - ```:h``` - list all headers and help
 - ```:r``` - restart current header
 - ```:p``` - got to previous header

### Making Decks

Bashcards 'decks' are just .txt files formatted a certain way.
You can make your own by formatting headers, questions and answers the following way:
```
h:This is a header - headers are asked in the order they appear in the file
q:This is a question - all questions within each header are shuffled.
a:This is the answer to the question above! Answers are specific and case sensitive at the minute.
 - The line after an answer is the explanation for the answer, no specific formatting needed - everything on this line will be printed
        Also as long as a line isn't following an answer and doesn't have a line type prefix, it will be ignored.
        This means you can edit a file with comments like this.
h: Example questions
q:True or False: The capital of Turkey is Constantinople.
a:False
  -> It's Istanbul not Constantinople!
q:What was the previous name of the city now called New York.
a:New Amsterdam
- New York was once a Dutch settlement before being taken by the English military in 1664.


q::   <<-- be sure to never write a question like this with another colon after the q: prefix.
            (a user will never be able to answer it without writing a command)
```

If stuck, man pages are provided, and feel free to consult ```bcards -h``` for help!

#### If anyone else finds any use out of this small project, thank you so much for taking an interest and I hope you find it useful!

