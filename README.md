Small CLI flashcards program to help my University studies.

## Installation

#### Linux

Run the following in your desired file destination:
```
git clone https://github.com/quaver12/BashCards.git
cd BashCards
sh install.sh
```

###### Will be adding further systems soon.

## How to Use

To begin, in your terminal simply run:
```
bcards
```

All your decks of flashcards are saved in one folder.
By default this is set the the '/decks' directory in the program files.
You can change your deck save location at any point by visiting ~/.config/bashcards/decksavelocation and changing it!

After starting bashcards, simply type the name of the deck you want to open and nail as many answers as possible!
You can also skip between different headers at any time by writing ```:``` followed by the header name whenever prompted to answer a question. (E.g. ```:5``` will jump you to the start of header 5).

Typing ```:0``` will jump to the next header and typing ```:-1``` will list all the headers you have avaliable.

**Note:** using any command with ```:``` will reset any of the progress you may have on the current header. This would only usually come if you accidentally enter one by mistake

### Making Decks

The download comes with some example 'decks' - which are just .txt files formatted a certain way.
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

#### If anyone else finds any use out of this small project, thank you so much for taking an interest!

